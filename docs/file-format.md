# File format

An encrypted file is the output of `Crypt::encrypt` of
[xyo-cryptography](https://github.com/g-stefan/xyo-cryptography), written
as is. The full description and the reasoning behind it are in
`docs/crypt.md` of that repository; this page summarizes what matters for
users of `file-crypt`.

## Layout

```
offset  size     content
0       64       seed: different for every encryption, public
64      64       signature: keyed SHA512 of everything after it
128     8        length of the original file, encrypted
136     m * 64   the file, encrypted, padded to 64 bytes;  m = n / 64 + 1
```

There is no header, magic number or version field: an encrypted file
looks like random bytes. Name the files so you know what they are
(`.crypt` is the convention).

## Size

For an input of `n` bytes the output is

```
136 + (n / 64 + 1) * 64 bytes        // integer division
```

| Input | Output |
|-------|--------|
| 0 .. 63 bytes | 200 bytes |
| 64 .. 127 bytes | 264 bytes |
| 1 MiB | 1 MiB + 200 bytes |

The overhead is always between 137 and 200 bytes; the original length is
visible only to within 64 bytes. An empty file encrypts to 200 bytes and
decrypts back to an empty file.

## What decryption checks

Before anything is decrypted, `--decrypt` / `--gen-key-decrypt`:

1. rejects input shorter than 136 bytes,
2. recomputes the signature with the key and compares it with bytes 64..127,
3. checks that the decrypted length fits the input.

Any failure — wrong key, one changed bit anywhere, a truncated or extended
file, a file that is not `Crypt` data — gives exit `1` and **no output
file**. A successful decryption is therefore both *authentic* (made by
someone with the key) and *complete*.

## The signature and `--extract-integrity`

Bytes 64..127 are the file's signature. `--extract-integrity` copies them
to a separate 64 byte file without needing the key:

```bash
file-crypt --extract-integrity contract.pdf.crypt contract.pdf.sig
```

Uses:

- **Recognize a known version.** Each encryption has a new seed and
  therefore a new signature, even for the same content and key. A saved
  signature identifies one specific encrypted file: compare it with a
  fresh extraction to see whether a file is still the version you
  recorded, or was replaced by another encryption.
- **With the key, from code**, `Crypt::checkIntegrity(key, 64, data,
  size, signature)` verifies both that the file is intact for that key and
  that its signature equals the saved one, without decrypting.

Limits: comparing signatures *without* the key only compares those 64
bytes. Someone could keep them and change the rest of the file; that is
detected only when the file is decrypted (or checked with
`checkIntegrity`) using the key. The signature reveals nothing about the
content.

## Compatibility

- The same format is read and written by `Crypt::encrypt` / `decrypt` /
  `encryptFile` / `decryptFile` in C++ and by the `quantum-script` `Crypt`
  extension. They interoperate as long as they use the same key bytes;
  XYO code conventionally passes `SHA512::hashToU8(password)`, which is
  what `--key-sha512` does.
- Files encrypted on Windows decrypt on Linux and the other way round; all
  numbers in the format are little endian.

## Security notes

- `Crypt` is a custom construction — a SHA512 based stream cipher with a
  SHA512 based signature — not a standard authenticated cipher. It is
  tested, but not externally reviewed. For data that must meet a standard
  or a compliance rule, use a standard tool (age, GnuPG, OpenSSL with
  AES-GCM).
- Password keys (`--key-sha512`) are one fast SHA512: see
  [password strength](keys.md#password-strength).
- `--gen-key-encrypt` derives the key from the file content and the time,
  not from the system random generator: see
  [how the key is made](keys.md#how---gen-key-encrypt-makes-the-key).
- The whole file is read into memory, encrypted, then written: large files
  need as much free memory as about twice their size.
- `file-crypt` does not delete or overwrite the plain text input. Remove it
  yourself if it must not stay on disk (and remember that deleted files can
  often be recovered from the disk).
- Passwords on the command line can be seen in the process list and the
  shell history.
