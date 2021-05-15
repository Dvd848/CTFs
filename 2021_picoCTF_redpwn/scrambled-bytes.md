# scrambled-bytes
Forensics, 200 points

## Description

> I sent my secret flag over the wires, but the bytes got all mixed up!

A network capture and the following script were attached:

```python
#!/usr/bin/env python3

import argparse
from progress.bar import IncrementalBar

from scapy.all import *
import ipaddress

import random
from time import time

def check_ip(ip):
  try:
    return ipaddress.ip_address(ip)
  except:
    raise argparse.ArgumentTypeError(f'{ip} is an invalid address')

def check_port(port):
  try:
    port = int(port)
    if port < 1 or port > 65535:
      raise ValueError
    return port
  except:
    raise argparse.ArgumentTypeError(f'{port} is an invalid port')

def main(args):
  with open(args.input, 'rb') as f:
    payload = bytearray(f.read())
  random.seed(int(time()))
  random.shuffle(payload)
  with IncrementalBar('Sending', max=len(payload)) as bar:
    for b in payload:
      send(
        IP(dst=str(args.destination)) /
        UDP(sport=random.randrange(65536), dport=args.port) /
        Raw(load=bytes([b^random.randrange(256)])),
      verbose=False)
      bar.next()

if __name__=='__main__':
  parser = argparse.ArgumentParser()
  parser.add_argument('destination', help='destination IP address', type=check_ip)
  parser.add_argument('port', help='destination port number', type=check_port)
  parser.add_argument('input', help='input file')
  main(parser.parse_args())

```

## Solution

From the script we see that some payload is read from a file, randomly shuffled, and then XORed with a random value and sent with a random source port to some known destination. We obviously need to recover the payload using the network capture.

Let's take a look at the packets in the network capture. We can search for UDP packets with a data length of `1`:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/scrambled-bytes]
└─$ tshark -r ./capture.pcapng -Y "udp && data.len==1" | tail
10926 83.266506563   172.17.0.2 → 172.17.0.3   UDP 43 17207 → 56742 Len=1
10927 83.314105386   172.17.0.2 → 172.17.0.3   UDP 43 51423 → 56742 Len=1
10928 83.362080970   172.17.0.2 → 172.17.0.3   UDP 43 7689 → 56742 Len=1
10929 83.398016166   172.17.0.2 → 172.17.0.3   UDP 43 10149 → 56742 Len=1
10930 83.434074029   172.17.0.2 → 172.17.0.3   UDP 43 43851 → 56742 Len=1
10931 83.470284116   172.17.0.2 → 172.17.0.3   UDP 43 26482 → 56742 Len=1
10932 83.510271119   172.17.0.2 → 172.17.0.3   UDP 43 32839 → 56742 Len=1
10933 83.542154486   172.17.0.2 → 172.17.0.3   UDP 43 27300 → 56742 Len=1
10934 83.542199430   172.17.0.3 → 172.17.0.2   ICMP 71 Destination unreachable (Port unreachable)
10935 83.574211122   172.17.0.2 → 172.17.0.3   UDP 43 15891 → 56742 Len=1
```

This is just a subset of the results, but according to what we see we can be fairly sure that the packets we are looking for are sent from (172.17.0.2, random port) to (172.17.0.3, 56742). We can therefore start programmatically parsing the capture. 

```python
from scapy.all import *
from collections import namedtuple

packets = rdpcap('capture.pcapng')

Packet = namedtuple("Packet", "src_port payload")

payload_packets = []

for packet in packets:
    if not packet.haslayer(IP):
        continue
    if not packet.haslayer(UDP):
        continue
    if not packet[IP].src == "172.17.0.2":
        continue
    if not packet[IP].dst == "172.17.0.3":
        continue
    if not packet[UDP].dport == 56742:
        continue
    if not len(packet[UDP].payload) == 1:
        continue

    payload_packets.append(Packet(packet[UDP].sport, bytes(packet[UDP].payload)))

print(len(payload_packets))
```

Running it, we get:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/scrambled-bytes]
└─$ python3 solve.py
1992
```

But what do we do now? We don't know what the payload was XORed with, so how can we recover the original data?

Fortunately, the script uses Python's pseudo-random number generator, which is predictable if we manage to call `random.seed` with the same seed provided during the original run. And since we see in our script that `random.seed` was called with `int(time())`, we just need to figure out what the time was when the script was executed.

Now, `172.16.0.0/12` (i.e. `172.16.0.0` – `172.31.255.255`) IP addresses are reserved for private IP usage, so we can hope that all the network traffic is captured within a local network, and perhaps we can find some metadata about the capture time to infer the estimated time the script was run.

