# shark on wire 2
Forensics, 300 points

## Description:
> We found this packet capture. Recover the flag that was pilfered from the network.


## Solution: 


This is the follow-up of [shark on wire 1](shark_on_wire_1.md).

Again, we receive a capture file:

```console
root@kali:/media/sf_CTFs/pico/shark_on_wire_2# tshark -r capture.pcap | head
Running as user "root" and group "root". This could be dangerous.
    1   0.000000 fe80::20c:29ff:fef7:6ad → ff02::fb     MDNS 180 Standard query 0x0000 PTR _nfs._tcp.local, "QM" question PTR _ftp._tcp.local, "QM" question PTR _webdav._tcp.local, "QM" question PTR _webdavs._tcp.local, "QM" question PTR _sftp-ssh._tcp.local, "QM" question PTR _smb._tcp.local, "QM" question PTR _afpovertcp._tcp.local, "QM" question 5353 5353
    2   0.000494     10.0.0.6 → 224.0.0.251  MDNS 160 Standard query 0x0000 PTR _nfs._tcp.local, "QM" question PTR _ftp._tcp.local, "QM" question PTR _webdav._tcp.local, "QM" question PTR _webdavs._tcp.local, "QM" question PTR _sftp-ssh._tcp.local, "QM" question PTR _smb._tcp.local, "QM" question PTR _afpovertcp._tcp.local, "QM" question 5353 5353
    3  11.996569 fe80::5ca9:605:a9be:272d → ff02::fb     MDNS 180 Standard query 0x0000 PTR _ftp._tcp.local, "QM" question PTR _nfs._tcp.local, "QM" question PTR _afpovertcp._tcp.local, "QM" question PTR _smb._tcp.local, "QM" question PTR _sftp-ssh._tcp.local, "QM" question PTR _webdavs._tcp.local, "QM" question PTR _webdav._tcp.local, "QM" question 5353 5353
    4  11.996897     10.0.0.5 → 224.0.0.251  MDNS 160 Standard query 0x0000 PTR _ftp._tcp.local, "QM" question PTR _nfs._tcp.local, "QM" question PTR _afpovertcp._tcp.local, "QM" question PTR _smb._tcp.local, "QM" question PTR _sftp-ssh._tcp.local, "QM" question PTR _webdavs._tcp.local, "QM" question PTR _webdav._tcp.local, "QM" question 5353 5353
    5  16.004668 fe80::20c:29ff:fef7:6ad → ff02::fb     MDNS 180 Standard query 0x0000 PTR _nfs._tcp.local, "QM" question PTR _ftp._tcp.local, "QM" question PTR _webdav._tcp.local, "QM" question PTR _webdavs._tcp.local, "QM" question PTR _sftp-ssh._tcp.local, "QM" question PTR _smb._tcp.local, "QM" question PTR _afpovertcp._tcp.local, "QM" question 5353 5353
    6  16.005020     10.0.0.6 → 224.0.0.251  MDNS 160 Standard query 0x0000 PTR _nfs._tcp.local, "QM" question PTR _ftp._tcp.local, "QM" question PTR _webdav._tcp.local, "QM" question PTR _webdavs._tcp.local, "QM" question PTR _sftp-ssh._tcp.local, "QM" question PTR _smb._tcp.local, "QM" question PTR _afpovertcp._tcp.local, "QM" question 5353 5353
    7  23.397257  192.168.2.1 → 192.168.2.3  TCP 63 60218 → 80 [PSH, ACK] Seq=1 Ack=1 Win=2051 Len=9 60218 80
    8  23.398075  192.168.2.3 → 192.168.2.1  TCP 60 80 → 60218 [PSH, ACK] Seq=1 Ack=10 Win=238 Len=3 80 60218
    9  23.439183  192.168.2.1 → 192.168.2.3  TCP 60 60218 → 80 [ACK] Seq=10 Ack=4 Win=2051 Len=0 60218 80
   10  29.557413 Vmware_b9:02:a9 → Broadcast    ARP 60 Who has 10.0.0.11? Tell 10.0.0.5
```

Trying the same strategy as before does not lead to any result this time:

```console
root@kali:/media/sf_CTFs/pico/shark_on_wire_2# PCAP=capture.pcap; END=$(tshark -r $PCAP -T fields -e udp.stream | sort -n | tail -1); for ((i=0;i<=END;i++)); do tshark -r $PCAP -Y "udp.stream eq $i" -T fields -e data.text -o data.show_as_text:TRUE 2>/dev/null | tr -d '\n' | grep "picoCTF"; if [ $? -eq 0 ]; then echo "(Stream #$i)"; fi; done
Running as user "root" and group "root". This could be dangerous.
picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!aaaaa
(Stream #9)
I really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flags
(Stream #10)
```

Inspecting the different UDP streams, we can see an interesting sequence which starts with a "start" message and ends with an "end" message:

```console
root@kali:/media/sf_CTFs/pico/shark_on_wire_2# PCAP=capture.pcap; END=$(tshark -r $PCAP -T fields -e udp.stream | sort -n | tail -1); for ((i=0;i<=END;i++)); do tshark -r $PCAP -Y "udp.stream eq $i" -T fields -e data.text -o data.show_as_text:TRUE 2>/dev/null | tr -d '\n'; echo ""; done
Running as user "root" and group "root". This could be dangerous.





i6f6e
kfdsalkfsalkico{N0t_a_fLag}
icoCTF{StaT31355e
fjdbanlkfdnfjdbanlkfdnfjdbanlkfdnfjdbanlkfdnfjdbanlkfdnfjdbanlkfdnfjdbanlkfdnfjdbanlkfdnfjdbanlkfdn
picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!picoCTF Sure is fun!aaaaa
I really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flagsI really want to find some picoCTF flags

C
T
F
fjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdnfjdsakf;lankeflksanlkfdn


AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA


ffffffffffffffffffffffffffffffffffffffff

_
36
}
zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa




start
aaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaa
aaaaa
aaaaa
aaaaa
aaaaa
aaaaa
aaaaa
aaaaaaaaaa
aaaaaaaaaa
aaaaa
aaaaaaaaaaaaaaa
aaaaa
aaaaaaaaaa
aaaaaaaaaaaaaaa

aaaaaaaaaaaaaaa
bbb
aaaaaaaaaa

aaaaa
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
aaaaa
aaaaa
aaaaa
aaaaa
end
aaaaaaaaaa
aaaaa
aaaaaaaaaa
aaaaa
aaaaa
```

Let's take a closer look at these two message:
```console
root@kali:/media/sf_CTFs/pico/shark_on_wire_2# tshark -nr capture.pcap  -Y 'frame contains "start"'
Running as user "root" and group "root". This could be dangerous.
 1104 991.587437    10.0.0.66 → 10.0.0.1     UDP 60 5000 → 22 Len=5 5000 22
root@kali:/media/sf_CTFs/pico/shark_on_wire_2# tshark -nr capture.pcap  -Y 'frame contains "end"'
Running as user "root" and group "root". This could be dangerous.
 1303 1171.059146    10.0.0.80 → 10.0.0.1     UDP 60 5000 → 22 Len=3 5000 22
```

The similar attributes are that they were both sent to 10.0.0.1, the source port is 5000 and the destination port is 22.

Filtering according to source and destination port, we only get these two messages:
```console
root@kali:/media/sf_CTFs/pico/shark_on_wire_2# tshark -nr capture.pcap  -Y 'udp.srcport == 5000 && udp.dstport == 22'
Running as user "root" and group "root". This could be dangerous.
 1104 991.587437    10.0.0.66 → 10.0.0.1     UDP 60 5000 → 22 Len=5 5000 22
 1303 1171.059146    10.0.0.80 → 10.0.0.1     UDP 60 5000 → 22 Len=3 5000 22
```

