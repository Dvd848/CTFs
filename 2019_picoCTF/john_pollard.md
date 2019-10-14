# john_pollard
Cryptography, 500 points

## Description:
> Sometimes RSA certificates are breakable

Hints:

* The flag is in the format picoCTF{p,q}
* Try swapping p and q if it does not work

## Solution: 

Let's extract the public key from the attached certificate:

```console
root@kali:/media/sf_CTFs/pico/john_pollard# openssl x509 -pubkey -noout -in cert.pem > key.pub
root@kali:/media/sf_CTFs/pico/john_pollard# openssl rsa -pubin -in key.pub -text
RSA Public-Key: (53 bit)
Modulus: 4966306421059967 (0x11a4d45212b17f)
Exponent: 65537 (0x10001)
writing RSA key
-----BEGIN PUBLIC KEY-----
MCIwDQYJKoZIhvcNAQEBBQADEQAwDgIHEaTUUhKxfwIDAQAB
-----END PUBLIC KEY-----
```

The modulus is really small, we can probably factor it.

We can use the [FactorDB API](http://factordb.com/api/index.php?query=4966306421059967):
```json
{
    "id":       "4966306421059967",
    "status":   "FF",
    "factors":[
        ["67867967", 1],
        ["73176001",1]
    ]
}
```

Or YAFU:
```console
root@kali:/media/sf_CTFs/pico/john_pollard# yafu "factor(4966306421059967)"


fac: factoring 4966306421059967
fac: using pretesting plan: normal
fac: no tune info: using qs/gnfs crossover of 95 digits
div: primes less than 10000
fmt: 1000000 iterations
Total factoring time = 0.0189 seconds


***factors found***

P8 = 73176001
P8 = 67867967

ans = 1
```

The flag: `picoCTF{73176001,67867967}`.