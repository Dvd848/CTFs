# Shoes
Category: Networking, 80 Points

## Description

> Hi Ash! We have found Team Rocket's secret server. 
> 
> We have a good feeling that there is a hidden message inside it, and you have to find it. In order to help you, we have managed to get a PCAP from someone who had access to the server in the past, we hope you find it useful.
> 
> In addition, we have discovered an email that may be connected. All of the files are safe for download, don't worry.
> 
> Good luck!

A network capture and an email were attached.

## Solution

The attached email describes a conversation between two members of Team Rocket:

```
To:                                               jessie@team-rocket.io
Subject:                                     RE: My protocolll

Hi sis! Make it double haha ;)
I reviewed your work, love how you didn't reinvent the wheel in your implementation, smart!
Regarding the checksum, Meowth suggested for using a standard error detection mechanism, you can use a similar algorithm of Ethernet's checksum, but keep it simple...
I hope it will keep Ash outside of our servers!

James,
Team Rocket inc.

From: Jessie <jessie@team-rocket.io>
Sent: Wednesday, January 8, 2020 11:35 AM
To: james@team-rocket.io
Subject: My protocolll

Sup Jessie! Prepare for trouble... just kidding.
I'm trying to implement our server communication protocol, do ya think we need some kind of checksum?

Jessie,
Team Rocket inc.
```

The attached network capture includes the following protocols:
```console
root@kali:/media/sf_CTFs/checkpoint/Shoes# tshark -nr capture.pcapng -qz io,phs
Running as user "root" and group "root". This could be dangerous.

===================================================================
Protocol Hierarchy Statistics
Filter:

eth                                      frames:111 bytes:11496
  ip                                     frames:108 bytes:11324
    tcp                                  frames:78 bytes:7682
      http                               frames:8 bytes:2072
      socks                              frames:14 bytes:2242
    icmp                                 frames:14 bytes:1372
    udp                                  frames:16 bytes:2270
      dns                                frames:16 bytes:2270
  arp                                    frames:2 bytes:102
  ipv6                                   frames:1 bytes:70
    icmpv6                               frames:1 bytes:70
===================================================================
```

The "socks" messages seems to contain an interesting payload, and seem related to the challenge name.

Let's take a look at the payload:


