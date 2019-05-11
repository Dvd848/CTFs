# 16-bit-AES
Crypto, 100 points

## Description

> Why so small?

## Solution

We connect to the server and see the following message:

```console
root@kali:/media/sf_CTFs/sunshine/16_bit_AES# nc archive.sunshinectf.org 19003
Welcome, I'm using an AES-128 cipher with a 16-bit key in ECB mode.

I'll give you some help: give me some text, and I'll tell you what it looks like

Your text:
test
5daa39d0cd90af3ded4b2813413f138c

Ok, now encrypt this text with the same key: LGSQKtc4T25nsl29
```

The service allows us to encrypt a string of our choice and observe the result, then requests us to encrypt a string with the same key.

What happens if we run the service again and enter the same string:
```
Welcome, I'm using an AES-128 cipher with a 16-bit key in ECB mode.

I'll give you some help: give me some text, and I'll tell you what it looks like

Your text:
test
5daa39d0cd90af3ded4b2813413f138c

Ok, now encrypt this text with the same key: AjEdDZgk25hf1ByV
```

We get the same result for our text, but a different string to encrypt ourselves. In other words, the key isn't changed throughout sessions.

The obvious solution is to open two sessions, take the string they ask us to encrypt from session #1, paste it in session #2 as the script we can encrypt ourselves, and copy the result back to session #1. However, that would work for any key length, so it feels like cheating. Since the key is so small (16 bits), we are probably expected to brute force it.

First, we try the following code, to try and encrypt AES-ECB with a 16-bit key:
```python
from Crypto.Cipher import AES
 
key = 'aa'
 
cipher = AES.new(key, AES.MODE_ECB)
msg = cipher.encrypt('0123456789abcdef')
 
print(msg.encode("hex"))
```

However, we get the following error:
```
ValueError: AES key must be either 16, 24, or 32 bytes long
```

Either the challenge is expecting us to implement AES with a 16-bit key, or the 16-bit key is duplicated 8 times to create a 16-byte key (or more for another key length). Since the latter is easier to test, we start from it.


The script:
```python
from Crypto.Cipher import AES
from itertools import product
from pwn import *
import string

def find_key(plaintext, ciphertext_hex):
    for key_chars in product(string.printable, repeat=2):
        key = (''.join(key_chars)) * 8
        cipher = AES.new(key, AES.MODE_ECB)
        msg = cipher.encrypt(plaintext)
        if msg.encode("hex") == "552403aa43019223392a5516cd788601":
            return key
    return None

text_to_encrypt = '0123456789abcdef'
p = remote("archive.sunshinectf.org", 19003)
p.sendlineafter("Your text:", text_to_encrypt)
encrypted = p.recvline()
server_key = find_key(text_to_encrypt, encrypted)
log.info("Server key: {}".format(server_key))
if server_key is not None:
    p.recvuntil("same key: ")
    server_text_to_encrypt = p.recvline(keepends = False).rstrip()
    log.info("Text to encrypt: {}".format(server_text_to_encrypt))
    encrypted_server_text = AES.new(server_key, AES.MODE_ECB).encrypt(server_text_to_encrypt)
    p.sendline(encrypted_server_text.encode("hex"))
    print p.recvall()
```

The output:
```console
root@kali:/media/sf_CTFs/sunshine/16_bit_AES# python solve.py
[+] Opening connection to archive.sunshinectf.org on port 19003: Done
[*] Server key: fLfLfLfLfLfLfLfL
[*] Text to encrypt: 8VLnmj1cTliKa8Ht
[+] Receiving all data: Done (68B)
[*] Closed connection to archive.sunshinectf.org port 19003

Correct! The flag is sun{Who_kn3w_A3$_cou1d_be_s0_vulner8ble?}
```
