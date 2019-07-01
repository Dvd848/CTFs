# somestufflsimportant
* Category: Web Application
* 500 Points
* Solved by the JCTF Team

## Description
> Can you find it? https://somestufflsimportant.challenges.bsidestlv.com/

## Solution

We access the site and get the following response:

```console
root@kali:/media/sf_CTFs/bsidestlv/somestufflsimportant# curl https://somestufflsimportant.challenges.bsidestlv.com/
The Flag is: BSidesTLV{<strong>1 c</strong><br>
```

This was the only challenge with HTTPS, so we assumed it was related. However, we couldn't find any additional information or clues in the certificate, decrypted traffic or elsewhere. 

The breakthrough came when trying to access the website via TLS1.1:
```console
# curl https://somestufflsimportant.challenges.bsidestlv.com/ --tlsv1.1
The Flag is: BSidesTLV{<strong>3 p</strong><br>
```

After isolating the behavior, it was revealed that each different cipher provides a different part of the flag.

The following script tries all the ciphers OpenSSL has to offer:
```python
import socket, ssl
import http.client
import subprocess
import re

FLAG_RE = re.compile("The Flag is: BSidesTLV{<strong>(\d+) (.+)</strong><br>")
                      

def send_request(cipher):
    res = None
    c   = None
    try:
        context = ssl.create_default_context()
        context.set_ciphers(cipher)
        c = http.client.HTTPSConnection("somestufflsimportant.challenges.bsidestlv.com", context = context)
        c.request("GET", "/")
        response = c.getresponse()
        data = str(response.read())
        match = FLAG_RE.search(data)
        if match:
            res = (match.group(1), match.group(2))
        else:
            print ("\tWarning: The following response did not match the regex: {}".format(data))       
    except:
        pass

    if c is not None:
        c.close()
    print ("[{}] {}".format("V" if res is not None else "X", cipher))
    return res

flag_map = {}
cipher_list = subprocess.check_output(['openssl', 'ciphers']).decode("ascii").strip()

for cipher in cipher_list.split(":"):
    res = send_request(cipher)
    if res is None:
        continue
    index, char = res
    flag_map[int(index) - 1] = char

max_index = max(flag_map)

flag = ["?"] * (max_index + 1)

for k, v in flag_map.items():
    flag[k] = v

print ("BSidesTLV{" + "".join(flag))
```