```console
root@kali:/media/sf_CTFs/checkpoint/Shoes# tshark -nr capture.pcapng -Y 'socks' -T fields -e frame.number -e ip.src -e ip.dst -e tcp.payload
Running as user "root" and group "root". This could be dangerous.
67      10.0.2.15       52.28.255.56    5a:01:fe:dd:74:9c:2e
69      52.28.255.56    10.0.2.15       5a:fe:2c:91:60:5e:14:c8:b1:1b
71      10.0.2.15       52.28.255.56    5a:2c:d2:33:1f:a9:0b:b9:6a
73      10.0.2.15       52.28.255.56    5a:01:00:01:c0:a8:ad:0a:00:50:74:f2:be:19
75      52.28.255.56    10.0.2.15       5a:00:00:01:00:00:00:00:00:00:eb:cb:75:43
77      10.0.2.15       52.28.255.56    47:45:54:20:2f:20:48:54:54:50:2f:31:2e:31:0d:0a:55:73:65:72:2d:41:67:65:6e:74:3a:20:4d:6f:7a:69:6c:6c:61:2f:35:2e:30:20:28:57:69:6e:64:6f:77:73:20:4e:54:20:31:30:2e:30:3b:20:57:69:6e:36:34:3b:20:78:36:34:29:20:41:70:70:6c:65:57:65:62:4b:69:74:2f:35:33:37:2e:33:36:20:28:4b:48:54:4d:4c:2c:20:6c:69:6b:65:20:47:65:63:6b:6f:29:20:43:68:72:6f:6d:65:2f:37:34:2e:30:2e:33:37:32:39:2e:31:36:39:20:53:61:66:61:72:69:2f:35:33:37:2e:33:36:0d:0a:48:6f:73:74:3a:20:77:77:77:2e:74:75:74:6f:72:69:61:6c:73:70:6f:69:6e:74:2e:63:6f:6d:0d:0a:41:63:63:65:70:74:2d:4c:61:6e:67:75:61:67:65:3a:20:65:6e:2d:75:73:0d:0a:43:6f:6e:6e:65:63:74:69:6f:6e:3a:20:4b:65:65:70:2d:41:6c:69:76:65:0d:0a:0d:0a
79      52.28.255.56    10.0.2.15       48:54:54:50:2f:31:2e:31:20:32:30:30:20:4f:4b:0d:0a:44:61:74:65:3a:20:57:65:64:2c:20:32:32:20:41:70:72:20:32:30:32:30:20:31:31:3a:32:31:3a:35:30:20:47:4d:54:0d:0a:53:65:72:76:65:72:3a:20:41:70:61:63:68:65:2f:32:2e:34:2e:32:39:20:28:55:62:75:6e:74:75:29:0d:0a:4c:61:73:74:2d:4d:6f:64:69:66:69:65:64:3a:20:57:65:64:2c:20:32:32:20:41:70:72:20:32:30:32:30:20:31:30:3a:34:39:3a:35:36:20:47:4d:54:0d:0a:45:54:61:67:3a:20:22:39:64:2d:35:61:33:64:65:65:34:39:31:64:61:37:32:22:0d:0a:41:63:63:65:70:74:2d:52:61:6e:67:65:73:3a:20:62:79:74:65:73:0d:0a:43:6f:6e:74:65:6e:74:2d:4c:65:6e:67:74:68:3a:20:31:35:37:0d:0a:56:61:72:79:3a:20:41:63:63:65:70:74:2d:45:6e:63:6f:64:69:6e:67:0d:0a:4b:65:65:70:2d:41:6c:69:76:65:3a:20:74:69:6d:65:6f:75:74:3d:35:2c:20:6d:61:78:3d:31:30:30:0d:0a:43:6f:6e:6e:65:63:74:69:6f:6e:3a:20:4b:65:65:70:2d:41:6c:69:76:65:0d:0a:43:6f:6e:74:65:6e:74:2d:54:79:70:65:3a:20:74:65:78:74:2f:68:74:6d:6c:0d:0a:0d:0a:46:69:6c:65:73:20:69:6e:20:73:65:72:76:65:72:3a:0a:0a:69:6e:64:65:78:2e:68:74:6d:6c:0a:41:72:62:6f:6b:2e:6a:70:67:0a:42:6c:61:73:74:6f:69:73:65:2e:6a:70:67:0a:42:75:6c:62:61:73:61:75:72:2e:6a:70:67:0a:42:75:74:74:65:72:66:72:65:65:2e:6a:70:67:0a:43:68:61:72:69:7a:61:72:64:2e:6a:70:67:0a:46:6c:61:67:2e:6a:70:67:0a:4d:65:74:61:70:6f:64:2e:6a:70:67:0a:52:61:74:74:61:74:61:2e:6a:70:67:0a:53:71:75:69:72:74:6c:65:2e:6a:70:67:0a:57:61:72:74:6f:72:74:6c:65:2e:6a:70:67:0a:0a
91      10.0.2.15       52.28.255.56    5a:01:fe:dd:74:9c:2e
93      52.28.255.56    10.0.2.15       5a:fe:67:a6:f1:93:f4:76:98:64
95      10.0.2.15       52.28.255.56    5a:67:e5:a2:d2:49:b5:90:15
97      10.0.2.15       52.28.255.56    5a:01:00:01:c0:a8:ad:0a:00:50:74:f2:be:19
99      52.28.255.56    10.0.2.15       5a:00:00:01:00:00:00:00:00:00:eb:cb:75:43
101     10.0.2.15       52.28.255.56    47:45:54:20:2f:20:48:54:54:50:2f:31:2e:31:0d:0a:55:73:65:72:2d:41:67:65:6e:74:3a:20:4d:6f:7a:69:6c:6c:61:2f:35:2e:30:20:28:57:69:6e:64:6f:77:73:20:4e:54:20:31:30:2e:30:3b:20:57:69:6e:36:34:3b:20:78:36:34:29:20:41:70:70:6c:65:57:65:62:4b:69:74:2f:35:33:37:2e:33:36:20:28:4b:48:54:4d:4c:2c:20:6c:69:6b:65:20:47:65:63:6b:6f:29:20:43:68:72:6f:6d:65:2f:37:34:2e:30:2e:33:37:32:39:2e:31:36:39:20:53:61:66:61:72:69:2f:35:33:37:2e:33:36:0d:0a:48:6f:73:74:3a:20:77:77:77:2e:74:75:74:6f:72:69:61:6c:73:70:6f:69:6e:74:2e:63:6f:6d:0d:0a:41:63:63:65:70:74:2d:4c:61:6e:67:75:61:67:65:3a:20:65:6e:2d:75:73:0d:0a:43:6f:6e:6e:65:63:74:69:6f:6e:3a:20:4b:65:65:70:2d:41:6c:69:76:65:0d:0a:0d:0a
103     52.28.255.56    10.0.2.15       48:54:54:50:2f:31:2e:31:20:32:30:30:20:4f:4b:0d:0a:44:61:74:65:3a:20:57:65:64:2c:20:32:32:20:41:70:72:20:32:30:32:30:20:31:31:3a:32:31:3a:35:37:20:47:4d:54:0d:0a:53:65:72:76:65:72:3a:20:41:70:61:63:68:65:2f:32:2e:34:2e:32:39:20:28:55:62:75:6e:74:75:29:0d:0a:4c:61:73:74:2d:4d:6f:64:69:66:69:65:64:3a:20:57:65:64:2c:20:32:32:20:41:70:72:20:32:30:32:30:20:31:30:3a:34:39:3a:35:36:20:47:4d:54:0d:0a:45:54:61:67:3a:20:22:39:64:2d:35:61:33:64:65:65:34:39:31:64:61:37:32:22:0d:0a:41:63:63:65:70:74:2d:52:61:6e:67:65:73:3a:20:62:79:74:65:73:0d:0a:43:6f:6e:74:65:6e:74:2d:4c:65:6e:67:74:68:3a:20:31:35:37:0d:0a:56:61:72:79:3a:20:41:63:63:65:70:74:2d:45:6e:63:6f:64:69:6e:67:0d:0a:4b:65:65:70:2d:41:6c:69:76:65:3a:20:74:69:6d:65:6f:75:74:3d:35:2c:20:6d:61:78:3d:31:30:30:0d:0a:43:6f:6e:6e:65:63:74:69:6f:6e:3a:20:4b:65:65:70:2d:41:6c:69:76:65:0d:0a:43:6f:6e:74:65:6e:74:2d:54:79:70:65:3a:20:74:65:78:74:2f:68:74:6d:6c:0d:0a:0d:0a:46:69:6c:65:73:20:69:6e:20:73:65:72:76:65:72:3a:0a:0a:69:6e:64:65:78:2e:68:74:6d:6c:0a:41:72:62:6f:6b:2e:6a:70:67:0a:42:6c:61:73:74:6f:69:73:65:2e:6a:70:67:0a:42:75:6c:62:61:73:61:75:72:2e:6a:70:67:0a:42:75:74:74:65:72:66:72:65:65:2e:6a:70:67:0a:43:68:61:72:69:7a:61:72:64:2e:6a:70:67:0a:46:6c:61:67:2e:6a:70:67:0a:4d:65:74:61:70:6f:64:2e:6a:70:67:0a:52:61:74:74:61:74:61:2e:6a:70:67:0a:53:71:75:69:72:74:6c:65:2e:6a:70:67:0a:57:61:72:74:6f:72:74:6c:65:2e:6a:70:67:0a:0a
```

