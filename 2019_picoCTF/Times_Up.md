# Time's Up
Reverse Engineering, 400 points

## Description:
> Time waits for no one. Can you solve this before time runs out?


## Solution: 

Let's run the attached file:

```console
root@kali:/media/sf_CTFs/pico/Times_Up# ./times-up
Challenge: (((((185787775) + (-952261277)) + ((-46657828) + (837965729))) - (((1568696032) + (1659000306)) + ((-603020797) + (1768531904)))) - ((((-1716398977) + (-1336981012)) + ((169639072) + (1679069168))) - (((967152949) + (541599975)) + ((1236322328) - (799424436)))))
Setting alarm...
Solution? Alarm clock
```

We got a mathematical expression, and shortly after, the program got killed by an alarm. How long is the alarm? Let's use `strace` to see:
```console
root@kali:/media/sf_CTFs/pico/Times_Up# ./times-up
Challenge: (((((185787775) + (-952261277)) + ((-46657828) + (837965729))) - (((1568696032) + (1659000306)) + ((-603020797) + (1768531904)))) - ((((-1716398977) + (-1336981012)) + ((169639072) + (1679069168))) - (((967152949) + (541599975)) + ((1236322328) - (799424436)))))
Setting alarm...
Solution? Alarm clock
root@kali:/media/sf_CTFs/pico/Times_Up# strace ./times-up
execve("./times-up", ["./times-up"], 0x7ffc13ffff80 /* 22 vars */) = 0
brk(NULL)                               = 0x5569a8f2d000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=138446, ...}) = 0
mmap(NULL, 138446, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fe7a2030000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\260A\2\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0755, st_size=1824496, ...}) = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fe7a202e000
mmap(NULL, 1837056, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fe7a1e6d000
mprotect(0x7fe7a1e8f000, 1658880, PROT_NONE) = 0
mmap(0x7fe7a1e8f000, 1343488, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x22000) = 0x7fe7a1e8f000
mmap(0x7fe7a1fd7000, 311296, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x16a000) = 0x7fe7a1fd7000
mmap(0x7fe7a2024000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1b6000) = 0x7fe7a2024000
mmap(0x7fe7a202a000, 14336, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fe7a202a000
close(3)                                = 0
arch_prctl(ARCH_SET_FS, 0x7fe7a202f540) = 0
mprotect(0x7fe7a2024000, 16384, PROT_READ) = 0
mprotect(0x5569a8f1b000, 4096, PROT_READ) = 0
mprotect(0x7fe7a2079000, 4096, PROT_READ) = 0
munmap(0x7fe7a2030000, 138446)          = 0
rt_sigaction(SIGALRM, {sa_handler=SIG_DFL, sa_mask=[ALRM], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7fe7a1ea4940}, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
fstat(1, {st_mode=S_IFCHR|0600, st_rdev=makedev(136, 0), ...}) = 0
brk(NULL)                               = 0x5569a8f2d000
brk(0x5569a8f4e000)                     = 0x5569a8f4e000
write(1, "Challenge: (((((796509316) + (-1"..., 281Challenge: (((((796509316) + (-1598581952)) + ((329558096) + (-1452703832))) + (((721761900) + (-205391708)) - ((1948475778) + (-1428472112)))) + ((((-466274084) + (865555127)) + ((-487887526) + (-2106753504))) - (((-1742981120) + (692410312)) + ((-2046178240) + (-1815708775)))))
) = 281
write(1, "Setting alarm...\n", 17Setting alarm...
)      = 17
setitimer(ITIMER_REAL, {it_interval={tv_sec=0, tv_usec=0}, it_value={tv_sec=0, tv_usec=5000}}, {it_interval={tv_sec=0, tv_usec=0}, it_value={tv_sec=0, tv_usec=0}}) = 0
fstat(0, {st_mode=S_IFCHR|0600, st_rdev=makedev(136, 0), ...}) = 0
write(1, "Solution? ", 10Solution? )              = 10
read(0, 0x5569a8f2d670, 1024)           = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGALRM {si_signo=SIGALRM, si_code=SI_KERNEL} ---
+++ killed by SIGALRM +++
Alarm clock
```

We can see that the alarm is set for 5000 uSeconds - not much. In this time, we must evaluate the equation and input the answer back to the program.

We can evaluate the equation using `bc`:

```console
root@kali:/media/sf_CTFs/pico/Times_Up# equation="Challenge: (((((185787775) + (-952261277)) + ((-46657828) + (837965729))) - (((1568696032) + (1659000306)) + ((-603020797) + (1768531904)))) - ((((-1716398977) + (-1336981012)) + ((169639072) + (1679069168))) - (((967152949) + (541599975)) + ((1236322328) - (799424436)))))"
root@kali:/media/sf_CTFs/pico/Times_Up# echo $equation
Challenge: (((((185787775) + (-952261277)) + ((-46657828) + (837965729))) - (((1568696032) + (1659000306)) + ((-603020797) + (1768531904)))) - ((((-1716398977) + (-1336981012)) + ((169639072) + (1679069168))) - (((967152949) + (541599975)) + ((1236322328) - (799424436)))))
root@kali:/media/sf_CTFs/pico/Times_Up# echo ${equation:11}
(((((185787775) + (-952261277)) + ((-46657828) + (837965729))) - (((1568696032) + (1659000306)) + ((-603020797) + (1768531904)))) - ((((-1716398977) + (-1336981012)) + ((169639072) + (1679069168))) - (((967152949) + (541599975)) + ((1236322328) - (799424436)))))
root@kali:/media/sf_CTFs/pico/Times_Up# echo ${equation:11} | bc
-1218050481
```

Let's use `coproc` to communicate with the program - we'll read a line from the program's `stdout`, send it to `bc`, and write the result to the program's `stdin`.

Here's the script:
```bash
#!/bin/bash
coproc ./times-up
read LINE <&${COPROC[0]}
echo $(echo ${LINE:11} | bc) >&${COPROC[1]}
cat <&${COPROC[0]}
```


Running it on the server:
```console
dvdalt@pico-2019-shell1:~$ cd /problems/time-s-up_6_480d53541469436212e30dad5b4ce691
dvdalt@pico-2019-shell1:/problems/time-s-up_6_480d53541469436212e30dad5b4ce691$ coproc ./times-up
[1] 1921638
dvdalt@pico-2019-shell1:/problems/time-s-up_6_480d53541469436212e30dad5b4ce691$ read LINE <&${COPROC[0]}
dvdalt@pico-2019-shell1:/problems/time-s-up_6_480d53541469436212e30dad5b4ce691$ echo $(echo ${LINE:11} | bc) >&${COPROC[1]}
dvdalt@pico-2019-shell1:/problems/time-s-up_6_480d53541469436212e30dad5b4ce691$ cat <&${COPROC[0]}
Setting alarm...
picoCTF{Gotta go fast. Gotta go FAST. #1626a7fb}
Solution? Congrats! Here is the flag!
[1]+  Done                    coproc COPROC ./times-up
```
