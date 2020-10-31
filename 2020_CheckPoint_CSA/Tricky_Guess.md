# Tricky Guess
Category: Programming, 50 Points

## Description

> All you need to know is: netcat tricky-guess.csa-challenge.com 2222
> 
> Good luck!

A word list was attached.

## Solution

We'll take a look at the word list first:

```console
root@kali:/media/sf_CTFs/checkpoint/Tricky_Guess# head words.txt
krzvctoxdnya
mxehqdbglvjs
mishdcerlfzk
qjecszbfhard
upbikezhrgqj
etmwcbnvkjgz
jrxlizuhtbve
slaqphcmygfz
rnhvjicedgzp
plgexuarvmsz
root@kali:/media/sf_CTFs/checkpoint/Tricky_Guess# wc -l words.txt
10000 words.txt
```

So we have 10000 "words", one per line, in this file.

Let's connect to the server:

```console
root@kali:/media/sf_CTFs/checkpoint/Tricky_Guess# nc tricky-guess.csa-challenge.com 2222

    In this tricky game we have randomized a word out of a given wordlist.
    Your job is to guess that word.
    On each guess we will provide you number of characters you were correct with,
    without any indication upon what characters or their position.
    You have multiple limitations though:
    - On each run a new word is randomized.
    - You have 15 tries.
    - We are limiting the connection to 30 seconds.

    To block bruteforce we will show you the next cat and wait...
                                _
                               | \
                               | |
                               | |
         |\                    | |
        /, ~\                 / /
        X     `-.....-------./ /
        ~-. ~  ~              |
           \             /    |    <------------- Louie The Cat
            \  /_     ___\   /
            | /\ ~~~~~   \ |
            | | \        || |
            | |\ \       || )
           (_/ (_/      ((_/

    GO !
xmhnqwteksfj
7
jrxlizuhtbve
4
xwgonartsdfq
10
```

We have to guess the word that the server has chosen within 15 attempts, when all we get back is the number of characters we were correct with, without any indication upon what characters or their position.

Well, it turns out that the most trivial strategy actually works here: We make a random guess, check the number of common letters, eliminate all words that don't match and continue.

Here's the code:

```python
from pwn import *
import random

def main():
    with open("words.txt") as f:
        words = [word.rstrip() for word in f.readlines()]

    r = remote("tricky-guess.csa-challenge.com", 2222)
    r.recvuntil("GO !\n")

    for i in range(16):
        log.info("Number of words: {}".format(len(words)))
        choice = random.choice(words)
        r.sendline(choice)
        log.info(f"\tGuessing: '{choice}'")
        response = r.recvline()
        try:
            common_chars = int(response)
            log.info(f"\tNumber of common characters: {common_chars}")
        except:
            return response
        words = list(filter(lambda x: len(set(choice).intersection(x)) == common_chars, words))

if __name__ == "__main__":
    flag = main()
    log.success("Flag: {}".format(flag.decode("ascii")))
```

Output:

```console
root@kali:/media/sf_CTFs/checkpoint/Tricky_Guess# python3 solve.py
[+] Opening connection to tricky-guess.csa-challenge.com on port 2222: Done
[*] Number of words: 10000
[*]     Guessing: 'onelqbpihkxf'
[*]     Number of common characters: 3
[*] Number of words: 450
[*]     Guessing: 'frgztsejmlda'
[*]     Number of common characters: 7
[*] Number of words: 144
[*]     Guessing: 'udnltwjasxmv'
[*]     Number of common characters: 6
[*] Number of words: 45
[*]     Guessing: 'sanzfryvgudb'
[*]     Number of common characters: 5
[*] Number of words: 6
[*]     Guessing: 'weuyzqmtogjd'
[*]     Number of common characters: 8
[*] Number of words: 1
[*]     Guessing: 'kydwmfctzqja'
[+] Flag: csa{4ll_th3sE_3v1l_c475_g3n3r4T1nG_w0rd5}
```