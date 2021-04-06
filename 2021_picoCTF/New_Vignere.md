# New Vignere
Category: Crypto, 300 points

## Description

> Another slight twist on a classic, see if you can recover the flag. (Wrap with picoCTF{}) 
> 
> `epdfglkfnbjbhbpicohidjgkhfnejeecmjfnejddgmhpndmchbmifnepdhdmhbah`

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
assert all([c in "abcdef0123456789" for c in flag])

key = "redacted"
assert all([k in ALPHABET for k in key]) and len(key) < 15

b16 = b16_encode(flag)
enc = ""
for i, c in enumerate(b16):
	enc += shift(c, key[i % len(key)])
print(enc)

```

## Solution

This is the follow-up for [New Caesar](New_Caesar.md). Basically it's a [Vigenère cipher](https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher) applied on the New Caesar scheme.

We've already implemented the following functions for the previous challenge:

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

To them, we add the following logic:

```console
from itertools import product, zip_longest

FLAG_CHARS = "abcdef0123456789"

encrypted = "epdfglkfnbjbhbpicohidjgkhfnejeecmjfnejddgmhpndmchbmifnepdhdmhbah"

good_letters = set(b16_encode("abcdef0123456789"))

for key_len in range(2, 15):
    print("Testing key length: {}".format(key_len))
    parts = [None] * key_len
    for i in range(key_len):
        parts[i] = encrypted[i::key_len]
        

    bf = []

    for i, part in enumerate(parts):
        bf.append([])
        for key in ALPHABET:
            decrypted = decrypt(part, key)
            if all(c in good_letters for c in decrypted):
                bf[i].append(decrypted)
            
    print("")

    if any(len(b) == 0 for b in bf):
        continue

    for prod in product(*bf):
        decrypted = ""
        for zipped in zip_longest(*prod):
            decrypted += "".join(filter(None, zipped))
        decoded = b16_decode(decrypted)
        if all(c in FLAG_CHARS for c in decoded):
            print(decoded)
```

So what's going on here?

We don't know the key length so we must try all possible key lengths (we ignore a length of 1 since that would just be a Caesar Cipher).

For a given key length of `k`, we create an array of `k` elements where the element at index `i` is a string of every `k-th` letter starting from the `i-th` letter. So, if `k == 9`, we get:

```python
['ebheedeh', 'pjijjmp', 'dbdedcd', 'fhjedhh', 'gbgcgbd', 'lpkmmmm', 'kihjhih', 'fcffpfb', 'nonnnna']
```

Now, due to the way that a Vigenère cipher works, each of these strings should be solvable as Caesar Cipher. So for each string, we try to decrypt it using every possible key, and check if it decrypts to a valid `base16` string. If it does, we add it to a list for further processing (`bf`).

Continuing with the example of a key of length 9, we will get:

```python
bf = [
    ['ebheedeh', 'dagddcdg', 'gdjggfgj', 'fciffefi'], 
    ['jdcddgj', 'icbccfi', 'hbabbeh'], 
    ['dbdedcd', 'cacdcbc', 'igijihi', 'hfhihgh', 'geghgfg', 'fdfgfef', 'ecefede'], 
    ['fhjedhh', 'egidcgg', 'dfhcbff', 'cegbaee'], 
    ['gbgcgbd', 'fafbfac', 'jejfjeg', 'idieidf', 'hchdhce'], 
    ['fjegggg', 'eidffff', 'dhceeee', 'cgbdddd', 'bfacccc'], 
    ['jhgighg', 'igfhfgf', 'hfegefe', 'gedfded', 'fdcecdc', 'ecbdbcb', 'dbacaba'], 
    ['jgjjdjf', 'ifiicie', 'hehhbhd', 'gdggagc'], 
    ['ghggggj', 'fgffffi', 'efeeeeh', 'deddddg', 'cdccccf', 'bcbbbbe', 'abaaaad']
]
```

Now, the `base16` string must be some combination of these members, once we "re-scatter" them according to their original locations. So we try all possible combinations and see if that produces a good `base_16` string (one that decodes to the characters used in the flag). 

For example, if we take the first member of each sub-array and "re-scatter" this group according to the original location, we will get:

```python
prod = ('ebheedeh', 'jdcddgj', 'dbdedcd', 'fhjedhh', 'gbgcgbd', 'fjegggg', 'jhgighg', 'jgjjdjf', 'ghggggj')
decrypted = 'ejdfgfjjgbdbhbjhghhcdjgegjgedeecgijgedddgggdgdgchbghjgejdhdggfjh'
```

But when we try to decode that as base 16, we get:

```python
decoded = 'I5e\x99a1q\x97gr9did4Bh\x96C3fccbqg\x96I76e\x97'
```

This isn't printable, so we skip it.

Eventually, we get:

```python
prod = ('dagddcdg', 'jdcddgj', 'dbdedcd', 'egidcgg', 'gbgcgbd', 'cgbdddd', 'gedfded', 'gdggagc', 'deddddg')
decrypted = 'djdegcggdadbgbgedegcdigbdgdddedcdfgddddcgddadcgcgbdegddjdgdddcgg'
decoded = '94bf01ad4b8a63425c32c02ba4c9632f'
```

The flag: `picoCTF{94bf01ad4b8a63425c32c02ba4c9632f}`