# Rick Roll Do Not Listen
Grab Bag, 100 points

## Description:

> *[Link to WAV file]*
> 
> I don't know what you expected.

A file was attached.

## Solution:

If we listen to the file, we hear the infamous Rick Astley song [Never Gonna Give You Up](https://www.youtube.com/watch?v=dQw4w9WgXcQ).

However, if we use `steghide` with an empty passphrase, we get the flag:
```console
root@kali:/media/sf_CTFs/mitre/rick# steghide extract -sf RR.wav
Enter passphrase:
wrote extracted data to "flag.txt".
root@kali:/media/sf_CTFs/mitre/rick# cat flag.txt
MCA{asFSA9123mflZ01f}
```