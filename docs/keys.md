# Keys

`Crypt` encrypts with **key bytes**. It never hashes or checks them; the
key option only decides how `file-crypt` turns its argument into bytes.
Encryption and decryption must produce the same bytes, otherwise
decryption fails with exit `1`.

## The key forms

| Option | Key bytes | Length |
|--------|-----------|--------|
| `--key-sha512 <text>` | SHA512 of `text` | 64 |
| `--key <text>` | `text` itself | length of `text` |
| `--key-hex <hex>` | the bytes written in hex | half the digits |
| `--key-read <key>` | the content of `<key>` (encrypt or decrypt) | file size |
| `--gen-key-encrypt <key>` | new key, saved to `<key>` | 64 |
| `--gen-key-decrypt <key>` | the content of `<key>` | file size |
| `--key-sha512-write <text> <key>` | SHA512 of `text`, saved to `<key>` | 64 |
| `--gen-key-sha512-write <text> <key>` | SHA512 of `text`, saved to `<key>` (no encryption) | 64 |

## Which forms open each other's files

The same bytes open the same files, whatever option produced them:

| Encrypted with | Opens with |
|----------------|------------|
| `--key-sha512 P` | `--key-sha512 P`; `--gen-key-decrypt` or `--decrypt --key-read` with a key file from `--gen-key-sha512-write P` or `--key-sha512-write P`; `--key-hex` with the 128 hex digits of SHA512(P) |
| `--key P` | `--key P`; `--key-hex` with the hex of the bytes of `P` — **not** `--key-sha512 P` |
| `--key-hex H` | `--key-hex H`; `--key` with the same bytes; `--gen-key-decrypt` or `--decrypt --key-read` with a file holding those bytes |
| `--gen-key-encrypt K` | `--gen-key-decrypt K` or `--decrypt --key-read K`; `--key-hex` with the hex of `K` |

The most common mistake is mixing `--key` and `--key-sha512`: a file
encrypted with `--key secret` (6 raw bytes) is not opened by
`--key-sha512 secret` (64 hashed bytes), and the other way round.

## Choosing a key

| Situation | Use | Why |
|-----------|-----|-----|
| A person types a password | `--key-sha512` | always 64 bytes, compatible with key files from `--gen-key-sha512-write` and with XYO code that uses `SHA512::hashToU8(password, key)` |
| Automated jobs, backups | a key file from `--gen-key-encrypt`, decrypted with `--gen-key-decrypt` | full 64 byte key, nothing secret on the command line |
| The key comes from another system (a KMS, a KDF, a hardware token) | `--key-hex` with its 64 bytes | bytes used exactly as given |
| Compatibility with old files that used a raw string | `--key` | otherwise avoid: short keys are weak |

### Password strength

`--key-sha512` is a single SHA512 with no salt and no work factor. Anyone
who has the encrypted file can test passwords very fast (billions per
second on a GPU). A password is only as strong as it is long and random: use
a passphrase of several random words, or a key file.

If you need a slow password hash (Argon2, scrypt, PBKDF2), compute it with
another tool and pass the 64 byte result with `--key-hex`.

### How `--gen-key-encrypt` makes the key

The new key is SHA512 of the input file content followed by the current
time in milliseconds. It is unique per file and per moment, but it is
**not** taken from the operating system's random generator: its
unpredictability comes from the content of the file. For a file whose
content could be guessed (a short, standard or templated file), an
attacker who guesses the content and the time can confirm the guess. For
such files generate the key elsewhere (64 random bytes) and use
`--key-hex`, or use `--key-sha512` with a strong passphrase.

The seed stored in every encrypted file *is* drawn from the operating
system random generator, so the encrypted output differs on every run in
all modes.

## Key files

- A key file is binary: exactly the key bytes, no new line, no encoding.
  The files written by `file-crypt` are 64 bytes.
- `--key-read` and `--gen-key-decrypt` use the whole file, so a key file edited in a text
  editor (adding a final new line) is a different key.
- Protect key files like passwords: restrict their permissions, store them
  apart from the encrypted files, back them up. A lost key file means the
  data is lost; there is no recovery.
- Convert a key file to hex (bash): `od -An -tx1 data.key | tr -d ' \n'`.
  Back from hex: `xxd -r -p data.hex data.key`.
