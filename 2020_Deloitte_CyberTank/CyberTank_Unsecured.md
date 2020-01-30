# CyberTank Unsecured
Network

A network capture was attached.

## Authentication (25 points)

> The CyberTank engineers build super-robust cars. Their website on the other hand appears to be using plain HTTP. Yikes!
> 
> After capturing some unsecured communications you could even find the password to the employee area!
> 
> Flag format: password

Let's take a look at the network capture:
```
root@kali:/media/sf_CTFs/hackazon/CyberTank_Unsecured# tshark -r cybertank.pcap | head
Running as user "root" and group "root". This could be dangerous.
    1   0.000000 192.168.164.1 → 203.0.113.9  TCP 66 50242 → 80 [SYN] Seq=0 Win=64240 Len=0 MSS=1460 WS=256 SACK_PERM=1 50242 80
    2   0.000092  203.0.113.9 → 192.168.164.1 TCP 66 80 → 50242 [SYN, ACK] Seq=0 Ack=1 Win=29200 Len=0 MSS=1460 SACK_PERM=1 WS=128 80 50242
    3   0.000469 192.168.164.1 → 203.0.113.9  TCP 60 50242 → 80 [ACK] Seq=1 Ack=1 Win=1051136 Len=0 50242 80
    4   0.002270 192.168.164.1 → 203.0.113.9  HTTP 509 GET /privacypolicy HTTP/1.1  50242 80
    5   0.002328  203.0.113.9 → 192.168.164.1 TCP 54 80 → 50242 [ACK] Seq=1 Ack=456 Win=30336 Len=0 80 50242
    6   0.002859  203.0.113.9 → 192.168.164.1 TCP 233 HTTP/1.1 200 OK  [TCP segment of a reassembled PDU] 80 50242
    7   0.002977  203.0.113.9 → 192.168.164.1 TCP 7354 80 → 50242 [ACK] Seq=180 Ack=456 Win=30336 Len=7300 [TCP segment of a reassembled PDU] 80 50242
    8   0.003164  203.0.113.9 → 192.168.164.1 HTTP 1431 HTTP/1.1 200 OK  (text/html) 80 50242
    9   0.003738 192.168.164.1 → 203.0.113.9  TCP 60 50242 → 80 [ACK] Seq=456 Ack=8858 Win=1051136 Len=0 50242 80
   10   0.005770 192.168.164.1 → 203.0.113.9  TCP 60 50242 → 80 [FIN, ACK] Seq=456 Ack=8858 Win=1051136 Len=0 50242 80
```

The capture contained many HTTP streams, capturing an HTTP session.

At some stage, the user asks for a page which requires authorization:
```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Unsecured# tshark -r cybertank.pcap -qz follow,tcp,ascii,20
Running as user "root" and group "root". This could be dangerous.

===================================================================
Follow: tcp,ascii
Filter: tcp.stream eq 20
Node 0: 192.168.164.1:50262
Node 1: 203.0.113.9:80
451
GET /employees HTTP/1.1
Host: cybertank.tlv
Connection: keep-alive
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate
Accept-Language: nl-NL,nl;q=0.9,en-US;q=0.8,en;q=0.7


        246
HTTP/1.1 401 Unauthorized
Host: cybertank.tlv
Date: Sat, 25 Jan 2020 20:20:42 GMT
Connection: close
X-Powered-By: PHP/7.2.24-0ubuntu0.18.04.1
WWW-Authenticate: Basic realm="CyberTank Employee Area"
Content-type: text/html; charset=UTF-8
```

This uses the HTTP Authentication scheme.

The next stream shows the request after the user has entered his username and password:

```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Unsecured# tshark -r cybertank.pcap -qz follow,tcp,ascii,21 | head -n 20
Running as user "root" and group "root". This could be dangerous.

===================================================================
Follow: tcp,ascii
Filter: tcp.stream eq 21
Node 0: 192.168.164.1:50265
Node 1: 203.0.113.9:80
536
GET /employees HTTP/1.1
Host: cybertank.tlv
Connection: keep-alive
Cache-Control: max-age=0
Authorization: Basic Y3liZXJ0YW5rOlN1cGVyQ3liZXJTZWNyZXQ=
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate
Accept-Language: nl-NL,nl;q=0.9,en-US;q=0.8,en;q=0.7
```

The "Authorization (Basic)" header is basically a base-64 encoded string of the username and password, separated by a semicolon:
```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Unsecured# echo Y3liZXJ0YW5rOlN1cGVyQ3liZXJTZWNyZXQ= | base64 -d
cybertank:SuperCyberSecret
```

Therefore, the password is `SuperCyberSecret`.

## Research server (75 points)

> It looks like a key of some sort was transferred and the attacker connected to a system on EC2. Can you gain access to the associated server?
> 
> Hint: the user account is "developer".
> 
> Flag format: CTF{32-hex}

