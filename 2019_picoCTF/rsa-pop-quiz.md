# rsa-pop-quiz
Cryptography, 200 points

## Description:
> Class, take your seats! It's PRIME-time for a quiz...


## Solution: 

This challenge is similar to last year's [rsa-madlibs](/2018_picoCTF/rsa-madlibs.md). The same script from last year worked with this challenge after making the following adjustments:

```console
root@kali:/media/sf_CTFs/pico/rsa-pop-quiz# diff solve.py ../../pico_2018/rsa-madlibs/solve.py
7,8c7,8
<     r.recvuntil("#### NEW PROBLEM ####\n")
<     lines = r.recvuntil("##### PRODUCE THE FOLLOWING ####\n")
---
>     r.recvuntil("#### NEW MADLIB ####\n")
>     lines = r.recvuntil("##### WE'RE GONNA NEED THE FOLLOWING ####\n")
38c38
< r = remote("2019shell1.picoctf.com", 20866)
---
> r = remote("2018shell3.picoctf.com", 18148)
54d53
<
104d102
<
```

Output:
```console
root@kali:/media/sf_CTFs/pico/rsa-pop-quiz# python solve.py
[+] Opening connection to 2019shell1.picoctf.com on port 20866: Done
[*] Requesting 'n' using ['q', 'p']
[*] Requesting 'q' using ['p', 'n']
[*] Requesting 'q' using ['e', 'n']
[*] Requesting 'totient(n)' using ['q', 'p']
[*] Requesting 'ciphertext' using ['plaintext', 'e', 'n']
[*] Requesting 'plaintext' using ['ciphertext', 'e', 'n']
[*] Requesting 'd' using ['q', 'p', 'e']
[*] Requesting 'plaintext' using ['p', 'ciphertext', 'e', 'n']
[+] Receiving all data: Done (116B)
[*] Closed connection to 2019shell1.picoctf.com port 20866
 Outstanding move!!!


If you convert the last plaintext to a hex number, then ascii, you'll find what you need! ;)

[+] picoCTF{wA8_th4t$_ill3aGal..o67980604}
```