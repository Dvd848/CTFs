# The Vault 1
Category: Pwn, 250 Points

## Description
> nc challenges.ctfd.io 30440


## Solution

Let's connect to the attached service:

```console
root@kali:/media/sf_CTFs/matrix/The_Vault_1# nc challenges.ctfd.io 30440
 _____  _              __     __               _  _              _
|_   _|| |__    ___    \ \   / /  __ _  _   _ | || |_           / |
  | |  | '_ \  / _ \    \ \ / /  / _` || | | || || __|  _____   | |
  | |  | | | ||  __/     \ V /  | (_| || |_| || || |_  |_____|  | |
  |_|  |_| |_| \___|      \_/    \__,_| \__,_||_| \__|          |_|

 ____  ____  ____
||1 ||||2 ||||3 ||
||__||||__||||__||
|/__\||/__\||/__\|
 ____  ____  ____
||4 ||||5 ||||6 ||
||__||||__||||__||
|/__\||/__\||/__\|
 ____  ____  ____
||7 ||||8 ||||9 ||
||__||||__||||__||
|/__\||/__\||/__\|
 ____  ____  ____
||* ||||0 ||||# ||
||__||||__||||__||
|/__\||/__\||/__\|


Welcome Agent, we need your help to open a Secure Vault.
In order to open the vault you need to submit the correct password.
According to our intelligence the password is 624 digits long.
Also, we know that the vault has been initialized on 11/02/2021
Good luck !!!

************ Main Menu ************
*                                 *
*  [ 1 ] ---- Guessing Mode       *
*  [ 2 ] ---- Submit Password     *
*  [-1 ] ---- Quit                *
*                                 *
***********************************

1
Enter digit number 1
7
Incorrect digit :(
bye bye :)
```

We need to guess a 624-digit password for a vault. There's a "Guessing Mode" which seems promising, and an option to submit the password. Let's try guessing:

```console
************ Main Menu ************
*                                 *
*  [ 1 ] ---- Guessing Mode       *
*  [ 2 ] ---- Submit Password     *
*  [-1 ] ---- Quit                *
*                                 *
***********************************

1
Enter digit number 1
1
Enter digit number 2
1
Incorrect digit :(
bye bye :)
```

Well, if we guess the first digit correctly, we can continue to the next digit. Seems easy enough, let's automate.

```python
from pwn import *
from enum import Enum

import string

PASSWORD_LENGTH = 624

class MenuChoice(Enum):
    GUESSING_MODE   = 1
    SUBMIT_PASSWORD = 2
    QUIT            = -1

def select_menu(choice: MenuChoice):
    r.sendlineafter("***********************************\n\n", str(choice.value))

def guess_digit(guess):
    r.sendline(guess)
    return r.recvlineS()

password = []

while len(password) < PASSWORD_LENGTH:
    for d in string.digits:
        with context.local(log_level='ERROR'):
            r = remote("challenges.ctfd.io", 30440)

        select_menu(MenuChoice.GUESSING_MODE)

        r.recvline() # "Enter digit number 1"

        # Send known password for previous digits
        for p in password:
            out = guess_digit(p)
            assert(out.startswith("Enter digit number"))

        # Try to guess current digit
        out = guess_digit(d)
        if not out.startswith("Incorrect digit"):
            password.append(d)
            log.info(f"Found digit: '{d}', current password: '{''.join(password)}'")
            break
    else:
        break
```

Output:

```console
root@kali:/media/sf_CTFs/matrix/The_Vault_1# python3 guess.py
[*] Found digit: '1', current password: '1'
[*] Found digit: '0', current password: '10'
[*] Found digit: '5', current password: '105'
[*] Found digit: '8', current password: '1058'
[*] Found digit: '4', current password: '10584'
[*] Found digit: '6', current password: '105846'
[*] Found digit: '3', current password: '1058463'
[*] Found digit: '0', current password: '10584630'
[*] Found digit: '2', current password: '105846302'
[*] Found digit: '5', current password: '1058463025'
Traceback (most recent call last):
  File "guess.py", line 38, in <module>
    assert(out.startswith("Enter digit number"))
AssertionError
```

Looks like this worked well at first but unexpectedly failed. What's the reason?

```console
root@kali:/media/sf_CTFs/matrix/The_Vault_1# printf "1\n1\n0\n5\n8\n4\n6\n3\n0\n2\n5\n" | nc challenges.ctfd.io 30440
// ...
************ Main Menu ************
*                                 *
*  [ 1 ] ---- Guessing Mode       *
*  [ 2 ] ---- Submit Password     *
*  [-1 ] ---- Quit                *
*                                 *
***********************************

Enter digit number 1
Enter digit number 2
Enter digit number 3
Enter digit number 4
Enter digit number 5
Enter digit number 6
Enter digit number 7
Enter digit number 8
Enter digit number 9
Enter digit number 10
You have limited resources on guessing mode
bye bye :)
```

Ok, so it obviously won't be so easy. There's a piece of information that we haven't used yet: `Also, we know that the vault has been initialized on 11/02/2021`. What can we do with that?

Well, if the vault code password was chosen randomly, and we know that the vault was initialized on a certain date, it might be reasonable to assume that the seed for the random number generator was based on the timestamp at the time of the vault initialization. And, since we also know the first few digits of the password, we can try all possible timestamps for that given date and see which one results in a "random" sequence like the one we already have. If we find the correct seed, all the remaining numbers are predictable as well. 

The obvious next step here is to write a short `C` program to try and find the seed:

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <stdint.h>

int genrate_random_number(int param_1, int param_2)
{
   int iVar1;
   iVar1 = rand();
   return param_1 + iVar1 % ((param_2 - param_1) + 1);
}

int main(int argc, char *argv[])
{
   uint8_t password_start[] = {1, 0, 5, 8, 4, 6, 3, 0, 2, 5};
   uint8_t i = 0;
   time_t seed = 0; 
   for(seed = 1612994400; seed <= 1613080800;  seed++)
   {
      srand(seed);
      for(i = 0; i < 10; i++)
      {
         if(genrate_random_number(0, 9) != password_start[i])
         {
            break;
         }
      }

      if(i == 10)
      {
         printf("%ld\n", seed);
         break;
      }
   }

   return 0;
}
```