It looks like we have two sessions: 67-79, and 91-103. The payload in both sessions is similar but not identical.
We'll ignore the long packets for now (77, 79, 101, 103) and try to analyze the shorter ones.

The communication is done over the SOCKS port (1080), but the protocol itself doesn't match the SOCKS spec. 

The mail mentioned a checksum, so maybe we should start by locating it. It should be standard according to the mail. The [Ethernet Checksum](https://en.wikipedia.org/wiki/Frame_check_sequence) is CRC32 (4 bytes long), so let's try that:

```console
root@kali:/media/sf_CTFs/checkpoint/Shoes# crc32 <(printf "\x5a\x01\xfe")
dd749c2e
root@kali:/media/sf_CTFs/checkpoint/Shoes# crc32 <(printf "\x5a\xfe\x2c\x91\x60\x5e")
14c8b11b
root@kali:/media/sf_CTFs/checkpoint/Shoes# crc32 <(printf "\x5a\x2c\xd2\x33\x1f")
a90bb96a
```

Looks like we have a match and the last four bytes are CRC32. 

Now, if we clean up the messages a bit, we have:
```
67      10.0.2.15       52.28.255.56    5a:01:fe
69      52.28.255.56    10.0.2.15       5a:fe:2c:91:60:5e
71      10.0.2.15       52.28.255.56    5a:2c:d2:33:1f
73      10.0.2.15       52.28.255.56    5a:01:00:01:c0:a8:ad:0a:00:50
75      52.28.255.56    10.0.2.15       5a:00:00:01:00:00:00:00:00:00
... <long messages> ...
```

Now, the mail said: *love how you didn't reinvent the wheel in your implementation*. Maybe this is similar to [SOCKS](https://en.wikipedia.org/wiki/SOCKS)?

> * Client connects and sends a greeting, which includes a list of authentication methods supported.
> * Server chooses one of the methods (or sends a failure response if none of them are acceptable).
> * Several messages may now pass between the client and the server, depending on the authentication method chosen.
> * Client sends a connection request similar to SOCKS4.
> * Server responds similar to SOCKS4.

In SOCKS, the client greeting consists of:
|   |	VER | NAUTH |	AUTH |
|---|---|---|---|
|Byte count |	1 |	1 |	variable |

* SOCKS version
* Number of authentication methods supported
* Authentication methods, 1 byte per method supported (0x80–0xFE: methods reserved for private use)

All the messages start with `0x5a`, so that might be a protocol version.

Then we have `0x1` which fits the *number of authentication methods supported*, followed by `0xfe` which can be an authentication method.

So, we have:
|   |	VER | NAUTH |	AUTH |
|---|---|---|---|
|Contents |	0x5a |	1 |	0xfa |

This is good, since the next message generally matches the SOCKS server response:

|   |	VER | CAUTH |
|---|---|---|
|Byte count |	1 |	1 |

* SOCKS version
* Chosen authentication method, or 0xFF if no acceptable methods were offered 
* The subsequent authentication is method-dependent

The message starts with `0x5a`, then the chosen authentication method (`0xfe`) and the subsequent authentication scheme (`2c:91:60:5e`). 

|   |	VER | CAUTH |
|---|---|---|
|Contents |	0x5a |	0xfa |

Let's try to crack the authentication scheme using the information we have from both sessions.

```
Session #1:
Receive from server: 2c:91:60:5e
Send to server:      2c:d2:33:1f

Session #2: 
Receive from server:  67:a6:f1:93
Send to server:       67:e5:a2:d2
```

The most trivial authentication scheme would be using a simple XOR key. Let's try to XOR the challenge and response to see if we can get a key.

```python
>>> def str_to_bytes(s):
...     return bytes.fromhex(s.replace(":", ""))
...
>>> def xor_str(a, b):
...     return bytes((x ^ y for x, y in zip(str_to_bytes(a), str_to_bytes(b))))
...
>>> xor_str("2c:91:60:5e", "2c:d2:33:1f")
b'\x00CSA'
>>> xor_str("67:a6:f1:93", "67:e5:a2:d2")
b'\x00CSA'
```

Both cases return the same key so we must have a match!

The next client message is similar to a SOCKS "Client connection request":

| |	VER |	CMD |	RSV |	DSTADDR |	DSTPORT |
|---|---|---|---|---|---|
|Byte Count |	1 |	1 |	1 |	Variable |	2 |

* SOCKS version
* Command Code (0x01: establish a TCP/IP stream connection)
* Reserved, must be 0x00
* Destination address
  * Type of the address (0x01: IPv4 address)
  * The address (4 bytes for IPv4 address)
* Port number, 2 bytes

This fits our message, making this a request for Address c0:a8:ad:0a, port 0x50, a.k.a. 192.168.173.10:80.

| |	VER |	CMD |	RSV |	DSTADDR |	DSTPORT |
|---|---|---|---|---|---|
|Contents |	0x5a |	1 |	0 |	0x1, [0xc0, 0xa8 0xad 0x0a] |	0x50 |

The response in the SOCKS protocol is:

| | 	VER |	STATUS |	RSV |	BNDADDR |	BNDPORT |
|---|---|---|---|---|---|
|Byte Count |	1 |	1 |	1 |	variable |	2 |

* SOCKS version
* Status code (0x00: request granted)
* Reserved, must be 0x00
* Server bound address
* Server bound port number

In this case we have a partial match for the response, since the bound address and port in the response we've received are 0 (perhaps that's ok?).

