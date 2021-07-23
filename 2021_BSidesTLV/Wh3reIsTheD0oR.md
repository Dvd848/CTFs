# Wh3reIsTheD0oR
* Category: Forensics
* 150 Points
* Solved by the JCTF Team

## Description

> I know the F1Ag is there, but i can't find the s3cr3t door
> 
> Maybe the PCAP file has a clue on the way to open it...

A PCAP file was attached

## Solution

Let's check what's happening in the attached PCAP file:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Wh3reIsTheD0oR]
└─$ tshark -qz io,phs -r w3reisthed0or.pcap

===================================================================
Protocol Hierarchy Statistics
Filter:

eth                                      frames:24918 bytes:22757132
  ip                                     frames:24912 bytes:22756826
    udp                                  frames:4428 bytes:2981384
      dns                                frames:632 bytes:72397
      quic                               frames:3796 bytes:2908987
        quic                             frames:244 bytes:232438
          quic                           frames:45 bytes:45685
    tcp                                  frames:20475 bytes:19774812
      http                               frames:140 bytes:77820
        data-text-lines                  frames:12 bytes:5256
          tcp.segments                   frames:1 bytes:60
        json                             frames:5 bytes:2972
          tcp.segments                   frames:1 bytes:60
        ocsp                             frames:84 bytes:53650
          tcp.segments                   frames:3 bytes:1786
      tls                                frames:5017 bytes:5011356
        tcp.segments                     frames:2570 bytes:3606663
          tls                            frames:2268 bytes:3307584
      data                               frames:287 bytes:15785
      _ws.malformed                      frames:6 bytes:330
    icmp                                 frames:9 bytes:630
  arp                                    frames:6 bytes:306
