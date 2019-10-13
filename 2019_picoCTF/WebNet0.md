# WebNet0
Forensics, 350 points

## Description:
> We found this packet capture and key. Recover the flag.


## Solution: 

We receive a network capture:
```console
root@kali:/media/sf_CTFs/pico/WebNet0# tshark -r capture.pcap
Running as user "root" and group "root". This could be dangerous.
    1   0.000000 128.237.140.23 → 172.31.22.220 TCP 78 57567 → 443 [SYN] Seq=0 Win=65535 Len=0 MSS=1386 WS=64 TSval=132865167 TSecr=0 SACK_PERM=1 57567 443
    2   0.000029 172.31.22.220 → 128.237.140.23 TCP 74 443 → 57567 [SYN, ACK] Seq=0 Ack=1 Win=26847 [TCP CHECKSUM INCORRECT] Len=0 MSS=8961 SACK_PERM=1 TSval=568332748 TSecr=132865167 WS=128 443 57567
    3   0.025161 128.237.140.23 → 172.31.22.220 TCP 78 57578 → 443 [SYN] Seq=0 Win=65535 Len=0 MSS=1386 WS=64 TSval=132865192 TSecr=0 SACK_PERM=1 57578 443
    4   0.025171 172.31.22.220 → 128.237.140.23 TCP 74 443 → 57578 [SYN, ACK] Seq=0 Ack=1 Win=26847 [TCP CHECKSUM INCORRECT] Len=0 MSS=8961 SACK_PERM=1 TSval=568332773 TSecr=132865192 WS=128 443 57578
    5   0.028804 128.237.140.23 → 172.31.22.220 TCP 66 57567 → 443 [ACK] Seq=1 Ack=1 Win=131904 Len=0 TSval=132865195 TSecr=568332748 57567 443
    6   0.028881 128.237.140.23 → 172.31.22.220 TLSv1 583 Client Hello 57567 443
    7   0.028902 172.31.22.220 → 128.237.140.23 TCP 66 443 → 57567 [ACK] Seq=1 Ack=518 Win=28032 [TCP CHECKSUM INCORRECT] Len=0 TSval=568332777 TSecr=132865195 443 57567
    8   0.029538 172.31.22.220 → 128.237.140.23 TLSv1.2 1073 Server Hello, Certificate, Server Hello Done 443 57567
    9   0.053871 128.237.140.23 → 172.31.22.220 TCP 66 57578 → 443 [ACK] Seq=1 Ack=1 Win=131904 Len=0 TSval=132865219 TSecr=568332773 57578 443
   10   0.058387 128.237.140.23 → 172.31.22.220 TLSv1 583 Client Hello 57578 443
   11   0.058417 172.31.22.220 → 128.237.140.23 TCP 66 443 → 57578 [ACK] Seq=1 Ack=518 Win=28032 [TCP CHECKSUM INCORRECT] Len=0 TSval=568332806 TSecr=132865222 443 57578
   12   0.058429 128.237.140.23 → 172.31.22.220 TCP 66 57567 → 443 [ACK] Seq=518 Ack=1008 Win=130880 Len=0 TSval=132865222 TSecr=568332777 57567 443
   13   0.058743 172.31.22.220 → 128.237.140.23 TLSv1.2 1073 Server Hello, Certificate, Server Hello Done 443 57578
   14   0.059645 128.237.140.23 → 172.31.22.220 TLSv1.2 384 Client Key Exchange, Change Cipher Spec, Encrypted Handshake Message 57567 443
   15   0.061383 172.31.22.220 → 128.237.140.23 TLSv1.2 324 New Session Ticket, Change Cipher Spec, Encrypted Handshake Message 443 57567
   16   0.088416 128.237.140.23 → 172.31.22.220 TCP 66 57578 → 443 [ACK] Seq=518 Ack=1008 Win=130880 Len=0 TSval=132865247 TSecr=568332806 57578 443
   17   0.092408 128.237.140.23 → 172.31.22.220 TCP 78 57581 → 443 [SYN] Seq=0 Win=65535 Len=0 MSS=1386 WS=64 TSval=132865249 TSecr=0 SACK_PERM=1 57581 443
   18   0.092423 172.31.22.220 → 128.237.140.23 TCP 74 443 → 57581 [SYN, ACK] Seq=0 Ack=1 Win=26847 [TCP CHECKSUM INCORRECT] Len=0 MSS=8961 SACK_PERM=1 TSval=568332840 TSecr=132865249 WS=128 443 57581
   19   0.092429 128.237.140.23 → 172.31.22.220 TLSv1.2 384 Client Key Exchange, Change Cipher Spec, Encrypted Handshake Message 57578 443
   20   0.093713 128.237.140.23 → 172.31.22.220 TCP 66 57567 → 443 [ACK] Seq=836 Ack=1266 Win=130752 Len=0 TSval=132865252 TSecr=568332809 57567 443
   21   0.094104 172.31.22.220 → 128.237.140.23 TLSv1.2 324 New Session Ticket, Change Cipher Spec, Encrypted Handshake Message 443 57578
   22   0.122048 128.237.140.23 → 172.31.22.220 TCP 66 57581 → 443 [ACK] Seq=1 Ack=1 Win=131904 Len=0 TSval=132865276 TSecr=568332840 57581 443
   23   0.122203 128.237.140.23 → 172.31.22.220 TLSv1 583 Client Hello 57581 443
   24   0.122220 172.31.22.220 → 128.237.140.23 TCP 66 443 → 57581 [ACK] Seq=1 Ack=518 Win=28032 [TCP CHECKSUM INCORRECT] Len=0 TSval=568332870 TSecr=132865276 443 57581
   25   0.122552 172.31.22.220 → 128.237.140.23 TLSv1.2 1073 Server Hello, Certificate, Server Hello Done 443 57581
   26   0.123046 128.237.140.23 → 172.31.22.220 TCP 66 57578 → 443 [ACK] Seq=836 Ack=1266 Win=130752 Len=0 TSval=132865277 TSecr=568332842 57578 443
   27   0.151669 128.237.140.23 → 172.31.22.220 TCP 66 57581 → 443 [ACK] Seq=518 Ack=1008 Win=130880 Len=0 TSval=132865303 TSecr=568332870 57581 443
   28   0.152210 128.237.140.23 → 172.31.22.220 TLSv1.2 384 Client Key Exchange, Change Cipher Spec, Encrypted Handshake Message 57581 443
   29   0.153206 172.31.22.220 → 128.237.140.23 TLSv1.2 324 New Session Ticket, Change Cipher Spec, Encrypted Handshake Message 443 57581
   30   0.183385 128.237.140.23 → 172.31.22.220 TCP 66 57581 → 443 [ACK] Seq=836 Ack=1266 Win=130752 Len=0 TSval=132865334 TSecr=568332901 57581 443
   31   0.187804 128.237.140.23 → 172.31.22.220 TLSv1.2 506 Application Data 57581 443
   32   0.188303 172.31.22.220 → 128.237.140.23 TLSv1.2 1299 Application Data 443 57581
   33   0.220287 128.237.140.23 → 172.31.22.220 TCP 66 57581 → 443 [ACK] Seq=1276 Ack=2499 Win=129792 Len=0 TSval=132865368 TSecr=568332936 57581 443
   34   0.357102 128.237.140.23 → 172.31.22.220 TLSv1.2 521 Application Data 57567 443
   35   0.357544 172.31.22.220 → 128.237.140.23 TLSv1.2 576 Application Data 443 57567
   36   0.386999 128.237.140.23 → 172.31.22.220 TCP 66 57567 → 443 [ACK] Seq=1291 Ack=1776 Win=130560 Len=0 TSval=132865528 TSecr=568333105 57567 443
   37   0.817074 128.237.140.23 → 172.31.22.220 TLSv1.2 438 Application Data 57567 443
   38   0.817393 172.31.22.220 → 128.237.140.23 TLSv1.2 637 Application Data 443 57567
   39   0.847407 128.237.140.23 → 172.31.22.220 TCP 66 57567 → 443 [ACK] Seq=1663 Ack=2347 Win=130496 Len=0 TSval=132865963 TSecr=568333565 57567 443
```

