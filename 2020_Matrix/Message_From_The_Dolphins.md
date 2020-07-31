# Message from the Dolphins
Category: Other, 70 Points

## Description

> The Zip got crumbled and the massage got scrambled.
> 
> Capture the flag to get the message from the Dolphins.


An archive file was attached.

## Solution

Let's try to extract the archive:

```console
root@kali:/media/sf_CTFs/matrix/Message_From_The_Dolphins# gunzip -v 42.gz
42.gz:    0.0% -- replaced with 42
```

What did we get?
```console
root@kali:/media/sf_CTFs/matrix/Message_From_The_Dolphins# file 42
42: PNG image data, 1024 x 768, 8-bit/color RGBA, non-interlaced
```

We run `zsteg` on it and get the flag:
```console
root@kali:/media/sf_CTFs/matrix/Message_From_The_Dolphins# zsteg 42
meta Raw profile type APP1.. text: "\ngeneric profile\n     246\n4578696600004d4d002a0000000800040128000300000001000200000213000300000001\n0001000087690004000000010000003e88250004000000010000008a0000000000049000\n00070000000430323331910100070000000401020300928600070000001500000074a000\n0007000000043031303000000000415343494900000053746567616e6f67726170687900\n000400010002000000024e0000000002000500000003000000c000030002000000024500\n00000004000500000003000000d800000000000000200000000100000000000000010000\n7d0900000946000000220000000100000034000000010001cfc9000014bc\n"
b1,rgb,lsb,xy       .. text: "Flag_So_long_and_thanks_for_all_the_fish"
b3,g,msb,xy         .. text: "'\t $B|mH"
b4,g,lsb,xy         .. file: 0420 Alliant virtual executable
b4,bgr,lsb,xy       .. file: PDP-11 UNIX/RT ldp
b4,abgr,msb,xy      .. file: Applesoft BASIC program data, first line number 143
```