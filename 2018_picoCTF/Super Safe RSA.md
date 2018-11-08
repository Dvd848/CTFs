# Super Safe RSA
Cryptography, 350 points

## Description:
> Dr. Xernon made the mistake of rolling his own crypto.. Can you find the bug and decrypt the message? 



## Solution:

After connecting to the server, we receive a response similar to the the following:
```console
root@kali:/media/sf_CTFs/pico/Super_Safe_RSA# nc 2018shell3.picoctf.com 1317
c: 12654858941499530461652100979885023413853686976170397252318656231551885813737242
n: 14388047625825038792581806368797782213408602208252281782268581158721542328850069
e: 65537
```

The n value seems pretty small, and it turns out we can factorize it using a service such as [FactorDB](http://factordb.com/).

FactorDB might take a while when attempting to factorize new numbers, so we need to poll the service after the initial request.

When we receive the result (p and q), we can easily decrypt the message.

The script:
```python
import gmpy2
from factordb.factordb import FactorDB
from pwn import *
import time
import requests

r = remote("2018shell3.picoctf.com", 1317)
output = r.recvall()
params = {}
for line in output.rstrip().split("\n"):
    param, value = line.split(": ")
    log.info("{}: {}".format(param, value))
    params[param] = int(value.rstrip())

log.info("Connecting to FactorDB")

# It looks like in some cases, the FactorDB won't factorize a number unless
# we first access the web API.
requests.get("http://factordb.com/index.php?query={}".format(params["n"]))

status = ''
while status != 'FF':
    f = FactorDB(params["n"])
    f.connect()
    factor_list = f.get_factor_list()
    status = f.get_status()
    log.info("Received from FactorDB: {} (status: {})".format(factor_list, status))
    time.sleep(15)

p, q = factor_list
log.info("p: {}".format(p))
log.info("q: {}".format(q))
assert(p * q == params["n"])

ph = (p-1)*(q-1)
log.info("Phi: {}".format(ph))
d = gmpy2.invert(params["e"], ph)
log.info("d: {}".format(d))

plaintext = pow(params["c"], d, params["n"])
log.success("Flag: {}".format(format(plaintext, 'x').decode("hex")))
```

The output:
```console
root@kali:/media/sf_CTFs/pico/Super_Safe_RSA# python solve.py
[+] Opening connection to 2018shell3.picoctf.com on port 1317: Done
[+] Receiving all data: Done (177B)
[*] Closed connection to 2018shell3.picoctf.com port 1317
[*] c: 11583360663584966291640415469545456678903953436240956253019470111711054578133277
[*] n: 15293141044389338319706248927379117212423193045531636442312081420698010099164067
[*] e: 65537
[*] Connecting to FactorDB
[*] Received from FactorDB: [15293141044389338319706248927379117212423193045531636442312081420698010099164067L] (status: C)
[*] Received from FactorDB: [15293141044389338319706248927379117212423193045531636442312081420698010099164067L] (status: C)
[*] Received from FactorDB: [15293141044389338319706248927379117212423193045531636442312081420698010099164067L] (status: C)
[*] Received from FactorDB: [15293141044389338319706248927379117212423193045531636442312081420698010099164067L] (status: C)
[*] Received from FactorDB: [15293141044389338319706248927379117212423193045531636442312081420698010099164067L] (status: C)
[*] Received from FactorDB: [15293141044389338319706248927379117212423193045531636442312081420698010099164067L] (status: C)
[*] Received from FactorDB: [15293141044389338319706248927379117212423193045531636442312081420698010099164067L] (status: C)
[*] Received from FactorDB: [15293141044389338319706248927379117212423193045531636442312081420698010099164067L] (status: C)
[*] Received from FactorDB: [93623660453440872539167520505354536801L, 163346967746413104744521163324467660078467L] (status: FF)
[*] p: 93623660453440872539167520505354536801
[*] q: 163346967746413104744521163324467660078467
[*] Phi: 15293141044389338319706248927379117212259752454124769896695021089853037084548800
[*] d: 11261998155001270213553610719641896255521006041487551226853458471374754349062273
[+] Flag: picoCTF{us3_l@rg3r_pr1m3$_7542}
```

The flag: picoCTF{us3_l@rg3r_pr1m3$_7542}