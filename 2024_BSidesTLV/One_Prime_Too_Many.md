# One Prime Too Many

 * Category: Cryptography
 * 150 Points
 * Solved by the JCTF Team

## Description

> I know how to use RSA, don't you tell me how to use it correctly!

`enc.py`: 

```python
from secret import FLAG
from Crypto.Util.number import getPrime, bytes_to_long, long_to_bytes
import binascii


def gen_key(bits):
    N = getPrime(bits)
    e = 65537
    return N, e


def encrypt_message(message, N, e):
    m_int = bytes_to_long(message.encode('utf-8'))
    ciphertext = pow(m_int, e, N)
    return binascii.hexlify(long_to_bytes(ciphertext))


N, e = gen_key(2048)
ciphertext = encrypt_message(FLAG, N, e)

with open('output.txt', 'w') as f:
    f.write(f"{N}\n")
    f.write(ciphertext.decode())

```

`output.txt`:

```
31335491405627691194571645073554022011187637848422811565193711874036089219766201733393631321180809722445122353375019386535514633342084358153533140066360483668109519529482784540444771058491308098228668740337291845906006681893080694261304774179344466765088521557512296566223510781693084197389577463524432772757719472917428537390788216055072033156904595165691669418172516728325211722655605316872432483220277394091575990922542016261664701633883222851237534301577594792423596079413456023603257915355954635133086680818451618814180307557976583073650205966525507032482902521819398461187399711836158211922450266151746149108457
a6459f8650ef4051bbdcb0b0d0ffdbc855a995562d7fa14e57aab15aa0cc889ee209c3de99ae370617bbee7ec72584cd632d2bb13804ec7acb43a62b65209b57233e9bbc0402108c390ab59a9353c1603a52e6227c36deac8da7f2653371b47cfea729bf921f4d60b363e2d88e25afd1de95fde59e7acc43ca8dfe93ee95d9bc88ca842c6bfd593feed92a3c7fbe918c7244dfac5571b8c0ebb8106c30385ec4ed048a8270b5bc4e2b6d33a965738ef41edf96b7c0a5cc0d91335fd5b965c4e36f7935f9a5d91dcd132a93301b1cc2a48c789211c2bcb85427ea7ca5f78970c4f800f3bb74d916d82500e2ddef032c70a327e904972a308600d73b6e06dcc9e3
```

## Solution

The code in `enc.py` looks like RSA, with one significant difference. While in standard RSA,
$N$ is the product of two prime numbers, in our case, it's a prime number itself.  
This has very dramatic consequences: It transforms the calculation of $\varphi(N)$ from 
extrimely hard to trivial. In fact, if $N$ is prime, then $\varphi(N) = N - 1$.

Once we have $\varphi(N)$, we can use it to calculate $d$ using the standard formula: $d = e^{-1} (mod(\varphi(N)))$. Then, we can proceed to decrypt the message as usual: $m = c^d (mod (N))$.

Script:

```python
import gmpy2
from Crypto.Util.number import long_to_bytes

with open("output.txt") as f:
    n = int(f.readline())
    c = int(f.readline(), 16)
    e = 65537

    phi_n = n - 1
    d = gmpy2.invert(e, phi_n)
    m = pow(c, d, n)
    print(long_to_bytes(m).decode())
```

Output: 

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/One_Prime_Too_Many]
└─$ python3 solve.py
BSidesTLV2024{wh0_g4v3_u_pr1m3_N?}
```