| | 	VER |	STATUS |	RSV |	BNDADDR |	BNDPORT |
|---|---|---|---|---|---|
|Contents |	0x5a |	0 |	0 |	0 |	0 |

Now to the long packets. If we decode them as ASCII we get an HTTP request and response:

```console
root@kali:/media/sf_CTFs/checkpoint/Shoes# tshark -nr capture.pcapng -Y "frame.number == 77" -T fields -e tcp.payload 2>/dev/null | xxd -r -p
GET / HTTP/1.1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36
Host: www.tutorialspoint.com
Accept-Language: en-us
Connection: Keep-Alive
root@kali:/media/sf_CTFs/checkpoint/Shoes# tshark -nr capture.pcapng -Y "frame.number == 79" -T fields -e tcp.payload 2>/dev/null | xxd -r -p
HTTP/1.1 200 OK
Date: Wed, 22 Apr 2020 11:21:50 GMT
Server: Apache/2.4.29 (Ubuntu)
Last-Modified: Wed, 22 Apr 2020 10:49:56 GMT
ETag: "9d-5a3dee491da72"
Accept-Ranges: bytes
Content-Length: 157
Vary: Accept-Encoding
Keep-Alive: timeout=5, max=100
Connection: Keep-Alive
Content-Type: text/html

Files in server:

index.html
Arbok.jpg
Blastoise.jpg
Bulbasaur.jpg
Butterfree.jpg
Charizard.jpg
Flag.jpg
Metapod.jpg
Rattata.jpg
Squirtle.jpg
Wartortle.jpg
```

The client is sending an HTTP GET request for "`www.tutorialspoint.com/`" and receiving a list of files, including `Flag.jpg`. We probably want to download that file ourselves.

Let's try to replicate this behavior.