We continue scanning through the PCAP file and find the following stream:

```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Unsecured# tshark -r cybertank.pcap -qz follow,tcp,ascii,42 | head -n 20
Running as user "root" and group "root". This could be dangerous.

===================================================================
Follow: tcp,ascii
Filter: tcp.stream eq 42
Node 0: 192.168.164.1:50286
Node 1: 63.32.136.47:22
        41
SSH-2.0-OpenSSH_7.6p1 Ubuntu-4ubuntu0.3

28
SSH-2.0-PuTTY_Release_0.73

        1080
...4..F..Q.vFz......M@....curve25519-sha256,curve25519-sha256@libssh.org,ecdh-sha2-nistp256,ecdh-sha2-nistp384,ecdh-sha2-nistp521,diffie-hellman-group-exchange-sha256,diffie-hellman-group16-sha512,diffie-hellman-group18-sha512,diffie-hellman-group14-sha256,diffie-hellman-group14-sha1...Assh-rsa,rsa-sha2-512,rsa-sha2-256,ecdsa-sha2-nistp256,ssh-ed25519...lchacha20-poly1305@openssh.com,aes128-ctr,aes192-ctr,aes256-ctr,aes128-gcm@openssh.com,aes256-gcm@openssh.com...lchacha20-poly1305@openssh.com,aes128-ctr,aes192-ctr,aes256-ctr,aes128-gcm@openssh.com,aes256-gcm@openssh.com....umac-64-etm@openssh.com,umac-128-etm@openssh.com,hmac-sha2-256-etm@openssh.com,hmac-sha2-512-etm@openssh.com,hmac-sha1-etm@openssh.com,umac-64@openssh.com,umac-128@openssh.com,hmac-sha2-256,hmac-sha2-512,hmac-sha1....umac-64-etm@openssh.com,umac-128-etm@openssh.com,hmac-sha2-256-etm@openssh.com,hmac-sha2-512-etm@openssh.com,hmac-sha1-etm@openssh.com,umac-64@openssh.com,umac-128@openssh.com,hmac-sha2-256,hmac-sha2-512,hmac-sha1....none,zlib@openssh.com....none,zlib@openssh.com...................
1168
......h4..9.p?A%..........curve25519-sha256@libssh.org,ecdh-sha2-nistp256,ecdh-sha2-nistp384,ecdh-sha2-nistp521,diffie-hellman-group-exchange-sha256,diffie-hellman-group-exchange-sha1,diffie-hellman-group14-sha256,diffie-hellman-group14-sha1,rsa2048-sha256,rsa1024-sha1,diffie-hellman-group1-sha1...Wssh-ed25519,ecdsa-sha2-nistp256,ecdsa-sha2-nistp384,ecdsa-sha2-nistp521,ssh-rsa,ssh-dss....aes256-ctr,aes256-cbc,rijndael-cbc@lysator.liu.se,aes192-ctr,aes192-cbc,aes128-ctr,aes128-cbc,chacha20-poly1305@openssh.com,3des-ctr,3des-cbc,blowfish-ctr,blowfish-cbc,arcfour256,arcfour128....aes256-ctr,aes256-cbc,rijndael-cbc@lysator.liu.se,aes192-ctr,aes192-cbc,aes128-ctr,aes128-cbc,chacha20-poly1305@openssh.com,3des-ctr,3des-cbc,blowfish-ctr,blowfish-cbc,arcfour256,arcfour128....hmac-sha2-256,hmac-sha1,hmac-sha1-96,hmac-md5,hmac-sha2-256-etm@openssh.com,hmac-sha1-etm@openssh.com,hmac-sha1-96-etm@openssh.com,hmac-md5-etm@openssh.com....hmac-sha2-256,hmac-sha1,hmac-sha1-96,hmac-md5,hmac-sha2-256-etm@openssh.com,hmac-sha1-etm@openssh.com,hmac-sha1-96-etm@openssh.com,hmac-md5-etm@openssh.com....none,zlib,zlib@openssh.com....none,zlib,zlib@openssh.com...............:.
48
...,..... Fl...-.....,~["gy.N.F...q"4.S..o!"g.N.
        208
..m..`j...P?....VCJ58.i/M..^O).'Ee .W...m...B.c.V?s.o.W0....S....ssh-ed25519...@
```

This looks like an SSH connection to a remote server. How can we connect too?

We go back one stream and see the following:
```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Unsecured# tshark -r cybertank.pcap -qz follow,tcp,ascii,41
Running as user "root" and group "root". This could be dangerous.

===================================================================
Follow: tcp,ascii
Filter: tcp.stream eq 41
Node 0: 192.168.164.1:50285
Node 1: 203.0.113.9:80
575
GET /employees/key_research_server.zip HTTP/1.1
Host: cybertank.tlv
Connection: keep-alive
Authorization: Basic Y3liZXJ0YW5rOlN1cGVyQ3liZXJTZWNyZXQ=
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Referer: http://cybertank.tlv/employees
Accept-Encoding: gzip, deflate
Accept-Language: nl-NL,nl;q=0.9,en-US;q=0.8,en;q=0.7


        170
