# Based
General Skills, 200 points

## Description:
> To get truly 1337, you must understand different data encodings, such as hexadecimal or binary. Can you get the flag from this program to prove you are on the way to becoming 1337?


## Solution: 

We connect to the service and get the following request:
```console
Let us see how data is stored
chair
Please give the 01100011 01101000 01100001 01101001 01110010 as a word.
...
you have 45 seconds.....

Input:
```

This looks like binary, let's decode:
```python
>>> for x in "01100011 01101000 01100001 01101001 01110010".split(" "):
...     sys.stdout.write(chr(int(x, 2)))
chair
```

Our input is accepted, and another request is received:
```console
Please give me the  146 141 154 143 157 156 as a word.
Input:
```

This looks like base 8 (highest value is 7), let's decode:
```python
>>> for x in "146 141 154 143 157 156".split(" "):
...     sys.stdout.write(chr(int(x,8)))
falcon
```

Since we keep getting new requests in different bases, let's write a script to decode a given sequence with an unknown base:
```python
from pwn import *
import re
import string

#context.log_level = "DEBUG"

r = remote("2019shell1.picoctf.com", 7380)

def get_base_encoded_str(r):
    s = r.recvuntil("the ")
    if ("Please" not in s):
        r.recvline()
        return None
    s = r.recvuntil(" as a word.", drop = True)
    return s.strip()

def decode_string_as_char_array(s, base):
    res = ""
    for unit in s.split(" "):
        c = chr(int(unit, base))
        if c not in string.ascii_letters:
            raise Exception("Non-ASCII result")
        res += c
    return res

def try_decode_as_char_array_with_unknown_base(s):
    for base in range(1, 17):
        try:
            res = decode_string_as_char_array(s, base)
            log.info("Decode successful with base {}".format(base))
            return res
        except:
            pass
    return None

def try_decode_as_hex(s):
    try:
        return s.decode("hex")
    except:
        return None

r.recvline()
r.recvline()
s = get_base_encoded_str(r)
while s is not None:
    log.info("Trying to decode '{}'".format(s))
    res = try_decode_as_char_array_with_unknown_base(s) or try_decode_as_hex(s)
    if res is None:
        log.error("Can't decode '{}'".format(s))
        break
    log.info("Decoded as '{}'".format(res))
    r.sendlineafter("Input:", res)
    s = get_base_encoded_str(r)

print r.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/Based# python solve.py
[+] Opening connection to 2019shell1.picoctf.com on port 7380: Done
[*] Trying to decode '01110000 01100101 01100001 01110010'
[*] Decode successful with base 2
[*] Decoded as 'pear'
[*] Trying to decode '164 145 163 164'
[*] Decode successful with base 8
[*] Decoded as 'test'
[*] Trying to decode '737472656574'
[*] Decoded as 'street'
[+] Receiving all data: Done (57B)
[*] Closed connection to 2019shell1.picoctf.com port 7380
Flag: picoCTF{learning_about_converting_values_8e70d435}
```