# Cheesy
Reversing, 100 points

## Description:

> Where will you find the flag?

A binary file was attached.

## Solution:

Let's run the file:

```console
./reversing1
QUFBQUFBQUFBQUFBQUFBQQ==
Hello! I bet you are looking for the flag..
I really like basic encoding.. can you tell what kind I used??
RkxBR2ZsYWdGTEFHZmxhZ0ZMQUdmbGFn
Q2FuIHlvdSByZWNvZ25pemUgYmFzZTY0Pz8=
RkxBR2ZsYWdGTEFHZmxhZ0ZMQUdmbGFn
WW91IGp1c3QgbWlzc2VkIHRoZSBmbGFn
```

We get a few string that look like base64.

Let's decode them:

```console
root@kali:/media/sf_CTFs/tamu/Cheesy# for i in 1 4 5 6 7; do ./reversing1 | head -n $i | tail -1 | base64 -d; echo ""; done
AAAAAAAAAAAAAAAA
FLAGflagFLAGflagFLAGflag
Can you recognize base64??
FLAGflagFLAGflagFLAGflag
You just missed the flag
```

The bash command above isolates lines 1, 4-7 using head and tail, and then pipes each line to `base64` in order to decode it.

The last line says that we just missed the flag.
Perhaps there's another string in the binary which isn't being printed?

Let's scan the binary for anything that looks like base64 using a regex expression:

```console
root@kali:/media/sf_CTFs/tamu/Cheesy# strings reversing1 | egrep "^([A-Za-z0-9+/]{4})*([A-Za-z0-9+/]{2}==|[A-Za-z0-9+/]{3}=|[A-Za-z0-9+/]{4})$"
CyIk
QUFBQUFBQUFBQUFBQUFBQQ==
RkxBR2ZsYWdGTEFHZmxhZ0ZMQUdmbGFn
Q2FuIHlvdSByZWNvZ25pemUgYmFzZTY0Pz8=
Z2lnZW17M2E1eV9SM3YzcjUxTjYhfQ==
WW91IGp1c3QgbWlzc2VkIHRoZSBmbGFn
zPLR
```

We have a few more results this time.

Let's decode them (piping through `cat -v` to avoid messing up the terminal):

```console
root@kali:/media/sf_CTFs/tamu/Cheesy# strings reversing1 | egrep "^([A-Za-z0-9+/]{4})*([A-Za-z0-9+/]{2}==|[A-Za-z0-9+/]{3}=|[A-Za-z0-9+/]{4})$" | while read line ; do echo $line | base64 -d | cat -v && echo ""; done
^K"$
AAAAAAAAAAAAAAAA
FLAGflagFLAGflagFLAGflag
Can you recognize base64??
gigem{3a5y_R3v3r51N6!}
You just missed the flag
M-LM-rM-Q
```

The flag was indeed hiding as a secret base64 string in the binary.