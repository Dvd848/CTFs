# Mind your Ps and Qs
Category: Crypto, 20 points

## Description

> In RSA, a small e value can be problematic, but what about N? Can you decrypt this?

```
Decrypt my super sick RSA:
c: 240986837130071017759137533082982207147971245672412893755780400885108149004760496
n: 831416828080417866340504968188990032810316193533653516022175784399720141076262857
e: 65537
```

## Solution

`n` is small and can be factored into `p` and `q`:

```python
from factordb.factordb import FactorDB
import gmpy2

c = 240986837130071017759137533082982207147971245672412893755780400885108149004760496
n = 831416828080417866340504968188990032810316193533653516022175784399720141076262857
e = 65537

f = FactorDB(n)
f.connect()
p, q = f.get_factor_list()
ph = (p-1)*(q-1)
d = gmpy2.invert(e, ph)
plaintext = pow(c, d, n)
print("Flag: {}".format(bytearray.fromhex(format(plaintext, 'x')).decode()))
```

Output:
```console
┌──(user@kali)-[/media/sf_CTFs/pico/Mind_your_Ps_and_Qs]
└─$ python3 solve.py
Flag: picoCTF{sma11_N_n0_g0od_23540368}
```

The flag: `picoCTF{sma11_N_n0_g0od_23540368}`