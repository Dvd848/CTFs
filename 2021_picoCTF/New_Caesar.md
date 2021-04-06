# New Caesar
Category: Crypto, 60 points

## Description

> We found a brand new type of encryption, can you break the secret code? (Wrap with picoCTF{}) 
> 
> `kjlijdliljhdjdhfkfkhhjkkhhkihlhnhghekfhmhjhkhfhekfkkkjkghghjhlhghmhhhfkikfkfhm`

A Python file was attached:

```python
import string

LOWERCASE_OFFSET = ord("a")
ALPHABET = string.ascii_lowercase[:16]

def b16_encode(plain):
	enc = ""
	for c in plain:
		binary = "{0:08b}".format(ord(c))
		enc += ALPHABET[int(binary[:4], 2)]
		enc += ALPHABET[int(binary[4:], 2)]
	return enc

def shift(c, k):
	t1 = ord(c) - LOWERCASE_OFFSET
	t2 = ord(k) - LOWERCASE_OFFSET
	return ALPHABET[(t1 + t2) % len(ALPHABET)]

flag = "redacted"
key = "redacted"
assert all([k in ALPHABET for k in key])
assert len(key) == 1

b16 = b16_encode(flag)
enc = ""
for i, c in enumerate(b16):
	enc += shift(c, key[i % len(key)])
print(enc)

```

## Solution

This encryption scheme first encodes the plaintext as `base16` (which essentially encodes each nibble of the plaintext as a character) and then applies a shift cipher to it.

So, in order to reverse the operation, let's start by implementing the opposite operations:

```python
import string

LOWERCASE_OFFSET = ord("a")
ALPHABET = string.ascii_lowercase[:16]

def b16_decode(enc):
    plain = ""
    for c1, c2 in zip(enc[0::2], enc[1::2]):
        n1 = "{0:04b}".format(ALPHABET.index(c1))
        n2 = "{0:04b}".format(ALPHABET.index(c2))
        binary = int(n1 + n2, 2)
        plain += chr(binary)
    return plain

def unshift(c, k):
    t1 = ord(c) - LOWERCASE_OFFSET
    t2 = ord(k) - LOWERCASE_OFFSET
    return ALPHABET[(t1 - t2) % len(ALPHABET)]

def decrypt(enc, key):
    dec = ""
    for i, c in enumerate(enc):
        dec += unshift(c, key[i % len(key)])
    return dec
```

Now we can iterate all the possible keys (which are all the letters in the alphabet), try to decrypt the ciphertext and if the result is a legal base16 string, try to decrypt that as well. We'll ignore anything that doesn't result in a printable plaintext.

```python
ciphertext = "kjlijdliljhdjdhfkfkhhjkkhhkihlhnhghekfhmhjhkhfhekfkkkjkghghjhlhghmhhhfkikfkfhm"

for k in ALPHABET:
    decrypted = decrypt(ciphertext, k)
    if all([c in ALPHABET for c in decrypted]):
        decoded = b16_decode(decrypted)
        if all([c in string.printable for c in decoded]):
            print(f"Key: {k}, Plaintext: {decoded}")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/New_Caesar]
└─$ python3 solve.py
Key: e, Plaintext: et_tu?_1ac5f3d7920a85610afeb2572831daa8
Key: f, Plaintext: TcNcd.N PR$U"S&(!/P'$% /PUTQ!$&!'" SPP'
```

The first one makes more sense, given the context. The flag: `picoCTF{et_tu?_1ac5f3d7920a85610afeb2572831daa8}`