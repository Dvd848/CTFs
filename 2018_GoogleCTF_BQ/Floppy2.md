# Floppy 2
Misc.

## Description:
> Looks like you found a way to open the file in the floppy! But that `www.com` file looks suspicious.. Dive in and take another look?


## Solution:

Let's take a look at `www.com`, which we extracted during the [First Floppy](Floppy.md) challenge:

```console
root@kali:/media/sf_CTFs/google/floppy/foo# file www.com
www.com: ASCII text, with CR, LF line terminators
root@kali:/media/sf_CTFs/google/floppy/foo# xxd www.com
00000000: 6844 3758 2d74 3675 675f 686c 285d 5768  hD7X-t6ug_hl(]Wh
00000010: 3824 5e31 3547 4731 2d68 6272 5835 7072  8$^15GG1-hbrX5pr
00000020: 5059 4757 5e51 4649 7578 5947 464b 2c31  PYGW^QFIuxYGFK,1
00000030: 2d46 4749 7571 5a68 4849 5825 4129 4921  -FGIuqZhHIX%A)I!
00000040: 6853 4c58 3453 4921 7b70 2a53 3a65 544d  hSLX4SI!{p*S:eTM
00000050: 277e 5f3f 6f3f 563b 6d3b 4367 4141 5d55  '~_?o?V;m;CgAA]U
00000060: 6429 484f 3b7b 206c 7b7d 396c 3e6a 4c4c  d)HO;{ l{}9l>jLL
00000070: 505b 2d60 7c30 6776 5059 306f 6e51 3067  P[-`|0gvPY0onQ0g
00000080: 655a 3077 5935 3e44 3067 5d68 2b28 582d  eZ0wY5>D0g]h+(X-
00000090: 6b26 3460 505b 302f 2c36 3422 5034 4150  k&4`P[0/,64"P4AP
000000a0: 470d 5468 6520 466f 6f62 616e 697a 6572  G.The Foobanizer
000000b0: 3930 3030 2069 7320 6e6f 206c 6f6e 6765  9000 is no longe
000000c0: 7220 6f6e 2074 6865 204f 6666 4875 6220  r on the OffHub
000000d0: 444d 5a2e 2020 2020 2020 2020 2020 240a  DMZ.          $.
000000e0: 0d                                       .
```

It's a com file, but it doesn't look like much of an executable.

After many failed attempts to figure out how to decode the text, I ran out of ideas and as a last resort, tried to run the file in DOSBox, since essentially that's one thing I'd do with a standard `com` file. 

To my great surprise, it ran:
```
C:\>WWW.COM
The Foobanizer 9000 is no longer on the OffHub DMZ.
```

Time for the [DOSBox Debugger](https://www.vogons.org/viewtopic.php?f=32&t=7323) (usage [here](https://www.vogons.org/viewtopic.php?t=3944)).

Launch the debugger, then:
1. Alt+Pause (break) - perform halt
2. `bpint 21 *` - Set breakpoint on `int 21` (the most common DOS interrupt)
3. `F5` - resume 
4. Run the program
5. BP is hit:
```
F000:20C6  CD21                int  21
F000:20C8  FE380100            callback 0001  (stop)
F000:20CC  CD22                int  22
F000:20CE  FE380100            callback 0001  (stop)
```
6. `F5` to resume, hopefully hit some logic which doesn't look like a callback table
7. BP is hit:
```
0192:013E  CD21                int  21
0192:0140  68534C              push 4C53
0192:0143  58                  pop  ax
0192:0144  3453                xor  al,53
0192:0146  CD21                int  21
```
8. `d ds:0` to set the data view to the beginning of the data segment
9. Scroll down with `PageDown` to inspect the data segment
10. Bingo:
```
---(Register Overview                   )---
EAX=00000940  ESI=00000181  DS=0192   ES=0192   FS=0000   GS=0000   SS=0192 Real
EBX=000001A0  EDI=0000016F  CS=0192   EIP=0000013E  C0 Z0 S0 O0 A0 P0 D0 I1 T0
ECX=00000000  EBP=00005A65                                          IOPL3  CPL0
EDX=00000148  ESP=0000FFFE                                  747473539
---(Data Overview   Scroll: page up/down)---

0192:0100     68 44 37 58 2D 74 36 75 67 5F 68 6C 28 5D 57 68  hD7X-t6ug_hl(]Wh
0192:0110     38 24 5E 31 35 47 47 31 2D 68 62 72 58 35 70 72  8$^15GG1-hbrX5pr
0192:0120     50 59 47 57 5E 51 46 49 75 FC 59 47 46 8B 2C 31  PYGW^QFIu.YGF.,1
0192:0130     2D 46 47 49 75 F5 5A 68 48 49 58 25 41 29 CD 21  -FGIu.ZhHIX%A).!
0192:0140     68 53 4C 58 34 53 CD 21 43 54 46 7B 67 30 30 64  hSLX4S.!CTF{g00d
0192:0150     6F 31 64 44 4F 53 2D 46 54 57 7D 0D 0D 0D 0D 0E  o1dDOS-FTW}.....
0192:0160     49 49 34 7F 5C 0D 70 35 4B 12 57 3D 0E 0D 29 16  II4.\.p5K.W=..).
0192:0170     50 5B 2D 60 7C 30 67 76 50 59 30 6F 6E 51 30 67  P[-`|0gvPY0onQ0g

---(Code Overview   Scroll: up/down     )---
0192:013E  CD21                int  21
0192:0140  68534C              push 4C53
0192:0143  58                  pop  ax
0192:0144  3453                xor  al,53
0192:0146  CD21                int  21
0192:0148  43                  inc  bx
0192:0149  54                  push sp
0192:014A  46                  inc  si
0192:014B  7B67                jpo  000001B4 ($+67)        (down)
0192:014D  3030                xor  [bx+si],dh             ds:[0321]=0000
-> _
```

The flag: CTF{g00do1dDOS-FTW}
