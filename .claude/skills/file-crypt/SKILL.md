---
name: file-crypt
description: >-
  How to use file-crypt, the XYO command line tool (namespace
  XYO::FileCrypt) that encrypts / decrypts a file with a key using Crypt of
  xyo-cryptography (SHA512 based stream cipher + keyed SHA512 signature,
  authenticated: wrong key or any change -> exit 1 and no output file): its
  options --encrypt / --decrypt with --key-sha512 (SHA512 of a password,
  the recommended password form), --key (raw string bytes, NOT hashed),
  --key-hex (hex bytes); the key file options --key-read (encrypt or
  decrypt with an existing key file), --gen-key-encrypt, --gen-key-decrypt,
  --key-sha512-write, --gen-key-sha512-write; --extract-integrity
  (bytes 64..127, the signature); --license / --help / --usage / --version;
  argument parsing, exit codes, "Error: Invalid parameters" / "Error: Empty
  key file", which key forms open each other's files, the encrypted file
  layout (seed, signature, length, padded blocks, size
  136 + (n / 64 + 1) * 64) and security limits. Use when running or scripting file-crypt (bash, cmd,
  fabricare Shell.system), choosing or converting keys and key files,
  debugging a file that will not decrypt, interoperating with
  Crypt::encryptFile / decryptFile / checkIntegrity or the quantum-script
  Crypt extension, or when working inside the file-crypt repository.
---

# file-crypt

Command line front end of `Crypt` from `xyo-cryptography`, built on
`xyo-system` (see the `fabricare` and `xyo-system` skills for building and
the application layer). Purpose: **encrypt and sign a file with a key in
one command; decrypt gives back exactly the original or fails.**

Full documentation: `docs/` in the file-crypt repository
(`X:\Storage\XYO\Gitea\CPP\file-crypt\docs` on this machine): README,
getting-started, **command-line** (every option, parsing, exit codes,
examples), **keys** (forms, compatibility, choosing), **file-format**
(layout, sizes, signature, security notes). The cipher itself:
`xyo-cryptography/docs/crypt.md`. The whole tool is
`source/XYO/FileCrypt/Application.cpp`; read it when in doubt.

## Commands

```bash
file-crypt --encrypt --key-sha512 "pass phrase" in.txt in.txt.crypt   # password (recommended form)
file-crypt --decrypt --key-sha512 "pass phrase" in.txt.crypt in.txt
file-crypt --gen-key-encrypt in.key in.txt in.txt.crypt               # new 64 byte key file
file-crypt --gen-key-decrypt in.key in.txt.crypt in.txt               # decrypt with ANY key file
file-crypt --encrypt --key-read in.key in.txt in.txt.crypt            # existing key file, encrypt
file-crypt --decrypt --key-read in.key in.txt.crypt in.txt            # existing key file, decrypt
file-crypt --gen-key-sha512-write "pass phrase" pass.key              # password -> key file only
file-crypt --encrypt --key-sha512-write "pass phrase" pass.key in out # encrypt + save key file
file-crypt --encrypt --key-hex 00ff... in out                         # key bytes in hex
file-crypt --encrypt --key text in out                                # raw bytes, NOT hashed
file-crypt --extract-integrity in.txt.crypt in.txt.sig                # signature, no key needed
```

| Option | Key bytes |
|--------|-----------|
| `--key-sha512 <text>` | SHA512(text), 64 |
| `--key <text>` | text as given (shell bytes) |
| `--key-hex <hex>` | hex decoded; case insensitive, non hex digit -> 0, odd last digit dropped, unchecked |
| `--gen-key-encrypt <key>` | new key = SHA512(file content + time ms), written to `<key>` (64) |
| `--key-read <key>` | whole content of `<key>`, any length (empty -> `Error: Empty key file`); with `--encrypt` or `--decrypt` |
| `--gen-key-decrypt <key>` | whole content of `<key>`, any length (decrypt only) |
| `--key-sha512-write <text> <key>` | SHA512(text), also written to `<key>`; with `--encrypt` or `--decrypt` |
| `--gen-key-sha512-write <text> <key>` | writes SHA512(text) to `<key>`, nothing else, no input/output |

