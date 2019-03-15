# Who do I Trust
Misc, 100 points

## Description:

> Who issued the certificate to tamuctf.com?
> (Not in standard gigem{flag} format)

## Solution:

We can use `curl` to determine the CN of the certificate issuer.

```console
root@kali:/media/sf_CTFs/tamu# curl -vI https://tamuctf.com
* Rebuilt URL to: https://tamuctf.com/
*   Trying 52.33.57.247...
* TCP_NODELAY set
* Connected to tamuctf.com (52.33.57.247) port 443 (#0)
* ALPN, offering h2
* ALPN, offering http/1.1
* successfully set certificate verify locations:
*   CAfile: none
  CApath: /etc/ssl/certs
* (304) (OUT), TLS handshake, Client hello (1):
* (304) (IN), TLS handshake, Server hello (2):
* TLSv1.2 (IN), TLS handshake, Certificate (11):
* TLSv1.2 (IN), TLS handshake, Server key exchange (12):
* TLSv1.2 (IN), TLS handshake, Server finished (14):
* TLSv1.2 (OUT), TLS handshake, Client key exchange (16):
* TLSv1.2 (OUT), TLS change cipher, Client hello (1):
* TLSv1.2 (OUT), TLS handshake, Finished (20):
* TLSv1.2 (IN), TLS handshake, Finished (20):
* SSL connection using TLSv1.2 / ECDHE-RSA-CHACHA20-POLY1305
* ALPN, server accepted to use http/1.1
* Server certificate:
*  subject: CN=tamuctf.com
*  start date: Feb  4 22:49:02 2019 GMT
*  expire date: May  5 22:49:02 2019 GMT
*  subjectAltName: host "tamuctf.com" matched cert's "tamuctf.com"
*  issuer: C=US; O=Let's Encrypt; CN=Let's Encrypt Authority X3
*  SSL certificate verify ok.```