# File Crypt (file-crypt) — Documentation

`file-crypt` is a small command line tool that encrypts and decrypts a file
with a key. It is the command line front end of `Crypt` from
[xyo-cryptography](https://github.com/g-stefan/xyo-cryptography):

- **Encrypt and sign in one step.** Every encrypted file carries a keyed
  SHA512 signature. Decryption either gives back exactly the original
  bytes or fails; a wrong key or a single changed byte is detected before
  anything is written.
- **Several ways to give the key.** A password hashed with SHA512
  (`--key-sha512`), the raw bytes of a string (`--key`), bytes written in
  hex (`--key-hex`), or a 64 byte key file (`--key-read`, `--gen-key-encrypt`,
  `--gen-key-decrypt`, `--key-sha512-write`, `--gen-key-sha512-write`).
- **Integrity tracking.** `--extract-integrity` copies the 64 byte
  signature out of an encrypted file, so a known version of the file can
  be recognized later.
- **Same format everywhere.** Files written by `file-crypt` are the
  `Crypt` format of xyo-cryptography, also used by the `quantum-script`
  `Crypt` extension: anything that has the same 64 key bytes can open them.

```
file-crypt                 <-- this project: command line tool
xyo-cryptography           (Crypt, SHA512)
xyo-system                 (IApplication, Shell, Buffer, DateTime)
xyo-encoding               (String, UConvert)
xyo-multithreading
xyo-data-structures
xyo-managed-memory
xyo-platform
```

## Why use it

- **Protect a file at rest** — backups, configuration with secrets, files
  sent over an untrusted channel — with a single command and no external
  dependency beyond the XYO stack.
- **Tamper detection for free.** Because the signature is checked first,
  `--decrypt` failing means *wrong key or damaged file*; you never get a
  silently corrupted output.
- **Scriptable.** No prompts, exit code `0` / `1`, works the same on
  Windows and Linux; easy to call from `fabricare` scripts, CI jobs or
  shell scripts.
- **Interoperable with XYO code.** A C++ program using
  `Crypt::decryptFile` or a `quantum-script` using the `Crypt` extension can
  open the output, as long as it derives the same 64 key bytes.

## What it is not

- It is a custom construction (SHA512 based stream cipher plus signature),
  not a standard, externally reviewed scheme such as AES-GCM or age. Read
  the [security notes](file-format.md#security-notes) before relying on it.
- A password given with `--key-sha512` is hashed once, with no salt and no
  work factor. Use long passphrases or random key files.
- The whole file is processed in memory: it suits files that fit in RAM.

## Concepts at a glance

| Need | Command | Notes |
|------|---------|-------|
| Encrypt with a password | `file-crypt --encrypt --key-sha512 "pass phrase" in.txt in.txt.crypt` | key = SHA512(password), 64 bytes — the recommended password form |
| Decrypt with a password | `file-crypt --decrypt --key-sha512 "pass phrase" in.txt.crypt in.txt` | exit `1` and no output on a wrong password |
| Encrypt with a new key file | `file-crypt --gen-key-encrypt in.key in.txt in.txt.crypt` | writes a 64 byte key to `in.key` |
| Decrypt with a key file | `file-crypt --decrypt --key-read in.key in.txt.crypt in.txt` | any key file, also one from `--gen-key-sha512-write`; same as `--gen-key-decrypt in.key ...` |
| Encrypt with an existing key file | `file-crypt --encrypt --key-read in.key in.txt in.txt.crypt` | |
| Turn a password into a key file | `file-crypt --gen-key-sha512-write "pass phrase" pass.key` | the file opens what `--key-sha512` encrypted |
| Encrypt with a password and keep the key file | `file-crypt --encrypt --key-sha512-write "pass phrase" pass.key in.txt in.txt.crypt` | |
| Key as hex bytes | `--key-hex 0011aabb...` | bytes used as given |
| Key as raw string bytes | `--key text` | **not** hashed, not compatible with `--key-sha512` |
| Save the signature of an encrypted file | `file-crypt --extract-integrity in.txt.crypt in.txt.sig` | bytes 64..127, no key needed |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, first encryption and decryption, key files, scripting |
| [Command line](command-line.md) | Every option, how arguments are parsed, exit codes and messages, examples |
| [Keys](keys.md) | The key modes, which ones open each other's files, choosing a key, key files |
| [File format](file-format.md) | Layout of an encrypted file, sizes, the signature and `--extract-integrity`, security notes |

The encryption itself is implemented and documented in
[xyo-cryptography](https://github.com/g-stefan/xyo-cryptography)
(`docs/crypt.md`); this repository only contains the command line tool.

## Source map

```
source/XYO/FileCrypt/
    Application.hpp / .cpp         command line parsing and every action (the whole tool)
    Dependency.hpp                 includes <XYO/System.hpp> and <XYO/Cryptography.hpp>
    Application.rc / .rh / .ico / .manifest   Windows resources of file-crypt.exe
    Copyright / License / Version  application metadata (--license, --version)
    Version.Template.rh            template for Version.rh, filled by fabricare version
fabricare.json                     one project: file-crypt, "make": "exe"
fabricare/release-install.js       copies release archives to the release folder
version.json                       version and build number
```

`Application.cpp` ends with `XYO_APPLICATION_MAIN(...)` unless
`XYO_FILECRYPT_LIBRARY` is defined, so the `Application` class can be
compiled into another program and called as
`XYO::FileCrypt::Application app; app.main(argc, argv);`.

## AI assistant skill

A Claude Code skill describing how to use `file-crypt` lives in
[`.claude/skills/file-crypt/`](../.claude/skills/file-crypt/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in other projects that call
`file-crypt`.