The first packet sent is:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/scrambled-bytes]
└─$ tshark -r ./capture.pcapng -Y "udp && data.len==1" | head -n 1
 1921 3.514431477   172.17.0.2 → 172.17.0.3   UDP 43 45829 → 56742 Len=1
 ```

Let's get everything we can about this packet:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/scrambled-bytes]
└─$ tshark -r ./capture.pcapng -Y "frame.number == 1921" -V -t e
Frame 1921: 43 bytes on wire (344 bits), 43 bytes captured (344 bits) on interface docker0, id 0
    Interface id: 0 (docker0)
        Interface name: docker0
    Encapsulation type: Ethernet (1)
    Arrival Time: Feb 23, 2021 03:44:10.913789387 IST
    [Time shift for this packet: 0.000000000 seconds]
    Epoch Time: 1614044650.913789387 seconds
    [Time delta from previous captured frame: 0.015624605 seconds]
    [Time delta from previous displayed frame: 0.000000000 seconds]
    [Time since reference or first frame: 3.514431477 seconds]
    Frame Number: 1921
    Frame Length: 43 bytes (344 bits)
    Capture Length: 43 bytes (344 bits)
    [Frame is marked: False]
    [Frame is ignored: False]
    [Protocols in frame: eth:ethertype:ip:udp:data]
Ethernet II, Src: 02:42:ac:11:00:02 (02:42:ac:11:00:02), Dst: 02:42:ac:11:00:03 (02:42:ac:11:00:03)
    Destination: 02:42:ac:11:00:03 (02:42:ac:11:00:03)
        Address: 02:42:ac:11:00:03 (02:42:ac:11:00:03)
        .... ..1. .... .... .... .... = LG bit: Locally administered address (this is NOT the factory default)
        .... ...0 .... .... .... .... = IG bit: Individual address (unicast)
    Source: 02:42:ac:11:00:02 (02:42:ac:11:00:02)
        Address: 02:42:ac:11:00:02 (02:42:ac:11:00:02)
        .... ..1. .... .... .... .... = LG bit: Locally administered address (this is NOT the factory default)
        .... ...0 .... .... .... .... = IG bit: Individual address (unicast)
    Type: IPv4 (0x0800)
Internet Protocol Version 4, Src: 172.17.0.2, Dst: 172.17.0.3
    0100 .... = Version: 4
    .... 0101 = Header Length: 20 bytes (5)
    Differentiated Services Field: 0x00 (DSCP: CS0, ECN: Not-ECT)
        0000 00.. = Differentiated Services Codepoint: Default (0)
        .... ..00 = Explicit Congestion Notification: Not ECN-Capable Transport (0)
    Total Length: 29
    Identification: 0x0001 (1)
    Flags: 0x00
        0... .... = Reserved bit: Not set
        .0.. .... = Don't fragment: Not set
        ..0. .... = More fragments: Not set
    Fragment Offset: 0
    Time to Live: 64
    Protocol: UDP (17)
    Header Checksum: 0x22a8 [validation disabled]
    [Header checksum status: Unverified]
    Source Address: 172.17.0.2
    Destination Address: 172.17.0.3
User Datagram Protocol, Src Port: 45829, Dst Port: 56742
    Source Port: 45829
    Destination Port: 56742
    Length: 9
    Checksum: 0xdc07 [unverified]
    [Checksum Status: Unverified]
    [Stream index: 3]
    [Timestamps]
        [Time since first frame: 0.000000000 seconds]
        [Time since previous frame: 0.000000000 seconds]
    UDP payload (1 byte)
Data (1 byte)

0000  3b                                                ;
    Data: 3b
    [Length: 1]
```

We can see when it was captured:

```
    Arrival Time: Feb 23, 2021 03:44:10.913789387 IST
    [Time shift for this packet: 0.000000000 seconds]
    Epoch Time: 1614044650.913789387 seconds
```

So to check our theory, we can initialize `random.seed` to `1614044650` and see if we are able to arrive to the same source port as seen in the packet: `45829`.

If we summarize the random-related logic from the script, we get:

```python
import random
random.seed(int(time()))
random.shuffle(payload)
sport=random.randrange(65536)
load=bytes([b^random.randrange(256)])
```

So in our experiment, we will try to follow these steps and see what we get:

```python
>>> import random
>>> random.seed(1614044650)
>>> random.shuffle([None]*1992)
>>> sport=random.randrange(65536)
>>> sport
45829
```

We got `45829` as expected. Looks like we're on the right track. Now it's just a matter of reversing the send operation:

```python
random.seed(1614044650)
locations = list(range(len(payload_packets)))
random.shuffle(locations)
output = [None] * len(payload_packets)

for i, packet in enumerate(payload_packets):
    srcport = random.randrange(65536)

    if(srcport != packet.src_port):
        raise RuntimeError(f"Iteration #{i}: srcport ({srcport}) != port ({packet.src_port})")

    output[locations[i]] = ord(packet.payload) ^ random.randrange(256)

with open("output.bin", "wb") as o:
    for b in output:
        o.write(bytes([b]))
```

We start by initializing the random seed to the epoch time we found. We then create a list for tracking purposes: We initialize it with `[0, 1, 2, ... len(payload_packets)]` and shuffle it in the exact same manner that the payload was originally shuffled. This way, we can know where each payload byte ended up at. It's also important to perform the shuffle at this exact location in the flow to bring the `random` state to the same state it was in the original script, before we start reading values for the source port and XOR operation.

Now we iterate the packets we've extracted. We verify that the random number we read from `random` is equal to the source port of the packet as sanity, and then move on by XORing the payload byte with the next random value. We place the result at the correct location based on our `location` list.

Finally, we get:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/scrambled-bytes]
└─$ python3 solve.py

┌──(user@kali)-[/media/sf_CTFs/pico/scrambled-bytes]
└─$ file output.bin
output.bin: PNG image data, 426 x 240, 1-bit grayscale, non-interlaced
```

The PNG image contained the flag: `picoCTF{n0_t1m3_t0_w4st3_5hufflin9_ar0und}`