# shortcut

 * Category: Crypto
 * Solved by the JCTF Team

## Description

> Login to get the flag :)
> 
> Take some shortcuts if you find them.

```python
from flask import Flask, request, abort
import bcrypt
import os
from password import PASSWORD

FLAG = os.getenv("FLAG", "lol{fake_flag_for_testing}")

# Initialize the Flask application
app = Flask(__name__)

def auth(params):
    salt = bcrypt.gensalt(rounds=12)
    version = str(params["api_version"]).encode()
    expected = bcrypt.hashpw(version + b"admin" + PASSWORD.encode(), salt)
    received = bcrypt.hashpw(version + params["user"].encode() + params["pass"].encode(), salt)
    return expected == received

def validate_params(params):
    if any([x not in params for x in ["user", "pass", "cmd", "api_version"]]):
        return False

    if params["cmd"] != "get_flag": # only one command is supported right now
        return False
    if len(params["api_version"]) != 1: # version is only one char
        return False
    return True

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'GET':
        params = request.args
    elif request.method == 'POST':
        params = request.get_json()
    else:
        # should never reach here...
        abort(500)

    if not validate_params(params):
        abort(422)
    if not auth(params):
        abort(401)

    return FLAG + "\n"

```

## Solution

The server exposes a `/login` endpoint that requires four parameters: `user`, `pass`, `cmd` and `api_version`.

Authentication is done using the following code:

```python
def auth(params):
    salt = bcrypt.gensalt(rounds=12)
    version = str(params["api_version"]).encode()
    expected = bcrypt.hashpw(version + b"admin" + PASSWORD.encode(), salt)
    received = bcrypt.hashpw(version + params["user"].encode() + params["pass"].encode(), salt)
    return expected == received
```

This function uses `bcrypt` to check the password correctness:

> bcrypt is a password-hashing function designed by Niels Provos and David Mazières. It is based on the Blowfish cipher and presented at USENIX in 1999. Besides incorporating a salt to protect against rainbow table attacks, bcrypt is an adaptive function: over time, the iteration count can be increased to make it slower, so it remains resistant to brute-force search attacks even with increasing computation power. [Wikipedia](https://en.wikipedia.org/wiki/Bcrypt)

It's a known fact about `bcrypt` that many implementations truncate the password to the first 72 bytes, following the OpenBSD implementation.

This means that if we can somehow control the first 72 characters passed to `bcrypt` for both the `expected` and `received` hashes,
we can ensure that they are equal, since we'll essentially "push" away the `PASSWORD` to the area that doesn't get included in the `expected` hash.

We control the `version` parameter, why don't we send a `72`-character string and be done with it? 
The reason is the validation function called before `auth`:

```python
def validate_params(params):
    if any([x not in params for x in ["user", "pass", "cmd", "api_version"]]):
        return False

    if params["cmd"] != "get_flag": # only one command is supported right now
        return False
    if len(params["api_version"]) != 1: # version is only one char
        return False
    return True
```


It ensures that `len(params["api_version"]) == 1` and fails otherwise.

Luckily, there's a type confusion bug that we can exploit to bypass this check.  
Notice how `POST` requests are allowed to send JSON structure, while `GET` requests
are limited to string parameters. For a `GET` request, we'll have to send a single character
in order to successfully pass the parameter validation. However, using a `POST` request,
we could send an array with one member, having `len(params["api_version"])` return `1` as well!  
Once we cause the validation restriction to apply to the length of the array and not the contents
of the array, we are free to provide a member of any length. We'll provide a long string,
having `version = str(params["api_version"]).encode()` push away the password as needed.

The final exploit:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/shortcut]
└─$ curl -X POST https://bstlv25-shortcut.chals.io/login \
  -H "Content-Type: application/json" \
  -d '{
        "api_version": ["AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"],
        "user": "admin",
        "pass": "",
        "cmd": "get_flag"
      }'
BSidesTLV2025{b3tter_t0_r3turn_4n_err0r}
```
