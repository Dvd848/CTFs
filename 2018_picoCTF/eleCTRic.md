# eleCTRic
Cryptography, 400 points

## Description:
> You came across a custom server that Dr Xernon's company eleCTRic Ltd uses. It seems to be storing some encrypted files. Can you get us the flag?

```python
#!/usr/bin/python

from Crypto import Random
from Crypto.Cipher import AES
import sys
import time
import binascii


class AESCipher(object):
    def __init__(self):
        self.bs = 32
        random = Random.new()
        self.key = random.read(AES.block_size)
        self.ctr = random.read(AES.block_size)

    def encrypt(self, raw):
        cipher = AES.new(self.key, AES.MODE_CTR, counter=lambda: self.ctr)
        return cipher.encrypt(raw).encode('base64').replace('\n', '')

    def decrypt(self, enc):
        try:
            enc = enc.decode('base64')
        except binascii.Error:
            return None
        cipher = AES.new(self.key, AES.MODE_CTR, counter=lambda: self.ctr)
        return cipher.decrypt(enc)

class Unbuffered(object):
    def __init__(self, stream):
        self.stream = stream
    def write(self, data):
        self.stream.write(data)
        self.stream.flush()
    def writelines(self, datas):
        self.stream.writelines(datas)
        self.stream.flush()
    def __getattr__(self, attr):
        return getattr(self.stream, attr)
    
sys.stdout = Unbuffered(sys.stdout)
    
def get_flag():
    try:
        with open("flag.txt") as f:
            return f.read().strip()
    except IOError:
        return "picoCTF{xxxFAKEFLAGxxx} Something went wrong. Contact organizers."
    
def welcome():
    print "Welcome to eleCTRic Ltd's Safe Crypto Storage"
    print "---------------------------------------------"


def menu():
    print ""
    print "Choices:"
    print "  E[n]crypt and store file"
    print "  D[e]crypt file"
    print "  L[i]st files"
    print "  E[x]it"
    while True:
        choice = raw_input("Please choose: ")
        if choice in list('neix'):
            print ""
            return choice


def do_encrypt(aes, files):
    filename = raw_input("Name of file? ")
    if any(x in filename for x in '._/\\ '):
        print "Disallowed characters"
        return
    filename += '.txt'
    if filename in files:
        if raw_input("Clobber previously existing file? [yN] ") != 'y':
            return
    data = raw_input("Data? ")
    files[filename] = aes.encrypt(data)
    print "Share code:"
    print aes.encrypt(filename)


def do_decrypt(aes, files):
    enc = raw_input("Share code? ")
    filename = aes.decrypt(enc)
    if filename is None:
        print "Invalid share code"
        return
    if filename in files:
        print "Data: "
        print aes.decrypt(files[filename])
    else:
        print "Could not find file"
        return


def do_list_files(files):
    print "Files:"
    for f in files:
        print "  " + f


def main():
    print "Initializing Problem..."
    aes = AESCipher()
    flag = get_flag()
    flag_file_name = "flag_%s" % Random.new().read(10).encode('hex')

    files = {flag_file_name + ".txt": aes.encrypt(flag)}

    welcome()
    while True:
        choice = menu()
        if choice == 'n':       # Encrypt
            do_encrypt(aes, files)
        elif choice == 'e':     # Decrypt
            do_decrypt(aes, files)
        elif choice == 'i':     # List files
            do_list_files(files)
        elif choice == 'x':     # Exit
            break
        else:
            print "Impossible! Contact contest admins."
            sys.exit(1)


main()
```

## Solution:

Connecting to the service, we can choose to encrypt, decrypt or list files.

If we start by listing files, we see the flag file is already there:
```
root@kali:/media/sf_CTFs/pico/Radixs_Terminal# nc 2018shell3.picoctf.com 15037
Initializing Problem...
Welcome to eleCTRic Ltd's Safe Crypto Storage
---------------------------------------------

Choices:
  E[n]crypt and store file
  D[e]crypt file
  L[i]st files
  E[x]it
Please choose: i

Files:
  flag_5747f8d4060b328f0fbc.txt
```

However, in order to decrypt it, we need the encrypted name of the file, which is received only during encryption:
```
Choices:
  E[n]crypt and store file
  D[e]crypt file
  L[i]st files
  E[x]it
Please choose: n

Name of file? test
Data? a
Share code:
HzoBdxcjpWM=

Choices:
  E[n]crypt and store file
  D[e]crypt file
  L[i]st files
  E[x]it
Please choose: e

Share code? HzoBdxcjpWM=
Data:
a
```

According to the source code, encryption is performed by AES-CTR:
```python
cipher = AES.new(self.key, AES.MODE_CTR, counter=lambda: self.ctr)
```

