# Artisinal Handcrafted HTTP 3
Web Exploitation, 300 points

## Description:
> We found a hidden flag server hiding behind a proxy, but the proxy has some... _interesting_ ideas of what qualifies someone to make HTTP requests. Looks like you'll have to do this one by hand. Try connecting via nc 2018shell3.picoctf.com 29621, and use the proxy to send HTTP requests to `flag.local`. We've also recovered a username and a password for you to use on the login page: `realbusinessuser`/`potoooooooo`. 

## Solution:

This challenge requires us to handcraft HTTP requests in order to log into a remote page.

The following script solves the challenge:

```python
from pwn import *
import re

def get(url, headers = None):
    http = "GET {} HTTP/1.1\r\nHost: flag.local\r\nConnection: keep-alive\r\n".format(url)
    if headers is not None: 
        for header in headers:
            http += header + "\r\n"
    http += "\r\n"
    log.info("HTTP GET Request:\n{}".format(http))
    return http


def post(url, content):
    http = """POST {} HTTP/1.1
Host: flag.local
Content-Type: application/x-www-form-urlencoded
Content-Length: {}
Connection: keep-alive

{}
""".format(url, len(content), content)
    log.info("HTTP POST Request:\n{}".format(http))
    return http


r = remote("2018shell3.picoctf.com", 29621)
print r.recvuntil("\n\n>")
answer = raw_input("Answer? ")
r.send(answer)

r.recvuntil("HTTP.")


r.send(get("/login"))
res = r.recvuntil("</html>")
log.info("Received:\n{}".format(res))


r.send(post("/login", "user=realbusinessuser&pass=potoooooooo"))
res = r.recvuntil("Redirecting to /")
log.info("Received:\n{}".format(res))

match = re.search("set-cookie: real_business_token=([^;]+);", res, re.M)
cookie = match.group(1)
log.info("Cookie: {}".format(cookie))

cookie = "Cookie: real_business_token={};".format(cookie)

r.send(get("/", headers = [cookie]))
res = r.recv(timeout=6)
log.info("Received:\n{}".format(res))


print r.recvall()
```

The output is:
```console
root@kali:/media/sf_CTFs/pico/Artisinal_Handcrafted# python solve.py
[+] Opening connection to 2018shell3.picoctf.com on port 29621: Done
Real Business Corp., Internal Proxy
Version 2.0.7
To proceed, please solve the following captcha:

 __             ____
/  |     _     / ___|  ______
`| |   _| |_  / /___  |______|
 | |  |_   _| | ___ \  ______
_| |_   |_|   | \_/ | |______|
\___/         \_____/




>
Answer? 7
[*] HTTP GET Request:
    GET /login HTTP/1.1
    Host: flag.local
    Connection: keep-alive

[*] Received:


    HTTP/1.1 200 OK
    x-powered-by: Express
    content-type: text/html; charset=utf-8
    content-length: 498
    etag: W/"1f2-UE5AGAqbLVQn1qrfKFRIqanxl9I"
    date: Tue, 06 Nov 2018 18:54:17 GMT
    connection: keep-alive


            <html>
                <head>
                    <link rel="stylesheet" type="text/css" href="main.css" />
                </head>
                <body>
                    <header>
                        <h1>Real Business Internal Flag Server</h1>
                        <a href="/login">Login</a>
                    </header>
                    <main>
                        <h2>Log In</h2>

                        <form method="POST" action="login">
                            <input type="text" name="user" placeholder="Username" />
                            <input type="password" name="pass" placeholder="Password" />
                            <input type="submit" />
                        </form>
                    </main>
                </body>
            </html>
[*] HTTP POST Request:
    POST /login HTTP/1.1
    Host: flag.local
    Content-Type: application/x-www-form-urlencoded
    Content-Length: 38
    Connection: keep-alive

    user=realbusinessuser&pass=potoooooooo
[*] Received:

            HTTP/1.1 302 Found
    x-powered-by: Express
    set-cookie: real_business_token=PHNjcmlwdD5hbGVydCgid2F0Iik8L3NjcmlwdD4%3D; Path=/
    location: /
    vary: Accept
    content-type: text/plain; charset=utf-8
    content-length: 23
    date: Tue, 06 Nov 2018 18:54:17 GMT
    connection: keep-alive

    Found. Redirecting to /
[*] Cookie: PHNjcmlwdD5hbGVydCgid2F0Iik8L3NjcmlwdD4%3D
[*] HTTP GET Request:
    GET / HTTP/1.1
    Host: flag.local
    Connection: keep-alive
    Cookie: real_business_token=PHNjcmlwdD5hbGVydCgid2F0Iik8L3NjcmlwdD4%3D;

[*] Received:
    HTTP/1.1 200 OK
    x-powered-by: Express
    content-type: text/html; charset=utf-8
    content-length: 438
    etag: W/"1b6-bgxSS92CBVm1uJx+NK7DdppIBp8"
    date: Tue, 06 Nov 2018 18:54:18 GMT
    connection: keep-alive


            <html>
                <head>
                    <link rel="stylesheet" type="text/css" href="main.css" />
                </head>
                <body>
                    <header>
                        <h1>Real Business Internal Flag Server</h1>
                        <div class="user">Real Business Employee</div>
                        <a href="/logout">Logout</a>
                    </header>
                    <main>
                        <p>Hello <b>Real Business Employee</b>!  Today's flag is: <code>picoCTF{0nLY_Us3_n0N_GmO_xF3r_pR0tOcol5_5f5f}</code>.</p>
                    </main>
                </body>
            </html>

[+] Receiving all data: Done (0B)
[*] Closed connection to 2018shell3.picoctf.com port 29621
```

The flag: picoCTF{0nLY_Us3_n0N_GmO_xF3r_pR0tOcol5_5f5f}