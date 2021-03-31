# The Vault 2
Category: Pwn, 350 Points

## Description
> nc challenges.ctfd.io 30441



## Solution

This is the follow-up challenge for [The Vault 1](The_Vault_1.md).

Let's connect to the attached service:

```console
root@kali:~# nc challenges.ctfd.io 30441
 _____  _              __     __               _  _              ____
|_   _|| |__    ___    \ \   / /  __ _  _   _ | || |_           |___ \
  | |  | '_ \  / _ \    \ \ / /  / _` || | | || || __|  _____     __) |
  | |  | | | ||  __/     \ V /  | (_| || |_| || || |_  |_____|   / __/
  |_|  |_| |_| \___|      \_/    \__,_| \__,_||_| \__|          |_____|

 ____  ____  ____  ____  ____  ____  ____
||A ||||B ||||C ||||D ||||E ||||F ||||G ||
||__||||__||||__||||__||||__||||__||||__||
|/__\||/__\||/__\||/__\||/__\||/__\||/__\|
 ____  ____  ____  ____  ____  ____  ____
||H ||||I ||||J ||||K ||||L ||||M ||||N ||
||__||||__||||__||||__||||__||||__||||__||
|/__\||/__\||/__\||/__\||/__\||/__\||/__\|
 ____  ____  ____  ____  ____  ____  ____
||O ||||P ||||Q ||||R ||||S ||||T ||||U ||
||__||||__||||__||||__||||__||||__||||__||
|/__\||/__\||/__\||/__\||/__\||/__\||/__\|
 ____  ____  ____  ____  ____  ____  ____
||* ||||V ||||W ||||X ||||Y ||||Z ||||# ||
||__||||__||||__||||__||||__||||__||||__||
|/__\||/__\||/__\||/__\||/__\||/__\||/__\|


Welcome Agent, we need your help to open a Secure Vault.
This time, More Twisted Vault !!!
We managed to intercept some of the encryption method of the vault.
Be aware, You have one time chance to submit the password
Good luck !!!

*************** Main Menu ***************
*                                       *
*  [ 1 ] ---- Encrypt a Letter          *
*  [ 2 ] ---- Print Encrypted Password  *
*  [ 3 ] ---- Submit Password           *
*  [-1 ] ---- Quit                      *
*                                       *
*****************************************
```

Using this UI, we can encrypt letters:

```console
Enter input:
1
Please enter a letter to encrypt
A
Your encrypted letter is: K
Based on: 2317227390

Enter input:
1
Please enter a letter to encrypt
A
Your encrypted letter is: N
Based on: 712378095

Enter input:
1
Please enter a letter to encrypt
A
Your encrypted letter is: V
Based on: 2480001805

Enter input:
1
Please enter a letter to encrypt
B
Your encrypted letter is: S
Based on: 459486525

Enter input:
1
Please enter a letter to encrypt
B
Your encrypted letter is: C
Based on: 4235487127
```

We can see that encrypting the same letter a few times results in different outputs (and a different base).

Also, it looks like the program accepts only uppercase letters:
```console
*************** Main Menu ***************
*                                       *
*  [ 1 ] ---- Encrypt a Letter          *
*  [ 2 ] ---- Print Encrypted Password  *
*  [ 3 ] ---- Submit Password           *
*  [-1 ] ---- Quit                      *
*                                       *
*****************************************

Enter input:
1
Please enter a letter to encrypt
a
You can only encrpyt one uppercase letter
```

To try and reverse engineer the encryption scheme let's try to concentrate on the cases where a specific letter gets encrypted to a the same output. For example, let's examine the cases where `A` gets encrypted to `G`:

```console
root@kali:/media/sf_CTFs/matrix/The_Vault_2/data# python3 -c "print ('1\nA\n'*100 + '-1\n')" | nc challenges.ctfd.io 30441 | grep "Your encrypted letter is: G" -A 1 | grep "Based" | awk '{ print $3 }'
456372130
3593762906
2164999076
4183105010
540713530
```

Here we extract the bases for the cases where `A` was encrypted to `G` out of 100 attempts.

Now, the most trivial way to arrive from such large values to something in the neighborhood is to apply the modulus operator. First thing that comes to mind it to use `value % 26`:
```python
>>> for x in [456372130, 3593762906, 2164999076, 4183105010, 540713530]:
...     print(x % 26, end=', ')
...
6, 6, 6, 6, 6,
```

We can see that all the original values have the same `mod 26` value. It's unlikely that this is by chance. Furthermore, `A` and `G` are 6 letters apart. Let's try another example:

```console
root@kali:/media/sf_CTFs/matrix/The_Vault_2# python3 -c "print ('1\nZ\n'*100 + '-1\n')" | nc challenges.ctfd.io 30441 | grep "Your encrypted letter is: B" -A 1 | grep "Based" | awk '{ print $3 }'
3630989832
3731559756
1427472802
2765159282
3826572700
3456151740
1720386592
```

Here we are testing when `Z` gets encrypted to `B`. Again, let's apply modulus:

```python
>>> for x in [3630989832, 3731559756, 1427472802, 2765159282, 3826572700, 3456151740, 1720386592]:
...     print(x % 26, end=', ')
...
2, 2, 2, 2, 2, 2, 2,
```

We can see that the same behavior is presented. Furthermore, `Z` and `B` are two letters apart in ROT ciphers, so this is starting to look like a ROT cipher based on `value % 26`:

```python
>>> def rot_alpha(n):
...     from string import ascii_uppercase as uc
...     lookup = str.maketrans(uc, uc[n:] + uc[:n])
...     return lambda s: s.translate(lookup)
>>> rot_alpha(6)('A')
'G'
>>> rot_alpha(2)('Z')
'B'
```

To reverse the encryption, we just need to apply the reverse modulus:

```python
>>> rot_alpha(26 - 6)('G')
'A'
>>> rot_alpha(26 - 2)('B')
'Z'
```

So now we know how to encrypt and decrypt given a base. Let's see what option #2 of the UI gives us:
```console
root@kali:/media/sf_CTFs/matrix/The_Vault_2# nc challenges.ctfd.io 30441
 _____  _              __     __               _  _              ____
|_   _|| |__    ___    \ \   / /  __ _  _   _ | || |_           |___ \
  | |  | '_ \  / _ \    \ \ / /  / _` || | | || || __|  _____     __) |
  | |  | | | ||  __/     \ V /  | (_| || |_| || || |_  |_____|   / __/
  |_|  |_| |_| \___|      \_/    \__,_| \__,_||_| \__|          |_____|

 ____  ____  ____  ____  ____  ____  ____
||A ||||B ||||C ||||D ||||E ||||F ||||G ||
||__||||__||||__||||__||||__||||__||||__||
|/__\||/__\||/__\||/__\||/__\||/__\||/__\|
 ____  ____  ____  ____  ____  ____  ____
||H ||||I ||||J ||||K ||||L ||||M ||||N ||
||__||||__||||__||||__||||__||||__||||__||
|/__\||/__\||/__\||/__\||/__\||/__\||/__\|
 ____  ____  ____  ____  ____  ____  ____
||O ||||P ||||Q ||||R ||||S ||||T ||||U ||
||__||||__||||__||||__||||__||||__||||__||
|/__\||/__\||/__\||/__\||/__\||/__\||/__\|
 ____  ____  ____  ____  ____  ____  ____
||* ||||V ||||W ||||X ||||Y ||||Z ||||# ||
||__||||__||||__||||__||||__||||__||||__||
|/__\||/__\||/__\||/__\||/__\||/__\||/__\|


Welcome Agent, we need your help to open a Secure Vault.
This time, More Twisted Vault !!!
We managed to intercept some of the encryption method of the vault.
Be aware, You have one time chance to submit the password
Good luck !!!

*************** Main Menu ***************
*                                       *
*  [ 1 ] ---- Encrypt a Letter          *
*  [ 2 ] ---- Print Encrypted Password  *
*  [ 3 ] ---- Submit Password           *
*  [-1 ] ---- Quit                      *
*                                       *
*****************************************

Enter input:
2
Encrypted password: YAENPARCRZCGPUATSPKNOSSGXNGFPGNNWITZBGKIKCHLDPLOMUXY
Enter input:
-1
bye bye :)
```

We know how to decrypt letters given a base, but where's the base? This is the catch - we need to predict it.

As we saw in the previous challenge, the algorithm used in Python's `getrandbits` is MersenneTwister, which isn't cryptographically secure:

> [The algorithm] Is not cryptographically secure, unless the CryptMT variant is used. The reason is that observing a sufficient number of iterations (624 in the case of MT19937, since this is the size of the state vector from which future iterations are produced) allows one to predict all future iterations. (Source: [Wikipedia](https://en.wikipedia.org/wiki/Mersenne_Twister))

Assuming that the encryption is based on random numbers generated using MersenneTwister, we can use the encryption API to receive an infinite number of samples which will allow us to predict all future results of the algorithm.

We can use [this predictor](https://github.com/kmyk/mersenne-twister-predictor) to predict the upcoming random numbers, given 624 samples from the program.

The code:

```python
from pwn import *
from enum import Enum
from mt19937predictor import MT19937Predictor

NUM_SAMPLES = 624

class MenuChoice(Enum):
    ENCRYPT_LETTER              = 1
    PRINT_ENCRYPTED_PASSWORD    = 2
    SUBMIT_PASSWORD             = 3
    QUIT                        = -1

def encrypt_letter_multiple_times(letter, times):
    r.sendafter("***********************************\n\n", f"{MenuChoice.ENCRYPT_LETTER.value}\n{letter}\n" * times)

# https://eddmann.com/posts/implementing-rot13-and-rot-n-caesar-ciphers-in-python/
def rot_alpha(n):
    from string import ascii_uppercase as uc
    lookup = str.maketrans(uc, uc[n:] + uc[:n])
    return lambda s: s.translate(lookup)

predictor = MT19937Predictor()
r = remote("challenges.ctfd.io", 30441)

encrypt_letter_multiple_times('A', NUM_SAMPLES)

for i in range(NUM_SAMPLES):
    r.recvuntil("Based on:")
    base = int(r.recvlineS(keepends = False).strip())
    predictor.setrandbits(base, 32)
log.info(f"Received samples for {NUM_SAMPLES} random numbers")

r.sendline(str(MenuChoice.PRINT_ENCRYPTED_PASSWORD.value))
r.recvuntil("Encrypted password:")
encrypted_password = r.recvlineS(keepends = False).strip()
log.info(f"Encrypted password: '{encrypted_password}'")

password = ""

for c in encrypted_password:
    password += rot_alpha(26 - (predictor.getrandbits(32) % 26))(c)

log.info(f"Decrypted password: '{password}'")

r.sendlineafter("Enter input:\n", str(MenuChoice.SUBMIT_PASSWORD.value))
r.sendlineafter("Please submit the password\n", password)

print(r.recvallS())

```

The output:

```console
root@kali:/media/sf_CTFs/matrix/The_Vault_2# python3 solve.py
[+] Opening connection to challenges.ctfd.io on port 30441: Done
[*] Received samples for 624 random numbers
[*] Encrypted password: 'HTRGOPGIJREHUPEOXESASINIHVKTLRCCUEQQWQUFIZRPROKHYCFT'
[*] Decrypted password: 'SEEMSLIKEYOUUNTWISTEDTHEMERSENNETWISTERPRNGALGOIRTHM'
[+] Receiving all data: Done (59B)
[*] Closed connection to challenges.ctfd.io port 30441
Congrats !!!
MCL{M3rSenne_Tw1s7er_1s_STi11_Prn9_4ft3r_A11}
```