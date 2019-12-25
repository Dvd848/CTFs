# BackHash
Crypto, 10 points

## Description
> We found this thingy - we dont know what it does.

## Solution

We enter the provided link and see the following form:
```html
<p style="font-size: 175%;">BackHash</p>
<p>The yeet replaces all occurences of f1a9 with the flag</p>
<p id="out"></p>
<input id="input" placeholder="Input">
<button onclick="gen()">Result</button>
```

Entering a value and clicking the button results in a request being sent to the server:
```console
root@kali:/media/sf_CTFs/kaf/BackHash# curl 'http://ctf.kaf.sh:1070/scripts/backend/backhash/backhash.php' -H 'Content-Type: application/x-www-form-urlencoded' --data-binary 'backhash={"action":"generate","p
arameters":{"input":"test"}}' && echo
{"backhash":{"status":{"generate":true},"result":{"generate":"68358d5d9cbbf39fe571ba41f26524b6"}}}
```

In the example above, we sent `test` and received a response of `68358d5d9cbbf39fe571ba41f26524b6`. Searching for this response online, we find out that it is the MD5 of the SHA1 of `test`:
```console
root@kali:/media/sf_CTFs/kaf/BackHash# echo -n "test" | sha1sum | awk '{ printf $1 }' | md5sum
68358d5d9cbbf39fe571ba41f26524b6  -
```

So it looks like we have to find a string `s` such that `MD5(SHA1(s)).contains('f1a9')`.

The following script does that:
```python
import hashlib

target = 'f1a9'
candidate = 0
while True:
    plaintext = str(candidate)
    hash = hashlib.sha1(plaintext.encode('ascii')).hexdigest()
    hash = hashlib.md5(hash.encode('ascii')).hexdigest()
    if target in hash:
        print('plaintext: "' + plaintext + '", hash: ' + hash)
        break
    candidate = candidate + 1
```

Output:
```console
root@kali:/media/sf_CTFs/kaf/BackHash# python solve.py
plaintext: "2286", hash: 904127e6901da6f1a9cdd436701a253f
```

We send `2286` to the server and get back the flag:
```console
root@kali:/media/sf_CTFs/kaf/BackHash# curl 'http://ctf.kaf.sh:1070/scripts/backend/backhash/backhash.php' -H 'Content-Type: application/x-www-form-urlencoded' --data-binary 'backhash={"action":"generate","parameters":{"input":"2286"}}' && echo
{"backhash":{"status":{"generate":true},"result":{"generate":"904127e6901da6KAF{Dn4k_f1a9z___much_f1a9_l0t5_h4ppy}cdd436701a253f"}}}
```

The flag: `KAF{Dn4k_f1a9z___much_f1a9_l0t5_h4ppy}`

By the way, the server also accepted a string who's MD5 contained `f1a9` directly:
```console
root@kali:/media/sf_CTFs/kaf/BackHash# echo -n "812" | md5sum
81e74d678581a3bb7a720b019f4f1a93  -
root@kali:/media/sf_CTFs/kaf/BackHash# curl 'http://ctf.kaf.sh:1070/scripts/backend/backhash/backhash.php' -H 'Content-Type: application/x-www-form-urlencoded' --data-binary 'backhash={"action":"generate","parameters":{"input":"812"}}' && echo
{"backhash":{"status":{"generate":true},"result":{"generate":"81e74d678581a3bb7a720b019f4KAF{Dn4k_f1a9z___much_f1a9_l0t5_h4ppy}3"}}}
```