We start from implementing classes for the different SHOES messages we've reversed earlier:
```python
# shoes.py

from ctypes import *
import binascii, ipaddress

SHOES_PROTOCOL_VERSION = 0x5A
SHOES_AUTH_TYPE_XOR = 0xFE
SHOES_ADDRESS_TYPE_IPV4 = 0x1
SHOES_COMMAND_ESTABLISH_TCP_IP = 0x1
SHOES_STATUS_SUCCESS = 0x0

class ShoesStruct(BigEndianStructure):
    _pack_ = 1
    CHECKSUM_LENGTH = 4

    def to_bytes(self, include_checksum = True):
        res = bytes(self)
        if include_checksum:
            res += self.calculate_checksum(res)
        return res

    @classmethod
    def from_bytes(cls, buf, callback = None):
        buf_without_checksum, checksum = cls.split_checksum(buf)
        obj = cls.from_buffer_copy(buf_without_checksum)
        initial_len = len(obj.to_bytes(include_checksum = False))

        if callback:
            obj = callback(obj, buf_without_checksum, initial_len)

        calc_checksum = cls.calculate_checksum(obj.to_bytes(include_checksum = False))

        if (checksum != calc_checksum):
            raise ValueError("Calculated checksum (0x{}) different from provided checksum (0x{})"
                                .format(calc_checksum.hex(), checksum.hex()))

        return obj


    @classmethod
    def split_checksum(cls, buf):
        if len(buf) < cls.CHECKSUM_LENGTH:
            raise ValueError("Provided buffer too short to contain checksum")
        buf_without_checksum = buf[:-1 * cls.CHECKSUM_LENGTH]
        checksum = buf[-1 * cls.CHECKSUM_LENGTH:]

        return (buf_without_checksum, checksum)

    @classmethod
    def calculate_checksum(cls, buf):
        return binascii.crc32(buf).to_bytes(cls.CHECKSUM_LENGTH, byteorder='big')



class ClientGreeting(ShoesStruct):
    _fields_ = [('version', c_uint8),
                ('num_auth', c_uint8)]
    _auth_list = (c_ubyte * 0)()

    def __init__(self, version = None, auth_list = None):
        super().__init__()
        if version is not None:
            self.version = version
        if auth_list is not None:
            self.auth_list = auth_list

    @property
    def auth_list(self):
        return list(bytes(self._auth_list))

    @auth_list.setter
    def auth_list(self, auth_list):
        self.num_auth = len(auth_list)
        self._auth_list = (self._auth_list._type_ * len(auth_list))()
        al = bytes(auth_list)
        assert(len(al) == len(auth_list))
        memmove(self._auth_list, al, len(auth_list))

    @classmethod
    def from_bytes(cls, buf):
        try:
            def handle_auth_list_callback(obj, buf_without_checksum, initial_len):
                if len(buf_without_checksum) != initial_len:
                    obj.auth_list = buf_without_checksum[initial_len:]
                if (len(obj._auth_list) != obj.num_auth):
                    raise ValueError("Declared number of authentication methods does not match provided number")
                return obj

            obj = super().from_bytes(buf, handle_auth_list_callback)
            return obj
        except Exception:
            raise

    def to_bytes(self, include_checksum = True):
        res = super().to_bytes(include_checksum = False) + memoryview(self._auth_list)
        if include_checksum:
            res += self.calculate_checksum(res)
        return res

from abc import ABC, abstractmethod, ABCMeta
class ShoesAuthBase(ShoesStruct):

    @property
    def auth_data(self):
        return list(bytes(self._auth_data))

    @auth_data.setter
    def auth_data(self, auth_data):
        self.num_auth = len(auth_data)
        self._auth_data = (self._auth_data._type_ * len(auth_data))()
        al = bytes(auth_data)
        assert(len(al) == len(auth_data))
        memmove(self._auth_data, al, len(auth_data))

    @classmethod
    def from_bytes(cls, buf):
        try:
            def handle_auth_data_callback(self, buf_without_checksum, initial_len):
                if len(buf_without_checksum) != initial_len:
                    self.auth_data = buf_without_checksum[initial_len:]
                return self

            self = super().from_bytes(buf, handle_auth_data_callback)
            return self
        except Exception:
            raise

    def to_bytes(self, include_checksum = True):
        res = super().to_bytes(include_checksum = False) + memoryview(self._auth_data)
        if include_checksum:
            res += self.calculate_checksum(res)
        return res

class ServerChoice(ShoesAuthBase):
    _fields_ = [('version', c_uint8),
                ('chosen_auth', c_uint8)]
    _auth_data = (c_ubyte * 0)()

    def __init__(self, version = None, chosen_auth = None, auth_data = None):
        super().__init__()
        if version is not None:
            self.version = version
        if chosen_auth is not None:
            self.chosen_auth = chosen_auth
        if auth_data is not None:
            self.auth_data = auth_data



class ClientAuth(ShoesAuthBase):
    _fields_ = [('version', c_uint8)]
    _auth_data = (c_ubyte * 0)()

    def __init__(self, version = None, auth_data = None):
        super().__init__()
        if version is not None:
            self.version = version
        if auth_data is not None:
            self.auth_data = auth_data


class ClientConnectionRequest(ShoesStruct):
    _fields_ = [('version', c_uint8),
                ('command', c_uint8),
                ('reserved', c_uint8),
                ('address_type', c_uint8),
                ('address', c_uint32),
                ('port', c_uint16)]
    
    def __init__(self, version = None, command = None, address = None, port = None):
        super().__init__()
        if version is not None:
            self.version = version
        if command is not None:
            self.command = command
        if address is not None:
            self.address = address
            self.address_type = SHOES_ADDRESS_TYPE_IPV4
        if port is not None:
            self.port = port
        self.reserved = 0
        
class ServerConnectionResponse(ShoesStruct):
    _fields_ = [('version', c_uint8),
                ('status', c_uint8),
                ('reserved', c_uint8),
                ('address_type', c_uint8),
                ('address', c_uint32),
                ('port', c_uint16)]
    
    def __init__(self, version = None, status = None, address = None, port = None):
        super().__init__()
        if version is not None:
            self.version = version
        if status is not None:
            self.status = status
        if address is not None:
            self.address = address
            self.address_type = SHOES_ADDRESS_TYPE_IPV4
        if port is not None:
            self.port = port
        self.reserved = 0
    
# Resources for implementing variable-length ctypes structures:
#  https://wumb0.in/a-better-way-to-work-with-raw-data-types-in-python.html
#  https://stackoverflow.com/questions/7015487/ctypes-variable-length-structures
#  http://dabeaz.blogspot.com/2009/08/python-binary-io-handling.html
#  https://stackoverflow.com/questions/8392203/dynamic-arrays-and-structures-in-structures-in-python
#  https://docs.python.org/2.5/lib/ctypes-variable-sized-data-types.html

def test():
    cg1 = ClientGreeting()
    cg1.version = SHOES_PROTOCOL_VERSION
    cg1.auth_list = [SHOES_AUTH_TYPE_XOR]
    assert(cg1.to_bytes().hex() == "5a01fedd749c2e")

    cg2 = ClientGreeting.from_bytes(cg1.to_bytes())
    assert(cg2.version == SHOES_PROTOCOL_VERSION)
    assert(cg2.num_auth == 1)
    assert(cg2.auth_list == [SHOES_AUTH_TYPE_XOR])

    cg3 = ClientGreeting(version = SHOES_PROTOCOL_VERSION, auth_list = [SHOES_AUTH_TYPE_XOR])
    assert(cg3.to_bytes().hex() == cg1.to_bytes().hex())

    sc1 = ServerChoice.from_bytes(bytes.fromhex("5afe2c91605e14c8b11b"))
    assert(bytes(sc1.auth_data).hex() == "2c91605e")

    ca1 = ClientAuth(version = SHOES_PROTOCOL_VERSION, auth_data = b'\x2c\xd2\x33\x1f')
    assert(ca1.to_bytes().hex() == "5a2cd2331fa90bb96a")

    ccr1 = ClientConnectionRequest.from_bytes(bytes.fromhex("5a010001c0a8ad0a005074f2be19"))
    assert(ccr1.version == SHOES_PROTOCOL_VERSION)
    assert(ccr1.command == SHOES_COMMAND_ESTABLISH_TCP_IP)
    assert(ccr1.reserved == 0)
    assert(ccr1.address_type == SHOES_ADDRESS_TYPE_IPV4)
    assert(str(ipaddress.IPv4Address(ccr1.address)) == "192.168.173.10")
    assert(ccr1.port == 80)

    ccr2 = ClientConnectionRequest(version = SHOES_PROTOCOL_VERSION, command = SHOES_COMMAND_ESTABLISH_TCP_IP,
                                         address = int(ipaddress.IPv4Address("192.168.173.10")), port = 80)
    assert(ccr2.to_bytes().hex() == "5a010001c0a8ad0a005074f2be19")

    scr1 = ServerConnectionResponse.from_bytes(bytes.fromhex("5a000001000000000000ebcb7543"))
    assert(scr1.version == SHOES_PROTOCOL_VERSION)
    assert(scr1.status == SHOES_STATUS_SUCCESS)
    assert(scr1.reserved == 0)
    assert(scr1.address_type == SHOES_ADDRESS_TYPE_IPV4)
    assert(str(ipaddress.IPv4Address(scr1.address)) == "0.0.0.0")
    assert(scr1.port == 0)

if __name__ == "__main__":
    test()
```

