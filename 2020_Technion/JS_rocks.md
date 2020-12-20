# JS rocks
Category: Web

## Description
> Part 1 of Imperva's challenge
> 
> Put your JavaScript to the test, can you find the flag?
> 

## Solution

We enter the attached website and are greeted with a login form:
```html
<div class="container h-100">
  <div class="row h-100 justify-content-center align-items-center">
    <div class="wrapper">
        <h2>Login</h2>
        <p>Please fill in your credentials to login.</p>
        <form action="/login.php" method="post">
        <span class="help-block"></span>
            <div class="form-group">
                <label>Username</label>
                <input type="text" name="username" class="form-control" value="">
            </div>    
            <div class="form-group">
                <label>Password</label>
                <input type="password" name="password" class="form-control">
            </div>
            <div class="form-group">
                <input type="submit" class="btn btn-primary" value="Login">
            </div>
            <p>Don't have an account? <a href="register.php">Sign up now</a>.</p>
        </form>
    </div>
    </div>
    </div>
<script src="js/secret.js"></script>
```

Challenge name refers to JS, so obviously the next step would be to check out `js/secret.js`:

```javascript
var s="var impvcs=[\'\\x5f\\x6b\',\"\\x65\\x78\",\"\\x72\\x65\",\"\\x62\\x6f\\x64\",\"\\x73\\x70\",\'\\x35\\x2e\\x70\',\"\\x2e\\x74\",\'\\x66\\x75\\x6e\\x63\\x74\\x69\\x6f\\x6e\',\'\\x68\\x70\',\'\\x79\\x30\\x75\',\'\\x74\\x69\\x6d\\x65\',\'\\x69\\x2f\',\'\\x6e\\x30\\x77\',\"\\x74\\x28\\x29\",\'\\x5f\\x6a\',\'\\x2f\\x61\\x70\',\"\\x6f\\x6e\\x73\"]; var impvg0 = impvcs[15]+impvcs[11]; var impvg1 = impvcs[9]+impvcs[0]+impvcs[12]+impvcs[14]+impvcs[5]+impvcs[8]; var impvg2 = impvg0+impvg1; var impvg3 = impvcs[2]+impvcs[4]+impvcs[16]+\"e\" + impvcs[6]+impvcs[1]+impvcs[13]; var impvg4 = impvcs[3]+\"y\"; fetch(impvg2) .then(function (response) { return eval(impvg3); }) .then(function (body) { console.log(eval(impvg4)); });"
```

We get an obfuscated Javascript snippet. Fortunately, we don't really need to reverse it, we can just run it:

```
>> eval(s)
Promise { <state>: "pending" }

{"flag":"Y3N0ZWNobmlvbntjYW5feW91X2J5cGFzc190aGVfbG9naW59"}
```

We decode as base64 and get the flag:

```console
root@kali:/media/sf_CTFs/technion/JS_rocks# echo Y3N0ZWNobmlvbntjYW5feW91X2J5cGFzc190aGVfbG9naW59 | base64 -d
cstechnion{can_you_bypass_the_login}
```

The flag is a hint for the [next challenge](Hard_login.md).