# Double DES
Category: Forensics, 130 points

## Description
> I wanted an encryption service that's more secure than regular DES, but not as slow as 3DES... The flag is not in standard format.

```python
#!/usr/bin/python3 -u
from Crypto.Cipher import DES
import binascii
import itertools
import random
import string


def pad(msg):
    block_len = 8
    over = len(msg) % block_len
    pad = block_len - over
    return (msg + " " * pad).encode()

def generate_key():
    return pad("".join(random.choice(string.digits) for _ in range(6)))


FLAG = open("flag").read().rstrip()
KEY1 = generate_key()
KEY2 = generate_key()


def get_input():
    try:
        res = binascii.unhexlify(input("What data would you like to encrypt? ").rstrip()).decode()
    except:
        res = None
    return res

def double_encrypt(m):
    msg = pad(m)

    cipher1 = DES.new(KEY1, DES.MODE_ECB)
    enc_msg = cipher1.encrypt(msg)
    cipher2 = DES.new(KEY2, DES.MODE_ECB)
    return binascii.hexlify(cipher2.encrypt(enc_msg)).decode()


print("Here is the flag:")
print(double_encrypt(FLAG))

while True:
    inputs = get_input()
    if inputs:
        print(double_encrypt(inputs))
    else:
        print("Invalid input.")
```

## Solution

We get the encrypted output of the flag. We are also allowed to encrypt our own input, which means that we are able to inspect an encrypted output for which we know the plaintext.

Since the encryption scheme just encrypts the data twice using two keys, it is vulnerable to a [Meet-in-the-middle](https://en.wikipedia.org/wiki/Meet-in-the-middle_attack) attack:

> When trying to improve the security of a block cipher, a tempting idea is to encrypt the data several times using multiple keys. One might think this doubles or even n-tuples the security of the multiple-encryption scheme, depending on the number of times the data is encrypted, because an exhaustive search on all possible combination of keys (simple brute-force) would take 2n·k attempts if the data is encrypted with k-bit keys n times.
> 
> The MITM is a generic attack which weakens the security benefits of using multiple encryptions by storing intermediate values from the encryptions or decryptions and using those to improve the time required to brute force the decryption keys. This makes a Meet-in-the-Middle attack (MITM) a generic space–time tradeoff cryptographic attack.
> 
> The MITM attack attempts to find the keys by using both the range (ciphertext) and domain (plaintext) of the composition of several functions (or block ciphers) such that the forward mapping through the first functions is the same as the backward mapping (inverse image) through the last functions, quite literally meeting in the middle of the composed function.


So, we first encrypt a random string, such as `"a"` using 2DES. We receive the encrypted output from the server, and start attacking it.

We encrypt `a` with all possible keys (which are essentially all possible strings of 6 digits, padded to the length of a block) using single DES, and save each encrypted output in a dictionary, together with the key that was used for the encryption. 

Then, we take the encrypted output we got from the server when we encrypted `a` using 2DES, and try to decrypt it with all possible keys using single DES. Eventually, we should get a result which matches some encrypted output from the first phase. The keys that were used to achieve this are the secret keys also used to encrypt the flag.

```python
#!/usr/bin/python3 -u
from pwn import *

from Crypto.Cipher import DES
import itertools
import string

KEY_LEN = 6

def pad(msg):
    block_len = 8
    over = len(msg) % block_len
    pad = block_len - over
    return (msg + " " * pad).encode()

def double_decrypt(m, key1, key2):
    cipher2 = DES.new(key2, DES.MODE_ECB)
    dec_msg = cipher2.decrypt(m)

    cipher1 = DES.new(key1, DES.MODE_ECB)
    return cipher1.decrypt(dec_msg)

def all_possible_keys():
    return itertools.product(string.digits, repeat=KEY_LEN)

r = remote("mercury.picoctf.net", 1903)
r.recvline()
flag = r.recvlineS().strip()
log.info("Encrypted flag: {}".format(flag))

to_encrypt = b'a'
log.info("Trying to encrypt '{}'".format(to_encrypt))
r.sendlineafter("What data would you like to encrypt?", enhex(to_encrypt))
a_enc = r.recvlineS().strip()
log.info("Encrypted form: {}".format(a_enc))
a_enc = bytes.fromhex(a_enc)

a_padded = pad(to_encrypt.decode())

d = {}

with log.progress('Encrypting plaintext with all possible keys') as p:
    for k1 in all_possible_keys():
        k1 = pad("".join(k1))
        p.status("Key: {}".format(k1))
        cipher1 = DES.new(k1, DES.MODE_ECB)
        enc = cipher1.encrypt(a_padded)
        d[enc] = k1

with log.progress('Decrypting ciphertext with all possible keys') as p:
    for k2 in all_possible_keys():
        k2 = pad("".join(k2))
        p.status("Key: {}".format(k2))
        cipher2 = DES.new(k2, DES.MODE_ECB)
        dec = cipher2.decrypt(a_enc)
        if dec in d:
            k1 = d[dec]
            log.info("Found match, key1 = {}, key2 = {}".format(k1, k2))
            log.success(double_decrypt(unhex(flag), k1, k2))
            break
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Double_DES]
└─$ python3 solve.py
[+] Opening connection to mercury.picoctf.net on port 1903: Done
[*] Encrypted flag: a47d8e5b366215bea6621ae98d347e7e02b22bae01beb596ead9f4fdbeb09629e12aade04cc8df1a
[*] Trying to encrypt 'b'a''
[*] Encrypted form: 00c9603b6bb1a03b
[+] Encrypting plaintext with all possible keys: Done
[+] Decrypting ciphertext with all possible keys: Done
[*] Found match, key1 = b'137731  ', key2 = b'004204  '
[+] cb120914153b84dbc68fedd574b395f2
```

The flag: `cb120914153b84dbc68fedd574b395f2`