These classes allow serializing and deserializing the SHOES messages, as can be seen in the `test()` function.

We'll use the following context manager as a wrapper for the TCP connection, for easier debug:
```python
import socket

class RemoteServer:
    BUFFER_SIZE = 4096

    def __init__(self, ip, port, verbose = False):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.ip = ip
        self.port = port
        self.verbose = verbose

    def __enter__(self):
        self.s.connect((self.ip, self.port))
        return self

    def __exit__(self, *args):
        self.s.close()

    def _print(self, prefix, msg):
        if self.verbose:
            print("{}{}".format(prefix, " ".join(["{:02x}".format(x) for x in msg])))

    def send(self, msg):
        self._print("Sending : ", msg)
        self.s.send(msg)

    def recv(self):
        msg = self.s.recv(self.BUFFER_SIZE)
        self._print("Received: ", msg)
        return msg
```

Now, we start by attempting to setup the connection with the SHOES server:
```python
import socket, os, re
import shoes

TCP_IP = '52.28.255.56'
TCP_PORT = 1080

KEY = bytes.fromhex("00 43 53 41") # \x00CSA

def xor(one, two):
    return bytes(a ^ b for (a, b) in zip(one, two))

def setup_connection(ip):
    s.send(shoes.ClientGreeting(version = shoes.SHOES_PROTOCOL_VERSION, auth_list = [shoes.SHOES_AUTH_TYPE_XOR]).to_bytes())

    challenge_msg =  shoes.ServerChoice.from_bytes(s.recv())
    
    s.send(shoes.ClientAuth(version = shoes.SHOES_PROTOCOL_VERSION, auth_data = xor(challenge_msg.auth_data, KEY)).to_bytes())
    
    s.send(shoes.ClientConnectionRequest(version = shoes.SHOES_PROTOCOL_VERSION, command = shoes.SHOES_COMMAND_ESTABLISH_TCP_IP,
                                         address = shoes.ipaddress.IPv4Address(ip), port = 80).to_bytes())

    server_response = shoes.ServerConnectionResponse.from_bytes(s.recv())

    if server_response.status != shoes.SHOES_STATUS_SUCCESS:
        raise Exception("Error setting up connection, status = {}".format(server_response.status))
    assert(server_response.status == shoes.SHOES_STATUS_SUCCESS)

with RemoteServer(TCP_IP, TCP_PORT, verbose=True) as s:    
    setup_connection("192.168.173.10")
```

Output:
```console
root@kali:/media/sf_CTFs/checkpoint/Shoes# python3 solve.py
Sending : 5a 01 fe dd 74 9c 2e
Received: 5a fe a0 ca c5 a5 2a b1 1a f1
Sending : 5a a0 89 96 e4 97 72 12 80
Sending : 5a 01 00 01 c0 a8 ad 0a 00 50 74 f2 be 19
Received: 5a 00 00 01 00 00 00 00 00 00 eb cb 75 43
```

This looks good and similar to the session from the network capture. We can continue by attempting to make an HTTP request:

```python
HTTP_REQUEST = 'GET /{} HTTP/1.1\r\n' \
                'User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36\r\n' \
                'Host: www.tutorialspoint.com\r\n' \
                'Accept-Language: en-us\r\n' \
                'Connection: Keep-Alive\r\n\r\n'

def get_page(s, page):
    s.send(HTTP_REQUEST.format(page).encode("ascii"))
    data = s.recv()
    return data.decode("ascii")

with RemoteServer(TCP_IP, TCP_PORT, verbose=True) as s:    
    setup_connection("192.168.173.10")

    print(get_page(s, "/"))
```

