# Command line

```
file-crypt [options] [input] [output]
```

## Options

### Encrypt / decrypt with a key given on the command line

| Command | Key used |
|---------|----------|
| `--encrypt --key-sha512 <text> <input> <output>` | SHA512(`text`), 64 bytes |
| `--decrypt --key-sha512 <text> <input> <output>` | SHA512(`text`), 64 bytes |
| `--encrypt --key <text> <input> <output>` | the bytes of `text` as given (length of `text`) |
| `--decrypt --key <text> <input> <output>` | the bytes of `text` as given |
| `--encrypt --key-hex <hex> <input> <output>` | the bytes written in `hex` |
| `--decrypt --key-hex <hex> <input> <output>` | the bytes written in `hex` |

### Key files

| Command | What it does |
|---------|--------------|
| `--gen-key-encrypt <key> <input> <output>` | creates a 64 byte key, writes it to `<key>`, encrypts `<input>` into `<output>` with it |
| `--gen-key-decrypt <key> <input> <output>` | reads the key from `<key>` (all its bytes) and decrypts `<input>` into `<output>` |
| `--encrypt --key-sha512-write <text> <key> <input> <output>` | encrypts with SHA512(`text`) and writes those 64 bytes to `<key>` |
| `--gen-key-sha512-write <text> <key>` | only writes SHA512(`text`) to `<key>`; no input or output |
| `--encrypt --key-read <key> <input> <output>` | encrypts with the key read from `<key>` (all its bytes) |
| `--decrypt --key-read <key> <input> <output>` | decrypts with the key read from `<key>`; same as `--gen-key-decrypt <key>` |

`--key-sha512-write` also works with `--decrypt` (decrypt with
SHA512(`text`) and write the key file). An empty key file is rejected with
`Error: Empty key file`.

### Integrity

| Command | What it does |
|---------|--------------|
| `--extract-integrity <file> <integrity>` | writes bytes 64..127 of the encrypted `<file>` (its signature) to `<integrity>`; no key needed; fails if `<file>` is shorter than 128 bytes |

See [the signature](file-format.md#the-signature-and---extract-integrity).

### Information

| Option | What it does |
|--------|--------------|
| *(no arguments)* | prints the version, copyright and the list of options; exit `1` |
| `--help`, `--usage` | prints the version, copyright and the list of options |
| `--version` | prints the version, build number and build date |
| `--license` | prints the MIT license |

When one of the information options is the only argument, `file-crypt`
exits with `0` after printing. With other arguments it prints and then goes
on with the rest of the command line.

## How the command line is read

- Arguments are read left to right. An argument starting with `--` is an
  option; any other argument is a file name: the first one is `<input>`,
  the second `<output>`, further ones are ignored.
- Options that take values consume the next one or two arguments,
  whatever they look like: `--key-sha512 --x` uses `--x` as the password.
  A value starting with `-` is fine; quote values with spaces.
- The order of options and file names does not matter, except that the
  key file of `--gen-key-encrypt` / `--gen-key-decrypt` and the values of
  `--key*` options must follow their option directly:

  ```bash
  file-crypt notes.txt notes.txt.crypt --key-sha512 secret --encrypt   # same as the usual order
  ```

- Unknown options are silently ignored (`--encrpyt` is a typo you only
  notice by the `Error: Invalid parameters` that follows, because no action
  was selected).
- If both `--encrypt` and `--decrypt` are given, `--encrypt` wins.
- If several key options are given, the value of the last one is used,
  and the form is chosen by precedence, not by order: `--key-sha512` over
  `--key` over `--key-hex`; a `--key-read` file replaces all of them. Give
  exactly one key option.

### Key values

- `--key <text>`: the bytes of the argument as the shell passes them
  (UTF-8 on Linux, the active code page on Windows). No hashing.
- `--key-hex <hex>`: two hex digits per byte, upper or lower case, no
  separators or `0x`. A character that is not a hex digit counts as `0`, a
  trailing odd digit is dropped. Nothing is checked, so verify the value.
- `--key-sha512 <text>`: SHA512 of the bytes of the argument, always 64
  bytes.
- Key files are used byte for byte, whatever their length. Files written by
  `file-crypt` are always 64 bytes.

## Exit codes and messages

| Exit | Meaning |
|------|---------|
| `0` | success; also `--license` / `--help` / `--usage` / `--version` used alone |
| `1` | any failure, or no arguments |

| Output | Cause |
|--------|-------|
| `Error: Invalid parameters` | a value is missing after an option, `<input>` or `<output>` is missing, no action selected, or an empty key value |
| `Error: Empty key file` | the `--key-read` file is empty |
| *(nothing)* | the input or key file cannot be read, the output cannot be written, a wrong key, a damaged or truncated encrypted file, or a file shorter than 128 bytes for `--extract-integrity` |

Decryption failures never leave a partial output file: the output is only
written after the signature and the length were verified. Key files are
written **before** encrypting (`--gen-key-encrypt` after reading the
input, `--key-sha512-write` before reading it), so a failed encryption can
still leave a new key file behind.

## Examples

```bash
# password
file-crypt --encrypt --key-sha512 "pass phrase" report.pdf report.pdf.crypt
file-crypt --decrypt --key-sha512 "pass phrase" report.pdf.crypt report.pdf

# new random key file
file-crypt --gen-key-encrypt report.key report.pdf report.pdf.crypt
file-crypt --gen-key-decrypt report.key report.pdf.crypt report.pdf

# password -> key file, decrypt with the key file
file-crypt --gen-key-sha512-write "pass phrase" report.key
file-crypt --gen-key-decrypt report.key report.pdf.crypt report.pdf

# existing key file, both directions
file-crypt --encrypt --key-read report.key report.pdf report.pdf.crypt
file-crypt --decrypt --key-read report.key report.pdf.crypt report.pdf

# key in hex, here the bytes of an existing key file (bash)
file-crypt --encrypt --key-hex "$(od -An -tx1 report.key | tr -d ' \n')" in.bin in.bin.crypt

# signature of an encrypted file
file-crypt --extract-integrity report.pdf.crypt report.pdf.sig
```
