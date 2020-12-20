# Extremely Covert Bytes
Category: Cryptography

## Description
> DuckyDebugDuck is taking Computer Security this semester and he decided to create this encryption service using an encryption he seen in class, and again he hid a flag in it, see if you can get it.

## Solution

Let's connect to the attached service:

```console
root@kali:/media/sf_CTFs/technion/Extremely_Covert_Bytes# nc ctf.cs.technion.ac.il 4013

______            _         ______     _                ______            _
|  _  \          | |        |  _  \   | |               |  _  \          | |
| | | |_   _  ___| | ___   _| | | |___| |__  _   _  __ _| | | |_   _  ___| | __
| | | | | | |/ __| |/ / | | | | | / _ \ '_ \| | | |/ _` | | | | | | |/ __| |/ /
| |/ /| |_| | (__|   <| |_| | |/ /  __/ |_) | |_| | (_| | |/ /| |_| | (__|   <
|___/  \__,_|\___|_|\_\\__, |___/ \___|_.__/ \__,_|\__, |___/  \__,_|\___|_|\_\
                        __/ |                       __/ |
                       |___/                       |___/

Enter the message you want to encrypt:
> test
f08e51ae8bff6986a3e3ad94c1e8efa7de53d1dd8325ce35fedcb2278090cd67
```

So this service allows us to enter a string and have it encrypted. The challenge title hints that the encryption algorithm is [ECB](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#ECB):

> The simplest of the encryption modes is the electronic codebook (ECB) mode (named after conventional physical codebooks). The message is divided into blocks, and each block is encrypted separately.
> 
> The disadvantage of this method is a lack of diffusion. Because ECB encrypts identical plaintext blocks into identical ciphertext blocks, it does not hide data patterns well. ECB is not recommended for use in cryptographic protocols.

Let's check by trying to encrypt a long message, and see if any of the encrypted blocks are identical:

```console
root@kali:/media/sf_CTFs/technion/Extremely_Covert_Bytes# nc ctf.cs.technion.ac.il 4013

______            _         ______     _                ______            _
|  _  \          | |        |  _  \   | |               |  _  \          | |
| | | |_   _  ___| | ___   _| | | |___| |__  _   _  __ _| | | |_   _  ___| | __
| | | | | | |/ __| |/ / | | | | | / _ \ '_ \| | | |/ _` | | | | | | |/ __| |/ /
| |/ /| |_| | (__|   <| |_| | |/ /  __/ |_) | |_| | (_| | |/ /| |_| | (__|   <
|___/  \__,_|\___|_|\_\\__, |___/ \___|_.__/ \__,_|\__, |___/  \__,_|\___|_|\_\
                        __/ |                       __/ |
                       |___/                       |___/

Enter the message you want to encrypt:
> aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
0b7103a86e5875c79f8b4acd56eb6eb00b7103a86e5875c79f8b4acd56eb6eb0236ea490346bb193105bc0d7c08516e266a2b8cf0c236433671ff3c1948934d6
Enter the message you want to encrypt:
```

We've entered 32 `a` characters, and we can see that the `0b7103a86e5875c79f8b4acd56eb6eb0` block repeats itself twice, which matches our assumption about ECB. It also tells us that the block size being used here is 16 (and therefore 32 identical characters get encrypted into two identical blocks of 16 bytes each).

But what's all that after the two identical blocks? Let's try to encrypt an empty string:

```console
Enter the message you want to encrypt:
>
236ea490346bb193105bc0d7c08516e266a2b8cf0c236433671ff3c1948934d6
```

Notice how we still get an output. This is common for ECB-based challenges. What happens is that the service allows us to input any string we'd like to, and then appends to it a secret string (such as the flag). We must use ECB's weaknesses to recover the secret string without knowing the key. The attack we'll use is called a [Chosen-plaintext attack](https://en.wikipedia.org/wiki/Chosen-plaintext_attack).

For the sake of the explanation, let's assume that the flag is `cstechnion{this_is_a_fake_flag}` (but that the attacker doesn't know that). If the attacker tries to encrypt 15 `a` characters, the block alignment will be as follows (where P stands for padding):

```
aaaaaaaaaaaaaaacstechnion{this_is_a_fake_flag}PP
|-- block #0 --||-- block #1 --||-- block #2 --|
```

Let's see it in practice (of course, the real flag is different but that doesn't matter for the sake of the example):
```console
Enter the message you want to encrypt:
> aaaaaaaaaaaaaaa
85be86bb45ee1adad329b1e0211e0dbd6f8e2ff1846a86c415fff222aba36dcf2433cbd5ad330b0537dfe01585021f53
```

If we align block #0 of the result to our diagram, we get:
```
85 be 86 bb 45 ee 1a da d3 29 b1 e0 21 1e 0d bd
a  a  a  a  a  a  a  a  a  a  a  a  a  a  a  c 
|------------------ block #0 -----------------|
```

Let's call this our "target block value".

Now, the attacker wants to reveal the first character of the flag. So the next step is to take `aaaaaaaaaaaaaaa` and append every possible character to it:

```console
Enter the message you want to encrypt:
> aaaaaaaaaaaaaaaa
0b7103a86e5875c79f8b4acd56eb6eb0236ea490346bb193105bc0d7c08516e266a2b8cf0c236433671ff3c1948934d6
Enter the message you want to encrypt:
> aaaaaaaaaaaaaaab
019e44fdd9dd85fd7a70cd5ca527fa1d236ea490346bb193105bc0d7c08516e266a2b8cf0c236433671ff3c1948934d6
Enter the message you want to encrypt:
> aaaaaaaaaaaaaaac
85be86bb45ee1adad329b1e0211e0dbd236ea490346bb193105bc0d7c08516e266a2b8cf0c236433671ff3c1948934d6
```

If the first block received from the encryption service is identical to the target block value, the attacker knows that the first character of the flag is the one that was used for this attempt (in our example: `c`, resulting in a block value of `85be86bb45ee1adad329b1e0211e0dbd` like we were searching for).

After the first character is revealed, the process continues by removing one character from the prefix and repeating the process (first encrypting `aaaaaaaaaaaaaac` and marking the result, then appending every possible character to `aaaaaaaaaaaaaac` and looking for an identical block value).

We'll use a tool called [ECB Adaptive Chosen Plaintext](https://github.com/tabneib/ecbacp) to recover the flag. It requires us to implement a method for encrypting a given string (which in our case we'll just forward to the encryption service) and takes care of the rest.

Our encryption module:
```python
# technion.py
from pwn import *

r = remote("ctf.cs.technion.ac.il", 4013)

def encrypt(text):
    r.sendlineafter("> ", text)
    return r.recvline(keepends=False).decode("ascii")
```

Let's run the attack:
```console
root@kali:/media/sf_CTFs/technion/Extremely_Covert_Bytes/ecbacp/ecbacp# python3 ecbacp.py --enc technion -v
# ...
[+] Real:    a50294bbb0a858d7494652b7e60bc861
[+] Expect:    2c07cc1ff510a4369efdcabada202179

[+] Trying: cstechnion{1_byt3_4t_a_t1mb
[+] Payload: AAAAAcstechnion{1_byt3_4t_a_t1mb
[+] Cipher:
        c4efe7d57e3ac8da2b051531b04f4338
        5b9c4c0dac0da611881c1243720f0a70
        236ea490346bb193105bc0d7c08516e2
        66a2b8cf0c236433671ff3c1948934d6

[+] Real:    5b9c4c0dac0da611881c1243720f0a70
[+] Expect:    2c07cc1ff510a4369efdcabada202179

[+] Trying: cstechnion{1_byt3_4t_a_t1mc
[+] Payload: AAAAAcstechnion{1_byt3_4t_a_t1mc
[+] Cipher:
        c4efe7d57e3ac8da2b051531b04f4338
        f6de1f55eb2a0646be2c92f2e44c1d64
        236ea490346bb193105bc0d7c08516e2
        66a2b8cf0c236433671ff3c1948934d6

[+] Real:    f6de1f55eb2a0646be2c92f2e44c1d64
[+] Expect:    2c07cc1ff510a4369efdcabada202179

[+] Trying: cstechnion{1_byt3_4t_a_t1md
[+] Payload: AAAAAcstechnion{1_byt3_4t_a_t1md
[+] Cipher:
        c4efe7d57e3ac8da2b051531b04f4338
        9e352afb3b97868dea31162ce401f308
        236ea490346bb193105bc0d7c08516e2
        66a2b8cf0c236433671ff3c1948934d6

[+] Real:    9e352afb3b97868dea31162ce401f308
[+] Expect:    2c07cc1ff510a4369efdcabada202179

[+] Trying: cstechnion{1_byt3_4t_a_t1me
[+] Payload: AAAAAcstechnion{1_byt3_4t_a_t1me
[+] Cipher:
        c4efe7d57e3ac8da2b051531b04f4338
        2c07cc1ff510a4369efdcabada202179
        236ea490346bb193105bc0d7c08516e2
        66a2b8cf0c236433671ff3c1948934d6

[+] Real:    2c07cc1ff510a4369efdcabada202179
[+] Expect:    2c07cc1ff510a4369efdcabada202179
[+] Found one character: e
[+] Current suffix: cstechnion{1_byt3_4t_a_t1me
[OK]
[+] No prefix found
[+] Suffix found, length: 27
[+] The suffix content is successfully brute-forced:
    cstechnion{1_byt3_4t_a_t1me
```

The flag: `cstechnion{1_byt3_4t_a_t1me}`.