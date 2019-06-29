# Secret Service
* Category: Cryptography
* 1500 Points
* Solved by the JCTF Team

## Description
> Hello there,
> 
> In order for us to trust you, you must prove the you either have access to our RSA keys or already have the encrypted command: "FLAGPLX" (flag please thanks, in short).
> 
> Come back to us when you have the flag so we know who we're talking to.
> 
> http://secretservice.challenges.bsidestlv.com/index.php


## Solution

We enter the website and see two APIs:

1. Execute an encrypted command and view the result
2. "Create Demo Commands" - view the encrypted value for a list of several example commands

The demo commands included the following options:
* Number 2
* Number 3
* Number 4
* Number 5
* Number 6
* Number 7
* Number 8
* Number 9
* cat hi.txt
* cat bye.txt
* CANIHAZSOME?

Selecting a demo command returned the encrypted version of it, encoded in Base 64:

```console
root@kali:/media/sf_CTFs/bsidestlv/Secret_Service# curl "http://secretservice.challenges.bsidestlv.com/index.php?action=demo&value=2" && echo ""
NDQ5NTAyMTI4NDQ0NTY2OTAyMjQ4NDA5NjczNjQ5MjY5ODAzNjEyOTkxMTU0NzQ3NDE5NzYxMzI0MjcyOTE1MDM3NjQ1NTUxMTAwNTc=
root@kali:/media/sf_CTFs/bsidestlv/Secret_Service# curl "http://secretservice.challenges.bsidestlv.com/index.php?action=demo&value=cat%20hi.txt" && echo ""
MTc3MzA0MDM4NDk0OTExNzQyODYyOTM3ODIxMDk5MTAwNDU2NjgyNTAxMDY2NjYyODcxNjQzMjQ5MTkxNDI3MDg2MDY3MTQwMjk1ODM=
```

We could later send this to the "execute" API, and see the return value:

```console
root@kali:/media/sf_CTFs/bsidestlv/Secret_Service# curl "http://secretservice.challenges.bsidestlv.com/index.php?action=execute&value=NDQ5NTAyMTI4NDQ0NTY2OTAyMjQ4NDA5NjczNjQ5MjY5ODAzNjEyOTkxMTU0NzQ3NDE5NzYxMzI0MjcyOTE1MDM3NjQ1NTUxMTAwNTc=" && echo ""
2
root@kali:/media/sf_CTFs/bsidestlv/Secret_Service# curl "http://secretservice.challenges.bsidestlv.com/index.php?action=execute&value=MTc3MzA0MDM4NDk0OTExNzQyODYyOTM3ODIxMDk5MTAwNDU2NjgyNTAxMDY2NjYyODcxNjQzMjQ5MTkxNDI3MDg2MDY3MTQwMjk1ODM=" && echo ""
cat hi.txt
```

We need to send the "FLAGPLX" command. Obviously, using the "demo" API in order to receive the encrypted version of this command didn't work.

Let's take a deeper look at the results we've received from the demo API.

```console
root@kali:/media/sf_CTFs/bsidestlv/Secret_Service# curl -s "http://secretservice.challenges.bsidestlv.com/index.php?action=demo&value=2" | base64 -d
44950212844456690224840967364926980361299115474741976132427291503764555110057
```

We get a long number. If we assume that RSA is being used behind the scenes, then it's not too farfetched to assume that this is "`c`",  the result of `m^e (mod n)`.

What happen if we multiply the encrypted version of 2 with itself, and send that as a command?

```python
root@kali:/media/sf_CTFs/bsidestlv/Secret_Service# ptpython
>>> import requests
>>> import base64
>>> URI = "http://secretservice.challenges.bsidestlv.com/index.php"
>>> def get_representation(n):
...     r = requests.get("{}?action=demo&value={}".format(URI, n))
...     return int(base64.b64decode(r.text))
...
... def run_command(cmd):
...     r = requests.get("{}?action=execute&value={}".format(URI, base64.b64encode(str(cmd).rstrip("L"))))
...     return r.text
>>> 
>>> get_representation(2)
44950212844456690224840967364926980361299115474741976132427291503764555110057L
>>> run_command(get_representation(2))
u'2'
>>> run_command(get_representation(2) * get_representation(2))
u'4'
```

We got `4` as a response! 

From the fact that we were able to modify the command to a predictable outcome by multiplying two ciphertexts, it looks like textbook-RSA is being used behind the scenes, which is known to be [malleable](https://en.wikipedia.org/wiki/Malleability_(cryptography)):

> In the RSA cryptosystem, a plaintext m is encrypted as `E(m) = m^e (mod n)` where `(e, n)` is the public key. Given such a ciphertext, an adversary can construct an encryption of `mt` for any `t`, as `E(m)* t^e (mod n) = (mt)^e (mod n) = E(mt)`. For this reason, RSA is commonly used together with padding methods such as OAEP or PKCS1. 


What happens if we raise `2` to the power of `6`?
```python
>>> run_command(get_representation(2) ** 6)
u'@'
```

We got `@`, which is the ASCII value of 2^6. So what is happening here is that our input is eventually interpreted as ASCII and then executed.

Since we want to execute `FLAGPLX`, we first need to translate it to an integer value, which is simply representing each character according to its ASCII value and reading the result as a number. We can use `xxd` for this:

```console
root@kali:/media/sf_CTFs/bsidestlv/Secret_Service# echo -n FLAGPLX | xxd -p
464c4147504c58
root@kali:/media/sf_CTFs/bsidestlv/Secret_Service# echo $((16#464c4147504c58))
19787091622775896
```

BTW, It works the other way too:
```console
root@kali:/media/sf_CTFs/bsidestlv/Secret_Service# printf "%x" 19787091622775896 | xxd -p -r
FLAGPLX
```

Now, how do we get to this value by only using multiplication? Simple, using [factorization](https://en.wikipedia.org/wiki/Factorization) and the [Fundamental theorem of arithmetic](https://en.wikipedia.org/wiki/Fundamental_theorem_of_arithmetic): 

> Every integer greater than 1 either is a prime number itself or can be represented as the product of prime numbers


19787091622775896 can be represented as 2 * 2 * 2 * 83 * 179 * 166479535091. We use the demo API (which conveniently returned a result for any prime number) to get the representation of these building blocks, multiply them, and send the result as our command.

The following script does this:
```python
from Crypto.Util.number import bytes_to_long
import base64
import requests

URI = "http://secretservice.challenges.bsidestlv.com/index.php"

def get_prime_factors(n):
    r = requests.get("http://factordb.com/api?query={}".format(n))
    j = r.json()
    assert(int(j["id"]) == n)
    assert(j["status"] == "FF")
    return j["factors"]

def get_representation(n):
    r = requests.get("{}?action=demo&value={}".format(URI, n))
    return int(base64.b64decode(r.text))

def run_command(cmd):
    r = requests.get("{}?action=execute&value={}".format(URI, base64.b64encode(str(cmd).rstrip("L"))))
    return r.text

plaintext = bytes_to_long("FLAGPLX")
print "Plaintext representation: {}".format(plaintext)

factors = get_prime_factors(plaintext)
print "Factors: {}".format(factors)

cmd = 1
for x, times in factors:
    cmd *= get_representation(x)**times

print run_command(cmd)
```

The result:
```console
root@kali:/media/sf_CTFs/bsidestlv/Secret_Service# python solve.py
Plaintext representation: 19787091622775896
Factors: [[u'2', 3], [u'83', 1], [u'179', 1], [u'166479535091', 1]]
congrats! :) your flag is BsidesTLV{iloveyou!nohomo(morphic)}
```