However, the counter is implemented as a constant, when in fact it should change for each block (more about this [here](https://ctftime.org/writeup/7199)):
```python
        self.ctr = random.read(AES.block_size)
```

This means that instead of a stream cipher, AES-CTR is being reduced to a block cipher where throughout each session, the nth block of the plaintext will always be encrypted to the same ciphertext. Furthermore, the encrypted nth block of two plaintexts which differ by x bytes will only differ by x bytes as well.

Let's see an example:
```
root@kali:/media/sf_CTFs/pico/Radixs_Terminal# nc 2018shell3.picoctf.com 15037
Initializing Problem...
Welcome to eleCTRic Ltd's Safe Crypto Storage
---------------------------------------------

Choices:
  E[n]crypt and store file
  D[e]crypt file
  L[i]st files
  E[x]it
Please choose: n

Name of file? test
Data? a
Share code:
xrJuQaPVc2o=

Choices:
  E[n]crypt and store file
  D[e]crypt file
  L[i]st files
  E[x]it
Please choose: n

Name of file? tess
Data? b
Share code:
xrJuRqPVc2o=

Choices:
  E[n]crypt and store file
  D[e]crypt file
  L[i]st files
  E[x]it
Please choose: x

root@kali:/media/sf_CTFs/pico/Radixs_Terminal# echo "xrJuQaPVc2o=" | base64 -d && echo
ƲnAsj
root@kali:/media/sf_CTFs/pico/Radixs_Terminal# echo "xrJuRqPVc2o=" | base64 -d && echo
ƲnFsj
```

We can see that "test" got encrypted to "ƲnAsj", and "tess" got encrypted to "ƲnFsj", where only the "A" and "F" differ.

So, if the name of the flag file is "flag_5747f8d4060b328f0fbc.txt", we can encrypt a file called "flagX5747f8d4060b328f0fbc.txt" and then brute force all possibilities for the encrypted byte in X's location until we get the correct one.

The code:
```python
from pwn import *
import base64
import re

r = remote("2018shell3.picoctf.com", 15037)

def list_files():
	log.info("Listing files")
	r.sendlineafter("Please choose:", "i")
	r.recvuntil("Files:")
	out = r.recvuntil("\n\n")
	return out

def encrypt(file_name, data):
	log.info("Encrypting file: Name: '{}', Data: '{}'".format(file_name, data))
	r.sendlineafter("Please choose:", "n")
	r.sendlineafter("Name of file? ", file_name)
	r.sendlineafter("Data? ", data)
	r.recvline()
	code = r.recvline()
	log.info("Base64 Code: {}".format(code))
	return code


def decrypt(file_name):
	log.info("Requesting to decrypt file '{}'".format(file_name))
	r.sendlineafter("Please choose:", "e")
	r.sendlineafter("Share code? ", file_name)
	line = r.recvline()
	if "Data:" in line:
		until = "Choices:"
		out = r.recvuntil(until)
		return out[:-len(until)].rstrip()
	elif "Could not find file" in line:
		log.warning("File not found")
		return None


out = list_files()
flag_match = re.search("(flag_[^.]+)\.txt", out)
flag_basename = flag_match.group(1)

log.info("Flag file: {}".format(flag_basename))

code_b64 = encrypt(flag_basename.replace("_", "X"), "a")
code = base64.b64decode(code_b64)

log.info("Code: {}".format(enhex(code)))

for i in range(256):
	x = code[:4] + chr(i) + code[5:]
	if x == code:
		continue
	out = decrypt(base64.b64encode(x))
	if out is not None:
		log.success("Flag: {}".format(out))
		break
		
```

The output:
```console
root@kali:/media/sf_CTFs/pico/eleCTRic# python exploit.py
[+] Opening connection to 2018shell3.picoctf.com on port 15037: Done
[*] Listing files
[*] Flag file: flag_0443c2f2f396a1393e76
[*] Encrypting file: Name: 'flagX0443c2f2f396a1393e76', Data: 'a'
[*] Base64 Code: G0c3ZCEdVNzsA2/V2goEjktKZzBAHgXf6U4py5w=
[*] Code: 1b473764211d54dcec036fd5da0a048e4b4a6730401e05dfe94e29cb9c
[*] Requesting to decrypt file 'G0c3ZAAdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZAEdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZAIdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZAMdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZAQdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZAUdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZAYdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZAcdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZAgdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZAkdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZAodVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZAsdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZAwdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZA0dVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZA4dVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZA8dVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZBAdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZBEdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZBIdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZBMdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZBQdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZBUdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZBYdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZBcdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZBgdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZBkdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZBodVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZBsdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZBwdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZB0dVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZB4dVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZB8dVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZCAdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZCIdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZCMdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZCQdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZCUdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[!] File not found
[*] Requesting to decrypt file 'G0c3ZCYdVNzsA2/V2goEjktKZzBAHgXf6U4py5w='
[+] Flag: picoCTF{alw4ys_4lways_Always_check_int3grity_9d5e42a2}
[*] Closed connection to 2018shell3.picoctf.com port 15037
```

The flag: picoCTF{alw4ys_4lways_Always_check_int3grity_9d5e42a2}