However, that did not produce any result. Moving the borders to account for time zone differences didn't help either. Of course, there is the possibility that the random number generator used wasn't `C`'s `rand()`.

Eventually, we found the sequence using Python's `random.getrandbits(32)`:

>  random.getrandbits(k)
>
> Returns a non-negative Python integer with k random bits. This method is supplied with the MersenneTwister generator [...]

In retrospective, the number of digits in the password (624) must have been a hint towards MersenneTwister:

> [The algorithm] Is not cryptographically secure, unless the CryptMT variant is used. The reason is that observing a sufficient number of iterations (624 in the case of MT19937, since this is the size of the state vector from which future iterations are produced) allows one to predict all future iterations. (Source: [Wikipedia](https://en.wikipedia.org/wiki/Mersenne_Twister))

The following script finds the correct seed and uses it to reveal the password and retrieve the flag:

```python
from pwn import *
from enum import Enum

import time
import datetime
import random

PASSWORD_LENGTH = 624

def unix_timestamp(str_date):
    return int(time.mktime(datetime.datetime.strptime(str_date, "%d/%m/%Y").timetuple()))

def get_next_rand_digit():
    return (random.getrandbits(32) % 10)

def check_seed_for_sequence(seed, expected_sequence):
    random.seed(seed)
    for i in range(len(expected_sequence)):
        if get_next_rand_digit() != expected_sequence[i]:
            return False
    return True

class MenuChoice(Enum):
    GUESSING_MODE   = 1
    SUBMIT_PASSWORD = 2
    QUIT            = -1

def guess_digit(guess):
    r.sendline(guess)
    return r.recvlineS()

def select_menu(choice: MenuChoice):
    r.sendlineafter("***********************************\n\n", str(choice.value))

def send_password(password):
    log.info(f"Sending password: {password}")
    r.sendlineafter("Submit your password", password)

r = remote("challenges.ctfd.io", 30440)

password_start = [1, 0, 5, 8, 4, 6, 3, 0, 2, 5]
for seed in range(unix_timestamp("11/02/2021"), unix_timestamp("12/02/2021")):
    if check_seed_for_sequence(seed, password_start):
        log.info(f"Found seed: {seed}")
        random.seed(seed)
        select_menu(MenuChoice.SUBMIT_PASSWORD)
        send_password("".join(str(get_next_rand_digit()) for _ in range(PASSWORD_LENGTH)))
        log.info(f"Output: {r.recvallS()}")
        break
```


Output:

```console
root@kali:/media/sf_CTFs/matrix/The_Vault_1# python3 solve.py
[+] Opening connection to challenges.ctfd.io on port 30440: Done
[*] Found seed: 1613053800
[*] Sending password: 105846302539567938323405515907032807907801091773011706044271195994443208898169677363135818467136596423209485460630398794491415901426456402515606463701758925768303806593250551966946582551120972955792537234384190960653110911971174771203307355681959233622484611316162932533877064250050702977290979129186854138495573314830568647290765162367145645428622165525086589282575839772809420620840571670358662507863187590578755528758367653681902083272904922515602281014957090759964606180357842719471913474692861377576392031374601472715845201601647073678993273558789889817945056411998821374340019602248308637258481221609292777211234811600
[+] Receiving all data: Done (67B)
[*] Closed connection to challenges.ctfd.io port 30440
[*] Output:
    Congrats !!!
    Your flag: MCL{T1m3_1s_4n_Illu5i0n_T1m1ng_1s_4n_AR7}
```