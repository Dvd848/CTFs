# QueueR
Misc., 15 points

## Description
> I found that pcap file on the USB drive of a communist developer I know. Can you find out what he's trying to say?
> 
> (p.s. there is a small fake flag in the challenge, you can just ignore it if you find it)


A network capture was provided.

## Solution

Let's take a look at the network capture:

```console
root@kali:/media/sf_CTFs/kaf/QueueR# tshark -r QueueR.pcap --disable-protocol x11
Running as user "root" and group "root". This could be dangerous.
    1   0.000000 192.168.231.142 → 34.89.220.233 TCP 74 33824 → 6010 [SYN] Seq=0 Win=64240 Len=0 MSS=1460 SACK_PERM=1 TSval=3238209211 TSecr=0 WS=128 33824 6010
    2   0.099851 34.89.220.233 → 192.168.231.142 TCP 60 6010 → 33824 [SYN, ACK] Seq=0 Ack=1 Win=64240 Len=0 MSS=1460 6010 33824
    3   0.099924 192.168.231.142 → 34.89.220.233 TCP 54 33824 → 6010 [ACK] Seq=1 Ack=1 Win=64240 Len=0 33824 6010
    4   0.208842 34.89.220.233 → 192.168.231.142 TCP 505 6010 → 33824 [PSH, ACK] Seq=1 Ack=1 Win=64240 Len=451 6010 33824
    5   0.208875 192.168.231.142 → 34.89.220.233 TCP 54 33824 → 6010 [ACK] Seq=1 Ack=452 Win=63789 Len=0 33824 6010
    6   0.881580 192.168.231.142 → 34.89.220.233 TCP 79 33824 → 6010 [PSH, ACK] Seq=1 Ack=452 Win=63789 Len=25 33824 6010
    7   0.881854 34.89.220.233 → 192.168.231.142 TCP 60 6010 → 33824 [ACK] Seq=452 Ack=26 Win=64240 Len=0 6010 33824
    8   0.968644 34.89.220.233 → 192.168.231.142 TCP 60 6010 → 33824 [PSH, ACK] Seq=452 Ack=26 Win=64240 Len=5 6010 33824
    9  25.865046 34.89.220.233 → 192.168.231.142 TCP 498 [TCP ACKed unseen segment] [TCP Previous segment not captured] 6010 → 33824 [PSH, ACK] Seq=11633 Ack=564 Win=64240 Len=444 6010 33824
   10  25.865104 192.168.231.142 → 34.89.220.233 TCP 54 [TCP ACKed unseen segment] [TCP Previous segment not captured] 33824 → 6010 [ACK] Seq=564 Ack=12077 Win=63789 Len=0 33824 6010
   11  26.594547 192.168.231.142 → 34.89.220.233 TCP 74 33824 → 6010 [PSH, ACK] Seq=564 Ack=12077 Win=63789 Len=20 33824 6010
   12  26.594969 34.89.220.233 → 192.168.231.142 TCP 60 [TCP ACKed unseen segment] 6010 → 33824 [ACK] Seq=12077 Ack=584 Win=64240 Len=0 6010 33824
   13  26.697453 34.89.220.233 → 192.168.231.142 TCP 60 6010 → 33824 [PSH, ACK] Seq=12077 Ack=584 Win=64240 Len=5 6010 33824
   14  26.697492 192.168.231.142 → 34.89.220.233 TCP 54 33824 → 6010 [ACK] Seq=584 Ack=12082 Win=63789 Len=0 33824 6010
   15  26.848374 34.89.220.233 → 192.168.231.142 TCP 493 6010 → 33824 [PSH, ACK] Seq=12082 Ack=584 Win=64240 Len=439 6010 33824
   16  27.566056 192.168.231.142 → 34.89.220.233 TCP 79 33824 → 6010 [PSH, ACK] Seq=584 Ack=12521 Win=63789 Len=25 33824 6010
   17  27.566149 34.89.220.233 → 192.168.231.142 TCP 60 6010 → 33824 [ACK] Seq=12521 Ack=609 Win=64240 Len=0 6010 33824
   18  27.827863 34.89.220.233 → 192.168.231.142 TCP 489 [TCP Previous segment not captured] 6010 → 33824 [PSH, ACK] Seq=12526 Ack=609 Win=64240 Len=435 6010 33824
   19  28.573485 192.168.231.142 → 34.89.220.233 TCP 74 [TCP ACKed unseen segment] 33824 → 6010 [PSH, ACK] Seq=609 Ack=12961 Win=63789 Len=20 33824 6010
   20  28.573624 34.89.220.233 → 192.168.231.142 TCP 60 6010 → 33824 [ACK] Seq=12961 Ack=629 Win=64240 Len=0 6010 33824
   21  28.689991 34.89.220.233 → 192.168.231.142 TCP 60 6010 → 33824 [PSH, ACK] Seq=12961 Ack=629 Win=64240 Len=5 6010 33824
   22  28.690024 192.168.231.142 → 34.89.220.233 TCP 54 33824 → 6010 [ACK] Seq=629 Ack=12966 Win=63789 Len=0 33824 6010
   23  29.540499 192.168.231.142 → 34.89.220.233 TCP 63 [TCP ACKed unseen segment] 33824 → 6010 [PSH, ACK] Seq=629 Ack=13393 Win=63789 Len=9 33824 6010
   24  29.540865 34.89.220.233 → 192.168.231.142 TCP 60 [TCP Previous segment not captured] 6010 → 33824 [ACK] Seq=13393 Ack=638 Win=64240 Len=0 6010 33824
   25  29.642849 34.89.220.233 → 192.168.231.142 TCP 60 6010 → 33824 [PSH, ACK] Seq=13393 Ack=638 Win=64240 Len=5 6010 33824
   26  29.642882 192.168.231.142 → 34.89.220.233 TCP 54 [TCP ACKed unseen segment] 33824 → 6010 [ACK] Seq=638 Ack=13398 Win=63789 Len=0 33824 6010
   27  29.643591 34.89.220.233 → 192.168.231.142 TCP 80 6010 → 33824 [FIN, PSH, ACK] Seq=13398 Ack=638 Win=64240 Len=26 6010 33824
   28  29.643748 192.168.231.142 → 34.89.220.233 TCP 54 33824 → 6010 [FIN, ACK] Seq=638 Ack=13425 Win=63789 Len=0 33824 6010
   29  29.643828 34.89.220.233 → 192.168.231.142 TCP 60 6010 → 33824 [ACK] Seq=13425 Ack=639 Win=64239 Len=0 6010 33824
```

