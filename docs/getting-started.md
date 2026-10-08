# Getting started

## 1. Build and install

`file-crypt` is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool of all XYO C++ projects. `xyo-platform`, `xyo-managed-memory`,
`xyo-data-structures`, `xyo-multithreading`, `xyo-encoding`, `xyo-system` and
`xyo-cryptography` must be installed to the SDK first. From the repository
root:

```bash
fabricare make       # build into output/
fabricare install    # copy output/bin to ~/.fabricare/<platform>/bin
fabricare clean      # remove output/ and temp/
```

`fabricare.json` defines a single project, `file-crypt`, built as an
executable (`file-crypt.exe` on Windows, `file-crypt` on Linux). After
`fabricare install` it is in `~/.fabricare/<platform>/bin`, which is on the
`PATH` of an XYO development machine. Prebuilt archives for Windows and
Ubuntu are published on the
[releases page](https://github.com/g-stefan/file-crypt/releases).

Check it — running it without arguments prints the options and exits
with `1`:

```bash
file-crypt
```

```
file-crypt - Encrypt/Decrypt file with key
version 5.0.0 build 10 [2026-10-03 22:40:53]
Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>

options:
    ...
```

## 2. Encrypt and decrypt with a password

```bash
file-crypt --encrypt --key-sha512 "correct horse battery staple" notes.txt notes.txt.crypt
file-crypt --decrypt --key-sha512 "correct horse battery staple" notes.txt.crypt notes.txt
```

- The first file name is the input, the second the output. The output is
  overwritten if it exists.
- `--key-sha512` turns the password into a 64 byte key with SHA512. This is
  the form to use for passwords; see [Keys](keys.md) for the others.
- The encrypted file is 137 to 200 bytes larger than the original
  ([sizes](file-format.md#size)).
- Encrypting the same file twice gives different output: every encryption
  uses a new random seed.

A wrong password, or an encrypted file that was modified or truncated,
makes `--decrypt` exit with `1` **without writing the output file**:

```bash
file-crypt --decrypt --key-sha512 "wrong" notes.txt.crypt out.txt
echo $?        # 1, and out.txt does not exist
```

`file-crypt` prints nothing in that case; only argument errors print
`Error: Invalid parameters`. Always test the exit code.

## 3. Use a key file instead of a password

A key file is a file whose bytes are the key — normally 64 bytes.

Generate a new key while encrypting:

```bash
file-crypt --gen-key-encrypt backup.key backup.tar backup.tar.crypt
file-crypt --gen-key-decrypt backup.key backup.tar.crypt backup.tar
```

`--gen-key-encrypt` writes a new 64 byte key to `backup.key`, then encrypts
with it. Keep the key file safe and separate from the encrypted file;
without it the data cannot be recovered.

Or make a key file from a password, so scripts do not need the password on
the command line:

```bash
file-crypt --gen-key-sha512-write "correct horse battery staple" notes.key
file-crypt --gen-key-decrypt notes.key notes.txt.crypt notes.txt
```

`notes.key` contains SHA512 of the password, so it opens files encrypted
with `--key-sha512 "correct horse battery staple"`, and the other way round.
Both steps can be done at once while encrypting:

```bash
file-crypt --encrypt --key-sha512-write "correct horse battery staple" notes.key notes.txt notes.txt.crypt
```

To use any existing key file, in both directions, use `--key-read`:

```bash
file-crypt --encrypt --key-read notes.key notes.txt notes.txt.crypt
file-crypt --decrypt --key-read notes.key notes.txt.crypt notes.txt
```

`--gen-key-decrypt <key file> <input> <output>` decrypts the same way
(despite the name, it does not generate anything when decrypting).

## 4. Remember a known version of an encrypted file

```bash
file-crypt --extract-integrity notes.txt.crypt notes.txt.sig
```

writes the 64 byte signature of the encrypted file to `notes.txt.sig`. No
key is needed. The signature changes whenever the file is encrypted again,
so comparing a saved signature with a fresh extraction tells whether you
still have the same encrypted file. See
[the signature](file-format.md#the-signature-and---extract-integrity) for
what this does and does not prove.

## 5. From scripts

Bash:

```bash
if ! file-crypt --decrypt --key-sha512 "$PASSWORD" secrets.json.crypt secrets.json; then
	echo "cannot decrypt secrets.json.crypt: wrong password or damaged file" >&2
	exit 1
fi
```

Windows `cmd`:

```bat
file-crypt --encrypt --key-sha512 "%PASSWORD%" secrets.json secrets.json.crypt
if errorlevel 1 exit /b 1
```

fabricare script (`fabricare/*.js`):

```js
exitIf(Shell.system("file-crypt --decrypt --key-sha512 \"" + password + "\" secrets.json.crypt secrets.json"));
```

A password on the command line can be seen by other users of the machine
(process list) and may end up in the shell history. On shared machines
prefer a key file with restricted permissions and `--gen-key-decrypt`.
