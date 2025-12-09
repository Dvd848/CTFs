# Noncense

 * Category: Crypto
 * Solved by the JCTF Team

## Description

> This web app uses digital signatures to secure its session cookies - but one particular username seems off limits.
> 
> Can you find a way to bypass the restrictions and claim the flag?

Sources were attached.

<details>

<summary>Expand sources...</summary>

### app.py

```python
from flask import Flask, request, g, render_template, make_response, redirect, url_for
import sqlite3
from crypto import get_token, verify
import os
from base64 import urlsafe_b64decode

app = Flask(__name__)
DATABASE = 'users.db'
AUTHORIZED_USERNAME = "Da Goat"

FLAG = os.getenv("FLAG", "lol{fake_flag_for_testing}")


conn = sqlite3.connect(DATABASE)
cursor = conn.cursor()
cursor.execute('''CREATE TABLE IF NOT EXISTS users (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    username TEXT UNIQUE NOT NULL,
                    password TEXT NOT NULL)''')
conn.commit()

def is_authorized(token) -> bool:
    if verify(token):
        msg = urlsafe_b64decode(token.split(".")[0].encode())
        username = msg.decode()
        return username == AUTHORIZED_USERNAME
    return False   

@app.route('/')
def index():
    error = request.args.get('error', '')
    msg = request.args.get('msg', '')
    return render_template('index.html', error=error, msg=msg)

@app.route('/flag')
def get_flag():
    token = request.cookies.get("session")
    if(is_authorized(token)):
        msg = FLAG
    else:    
        msg = 'Not for you🤣'
    return render_template('flag.html', flag=msg)

@app.route('/login', methods=['POST'])
def login():
    username = request.form.get('username')
    password = request.form.get('password')
    db = sqlite3.connect(DATABASE)
    cursor = db.cursor()
    query = f"SELECT username FROM users WHERE username = ? AND password = ?"
    try:
        cursor.execute(query, (username, password))
        user = cursor.fetchone()
        if user:
            resp = make_response(render_template('flag.html', flag=""))
            username = user[0]
            resp.set_cookie("session", get_token(username))
            return resp
        else:
            error_msg = "Invalid credentials."
    except (sqlite3.Error, sqlite3.Warning) as e:
        print(f"SQLite error: {str(e)}")
        error_msg = "Invalid credentials."

    except Exception as e: 
        error_msg = "Unknown error occured. Try again later"
    return redirect(url_for("index", error=error_msg))  


@app.route('/signup', methods=['POST'])
def signup():
    username = request.form.get('username')
    password = request.form.get('password')
    if not username or not password:
        return redirect(url_for("index", error="Username and password required"))  
    if username == AUTHORIZED_USERNAME:
        return redirect(url_for("index", error=f"You can't register as {AUTHORIZED_USERNAME}."))  
    db = sqlite3.connect(DATABASE)
    cursor = db.cursor()
    try:
        cursor.execute('INSERT INTO users (username, password) VALUES (?, ?)', (username, password))
        db.commit()
        return redirect(url_for("index", msg="User registered successfully"))  
    except sqlite3.IntegrityError:
        return redirect(url_for("index", error="Username already exists"))  
    
if __name__ == '__main__':
    app.run()
```

### crypto.py

