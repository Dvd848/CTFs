# Wireshark doo dooo do doo...
Category: Forensics, 50 points

## Description

> Can you find the flag?

A network capture was attached.

## Solution

We get a network capture with mainly HTTP traffic:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Wireshark_doo_dooo_do_doo]
└─$ tshark -qz io,phs -r shark1.pcapng

===================================================================
Protocol Hierarchy Statistics
Filter:

eth                                      frames:987 bytes:1524814
  ip                                     frames:985 bytes:1524716
    tcp                                  frames:985 bytes:1524716
      http                               frames:288 bytes:1158734
        tcp.segments                     frames:142 bytes:1078777
        mime_multipart                   frames:142 bytes:78526
          tcp.segments                   frames:142 bytes:78526
        data-text-lines                  frames:2 bytes:695
      tls                                frames:43 bytes:75441
        tcp.segments                     frames:2 bytes:6576
  arp                                    frames:2 bytes:98
===================================================================
```

Scanning through the streams, we see this at stream #5:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Wireshark_doo_dooo_do_doo]
└─$ tshark -r shark1.pcapng -qz follow,tcp,ascii,5

===================================================================
Follow: tcp,ascii
Filter: tcp.stream eq 5
Node 0: 192.168.38.104:64093
Node 1: 18.222.37.134:80
447
GET / HTTP/1.1
Host: 18.222.37.134
Connection: keep-alive
Cache-Control: max-age=0
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.105 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9


        330
HTTP/1.1 200 OK
Date: Mon, 10 Aug 2020 01:51:45 GMT
Server: Apache/2.4.29 (Ubuntu)
Last-Modified: Fri, 07 Aug 2020 00:45:02 GMT
ETag: "2f-5ac3eea4fcf01"
Accept-Ranges: bytes
Content-Length: 47
Keep-Alive: timeout=5, max=100
Connection: Keep-Alive
Content-Type: text/html

Gur synt vf cvpbPGS{c33xno00_1_f33_h_qrnqorrs}

===================================================================
```

Looks like a simple encryption of the flag. Perhaps ROT13?

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Wireshark_doo_dooo_do_doo]
└─$ node
> const chef = require("/home/user/utils/CyberChef/");
undefined
> chef.ROT13("Gur synt vf cvpbPGS{c33xno00_1_f33_h_qrnqorrs}");
The flag is picoCTF{p33kab00_1_s33_u_deadbeef}
```

The flag: `picoCTF{p33kab00_1_s33_u_deadbeef}`