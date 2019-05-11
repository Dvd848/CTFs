# CB1
Crypto, 50 points

## Description

> ~C8 =39 A?2V8 73J:C 8FG7 AF?JJC2ECP
> 
> DF?LHb=r_>b0%_0520<c8bPN


## Solution

We'll use the same script from [CB1](CB1.md) to try and guess the solution using ROT and frequency analysis.

If we use the script and attempt to guess the solution for the complete text, we get:
```
root@kali:/media/sf_CTFs/sunshine/WelcomeCrypto# python3 ../CB1/rot.py -i ciphertext.txt -g
1TI NDJ RPCgI HD[KT IWXH RWP[[TCVTa

UWP]YsN%pOsA6pAFCAMtIsa_
```

Not close.

But - if we take the first line and try to guess the solution, we get:

```console
root@kali:/media/sf_CTFs/sunshine/WelcomeCrypto# python3 ../CB1/rot.py -s "$(head ciphertext.txt -n 1)" -g
Org lbh pna'g fbyir guvf punyyratr!
```

This looks like a good direction. What if we perform another guessing cycle?

```console
root@kali:/media/sf_CTFs/sunshine/WelcomeCrypto# python3 ../CB1/rot.py -s "$(head ciphertext.txt -n 1)" -g | python3 ../CB1/rot.py -i -g
Bet you can't solve this challenge!
```

This doesn't work on the second line though. There, the answer is just ROT47:
```console
root@kali:/media/sf_CTFs/sunshine/WelcomeCrypto# python3 ../CB1/rot.py -s "$(tail ciphertext.txt -n 1)" --rot47_n 47
sun{w3lC0m3_T0_da_k4g3!}
```