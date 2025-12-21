# Redacted RSA 2

## Description

> I have a RSA private key again, but it is partially redacted. Can you recover that?

The following key was provided:

```
-----BEGIN RSA PRIVATE KEY-----
MIIJJwIBAAKCAgEAhbIueAz0eErBWcRu9aNkArpuAwtysp3egIzD+dqGpSLWPIm+
HLP8gIBK2wAVXXgqLYqq4ozUslmxhy1u0BfPzla/gmj0AZAEL1DtGiMenKU5a16J
oP2pkCcXpK6BV3alEg97yGU4TN3Fj9RysLlQGUC+xF37iit4YlFPk3TFlJK3TWMe
IAj21MdDh00XzP1r198l8NZSXEuCuKaM16m40H93QQpnVnW/Feo3rq6t/SS0dmbz
RiTTdF9kFBlTQToc8PnQOmdBgy+AdM891lLgQUygjcOYGPFfpEwlDtjTH7e0KiMX
j8i7fMy6p2hvon6JSJ+p7jyBidj7hveEMDFe2EHWLbHkfAze5pX3qlXmXzD4lEyQ
zwrRFIVIrWWiuQyCo5ivNGNAyepfl7BMNKmBc4roE83pNch02q6oU5Fl2VFiGJZQ
cImPO6Uy60/NFbUozAdllwuw/WywlKemno9mxPJSfrB+Z5n5NZbU5/kHye/v8gh3
+uUGbEfaJZlP4TqE/QlS2AVaojju5Uwdkbjzn+Rixzs4dzQYuRU93Qd/Zu5rLeUv
ftgCjRO9bIFJ2Cr7dwKPuwetwxDBGR8dgd0xorodajEKCvm5CRC/7MM4igsALTUP
ptjcpSihDp6NzvcNJa8wmKC4wB00Rvj4NJscd4GQckEK9OSLUMxtGlDVBj8CAwEA
AQ**************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****AoIBAH7MSLvBg7UmdC0mCMuRBVO+V6/9yWbBsSgMDteqgJrSPRGlAx8hp5Lg
+XNlZ/b78AoSYRGkt7A1y0t63hXWnZtLLtnb9+vhlV7kei1TPEIxqOMDryFeGGJx
ev3pOr5ghjdFp2jqO/PCLRmQ2OY3eKyqy+W4v9hy9lkasZec/1AI9X/TrLIYcjgI
Mav1kTyjuZiN5xM2h1mnResjl8B0m4nEokZZ+WckE3OsqpCYf9Y6eXFnKnInkZ92
ygwtNYCFAzIfs6YonY6Fm6DtYiei+1MtC/CXhmUbsA3zLJZAgA6JdIiJaV2O5jPJ
ApxJ1OpLoMQ/10knSR76EJBhK2I8LHs*********************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
************************************************************
-----END RSA PRIVATE KEY-----
```

In addition, a ciphertext was provided.

## Solution

This is the second challenge in the "Redacted RSA" series.

Reminder: When we decode an RSA private key in PEM format as ASN.1, we get a structured sequence of 
integers representing different components of the RSA key. 
The ASN.1 structure typically looks like this:

```sql
RSAPrivateKey ::= SEQUENCE {
    version           Version,
    modulus           INTEGER,  -- n
    publicExponent    INTEGER,  -- e
    privateExponent   INTEGER,  -- d
    prime1            INTEGER,  -- p
    prime2            INTEGER,  -- q
    exponent1         INTEGER,  -- d mod (p-1)
    exponent2         INTEGER,  -- d mod (q-1)
    coefficient       INTEGER,  -- (inverse of q) mod p
    otherPrimeInfos   OtherPrimeInfos OPTIONAL
}
```

This time, the unredacted values are split across two locations.

The first part is easy. As before, we can decode the key content as Base64 and push the result to an ASN.1 parser to see what we
have:

```
SEQUENCE
  INTEGER 00
  INTEGER 0085b22e780cf4784ac159c46ef5a36402ba6e030b72b29dde808cc3f9da86a522d63c89be1cb3fc80804adb00155d782a2d8aaae28cd4b259b1872d6ed017cfce56bf8268f40190042f50ed1a231e9ca5396b5e89a0fda9902717a4ae815776a5120f7bc865384cddc58fd472b0b9501940bec45dfb8a2b7862514f9374c59492b74d631e2008f6d4c743874d17ccfd6bd7df25f0d6525c4b82b8a68cd7a9b8d07f77410a675675bf15ea37aeaeadfd24b47666f34624d3745f64141953413a1cf0f9d03a6741832f8074cf3dd652e0414ca08dc39818f15fa44c250ed8d31fb7b42a23178fc8bb7cccbaa7686fa27e89489fa9ee3c8189d8fb86f78430315ed841d62db1e47c0cdee695f7aa55e65f30f8944c90cf0ad1148548ad65a2b90c82a398af346340c9ea5f97b04c34a981738ae813cde935c874daaea8539165d9516218965070898f3ba532eb4fcd15b528cc0765970bb0fd6cb094a7a69e8f66c4f2527eb07e6799f93596d4e7f907c9efeff20877fae5066c47da25994fe13a84fd0952d8055aa238eee54c1d91b8f39fe462c73b38773418b9153ddd077f66ee6b2de52f7ed8028d13bd6c8149d82afb77028fbb07adc310c1191f1d81dd31a2ba1d6a310a0af9b90910bfecc3388a0b002d350fa6d8dca528a10e9e8dcef70d25af3098a0b8c01d3446f8f8349b1c77819072410af4e48b50cc6d1a50d5063f
  INTEGER 010001
```

