# rsa-madlibs
Binary Exploitation, 250 points

## Description:
> We ran into some weird puzzles we think may mean something, can you help me solve one?

## Solution:

This challenge consists of a series of RSA-related question. 

for example:

```
root@kali:/media/sf_CTFs/pico/rsa-madlibs# nc 2018shell3.picoctf.com 18148
0x7069636f4354467b64305f755f6b6e30775f7468335f7740795f325f5253405f62333862653138617dL <type 'long'>
Hello, Welcome to RSA Madlibs
Keeping young children entertained, since, well, nev3r
Tell us how to fill in the blanks, or if it's even possible to do so
Everything, input and output, is decimal, not hex
#### NEW MADLIB ####
q : 93187
p : 94603
##### WE'RE GONNA NEED THE FOLLOWING ####
n
IS THIS POSSIBLE and FEASIBLE? (Y/N):
```

For each question, we are required to answer if it is "POSSIBLE and FEASIBLE", and if so, provide the answer.

There are many sites which contains the relevant RSA math formulates, e.g. [Wikipedia](https://en.wikipedia.org/wiki/RSA_(cryptosystem)) and [dCode](https://www.dcode.fr/rsa-cipher).

The following script solves the challenge:

```python
from pwn import *
import gmpy2

PARAM_REGEX = re.compile("^(\w+)\s*:\s*(\d+)\s*$")
def get_parameters(r):
    parameters = {}
    r.recvuntil("#### NEW MADLIB ####\n")
    lines = r.recvuntil("##### WE'RE GONNA NEED THE FOLLOWING ####\n")
    for line in lines.split("\n"):
        match = PARAM_REGEX.match(line)
        if match:
            parameter, value = match.groups()
            parameters[parameter] = int(value)
    requsted = r.recvline()
    log.info("Requesting '{}' using {}".format(requsted.strip(), parameters.keys()))
    return parameters

def send_answer(r, answer):
    r.recvuntil("(Y/N):")
    if answer is None:
        r.sendline("n")
        return
    r.sendline("y")
    r.recvuntil(":")
    r.sendline(str(answer))


class MadLibsFunc:
    def __init__(self):
        self.registry = []

    def __call__(self, m):
        self.registry.append(m)
        return m

mad_libs_func = MadLibsFunc()

r = remote("2018shell3.picoctf.com", 18148)

@mad_libs_func
def phase1():
    params = get_parameters(r)
    res = params["p"] * params["q"]
    send_answer(r, res)
    return res

@mad_libs_func
def phase2():
    params = get_parameters(r)
    res = params["n"] // params["p"]
    send_answer(r, res)
    return res

@mad_libs_func
def phase3():
    params = get_parameters(r)
    res = None
    send_answer(r, res)
    return res

@mad_libs_func
def phase4():
    params = get_parameters(r)
    res = (params["p"] - 1) * (params["q"] - 1)
    send_answer(r, res)
    return res

@mad_libs_func
def phase5():
    params = get_parameters(r)
    res = pow(params["plaintext"], params["e"], params["n"])
    send_answer(r, res)
    return res

@mad_libs_func
def phase6():
    params = get_parameters(r)
    res = None
    send_answer(r, res)
    return res

@mad_libs_func
def phase7():
    params = get_parameters(r)
    n = params["p"] * params["q"]
    ph = (params["p"] - 1) * (params["q"] - 1)
    d = gmpy2.invert(params["e"], ph)
    res = d
    send_answer(r, res)
    return res


@mad_libs_func
def phase8():
    params = get_parameters(r)
    q = params["n"] // params["p"]
    ph = (params["p"] - 1) * (q - 1)
    d = gmpy2.invert(params["e"], ph)
    plaintext = pow(params["ciphertext"], d, params["n"])
    res = plaintext
    send_answer(r, res)
    return res

results = []
for func in mad_libs_func.registry:
    results.append(func())

print r.recvall()

plaintext = results[-1]
log.success(unhex(format(plaintext, 'x')))

```

Output:
```console
root@kali:/media/sf_CTFs/pico/rsa-madlibs# python solve.py
[+] Opening connection to 2018shell3.picoctf.com on port 18148: Done
[*] Requesting 'n' using ['q', 'p']
[*] Requesting 'q' using ['p', 'n']
[*] Requesting 'q' using ['e', 'n']
[*] Requesting 'totient(n)' using ['q', 'p']
[*] Requesting 'ciphertext' using ['plaintext', 'e', 'n']
[*] Requesting 'plaintext' using ['ciphertext', 'e', 'n']
[*] Requesting 'd' using ['q', 'p', 'e']
[*] Requesting 'plaintext' using ['p', 'ciphertext', 'e', 'n']
[+] Receiving all data: Done (145B)
[*] Closed connection to 2018shell3.picoctf.com port 18148
 YAHHH! That one was a great madlib!!!


If you convert the last plaintext to a hex number, then ascii, you'll find what you're searching for ;)

[+] picoCTF{d0_u_kn0w_th3_w@y_2_RS@_b38be18a}
```

The flag: picoCTF{d0_u_kn0w_th3_w@y_2_RS@_b38be18a}