===================================================================
```

We have some HTTP requests, a few are `data-text-lines` and `json` requests and most are `ocsp` requests. Following the different TCP streams, we find the following one:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Wh3reIsTheD0oR]
└─$ tshark -r ./w3reisthed0or.pcap -qz follow,tcp,ascii,3

===================================================================
Follow: tcp,ascii
Filter: tcp.stream eq 3
Node 0: 10.0.3.15:49482
Node 1: 3.126.235.224:80
352
GET / HTTP/1.1
Host: w3reisthed0or.ctf.bsidestlv.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:90.0) Gecko/20100101 Firefox/90.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Connection: keep-alive
Upgrade-Insecure-Requests: 1


        334
HTTP/1.1 200 OK
x-powered-by: Express
set-cookie: session=s%3AZLuxsykXO-Hx4tjM9q2LtwI-j65KTw1H.neqfAVBoRfP%2FSlg88nZEAgo00yD7XzkfGAidnKY%2BYeE; Path=/; HttpOnly
date: Sat, 17 Jul 2021 08:28:44 GMT
transfer-encoding: chunked
set-cookie: BSidesTLV=1b5ae5e6721d6933ee6119e18286258a390cc724; path=/; domain=ctf.bsidestlv.com

0


509
GET /CaN HTTP/1.1
Host: w3reisthed0or.ctf.bsidestlv.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:90.0) Gecko/20100101 Firefox/90.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Connection: keep-alive
Cookie: session=s%3AZLuxsykXO-Hx4tjM9q2LtwI-j65KTw1H.neqfAVBoRfP%2FSlg88nZEAgo00yD7XzkfGAidnKY%2BYeE; BSidesTLV=1b5ae5e6721d6933ee6119e18286258a390cc724
Upgrade-Insecure-Requests: 1


        112
HTTP/1.1 200 OK
x-powered-by: Express
date: Sat, 17 Jul 2021 08:28:45 GMT
transfer-encoding: chunked

0


508
GET /D0 HTTP/1.1
Host: w3reisthed0or.ctf.bsidestlv.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:90.0) Gecko/20100101 Firefox/90.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Connection: keep-alive
Cookie: session=s%3AZLuxsykXO-Hx4tjM9q2LtwI-j65KTw1H.neqfAVBoRfP%2FSlg88nZEAgo00yD7XzkfGAidnKY%2BYeE; BSidesTLV=1b5ae5e6721d6933ee6119e18286258a390cc724
Upgrade-Insecure-Requests: 1


        112
HTTP/1.1 200 OK
x-powered-by: Express
date: Sat, 17 Jul 2021 08:28:47 GMT
transfer-encoding: chunked

0


511
GET /dAnC3 HTTP/1.1
Host: w3reisthed0or.ctf.bsidestlv.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:90.0) Gecko/20100101 Firefox/90.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Connection: keep-alive
Cookie: session=s%3AZLuxsykXO-Hx4tjM9q2LtwI-j65KTw1H.neqfAVBoRfP%2FSlg88nZEAgo00yD7XzkfGAidnKY%2BYeE; BSidesTLV=1b5ae5e6721d6933ee6119e18286258a390cc724
Upgrade-Insecure-Requests: 1


        112
HTTP/1.1 200 OK
x-powered-by: Express
date: Sat, 17 Jul 2021 08:28:48 GMT
transfer-encoding: chunked

0


508
GET /M3 HTTP/1.1
Host: w3reisthed0or.ctf.bsidestlv.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:90.0) Gecko/20100101 Firefox/90.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Connection: keep-alive
Cookie: session=s%3AZLuxsykXO-Hx4tjM9q2LtwI-j65KTw1H.neqfAVBoRfP%2FSlg88nZEAgo00yD7XzkfGAidnKY%2BYeE; BSidesTLV=1b5ae5e6721d6933ee6119e18286258a390cc724
Upgrade-Insecure-Requests: 1


        121
HTTP/1.1 200 OK
x-powered-by: Express
date: Sat, 17 Jul 2021 08:28:48 GMT
transfer-encoding: chunked

4
open
0


506
GET / HTTP/1.1
Host: w3reisthed0or.ctf.bsidestlv.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:90.0) Gecko/20100101 Firefox/90.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Connection: keep-alive
Cookie: session=s%3AZLuxsykXO-Hx4tjM9q2LtwI-j65KTw1H.neqfAVBoRfP%2FSlg88nZEAgo00yD7XzkfGAidnKY%2BYeE; BSidesTLV=1b5ae5e6721d6933ee6119e18286258a390cc724
Upgrade-Insecure-Requests: 1


        1460
HTTP/1.1 200 OK
x-powered-by: Express
content-type: text/html; charset=utf-8
content-length: 1639
etag: W/"667-OwsjGJyMzUT+qfTWfwDpICCpCF0"
date: Sat, 17 Jul 2021 08:28:50 GMT

<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>BSidesTLV 2021 - Home</title>

    <!-- Bootstrap core CSS -->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css"
          integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">
    <style>
        body {
            min-height: 75rem;
            padding-top: 4.5rem;
        }
    </style>
</head>

<body>

<nav class="navbar navbar-expand-md navbar-dark fixed-top bg-dark">
    <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarCollapse"
            aria-controls="navbarCollapse" aria-expanded="false" aria-label="Toggle navigation">
        <span class="navbar-toggler-icon"></span>
    </button>
    <div class="collapse navbar-collapse" id="navbarCollapse">
        <ul class="navbar-nav mr-auto">
            <li class="nav-item active">
                <a class="nav-link" href="/">Home <span class="sr-only">(current)</span></a>

        362
</li>
            <li class="nav-item">
                <a class="nav-link" href="/flag">Flag</a>
            </li>
        </ul>
    </div>
</nav>

<main role="main" class="container">
    <div class="jumbotron">
        <h1>Get the Flag!</h1>
        <p class="lead">Can you help me find the L0st F1ag ?</p>
    </div>
</main>

</body>
</html>
        1


===================================================================
```

Let's concentrate on the `GET` request URI:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Wh3reIsTheD0oR]
└─$ tshark -r ./w3reisthed0or.pcap -qz follow,tcp,ascii,3 | grep GET
GET / HTTP/1.1
GET /CaN HTTP/1.1
GET /D0 HTTP/1.1
GET /dAnC3 HTTP/1.1
GET /M3 HTTP/1.1
GET / HTTP/1.1
```

This looks interesting, but doesn't make sense as a sentence. If we move to stream #4, we also find a similar set of requests:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Wh3reIsTheD0oR]
└─$ tshark -r ./w3reisthed0or.pcap -qz follow,tcp,ascii,4 | grep GET
GET /favicon.ico HTTP/1.1
GET /yoU HTTP/1.1
GET /tA9 HTTP/1.1
GET /4 HTTP/1.1
```

