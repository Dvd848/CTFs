# Y3tAn0thErSecr3t

 * Category: Forensics
 * 100 Points
 * Solved by the JCTF Team

## Description

> OK ! you got here.... You probably have few ways to find the flag:
> 
> look in the PCAP, it might have hints i think they used figlet to generate the flag

```
RUN FLAGPRE="BSidesTLV2024" && \
flagid=Flag_M2_Foremost && \
echo -e "* Update flag [$flagid]" && \
FLAG=`cat flag.flag` && \
FLAGPOINTER=`tr -dc A-Za-z0-9 </dev/urandom | head -c 23` && \
figlet -w 250 -kp < flag.flag > ${FLAGPOINTER} && \
rm -f  ./flag.flag
```

> When you find the flag don't forget to submit it in the correct format: `BSidesTLV2024{FLAG}`

A PCAP file was attached.

## Solution

Let's use ChatGPT to break down what each line does:

 * `RUN FLAGPRE="BSidesTLV2024"`: Sets an environment variable `FLAGPRE` to "BSidesTLV2024". The RUN keyword suggests this might be used within a Dockerfile or a similar build script.
 * `flagid=Flag_M2_Foremost`: Sets another variable `flagid` to "Flag_M2_Foremost".
 * `echo -e "* Update flag [$flagid]"`: Prints the message "* Update flag [Flag_M2_Foremost]" to the console.
 * ``FLAG=`cat flag.flag` ``: Reads the contents of a file named `flag.flag` into the variable FLAG.
 * ``FLAGPOINTER=`tr -dc A-Za-z0-9 </dev/urandom | head -c 23` ``: Generates a random string of 23 alphanumeric characters and stores it in the variable `FLAGPOINTER`.
 * `figlet -w 250 -kp < flag.flag > ${FLAGPOINTER}`: Uses `figlet` to create a large ASCII art representation of the contents of `flag.flag` and writes it to a file named with the random string stored in `FLAGPOINTER`. The `-w 250` option sets the width, and `-kp` are options to control figlet's behavior.
 * `rm -f ./flag.flag`: Deletes the original `flag.flag` file.

Now, we can proceed to inspecting the PCAP content. Browsing the TCP streams, we find:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Y3tAn0thErSecr3t]
└─$ tshark -r pcapfile.pcap -qz follow,tcp,ascii,9

===================================================================
Follow: tcp,ascii
Filter: tcp.stream eq 9
Node 0: 172.17.0.2:45144
Node 1: 143.244.222.116:80
126
GET //?link=H3X5o7GK8uj83acZAfw7iWQ HTTP/1.1
Host: bstlv24-y3tan0theraecr3t.chals.io
User-Agent: curl/8.7.1
Accept: */*


        428
HTTP/1.1 301 Moved Permanently
Server: openresty/1.19.3.1
Date: Mon, 24 Jun 2024 08:47:04 GMT
Content-Type: text/html
Content-Length: 175
Connection: keep-alive
Location: https://bstlv24-y3tan0theraecr3t.chals.io//?link=H3X5o7GK8uj83acZAfw7iWQ

<html>
<head><title>301 Moved Permanently</title></head>
<body>
<center><h1>301 Moved Permanently</h1></center>
<hr><center>openresty/1.19.3.1</center>
</body>
</html>
```

Trying to access `https://bstlv24-y3tan0theraecr3t.chals.io//?link=H3X5o7GK8uj83acZAfw7iWQ` 
does not lead anywhere, but modifying the address to the following gives us the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Y3tAn0thErSecr3t]
└─$ curl https://bstlv24-y3tan0theraecr3t.chals.io/H3X5o7GK8uj83acZAfw7iWQ
 __  __            _____    _  _  _    _    _      _____    _  __ _  _    _    _____          _   ____
|  \/  | _   _    |___ /   | |/ || |_ | |_ | |    |___ /   | |/ // || |_ | |_ | ____| _ __   | | | __ )   ___    ___
| |\/| || | | |     |_ \   | || || __|| __|| |      |_ \   | ' / | || __|| __||  _|  | '_ \ / __)|  _ \  / _ \  / _ \
| |  | || |_| | _  ___) |_ | || || |_ | |_ | |___  ___) |_ | . \ | || |_ | |_ | |___ | | | |\__ \| |_) || (_) || (_) |
|_|  |_| \__, |(_)|____/(_)|_||_| \__| \__||_____||____/(_)|_|\_\|_| \__| \__||_____||_| |_|(   /|____/  \___/  \___/
         |___/                                                                               |_|
```

The flag: `BSidesTLV2024{My.3.l1ttL3.K1ttEn$Boo}`