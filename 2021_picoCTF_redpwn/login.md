# login
Web, 100 points

## Description

> My dog-sitter's brother made this website but I can't get in; can you help?

## Solution

We enter the website and see a simple login form:

```html
<!doctype html>
<html>
    <head>
        <link rel="stylesheet" href="styles.css">
        <script src="index.js"></script>
    </head>
    <body>
        <div>
          <h1>Login</h1>
          <form method="POST">
            <label for="username">Username</label>
            <input name="username" type="text"/>
            <label for="username">Password</label>
            <input name="password" type="password"/>
            <input type="submit" value="Submit"/>
          </form>
        </div>
    </body>
</html>
```

Let's check the Javascript file:

```javascript
(async () => {
    await new Promise(
        (e => window.addEventListener("load", e))), document.querySelector("form").addEventListener("submit", (e => {
        e.preventDefault();
        const r = {
                u: "input[name=username]",
                p: "input[name=password]"
            },
            t = {};
        for (const e in r) t[e] = btoa(document.querySelector(r[e]).value).replace(/=/g, "");
        return "YWRtaW4" !== t.u ? alert("Incorrect Username") : "cGljb0NURns1M3J2M3JfNTNydjNyXzUzcnYzcl81M3J2M3JfNTNydjNyfQ" !== t.p ? alert("Incorrect Password") : void alert(`Correct Password! Your flag is ${atob(t.p)}.`)
    }))
})();
```

We can easily see that the username and password are compared to hardcoded values. We also see that the flag is the decoded password:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/login]
└─$ echo cGljb0NURns1M3J2M3JfNTNydjNyXzUzcnYzcl81M3J2M3JfNTNydjNyfQ== | base64 -d
picoCTF{53rv3r_53rv3r_53rv3r_53rv3r_53rv3r}
```