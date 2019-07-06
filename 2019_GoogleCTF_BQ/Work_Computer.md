# Work Computer
Category: SandBox

## Description
> With the confidence of conviction and decision making skills that made you a contender for Xenon's Universal takeover council, now disbanded, you forge ahead to the work computer.   This machine announces itself to you, surprisingly with a detailed description of all its hardware and peripherals. Your first thought is "Why does the display stand need to announce its price? And exactly how much does 999 dollars convert to in Xenonivian Bucklets?" You always were one for the trivialities of things.
> 
> Also presented is an image of a fascinating round and bumpy creature, labeled "Cauliflower for cWo" - are "Cauliflowers" earthlings?  Your 40 hearts skip a beat - these are not the strange unrelatable bipeds you imagined earthings to be.. this looks like your neighbors back home. Such curdley lobes. Will it be at the party?
> 
> SarahH, who appears to be  a programmer with several clients, has left open a terminal.  Oops.  Sorry clients!  Aliens will be poking around attempting to access your networks.. looking for Cauliflower.   That is, *if* they can learn to navigate such things.
> 
> readme.ctfcompetition.com 1337

## Solution

If we connect to the attached server, we get a shell:

```console
> ls -al
total 12
drwxrwxrwt    2 0        0               80 Jul  5 19:12 .
drwxr-xr-x   20 0        0             4096 Jun 13 14:28 ..
----------    1 1338     1338            33 Jul  5 19:12 ORME.flag
-r--------    1 1338     1338            28 Jul  5 19:12 README.flag
> whoami
whoami: unknown uid 1338
```

However, the shell blocks many commands, making it ~~impossible~~ harder to read files:
```console
> cat README.flag
error: No such file or directory
> more README.flag
error: No such file or directory
> grep -r CTF .
error: No such file or directory
> less README.flag
error: No such file or directory
```

Let's check what we have available:

```console
> ls /bin
arch
busybox
chgrp
chown
conspy
date
df
dmesg
dnsdomainname
dumpkmap
echo
false
fdflush
fsync
getopt
hostname
ionice
iostat
ipcalc
kill
login
ls
lzop
makemime
mkdir
mknod
mktemp
mount
mountpoint
mpstat
netstat
nice
pidof
ping
ping6
pipe_progress
printenv
ps
pwd
reformime
rm
rmdir
run-parts
setpriv
setserial
shell
sleep
stat
stty
sync
tar
true
umount
uname
usleep
watch
```

`tar` looks useful, since it needs to access the content of files:

```console
> tar c README.flag
README.flag                0000400 0002472 0002472 00000000034 13507730557 010427  0
ustar   1338                            1338
CTF{4ll_D474_5h4ll_B3_Fr33}
```

We got a flag: CTF{4ll_D474_5h4ll_B3_Fr33}

Let's try it with the other file:
```console
> tar c ORME.flag
tar: can't open 'ORME.flag': Permission denied
tar: error exit delayed from previous errors
```

Didn't work this time, since `ORME.flag` has `0000` permissions.

We can't use `chmod`, since there's no such binary. [This StackOverflow Question](https://unix.stackexchange.com/questions/83862/how-to-chmod-without-usr-bin-chmod) included some suggestion regarding how to `chmod` without `chmod`. 

One suggestion was to use `busybox`, however it was blocked:
```console
> busybox
busybox can not be called for alien reasons.
```

Another was to use `install`, but it wasn't able to read the file in order to `chmod` it:

```console
> install -m +r ORME.flag /tmp/flag
install: can't open 'ORME.flag': Permission denied
```

Going over `/bin` to find other programs that might be helpful, `setpriv` caught my eye. I was hoping it would allow changing permissions, but instead it is used to `run a program with different Linux privilege settings`. However, it did turn out to be useful since it allowed running `busybox` for some reason:

```console
> setpriv busybox
BusyBox v1.29.3 (2019-01-24 07:45:07 UTC) multi-call binary.
BusyBox is copyrighted by many authors between 1998-2015.
Licensed under GPLv2. See source distribution for detailed
copyright notices.

Usage: busybox [function [arguments]...]
   or: busybox --list[-full]
   or: busybox --install [-s] [DIR]
   or: function [arguments]...

        BusyBox is a multi-call binary that combines many common Unix
        utilities into a single executable.  Most people will create a
        link to busybox for each function they wish to use and BusyBox
        will act like whatever it was invoked as.
```

So now we can use all the functionality that `busybox` offers:

```console
> setpriv busybox cat README.flag
CTF{4ll_D474_5h4ll_B3_Fr33}
> setpriv busybox cat ORME.flag
cat: can't open 'ORME.flag': Permission denied
> setpriv busybox chmod +r ORME.flag
> setpriv busybox cat ORME.flag
CTF{Th3r3_1s_4lw4y5_4N07h3r_W4y}
```