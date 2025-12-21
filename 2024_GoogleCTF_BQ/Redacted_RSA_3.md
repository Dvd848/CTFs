# Redacted RSA 3

## Description

> I have a RSA private key again and again, but it is partially redacted. Can you recover that?

The following key was provided:

```
-----BEGIN RSA PRIVATE KEY-----
MIIJKAIBAAKCAgEAoovj1+p3c8duZu7Bq91nblqfR973/Sbtkcsq22GxTJsFYfS8
LpVTgYX67Mu94Jvezivo6UsNNV3J+8tqqDHjylgfhLq1KNgCGjASTrPv8w17ogvU
0mcYbBdnBQUgX6HgmfnfDoNROcN+M5xTbGxWsmJPbLzqyS991Mt30ysNyqX3Xu+m
IcMndkDUeUpwOK86Ane6WLgmYS2rxjyoiyKB6uASZkKVoUlxNO/L089j/eatStc+
2FLOI1/+IbJf32lYO+hP/13vJTtsynjeFAobIHasJuU+mxdAqN1iGYCXSv9CzL8Z
zkjybDujXY0P4kAh285q04jqNxpmS1XWTOxnPgz747eyE7w7SMcFsQQzMRu7+Fmm
x8rknAp8gG1W4A6Grh8xoRq1N4kgeF/L8VT7LG+z1o1o+uMj6CDuS1F2LKWt9U8A
ZgWAALX2LU19irgE3xTy7tEHFumTMRMu3kw/dy7lHdOf5L1K0DZu7KIvEyOXcd5K
OTyo1whgizctSXNuIEjMZsDcDreYu3XW+KQq+7i5BbANyRaWETiJi0Vj/5MPYgB/
yHq9VPBwt66aXmYTRX3ZzxU/GGXbCsjlVVv2Z4bofFc+P/nktBKQ7ctLUWJaarM5
IHqFhiwEX+07LylushSguCcp98ch3XbZRbW5nzIuJeqE4K1pb1g4eyq1FSsCAwEA
AQKCAgA2********************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
****************************************************************
************************************************QQKCAQEAvp0qAvuY
Fqbfz7SS9eOEvt0Av49SBiGyJ+07JPRI3RNpV2DfpQtjpWjz7yWQ35GR9eyWb4n+
McUezGVZGx3438lsEF5ox5fiYCD9lea2p5OL/sHLTD6k8WSpsHJVObP/lB+HiW+4
0jbg+f3V8CyOwm7tEhdGWuQCli0PGrbp8SMGQSC89/NynJRGw48fARJh71u5/pUK
RXONYqHRgApBapPpnhjtVg4/tV9LsvE/uK4C6QbJiiMp0v2pg7jsmoDtQdjSz9jx
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
************************************************************
-----END RSA PRIVATE KEY-----
```

In addition, a ciphertext was provided.

## Solution

This is the third challenge in the "Redacted RSA" series.

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

Again, the unredacted values are split across two locations.

The first part is easy. As before, we can decode the key content as Base64 and push the result to an ASN.1 parser to see what we
have:

```
SEQUENCE
  INTEGER 00
  INTEGER 00a28be3d7ea7773c76e66eec1abdd676e5a9f47def7fd26ed91cb2adb61b14c9b0561f4bc2e95538185faeccbbde09bdece2be8e94b0d355dc9fbcb6aa831e3ca581f84bab528d8021a30124eb3eff30d7ba20bd4d267186c17670505205fa1e099f9df0e835139c37e339c536c6c56b2624f6cbceac92f7dd4cb77d32b0dcaa5f75eefa621c3277640d4794a7038af3a0277ba58b826612dabc63ca88b2281eae012664295a1497134efcbd3cf63fde6ad4ad73ed852ce235ffe21b25fdf69583be84fff5def253b6cca78de140a1b2076ac26e53e9b1740a8dd621980974aff42ccbf19ce48f26c3ba35d8d0fe24021dbce6ad388ea371a664b55d64cec673e0cfbe3b7b213bc3b48c705b10433311bbbf859a6c7cae49c0a7c806d56e00e86ae1f31a11ab5378920785fcbf154fb2c6fb3d68d68fae323e820ee4b51762ca5adf54f0066058000b5f62d4d7d8ab804df14f2eed10716e99331132ede4c3f772ee51dd39fe4bd4ad0366eeca22f13239771de4a393ca8d708608b372d49736e2048cc66c0dc0eb798bb75d6f8a42afbb8b905b00dc916961138898b4563ff930f62007fc87abd54f070b7ae9a5e6613457dd9cf153f1865db0ac8e5555bf66786e87c573e3ff9e4b41290edcb4b51625a6ab339207a85862c045fed3b2f296eb214a0b82729f7c721dd76d945b5b99f322e25ea84e0ad696f58387b2ab5152b
  INTEGER 010001
  INTEGER 36
```

