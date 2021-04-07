# Who are you?
Category: Web, 100 points

## Description

> Let me in. Let me iiiiiiinnnnnnnnnnnnnnnnnnnn 


## Solution

Let's visit the attached website:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Who_are_you]
└─$ curl -s http://mercury.picoctf.net:38322/
<!DOCTYPE html>
<html lang="en">

<head>
    <title>Who are you?</title>


    <link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css" rel="stylesheet">

    <link href="https://getbootstrap.com/docs/3.3/examples/jumbotron-narrow/jumbotron-narrow.css" rel="stylesheet">

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>

    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>


</head>

<body>

    <div class="container">
      <div class="jumbotron">
        <p class="lead"></p>
                <div class="row">
                        <div class="col-xs-12 col-sm-12 col-md-12">
                                <h3 style="color:red">Only people who use the official PicoBrowser are allowed on this site!</h3>
                        </div>
                </div>
                <br/>

                        <img src="/static/who_r_u.gif"></img>

        </div>
    <footer class="footer">
        <p>&copy; PicoCTF</p>
    </footer>

</div>
<script>
$(document).ready(function(){
    $(".close").click(function(){
        $("myAlert").alert("close");
    });
});
</script>
</body>

</html>
```

The site accepts only users who use the `picoctf` browser. We can spoof that using the user agent:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Who_are_you]
└─$ curl -s --user-agent "picobrowser" "http://mercury.picoctf.net:38322/" | grep h3
                                <h3 style="color:red">I don&#39;t trust users visiting from another site.</h3>
```

We can spoof that using the referrer:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Who_are_you]
└─$ curl --user-agent "picobrowser" "http://mercury.picoctf.net:38322/" --referer "http://mercury.picoctf.net:38322/" -s | grep h3
                                <h3 style="color:red">Sorry, this site only worked in 2018.</h3>
```

We can spoof that using the `Date` header:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Who_are_you]
└─$ curl --user-agent "picobrowser" "http://mercury.picoctf.net:38322/" --referer "http://mercury.picoctf.net:38322/" -H "Date: Wed, 21 Oct 2018 07:28:00 GMT" -s | grep h3
                                <h3 style="color:red">I don&#39;t trust users who can be tracked.</h3>

```

We can set that using the "Do Not Track" header:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Who_are_you]
└─$ curl --user-agent "picobrowser" "http://mercury.picoctf.net:38322/" --referer "http://mercury.picoctf.net:38322/" -H "Date: Wed, 21 Oct 2018 07:28:00 GMT" -H "DNT: 1" -s | grep h3
                                <h3 style="color:red">This website is only for people from Sweden.</h3>
```

We can spoof that using a Swedish IP together with `X-Forwarded-For`:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Who_are_you]
└─$ curl --user-agent "picobrowser" "http://mercury.picoctf.net:38322/" --referer "http://mercury.picoctf.net:38322/" -H "Date: Wed, 21 Oct 2018 07:28:00 GMT" -H "DNT: 1" -H "X-Forwarded-For: 193.150.233.115" -s | grep h3
                                <h3 style="color:red">You&#39;re in Sweden but you don&#39;t speak Swedish?</h3>
```

We can set that using the `Accept-Language` header:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Who_are_you]
└─$ curl --user-agent "picobrowser" "http://mercury.picoctf.net:38322/" --referer "http://mercury.picoctf.net:38322/" -H "Date: Wed, 21 Oct 2018 07:28:00 GMT" -H "DNT: 1" -H "X-Forwarded-For: 193.150.233.115" -H "Accept-Language: sv" -s | grep h3
                                <h3 style="color:green">What can I say except, you are welcome</h3>
                        <b>picoCTF{http_h34d3rs_v3ry_c0Ol_much_w0w_b22d773c}</b>
```

The flag: `picoCTF{http_h34d3rs_v3ry_c0Ol_much_w0w_b22d773c}`