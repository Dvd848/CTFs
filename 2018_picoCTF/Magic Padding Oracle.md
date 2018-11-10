# Magic Padding Oracle
Cryptography, 450 points

## Description:
> Can you help us retreive the flag from this crypto service?

```python
#!/usr/bin/python2
import os
import json
import sys
import time

from Crypto.Cipher import AES

cookiefile = open("cookie", "r").read().strip()
flag = open("flag", "r").read().strip()
key = open("key", "r").read().strip()

welcome = """
Welcome to Secure Encryption Service version 1.51
"""
def pad(s):
  return s + (16 - len(s) % 16) * chr(16 - len(s) % 16)

def isvalidpad(s):
  return ord(s[-1])*s[-1:]==s[-ord(s[-1]):]

def unpad(s):
  return s[:-ord(s[len(s)-1:])]

def encrypt(m):
  IV="This is an IV456"
  cipher = AES.new(key.decode('hex'), AES.MODE_CBC, IV)
  return IV.encode("hex")+cipher.encrypt(pad(m)).encode("hex")

def decrypt(m):
  cipher = AES.new(key.decode('hex'), AES.MODE_CBC, m[0:32].decode("hex"))
  return cipher.decrypt(m[32:].decode("hex"))
  

# flush output immediately
sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)
print welcome
print "Here is a sample cookie: " + encrypt(cookiefile)

# Get their cookie
print "What is your cookie?"
cookie2 = sys.stdin.readline()
# decrypt, but remove the trailing newline first
cookie2decoded = decrypt(cookie2[:-1])

if isvalidpad(cookie2decoded):
   d=json.loads(unpad(cookie2decoded))
   print "username: " + d["username"]
   print "Admin? " + d["is_admin"]
   exptime=time.strptime(d["expires"],"%Y-%m-%d")
   if exptime > time.localtime():
      print "Cookie is not expired"
   else:
      print "Cookie is expired"
   if d["is_admin"]=="true" and exptime > time.localtime():
      print "The flag is: " + flag
else:
   print "invalid padding"
```

## Solution:

The service accepts a cookie encrypted with AES-CBC and if the cookie is valid, outputs the flag in case the cookie belongs to an admin.

However, the service has a critical flaw - given an encrypted input, it will tell the user if the input's padding is correct or not. This allows executing a [Padding oracle attack on CBC encryption](https://en.wikipedia.org/wiki/Padding_oracle_attack#Padding_oracle_attack_on_CBC_encryption).

We'll use [python-paddingoracle](https://github.com/mwielgoszewski/python-paddingoracle) to execute the attack. The library just needs us to implement the transport layer, and it does the rest.


The script:
```python
from pwn import *
from paddingoracle import BadPaddingException, PaddingOracle
import json
from Crypto.Cipher import AES

class PadBuster(PaddingOracle):
    def oracle(self, data):
        while True:
            try:
                r = remote("2018shell3.picoctf.com", 24933)
                r.recvuntil("cookie?")
                s = data
                s = str(data).encode("hex")
                r.sendline(s)
                out = r.recvall()
                if "invalid padding" in out:
                    raise BadPaddingException
                return
            except (socket.error, socket.gaierror, socket.herror, socket.timeout) as e:
                print str(e)

if __name__ == '__main__':
    d = {"username": "User", "is_admin": "true", "expires": "2019-01-01"}
    s = json.dumps(d)
    print s
    padbuster = PadBuster()
    encrypted = padbuster.encrypt(s, block_size=AES.block_size, iv="This is an IV456")

    print "Ciphertext: %r" % (str(encrypted).encode("hex"))
```

The output (truncated, since it runs for a while):
```
...
[+] Receiving all data: Done (17B)
[*] Closed connection to 2018shell3.picoctf.com port 24933
[+] Opening connection to 2018shell3.picoctf.com on port 24933: Done
[+] Receiving all data: Done (17B)
[*] Closed connection to 2018shell3.picoctf.com port 24933
[+] Opening connection to 2018shell3.picoctf.com on port 24933: Done
[+] Receiving all data: Done (17B)
[*] Closed connection to 2018shell3.picoctf.com port 24933
[+] Opening connection to 2018shell3.picoctf.com on port 24933: Done
[+] Receiving all data: Done (17B)
[*] Closed connection to 2018shell3.picoctf.com port 24933
[+] Opening connection to 2018shell3.picoctf.com on port 24933: Done
[+] Receiving all data: Done (17B)
[*] Closed connection to 2018shell3.picoctf.com port 24933
[+] Opening connection to 2018shell3.picoctf.com on port 24933: Done
[+] Receiving all data: Done (573B)
[*] Closed connection to 2018shell3.picoctf.com port 24933
Ciphertext: '2508f4cf9a524e46de043391bd3ef75791a6df158072e5132d2980324e2319012736e01a6829fa90e30039723a759a65f99065ef86e9cd5916a8c36992de3c553d89e4437a9c271a079ee34a352f49e95468697320697320616e204956343536'
```


Submitting the cookie:
```console
root@kali:/media/sf_CTFs/pico/Magic_Padding_Oracle# nc 2018shell3.picoctf.com 24933

Welcome to Secure Encryption Service version 1.51

Here is a sample cookie: 5468697320697320616e204956343536d6ca0a2883280762915414c54e97df1b40871b72f45ec7f9510a080095436d514129e137aaac86a0f7fa8bd3d250b9d1df35b668fcb93f00bb06692560a3fed8a3b523d385f1477b6daac14ff2416c67
What is your cookie?
2508f4cf9a524e46de043391bd3ef75791a6df158072e5132d2980324e2319012736e01a6829fa90e30039723a759a65f99065ef86e9cd5916a8c36992de3c553d89e4437a9c271a079ee34a352f49e95468697320697320616e204956343536
username: User
Admin? true
Cookie is not expired
The flag is: picoCTF{0r4cl3s_c4n_l34k_ae6a1459}
```

The flag: picoCTF{0r4cl3s_c4n_l34k_ae6a1459}