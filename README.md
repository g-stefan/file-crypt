# File Crypt

Encrypt/Decrypt file with key
- Encrypts and signs a file in one step; decryption gives back exactly the
original file or fails (wrong key, modified or truncated file), without
writing a partial output.
- Keys from a password (`--key-sha512`), raw string (`--key`), hex bytes
(`--key-hex`) or 64 byte key files (`--key-read`, `--gen-key-encrypt`, `--gen-key-decrypt`,
`--key-sha512-write`, `--gen-key-sha512-write`).
- `--extract-integrity` saves the signature of an encrypted file to
recognize a known version later.

Built on `xyo-system` and `xyo-cryptography`; the file format is `Crypt` of
`xyo-cryptography`, shared with the `quantum-script` `Crypt` extension.

```bash
file-crypt --encrypt --key-sha512 "pass phrase" notes.txt notes.txt.crypt
```

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, first encryption, key files, scripting
- [Command line](docs/command-line.md) - every option, parsing, exit codes, examples
- [Keys](docs/keys.md) - key forms, which open each other's files, choosing a key
- [File format](docs/file-format.md) - layout, sizes, signature, security notes

A Claude Code skill for this project is in
[.claude/skills/file-crypt](.claude/skills/file-crypt/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