Output:
```console
root@kali:/media/sf_CTFs/checkpoint/Shoes# python3 solve.py
Sending : 5a 01 fe dd 74 9c 2e
Received: 5a fe a0 ca c5 a5 2a b1 1a f1
Sending : 5a a0 89 96 e4 97 72 12 80
Sending : 5a 01 00 01 c0 a8 ad 0a 00 50 74 f2 be 19
Received: 5a 00 00 01 00 00 00 00 00 00 eb cb 75 43
Sending : 47 45 54 20 2f 2f 20 48 54 54 50 2f 31 2e 31 0d 0a 55 73 65 72 2d 41 67 65 6e 74 3a 20 4d 6f 7a 69 6c 6c 61 2f 35 2e 30 20 28 57 69 6e 64 6f 77 73 20 4e 54 20 31 30 2e 30 3b 20 57 69 6e 36 34 3b 20 78 36 34 29 20 41 70 70 6c 65 57 65 62 4b 69 74 2f 35 33 37 2e 33 36 20 28 4b 48 54 4d 4c 2c 20 6c 69 6b 65 20 47 65 63 6b 6f 29 20 43 68 72 6f 6d 65 2f 37 34 2e 30 2e 33 37 32 39 2e 31 36 39 20 53 61 66 61 72 69 2f 35 33 37 2e 33 36 0d 0a 48 6f 73 74 3a 20 77 77 77 2e 74 75 74 6f 72 69 61 6c 73 70 6f 69 6e 74 2e 63 6f 6d 0d 0a 41 63 63 65 70 74 2d 4c 61 6e 67 75 61 67 65 3a 20 65 6e 2d 75 73 0d 0a 43 6f 6e 6e 65 63 74 69 6f 6e 3a 20 4b 65 65 70 2d 41 6c 69 76 65 0d 0a 0d 0a
Received: 48 54 54 50 2f 31 2e 31 20 33 30 31 20 4d 6f 76 65 64 20 50 65 72 6d 61 6e 65 6e 74 6c 79 0d 0a 44 61 74 65 3a 20 46 72 69 2c 20 32 39 20 4d 61 79 20 32 30 32 30 20 31 39 3a 30 33 3a 32 39 20 47 4d 54 0d 0a 53 65 72 76 65 72 3a 20 41 70 61 63 68 65 2f 32 2e 34 2e 32 39 20 28 55 62 75 6e 74 75 29 0d 0a 4c 6f 63 61 74 69 6f 6e 3a 20 68 74 74 70 3a 2f 2f 31 39 32 2e 31 36 38 2e 31 37 33 2e 32 30 0d 0a 43 6f 6e 74 65 6e 74 2d 4c 65 6e 67 74 68 3a 20 33 31 37 0d 0a 4b 65 65 70 2d 41 6c 69 76 65 3a 20 74 69 6d 65 6f 75 74 3d 35 2c 20 6d 61 78 3d 31 30 30 0d 0a 43 6f 6e 6e 65 63 74 69 6f 6e 3a 20 4b 65 65 70 2d 41 6c 69 76 65 0d 0a 43 6f 6e 74 65 6e 74 2d 54 79 70 65 3a 20 74 65 78 74 2f 68 74 6d 6c 3b 20 63 68 61 72 73 65 74 3d 69 73 6f 2d 38 38 35 39 2d 31 0d 0a 0d 0a 3c 21 44 4f 43 54 59 50 45 20 48 54 4d 4c 20 50 55 42 4c 49 43 20 22 2d 2f 2f 49 45 54 46 2f 2f 44 54 44 20 48 54 4d 4c 20 32 2e 30 2f 2f 45 4e 22 3e 0a 3c 68 74 6d 6c 3e 3c 68 65 61 64 3e 0a 3c 74 69 74 6c 65 3e 33 30 31 20 4d 6f 76 65 64 20 50 65 72 6d 61 6e 65 6e 74 6c 79 3c 2f 74 69 74 6c 65 3e 0a 3c 2f 68 65 61 64 3e 3c 62 6f 64 79 3e 0a 3c 68 31 3e 4d 6f 76 65 64 20 50 65 72 6d 61 6e 65 6e 74 6c 79 3c 2f 68 31 3e 0a 3c 70 3e 54 68 65 20 64 6f 63 75 6d 65 6e 74 20 68 61 73 20 6d 6f 76 65 64 20 3c 61 20 68 72 65 66 3d 22 68 74 74 70 3a 2f 2f 31 39 32 2e 31 36 38 2e 31 37 33 2e 32 30 22 3e 68 65 72 65 3c 2f 61 3e 2e 3c 2f 70 3e 0a 3c 68 72 3e 0a 3c 61 64 64 72 65 73 73 3e 41 70 61 63 68 65 2f 32 2e 34 2e 32 39 20 28 55 62 75 6e 74 75 29 20 53 65 72 76 65 72 20 61 74 20 77 77 77 2e 74 75 74 6f 72 69 61 6c 73 70 6f 69 6e 74 2e 63 6f 6d 20 50 6f 72 74 20 38 30 3c 2f 61 64 64 72 65 73 73 3e 0a 3c 2f 62 6f 64 79 3e 3c 2f 68 74 6d 6c 3e 0a
HTTP/1.1 301 Moved Permanently
Date: Fri, 29 May 2020 19:03:29 GMT
Server: Apache/2.4.29 (Ubuntu)
Location: http://192.168.173.20
Content-Length: 317
Keep-Alive: timeout=5, max=100
Connection: Keep-Alive
Content-Type: text/html; charset=iso-8859-1

<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
<html><head>
<title>301 Moved Permanently</title>
</head><body>
<h1>Moved Permanently</h1>
<p>The document has moved <a href="http://192.168.173.20">here</a>.</p>
<hr>
<address>Apache/2.4.29 (Ubuntu) Server at www.tutorialspoint.com Port 80</address>
</body></html>
```

This is unexpected. But the error message says that the document has moved to `http://192.168.173.20`, while we specified `192.168.173.10` in the setup request. So, let's change the address and try again:
```python
with RemoteServer(TCP_IP, TCP_PORT, verbose=True) as s:    
    setup_connection("192.168.173.20")

    print(get_page(s, "/"))
```

