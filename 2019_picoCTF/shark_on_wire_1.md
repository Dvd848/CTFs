# shark on wire 1
Forensics, 150 points

## Description:
> We found this packet capture. Recover the flag.


## Solution: 

A network capture was attached.

```console
root@kali:/media/sf_CTFs/pico/shark_on_wire_1# tshark -r capture.pcap | head
Running as user "root" and group "root". This could be dangerous.
    1   0.000000  192.168.2.1 → 239.255.255.250 SSDP 216 M-SEARCH * HTTP/1.1  52688 1900
    2   1.000498  192.168.2.1 → 239.255.255.250 SSDP 216 M-SEARCH * HTTP/1.1  52688 1900
    3   2.001016  192.168.2.1 → 239.255.255.250 SSDP 216 M-SEARCH * HTTP/1.1  52688 1900
    4   3.002419  192.168.2.1 → 239.255.255.250 SSDP 216 M-SEARCH * HTTP/1.1  52688 1900
    5  14.589681  192.168.2.1 → 192.168.2.255 BROWSER 243 Local Master Announcement LAPTOP-HCSFMST7, Workstation, Server, NT Workstation, Potential Browser, Master Browser 138 138
    6  21.094272 fe80::8dd3:64c9:4ef3:82a3 → ff02::1:3    LLMNR 84 Standard query 0x724e A wpad 50462 5355
    7  21.094285  192.168.2.1 → 224.0.0.252  LLMNR 64 Standard query 0x724e A wpad 50462 5355
    8  21.506682 fe80::8dd3:64c9:4ef3:82a3 → ff02::1:3    LLMNR 84 Standard query 0x724e A wpad 50462 5355
    9  21.506697  192.168.2.1 → 224.0.0.252  LLMNR 64 Standard query 0x724e A wpad 50462 5355
   10  35.861119 Vmware_b9:02:a9 → Broadcast    ARP 60 Who has 10.0.0.5? Tell 10.0.0.6
```

The following command searches the UDP streams for the flag format:
```console
root@kali:/media/sf_CTFs/pico/shark_on_wire_1# PCAP=capture.pcap; END=$(tshark -r $PCAP -T fields -e udp.stream | sort -n | tail -1); for ((i=0;i<=END;i++)); do tshark -r $PCAP -Y "udp.stream eq $i" -T fields -e data.text -o data.show_as_text:TRUE 2>/dev/null | tr -d '\n' | grep "picoCTF"; if [ $? -eq 0 ]; then echo "(Stream #$i)"; fi; done
Running as user "root" and group "root". This could be dangerous.
picoCTF{StaT31355_636f6e6e}
(Stream #6)
picoCTF{N0t_a_fLag}
(Stream #7)
```

The flag was found in the sixth stream.