Based on the expected structure, this is the version, `n` and `e` (and the very first byte of `d`). But what is the second part?

Judging by the location of the second unredacted text, and using the key from part 1 as reference for the location,
it looks like this is a partial `p` value. Since it's partial, it won't get decoded cleanly by an
ASN.1 parser, but we can decode it using base64 and manually parse the ASN.1:

```
41 02 82 01 01 00 be 9d 2a 02 fb 98 16 a6 df cf b4 92 f5 e3 84 be dd 00 bf 8f 52 06 21 b2 27 ed 3b 24 f4 48 dd 13 69 57 60 df a5 0b 63 a5 68 f3 ef 25 90 df 91 91 f5 ec 96 6f 89 fe 31 c5 1e cc 65 59 1b 1d f8 df c9 6c 10 5e 68 c7 97 e2 60 20 fd 95 e6 b6 a7 93 8b fe c1 cb 4c 3e a4 f1 64 a9 b0 72 55 39 b3 ff 94 1f 87 89 6f b8 d2 36 e0 f9 fd d5 f0 2c 8e c2 6e ed 12 17 46 5a e4 02 96 2d 0f 1a b6 e9 f1 23 06 41 20 bc f7 f3 72 9c 94 46 c3 8f 1f 01 12 61 ef 5b b9 fe 95 0a 45 73 8d 62 a1 d1 80 0a 41 6a 93 e9 9e 18 ed 56 0e 3f b5 5f 4b b2 f1 3f b8 ae 02 e9 06 c9 8a 23 29 d2 fd a9 83 b8 ec 9a 80 ed 41 d8 d2 cf d8 f1
```

That `41` looks like a remainder from the previous integer, based on the fact that the following 4 bytes decode nicely as meaningful ASN.1 in the context:

 * `02`: This is the tag for an integer in ASN.1.
 * `82`: This is the length descriptor, indicating that the length of the integer is specified in the next two bytes.
 * `01 01`: This is the length of the integer in bytes, which is 0x0101 (or 257 in decimal).

The remainder is `199` bytes of `p`. We need to somehow find the remaining `58`. Well, 
clearly there's an attack for that, and I found a reference [here](https://github.com/yifeng-lee/RSA-In-CTF/blob/master/exp6.sage).

```python
e = 0x010001

n = 0x00a28be3d7ea7773c76e66eec1abdd676e5a9f47def7fd26ed91cb2adb61b14c9b0561f4bc2e95538185faeccbbde09bdece2be8e94b0d355dc9fbcb6aa831e3ca581f84bab528d8021a30124eb3eff30d7ba20bd4d267186c17670505205fa1e099f9df0e835139c37e339c536c6c56b2624f6cbceac92f7dd4cb77d32b0dcaa5f75eefa621c3277640d4794a7038af3a0277ba58b826612dabc63ca88b2281eae012664295a1497134efcbd3cf63fde6ad4ad73ed852ce235ffe21b25fdf69583be84fff5def253b6cca78de140a1b2076ac26e53e9b1740a8dd621980974aff42ccbf19ce48f26c3ba35d8d0fe24021dbce6ad388ea371a664b55d64cec673e0cfbe3b7b213bc3b48c705b10433311bbbf859a6c7cae49c0a7c806d56e00e86ae1f31a11ab5378920785fcbf154fb2c6fb3d68d68fae323e820ee4b51762ca5adf54f0066058000b5f62d4d7d8ab804df14f2eed10716e99331132ede4c3f772ee51dd39fe4bd4ad0366eeca22f13239771de4a393ca8d708608b372d49736e2048cc66c0dc0eb798bb75d6f8a42afbb8b905b00dc916961138898b4563ff930f62007fc87abd54f070b7ae9a5e6613457dd9cf153f1865db0ac8e5555bf66786e87c573e3ff9e4b41290edcb4b51625a6ab339207a85862c045fed3b2f296eb214a0b82729f7c721dd76d945b5b99f322e25ea84e0ad696f58387b2ab5152b

p = 0x00be9d2a02fb9816a6dfcfb492f5e384bedd00bf8f520621b227ed3b24f448dd13695760dfa50b63a568f3ef2590df9191f5ec966f89fe31c51ecc65591b1df8dfc96c105e68c797e26020fd95e6b6a7938bfec1cb4c3ea4f164a9b0725539b3ff941f87896fb8d236e0f9fdd5f02c8ec26eed1217465ae402962d0f1ab6e9f123064120bcf7f3729c9446c38f1f011261ef5bb9fe950a45738d62a1d1800a416a93e99e18ed560e3fb55f4bb2f13fb8ae02e906c98a2329d2fda983b8ec9a80ed41d8d2cfd8f100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

beta = 0.5
epsilon = beta^2/7

pbits = p.nbits()
kbits = floor(n.nbits() * (beta ^ 2-epsilon))
pbar = p & (2 ^ pbits - 2 ^ kbits)
print ("Upper %d bits (of %d bits) are given" % (pbits - kbits, pbits))

PR.<x> = PolynomialRing(Zmod(n))
f = x + pbar

print ('p_fake =', p)
x0 = f.small_roots(X = 2 ^ kbits, beta = 0.3)[0]  # find root < 2^kbits with factor >= n^0.3
new_p = x0 + pbar
print ('p =', new_p)
```

