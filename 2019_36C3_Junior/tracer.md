# tracer
Forensics, Easy

## Description

> Tracing the Kuchenblech-Mafia is hard!

A text file was attached.

## Solution

Let's check the attached file:

```console
root@kali:/media/sf_CTFs/36c3/tracer# cat chal2-98f6917950f95448890949f2d9b9850a.txt | head
264   execve("/bin/bash", ["/bin/bash"], 0x7ffdebc8fb30 /* 8 vars */) = 0
264   read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\220\311\0\0\0\0\0\0"..., 832) = 832
264   read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P\16\0\0\0\0\0\0"..., 832) = 832
264   read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\260\34\2\0\0\0\0\0"..., 832) = 832
264   read(3, "# /etc/nsswitch.conf\n#\n# Example"..., 4096) = 497
264   read(3, "", 4096)                 = 0
264   read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\240\22\0\0\0\0\0\0"..., 832) = 832
264   read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0p \0\0\0\0\0\0"..., 832) = 832
264   read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\220@\0\0\0\0\0\0"..., 832) = 832
264   read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P#\0\0\0\0\0\0"..., 832) = 832
root@kali:/media/sf_CTFs/36c3/tracer# cat chal2-98f6917950f95448890949f2d9b9850a.txt | tail
541   write(1, "\33[?2004l\33[?1l\33>", 15) = 15
541   write(1, "\33[?25h\33[?1049l\33[23;0;0t", 23) = 23
541   +++ exited with 0 +++
264   --- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=541, si_uid=0, si_status=0, si_utime=14, si_stime=5} ---
264   write(2, "\33]0;root@c28a269da35a: /\7root@c2"..., 46) = 46
264   read(0, "\4", 1)                  = 1
264   write(2, "exit\n", 5)             = 5
264   write(3, "apt install vim\nvim Flag\n", 25) = 25
264   read(3, "apt install vim\nvim Flag\n", 25) = 25
264   +++ exited with 0 +++
```

It looks like the output of `strace`, mainly describing the process of installing `vim` via `apt install` and using it to open a file named `Flag`.

The trace itself is 66047 lines long, but up to line ~65600 the output is only related to the installation itself.

On line 65624 we have:
```
541   execve("/usr/bin/vim", ["vim", "Flag"], 0x5592eb5cdef0 /* 8 vars */) = 0
```

This is where `vim` is requested to open the `Flag` file.

Slowly scanning through the rest of the file, we eventually meet the following section:
```
541   read(0, "j", 4096)                = 1
541   write(3, "b0VIM 8.0\0\0\0\0\20\0\0\0\0\0\0\0\0\0\0\35\2\0\0root"..., 4096) = 4096
541   write(1, "\33[?25lj\33[53;174H1,2\33[1;2H\33[?25h", 31) = 31
541   read(0, "\r", 4096)               = 1
541   write(1, "\33[?25l\33[2;1H\33[K\33[53;174H2,1\33[2;1"..., 39) = 39
541   read(0, "\r", 4096)               = 1
541   write(1, "\33[?25l\33[3;1H\33[K\33[53;174H3\33[3;1H\33"..., 37) = 37
541   read(0, "\r", 4096)               = 1
541   write(1, "\33[?25l\33[4;1H\33[K\33[53;174H4\33[4;1H\33"..., 37) = 37
541   read(0, "u", 4096)                = 1
541   write(1, "\33[?25lu\33[53;176H2\33[4;2H\33[?25h", 29) = 29
541   read(0, "n", 4096)                = 1
541   write(1, "\33[?25ln\33[53;176H3\33[4;3H\33[?25h", 29) = 29
541   read(0, "\r", 4096)               = 1
541   write(1, "\33[?25l\33[5;1H\33[K\33[53;174H5,1\33[5;1"..., 39) = 39
541   read(0, "\r", 4096)               = 1
541   write(1, "\33[?25l\33[6;1H\33[K\33[53;174H6\33[6;1H\33"..., 37) = 37
541   read(0, "i", 4096)                = 1
541   write(1, "\33[?25li\33[53;176H2\33[6;2H\33[?25h", 29) = 29
541   read(0, "o", 4096)                = 1
541   write(1, "\33[?25lo\33[53;176H3\33[6;3H\33[?25h", 29) = 29
541   read(0, "\r", 4096)               = 1
541   write(1, "\33[?25l\33[7;1H\33[K\33[53;174H7,1\33[7;1"..., 39) = 39
541   read(0, "r", 4096)                = 1
541   write(1, "\33[?25lr\33[53;176H2\33[7;2H\33[?25h", 29) = 29
```

