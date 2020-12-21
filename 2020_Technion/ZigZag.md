# ZigZag
Category: Reversing & Binary Exploitation

## Description
> Some binaries are zigzag


A binary file was attached.

## Solution

Let's run `strings` on the file:

```console
root@kali:/media/sf_CTFs/technion/ZigZag# strings ./zigzag | head -n 40
/lib64/ld-linux-x86-64.so.2
libc.so.6
__cxa_finalize
__libc_start_main
GLIBC_2.2.5
_ITM_deregisterTMCloneTable
__gmon_start__
_ITM_registerTMCloneTable
u+UH
[]A\A]A^A_
c############
#s###########
##t##########
###e#########
####c########
#####h#######
######n######
########i####
#########o###
##########n##
############{
###########4#
##########d##
#########V###
########3####
#######n#####
######c######
#####3#######
####d########
###_#########
##s##########
#7###########
r############
#i###########
##N##########
###g#########
####s########
:*3$"
GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
crtstuff.c
```

Let's print the string with a bash snippet:
```console
root@kali:/media/sf_CTFs/technion/ZigZag# strings ./zigzag | grep "#" | while read line; do for (( i=0; i<${#line}; i++ )); do char=${line:$i:1}; if [ "$char" != "#" ]; then echo -n "$char"; fi; done; done && echo "}"
cstechnion{4dV3nc3d_s7riNgs}
```