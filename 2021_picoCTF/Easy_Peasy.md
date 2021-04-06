# Easy Peasy
Category: Crypto, 40 points

## Description
> A one-time pad is unbreakable, but can you manage to recover the flag? (Wrap with picoCTF{})

```python
#!/usr/bin/python3 -u
import os.path

KEY_FILE = "key"
KEY_LEN = 50000
FLAG_FILE = "flag"


def startup(key_location):
	flag = open(FLAG_FILE).read()
	kf = open(KEY_FILE, "rb").read()

	start = key_location
	stop = key_location + len(flag)

	key = kf[start:stop]
	key_location = stop

	result = list(map(lambda p, k: "{:02x}".format(ord(p) ^ k), flag, key))
	print("This is the encrypted flag!\n{}\n".format("".join(result)))

	return key_location

def encrypt(key_location):
	ui = input("What data would you like to encrypt? ").rstrip()
	if len(ui) == 0 or len(ui) > KEY_LEN:
		return -1

	start = key_location
	stop = key_location + len(ui)

	kf = open(KEY_FILE, "rb").read()

	if stop >= KEY_LEN:
		stop = stop % KEY_LEN
		key = kf[start:] + kf[:stop]
	else:
		key = kf[start:stop]
	key_location = stop

	result = list(map(lambda p, k: "{:02x}".format(ord(p) ^ k), ui, key))

	print("Here ya go!\n{}\n".format("".join(result)))

	return key_location


print("******************Welcome to our OTP implementation!******************")
c = startup(0)
while c >= 0:
	c = encrypt(c)

```

## Solution

When we connect to the attached service, we first get the encrypted flag, then are able to encrypt as much data as we want:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Easy_Peasy]
└─$ nc mercury.picoctf.net 20266
******************Welcome to our OTP implementation!******************
This is the encrypted flag!
5b1e564b6e415c0e394e0401384b08553a4e5c597b6d4a5c5a684d50013d6e4b

What data would you like to encrypt? test
Here ya go!
15532f0c

What data would you like to encrypt?
```

From the service implementation, we see that it uses a XOR pad of length 50000 to encrypt the input. This should be unbreakable if it's used as a one-time-pad, but in our case the service performs a wrap-around and reuses the same pad for every 50000 characters.

So, to retrieve the XOR values used to encrypt the flag, we just need to cause a wraparound, allowing our known input to be re-encrypted with the same XOR values. Since we know the input, we can XOR it with the encrypted result to get the key. Then, it's just a matter of XOR-ing the key with the encrypted flag. 

In fact, we can even make this a bit more efficient by re-encrypting the same encrypted flag with the XOR stream that was used to originally encrypt it. This should result in the plaintext flag.

The script:

```python
from pwn import *

KEY_LEN = 50000
MAX_CHUNK = 1000

r = remote("mercury.picoctf.net", 20266)
r.recvuntil("This is the encrypted flag!\n")
flag = r.recvlineS(keepends = False)
log.info(f"Flag: {flag}")
bin_flag = unhex(flag)

counter = KEY_LEN - len(bin_flag)

with log.progress('Causing wrap-around') as p:
    while counter > 0:
        p.status(f"{counter} bytes left")
        chunk_size = min(MAX_CHUNK, counter)
        r.sendlineafter("What data would you like to encrypt? ", "a" * chunk_size)
        
        counter -= chunk_size

r.sendlineafter("What data would you like to encrypt? ", bin_flag)
r.recvlineS()
log.success("The flag: {}".format(unhex(r.recvlineS())))

```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Easy_Peasy]
└─$ python3 solve.py
[+] Opening connection to mercury.picoctf.net on port 20266: Done
[*] Flag: 5b1e564b6e415c0e394e0401384b08553a4e5c597b6d4a5c5a684d50013d6e4b
[+] Causing wrap-around: Done
[+] The flag: b'99072996e6f7d397f6ea0128b4517c23'
```

