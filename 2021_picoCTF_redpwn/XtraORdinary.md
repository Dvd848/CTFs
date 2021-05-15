# XtraORdinary
Crypto, 150 points

## Description

> Check out my new, never-before-seen method of encryption! I totally invented it myself. I added so many for loops that I don't even know what it does. It's extraordinarily secure!

Two files were attached:

`encrypt.py`:

```python
#!/usr/bin/env python3

from random import randint

with open('flag.txt', 'rb') as f:
    flag = f.read()

with open('secret-key.txt', 'rb') as f:
    key = f.read()

def encrypt(ptxt, key):
    ctxt = b''
    for i in range(len(ptxt)):
        a = ptxt[i]
        b = key[i % len(key)]
        ctxt += bytes([a ^ b])
    return ctxt

ctxt = encrypt(flag, key)

random_strs = [
    b'my encryption method',
    b'is absolutely impenetrable',
    b'and you will never',
    b'ever',
    b'ever',
    b'ever',
    b'ever',
    b'ever',
    b'ever',
    b'break it'
]

for random_str in random_strs:
    for i in range(randint(0, pow(2, 8))):
        for j in range(randint(0, pow(2, 6))):
            for k in range(randint(0, pow(2, 4))):
                for l in range(randint(0, pow(2, 2))):
                    for m in range(randint(0, pow(2, 0))):
                        ctxt = encrypt(ctxt, random_str)

with open('output.txt', 'w') as f:
    f.write(ctxt.hex())
```

`output.txt`:
```
57657535570c1e1c612b3468106a18492140662d2f5967442a2960684d28017931617b1f3637
```

## Solution

From what we see here, the program opens a flag file and a secret key, XORs them, and them XORs the result with a few predefined strings for a random amount of times.

The key point here is that `x ^ x = 0`, so that each time the result is XORed with the same string twice, it's as if it wasn't XORed with the string in the first place.

This means that for each of the random strings, we can say that:

 * If `ctxt` was XORed with a given string for an odd amount of times, we can treat it as if `ctxt` was XORed with the string just once
 * If `ctxt` was XORed with a given string for an even amount of times, we can treat it as if `ctxt` wasn't XORed with the string at all

The rules above can also be applied to the string `ever` which repeats itself multiple times, but we can reduce it to just one appearance.

Based on this, we can try to take `output.txt` and XOR it with every possible combination of the random strings:

 * None of them (i.e. each of the random strings was used an even amount of times)
 * Just the first string 
 * Just the first and the second string
 * ...
 * All of the strings (i.e. each of the strings was used an odd amount of times)

For each such combination, we take the result and XOR it with `output`, then XOR it again with the flag prefix and check to see if we get something familiar. If we do, we can assume that we found the original key used for encryption.

The following script does this:

```python
import itertools

with open("output.txt") as f:
    flag = bytes.fromhex(f.read())

def encrypt(ptxt, key):
    ctxt = b''
    for i in range(len(ptxt)):
        a = ptxt[i]
        b = key[i % len(key)]
        ctxt += bytes([a ^ b])
    return ctxt


random_strs = [
    b'my encryption method',
    b'is absolutely impenetrable',
    b'and you will never',
    b'ever',
    b'break it'
]

prefix = b"picoCTF{"

def apply_encryption(ctxt, truth_table):
    for i, to_xor in enumerate(truth_table):
        if to_xor:
            ctxt = encrypt(ctxt, random_strs[i])
    return ctxt

for p in itertools.product([True, False], repeat=len(random_strs)):
    ctxt = apply_encryption(flag, p)
    print(f"{p}: \t{encrypt(ctxt[:len(prefix)], prefix)}")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/XtraORdinary]
└─$ python3 solve.py
(True, True, True, True, True):         b'Elrmoq<T'
(True, True, True, True, False):        b"'\x1e\x17\x0c\x04QU "
(True, True, True, False, True):        b' \x1a\x17\x1f\n\x07Y&'
(True, True, True, False, False):       b"Bhr~a'0R"
(True, True, False, True, True):        b'$\x02\x16M\x16\x1eIt'
(True, True, False, True, False):       b'Fps,}> \x00'
(True, True, False, False, True):       b'Ats?sh,\x06'
(True, True, False, False, False):      b'#\x06\x16^\x18HEr'
(True, False, True, True, True):        b',\x1fR\x0c\r\x02S8'
(True, False, True, True, False):       b'Nm7mf":L'
(True, False, True, False, True):       b'Ii7~ht6J'
(True, False, True, False, False):      b'+\x1bR\x1f\x03T_>'
(True, False, False, True, True):       b'Mq6,tm&\x18'
(True, False, False, True, False):      b'/\x03SM\x1fMOl'
(True, False, False, False, True):      b'(\x07S^\x11\x1bCj'
(True, False, False, False, False):     b'Ju6?z;*\x1e'
(False, True, True, True, True):        b'(\x15R\x08\x01\x12N-'
(False, True, True, True, False):       b"Jg7ij2'Y"
(False, True, True, False, True):       b'Mc7zdd+_'
(False, True, True, False, False):      b'/\x11R\x1b\x0fDB+'
(False, True, False, True, True):       b'I{6(x};\r'
(False, True, False, True, False):      b'+\tSI\x13]Ry'
(False, True, False, False, True):      b',\rSZ\x1d\x0b^\x7f'
(False, True, False, False, False):     b'N\x7f6;v+7\x0b'
(False, False, True, True, True):       b'Africa!A'
(False, False, True, True, False):      b'#\x14\x17\x08\x08AH5'
(False, False, True, False, True):      b'$\x10\x17\x1b\x06\x17D3'
(False, False, True, False, False):     b'Fbrzm7-G'
(False, False, False, True, True):      b' \x08\x16I\x1a\x0eTa'
(False, False, False, True, False):     b'Bzs(q.=\x15'
(False, False, False, False, True):     b'E~s;\x7fx1\x13'
(False, False, False, False, False):    b"'\x0c\x16Z\x14XXg"
```


We can see that the following line contains a dictionary word:

```
(False, False, True, True, True):       b'Africa!A'
```

So let's use it to decrypt the ciphertext:

```python
ctxt = apply_encryption(flag, (False, False, True, True, True))
print(encrypt(ctxt, b"Africa!"))
```

And we get the flag: `picoCTF{w41t_s0_1_d1dnt_1nv3nt_x0r???}`