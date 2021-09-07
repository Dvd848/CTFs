# Unknown - Aboard boat
Category: Net

## Description

> That was close! The armed guard didn’t notice you. The floor shakes, the boat is leaving the harbor. You are trying to stay hidden. You see two guards coming your way, you sneak into a small scrubber, they pass it, but then one of the guards takes out his phone and says “OK Google” and your phone suddenly makes a noise: PLING! The guards heard it: Guard 1: "Did you hear that?" Guard 2: "It must have come from the scrubber" Guard 1: "Let's have a look!" The guards close in, you are trapped! Suddenly a bell rings in the distance, and a voice talks through speakers: "Every man to their positions, the ship is about to debark, I repeat, every man to their positions." The guards change their course and head for their positions. Pheww, that was close! You find a better place to hide and settle in for the journey. Then you notice an ethernet socket in the wall. Might as well sniff some traffic while you're here.
> 
> Challenge: Noise on the wire (net)
> 
> You connect your laptop to the ethernet socket and start wireshark. It taks a while before something interesting pops up - perhaps the crew as busy with whatever is that they normally do. You look through the packets, and hey, these look pretty interesting...

A network capture was attached.

## Solution

An overview of the network capture:

```console
┌──(user@kali)-[/media/sf_CTFs/google/13_Unknown_-_Aboard_boat/download]
└─$ tshark -qz io,phs -r httponly.pcap

===================================================================
Protocol Hierarchy Statistics
Filter:

eth                                      frames:54 bytes:12435
  ip                                     frames:54 bytes:12435
    tcp                                  frames:54 bytes:12435
      http                               frames:8 bytes:4583
        media                            frames:1 bytes:546
        data-text-lines                  frames:2 bytes:1759
          tcp.segments                   frames:1 bytes:1306
      websocket                          frames:8 bytes:1324
        data-text-lines                  frames:8 bytes:1324
===================================================================
```

Scanning through the TCP streams, we see the following interesting results:

```console
┌──(user@kali)-[/media/sf_CTFs/google/13_Unknown_-_Aboard_boat/download]
└─$ tshark -r httponly.pcap -qz follow,tcp,ascii,1

===================================================================
Follow: tcp,ascii
Filter: tcp.stream eq 1
Node 0: 192.168.56.1:63546
Node 1: 192.168.56.198:80
476
GET / HTTP/1.1
Host: 192.168.56.198
Connection: keep-alive
Pragma: no-cache
Cache-Control: no-cache
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.107 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9


        1460
HTTP/1.1 200 OK
X-Powered-By: Express
Accept-Ranges: bytes
Cache-Control: public, max-age=0
Last-Modified: Sat, 31 Jul 2021 11:09:12 GMT
ETag: W/"14d9-17afc40f170"
Content-Type: text/html; charset=UTF-8
Content-Length: 5337
Date: Sat, 31 Jul 2021 11:11:18 GMT
Connection: keep-alive

<html>
<!--
___  ________ _     _____ _____ ___  ________   __  _____ ______  ___ ______ _____
|  \/  |_   _| |   |_   _|_   _/ _ \ | ___ \ \ / / |  __ \| ___ \/ _ \|  _  \  ___|
| .  . | | | | |     | |   | |/ /_\ \| |_/ /\ V /  | |  \/| |_/ / /_\ \ | | | |__
| |\/| | | | | |     | |   | ||  _  ||    /  \ /   | | __ |    /|  _  | | | |  __|
| |  | |_| |_| |_____| |_  | || | | || |\ \  | |   | |_\ \| |\ \| | | | |/ /| |___
\_|  |_/\___/\_____/\___/  \_/\_| |_/\_| \_| \_/    \____/\_| \_\_| |_/___/ \____/


 _____ _   _ _____ ________   _______ _____ _____ _____ _   _   ___________  _____
|  ___| \ | /  __ \| ___ \ \ / / ___ \_   _|_   _|  _  | \ | | |_   _| ___ \/  __ \
| |__ |  \| | /  \/| |_/ /\ V /| |_/ / | |   | | | | | |  \| |   | | | |_/ /| /  \/
|  __|| . ` | |    |    /  \ / |  __/  | |   | | | | | | . ` |   | | |    / | |
| |___| |\  | \__/\| |\ \  | | | |     | |  _| |_\ \_/ / |\  |  _| |_| |\ \ | \__/\
\____/\_| \_/\____/\_| \_| \_/ \_|     \_/  \___/ \___/\_| \_/  \___/\_| \_| \____/

                                 UNHACKABLE! (TM)

-->
<head>
  <title>Military Grade IRC</title>
  <style>
@import url('https://fonts.googleapis.com/css
        1460
2?family=Montserrat:wght@300&family=Share+Tech+Mono&display=swap');

body {
  margin: 0;
  width: 100vw;
  height: 100vh;
  background-color: #0d497f;
  color: white;
  font-family: 'Montserrat', sans-serif;
  font-size: 1.25em;
}

input {
  font-family: 'Share Tech Mono', monospace;
  font-size: 1.25em;
  width: 15em;
}

input[type=submit] {
  font-family: 'Montserrat', sans-serif;
}

div.outer {
  display: flex;
  align-items: center;
  flex-direction: row;
  align-content: center;
  justify-content: center;
  width: 100%;
  height: 100%;
}

div.chat {
  margin: 2em;
}

div.chatbox {
  background-color: #1463ab;
  margin-top: 2em;
  height: 18em;
  padding: 0.25em;
  font-size: 0.75em;
}

div.chatbox p {
  margin: 0;
}

span.name {
  color: white;
  font-weight: bold;
}

button {
  width: 100%;
  font-family: 'Montserrat', sans-serif;
}

  </style>
</head>
<body>
  <div class="outer">
    <div class="chat">
      <div>
        <input name="codename" id="codename" placeholder="Code name">
        <input name="msg" id="msg" placeholder="Message">
      </div>
      <div>
        <button id="milgrade">Enable Military Grade Encryption</button>
      </div>
      <div class="chatbox" id="chatbox"></div>
    </div>
  </div>
</body>
<script>
let elCodename = null;
let elMsg = null;
let elMilGrade = null;
let elChatBox = null;
let milGradeEnabled = false;
let ws = null;

function appendMessage(codename, text) {
  const elSpan = document.creat
        1460
eElement("span");
  elSpan.classList.add("name");
  elSpan.innerText = codename;

  const elP = document.createElement("p");
  elP.appendChild(document.createTextNode("["));
  elP.appendChild(elSpan);
  elP.appendChild(document.createTextNode("] " + text));

  elChatBox.appendChild(elP);
}

function toHex(n) {
  return (n >> 4).toString(16) + (n & 0xf).toString(16);
}

function encryptWithMilitaryGradeEncryption(text) {
  const encoder = new TextEncoder();
  const encoded = encoder.encode(text);
  const len = encoded.length;

  const key = Uint8Array.from([
    11, 22, 33, 44, 55, 66, 77, 88, 99, 101, 202
  ]);
  const keylen = key.length;

  let hexstr = "";

  for (let i = 0; i < len; i++) {
    hexstr += toHex(encoded[i] ^ key[i % keylen]);
  }

  return hexstr;
}

function decryptWithMilitaryGradeEncryption(hexstr) {
  const len = hexstr.length;

  const key = Uint8Array.from([
    11, 22, 33, 44, 55, 66, 77, 88, 99, 101, 202
  ]);
  const keylen = key.length;

  const arr = [];

  for (let i = 0; i < len; i += 2) {
    const byte = parseInt(hexstr.substring(i, i + 2), 16);
    arr.push(byte ^ key[(i >> 1) % keylen]);
  }

  const decoder = new TextDecoder();
  return decoder.decode(Uint8Array.from(arr));
}

function sendMessage() {
  const name = elCodename.value;
  const text = elMsg.value;
  const message = milGradeEnabled ?
      encryptWithMilitaryGradeEncryption(text) : text;
  elMsg.value = "";

  const data = JSON.stringify(
        1252
{
    militaryGradeEncryption: milGradeEnabled,
    codename: name,
    message: message
  });

  appendMessage(name, text);

  ws.send(data);
}

function onMilGradeClick() {
  milGradeEnabled = !milGradeEnabled;

  if (milGradeEnabled) {
    elMilGrade.innerText = "Disable Military Grade Encryption";
  } else {
    elMilGrade.innerText = "Enable Military Grade Encryption";
  }
}

function onReciveMessage(ev) {
  const packet = ev.data;
  const data = JSON.parse(packet);
  const text = data.militaryGradeEncryption ?
      decryptWithMilitaryGradeEncryption(data.message) : data.message;

  appendMessage(data.codename, text);
}

function onMsgKeyDown(event) {
  if (event.keyCode === 13) {
    event.preventDefault();
    sendMessage();
  }
}

function main() {
  elCodename = document.getElementById("codename");
  elMsg = document.getElementById("msg");
  elMilGrade = document.getElementById("milgrade");
  elChatBox = document.getElementById("chatbox");

  elMilGrade.addEventListener("click", onMilGradeClick);
  elMsg.addEventListener("keydown", onMsgKeyDown);

  ws = new WebSocket("ws://192.168.56.198:80");
  ws.addEventListener("message", onReciveMessage);
}

window.addEventListener("DOMContentLoaded", main, false);
</script>
</html>

419
GET /favicon.ico HTTP/1.1
Host: 192.168.56.198
Connection: keep-alive
Pragma: no-cache
Cache-Control: no-cache
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.107 Safari/537.36
Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8
Referer: http://192.168.56.198/
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9


        399
HTTP/1.1 404 Not Found
X-Powered-By: Express
Content-Security-Policy: default-src 'none'
X-Content-Type-Options: nosniff
Content-Type: text/html; charset=utf-8
Content-Length: 150
Date: Sat, 31 Jul 2021 11:11:18 GMT
Connection: keep-alive

<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Error</title>
</head>
<body>
<pre>Cannot GET /favicon.ico</pre>
</body>
</html>

===================================================================

┌──(user@kali)-[/media/sf_CTFs/google/13_Unknown_-_Aboard_boat/download]
└─$ tshark -r httponly.pcap -qz follow,tcp,ascii,3

===================================================================
Follow: tcp,ascii
Filter: tcp.stream eq 3
Node 0: 192.168.56.1:62106
Node 1: 192.168.56.198:80
491
GET / HTTP/1.1
Host: 192.168.56.198
Connection: Upgrade
Pragma: no-cache
Cache-Control: no-cache
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.107 Safari/537.36
Upgrade: websocket
Origin: http://192.168.56.198
Sec-WebSocket-Version: 13
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9
Sec-WebSocket-Key: ylE8fRTaGQmdMzW36op4HA==
Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits


        129
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: qxA6c0bXwkHUXALTC5mfFrg8438=


110
.......'...............Z.....F.........B.H.....Q.Y.L.........P.....G.J...J...........J...J...J.....B.
        85
.S{"militaryGradeEncryption":false,"codename":"BadGuy87","message":"which zip file?"}
102
u...f.&)i.1.w.*.i.y
j.aV%.,.i.r_4.oNj.0.f.&N=.4.k.oL~.6Ll.,.+..$B.9.w.%.k.a.
        142
.~..{"militaryGradeEncryption":false,"codename":"BadGuy87","message":"oh, that one... gimme a sec, need to turn on military grade encryption"}
76
..0...K...\...B...Q...^...@...^...Q.......T...].......^.......]...Q...
.....
        211
.~..{"militaryGradeEncryption":true,"codename":"BadGuy87","message":"717f510b44623d391016bd6464450c5e316d1a0c16b95f794d487a2719373000be4a54445843273f080216b97c795348642d19300a169d627a4d645634280c0c21a53a241218"}
89
..6>..+...,...;...mC..:...,...!...mC.. ...~J..c[..<
..*[..yN..{...{L..|M..m.
        77
.K{"militaryGradeEncryption":true,"codename":"BadGuy87","message":"72734044"}
===================================================================
```