```python
from hashlib import sha256
import base64
import os
from Crypto.Util.number import bytes_to_long
from collections import namedtuple
from hashlib import sha256
import binascii
import zlib
import random

def generate_nonce(msg: bytes, rand: int):
    rand = hash(rand) #for extra randomness
    return zlib.crc32(msg) ^ rand

def create_signature(msg: bytes) -> bytes:
    return base64.urlsafe_b64encode(sign(msg))
    
def get_token(data: str) -> str:
    msg_to_sign = data.encode()
    signature = create_signature(msg_to_sign)
    return base64.urlsafe_b64encode(msg_to_sign).decode() + "." + signature.decode()

 
Point = namedtuple("Point", "x y")
O = 'Infinity'

def check_point(P):
    if P == O:
        return True
    else:
        return (P.y**2 - (P.x**3 + a*P.x + b)) % p == 0 and 0 <= P.x < p and 0 <= P.y < p

def point_inverse(P):
    if P == O:
        return P
    return Point(P.x, -P.y % p)

def point_addition(P, Q):
    if P == O:
        return Q
    elif Q == O:
        return P
    elif Q == point_inverse(P):
        return O
    else:
        if P == Q:
            lam = (3*P.x**2 + a)*pow(2*P.y, -1, p)
            lam %= p
        else:
            lam = (Q.y - P.y) * pow((Q.x - P.x), -1, p)
            lam %= p
    Rx = (lam**2 - P.x - Q.x) % p
    Ry = (lam*(P.x - Rx) - P.y) % p
    R = Point(Rx, Ry)
    assert check_point(R)
    return R


def point_multiplication(P, n):
    Q = P
    R = O
    while n > 0:
        if n % 2 == 1:
            R = point_addition(R, Q)
        Q = point_addition(Q, Q)
        n = n // 2
    assert check_point(R)
    return R


# Standard ECDSA operations
def number_to_string(num):
    l = p.bit_length() // 8
    fmt_str = "%0" + str(2 * l) + "x"
    string = binascii.unhexlify((fmt_str % num))
    assert len(string) == l, (len(string), l)
    return string

def string_to_number(string: bytes):
    return int(binascii.hexlify(string), 16)

def hash_message(message):
    return bytes_to_long(sha256(message).digest())

def sign(message: bytes, k=None):
    if k is None:
        k = generate_nonce(message, rand=os.urandom)
    print("k=", k)    
    hash = hash_message(message)
    print("hash=", hash)
    r = point_multiplication(G, k).x % n
    print("r=", r)
    s = pow(k, -1, n) * (hash + r * private_key) % n
    print("s=", s)
    return number_to_string(r) + number_to_string(s)


def verify(token: str):
    message, signature = token.split(".")
    message = base64.urlsafe_b64decode(message.encode())
    signature = base64.urlsafe_b64decode(signature.encode())
    r = string_to_number(signature[:len(signature)//2])
    s = string_to_number(signature[len(signature)//2:])
    if r < 1 or r > n - 1 or s < 1 or s > n-1:
        return False
    hash = hash_message(message)
    u1 = (hash * pow(s, -1, n)) % n
    u2 = (r * pow(s, -1, n)) % n
    P = point_addition(
        point_multiplication(G, u1), 
        point_multiplication(public_key, u2)
        )
    return P.x % n == r


a = 0xffffffff00000001000000000000000000000000fffffffffffffffffffffffc
b = 0x5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b
p = 0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff
G = Point(0x6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296, 0x4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5)
n = 0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551

private_key = random.randrange(n)
public_key = point_multiplication(G, private_key)
```

</details>

## Solution

The application signs authentication cookies using a custom ECDSA-like scheme. The goal is to forge a valid cookie for the privileged user `Da Goat`.

Access to `/flag` is granted only if a valid signature for this exact username is provided.

The authentication token is generated as:

```python
token = base64(username) + "." + base64(signature)
```

Where the signature is produced by:

```python
def sign(message: bytes, k=None):
    if k is None:
        k = generate_nonce(message, rand=os.urandom)
    hash = hash_message(message)
    r = point_multiplication(G, k).x % n
    s = pow(k, -1, n) * (hash + r * private_key) % n
```

The critical weakness is related to the call to `generate_nonce`:

```python
def generate_nonce(msg: bytes, rand: int):
    rand = hash(rand)
    return zlib.crc32(msg) ^ rand
```

`generate_nonce` expects an integer as the value of `rand`, while `sign` sends it a function pointer.

```python
>>> import os
>>> print(os.urandom)
<built-in function urandom>
```

This means that `hash(os.urandom)` becomes a fixed constant per process. In other words,
the nonce `k` becomes `k(message) = crc32(message) ^ C` for some constant `C`.
This makes the nonce deterministic and collidable via CRC32.

Once we find two different usernames with the same CRC32, they will not only use the same nonce `k`,
but also produce two signatures with the same `r`, allowing us to recover the ECDSA private key.

Given two signatures for two messages (where `h1`, `h2` are the hashes for the messages):
```
s1 = k⁻¹(h1 + r·d)
s2 = k⁻¹(h2 + r·d)
```

We can compute the following to recover the private key `d`:

```
k = (h1 - h2) · (s1 - s2)⁻¹ mod n
d = (k·s1 - h1) · r⁻¹ mod n
```

