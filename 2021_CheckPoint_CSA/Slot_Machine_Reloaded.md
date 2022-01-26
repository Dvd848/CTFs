# Slot Machine Reloaded
Category: Reversing, 300 Points

## Description

> Remember the Slot Machine from CSA 2019?
> 
> Well, this one is a little harder...

```python
#!/usr/bin/env python3

import random
import collections
import math
from .secret import flag


PRINTABLE = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+-/:.;<=>?@[]^_`{}"
flag_len = len(flag)
NO_COINS = "NO MORE COINS! GOODBYE."
NOT_ENOUGH_COINS = "YOU DON'T HAVE ENOUGH COINS!"
INVALID_COIN_NUMBER = "COIN NUMBER CAN'T BE NEGATIVE"
INITIAL_COINS = 10


class Slotmachine(object):
    def __init__(self):
        seed = random.SystemRandom().getrandbits(64) # Using SystemRandom is slow, use only for seed.
        self.random = random.Random(seed) # This will make sure no one messes with my seeds!
        self.slots = [list(PRINTABLE) for i in range(flag_len)]
        self.attempt_num = 0
        self.total_coins = INITIAL_COINS
        self.last_result = ""
        self.last_gamble = 0

    def get_prize(self):
        result = self.last_result
        prize = sum([x for x in collections.Counter(result).values() if x > 2])
        prize *= self.last_gamble
        self.total_coins += prize
        return prize

    def prepend_flag(self):
        for i in range(flag_len):
            self.slots[i].remove(flag[i])
            self.slots[i] = [flag[i]] + self.slots[i]

    def check_invalid_input(self, coins):
        if self.total_coins <= 0:
            self.last_result = ""
            return NO_COINS
        if self.total_coins < coins:
            self.last_result = ""
            return NOT_ENOUGH_COINS
        if coins < 0:
            self.last_result = ""
            return INVALID_COIN_NUMBER
        return None

    # My cat wrote this function
    def choice(self):
        rand_num = format(self.random._randbelow((1 << (flag_len*len(f'{len(PRINTABLE) - 1:b}'))) - 1),
            '#0%db' % (len(self.slots)*int(math.log(len(PRINTABLE), 2)) + 2))[2:]
        result = ""
        j = 0
        for i in range(0,len(rand_num),len(f'{len(PRINTABLE) - 1:b}')):
            result += self.slots[j][int(rand_num[i:i+len(f'{len(PRINTABLE) - 1:b}')],2)]
            j += 1
        return result

    def spin(self, coins):
        invalid_message = self.check_invalid_input(coins)
        if invalid_message:
            return invalid_message.center(flag_len)
        
        self.last_gamble = coins
        self.total_coins -= coins

        if self.attempt_num == 200:
            self.prepend_flag()
        self.attempt_num += 1

        result = self.choice()
        self.last_result = result
        return result


def main():
    slotmachine = Slotmachine()
    print(f"You have {slotmachine.total_coins} coins")
    get_next_num = True
    while get_next_num:
        try:
            prize = 0
            coins = int(input("Enter number of coins:\n"))
            result = slotmachine.spin(coins)
            if result == NO_COINS:
                get_next_num = False
            elif result != NOT_ENOUGH_COINS:
                prize = slotmachine.get_prize()
            print(result)
            print(f"You won {prize} coins!")
            print(f"{slotmachine.total_coins} coins left.")

        except ValueError:
            get_next_num = False
        except NameError:
            get_next_num = False


if __name__ == "__main__":
    main()

```


## Solution

This is a follow-up for the slot-machine challenge in [CSA 2019](https://github.com/Dvd848/CTFs/tree/master/2019_CheckPoint_CSA). In that challenge, the slot machine leaked us its random seed, allowing us to recreate the list of random number that Python's `random` library would produce. In this challenge, the creators took care of this problem, by using `SystemRandom` to generate the seed for `random`:

```python
        seed = random.SystemRandom().getrandbits(64) # Using SystemRandom is slow, use only for seed.
        self.random = random.Random(seed) # This will make sure no one messes with my seeds!
