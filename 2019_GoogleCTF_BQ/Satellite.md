# Satellite
Category: Networking

## Description
> Placing your ship in range of the Osmiums, you begin to receive signals. Hoping that you are not detected, because it's too late now, you figure that it may be worth finding out what these signals mean and what information might be "borrowed" from them. Can you hear me Captain Tim? Floating in your tin can there? Your tin can has a wire to ground control? Find something to do that isn't staring at the Blue Planet.

A binary file was attached.

## Solution

Let's run the attached binary:

```
Hello Operator. Ready to connect to a satellite?
Enter the name of the satellite to connect to or 'exit' to quit
test
Unrecognized satellite: test
```

Since we don't know what the program is expecting, let's check the binary with Ghidra (according to the strings this is a Golang binary, so we first run [golang_renamer.py](https://github.com/ghidraninja/ghidra_scripts) on it):

```assembly
        004f8b95 e8 e6 12        CALL       strings.ToLower                                                                undefined strings.ToLower(long *
                 fe ff
        004f8b9a 48 8b 44        MOV        RAX,qword ptr [RSP + local_160]
                 24 10
        004f8b9f 48 8b 4c        MOV        param_4,qword ptr [RSP + local_158]
                 24 18
        004f8ba4 48 83 f9 05     CMP        param_4,0x5
        004f8ba8 75 12           JNZ        LAB_004f8bbc
        004f8baa 81 38 65        CMP        dword ptr [RAX],"exit"
                 78 69 74
        004f8bb0 75 0a           JNZ        LAB_004f8bbc
        004f8bb2 80 78 04 0a     CMP        byte ptr [RAX + 0x4]=>DAT_00000004+1,0xa                                       = null
        004f8bb6 0f 84 ea        JZ         LAB_004f8ca6
                 00 00 00
                             LAB_004f8bbc                                    XREF[2]:     004f8ba8(j), 004f8bb0(j)  
        004f8bbc 48 83 f9 07     CMP        param_4,0x7
        004f8bc0 75 1a           JNZ        LAB_004f8bdc
        004f8bc2 81 38 6f        CMP        dword ptr [RAX],"osmi"
                 73 6d 69
        004f8bc8 75 12           JNZ        LAB_004f8bdc
        004f8bca 66 81 78        CMP        word ptr [RAX + 0x4]=>DAT_00000004+1,"um"                                      = null
                 04 75 6d
        004f8bd0 75 0a           JNZ        LAB_004f8bdc
        004f8bd2 80 78 06 0a     CMP        byte ptr [RAX + 0x6]=>DAT_00000004+3,0xa                                       = null
        004f8bd6 0f 84 ad        JZ         LAB_004f8c89
                 00 00 00
; .....
                             LAB_004f8c89                                    XREF[1]:     004f8bd6(j)  
        004f8c89 48 8b 44        MOV        RAX,qword ptr [RSP + local_108]
                 24 68
        004f8c8e 48 89 04 24     MOV        qword ptr [RSP]=>local_170,RAX
        004f8c92 48 8b 44        MOV        RAX,qword ptr [RSP + local_130]
                 24 40
        004f8c97 48 89 44        MOV        qword ptr [RSP + local_168],RAX
                 24 08
        004f8c9c e8 8f 00        CALL       main.connectToSat                                                              undefined main.connectToSat(long
                 00 00
        004f8ca1 e9 11 fd        JMP        LAB_004f89b7
                 ff ff

```

It looks like the satellite name should be "osmium", let's try that:

```console
Enter the name of the satellite to connect to or 'exit' to quit
osmium
Establishing secure connection to osmium
 satellite...
Welcome. Enter (a) to display config data, (b) to erase all data or (c) to disconnect
a
Username: brewtoot password: ********************       166.00 IS-19 2019/05/09 00:00:00        Swath 640km     Revisit capacity twice daily, anywhere Resolution panchromatic: 30cm multispectral: 1.2m        Daily acquisition capacity: 220,000km²  Remaining config data written to: https://docs.google.com/document/d/14eYPluD_pi3824GAFanS29tWdTcKxP_XUxx7e303-3E
```

The password is redacted! 


First, let's peek at the Google Docs file.It contains a single line, base64 encoded:
```
VXNlcm5hbWU6IHdpcmVzaGFyay1yb2NrcwpQYXNzd29yZDogc3RhcnQtc25pZmZpbmchCg==
```

Let's decode it:
```console
root@kali:/media/sf_CTFs/google/Satellite# echo TG9naW5zIGZvciBob21lIGFuZCB3b3JrIGNvbXB1dGVyOgpVc2VybmFtZTogd2Vib3J0dG8KUGFzc3dvcmQ6IHRvdGFsbHktbm90LWEtZmxhZy1rZWVwLXNuaWZmaW5nCg== | base64 -d
Logins for home and work computer:
Username: webortto
Password: totally-not-a-flag-keep-sniffing
```

Obviously, the password must have been sent over the network, let's try to sniff it.

```console
root@kali:/media/sf_CTFs/google/Satellite# tshark -nr osmium.pcapng  -Y 'frame contains "password"'
   16 28.632225193 34.76.101.29 → 10.0.2.15    TCP 417 1337 → 52616 [PSH, ACK] Seq=88 Ack=3 Win=65535 Len=363 1337 52616
root@kali:/media/sf_CTFs/google/Satellite# tshark -nr osmium.pcapng  -Y 'frame contains "password"' -T fields -e data.text -o data.show_as_text:TRUE
Username: brewtoot password: CTF{4efcc72090af28fd33a2118985541f92e793477f}      166.00 IS-19 2019/05/09 00:00:00       Swath 640km      Revisit capacity twice daily, anywhere Resolution panchromatic: 30cm multispectral: 1.2m        Daily acquisition capacity: 220,000km Remaining config data written to: https://docs.google.com/document/d/14eYPluD_pi3824GAFanS29tWdTcKxP_XUxx7e303-3E
```

We got the flag - CTF{4efcc72090af28fd33a2118985541f92e793477f}.