At this point we can use `d` to sign any token, specifically one for "Da Goat" which is
the user we need to impersonate in order to get the flag:

```python
AUTHORIZED_USERNAME = "Da Goat"

@app.route('/flag')
def get_flag():
    token = request.cookies.get("session")
    if(is_authorized(token)):
        msg = FLAG
    else:    
        msg = 'Not for you🤣'
    return render_template('flag.html', flag=msg)
```

The following AI-generated script puts it all together:

```python
#!/usr/bin/env python3
import base64
import random
import requests
import zlib
import sys
import re

# Import as much logic as possible from the challenge crypto
from crypto import (
    number_to_string,
    string_to_number,
    hash_message,
    point_multiplication,
    G,
    n,
)

# -----------------------------
# CONFIG
# -----------------------------

BASE_URL   = "https://bstlv25-noncense.chals.io"
SIGNUP_URL = BASE_URL + "/signup"
LOGIN_URL  = BASE_URL + "/login"
FLAG_URL   = BASE_URL + "/flag"

AUTHORIZED_USERNAME = "Da Goat"

# Precomputed CRC32 collision pair (same zlib.crc32)
USER1 = "h58r"
USER2 = "k5bZz"


# -----------------------------
# Helper: token encoding/decoding
# -----------------------------

def b64url_decode(s: str) -> bytes:
    # Server uses urlsafe_b64encode, so this matches it
    return base64.urlsafe_b64decode(s.encode())


def b64url_encode(b: bytes) -> str:
    return base64.urlsafe_b64encode(b).decode()


def parse_token(token: str):
    """
    token = base64url(username) + "." + base64url(signature)
    signature = number_to_string(r) || number_to_string(s)
    """
    msg_b64, sig_b64 = token.split(".")
    msg = b64url_decode(msg_b64)
    sig = b64url_decode(sig_b64)

    # crypto.sign() returns r||s where each is number_to_string() (P-256 -> 32 bytes each)
    assert len(sig) % 2 == 0
    half = len(sig) // 2
    r = string_to_number(sig[:half])
    s = string_to_number(sig[half:])
    h = hash_message(msg)
    return msg, r, s, h


# -----------------------------
# HTTP helpers
# -----------------------------

def signup(session: requests.Session, username: str, password: str):
    print(f"[+] Signup {username!r}")
    r = session.post(
        SIGNUP_URL,
        data={"username": username, "password": password},
        allow_redirects=False,
    )
    print("    status:", r.status_code)


def login_get_cookie(session: requests.Session, username: str, password: str) -> str:
    print(f"[+] Login {username!r}")
    r = session.post(
        LOGIN_URL,
        data={"username": username, "password": password},
        allow_redirects=False,
    )
    print("    status:", r.status_code)
    if "session" not in r.cookies:
        raise RuntimeError("No 'session' cookie in response")
    token = r.cookies["session"]
    print("    session:", token)
    return token


# -----------------------------
# Crypto attack: recover d from nonce reuse
# -----------------------------

def recover_private_key(token1: str, token2: str) -> int:
    m1, r1, s1, h1 = parse_token(token1)
    m2, r2, s2, h2 = parse_token(token2)

    print(f"[+] msg1 = {m1!r}")
    print(f"[+] msg2 = {m2!r}")

    if r1 != r2:
        raise RuntimeError("r1 != r2 (nonces differ) – CRC32 collision failed")

    r = r1
    print(f"[+] shared r = {hex(r)}")

    # Classic ECDSA nonce-reuse equations:
    # k = (h1 - h2) * (s1 - s2)^(-1) mod n
    # d = (k*s1 - h1) * r^(-1) mod n
    num = (h1 - h2) % n
    den = (s1 - s2) % n
    k = (num * pow(den, -1, n)) % n
    print(f"[+] recovered k = {hex(k)}")

    d = ((k * s1 - h1) * pow(r, -1, n)) % n
    print(f"[+] recovered private key d = {hex(d)}")
    return d


# -----------------------------
# Sign arbitrary message with recovered d
# (using curve logic imported from crypto.py)
# -----------------------------

def ecdsa_sign_with_d(message: bytes, d: int):
    """
    ECDSA sign using the same curve & encoding as the server,
    but with our recovered private key d and our own nonce k.
    """
    h = hash_message(message)
    while True:
        # Any non-zero random k in [1, n-1] is fine here
        k = random.randrange(1, n)
        R = point_multiplication(G, k)
        r = R.x % n
        if r == 0:
            continue
        s = (pow(k, -1, n) * (h + r * d)) % n
        if s == 0:
            continue
        return r, s


def build_token_for_username(username: str, d: int) -> str:
    msg = username.encode()
    r, s = ecdsa_sign_with_d(msg, d)
    sig = number_to_string(r) + number_to_string(s)
    token = b64url_encode(msg) + "." + b64url_encode(sig)
    print(f"[+] forged token for {username!r}: {token}")
    return token


# -----------------------------
# Use forged token to get flag
# -----------------------------

def get_flag_with_token(token: str):
    print("[+] Fetching /flag with forged cookie...")
    s = requests.Session()
    r = s.get(FLAG_URL, cookies={"session": token})

    # Look for BSidesTLV2025{...}
    m = re.search(r"BSidesTLV2025\{[^}]*\}", r.text)
    if m:
        print(m.group(0))
    else:
        print("[!] Flag not found in response")


# -----------------------------
# Main end-to-end exploit
# -----------------------------

def main():
    # sanity check: CRC32 collision usernames really collide
    c1 = zlib.crc32(USER1.encode()) & 0xffffffff
    c2 = zlib.crc32(USER2.encode()) & 0xffffffff
    print(f"[*] CRC32({USER1}) = {hex(c1)}")
    print(f"[*] CRC32({USER2}) = {hex(c2)}")
    assert c1 == c2, "Hard-coded usernames don't collide, aborting"

    pwd = "Passw0rd!!"
    sess = requests.Session()

    # 1. Signup both users (ignore 'already exists' errors; status 302 is fine)
    signup(sess, USER1, pwd)
    signup(sess, USER2, pwd)

    # 2. Login as both and grab their session cookies
    t1 = login_get_cookie(sess, USER1, pwd)
    t2 = login_get_cookie(sess, USER2, pwd)

    # 3. Recover private key from two signatures with same nonce
    d = recover_private_key(t1, t2)

    # 4. Forge a valid token for "Da Goat"
    forged = build_token_for_username(AUTHORIZED_USERNAME, d)

    # 5. Use forged cookie to grab the flag
    get_flag_with_token(forged)


if __name__ == "__main__":
    main()

```

