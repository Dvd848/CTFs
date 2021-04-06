# Disk, disk, sleuth!
Category: Forensics, 110 points

## Description
> Use `srch_strings` from the sleuthkit and some terminal-fu to find a flag in this disk image

A disk image was attached.

## Solution

The description pretty much says it all:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Disk_disk_sleuth]
└─$ srch_strings dds1-alpine.flag.img | grep pico
ffffffff81399ccf t pirq_pico_get
ffffffff81399cee t pirq_pico_set
ffffffff820adb46 t pico_router_probe
  SAY picoCTF{f0r3ns1c4t0r_n30phyt3_a011c142}
```