Running it with sage, we get:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Redacted_RSA_3]
└─$ sage solve.sage
Upper 1171 bits (of 2048 bits) are given
p_fake = 24062778453696290415414136768741451842043275382807840410564608758848062227118935538773222433315698990327170827569515922985583252489213816561313987007794475960173088683846290840006569044002864887557749732699515233581796881526159507501756641234557287601419268354969902862661130396191269135542380934166105684683197532222186659375904708795398145459649982001412688760701043864789745454471106604702682360689253196202537650393786711843974232198342051485028037261042422149282578408440086965877010457078135127968427674560014477507706296796140704327536945362705684532995492896159563833419018618364405468158699238913681255301120
p = 24062778453696290415414136768741451842043275382807840410564608758848062227118935538773222433315698990327170827569515922985583252489213816561313987007794475960173088683846290840006569044002864887557749732699515233581796881526159507501756641234557287601419268354969902862661130396191269135542380934166105684683197532222186659375904708795398145459649982001412688760701043864789745454471106604702682360689253196202537650393786711843974232198342051485028037261042422149282578408440097530978507893906477121320500646805851674476963605544477598247119258563817229552637496407925091216513002732762160505035028948361887919027809
```

We can now calculate `d` and create a full key using:

```python
>>> import gmpy2
>>> e = 0x010001
>>> n = gmpy2.mpz(0x00a28be3d7ea7773c76e66eec1abdd676e5a9f47def7fd26ed91cb2adb61b14c9b0561f4bc2e95538185faeccbbde09bdece2be8e94b0d355dc9fbcb6aa831e3ca581f84bab528d8021a30124eb3eff30d7ba20bd4d267186c17670505205fa1e099f9df0e835139c37e339c536c6c56b2624f6cbceac92f7dd4cb77d32b0dcaa5f75eefa621c3277640d4794a7038af3a0277ba58b826612dabc63ca88b2281eae012664295a1497134efcbd3cf63fde6ad4ad73ed852ce235ffe21b25fdf69583be84fff5def253b6cca78de140a1b2076ac26e53e9b1740a8dd621980974aff42ccbf19ce48f26c3ba35d8d0fe24021dbce6ad388ea371a664b55d64cec673e0cfbe3b7b213bc3b48c705b10433311bbbf859a6c7cae49c0a7c806d56e00e86ae1f31a11ab5378920785fcbf154fb2c6fb3d68d68fae323e820ee4b51762ca5adf54f0066058000b5f62d4d7d8ab804df14f2eed10716e99331132ede4c3f772ee51dd39fe4bd4ad0366eeca22f13239771de4a393ca8d708608b372d49736e2048cc66c0dc0eb798bb75d6f8a42afbb8b905b00dc916961138898b4563ff930f62007fc87abd54f070b7ae9a5e6613457dd9cf153f1865db0ac8e5555bf66786e87c573e3ff9e4b41290edcb4b51625a6ab339207a85862c045fed3b2f296eb214a0b82729f7c721dd76d945b5b99f322e25ea84e0ad696f58387b2ab5152b)
>>> p = gmpy2.mpz(24062778453696290415414136768741451842043275382807840410564608758848062227118935538773222433315698990327170827569515922985583252489213816561313987007794475960173088683846290840006569044002864887557749732699515233581796881526159507501756641234557287601419268354969902862661130396191269135542380934166105684683197532222186659375904708795398145459649982001412688760701043864789745454471106604702682360689253196202537650393786711843974232198342051485028037261042422149282578408440097530978507893906477121320500646805851674476963605544477598247119258563817229552637496407925091216513002732762160505035028948361887919027809)
>>> q = gmpy2.divexact(n, p)
>>> phi_n = (p - 1) * (q - 1)
>>> d = gmpy2.invert(e, phi_n)
>>> from Crypto.PublicKey import RSA
>>> key = RSA.construct((int(n), e, int(d)))
>>> with open('private_key.pem', 'wb') as f:
...     f.write(key.export_key('PEM'))
...
3242
```

Using the key, we decrypt the ciphertext:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Redacted_RSA_3]
└─$ openssl pkeyutl -decrypt -inkey private_key.pem -in encrypted.txt -out -
CTF{lattices_are_frequently_involved_when_recovering_full_secret_from_partial_ones}
```