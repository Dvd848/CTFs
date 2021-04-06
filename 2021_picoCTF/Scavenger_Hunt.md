# Scavenger Hunt
Category: Web Exploitation, 50 points

## Description

> There is some interesting information hidden around this site. Can you find it?


## Solution

We start from the site itself:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Scavenger_Hunt]
└─$ curl http://mercury.picoctf.net:39491/
<!doctype html>
<html>
  <head>
    <title>Scavenger Hunt</title>
    <link href="https://fonts.googleapis.com/css?family=Open+Sans|Roboto" rel="stylesheet">
    <link rel="stylesheet" type="text/css" href="mycss.css">
    <script type="application/javascript" src="myjs.js"></script>
  </head>

  <body>
    <div class="container">
      <header>
                <h1>Just some boring HTML</h1>
      </header>

      <button class="tablink" onclick="openTab('tabintro', this, '#222')" id="defaultOpen">How</button>
      <button class="tablink" onclick="openTab('tababout', this, '#222')">What</button>

      <div id="tabintro" class="tabcontent">
                <h3>How</h3>
                <p>How do you like my website?</p>
      </div>

      <div id="tababout" class="tabcontent">
                <h3>What</h3>
                <p>I used these to make this site: <br/>
                  HTML <br/>
                  CSS <br/>
                  JS (JavaScript)
                </p>
        <!-- Here's the first part of the flag: picoCTF{t -->
      </div>

    </div>

  </body>
</html>
```

We can see here the first part of the flag in the comment:

```html
<!-- Here's the first part of the flag: picoCTF{t -->
```

Let's take a look at the style sheet:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Scavenger_Hunt]
└─$ curl http://mercury.picoctf.net:39491/mycss.css -s | tail
    color: #111;
    display: none;
    padding: 50px;
    text-align: center;
}

#tabintro { background-color: #ccc; }
#tababout { background-color: #ccc; }

/* CSS makes the page look nice, and yes, it also has part of the flag. Here's part 2: h4ts_4_l0 */     
```

We got the second part. 

What about the Javascript file?

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Scavenger_Hunt]
└─$ curl http://mercury.picoctf.net:39491/myjs.js -s | tail
    if(elmnt.style != null) {
        elmnt.style.backgroundColor = color;
    }
}

window.onload = function() {
    openTab('tabintro', this, '#222');
}

/* How can I keep Google from indexing my website? */
```

We can follow the hint or use `dirsearch` to give us a list of common files publicly available:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Scavenger_Hunt]
└─$ ~/utils/web/dirsearch/dirsearch.py -u http://mercury.picoctf.net:39491/ -e *

  _|. _ _  _  _  _ _|_    v0.4.1
 (_||| _) (/_(_|| (_| )

Extensions: 1.txt | HTTP method: GET | Threads: 30 | Wordlist size: 8948

Error Log: /home/user/utils/web/dirsearch/logs/errors-21-04-05_21-06-16.log

Target: http://mercury.picoctf.net:39491/

Output File: /home/user/utils/web/dirsearch/reports/mercury.picoctf.net/_21-04-05_21-06-16.txt

[21:06:16] Starting:
[21:06:19] 200 -   62B  - /.DS_Store
[21:06:25] 200 -   95B  - /.htaccess
[21:06:25] 200 -   95B  - /.htaccess/
[21:07:24] 200 -  961B  - /index.html
[21:07:47] 200 -  124B  - /robots.txt

Task Completed

```

Let's check what we got:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Scavenger_Hunt]
└─$ curl http://mercury.picoctf.net:39491/robots.txt
User-agent: *
Disallow: /index.html
# Part 3: t_0f_pl4c
# I think this is an apache server... can you Access the next flag?

┌──(user@kali)-[/media/sf_CTFs/pico/Scavenger_Hunt]
└─$ curl http://mercury.picoctf.net:39491/.htaccess
# Part 4: 3s_2_lO0k
# I love making websites on my Mac, I can Store a lot of information there.                                                                                                                                    
┌──(user@kali)-[/media/sf_CTFs/pico/Scavenger_Hunt]
└─$ curl http://mercury.picoctf.net:39491/.DS_Store
Congrats! You completed the scavenger hunt. Part 5: _f7ce8828}  
```

The flag: `picoCTF{th4ts_4_l0t_0f_pl4c3s_2_lO0k_f7ce8828}`