We can extract the following conversation:

```
{"militaryGradeEncryption":false,"codename":"BadGuy87","message":"which zip file?"}

{"militaryGradeEncryption":false,"codename":"BadGuy87","message":"oh, that one... gimme a sec, need to turn on military grade encryption"}

{"militaryGradeEncryption":true,"codename":"BadGuy87","message":"717f510b44623d391016bd6464450c5e316d1a0c16b95f794d487a2719373000be4a54445843273f080216b97c795348642d19300a169d627a4d645634280c0c21a53a241218"}

{"militaryGradeEncryption":true,"codename":"BadGuy87","message":"72734044"}
```

Using the implementation in the HTML file, we can decode the military grade encryption:

```javascript
>>> decryptWithMilitaryGradeEncryption("717f510b44623d391016bd6464450c5e316d1a0c16b95f794d487a2719373000be4a54445843273f080216b97c795348642d19300a169d627a4d645634280c0c21a53a241218")
"zip's password is BossToldMeToSetABetterPasswordSoThisWillHaveToDo1234"

>>> decryptWithMilitaryGradeEncryption("72734044")
"yeah"
```

We just need to extract the zip file from the network capture:

```console
┌──(user@kali)-[/media/sf_CTFs/google/13_Unknown_-_Aboard_boat/download]
└─$ tshark -r httponly.pcap -o "tcp.desegment_tcp_streams: TRUE" -o "tcp.no_subdissector_on_error: FALSE" --export-objects "http,exported_objects"

┌──(user@kali)-[/media/sf_CTFs/google/13_Unknown_-_Aboard_boat/download]
└─$ ls exported_objects
%2f  favicon.ico  flag.zip

┌──(user@kali)-[/media/…/google/13_Unknown_-_Aboard_boat/download/exported_objects]
└─$ unzip -P BossToldMeToSetABetterPasswordSoThisWillHaveToDo1234 flag.zip
Archive:  flag.zip
 extracting: flag.txt

┌──(user@kali)-[/media/…/google/13_Unknown_-_Aboard_boat/download/exported_objects]
└─$ cat flag.txt
CTF{PleaseAssumeThisIsSomeSecretStuffThankYou}
```