We can see a short session between two IPs, let's follow the TCP stream:
```console
root@kali:/media/sf_CTFs/kaf/QueueR# tshark -r QueueR.pcap  -z follow,tcp,ascii,0
Follow: tcp,ascii
Filter: tcp.stream eq 0
Node 0: 192.168.231.142:33824
Node 1: 34.89.220.233:6010
        451
.PNG
.
.(=.$.lB.f.Tk......J....|UxB........RJ).T...a!..D.PV.]u..r$.......I....u..
.eeV..i....P..S$?....\}...cv.p....0....Mj....lw. ...t..Z{...=...n.G...u....L...6.V.mSO....j.U.9.PO=.}...RJ)..'Q..&..........IEND.B`....T.>.i..f^K......0...............J..X|a9.RGs.
25
TCP/IP Illustrated Vol. 1
        5
Good!
        38
[11176 bytes missing in capture file].
        444
.PNG
.
r.|.......*.e$....&..vC 0^..=._.{<|a...^...SN9..S[.l\....y..Uu..j%I=....QI.tK=^.!.<z\.`.......L..J.......o...<u...x\j.}- .R.....][U....W*.h.J..Ti%.........=.......~...2..H...X.b+..75..J.S.QW...=gy....... .J.....vU...]P..I.P..<.|.5.....E.....=S?sL./.t:.....t..2k.....J.7.k..]...`....6.:[U...].Y.{(.....R..F9..SN.....2.fa.XI....IEND.B`.
36
[538 bytes missing in capture file].
20
Hacking, 2nd Edition
        5
Good!
        439
.PNG
.
IHDR..."...".....u......~IDATx...M..0....g..;...n...od....!.d.3.......'..j.m...#.....
}.YR;.o....M.%&..=.....j.?.U.gS5.t..k.........Y..1b.r........?..{.MW.%.1.G..Fmr...b.t...=..9kS.&g.]....V...?.bS...@.;..ipT....}L......1...R..ku...4..O.....l.9..Z\^4)z.=.g....
.c.I..4HZ_........R...TPA.u....0...f......IEND.B`.
25
Practical Binary Analysis
        34
[5 bytes missing in capture file].
        435
.PNG
.
IHDR..."...".....u......zIDATx...An.0.E......=J.f..p.. ,..~.I........{...I|9&......{....r.)..NY.....4......AEIR..W...I.n...:.5.9.......u..j~..-......_.}&....x\..}+`..h.\7][U...7.Z.....e...vM.........g...#P.R2j*~...DL..y.._.*.....DL!/.<..P...~LA:.j.{......(.M.T.Ci>....m...!......c.4U.=Qf
..}...}S5.........SWsL..>7Xk.:.5.1..<H.........aoG#&(...t....!wa..+S.x^G........ .....%..:....I.......3...)..r..G..............IEND.B`.
20
Serious Cryptography
        5
Good!
9
Reversing
        36
[427 bytes missing in capture file].
        5
Good!
        26
KAF{n07_7h47_345y_c0mr4d3}
===================================================================
```

Apart from the fake flag, we can see several PNG files and titles of books. Let's try to extract the PNG files:
```console
root@kali:/media/sf_CTFs/kaf/QueueR/temp# foremost ../QueueR.pcap
Processing: ../QueueR.pcap
|*|
root@kali:/media/sf_CTFs/kaf/QueueR/temp# cd output/
root@kali:/media/sf_CTFs/kaf/QueueR/temp/output# ls
audit.txt  png
root@kali:/media/sf_CTFs/kaf/QueueR/temp/output# cd png
root@kali:/media/sf_CTFs/kaf/QueueR/temp/output/png# ls
00000000.png  00000002.png  00000004.png  00000005.png
```

We got four files, which upon inspection seem to be QR codes:
```console
root@kali:/media/sf_CTFs/kaf/QueueR/temp/output/png# zbarimg *
QR-Code:9780201633467
QR-Code:9781593271442
QR-Code:9781593279127
QR-Code:9781593278267
scanned 4 barcode symbols from 4 images in 0.11 seconds
```

Searching for these numbers, they seem to represent [ISBN Codes](https://en.wikipedia.org/wiki/International_Standard_Book_Number):

```console
root@kali:/media/sf_CTFs/kaf/QueueR/temp/output/png# for isbn in `zbarimg *.png -q | sed 's/^QR-Code:\(.*\)$/\1/'`; do echo $isbn; curl -s https://api.altmetric.com/v1/isbn/$isbn | python -m json.tool | grep title; echo "--"; done
9780201633467
    "title": "TCP/IP illustrated",
--
9781593271442
    "title": "Hacking: The Art of Exploitation",
--
9781593279127
No JSON object could be decoded
--
9781593278267
    "title": "Serious Cryptography",
--
```

Let's further inspect the session between the two IPs. We'll dive into the following section as an example:
```
    1   0.000000 192.168.231.142 → 34.89.220.233 TCP 74 33824 → 6010 [SYN] Seq=0 Win=64240 Len=0 MSS=1460 SACK_PERM=1 TSval=3238209211 TSecr=0 WS=128 33824 6010
    2   0.099851 34.89.220.233 → 192.168.231.142 TCP 60 6010 → 33824 [SYN, ACK] Seq=0 Ack=1 Win=64240 Len=0 MSS=1460 6010 33824
    3   0.099924 192.168.231.142 → 34.89.220.233 TCP 54 33824 → 6010 [ACK] Seq=1 Ack=1 Win=64240 Len=0 33824 6010
    4   0.208842 34.89.220.233 → 192.168.231.142 TCP 505 6010 → 33824 [PSH, ACK] Seq=1 Ack=1 Win=64240 Len=451 6010 33824
    5   0.208875 192.168.231.142 → 34.89.220.233 TCP 54 33824 → 6010 [ACK] Seq=1 Ack=452 Win=63789 Len=0 33824 6010
    6   0.881580 192.168.231.142 → 34.89.220.233 TCP 79 33824 → 6010 [PSH, ACK] Seq=1 Ack=452 Win=63789 Len=25 33824 6010
    7   0.881854 34.89.220.233 → 192.168.231.142 TCP 60 6010 → 33824 [ACK] Seq=452 Ack=26 Win=64240 Len=0 6010 33824
    8   0.968644 34.89.220.233 → 192.168.231.142 TCP 60 6010 → 33824 [PSH, ACK] Seq=452 Ack=26 Win=64240 Len=5 6010 33824
```

First, the client opens a connection to the server. The server immediately sends some data (a PNG file):
```console
root@kali:/media/sf_CTFs/kaf/QueueR# tshark -r QueueR.pcap  -Y 'frame.number==4' -x
Running as user "root" and group "root". This could be dangerous.
0000  00 0c 29 36 af 15 00 50 56 e4 8c 53 08 00 45 00   ..)6...PV..S..E.
0010  01 eb c2 16 00 00 80 06 cf 7c 22 59 dc e9 c0 a8   .........|"Y....
0020  e7 8e 17 7a 84 20 4f d6 7c 37 af b3 5d d2 50 18   ...z. O.|7..].P.
0030  fa f0 cc 88 00 00 89 50 4e 47 0d 0a 1a 0a 00 00   .......PNG......
0040  00 0d 49 48 44 52 00 00 01 22 00 00 01 22 01 00   ..IHDR..."..."..
0050  00 00 00 75 c5 e2 1b 00 00 01 8a 49 44 41 54 78   ...u.......IDATx
0060  9c ed 98 41 8e 83 20 18 85 df 3f 90 74 a9 c9 1c   ...A.. ...?.t...
0070  a0 47 c1 2b cf 0d e4 28 3d c0 24 b2 6c 42 f3 66   .G.+...(=.$.lB.f
0080  01 54 6b 17 9d 8d 15 ea cf 4a e8 97 f8 f2 7c 55   .Tk......J....|U
0090  78 42 bc 1e fe eb 1f 10 a0 94 52 4a 29 a5 54 ed   xB........RJ).T.
00a0  94 e4 61 21 d2 df 44 86 50 56 86 5d 75 1d 82 72   ..a!..D.PV.]u..r
00b0  24 c9 09 00 c2 89 00 0c 49 92 8f d4 fb 75 1d 82   $.......I....u..
00c0  0a f7 8c 97 21 22 76 cb 3b 2a b5 a6 38 76 57 79   ....!"v.;*..8vWy
00d0  eb 1d 8f 4b d9 d5 5c dc e5 c4 27 f3 6b 55 df 36   ...K..\...'.kU.6
00e0  55 bc ef 08 20 00 f4 3d 92 f5 cb 43 57 ad ea db   U... ..=...CW...
00f0  a6 b2 f7 3e f9 6d 20 6e fa 8e 02 dc 1e b2 5f ab   ...>.m n......_.
0100  fa b6 a9 e4 fd 9c 71 fa f3 55 f2 df 60 4f 5d 47   ......q..U..`O]G
0110  a0 90 b6 92 8e 31 cf dd 64 08 74 24 dc 84 bc f9   .....1..d.t$....
0120  1c 6b 55 df 36 55 72 1f 7a 00 dd af a5 3f 47 c0   .kU.6Ur.z....?G.
0130  fd d8 08 c0 94 07 52 ab fa b6 a9 92 fb 09 e0 88   ......R.........
0140  1c f9 7c 05 43 cd fd 86 54 f2 3e 1f 69 cb 9b 66   ..|.C...T.>.i..f
0150  5e 4b 17 ea fd 86 94 0c 30 04 82 08 fc 99 04 82   ^K......0.......
0160  05 90 c2 bf a7 ae cf a6 4a c6 cb 58 7c 61 39 e5   ........J..X|a9.
0170  52 47 73 bf 0d 85 65 65 56 de f7 69 e2 d4 fb b7   RGs...eeV..i....
0180  50 cb 1e 53 24 3f 80 b9 d9 a9 5c 7d e3 d4 dc 63   P..S$?....\}...c
0190  76 11 70 17 0b f8 de 30 af d5 ae be 4d 6a dd a5   v.p....0....Mj..
01a0  e5 cf 6c 77 15 20 d8 c8 bd 74 1d 81 5a 7b 9f cd   ..lw. ...t..Z{..
01b0  0e 3d 08 dc 04 6e dc 47 d7 11 a8 75 8f 99 86 1f   .=...n.G...u....
01c0  4c 14 c0 94 36 b9 56 f5 6d 53 4f fb 9c fb d9 6a   L...6.V.mSO....j
01d0  fe 55 f7 39 db 50 4f 3d e6 7d a2 fd bd 52 4a 29   .U.9.PO=.}...RJ)
01e0  a5 d4 27 51 7f c0 26 dc 00 a0 1f 03 0f 00 00 00   ..'Q..&.........
01f0  00 49 45 4e 44 ae 42 60 82                        .IEND.B`.
```

The client responds with the name of the book:
```console
root@kali:/media/sf_CTFs/kaf/QueueR# tshark -r QueueR.pcap  -Y 'frame.number==6' -x
Running as user "root" and group "root". This could be dangerous.
0000  00 50 56 e4 8c 53 00 0c 29 36 af 15 08 00 45 00   .PV..S..)6....E.
0010  00 41 ec 8f 40 00 40 06 a6 ad c0 a8 e7 8e 22 59   .A..@.@......."Y
0020  dc e9 84 20 17 7a af b3 5d d2 4f d6 7d fa 50 18   ... .z..].O.}.P.
0030  f9 2d a7 ad 00 00 54 43 50 2f 49 50 20 49 6c 6c   .-....TCP/IP Ill
0040  75 73 74 72 61 74 65 64 20 56 6f 6c 2e 20 31      ustrated Vol. 1
```

And the server confirms the solution:
```console
root@kali:/media/sf_CTFs/kaf/QueueR# tshark -r QueueR.pcap  -Y 'frame.number==8' -x
Running as user "root" and group "root". This could be dangerous.
0000  00 0c 29 36 af 15 00 50 56 e4 8c 53 08 00 45 00   ..)6...PV..S..E.
0010  00 2d c2 26 00 00 80 06 d1 2a 22 59 dc e9 c0 a8   .-.&.....*"Y....
0020  e7 8e 17 7a 84 20 4f d6 7d fa af b3 5d eb 50 18   ...z. O.}...].P.
0030  fa f0 be 7e 00 00 47 6f 6f 64 21 00               ...~..Good!.
```

Let's try contacting the server ourselves:
```python
import socket

