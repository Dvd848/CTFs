# Compress and Attack
Category: Cryptography, 130 points

## Description

> Your goal is to find the flag.
> 
> Hint:
> 
> * The flag only contains uppercase and lowercase letters, underscores, and braces (curly brackets)

A Python script was attached:

```python
#!/usr/bin/python3 -u

import zlib
from random import randint
import os
from Crypto.Cipher import Salsa20

flag = open("./flag").read()

def compress(text):
    return zlib.compress(bytes(text.encode("utf-8")))

def encrypt(plaintext):
    secret = os.urandom(32)
    cipher = Salsa20.new(key=secret)
    return cipher.nonce + cipher.encrypt(plaintext)

def main():
    while True:
        usr_input = input("Enter your text to be encrypted: ")
        compressed_text = compress(flag + usr_input)
        encrypted = encrypt(compressed_text)
        
        nonce = encrypted[:8]
        encrypted_text =  encrypted[8:]
        print(nonce)
        print(encrypted_text)
        print(len(encrypted_text))

if __name__ == '__main__':
    main() 
```

## Solution

The attached Python script requests input from the user, then encrypts it together with the flag and outputs the ciphertext.

We can try it via the attached service:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Compress_and_Attack]
└─$ nc mercury.picoctf.net 50899
Enter your text to be encrypted: test
b'>\xa1\xed?0a\x05\xc8'
b'\xfc\x10a\xadjm\x02\xd9\x03s\xef\xd9\\\x81\xd9\xcc8\x15j\xdf\xaf\xfe\xa7\xff,\x96U\xde\x80#r\x9e\x9b\xa0\x12v@\xe5\xfe\xed\xcaF\xa3\x95\xed\x0f~_\x9d'
49
```

Things to note here:

 * The program first compresses `flag + usr_input`, then encrypts it
 * The compression is performed using `zlib`, which presents a more efficient compression ration if the payload has a longer repeating sequence
 * The encryption is performed using `Salsa20` which is a stream cipher, and therefore the length of the plaintext is equal to the length of the ciphertext

Taking advantage of these facts, we can try to bruteforce the flag by guessing it character after character, inspecting the length of the ciphertext, and selecting the character which provided the shortest length (most efficient compression ratio).

For example:

```python
>>> import zlib
>>> len(zlib.compress(bytes("picoCTF{fake_flag}picoCTF{a".encode("utf-8"))))
30
>>> len(zlib.compress(bytes("picoCTF{fake_flag}picoCTF{b".encode("utf-8"))))
30
>>> len(zlib.compress(bytes("picoCTF{fake_flag}picoCTF{f".encode("utf-8"))))
29
>>> len(zlib.compress(bytes("picoCTF{fake_flag}picoCTF{g".encode("utf-8"))))
30
```

As we can see, when we append to the (fake) flag a substring which shares a longer common prefix with it, we get a shorter output.

The following script automates the process:

```python
from pwn import *
from collections import namedtuple
import string


Pair = namedtuple("Pair", "enc_len char")

def get_encrypted_length(plaintext):
    while True:
        try:
            r = get_encrypted_length.r
            r.sendlineafter("Enter your text to be encrypted:", plaintext)
            nonce = r.recvline()
            encrypted = r.recvline()
            return int(r.recvlineS())
        except (AttributeError, EOFError):
            get_encrypted_length.r = remote("mercury.picoctf.net", 50899)


with log.progress("Brute forcing by encrypted length") as p:
    flag = "picoCTF{"
    while (flag[-1] != "}"):
        lengths = []
        for c in string.ascii_letters + "{}_":
            candidate = flag + c
            length = get_encrypted_length(candidate)
            p.status(f"Current flag: '{flag}', character '{c}' returned encrypted length of {length}")
            lengths.append(Pair(length, c))
        
        lengths.sort()
        smallest = lengths.pop(0)
        if (smallest.enc_len >= lengths[0].enc_len):
            raise RuntimeError(f"More than one character produced minimal encrypted length: '{smallest.char}', '{lengths[0].char}'")
        flag += smallest.char
        p.status(f"Appending '{smallest.char}' with minimal length of {smallest.enc_len} to flag")

log.success(f"Flag: {flag}")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Compress_and_Attack]
└─$ python3 solve.py
[+] Brute forcing by encrypted length: Done
[+] Opening connection to mercury.picoctf.net on port 50899: Done
[+] Opening connection to mercury.picoctf.net on port 50899: Done
[+] Opening connection to mercury.picoctf.net on port 50899: Done
[+] Opening connection to mercury.picoctf.net on port 50899: Done
[+] Flag: picoCTF{sheriff_you_solved_the_crime}
```