And a key file:
```console
root@kali:/media/sf_CTFs/pico/WebNet0# openssl rsa -in picopico.key -text
RSA Private-Key: (2048 bit, 2 primes)
modulus:
    00:b0:2a:51:4f:34:a8:ec:78:91:79:a6:e0:89:53:
    9c:77:f1:77:13:d5:e4:20:7b:9c:ce:28:d6:a1:02:
    56:2e:76:f1:95:38:4b:3a:d5:39:c8:82:f7:04:47:
    89:28:f2:2d:ce:0b:06:a4:db:f6:ad:70:69:37:a3:
    3f:63:14:a7:a9:ed:71:44:60:d3:f7:d4:8c:30:0f:
    d8:ff:61:ac:e5:2b:2e:03:44:b1:8e:6c:ec:88:65:
    45:35:7f:65:91:03:b5:21:7f:43:ce:41:7b:03:4f:
    5a:14:5f:7d:a3:30:a6:64:41:24:83:5b:83:11:65:
    df:6d:ac:96:1d:3b:64:eb:70:43:cc:b0:18:99:42:
    51:65:be:09:cd:c2:5d:d0:95:ac:28:cd:31:cb:00:
    92:88:df:a8:f5:70:fc:12:30:c7:8d:71:ad:5e:d1:
    98:b5:b3:b4:79:23:17:e1:a4:d5:ce:04:5d:05:9b:
    18:96:be:67:8e:1d:b6:ac:a7:21:e0:f1:41:26:18:
    1a:e4:77:89:38:c1:74:8a:19:0b:eb:73:c4:23:c9:
    c3:f8:49:c1:1d:aa:ec:49:89:89:c3:4f:c8:84:6c:
    0a:bb:d3:fe:df:ff:93:48:37:50:c4:f5:8a:06:26:
    a2:98:8d:34:bd:9d:13:c1:e1:8b:e3:24:df:d2:26:
    78:6f
publicExponent: 65537 (0x10001)
privateExponent:
    08:29:dd:dc:ba:c6:fd:36:55:1f:7b:11:3a:ab:ea:
    3b:50:b0:40:f6:0f:7d:45:dd:2d:5c:8d:1d:a6:fb:
    11:6a:27:a5:cf:97:04:e1:ee:ac:91:0d:1b:60:a9:
    45:81:7b:87:e9:d0:e4:00:e1:7c:86:12:0a:27:01:
    7f:f8:ec:10:1e:d5:b9:e2:76:d0:2c:44:56:d1:d5:
    2f:78:7a:47:a0:69:a0:73:25:7b:41:26:f0:e7:28:
    7e:e3:29:74:bf:e4:3b:ea:26:dd:3f:01:91:54:b3:
    0a:f0:a5:e4:d3:13:52:e0:05:ee:24:66:7d:7e:e8:
    0c:b0:0b:c0:cd:08:cf:34:2f:da:e9:fe:d9:49:93:
    d7:9a:e0:01:97:e5:dc:82:f5:3c:6b:c9:85:b8:4b:
    c5:f7:9e:c8:f1:3d:30:1c:b5:4a:a0:63:43:da:cd:
    16:7f:2c:42:ff:79:f4:9e:81:1f:3e:1b:12:92:bf:
    fc:4a:ed:34:fd:b2:87:ba:22:54:10:60:28:44:35:
    80:4b:8e:8d:00:bf:e2:8c:68:a8:21:5f:65:a7:fd:
    5c:d4:42:c4:1f:f3:63:59:d4:a6:bb:c9:cb:3d:3d:
    34:c4:16:34:5d:84:9a:f9:81:54:67:e8:4f:19:ae:
    ba:de:4d:d0:66:d5:af:65:32:1f:15:8c:2a:6d:ac:
    39
prime1:
    00:e9:6f:6f:80:5a:05:a5:1a:d7:ad:b8:b2:89:7c:
    9b:3c:76:77:7a:2e:19:da:7d:b2:82:39:73:0e:4f:
    af:2a:30:14:68:4e:90:6d:55:32:d1:55:23:6f:58:
    29:bc:9b:84:d3:11:ac:d7:e3:e6:40:f9:b2:45:c1:
    41:70:68:04:c3:98:77:2b:ea:53:08:de:d3:4a:ad:
    cb:27:63:61:7b:a3:92:38:cf:a9:b0:b9:1b:92:7a:
    cc:ea:fe:77:71:66:a0:b3:c0:2b:b8:9c:a8:b1:87:
    77:33:9e:9e:e3:26:21:25:34:6d:1d:f0:bb:b9:79:
    08:26:54:02:b5:02:15:97:7d
prime2:
    00:c1:31:af:60:6b:b5:49:50:fe:29:cd:c1:e7:58:
    0a:22:df:83:a9:7e:3b:d0:61:e1:a5:20:a2:f7:00:
    a3:b8:39:e7:5a:1d:d1:fd:aa:27:78:d4:f4:07:9c:
    be:ce:df:1c:cd:eb:af:52:90:b6:79:b3:47:7c:f0:
    0f:cb:14:b9:38:a1:93:4c:29:d9:12:4d:02:10:f8:
    03:1b:5c:7d:35:1c:61:6f:9c:23:ae:3e:0f:c5:6c:
    da:75:c1:2e:f3:24:48:39:bb:91:c5:41:6c:8c:3c:
    d2:4b:af:f8:59:ea:0d:98:a7:e5:06:a4:07:06:4f:
    03:3f:44:23:d5:00:f8:4b:5b
exponent1:
    00:c0:07:43:9a:3a:73:da:56:32:86:5e:21:c0:a8:
    18:ab:ac:68:ac:c1:af:d2:e5:04:2b:cc:46:b1:c7:
    2b:39:71:43:d8:6a:88:b4:e8:19:5d:ca:c3:d3:9c:
    9a:f8:e4:96:67:6b:6a:dc:4e:45:e3:bd:84:c1:8d:
    30:df:df:31:cc:15:68:33:60:17:de:7c:2f:24:87:
    c3:4f:2b:99:cd:b3:c9:5d:a2:b6:dd:01:e9:84:9e:
    30:64:3f:e0:d2:10:b2:b2:2b:ab:cb:ba:53:ab:76:
    dc:c0:42:04:42:a7:e3:2c:4f:ec:53:6c:ed:80:ad:
    e7:de:5f:cd:ba:49:74:a9:a1
exponent2:
    2d:af:9c:33:87:05:05:e3:7b:57:53:6b:09:54:4e:
    81:54:ae:04:04:f0:0c:25:39:81:1d:28:ac:94:a0:
    22:ce:be:a1:16:f0:33:b6:6b:43:2d:c8:cf:8c:07:
    ab:50:23:b5:a6:88:7d:53:ef:72:f4:2c:71:a5:2b:
    76:f0:dd:a4:40:c1:5e:7f:7e:ef:ce:fa:30:1d:16:
    4f:00:1e:33:d3:14:4f:9a:72:ed:9f:8b:87:3a:68:
    a6:f4:1a:30:31:62:4b:14:ca:32:05:78:af:e9:2a:
    29:ef:e1:21:12:32:48:e9:5b:45:a8:c0:68:83:82:
    d7:11:3c:10:00:fc:b6:85
coefficient:
    7c:43:35:ad:f3:34:bf:75:26:07:b3:d2:ea:ed:26:
    3f:77:24:3f:60:85:09:d6:ab:c9:73:df:0b:9d:86:
    05:c2:77:43:8e:98:a6:c4:2f:2d:35:68:b4:cf:ad:
    78:7b:d3:8c:dc:36:8f:0c:19:c4:89:78:35:e9:c6:
    48:48:f7:28:38:50:a0:e8:90:0b:d0:6b:0c:3f:83:
    07:82:3d:f9:3f:67:c5:3d:e0:ed:1e:8c:ae:02:13:
    82:10:78:59:ee:d3:56:12:ff:3e:58:e7:25:3c:83:
    aa:98:cd:03:89:18:4e:f7:80:24:fb:fa:5e:ad:44:
    46:de:4f:52:d5:f7:06:a4
writing RSA key
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAsCpRTzSo7HiReabgiVOcd/F3E9XkIHuczijWoQJWLnbxlThL
OtU5yIL3BEeJKPItzgsGpNv2rXBpN6M/YxSnqe1xRGDT99SMMA/Y/2Gs5SsuA0Sx
jmzsiGVFNX9lkQO1IX9DzkF7A09aFF99ozCmZEEkg1uDEWXfbayWHTtk63BDzLAY
mUJRZb4JzcJd0JWsKM0xywCSiN+o9XD8EjDHjXGtXtGYtbO0eSMX4aTVzgRdBZsY
lr5njh22rKch4PFBJhga5HeJOMF0ihkL63PEI8nD+EnBHarsSYmJw0/IhGwKu9P+
3/+TSDdQxPWKBiaimI00vZ0TweGL4yTf0iZ4bwIDAQABAoIBAAgp3dy6xv02VR97
ETqr6jtQsED2D31F3S1cjR2m+xFqJ6XPlwTh7qyRDRtgqUWBe4fp0OQA4XyGEgon
AX/47BAe1bnidtAsRFbR1S94ekegaaBzJXtBJvDnKH7jKXS/5DvqJt0/AZFUswrw
peTTE1LgBe4kZn1+6AywC8DNCM80L9rp/tlJk9ea4AGX5dyC9TxryYW4S8X3nsjx
PTActUqgY0PazRZ/LEL/efSegR8+GxKSv/xK7TT9soe6IlQQYChENYBLjo0Av+KM
aKghX2Wn/VzUQsQf82NZ1Ka7ycs9PTTEFjRdhJr5gVRn6E8ZrrreTdBm1a9lMh8V
jCptrDkCgYEA6W9vgFoFpRrXrbiyiXybPHZ3ei4Z2n2ygjlzDk+vKjAUaE6QbVUy
0VUjb1gpvJuE0xGs1+PmQPmyRcFBcGgEw5h3K+pTCN7TSq3LJ2Nhe6OSOM+psLkb
knrM6v53cWags8AruJyosYd3M56e4yYhJTRtHfC7uXkIJlQCtQIVl30CgYEAwTGv
YGu1SVD+Kc3B51gKIt+DqX470GHhpSCi9wCjuDnnWh3R/aoneNT0B5y+zt8czeuv
UpC2ebNHfPAPyxS5OKGTTCnZEk0CEPgDG1x9NRxhb5wjrj4PxWzadcEu8yRIObuR
xUFsjDzSS6/4WeoNmKflBqQHBk8DP0Qj1QD4S1sCgYEAwAdDmjpz2lYyhl4hwKgY
q6xorMGv0uUEK8xGsccrOXFD2GqItOgZXcrD05ya+OSWZ2tq3E5F472EwY0w398x
zBVoM2AX3nwvJIfDTyuZzbPJXaK23QHphJ4wZD/g0hCysiury7pTq3bcwEIEQqfj
LE/sU2ztgK3n3l/Nukl0qaECgYAtr5wzhwUF43tXU2sJVE6BVK4EBPAMJTmBHSis
lKAizr6hFvAztmtDLcjPjAerUCO1poh9U+9y9CxxpSt28N2kQMFef37vzvowHRZP
AB4z0xRPmnLtn4uHOmim9BowMWJLFMoyBXiv6Sop7+EhEjJI6VtFqMBog4LXETwQ
APy2hQKBgHxDNa3zNL91Jgez0urtJj93JD9ghQnWq8lz3wudhgXCd0OOmKbELy01
aLTPrXh704zcNo8MGcSJeDXpxkhI9yg4UKDokAvQaww/gweCPfk/Z8U94O0ejK4C
E4IQeFnu01YS/z5Y5yU8g6qYzQOJGE73gCT7+l6tREbeT1LV9wak
-----END RSA PRIVATE KEY-----
```

