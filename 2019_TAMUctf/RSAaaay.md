# RSAaaay
Crypto, 354 points

## Description:

> Hey, you're a hacker, right? I think I am too, look at what I made!
> 
> (2531257, 43)
> 
> My super secret message: 906851 991083 1780304 2380434 438490 356019 921472 822283 817856 556932 2102538 2501908 2211404 991083 1562919 38268
> 
> Problem is, I don't remember how to decrypt it... could you help me out?


## Solution:

The description mentions RSA, and we get a number pair, so the trivial interpretation is as a public key: (`N`, `e`).

However, `N` is usually the product of two very large prime numbers (`p`, `q`), making it hard to factorize. In our case, `N` is very small, and a simple brute-force algorithm can factorize it into two prime numbers:

```python
>>> def get_prime_factors(n):
...     # https://stackoverflow.com/a/22808285/10957683
...     i = 2
...     factors = []
...     while i * i <= n:
...         if n % i:
...             i += 1
...         else:
...             n //= i
...             factors.append(i)
...     if n > 1:
...         factors.append(n)
...     return factors
...
>>> get_prime_factors(2531257)
[509, 4973]
```

Now that we have `p` and `q`, we can use them to try and decrypt the ciphertext (a.k.a. `c`). It's not very clear what `c` is, though. Is it one long number with spaces in the middle, or several different ciphertexts encrypted with the same key? Since the number of digits isn't identical for all different groups of digits, we'll start with the assumption that these are different ciphertexts.

In order to perform the decryption, we'll use the following Python function:

```python
def rsa_decrypt(p, q, e, c):
    n = p * q
    ph = (p - 1) * (q - 1)
    d = gmpy2.invert(e, ph)
    plaintext = pow(c, d, n)
    return int(plaintext)
```

Calling it on `c[0]`:

```python
>>> rsa_decrypt(509, 4973, 43, 906851)
103
```

That's a really good start, since `chr(103) = 'g'`, and we know that all flags start with a `gigem` prefix. Let's move on to `c[1]`:

```python
>>> rsa_decrypt(509, 4973, 43, 991083)
105103
```

That's a bit unexpected. We were expecting `105` (i.e. `ord('i')`) and then another `103`, but we got them both from one decryption. Moving on to the next ciphertext produces the same behavior:

```python
>>> rsa_decrypt(509, 4973, 43, 1780304)
101109
```

If we read that as `(101, 109)`, we indeed get `(e, m)`. The conclusion is that we might need to "crack" some results into two in order to extract two ASCII letters.

The following script performs this end-to-end:

```python
import gmpy2
import string

def get_prime_factors(n):
    # https://stackoverflow.com/a/22808285/10957683
    i = 2
    factors = []
    while i * i <= n:
        if n % i:
            i += 1
        else:
            n //= i
            factors.append(i)
    if n > 1:
        factors.append(n)
    return factors

def rsa_decrypt(p, q, e, c):
    n = p * q
    ph = (p - 1) * (q - 1)
    d = gmpy2.invert(e, ph)
    plaintext = pow(c, d, n)
    return int(plaintext)

cipher = "906851 991083 1780304 2380434 438490 356019 921472 822283 817856 556932 2102538 2501908 2211404 991083 1562919 38268"

def is_printable(s):
    return all(c in string.ascii_letters + string.digits + string.punctuation for c in s)

def crack(s):
    """
    Crack a string representation of a number into one or two ASCII letters, assuming possible.
    e.g.: 
    "105103" -> "ig"
    "103"    -> "g"
    """
    attempts = []
    for i in range(len(s)):
        try:
            attempts.append((chr(int(s[:i])) if i > 0 else "", chr(int(s[i:]))))
        except ValueError:
            pass
    
    good_attempts = []
    for (a, b) in attempts:
        if is_printable(a+b):
            good_attempts.append(a+b)
    assert(len(good_attempts) == 1)
    print ("{}\t-> '{}'".format(s, good_attempts[0]))
    return good_attempts[0]

N = 2531257
E = 43

prime_factors = get_prime_factors(N)
assert(len(prime_factors) == 2)
(p, q) = prime_factors
    
res = ""
for c in cipher.split(" "):
    res += crack(str(rsa_decrypt(   p = p,
                                    q = q,
                                    e = E,
                                    c = int(c))))

print (res)
```

Output:

```console
root@kali:/media/sf_CTFs/tamu/RSAaaay# python solve.py
103     -> 'g'
105103  -> 'ig'
101109  -> 'em'
12383   -> '{S'
97118   -> 'av'
97103   -> 'ag'
10195   -> 'e_'
83105   -> 'Si'
12095   -> 'x_'
70108   -> 'Fl'
121105  -> 'yi'
110103  -> 'ng'
9584    -> '_T'
105103  -> 'ig'
101114  -> 'er'
115125  -> 's}'
gigem{Savage_Six_Flying_Tigers}
```