Same bytes open the same files, whatever produced them:
`--key-sha512 P` == key file from `--gen-key-sha512-write P` ==
`--key-hex <hex of SHA512(P)>`. `--key P` is **not** `--key-sha512 P` —
the most common reason a file "will not decrypt".

## Parsing rules

- `--x` = option; other arguments = files: 1st input, 2nd output, rest
  ignored. Order is free, but option values follow their option directly
  (they are consumed blindly, even if they start with `--`).
- Unknown options are ignored silently (a typo shows up only as
  `Error: Invalid parameters` because no action was chosen).
- `--encrypt` + `--decrypt` -> encrypt. Several key options: last value,
  form by precedence sha512 > key > hex, `--key-read` replaces all.
- Exit 0 = success (also an info option alone); 1 = any failure or no
  arguments. `Error: Invalid parameters` (on stdout) = missing value /
  input / output / action, empty key value; `Error: Empty key file` =
  empty `--key-read` file. Read / write errors, wrong key and
  damaged files exit 1 **silently** — always check the exit code.
- Decryption writes the output only after the signature and length check:
  never a partial file. Key files are written before encrypting, so a
  failed encryption can leave a key file.
- No arguments: prints usage + version, exit 1. `--help` / `--usage`:
  usage + version; `--version`: version line; `--license`: MIT text
  (exit 0 when alone, otherwise parsing continues).
- Key file to hex (bash): `od -An -tx1 <key> | tr -d ' \n'`.

## Format and limits

```
0    64      seed (public, new per encryption: SHA512 of key, data, time, OS random)
64   64      signature = keyed SHA512 of the rest   <- --extract-integrity
128  8       length, encrypted, little endian
136  m*64    data, encrypted, padded, m = n / 64 + 1
size = 136 + (n / 64 + 1) * 64   (0..63 bytes -> 200)
```

- No header / magic: name files `.crypt`. Cross platform.
- Same output on every run is impossible (new seed); the signature changes
  too, so a saved signature identifies one specific encrypted file.
  Comparing signatures without the key proves only those 64 bytes;
  `Crypt::checkIntegrity(key, 64, data, size, sig)` checks with the key.
- Custom, not externally reviewed construction; `--key-sha512` is one fast
  unsalted SHA512 (use long passphrases or key files);
  `--gen-key-encrypt` keys come from file content + time, not OS random
  (weak for guessable files: use `--key-hex` with 64 random bytes);
  whole file in memory; plain text input is left on disk; passwords on the
  command line show in the process list.

## Scripting

```bash
file-crypt --decrypt --key-sha512 "$PASS" s.json.crypt s.json || exit 1
```

```bat
file-crypt --encrypt --key-sha512 "%PASS%" s.json s.json.crypt
if errorlevel 1 exit /b 1
```

```js
// fabricare script
exitIf(Shell.system("file-crypt --gen-key-decrypt data.key data.crypt data.bin"));
```

From C++ use the library directly instead of spawning the tool:
`SHA512::hashToU8(password, key); Crypt::encryptFile(key, 64, in, out);`
(`#include <XYO/Cryptography.hpp>`, dependency `xyo-cryptography`).

## In this repository

- One project in `fabricare.json`: `file-crypt`, `"make": "exe"`,
  dependencies `xyo-system`, `xyo-cryptography`. `fabricare make` /
  `install` / `clean`; `fabricare version` updates `version.json` and
  `Version.rh`.
- All behavior is in `Application::main`; defining `XYO_FILECRYPT_LIBRARY`
  drops `XYO_APPLICATION_MAIN` so the class can be embedded.
- Keep `showUsage()`, `docs/command-line.md` and this skill in step when
  options change. `.reuse/dep5` covers `docs/*` (MIT) and `.claude/*`
  (Unlicense).