```

Now we can't predict the random seed, and therefore can't predict the values received from `random`. Or can we?

When visiting the documentation page of Python's [random](https://docs.python.org/3/library/random.html), we can see the following:

> **Warning**: The pseudo-random generators of this module should not be used for security purposes. For security or cryptographic uses, see the `secrets` module.
> 
> See also M. Matsumoto and T. Nishimura, “Mersenne Twister: A 623-dimensionally equidistributed uniform pseudorandom number generator”, ACM Transactions on Modeling and Computer Simulation Vol. 8, No. 1, January pp.3–30 1998.

We are warned not to use `random` for anything important, and informed that Python's `random` uses an algorithm called *Mersenne Twister* - A 623-dimension PRNG. A short research about [Mersenne Twister](https://en.wikipedia.org/wiki/Mersenne_Twister) reveals that:

> Is not cryptographically secure (...). The reason is that observing a sufficient number of iterations (624 in the case of MT19937, since this is the size of the state vector from which future iterations are produced) allows one to predict all future iterations.

So it's possible to predict the next random numbers even if we don't have the original seed, provided that we can identify `624` DWORDs generated by the PRNG. There's even a [Python library](https://github.com/kmyk/mersenne-twister-predictor) that wraps the whole thing in a nice API. Now what?

Let's take a closer look at the slot machine. It starts by initializing the slots in a predictable way:

```python
self.slots = [list(PRINTABLE) for i in range(flag_len)]
```

Then, for each iteration, it scrambles the slots using this horrible function:

```python
    # My cat wrote this function
    def choice(self):
        rand_num = format(self.random._randbelow((1 << (flag_len*len(f'{len(PRINTABLE) - 1:b}'))) - 1),
            '#0%db' % (len(self.slots)*int(math.log(len(PRINTABLE), 2)) + 2))[2:]
        result = ""
        j = 0
        for i in range(0,len(rand_num),len(f'{len(PRINTABLE) - 1:b}')):
            result += self.slots[j][int(rand_num[i:i+len(f'{len(PRINTABLE) - 1:b}')],2)]
            j += 1
        return result
```

After 200 iterations, it mixes in the flag via `prepend_flag()`, and that's when we need to start utilizing our predictor in order to understand where the flag characters are hiding.

Before anything, we need to understand the flag length. The length of the flag is equal to the length of the `slots` list, but we can't access it directly. All we get is the `result` after being manipulated by `choice`:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Slot_Machine_Reloaded]
└─$ curl http://slot-machine-reloaded.csa-challenge.com/ -I
HTTP/1.1 200 OK
Server: nginx/1.14.0 (Ubuntu)
Date: Sat, 18 Sep 2021 10:27:00 GMT
Content-Type: text/html; charset=utf-8
Content-Length: 3026
Connection: keep-alive
X-Frame-Options: SAMEORIGIN
Vary: Cookie
Set-Cookie: sessionid=4hr4aukcc0bxmv3c98q1xneb73wqrof0; expires=Sat, 02 Oct 2021 10:27:00 GMT; HttpOnly; Max-Age=1209600; Path=/; SameSite=Lax


┌──(user@kali)-[/media/sf_CTFs/checkpoint/Slot_Machine_Reloaded]
└─$ curl "http://slot-machine-reloaded.csa-challenge.com/spin/?coins=1" -H "Cookie: sessionid=4hr4aukcc0bxmv3c98q1xneb73wqrof0"
{"result": "K^U[0P}JA89P}^_;>KNLZTE]T]Z0<+PX", "prize": 3, "current_coins": 12}
```

`choice` somehow uses `flag_len` and `len(self.slots)` but it's not very clear what's the relationship between these three lengths. We'll simplify `choice` a bit in order to examine the relationship between the length of `result` and the length of the flag:

```python
>>> import random
>>> import math
>>> PRINTABLE = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+-/:.;<=>?@[]^_`{}"
>>> rand = random.Random(random.SystemRandom().getrandbits(64))
>>> def choice(flag_len):
...     rand_num = format(rand._randbelow((1 << (flag_len*len(f'{len(PRINTABLE) - 1:b}'))) - 1),
...         '#0%db' % (flag_len*int(math.log(len(PRINTABLE), 2)) + 2))[2:]
...     result_len = 0
...     for i in range(0,len(rand_num),len(f'{len(PRINTABLE) - 1:b}')):
...         result_len += 1
...     return(result_len)
```

This is mainly based on the fact that `slots` is a list of lists of characters, so each time `self.slots[j][int(rand_num[i:i+len(f'{len(PRINTABLE) - 1:b}')],2)]` is appended to `result`, we know that `result` is growing by one character.

```python
>>> for i in range(1, 40):
...     assert(i == choice(i))
...
>>>
```

It looks like the length of the result is equal to the flag length, so we know that `flag_len == len("K^U[0P}JA89P}^_;>KNLZTE]T]Z0<+PX") == 32`. Now we need to dig deeper into `choice` and understand how the random number gets encoded into `result`, in order to reverse the procedure and extract the original random number from the result. Once we know `flag_len` and `len(self.slots)`, most of the parameters in `choice` are constant:

```python
>>> flag_len = 32
>>> ((1 << (flag_len*len(f'{len(PRINTABLE) - 1:b}'))) - 1)
6277101735386680763835789423207666416102355444464034512895
>>> '#0%db' % (flag_len*int(math.log(len(PRINTABLE), 2)) + 2)
'#0194b'
>>> bin(6277101735386680763835789423207666416102355444464034512895)
'0b111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111'
>>> len(bin(6277101735386680763835789423207666416102355444464034512895))
194
>>>
>>> rand_num_func = lambda: format(rand._randbelow((1 << (flag_len*len(f'{len(PRINTABLE) - 1:b}'))) - 1), '#0%db' % (flag_len*int(math.log(len(PRINTABLE), 2)) + 2))[2:]
>>> rand_num_func()
'001110101000001001001101110011000011110001101110000011110000000101100101001000010011101000101000010110101011010000001000101011000100100111111000100100110100110100101111000010011011100110000001'
>>> rand_num_func()
'001001110001000001000101111010101100000000011001000111010101011101110000100110111000010001101111110110110111011001110010000110001111010111100101111101001001010111001000100000010010101110100110'
>>> rand_num_func()
'100101011110100111000111101110010000100111001011001110101111101001101101011010011101011110011101000101101000110001011101001110101110110110011100111101100110101000010111100111000101011011110100'
>>> rand_num_func()
'000001110001000111111110110110110111010010111101000100111011101100101101110101110011000001011101011101110010111111000000001000110101101000101100110110010111011000101011101101111000110011001011'
>>> len(rand_num_func())
192
>>> len(f'{len(PRINTABLE) - 1:b}')
6
>>> rand_num = rand_num_func()
>>> rand_num
'100011100101110000110000111001101101101101100111000001001110110100000100000111010101110011010000111110011011100100001101001100000111010000000100010001111110000000111011011011111001010101111001'
>>> j = 0
>>> for i in range(0,len(rand_num),len(f'{len(PRINTABLE) - 1:b}')):
...     print("j = {} -> {}".format(j, rand_num[i:i+len(f'{len(PRINTABLE) - 1:b}')]))
...     j += 1
...
j = 0 -> 100011
j = 1 -> 100101
j = 2 -> 110000
j = 3 -> 110000
j = 4 -> 111001
j = 5 -> 101101
j = 6 -> 101101
j = 7 -> 100111
j = 8 -> 000001
j = 9 -> 001110
j = 10 -> 110100
j = 11 -> 000100
j = 12 -> 000111
j = 13 -> 010101
j = 14 -> 110011
j = 15 -> 010000
j = 16 -> 111110
j = 17 -> 011011
j = 18 -> 100100
j = 19 -> 001101
j = 20 -> 001100
j = 21 -> 000111
j = 22 -> 010000
j = 23 -> 000100
j = 24 -> 010001
j = 25 -> 111110
j = 26 -> 000000
j = 27 -> 111011
j = 28 -> 011011
j = 29 -> 111001
j = 30 -> 010101
j = 31 -> 111001
```

As we can see from the example above, `rand_num` is simply a 192-bit random number, which is later divided into `32` chunks of `6` bits used as indices to retrieve a character from `slots`.

Let's use this to reconstruct the actual random number for the first iteration. In the example we saw earlier, the result we got was `"K^U[0P}JA89P}^_;>KNLZTE]T]Z0<+PX"`. So, to get the first `6` most significant bits of the random number for the first iteration, we check what's the (binary representation of the) index of the first character in `result` (i.e. `K`) in `slots[0]`.

```python
>>> PRINTABLE = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+-/:.;<=>?@[]^_`{}"
>>> FLAG_LEN = 32
>>> slots = [list(PRINTABLE) for _ in range(FLAG_LEN)]
>>> result = "K^U[0P}JA89P}^_;>KNLZTE]T]Z0<+PX"
>>> get_bits = lambda x: format(slots[x].index(result[x]), '06b')
>>> get_bits(0)
'010100'
```