We have three streams in this capture file:
```console
root@kali:/media/sf_CTFs/pico/WebNet0# tshark -r capture.pcap  -T fields -e tcp.stream | sort -u
Running as user "root" and group "root". This could be dangerous.
0
1
2
```

However, they are encrypted and we can't view their contents without the appropriate key:
```console
root@kali:/media/sf_CTFs/pico/WebNet0# tshark -r capture.pcap  -qz follow,tcp,ascii,0
Running as user "root" and group "root". This could be dangerous.

===================================================================
Follow: tcp,ascii
Filter: tcp.stream eq 0
Node 0: 128.237.140.23:57567
Node 1: 172.31.22.220:443
517
................>..}............O]Ee.d..N....r.0.,.(.$...
.......k.i.h.9.7.6.2...*.&.......=.5./.+.'.#...........g.?.>.3.1.0.1.-.).%.......<./.........
.....a...7.5..2ec2-18-223-184-200.us-east-2.compute.amazonaws.com.........
.....................
. ......................................................................................................................................................................................................................................................
        1007
.....0\1.0...U....US1.0...U....Michigan1.0...U....Kalamazoo1.0...U.......0...0..........R..&j.....>Lf....$..0
..Pico CTF1.0...U...
200811154129Z0\1.0...U....US1.0...U....Michigan1.0...U....Kalamazoo1.0...U.
..Pico CTF1.0...U...
..........0.."0
......*QO4..x.y...S.w.w... {..(...V.v..8K:.9....G.(.-.......pi7.?c....qD`....0...a..+..D..l..eE5.e...!.C.A{.OZ._}.0.dA$.[..e.m...;d.pC....BQe....]...(.1.......p..0..q.^.....y#......].....g.....!..A&...w.8.t....s.#...I....I...O..l
mo...E...yN.d...p..[m.b...zq........:...Ij..Q..L&........%...r..gJ6.29.T".*8..n....n.~+*...J.6.....yG...".7}...oO...k..G[3.-.8.(.gq."..(......
.....0&..9s...C.p....~.Qv............y.E...A]... \.D.2=C.c. ..B..m.r&.........
318
..g'\S.G#(.....t.(1Z9.PMB...;p.^#...PS..]....y|......mc.Vb..N....B[...._...nES....
.;p.0.|O..f.u..F.5id4..1 g..\.....X.f........C....2=eH.J&.ib.G.9....k;G.-.M.9 ..c#H.R..&.n..~.L.4.Hk........g.{rv....~_.....j..-..4vJ..h......_.CA..%7g...........(...B.r..zZ)..w.PRFD..:..z.DC.X>.O...F.#J
        258
........w........(I62.........}.Ja#.Fx.7.Z..G.!..o.F..;..^T.L...K..8.....i.E...=F. .P......j...."$...7.#.g.dd..s.=...V._.....I0R.....9|..........#=,..xI.K..Qx.[v.__c...
.-3..s...xTM..q.*............(..5.]fc....s.
.F.O.........7s...~....-..
455
;E.....*.+*.<.B.1.n..sG+N........}.W.7....[?..+.k....J.P...Y....m....
.@(.k.....N%.......k ...v........y.Q.K&.i.s..
.b.`.i..Z/...`wpY.H...<.]W=.....@...L(O<.2-1aou.:.r...-....2!.Vv.`.D.YR..1y.
=W...f.|.$...A......eZ-G...G.#e..[.w6-U..O.P&..&.......H..9..[......3g.e...~.[...4
t_.Z.r...7......../e.g<.....%.&>..C..p3.M.:..Z....,u........8>..P..9.K.$..8.*.....?..~2|p..i=e,..a..i..l...fHDMg.*7Z...uK..S.
        510
.......5.]fc.....G.....W.j.I..x..;.*.D~X.lX.9eY.@f...XG.f...|.k.l....$3..:g..Y..)e}....?K'.......J...]X.;7..cw.....$.PX..._)7...P]..B..$........xK..O...3..~G..G..Tz...c6.O...*.5....QT...f...s.h....[B.~..|...G..a.L.8.u..@.2....<9.J...r#.......5:i.Y .fG. .._...d..."*.1([^..........K....6"..Y....v...........v.3iL....0E..z~#....b|..]%P.o.
M..l......6..XkCw.i..........O...;..A.....1b...................:..gp....v4.5......Kf.......QK.--.~X...6...@..`..$..v......9.'scC?.5k
U.....\.....;L".w.nebd.V%......"....+m..
372
....o...B.r....._aa.....M..&.M..o..
.y=*y.....5.L.lm4......-.*.'H3H..OQS...'.._.:5".L.. 4)
4..Nt.e.6.....NO..X.^.>.hA.:.....%.=............=
J~HH9.......`m..*.G...RdA....`vOo.^...#...~.X.......v../z....]/*L..]-..GX.... .E...-4.......K........R.*.w...w.........&..\
]@ns.......\...M2...geM.)
...tN.....D.....|...{...P..entoxo...%..po.O..Z{
        571
....6..5.]fc....,.T=..u..v..0G.?....!am...+.g(5O].4\.o..(.|G...X......Hr...._...j5..{..j..W.!.,}7.....w.u0Y...
7.....)....U...T1.8r..#..$Zx.q.r..1?.b...`.;..0..h...I..t.d.V......&..-.S6(.3....q.B..t%...... ..LkpN..\B.FH(=JW....w..N..8.[`.:.Z....YxK..#..f....r.W......\....w..^....$.O.....T..KGW..O...`0..t.....J.lIMq..'..7...s....a......n......9.F...Q1...}..K.q)Ex..........U?.th.w. g...u8.6.y.'.gV..z.UO.}E...?>.
===================================================================
root@kali:/media/sf_CTFs/pico/WebNet0# tshark -r capture.pcap  -qz follow,ssl,ascii,0
Running as user "root" and group "root". This could be dangerous.

===================================================================
Follow: ssl,ascii
Filter: tcp.stream eq 0
Node 0: :0
Node 1: :0
===================================================================
```

Let's use the private key in order to decrypt the TLS layer and view the contents:
```console
root@kali:/media/sf_CTFs/pico/WebNet0# tshark -r capture.pcap  -o "ssl.debug_file:ssldebug.log" -o "ssl.desegment_ssl_records: TRUE" -o "ssl.desegment_ssl_application_data: TRUE" -o "ssl.keys_list:172.31.22.220,443,http,picopico.key" -qz follow,ssl,ascii,0
Running as user "root" and group "root". This could be dangerous.

===================================================================
Follow: ssl,ascii
Filter: tcp.stream eq 0
Node 0: 128.237.140.23:57567
Node 1: :0
426
GET /starter-template.css HTTP/1.1
Host: ec2-18-223-184-200.us-east-2.compute.amazonaws.com
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:68.0) Gecko/20100101 Firefox/68.0
Accept: text/css,*/*;q=0.1
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Referer: https://ec2-18-223-184-200.us-east-2.compute.amazonaws.com/
Pragma: no-cache
Cache-Control: no-cache


        481
HTTP/1.1 200 OK
Date: Fri, 23 Aug 2019 15:56:36 GMT
Server: Apache/2.4.29 (Ubuntu)
Last-Modified: Mon, 12 Aug 2019 16:47:05 GMT
ETag: "62-58fee462bf227-gzip"
Accept-Ranges: bytes
Vary: Accept-Encoding
Content-Encoding: gzip
Pico-Flag: picoCTF{nongshim.shrimp.crackers}
Content-Length: 100
Keep-Alive: timeout=5, max=100
Connection: Keep-Alive
Content-Type: text/css

..........K.O.T..RP(HLI..K.-./.R0-J......+.I,*I-.-I.-.I,IEVj.`.T.`..Q..P.ZQ......g.......2.. ...b...
343
GET /favicon.ico HTTP/1.1
Host: ec2-18-223-184-200.us-east-2.compute.amazonaws.com
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:68.0) Gecko/20100101 Firefox/68.0
Accept: image/webp,*/*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Pragma: no-cache
Cache-Control: no-cache


        542
HTTP/1.1 404 Not Found
Date: Fri, 23 Aug 2019 15:56:37 GMT
Server: Apache/2.4.29 (Ubuntu)
Content-Length: 326
Keep-Alive: timeout=5, max=99
Connection: Keep-Alive
Content-Type: text/html; charset=iso-8859-1

<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
<html><head>
<title>404 Not Found</title>
</head><body>
<h1>Not Found</h1>
<p>The requested URL /favicon.ico was not found on this server.</p>
<hr>
<address>Apache/2.4.29 (Ubuntu) Server at ec2-18-223-184-200.us-east-2.compute.amazonaws.com Port 443</address>
</body></html>

===================================================================
```

This can also be done using the Wireshark GUI: Edit -> Preferences -> Protocols -> SSL (or TLS in newer versions) -> Edit RSA Key List -> Add new entry:

![](images/wirehsark_ssl.png)

The flag is sent as one of the HTTP headers:
```
Pico-Flag: picoCTF{nongshim.shrimp.crackers}
```