Ignoring the `\r` inputs, we can identify `junior` which is the flag format.

Let's extract all the `read`s from this section:

```console
root@kali:/media/sf_CTFs/36c3/tracer# cat chal2-98f6917950f95448890949f2d9b9850a.txt | tail -n +65782 | grep read
541   read(0, "i", 4096)                = 1
541   read(0, "j", 4096)                = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "u", 4096)                = 1
541   read(0, "n", 4096)                = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "i", 4096)                = 1
541   read(0, "o", 4096)                = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "r", 4096)                = 1
541   read(0, "-", 4096)                = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "n", 4096)                = 1
541   read(0, "a", 4096)                = 1
541   read(0, "\33", 4096)              = 1
541   read(0, "y", 4096)                = 1
541   read(0, "y", 4096)                = 1
541   read(0, "p", 4096)                = 1
541   read(0, "A", 4096)                = 1
541   read(0, "\177", 4096)             = 1
541   read(0, "o", 4096)                = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "i", 4096)                = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "s", 4096)                = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "w", 4096)                = 1
541   read(0, "a", 4096)                = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "y", 4096)                = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "b", 4096)                = 1
541   read(0, "e", 4096)                = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "t", 4096)                = 1
541   read(0, "t", 4096)                = 1
541   read(0, "e", 4096)                = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "r", 4096)                = 1
541   read(0, "!", 4096)                = 1
541   read(0, "\33", 4096)              = 1
541   read(0, "g", 4096)                = 1
541   read(0, "g", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "J", 4096)                = 1
541   read(0, "b", 4096)                = 1
541   read(0, "b", 4096)                = 1
541   read(0, "~", 4096)                = 1
541   read(0, "~", 4096)                = 1
541   read(0, "~", 4096)                = 1
541   read(0, "~", 4096)                = 1
541   read(0, "~", 4096)                = 1
541   read(0, "~", 4096)                = 1
541   read(0, "~", 4096)                = 1
541   read(0, "~", 4096)                = 1
541   read(0, "\33", 4096)              = 1
541   read(0, ":", 4096)                = 1
541   read(0, "s", 4096)                = 1
541   read(0, "/", 4096)                = 1
541   read(0, " ", 4096)                = 1
541   read(0, "/", 4096)                = 1
541   read(0, "/", 4096)                = 1
541   read(0, "g", 4096)                = 1
541   read(0, "\r", 4096)               = 1
541   read(0, "\33", 4096)              = 1
541   read(0, ":", 4096)                = 1
541   read(0, "w", 4096)                = 1
541   read(0, "q", 4096)                = 1
541   read(0, "\r", 4096)               = 1
264   read(0, "\4", 1)                  = 1
264   read(3, "apt install vim\nvim Flag\n", 25) = 25
```

Let's clean it up a bit:
```
root@kali:/media/sf_CTFs/36c3/tracer# cat chal2-98f6917950f95448890949f2d9b9850a.txt | tail -n +65782 | grep read | awk '{printf $3}' | tr -d '",' | sed 's/\\r/\\r\n/g'
ij\r
\r
\r
un\r
\r
io\r
r-\r
na\33yypA\177o\r
\r
i\r
s\r
\r
wa\r
y\r
be\r
tte\r
\r
r!\33ggJJJJJJJJJJJJJJJJJJbb~~~~~~~~\33:s///g\r
\33:wq\r
\4apt
```

If it wasn't clear until now, the `:wq` is a dead giveaway: We need to open `vim` and re-enter this input in order to get the flag.

The only remaining question is what to type for `\33` and for `\177`.  It turns out that these are octal representations of ASCII characters:
```console
root@kali:/media/sf_CTFs/36c3/tracer# man ascii | grep 033
       033   27    1B    ESC (escape)                133   91    5B    [
root@kali:/media/sf_CTFs/36c3/tracer# man ascii | grep 177
       077   63    3F    ?                           177   127   7F    DEL
```

This means that `\33` is `ESC` and `\177` is `DEL`.

The flag is: `junior-nanoiswayBETTER!`. Indeed.
