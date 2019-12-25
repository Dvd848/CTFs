# SmpleKye
Crypto, 10 points

## Description
> Maybe if I try hard enough.

## Solution

We enter the provided link and see the following instructions:

> Definition of a simple key:
> 
> You are given this string:
> 
> ff
> 
> Find a 3+ character string that when combined with the given string (AABBB+), the final string will appear in its own sha256 digest


Simple enough, and easily solvable with the following script:

```python
import hashlib, itertools

alphabet = '1234567890abcdef'
prefix = 'ff'


keywords = itertools.product(alphabet, repeat = 3)
for keyword in keywords:
    plaintext = prefix + "".join(keyword)
    hash = hashlib.sha256(plaintext.encode('ascii')).hexdigest()
    if plaintext in hash:
        print('plaintext: "' + plaintext + '", hash: ' + hash)
        break
```

The output received from running the script is:
```
plaintext: "ff32e", hash: 6480a09a9eb3baad584d951ff32e245d3aa287f50fd71043a53dea9f76e2f6d7
```

The flag: `KAF{ju57_4_51mpl3_pr00F_0F_w0Rk}`