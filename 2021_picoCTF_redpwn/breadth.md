# Breadth
Reverse Engineering, 200 points

## Description

> Surely this is what people mean when they say "horizontal scaling," right?
> 
> TOP SECRET INFO:
> 
> Our operatives managed to exfiltrate an in-development version of this challenge, where the function with the real flag had a mistake in it. Can you help us get the flag?

Two binary files were attached.

## Solution

Let's check the binary files:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/breadth]
└─$ ./breadth.v1
Dead code? What's that?
Goodbye!

┌──(user@kali)-[/media/sf_CTFs/pico/breadth]
└─$ ./breadth.v2
Dead code? What's that?
Goodbye!
```

Each of the files contains multiple strings with the flag format, for example:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/breadth]
└─$ strings ./breadth.v1 | grep picoCTF | head
picoCTF{l56NgISIwGnzQ6itzm5JTGuE52rVejGW}
picoCTF{z6j6AFu1Zt0yqoAnf03VPuUt3EM6kOPz}
picoCTF{jStHjV07iN9zmjlScFmzYUkPSLM0LCnq}
picoCTF{hzP2N5rl08alRcfqps6yWFUnHfloV2MP}
picoCTF{yJ2sOC0ko67imrEBHoMm1kHZFX1HCmBM}
picoCTF{nG1QEOEpTWbLDgdCmGsZMQW5ue156kKs}
picoCTF{VhQHm6cqLAera5k3g6TpWY1qQJjsAdvF}
picoCTF{tTYva1pFAmMsVKPOocYt4rk3aCZ3skax}
picoCTF{4CfUT1dDz04zaMj9oF1uIAZ8raUANAtw}
picoCTF{sLGe27ZoFBR6czyH3QIph0ppWH3JR2BC}
```

Let's see what's the difference between them:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/breadth]
└─$ radiff2 breadth.v1 breadth.v2
0x000002d4 cd50612f31e893681973b990ffcbc7d7b8a8f1b4 => 4352e5d74f759ff99c57060c2bc2df2751a7dda9 0x000002d4
0x0009504b 54 => 44 0x0009504b
0x0009504e b83a8037d04839c27408c30f1f80 => 483d3ec71b04740ac3660f1f8400 0x0009504e
0x00253753 31 => 32 0x00253753
```

The first difference is the build ID:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/breadth]
└─$ diff <(readelf -a breadth.v1) <(readelf -a breadth.v2)
228c228
<     38: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS generated.v1.c
---
>     38: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS generated.v2.c
16655c16655
<     Build ID: cd50612f31e893681973b990ffcbc7d7b8a8f1b4
---
>     Build ID: 4352e5d74f759ff99c57060c2bc2df2751a7dda9
```

Let's check the next difference by inspecting the contents at this location (in the newer version):

```console
┌──(user@kali)-[/media/sf_CTFs/pico/breadth]
└─$ r2 ./breadth.v2
Warning: run r2 with -e bin.cache=true to fix relocations in disassembly
 -- How good is your C?
[0x00001070]> aa
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x00001070]> s 0x0009504b
[0x0009504b]> pdb
/ 35: sym.fcnkKTQpF ();
|           0x00095040      48c74424f03e.  mov qword [rsp - 0x10], 0x41bc73e ; [0x41bc73e:8]=-1
|           0x00095049      488b4424f0     mov rax, qword [rsp - 0x10]
|           0x0009504e      483d3ec71b04   cmp rax, 0x41bc73e
|       ,=< 0x00095054      740a           je 0x95060
[0x0009504b]>
```

It's in the middle of the function, and the description said that the newer version fixes a bug for the function with the real flag. Let's follow the jump:

```console
[0x0009504b]> s 0x95060
[0x00095060]> pdb
|       :   0x00095060      488d3d910e0c.  lea rdi, str.picoCTFVnDB2LUf1VFJkdfDJtdYtFlMexPxXS6X ; 0x155ef8 ; "picoCTF{VnDB2LUf1VFJkdfDJtdYtFlMexPxXS6X}"
\       `=< 0x00095067      e9c4bff6ff     jmp sym.imp.puts
```

`picoCTF{VnDB2LUf1VFJkdfDJtdYtFlMexPxXS6X}` was accepted as the correct flag.