Output:
```console
root@kali:/media/sf_CTFs/checkpoint/Shoes# python3 solve.py
Sending : 5a 01 fe dd 74 9c 2e
Received: 5a fe be b4 45 0a b9 f2 59 35
Sending : 5a be f7 16 4b 04 31 51 44
Sending : 5a 01 00 01 c0 a8 ad 14 00 50 62 4a 30 63
Received: 5a 00 00 01 00 00 00 00 00 00 eb cb 75 43
Sending : 47 45 54 20 2f 2f 20 48 54 54 50 2f 31 2e 31 0d 0a 55 73 65 72 2d 41 67 65 6e 74 3a 20 4d 6f 7a 69 6c 6c 61 2f 35 2e 30 20 28 57 69 6e 64 6f 77 73 20 4e 54 20 31 30 2e 30 3b 20 57 69 6e 36 34 3b 20 78 36 34 29 20 41 70 70 6c 65 57 65 62 4b 69 74 2f 35 33 37 2e 33 36 20 28 4b 48 54 4d 4c 2c 20 6c 69 6b 65 20 47 65 63 6b 6f 29 20 43 68 72 6f 6d 65 2f 37 34 2e 30 2e 33 37 32 39 2e 31 36 39 20 53 61 66 61 72 69 2f 35 33 37 2e 33 36 0d 0a 48 6f 73 74 3a 20 77 77 77 2e 74 75 74 6f 72 69 61 6c 73 70 6f 69 6e 74 2e 63 6f 6d 0d 0a 41 63 63 65 70 74 2d 4c 61 6e 67 75 61 67 65 3a 20 65 6e 2d 75 73 0d 0a 43 6f 6e 6e 65 63 74 69 6f 6e 3a 20 4b 65 65 70 2d 41 6c 69 76 65 0d 0a 0d 0a
Received: 48 54 54 50 2f 31 2e 31 20 32 30 30 20 4f 4b 0d 0a 44 61 74 65 3a 20 46 72 69 2c 20 32 39 20 4d 61 79 20 32 30 32 30 20 31 39 3a 30 36 3a 35 32 20 47 4d 54 0d 0a 53 65 72 76 65 72 3a 20 41 70 61 63 68 65 2f 32 2e 34 2e 32 39 20 28 55 62 75 6e 74 75 29 0d 0a 4c 61 73 74 2d 4d 6f 64 69 66 69 65 64 3a 20 57 65 64 2c 20 32 32 20 41 70 72 20 32 30 32 30 20 31 30 3a 34 39 3a 35 36 20 47 4d 54 0d 0a 45 54 61 67 3a 20 22 39 64 2d 35 61 33 64 65 65 34 39 31 64 61 37 32 22 0d 0a 41 63 63 65 70 74 2d 52 61 6e 67 65 73 3a 20 62 79 74 65 73 0d 0a 43 6f 6e 74 65 6e 74 2d 4c 65 6e 67 74 68 3a 20 31 35 37 0d 0a 56 61 72 79 3a 20 41 63 63 65 70 74 2d 45 6e 63 6f 64 69 6e 67 0d 0a 4b 65 65 70 2d 41 6c 69 76 65 3a 20 74 69 6d 65 6f 75 74 3d 35 2c 20 6d 61 78 3d 31 30 30 0d 0a 43 6f 6e 6e 65 63 74 69 6f 6e 3a 20 4b 65 65 70 2d 41 6c 69 76 65 0d 0a 43 6f 6e 74 65 6e 74 2d 54 79 70 65 3a 20 74 65 78 74 2f 68 74 6d 6c 0d 0a 0d 0a 46 69 6c 65 73 20 69 6e 20 73 65 72 76 65 72 3a 0a 0a 69 6e 64 65 78 2e 68 74 6d 6c 0a 41 72 62 6f 6b 2e 6a 70 67 0a 42 6c 61 73 74 6f 69 73 65 2e 6a 70 67 0a 42 75 6c 62 61 73 61 75 72 2e 6a 70 67 0a 42 75 74 74 65 72 66 72 65 65 2e 6a 70 67 0a 43 68 61 72 69 7a 61 72 64 2e 6a 70 67 0a 46 6c 61 67 2e 6a 70 67 0a 4d 65 74 61 70 6f 64 2e 6a 70 67 0a 52 61 74 74 61 74 61 2e 6a 70 67 0a 53 71 75 69 72 74 6c 65 2e 6a 70 67 0a 57 61 72 74 6f 72 74 6c 65 2e 6a 70 67 0a 0a
HTTP/1.1 200 OK
Date: Fri, 29 May 2020 19:06:52 GMT
Server: Apache/2.4.29 (Ubuntu)
Last-Modified: Wed, 22 Apr 2020 10:49:56 GMT
ETag: "9d-5a3dee491da72"
Accept-Ranges: bytes
Content-Length: 157
Vary: Accept-Encoding
Keep-Alive: timeout=5, max=100
Connection: Keep-Alive
Content-Type: text/html

Files in server:

index.html
Arbok.jpg
Blastoise.jpg
Bulbasaur.jpg
Butterfree.jpg
Charizard.jpg
Flag.jpg
Metapod.jpg
Rattata.jpg
Squirtle.jpg
Wartortle.jpg
```

Looks much better.

Finally, we want to request the flag:

```python
OUTPUT_FOLDER = "out"

def save_file(s, file):
    s.send(HTTP_REQUEST.format(file).encode("ascii"))
    data = s.recv()
    if not os.path.exists(OUTPUT_FOLDER):
        os.makedirs(OUTPUT_FOLDER)
    
    http_headers, binary_data = data.split(b"\r\n\r\n")
    http_headers = http_headers.decode("ascii")
    content_length = int(re.search(r"Content-Length: (\d+)", http_headers, re.MULTILINE).group(1))

    while len(binary_data) < content_length:
        binary_data += s.recv()

    with open(os.path.join(OUTPUT_FOLDER, file), "wb") as f:
        f.write(binary_data)

    print ("\nSaved {} to {}/\n".format(file, OUTPUT_FOLDER))

with RemoteServer(TCP_IP, TCP_PORT, verbose=True) as s:    
    setup_connection("192.168.173.20")

    save_file(s, "Flag.jpg")
```

We receive a JPEG file with the flag: `CSA{p1k4p1k4}`