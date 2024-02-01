# MyChaCha20
C implementation of the "ChaCha20" stream cipher following RFC 8439

## Initialization

Run the following command to build the program
```bash
make

```
To clean the forlder use:
```bash
make clean
```

## Usage

To encrypt/decrypt a file using ChaCha20, run the following command:

```bash
./chacha20 key NONCE inputfilename outputfilename
```
[] `key` should be a file containing a 32-byte long key.
[] `NONCE` should be a 24-character hexadecimal string.

For example, you can encrypt and decrypt the text file `sunscreen.txt` using:
```bash
./chacha20 keyfile 000000000000004a00000000 sunscreen.txt my_ciphertext.bin # encryption
./chacha20 keyfile 000000000000004a00000000 my_ciphertext.bin my_sunscreen.txt # decryption
```

The content of `sunscreen.txt` and `my_sunscreen.txt` should be the same.

### Author
Octave Charrin