We'll combine both streams to see what happened:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Wh3reIsTheD0oR]
└─$ tshark -r ./w3reisthed0or.pcap -Y "ip.src == 10.0.3.15 && ip.dst == 3.126.235.224" | grep GET
   46   0.380551    10.0.3.15 → 3.126.235.224 HTTP 406 GET / HTTP/1.1
  195   0.817496    10.0.3.15 → 3.126.235.224 HTTP 531 GET /favicon.ico HTTP/1.1
  297   1.761041    10.0.3.15 → 3.126.235.224 HTTP 563 GET /CaN HTTP/1.1
 1944   2.578609    10.0.3.15 → 3.126.235.224 HTTP 563 GET /yoU HTTP/1.1
 3749   3.286006    10.0.3.15 → 3.126.235.224 HTTP 562 GET /D0 HTTP/1.1
 4400   3.751447    10.0.3.15 → 3.126.235.224 HTTP 563 GET /tA9 HTTP/1.1
 4583   4.273925    10.0.3.15 → 3.126.235.224 HTTP 565 GET /dAnC3 HTTP/1.1
 4593   4.339010    10.0.3.15 → 3.126.235.224 HTTP 561 GET /4 HTTP/1.1
 5323   4.880017    10.0.3.15 → 3.126.235.224 HTTP 562 GET /M3 HTTP/1.1
 6751   6.091966    10.0.3.15 → 3.126.235.224 HTTP 560 GET / HTTP/1.1
```

Now it makes more sense: "CaN yoU D0 tA9 dAnC3 4 M3". Looks like a variation of standard port knocking: We should visit all of these URLs to unlock the flag. 

Notice how the last HTTP request from the dump received the following response:

```html
<li class="nav-item">
    <a class="nav-link" href="/flag">Flag</a>
</li>
```

So, we'll visit `/flag` once we're done knocking.

```python
import requests
from scapy.all import *
from scapy.layers.http import HTTPRequest

HOST = "w3reisthed0or.ctf.bsidestlv.com"
BASE_URI = "http://" + HOST

s = requests.session()

def request_uri(uri):
    print(f"Calling {uri}")
    r = s.get(uri)
    return r.text

def filter_packets(packet):
    if not packet.haslayer('HTTPRequest'):
        return
    if not packet[IP].src == "10.0.3.15":
        return
    if not packet[IP].dst == "3.126.235.224":
        return
    if not packet[HTTPRequest].Host.decode("utf-8") == HOST:
        return

    path = packet[HTTPRequest].Path.decode("utf-8")

    request_uri(BASE_URI + path)  

sniff(offline = 'w3reisthed0or.pcap', 
      session = TCPSession, 
      prn     = filter_packets)

print(request_uri(f"{BASE_URI}/flag"))
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Wh3reIsTheD0oR]
└─$ python3 knock.py
Calling http://w3reisthed0or.ctf.bsidestlv.com/
Calling http://w3reisthed0or.ctf.bsidestlv.com/favicon.ico
Calling http://w3reisthed0or.ctf.bsidestlv.com/CaN
Calling http://w3reisthed0or.ctf.bsidestlv.com/yoU
Calling http://w3reisthed0or.ctf.bsidestlv.com/D0
Calling http://w3reisthed0or.ctf.bsidestlv.com/tA9
Calling http://w3reisthed0or.ctf.bsidestlv.com/dAnC3
Calling http://w3reisthed0or.ctf.bsidestlv.com/4
Calling http://w3reisthed0or.ctf.bsidestlv.com/M3
Calling http://w3reisthed0or.ctf.bsidestlv.com/
Calling http://w3reisthed0or.ctf.bsidestlv.com/flag
<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>BSidesTLV 2021 - Flag</title>

    <!-- Bootstrap core CSS -->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css"
          integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">
    <style>
        body {
            min-height: 75rem;
            padding-top: 4.5rem;
        }
    </style>
</head>

<body>

<nav class="navbar navbar-expand-md navbar-dark fixed-top bg-dark">
    <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarCollapse"
            aria-controls="navbarCollapse" aria-expanded="false" aria-label="Toggle navigation">
        <span class="navbar-toggler-icon"></span>
    </button>
    <div class="collapse navbar-collapse" id="navbarCollapse">
        <ul class="navbar-nav mr-auto">
            <li class="nav-item">
                <a class="nav-link" href="/">Home</a>
            </li>
            <li class="nav-item active">
                <a class="nav-link" href="/flag">Flag <span class="sr-only">(current)</span></a>
            </li>
        </ul>
    </div>
</nav>

<main role="main" class="container">
    <div class="jumbotron">
        <h1>The Flag Is</h1>
        <p class="lead font-weight-bold">BSidesTLV2021{g0oD.4.Y0u.U.Kno0k3d.1T}</p>
    </div>
</main>

</body>
</html>
```

The flag: `BSidesTLV2021{g0oD.4.Y0u.U.Kno0k3d.1T}`