TCP_IP = '34.89.220.233'
TCP_PORT = 6010
BUFFER_SIZE = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
data = s.recv(BUFFER_SIZE)
with open("out.png", "wb") as o:
    o.write(data)
s.close()
```

Output:

```console
root@kali:/media/sf_CTFs/kaf/QueueR# python tcp.py
root@kali:/media/sf_CTFs/kaf/QueueR# file out.png
out.png: PNG image data, 290 x 290, 1-bit grayscale, non-interlaced
root@kali:/media/sf_CTFs/kaf/QueueR# zbarimg out.png
QR-Code:9781118026472
scanned 1 barcode symbols from 1 images in 0.03 seconds

root@kali:/media/sf_CTFs/kaf/QueueR# curl -s https://api.altmetric.com/v1/isbn/$(zbarimg out.png -q | sed 's/^QR-Code:\(.*\)$/\1/') | python -m json.tool | grep title
    "title": "The Web Application Hacker's Handbook: Finding and Exploiting Security Flaws, 2nd Edition",
```

Looks like we can make contact! Now let's try answering a challenge:
```python
from pyzbar.pyzbar import decode
from PIL import Image
import requests
import socket
import io

TCP_IP = '34.89.220.233'
TCP_PORT = 6010
BUFFER_SIZE = 1024

