# small
Crypto, Easy

## Description

> When things are small you have to be carefull!

```
message = int('REDACTED', base=35)
N = 31882864753733457706900355195561745936205728163688545344755624355885302677527509480805991969514641856022311950710014654686332759895303124949904557581766107448945073828773339824936328117599459705430379854436444155104737774883908742430619368768337640156577480749932446289330171110268995901030116001751822218657
c = message^3 % N
# c = 272712645051843502864020676686837219546440933810920336253597504130258033336636323130656292878088405243095416128

The message is the flag. No flag format.
```

## Solution

This looks like RSA with a small exponent. This is similar to the picoCTF [miniRSA](/2019_picoCTF/miniRSA.md) challenge. 

We find the cube root of `c` and decode it as base35 (since the message itself was encoded in base35).

```python
import gmpy2

n = 31882864753733457706900355195561745936205728163688545344755624355885302677527509480805991969514641856022311950710014654686332759895303124949904557581766107448945073828773339824936328117599459705430379854436444155104737774883908742430619368768337640156577480749932446289330171110268995901030116001751822218657
e = 3
cipher_str = 272712645051843502864020676686837219546440933810920336253597504130258033336636323130656292878088405243095416128 


gs = gmpy2.mpz(cipher_str)
gm = gmpy2.mpz(n)
ge = gmpy2.mpz(e)

root, exact = gmpy2.iroot(gs, ge)
print root

def base35encode(integer):
    """
    Convert from Base10 to Base35.
    Based on https://en.wikipedia.org/wiki/Base36
    """
    chars = '0123456789abcdefghijklmnopqrstuvwxy'

    sign = '-' if integer < 0 else ''
    integer = abs(integer)
    result = ''

    while integer > 0:
        integer, remainder = divmod(integer, 35)
        result = chars[remainder] + result

    return sign + result

print base35encode(root)
```

Output:
```console
root@kali:/media/sf_CTFs/36c3/small# python 1.py
6484877229948717415163579969767084212
juniorissmallkuchenblech
```