However filtering just according to the destination port, we get a list of messages between the start and the end:
```console
root@kali:/media/sf_CTFs/pico/shark_on_wire_2# tshark -nr capture.pcap  -Y 'udp.dstport == 22'
Running as user "root" and group "root". This could be dangerous.
 1104 991.587437    10.0.0.66 → 10.0.0.1     UDP 60 5000 → 22 Len=5 5000 22
 1106 993.672341    10.0.0.66 → 10.0.0.1     UDP 60 5112 → 22 Len=5 5112 22
 1118 1006.227400    10.0.0.66 → 10.0.0.1     UDP 60 5105 → 22 Len=5 5105 22
 1122 1008.323546    10.0.0.66 → 10.0.0.1     UDP 60 5099 → 22 Len=5 5099 22
 1124 1010.428768    10.0.0.66 → 10.0.0.1     UDP 60 5111 → 22 Len=5 5111 22
 1129 1012.535515    10.0.0.66 → 10.0.0.1     UDP 60 5067 → 22 Len=5 5067 22
 1131 1014.627130    10.0.0.66 → 10.0.0.1     UDP 60 5084 → 22 Len=5 5084 22
 1133 1016.719657    10.0.0.66 → 10.0.0.1     UDP 60 5070 → 22 Len=5 5070 22
 1135 1018.807279    10.0.0.66 → 10.0.0.1     UDP 60 5123 → 22 Len=5 5123 22
 1137 1020.899193    10.0.0.66 → 10.0.0.1     UDP 60 5112 → 22 Len=5 5112 22
 1139 1022.991480    10.0.0.66 → 10.0.0.1     UDP 60 5049 → 22 Len=5 5049 22
 1141 1025.083748    10.0.0.66 → 10.0.0.1     UDP 60 5076 → 22 Len=5 5076 22
 1143 1027.167730    10.0.0.66 → 10.0.0.1     UDP 60 5076 → 22 Len=5 5076 22
 1145 1029.255106    10.0.0.66 → 10.0.0.1     UDP 60 5102 → 22 Len=5 5102 22
 1147 1031.334799    10.0.0.66 → 10.0.0.1     UDP 60 5051 → 22 Len=5 5051 22
 1162 1043.850969    10.0.0.66 → 10.0.0.1     UDP 60 5114 → 22 Len=5 5114 22
 1164 1045.934960    10.0.0.66 → 10.0.0.1     UDP 60 5051 → 22 Len=5 5051 22
 1166 1048.019181    10.0.0.66 → 10.0.0.1     UDP 60 5100 → 22 Len=5 5100 22
 1172 1054.255069    10.0.0.66 → 10.0.0.1     UDP 60 5095 → 22 Len=5 5095 22
 1178 1060.507360    10.0.0.66 → 10.0.0.1     UDP 60 5100 → 22 Len=5 5100 22
 1180 1062.619741    10.0.0.66 → 10.0.0.1     UDP 60 5097 → 22 Len=5 5097 22
 1187 1066.779955    10.0.0.66 → 10.0.0.1     UDP 60 5116 → 22 Len=5 5116 22
 1189 1068.867478    10.0.0.66 → 10.0.0.1     UDP 60 5097 → 22 Len=5 5097 22
 1192 1070.959143    10.0.0.66 → 10.0.0.1     UDP 60 5095 → 22 Len=5 5095 22
 1196 1073.043525    10.0.0.66 → 10.0.0.1     UDP 60 5118 → 22 Len=5 5118 22
 1199 1075.127069    10.0.0.66 → 10.0.0.1     UDP 60 5049 → 22 Len=5 5049 22
 1267 1139.786992    10.0.0.66 → 10.0.0.1     UDP 60 5097 → 22 Len=5 5097 22
 1272 1141.870974    10.0.0.66 → 10.0.0.1     UDP 60 5095 → 22 Len=5 5095 22
 1274 1143.955404    10.0.0.66 → 10.0.0.1     UDP 60 5115 → 22 Len=5 5115 22
 1276 1146.043247    10.0.0.66 → 10.0.0.1     UDP 60 5116 → 22 Len=5 5116 22
 1284 1154.383039    10.0.0.66 → 10.0.0.1     UDP 60 5051 → 22 Len=5 5051 22
 1286 1156.475039    10.0.0.66 → 10.0.0.1     UDP 60 5103 → 22 Len=5 5103 22
 1296 1166.882937    10.0.0.66 → 10.0.0.1     UDP 60 5048 → 22 Len=5 5048 22
 1301 1168.975486    10.0.0.66 → 10.0.0.1     UDP 60 5125 → 22 Len=5 5125 22
 1303 1171.059146    10.0.0.80 → 10.0.0.1     UDP 60 5000 → 22 Len=3 5000 22
```

We can see that the second message originates from source port 5112. 112 is a number which should alert us, since its ASCII representation is `p`, which matches the flag template.

So, we can try to translate the source ports to ASCII using the following script:
```python
from scapy.all import *

flag = ""

packets = rdpcap('capture.pcap')
for packet in packets:
    if UDP in packet and packet[UDP].dport == 22:
        flag += chr(packet[UDP].sport - 5000)
print flag
```

Output:
```console
root@kali:/media/sf_CTFs/pico/shark_on_wire_2# python solve.py
 picoCTF{p1LLf3r3d_data_v1a_st3g0}
```