def get_isbn_from_img(img_data):
    img = Image.open(io.BytesIO(img_data))
    decoded = decode(img)
    return decoded[0].data

def get_name_from_isbn(isbn):
    r = requests.get("https://api.altmetric.com/v1/isbn/{}".format(isbn))
    return r.json()["title"]

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
data = s.recv(BUFFER_SIZE)
isbn = get_isbn_from_img(data)
print isbn
name = get_name_from_isbn(isbn)
print name
s.send(name)
data = s.recv(BUFFER_SIZE)
print (data)
s.close()
```

Output:
```console
root@kali:/media/sf_CTFs/kaf/QueueR# python tcp.py
9780131103627
The C Programming Language
Good!
```

As the last step, let's run in a loop and answer all the challenges until we receive the flag:
```python
from pyzbar.pyzbar import decode
from PIL import Image
import requests
import socket
import io

# Since the online service does not contain all ISBNs, we use this 
#  list as a fallback, manually adding entries to it if the online service fails.
d = {}
with open("db.txt") as db:
    for line in db:
        line = line.rstrip()
        isbn, name = line.split("|")
        d[isbn] = name


TCP_IP = '34.89.220.233'
TCP_PORT = 6010
BUFFER_SIZE = 1024

def get_isbn_from_img(img_data):
    img = Image.open(io.BytesIO(img_data))
    decoded = decode(img)
    return decoded[0].data