For the next 6 bits, we check the second character in `result`, against the second `slots` list:

```python
>>> get_bits(1)
'111011'
```

We can construct the full random number using:

```python
>>> rand_num = ""
>>> for i in range(32):
...     rand_num += get_bits(i)
...
>>> rand_num
'010100111011011110111001000000011001111111010011001010001000001001011001111111111011111100110011110110010100010111010101100011011101001110111010011101111010100011000000110100101110011001100001'
>>> int(rand_num, 2)
2052751215110038155256122112235701712256845575377300678241
```

This gives us `192` bits which we feed into the predictor:

```python
>>> predictor = MT19937Predictor()
>>> predictor.setrandbits(rand_num, 192)
```

We need to continue feeding random numbers until we're done feeding `624 * 32` bits. If each iterations gives us `192` bits, we should be done within `624 * 32 / 192 == 104` iterations. Then we wait for the 200th iteration, which has the special logic:

```python
        if self.attempt_num == 200:
            self.prepend_flag()
```

`prepend_flag` just prepends the flag to the `slots` list, one character to each internal list:

```python
    def prepend_flag(self):
        for i in range(flag_len):
            self.slots[i].remove(flag[i])
            self.slots[i] = [flag[i]] + self.slots[i]
```

So, after `prepend_flag()` is run, we will meet a character from the flag each time `self.slots[j][int(rand_num[i:i+len(f'{len(PRINTABLE) - 1:b}')],2)]` gets evaluated to `self.slots[j][0]`, and that's exactly when we'll collect the `j`th character of the flag.

Putting it all together, we get:

```python
from pwn import *
import math
import requests

from mt19937predictor import MT19937Predictor

def main(is_web):
    predictor = MT19937Predictor()
    needed_bits = 624 * 32

    PRINTABLE = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+-/:.;<=>?@[]^_`{}"
    FLAG_LEN = 32
    RAND_BITS = 192
    FLAG_APPENDED_ITERATION = 200

    slots = [list(PRINTABLE) for _ in range(FLAG_LEN)]

    if is_web:
        s = requests.session()
        s.get("http://slot-machine-reloaded.csa-challenge.com/")
    else:
        p = process(["python3", "-m", "Slot_Machine_Reloaded.Slot Machine Reloaded"])

    attempt_num = 0

    flag = [None] * FLAG_LEN

    with log.progress("Leaking flag...") as p:
        while True:
            if is_web:
                res = s.get("http://slot-machine-reloaded.csa-challenge.com/spin/?coins=1")
                out = res.json()["result"]
            else:
                p.sendlineafter(b"Enter number of coins:\n", b"1")
                out = p.recvlineS(keepends = False)

            if attempt_num < FLAG_APPENDED_ITERATION:
                binary_str = ""
                for i, c in enumerate(out):
                    index = slots[i].index(c)
                    b = format(index, '06b')
                    binary_str += b
                rand_num = int(binary_str, 2)

                if needed_bits > 0:
                    p.status(f"Gathering random bits, needed bits = {needed_bits}")
                    predictor.setrandbits(rand_num, RAND_BITS)
                    needed_bits -= RAND_BITS
                else:
                    p.status(f"Checking predictor, iteration #{attempt_num}/{FLAG_APPENDED_ITERATION}")
                    predicted = predictor.getrandbits(RAND_BITS)
                    assert(predicted == rand_num)

            else:
                rand_num = format(predictor.getrandbits(RAND_BITS),
                    '#0%db' % (len(slots)*int(math.log(len(PRINTABLE), 2)) + 2))[2:]
                j = 0
                for i in range(0,len(rand_num),len(f'{len(PRINTABLE) - 1:b}')):
                    index = int(rand_num[i:i+len(f'{len(PRINTABLE) - 1:b}')],2)
                    if index == 0:
                        flag[j] = out[j]
                        p.status(f"Found character {out[j]}, known flag = {''.join([c if c is not None else '?' for c in flag])}")
                        if all(x is not None for x in flag):
                            return "".join(flag)
                    j += 1


            attempt_num +=1

if __name__ == "__main__":
    flag = main(is_web = True)
    log.success(f"Flag: {flag}")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Slot_Machine_Reloaded]
└─$ python3 solve.py
[+] Leaking flag...: Done
[+] Flag: CSA{I_L1K3_THE_TW1ST_4T_THE_END}
```