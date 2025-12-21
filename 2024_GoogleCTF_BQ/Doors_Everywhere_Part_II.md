# Doors Everywhere (Part II)

Category: Web

## Description

> There are a few news articles there, but there's something odd about their URLs...

## Solution

This is the second part of the Doors Everywhere challenge. After logging in, one of the things
listed on the page is the following news articles:

```html
         
<li>2024-06-30 - <a href="/news/7">Secret Messages are Live</a></li>
<li>2024-05-04 - <a href="/news/5">Star Wars Day Celebrations</a></li>
<li>2024-04-09 - <a href="/news/4">Unicorn Day Celebrations</a></li>
<li>2024-04-04 - <a href="/news/3">We don&#39;t have a quantum computer</a></li>
<li>2024-04-01 - <a href="/news/2">April Fools: Moving to new quantum computer servers</a></li>
<li>2024-03-14 - <a href="/news/1">Pie Day Celebrations</a></li>
```

What's missing? New article #6, of course!

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Doors_Everywhere_Part_II]
└─$ curl 'https://secuweb-web.2024-bq.ctfcompetition.com/news/6'  -H "Cookie: session=$session" -L -s | grep CTF
CTF{DeletedButN0tG0n3}
```