Based on the expected structure, this is the version, `n` and `e`. But what is the second part?

Judging by the location of the second unredacted text, and using the key from part 1 as reference for the location, it's
safe to assume that we're talking about `exponent2` here. Let's decode it as ASN.1, and get:

```
INTEGER 7ecc48bbc183b526742d2608cb910553be57affdc966c1b1280c0ed7aa809ad23d11a5031f21a792e0f9736567f6fbf00a126111a4b7b035cb4b7ade15d69d9b4b2ed9dbf7ebe1955ee47a2d533c4231a8e303af215e1862717afde93abe60863745a768ea3bf3c22d1990d8e63778acaacbe5b8bfd872f6591ab1979cff5008f57fd3acb21872380831abf5913ca3b9988de713368759a745eb2397c0749b89c4a24659f967241373acaa90987fd63a7971672a7227919f76ca0c2d35808503321fb3a6289d8e859ba0ed6227a2fb532d0bf09786651bb00df32c9640800e89748889695d8ee633c9029c49d4ea4ba0c43fd74927491efa1090612b623c2c7b
```

So, we have `n`, `e`, and `exponent2`. How do we recover the original key?

If we know $d_q$ (or $d_p$), we can potentially recover the factors $p$ and $q$ of $n$, thereby breaking RSA. This is based on the fact that since $d_q \cdot e \equiv 1 \mod (q-1)$, we know that $d_q \cdot e - 1$ is a multiple of $q - 1$. Therefore, we can iterate over divisors of $d_q \cdot e - 1$, and for each divisor $i$, we can compute a possible value for $q = \frac{d_q \cdot e - 1}{i} + 1$. Then, for this $q$ candidate, we calculate $p = \frac{n}{q}$ and check if this yields integer values for both $p$ and $q$. In such a case, the correct factors have been found. (Credit: Based on [this script](https://github.com/yifeng-lee/RSA-In-CTF/blob/master/expa.py)).

The following script implements the attack and creates a full RSA key:

```python
import gmpy2
from Crypto.PublicKey import RSA

e = 0x10001
n = 0x0085b22e780cf4784ac159c46ef5a36402ba6e030b72b29dde808cc3f9da86a522d63c89be1cb3fc80804adb00155d782a2d8aaae28cd4b259b1872d6ed017cfce56bf8268f40190042f50ed1a231e9ca5396b5e89a0fda9902717a4ae815776a5120f7bc865384cddc58fd472b0b9501940bec45dfb8a2b7862514f9374c59492b74d631e2008f6d4c743874d17ccfd6bd7df25f0d6525c4b82b8a68cd7a9b8d07f77410a675675bf15ea37aeaeadfd24b47666f34624d3745f64141953413a1cf0f9d03a6741832f8074cf3dd652e0414ca08dc39818f15fa44c250ed8d31fb7b42a23178fc8bb7cccbaa7686fa27e89489fa9ee3c8189d8fb86f78430315ed841d62db1e47c0cdee695f7aa55e65f30f8944c90cf0ad1148548ad65a2b90c82a398af346340c9ea5f97b04c34a981738ae813cde935c874daaea8539165d9516218965070898f3ba532eb4fcd15b528cc0765970bb0fd6cb094a7a69e8f66c4f2527eb07e6799f93596d4e7f907c9efeff20877fae5066c47da25994fe13a84fd0952d8055aa238eee54c1d91b8f39fe462c73b38773418b9153ddd077f66ee6b2de52f7ed8028d13bd6c8149d82afb77028fbb07adc310c1191f1d81dd31a2ba1d6a310a0af9b90910bfecc3388a0b002d350fa6d8dca528a10e9e8dcef70d25af3098a0b8c01d3446f8f8349b1c77819072410af4e48b50cc6d1a50d5063f
dq = 0x7ecc48bbc183b526742d2608cb910553be57affdc966c1b1280c0ed7aa809ad23d11a5031f21a792e0f9736567f6fbf00a126111a4b7b035cb4b7ade15d69d9b4b2ed9dbf7ebe1955ee47a2d533c4231a8e303af215e1862717afde93abe60863745a768ea3bf3c22d1990d8e63778acaacbe5b8bfd872f6591ab1979cff5008f57fd3acb21872380831abf5913ca3b9988de713368759a745eb2397c0749b89c4a24659f967241373acaa90987fd63a7971672a7227919f76ca0c2d35808503321fb3a6289d8e859ba0ed6227a2fb532d0bf09786651bb00df32c9640800e89748889695d8ee633c9029c49d4ea4ba0c43fd74927491efa1090612b623c2c7b

for i in range(1, e + 1):
    if (dq * e - 1) % i == 0:
        q = ( (dq * e - 1) // i) + 1
        if n % q == 0:
            p = n // q
            phi = (q - 1) * (p - 1)
            d = gmpy2.invert(e, phi) % phi
            print(f"Found d: {d}")
            key = RSA.construct((n, e, int(d)))
            with open('private_key.pem', 'wb') as f:
                f.write(key.export_key('PEM'))
            break
```

Now we can use the key to decode the ciphertext:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Redacted_RSA_2]
└─$ openssl pkeyutl -decrypt -inkey private_key.pem -in encrypted.txt -out -
CTF{rsa_attacks_usually_involves_modulo_arithmetic}
```

