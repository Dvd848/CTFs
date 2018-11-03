# admin panel
Forensics, 150 points

## Description:
> We captured some traffic logging into the admin panel, can you find the password? 

A pcap file was attached ("data.pcap").

## Solution:

We can use WireShark to inspect the network capture via a GUI, or `tshark` as a command line utility.

First, we search for packets containing the word "password":
```console
root@kali:/media/sf_CTFs/pico/admin_panel# tshark -nr data.pcap -Y 'frame contains "password"'
    8   0.004782 192.168.3.128 → 192.168.3.129 HTTP 2354 HTTP/1.0 200 OK  (text/html) 80 37556
   17  12.370383 192.168.3.129 → 192.168.3.128 HTTP 522 POST /login HTTP/1.1  (application/x-www-form-urlencoded) 37894 80
   21  12.373739 192.168.3.128 → 192.168.3.129 HTTP 569 HTTP/1.0 302 FOUND  (text/html) 80 37894
   27  12.434002 192.168.3.129 → 192.168.3.128 HTTP 480 GET /admin HTTP/1.1  37896 80
   43  14.662211 192.168.3.129 → 192.168.3.128 HTTP 486 GET /logout HTTP/1.1  37946 80
   47  14.665356 192.168.3.128 → 192.168.3.129 HTTP 653 HTTP/1.0 302 FOUND  (text/html) 80 37946
   60  14.675592 192.168.3.128 → 192.168.3.129 HTTP 906 HTTP/1.0 200 OK  (text/html) 80 37948
   68  37.234879 192.168.3.129 → 192.168.3.128 HTTP 542 POST /login HTTP/1.1  (application/x-www-form-urlencoded)[Packet size limited during capture] 38526 80
```

We can inspect the contents of each packet. POST requests to "/login" seem look like a good place to start.
If we check packet #68, we will find the flag:

```console
root@kali:/media/sf_CTFs/pico/admin_panel# tshark -nr data.pcap -Y 'frame.number==68' -x
0000  00 0c 29 1c df f2 00 0c 29 79 dc c4 08 00 45 00   ..).....)y....E.
0010  02 10 f6 7c 40 00 40 06 ba 19 c0 a8 03 81 c0 a8   ...|@.@.........
0020  03 80 96 7e 00 50 12 95 30 5d 2e 5b be 02 80 18   ...~.P..0].[....
0030  00 e5 8a 54 00 00 01 01 08 0a 18 8c 0c 8d f0 d7   ...T............
0040  91 77 50 4f 53 54 20 2f 6c 6f 67 69 6e 20 48 54   .wPOST /login HT
0050  54 50 2f 31 2e 31 0d 0a 48 6f 73 74 3a 20 31 39   TP/1.1..Host: 19
0060  32 2e 31 36 38 2e 33 2e 31 32 38 0d 0a 55 73 65   2.168.3.128..Use
0070  72 2d 41 67 65 6e 74 3a 20 4d 6f 7a 69 6c 6c 61   r-Agent: Mozilla
0080  2f 35 2e 30 20 28 58 31 31 3b 20 55 62 75 6e 74   /5.0 (X11; Ubunt
0090  75 3b 20 4c 69 6e 75 78 20 78 38 36 5f 36 34 3b   u; Linux x86_64;
00a0  20 72 76 3a 35 39 2e 30 29 20 47 65 63 6b 6f 2f    rv:59.0) Gecko/
00b0  32 30 31 30 30 31 30 31 20 46 69 72 65 66 6f 78   20100101 Firefox
00c0  2f 35 39 2e 30 0d 0a 41 63 63 65 70 74 3a 20 74   /59.0..Accept: t
00d0  65 78 74 2f 68 74 6d 6c 2c 61 70 70 6c 69 63 61   ext/html,applica
00e0  74 69 6f 6e 2f 78 68 74 6d 6c 2b 78 6d 6c 2c 61   tion/xhtml+xml,a
00f0  70 70 6c 69 63 61 74 69 6f 6e 2f 78 6d 6c 3b 71   pplication/xml;q
0100  3d 30 2e 39 2c 2a 2f 2a 3b 71 3d 30 2e 38 0d 0a   =0.9,*/*;q=0.8..
0110  41 63 63 65 70 74 2d 4c 61 6e 67 75 61 67 65 3a   Accept-Language:
0120  20 65 6e 2d 55 53 2c 65 6e 3b 71 3d 30 2e 35 0d    en-US,en;q=0.5.
0130  0a 41 63 63 65 70 74 2d 45 6e 63 6f 64 69 6e 67   .Accept-Encoding
0140  3a 20 67 7a 69 70 2c 20 64 65 66 6c 61 74 65 0d   : gzip, deflate.
0150  0a 52 65 66 65 72 65 72 3a 20 68 74 74 70 3a 2f   .Referer: http:/
0160  2f 31 39 32 2e 31 36 38 2e 33 2e 31 32 38 2f 0d   /192.168.3.128/.
0170  0a 43 6f 6e 74 65 6e 74 2d 54 79 70 65 3a 20 61   .Content-Type: a
0180  70 70 6c 69 63 61 74 69 6f 6e 2f 78 2d 77 77 77   pplication/x-www
0190  2d 66 6f 72 6d 2d 75 72 6c 65 6e 63 6f 64 65 64   -form-urlencoded
01a0  0d 0a 43 6f 6e 74 65 6e 74 2d 4c 65 6e 67 74 68   ..Content-Length
01b0  3a 20 35 33 0d 0a 43 6f 6e 6e 65 63 74 69 6f 6e   : 53..Connection
01c0  3a 20 6b 65 65 70 2d 61 6c 69 76 65 0d 0a 55 70   : keep-alive..Up
01d0  67 72 61 64 65 2d 49 6e 73 65 63 75 72 65 2d 52   grade-Insecure-R
01e0  65 71 75 65 73 74 73 3a 20 31 0d 0a 0d 0a 75 73   equests: 1....us
01f0  65 72 3d 61 64 6d 69 6e 26 70 61 73 73 77 6f 72   er=admin&passwor
0200  64 3d 70 69 63 6f 43 54 46 7b 6e 30 74 73 33 63   d=picoCTF{n0ts3c
0210  75 72 33 5f 39 66 65 65 64 66 62 63 7d            ur3_9feedfbc}
```

The flag is: picoCTF{n0ts3cur3_9feedfbc}