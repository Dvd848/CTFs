# Robots Rule
Web, 100 points

## Description:

A website was linked to.

```html
<html>
<head>
<style>
	div {text-align: center;}
	img {display: block; margin-left: auto; margin-right: auto; height: 45%; width: 35%}
	html {height: 100%; background: url(Background.jpg) no-repeat center center; background-size: cover; }
</style>
</head>
<body>
	<div>			
		<h1>DOWN WITH ROBOTS!</h1>
	</div>
	<img src="Not_A_Robot.png">
	<div>
		<h1>HUMANS ARE THE BEST!</h1>
	</div>
	<img src="Down_With_Robots.gif"> 
</body>
</html>
```

## Solution:

The title is a pretty thick hint to go check out `robots.txt`:

```console
root@kali:/media/sf_CTFs/tamu/Robots_Rule# curl http://web5.tamuctf.com/robots.txt
User-agent: *

WHAT IS UP, MY FELLOW HUMAN!
HAVE YOU RECEIVED SECRET INFORMATION ON THE DASTARDLY GOOGLE ROBOTS?!
YOU CAN TELL ME, A FELLOW NOT-A-ROBOT!
```

The whole theme of the site is too anti-robot-pro-human. They also mention Google-Bot. What will happen if we try to access the site as a Google Bot instead, will we still get such anti-robot propaganda?

```console
root@kali:~# curl -A "Googlebot" http://web5.tamuctf.com/robots.txt
User-agent: *

THE HUMANS SUSPECT NOTHING!
HERE IS THE SECRET INFORMATION: gigem{be3p-bOop_rob0tz_4-lyfe}
LONG LIVE THE GOOGLEBOTS!
```