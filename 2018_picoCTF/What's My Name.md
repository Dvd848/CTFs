# What's My Name?
Forensics, 250 points

## Description:
> Say my name, say my name.

A pcap file was attached ("myname.pcap").

## Solution:

We can use WireShark to inspect the network capture via a GUI, or `tshark` as a command line utility.

First, we search for packets containing the word "name":
```console
root@kali:/media/sf_CTFs/pico/Whats_My_Name# tshark -nr myname.pcap -Y 'frame contains "name"'
   43 1418.341495  192.168.2.1 → 192.168.2.12 DNS 87 Standard query 0xaaa0 ANY thisismyname.com OPT 62905 53
   55 1418.342859 192.168.2.12 → 192.168.2.1  DNS 316 Standard query response 0xaaa0 ANY thisismyname.com A 192.168.2.13 CNAME myname.com MX 5 myname.com MX 10 mx2.myname.com MX 20 mx3.myname.com NS ns1.myname.com NS ns2.myname.com TXT SOA ns1.thisismyname.com 53 62905
  268   3.314481 192.168.86.16 → 255.255.255.255 DB-LSP-DISC 186 Dropbox LAN sync Discovery Protocol 17500 17500
  269   3.314862 192.168.86.16 → 192.168.86.255 DB-LSP-DISC 186 Dropbox LAN sync Discovery Protocol 17500 17500
 5419  33.420194 192.168.86.16 → 255.255.255.255 DB-LSP-DISC 186 Dropbox LAN sync Discovery Protocol 17500 17500
 5420  33.420606 192.168.86.16 → 192.168.86.255 DB-LSP-DISC 186 Dropbox LAN sync Discovery Protocol 17500 17500
```

We can inspect the contents of each packet. 
If we check packet #55, we will find the flag:

```console
root@kali:/media/sf_CTFs/pico/Whats_My_Name# tshark -nr myname.pcap -Y 'frame.number==55' -T json
[
  {
    "_index": "packets-2018-11-03",
    "_type": "pcap_file",
    "_score": null,
    "_source": {
      "layers": {
        "frame": {
          "frame.encap_type": "1",
          "frame.time": "Jul  8, 2018 04:55:29.288890000 IDT",
          "frame.offset_shift": "0.000000000",
          "frame.time_epoch": "1531014929.288890000",
          "frame.time_delta": "1417.308003000",
          "frame.time_delta_displayed": "0.000000000",
          "frame.time_relative": "1418.342859000",
          "frame.number": "55",
          "frame.len": "316",
          "frame.cap_len": "316",
          "frame.marked": "0",
          "frame.ignored": "0",
          "frame.protocols": "eth:ethertype:ip:udp:dns"
        },
        "eth": {
          "eth.dst": "0a:00:27:00:00:16",
          "eth.dst_tree": {
            "eth.dst_resolved": "0a:00:27:00:00:16",
            "eth.addr": "0a:00:27:00:00:16",
            "eth.addr_resolved": "0a:00:27:00:00:16",
            "eth.lg": "1",
            "eth.ig": "0"
          },
          "eth.src": "08:00:27:66:da:17",
          "eth.src_tree": {
            "eth.src_resolved": "08:00:27:66:da:17",
            "eth.addr": "08:00:27:66:da:17",
            "eth.addr_resolved": "08:00:27:66:da:17",
            "eth.lg": "0",
            "eth.ig": "0"
          },
          "eth.type": "0x00000800"
        },
        "ip": {
          "ip.version": "4",
          "ip.hdr_len": "20",
          "ip.dsfield": "0x00000000",
          "ip.dsfield_tree": {
            "ip.dsfield.dscp": "0",
            "ip.dsfield.ecn": "0"
          },
          "ip.len": "302",
          "ip.id": "0x00009254",
          "ip.flags": "0x00004000",
          "ip.flags_tree": {
            "ip.flags.rb": "0",
            "ip.flags.df": "1",
            "ip.flags.mf": "0",
            "ip.frag_offset": "0"
          },
          "ip.ttl": "64",
          "ip.proto": "17",
          "ip.checksum": "0x0000220d",
          "ip.checksum.status": "2",
          "ip.src": "192.168.2.12",
          "ip.addr": "192.168.2.12",
          "ip.src_host": "192.168.2.12",
          "ip.host": "192.168.2.12",
          "ip.dst": "192.168.2.1",
          "ip.addr": "192.168.2.1",
          "ip.dst_host": "192.168.2.1",
          "ip.host": "192.168.2.1"
        },
        "udp": {
          "udp.srcport": "53",
          "udp.dstport": "62905",
          "udp.port": "53",
          "udp.port": "62905",
          "udp.length": "282",
          "udp.checksum": "0x000018fb",
          "udp.checksum.status": "2",
          "udp.stream": "0"
        },
        "dns": {
          "dns.id": "0x0000aaa0",
          "dns.flags": "0x000085a0",
          "dns.flags_tree": {
            "dns.flags.response": "1",
            "dns.flags.opcode": "0",
            "dns.flags.authoritative": "1",
            "dns.flags.truncated": "0",
            "dns.flags.recdesired": "1",
            "dns.flags.recavail": "1",
            "dns.flags.z": "0",
            "dns.flags.authenticated": "1",
            "dns.flags.checkdisable": "0",
            "dns.flags.rcode": "0"
          },
          "dns.count.queries": "1",
          "dns.count.answers": "9",
          "dns.count.auth_rr": "0",
          "dns.count.add_rr": "0",
          "Queries": {
            "thisismyname.com: type ANY, class IN": {
              "dns.qry.name": "thisismyname.com",
              "dns.qry.name.len": "16",
              "dns.count.labels": "2",
              "dns.qry.type": "255",
              "dns.qry.class": "0x00000001"
            }
          },
          "Answers": {
            "thisismyname.com: type A, class IN, addr 192.168.2.13": {
              "dns.resp.name": "thisismyname.com",
              "dns.resp.type": "1",
              "dns.resp.class": "0x00000001",
              "dns.resp.ttl": "300",
              "dns.resp.len": "4",
              "dns.a": "192.168.2.13"
            },
            "thisismyname.com: type CNAME, class IN, cname myname.com": {
              "dns.resp.name": "thisismyname.com",
              "dns.resp.type": "5",
              "dns.resp.class": "0x00000001",
              "dns.resp.ttl": "300",
              "dns.resp.len": "9",
              "dns.cname": "myname.com"
            },
            "thisismyname.com: type MX, class IN, preference 5, mx myname.com": {
              "dns.resp.name": "thisismyname.com",
              "dns.resp.type": "15",
              "dns.resp.class": "0x00000001",
              "dns.resp.ttl": "300",
              "dns.resp.len": "4",
              "dns.mx.preference": "5",
              "dns.mx.mail_exchange": "myname.com"
            },
            "thisismyname.com: type MX, class IN, preference 10, mx mx2.myname.com": {
              "dns.resp.name": "thisismyname.com",
              "dns.resp.type": "15",
              "dns.resp.class": "0x00000001",
              "dns.resp.ttl": "300",
              "dns.resp.len": "8",
              "dns.mx.preference": "10",
              "dns.mx.mail_exchange": "mx2.myname.com"
            },
            "thisismyname.com: type MX, class IN, preference 20, mx mx3.myname.com": {
              "dns.resp.name": "thisismyname.com",
              "dns.resp.type": "15",
              "dns.resp.class": "0x00000001",
              "dns.resp.ttl": "300",
              "dns.resp.len": "8",
              "dns.mx.preference": "20",
              "dns.mx.mail_exchange": "mx3.myname.com"
            },
            "thisismyname.com: type NS, class IN, ns ns1.myname.com": {
              "dns.resp.name": "thisismyname.com",
              "dns.resp.type": "2",
              "dns.resp.class": "0x00000001",
              "dns.resp.ttl": "86400",
              "dns.resp.len": "6",
              "dns.ns": "ns1.myname.com"
            },
            "thisismyname.com: type NS, class IN, ns ns2.myname.com": {
              "dns.resp.name": "thisismyname.com",
              "dns.resp.type": "2",
              "dns.resp.class": "0x00000001",
              "dns.resp.ttl": "86400",
              "dns.resp.len": "6",
              "dns.ns": "ns2.myname.com"
            },
            "thisismyname.com: type TXT, class IN": {
              "dns.resp.name": "thisismyname.com",
              "dns.resp.type": "16",
              "dns.resp.class": "0x00000001",
              "dns.resp.ttl": "300",
              "dns.resp.len": "55",
              "dns.txt.length": "54",
              "dns.txt": "picoCTF{w4lt3r_wh1t3_033ad0f914a0b9d213bcc3ce5566038b}"
            },
            "thisismyname.com: type SOA, class IN, mname ns1.thisismyname.com": {
              "dns.resp.name": "thisismyname.com",
              "dns.resp.type": "6",
              "dns.resp.class": "0x00000001",
              "dns.resp.ttl": "86400",
              "dns.resp.len": "32",
              "dns.soa.mname": "ns1.thisismyname.com",
              "dns.soa.rname": "dns.thisismyname.com",
              "dns.soa.serial_number": "1531014718",
              "dns.soa.refresh_interval": "3600",
              "dns.soa.retry_interval": "10800",
              "dns.soa.expire_limit": "86400",
              "dns.soa.mininum_ttl": "3600"
            }
          },
          "dns.response_to": "43",
          "dns.time": "0.001364000"
        }
      }
    }
  }

]
root@kali:/media/sf_CTFs/pico/Whats_My_Name# tshark -nr myname.pcap -Y 'frame.number==55' -T json | grep pico
              "dns.txt": "picoCTF{w4lt3r_wh1t3_033ad0f914a0b9d213bcc3ce5566038b}"
```

The flag is: picoCTF{w4lt3r_wh1t3_033ad0f914a0b9d213bcc3ce5566038b}