HTTP/1.1 200 OK
Host: cybertank.tlv
Date: Sat, 25 Jan 2020 20:21:02 GMT
Connection: close
X-Powered-By: PHP/7.2.24-0ubuntu0.18.04.1
content-type: application/zip


        1460
PK..........9P..y,............CyberTank_Research_Server.pemUT...I.,^h.,^ux.............m....J.Ds....+..A....Ah.(..w..._.w_..v.u...?...(......?OW../.1....?....T."......9S.T..8..oHTIn.....Mr5.....A.......<............."OF.....!%'.t...&..v8*.so..........Ax..:.+8.......y..*6....h.OP..O..I6......q.......i/.no....}A.J..j.W...T'...Z#...;<.*qTS...).F...!........q...c...!)..$......nV....X.NZ.VC.v4..$.ih.J]...-.u....9.@%.......F..*n....D0.e....o.3}.0....qV.;y.A.I....\..B..........?"1.9'.~..4`.y.t7...0..F..'&.pbo..z...PM. .Q..&....$._Ym
l.n.`..r....!^....U.9...d.S...Z...t
=.J..O...u,Q...{<t.....1JJ'sk....)Z+..(.{.YT|&1C\#v......x.Z..d....NxZ......=.,..nIs...!.(..B.>7...P.!D./.....z..QP....=...z......D..M..X4..M.g.....\..>M%0.!:..z ..jLI....m..=T]..!.6.....>.p.woZ.{|6x.%..9.........k=.5j..H.<.1..$..g
$.J.hS.<.t......g2.K.d.=7.s&......sB..^W..c..m.......H.).X.S.I..y...Pmw.+ih.u...N:.Yiy.Px..}N......c.6.1.*....+.....&..XF..=d.....Z..F.X....x.vL.m....1.~.;...b.......#;.>I.V...W9xGV....W.w.V..zU\.(.F..Q..{........jL......+....BN...%`k...w..V8D6h.eJU!S=R
.Q..[.:.....`RAA.../.rx..'. .....]..b..rq.E...O.W./...i..n.|..K....).vj....9.........K<..K..."....ip2ePh.v.fA.......'.]'..~.f.?.c......1..:.~.5O...._.....5._PK............9P..y,..........................CyberTank_Research_Server.pemUT...I.,^ux
        35
.............PK..........c...^.....
===================================================================
```

The user is requesting `/employees/key_research_server.zip` and receiving a file which contains the string `CyberTank_Research_Server.pem`. `*.pem` is a known extension for certificates and keys.

Let's extract the zip file from the stream:
```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Unsecured# tshark -r cybertank.pcap -o "tcp.desegment_tcp_streams: TRUE" -o "tcp.no_subdissector_on_error: FALSE" --export-objects "http,exported_objects" | grep zip
Running as user "root" and group "root". This could be dangerous.
  784  29.321267 192.168.164.1 → 203.0.113.9  HTTP 629 GET /employees/key_research_server.zip HTTP/1.1  50285 80
  788  29.325097  203.0.113.9 → 192.168.164.1 HTTP 89 HTTP/1.1 200 OK  (application/zip) 80 50285
root@kali:/media/sf_CTFs/hackazon/CyberTank_Unsecured# file exported_objects/key_research_server.zip
exported_objects/key_research_server.zip: Zip archive data, at least v2.0 to extract
```

Now, let's extract the file:
```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_Unsecured# unzip exported_objects/key_research_server.zip
Archive:  exported_objects/key_research_server.zip
  inflating: CyberTank_Research_Server.pem
root@kali:/media/sf_CTFs/hackazon/CyberTank_Unsecured# file CyberTank_Research_Server.pem
CyberTank_Research_Server.pem: PEM RSA private key
```

We can use it to connect to the remote server IP:
```console
root@kali:~/CTFs/hackazon/CyberTank_Unsecured# ssh -i CyberTank_Research_Server.pem developer@63.32.136.47
Last login: Tue Jan 28 18:54:25 2020 from 109-186-108-242.bb.netvision.net.il

       __|  __|_  )
       _|  (     /   Amazon Linux 2 AMI
      ___|\___|___|

https://aws.amazon.com/amazon-linux-2/
3 package(s) needed for security, out of 24 available
Run "sudo yum update" to apply all updates.
[developer@ip-172-31-1-183 ~]$ cd /home
[developer@ip-172-31-1-183 home]$ ls
developer  ec2-user  flag.txt
[developer@ip-172-31-1-183 home]$ cat flag.txt
CTF{2ca81e4466864f4ee82648b1941f1f61}
```