def get_name_from_isbn(isbn):
    if isbn in d:
        return d[isbn]
    r = requests.get("https://api.altmetric.com/v1/isbn/{}".format(isbn))
    return r.json()["title"]

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
while True:
    data = ""
    try:
        data = s.recv(BUFFER_SIZE)
        isbn = get_isbn_from_img(data)
        print isbn
        name = get_name_from_isbn(isbn)
        print name
        s.send(name)
        data = s.recv(BUFFER_SIZE)
        print (data)
    except IOError:
        print data
        break
s.close()
```

Output:
```console
root@kali:/media/sf_CTFs/kaf/QueueR# python 1.py
9780735684188
Windows Internals, Part 1: System architecture, processes, threads, memory management, and more (7th Edition)
Good!
9781593278595
Malware Data Science: Attack Detection and Attribution
Good!
9780262640688
The Elements of Computing Systems: Building a Modern Computer from First Principles
Good!
9781476734293
The Communist Manifesto
Good!
9781977593375
Windows Kernel Programming
Good!
9781985086593
Operating Systems: Three Easy Pieces
Good!
9780321294319
Rootkits : subverting the Windows kernel
Good!
9781593272906
Practical Malware Analysis: The Hands-On Guide to Dissecting Malicious Software
Good!
9781118026472
The Web Application Hacker's Handbook: Finding And: Dafydd Stuttard
Good!
9780133591620
Modern Operating Systems
Good!
9781593278267
Serious Cryptography: A Practical Introduction to Modern Encryption
Good!
9781118787311
Practical Reverse Engineering : x86, x64, ARM, Windows Kernel, Reversing Tools, and Obfuscation
Good!
9781593270476
The TCP/IP Guide
Good!
9781593278021
Practical Packet Analysis
Good!
9781593277161
Rootkits and Bootkits Reversing Modern Malware and Next Generation Threats
Good!
9780133594140
Computer Networking: A Top-Down Approach
Good!
9780134997834
A Tour of C++
Good!
9780132126953
Computer Networks
Good!
9780201633467
TCP/IP Illustrated, Vol. 1: The Protocols (Addison-Wesley Professional Computing Series)
Good!
9781118825099
The Art of Memory Forensics: Detecting Malware and Threats in Windows, Linux, and Mac Memory
Good!
9780201633610
Design Patterns: Elements of Reusable Object-Oriented Software
Good!
9780131103627
C Programming Language
Good!
9781593276690
Game Hacking
Good!
9781593271442
Hacking : The Art of Exploitation
Good!
9781449626365
The Rootkit Arsenal
Good!
9781119096726
Applied Cryptography
Good!
9780764574818
Reversing : secrets of reverse engineering
Good!
9780321714114
C++ Primer
Good!
9780470080238
The Shellcoder's Handbook: Discovering and Exploiting Security Holes
Good!
9781593279127
Practical Binary Analysis: Build Your Own Linux Tools for Binary Instrumentation Analysis and Disassembly
Good!KAF{k4r1_m4rx_15_7h3_b357}
```