Running it in Kali with a recent OpenSSL version provided the following result:
```console
root@kali:/media/sf_CTFs/bsidestlv/somestufflsimportant# python3 solve.py
[X] TLS_AES_256_GCM_SHA384
[X] TLS_CHACHA20_POLY1305_SHA256
[X] TLS_AES_128_GCM_SHA256
[X] ECDHE-ECDSA-AES256-GCM-SHA384
[V] ECDHE-RSA-AES256-GCM-SHA384
[X] DHE-RSA-AES256-GCM-SHA384
[X] ECDHE-ECDSA-CHACHA20-POLY1305
[X] ECDHE-RSA-CHACHA20-POLY1305
[X] DHE-RSA-CHACHA20-POLY1305
[X] ECDHE-ECDSA-AES128-GCM-SHA256
[V] ECDHE-RSA-AES128-GCM-SHA256
[X] DHE-RSA-AES128-GCM-SHA256
[X] ECDHE-ECDSA-AES256-SHA384
[V] ECDHE-RSA-AES256-SHA384
[X] DHE-RSA-AES256-SHA256
[X] ECDHE-ECDSA-AES128-SHA256
[V] ECDHE-RSA-AES128-SHA256
[X] DHE-RSA-AES128-SHA256
[X] ECDHE-ECDSA-AES256-SHA
[V] ECDHE-RSA-AES256-SHA
[X] DHE-RSA-AES256-SHA
[X] ECDHE-ECDSA-AES128-SHA
[V] ECDHE-RSA-AES128-SHA
[X] DHE-RSA-AES128-SHA
[X] RSA-PSK-AES256-GCM-SHA384
[X] DHE-PSK-AES256-GCM-SHA384
[X] RSA-PSK-CHACHA20-POLY1305
[X] DHE-PSK-CHACHA20-POLY1305
[X] ECDHE-PSK-CHACHA20-POLY1305
[V] AES256-GCM-SHA384
[X] PSK-AES256-GCM-SHA384
[X] PSK-CHACHA20-POLY1305
[X] RSA-PSK-AES128-GCM-SHA256
[X] DHE-PSK-AES128-GCM-SHA256
[V] AES128-GCM-SHA256
[X] PSK-AES128-GCM-SHA256
[V] AES256-SHA256
[V] AES128-SHA256
[X] ECDHE-PSK-AES256-CBC-SHA384
[X] ECDHE-PSK-AES256-CBC-SHA
[X] SRP-RSA-AES-256-CBC-SHA
[X] SRP-AES-256-CBC-SHA
[X] RSA-PSK-AES256-CBC-SHA384
[X] DHE-PSK-AES256-CBC-SHA384
[X] RSA-PSK-AES256-CBC-SHA
[X] DHE-PSK-AES256-CBC-SHA
[V] AES256-SHA
[X] PSK-AES256-CBC-SHA384
[X] PSK-AES256-CBC-SHA
[X] ECDHE-PSK-AES128-CBC-SHA256
[X] ECDHE-PSK-AES128-CBC-SHA
[X] SRP-RSA-AES-128-CBC-SHA
[X] SRP-AES-128-CBC-SHA
[X] RSA-PSK-AES128-CBC-SHA256
[X] DHE-PSK-AES128-CBC-SHA256
[X] RSA-PSK-AES128-CBC-SHA
[X] DHE-PSK-AES128-CBC-SHA
[V] AES128-SHA
[X] PSK-AES128-CBC-SHA256
[X] PSK-AES128-CBC-SHA
BSidesTLV{cyp???????1mp??tan?????%)*
```

It looks like many weak ciphers were removed from OpenSSL and in order to use them, there is need to recompile the program with a special flag.

Luckily, WSL defaults to an outdated OpenSSL version with all needed ciphers:

```console
$ python3 solve.py
[V] ECDHE-RSA-AES256-GCM-SHA384
[X] ECDHE-ECDSA-AES256-GCM-SHA384
[V] ECDHE-RSA-AES256-SHA384
[X] ECDHE-ECDSA-AES256-SHA384
[V] ECDHE-RSA-AES256-SHA
[X] ECDHE-ECDSA-AES256-SHA
[X] SRP-DSS-AES-256-CBC-SHA
[X] SRP-RSA-AES-256-CBC-SHA
[X] SRP-AES-256-CBC-SHA
[X] DH-DSS-AES256-GCM-SHA384
[X] DHE-DSS-AES256-GCM-SHA384
[X] DH-RSA-AES256-GCM-SHA384
[V] DHE-RSA-AES256-GCM-SHA384
[V] DHE-RSA-AES256-SHA256
[X] DHE-DSS-AES256-SHA256
[X] DH-RSA-AES256-SHA256
[X] DH-DSS-AES256-SHA256
[V] DHE-RSA-AES256-SHA
[X] DHE-DSS-AES256-SHA
[X] DH-RSA-AES256-SHA
[X] DH-DSS-AES256-SHA
[V] DHE-RSA-CAMELLIA256-SHA
[X] DHE-DSS-CAMELLIA256-SHA
[X] DH-RSA-CAMELLIA256-SHA
[X] DH-DSS-CAMELLIA256-SHA
[X] ECDH-RSA-AES256-GCM-SHA384
[X] ECDH-ECDSA-AES256-GCM-SHA384
[X] ECDH-RSA-AES256-SHA384
[X] ECDH-ECDSA-AES256-SHA384
[X] ECDH-RSA-AES256-SHA
[X] ECDH-ECDSA-AES256-SHA
[V] AES256-GCM-SHA384
[V] AES256-SHA256
[V] AES256-SHA
[V] CAMELLIA256-SHA
[X] PSK-AES256-CBC-SHA
[V] ECDHE-RSA-AES128-GCM-SHA256
[X] ECDHE-ECDSA-AES128-GCM-SHA256
[V] ECDHE-RSA-AES128-SHA256
[X] ECDHE-ECDSA-AES128-SHA256
[V] ECDHE-RSA-AES128-SHA
[X] ECDHE-ECDSA-AES128-SHA
[X] SRP-DSS-AES-128-CBC-SHA
[X] SRP-RSA-AES-128-CBC-SHA
[X] SRP-AES-128-CBC-SHA
[X] DH-DSS-AES128-GCM-SHA256
[X] DHE-DSS-AES128-GCM-SHA256
[X] DH-RSA-AES128-GCM-SHA256
[V] DHE-RSA-AES128-GCM-SHA256
[V] DHE-RSA-AES128-SHA256
[X] DHE-DSS-AES128-SHA256
[X] DH-RSA-AES128-SHA256
[X] DH-DSS-AES128-SHA256
[V] DHE-RSA-AES128-SHA
[X] DHE-DSS-AES128-SHA
[X] DH-RSA-AES128-SHA
[X] DH-DSS-AES128-SHA
        Warning: The following response did not match the regex: b'The Flag is: BSidesTLV{'
[X] DHE-RSA-SEED-SHA
[X] DHE-DSS-SEED-SHA
[X] DH-RSA-SEED-SHA
[X] DH-DSS-SEED-SHA
[V] DHE-RSA-CAMELLIA128-SHA
[X] DHE-DSS-CAMELLIA128-SHA
[X] DH-RSA-CAMELLIA128-SHA
[X] DH-DSS-CAMELLIA128-SHA
[X] ECDH-RSA-AES128-GCM-SHA256
[X] ECDH-ECDSA-AES128-GCM-SHA256
[X] ECDH-RSA-AES128-SHA256
[X] ECDH-ECDSA-AES128-SHA256
[X] ECDH-RSA-AES128-SHA
[X] ECDH-ECDSA-AES128-SHA
[V] AES128-GCM-SHA256
[V] AES128-SHA256
[V] AES128-SHA
        Warning: The following response did not match the regex: b'The Flag is: BSidesTLV{'
[X] SEED-SHA
[V] CAMELLIA128-SHA
[X] PSK-AES128-CBC-SHA
[V] ECDHE-RSA-RC4-SHA
[X] ECDHE-ECDSA-RC4-SHA
[X] ECDH-RSA-RC4-SHA
[X] ECDH-ECDSA-RC4-SHA
[V] RC4-SHA
[V] RC4-MD5
[X] PSK-RC4-SHA
[V] ECDHE-RSA-DES-CBC3-SHA
[X] ECDHE-ECDSA-DES-CBC3-SHA
[X] SRP-DSS-3DES-EDE-CBC-SHA
[X] SRP-RSA-3DES-EDE-CBC-SHA
[X] SRP-3DES-EDE-CBC-SHA
[V] EDH-RSA-DES-CBC3-SHA
[X] EDH-DSS-DES-CBC3-SHA
[X] DH-RSA-DES-CBC3-SHA
[X] DH-DSS-DES-CBC3-SHA
[X] ECDH-RSA-DES-CBC3-SHA
[X] ECDH-ECDSA-DES-CBC3-SHA
[V] DES-CBC3-SHA
[X] PSK-3DES-EDE-CBC-SHA
BSidesTLV{cypheR54r31mp0rtanN7!@$%)*+[}
```

the challenge name is `somestufflsimportant` (with `LS Important`) is a hint for `S`ome`S`tuff`L`s(`SSL`)ץ