Output:

```console
┌──(py_ctf_env)─(user@kali3)-[/media/sf_CTFs/bsides/Noncense]
└─$ python3 exploit.py
[*] CRC32(h58r) = 0x54e28b60
[*] CRC32(k5bZz) = 0x54e28b60
[+] Signup 'h58r'
    status: 302
[+] Signup 'k5bZz'
    status: 302
[+] Login 'h58r'
    status: 200
    session: aDU4cg==.LdTSjF4rb0e01uw51tjbdFklEuVlSAhKkbl2Ga_rB3z6JWhWMH0bCPLiLy19YOR_F0o-d5XTA2W9UtZIW-1wKw==
[+] Login 'k5bZz'
    status: 200
    session: azViWno=.LdTSjF4rb0e01uw51tjbdFklEuVlSAhKkbl2Ga_rB3zSES7D97VVnzIQv6PyaXw30NSD_BFzTEgWKSclJtM33g==
[+] msg1 = b'h58r'
[+] msg2 = b'k5bZz'
[+] shared r = 0x2dd4d28c5e2b6f47b4d6ec39d6d8db74592512e56548084a91b97619afeb077c
[+] recovered k = 0x54e07e7c
[+] recovered private key d = 0xd4d322ed0752930dd11bf1d77d5274676e19b2fbf094dbd7cdb5893a0e931238
[+] forged token for 'Da Goat': RGEgR29hdA==.Nbp-H2qpCkHG2uT4wyeRnlN32OZMVu6seEqnZTlB0k-Vjs-SXyY6G2EW_z56tsR6rE_0helRZ4JTYX-TOPGIXQ==
[+] Fetching /flag with forged cookie...
BSidesTLV2025{Y0u_br0k3_ECD5A_4Nd_mY_H3ART!}
```