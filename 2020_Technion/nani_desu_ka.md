# nani desu ka??
Category: Cryptography

## Description
> この暗号は何ですか？

A binary ciphertext file and a Python script were attached.

```python
from Crypto.Cipher import DES

flag = ""
key = ""
iv = b'13371337'

def pad(m):
	size = len(m) + (8 - ( len(m) % 8 ))
	padding = bytes([size])
	return m.ljust(size, padding)

def encrypt(m, key, iv):
	des = DES.new(key, DES.MODE_OFB, iv)
	return des.encrypt(m)

def decrypt(c, key, iv):
	des = DES.new(key, DES.MODE_OFB, iv)
	return des.decrypt(c)


c = encrypt(pad(flag), key, iv) 
with open('ciphertext','wb') as handle:
	handle.write(c)
```

## Solution

The title means "what is (it)?" and the description translates to "What is this cipher?".

Let's inspect the binary file:

```console
root@kali:/media/sf_CTFs/technion/nani_desu_ka# xxd ciphertext
00000000: 29e5 ff03 571f 4ba6 1013 5b52 4356 135e  )...W.K...[RCV.^
00000010: 1daa e40f 1255 42a8 5054 0917 5240 4752  .....UB.PT..R@GR
00000020: 0de2 fe0e 181b 5ff0 6e44 0003 5a6c 5804  ......_.nD..ZlX.
00000030: 17d5 f402 0400 59fc                      ......Y.
```

Not much we can take from there.

However, from the Python file we see that the plaintext was encrypted with DES-OFB. Searching for vulnerabilities regarding this encryption method, we find a [similar challenge](https://github.com/Alpackers/CTF-Writeups/tree/master/2016/BostonKeyParty/Crypto/des-ofb) which was solved by trying to decrypt the message with some weak keys:

```
0x0000000000000000
0xFFFFFFFFFFFFFFFF
0xE1E1E1E1F0F0F0F0
0x1E1E1E1E0F0F0F0F
```

Let's try them all:

```python
import nani
import string, binascii

WEAK_KEYS = [b'\x00\x00\x00\x00\x00\x00\x00\x00', b'\x1E\x1E\x1E\x1E\x0F\x0F\x0F\x0F',
             b'\xE1\xE1\xE1\xE1\xF0\xF0\xF0\xF0', b'\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF']

with open('ciphertext','rb') as f:
    ciphertext = f.read()
    for key in WEAK_KEYS:
        plaintext = nani.decrypt(ciphertext, key, nani.iv)
        if (all(c in string.printable for c in plaintext)):
            print("Key: {}".format(binascii.hexlify(key)))
            print ("Text: {}".format(plaintext))
```

Output:
```console
root@kali:/media/sf_CTFs/technion/nani_desu_ka# python solve.py
Key: ffffffffffffffff
Text: Good job! here is the flag: cstechnion{4_w34k_k3y_desu}8
```