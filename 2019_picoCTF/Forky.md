# Forky
Reverse Engineering, 500 points

## Description:
> In this program, identify the last integer value that is passed as parameter to the function doNothing(). 

## Solution: 

Let's check the binary we've received:

```console
root@kali:/media/sf_CTFs/pico/Forky# file ./vuln
./vuln: ELF 32-bit LSB pie executable Intel 80386, version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux.so.2, for GNU/Linux 3.2.0, BuildID[sha1]=07ba8a5f157f85c5da80ebf5bd64b33cc60e98b6, not stripped
root@kali:/media/sf_CTFs/pico/Forky# ./vuln
root@kali:/media/sf_CTFs/pico/Forky#
```

Doesn't do much. The Ghidra decompiler shows the following code:

```c
undefined4 main(undefined1 param_1)
{
  int *piVar1;
  
  piVar1 = (int *)mmap((void *)0x0,4,3,0x21,-1,0);
  *piVar1 = 1000000000;
  fork();
  fork();
  fork();
  fork();
  *piVar1 = *piVar1 + 0x499602d2;
  doNothing(*piVar1);
  return 0;
}

void doNothing(undefined4 param_1)
{
  __x86.get_pc_thunk.ax();
  return;
}

```

So this program recursively forks itself and calls `doNothing`. We need to identify last integer value that is passed as parameter to `doNothing()`.

If this question isn't timing-dependent, I don't understand why. In any case, I started by trying to utilize GDB to provide the answer, with the following command:

```console
gdb ./vuln -ex 'set follow-fork-mode child' -ex 'dprintf *doNothing+19, "%d\n", $eax' -ex 'r' -ex 'q'
```

When `fork` is called in a program and a child process is created, GDB must choose which process it should debug. The default is to continue debugging the parent, but there's a setting to request GDB to attach to the child instead: `set follow-fork-mode child`.

After requesting GDB to attach to the child upon every fork, the command above uses `dprintf` to print `eax` at the following location:
```assembly
gdb-peda$ disas doNothing
Dump of assembler code for function doNothing:
   0x0000054d <+0>:     push   ebp
   0x0000054e <+1>:     mov    ebp,esp
   0x00000550 <+3>:     sub    esp,0x10
   0x00000553 <+6>:     call   0x5fa <__x86.get_pc_thunk.ax>
   0x00000558 <+11>:    add    eax,0x1a7c
   0x0000055d <+16>:    mov    eax,DWORD PTR [ebp+0x8]
   0x00000560 <+19>:    mov    DWORD PTR [ebp-0x4],eax <----------------- eax will be printed here
   0x00000563 <+22>:    nop
   0x00000564 <+23>:    leave
   0x00000565 <+24>:    ret
End of assembler dump.
```

The answer I received is `1104081276`, which wasn't accepted by the server.

Instead of a runtime solution, let's try to calculate what should happen. The first process creates 4 children. The first child creates 3 children, the second 2 children, the third 1 child and the forth 0 children. This continues recursively. This means that we'll have 16 processes altogether. Here's a diagram that might help visualize this:

```
 +                                                                     
 |                                                                     
 +-----------------------------------+                                 
 |                                   |                                 
 +-----------------+                 +-----------------+               
 |                 |                 |                 |               
 +--------+        +--------+        +--------+        +--------+      
 |        |        |        |        |        |        |        |      
 +---+    +---+    +---+    +---+    +---+    +---+    +---+    +---+  
 |   |    |   |    |   |    |   |    |   |    |   |    |   |    |   |  
 O   O    O   O    O   O    O   O    O   O    O   O    O   O    O   O  
```

Every `O` is a process, where the leftmost process is the parent which forks 4 children and so on.

We can double check this calculation using `ltrace -f`:

> -f: Trace child processes as they are created by currently traced processes as a result of the fork(2) or clone(2) system calls. The new process is attached immediately.

Output:

```console
root@kali:/media/sf_CTFs/pico/Forky# ltrace -f ./vuln 2>&1 | grep "+++ exited"
[pid 3877] +++ exited (status 0) +++
[pid 3881] +++ exited (status 0) +++
[pid 3880] +++ exited (status 0) +++
[pid 3882] +++ exited (status 0) +++
[pid 3879] +++ exited (status 0) +++
[pid 3883] +++ exited (status 0) +++
[pid 3884] +++ exited (status 0) +++
[pid 3886] +++ exited (status 0) +++
[pid 3887] +++ exited (status 0) +++
[pid 3888] +++ exited (status 0) +++
[pid 3891] +++ exited (status 0) +++
[pid 3890] +++ exited (status 0) +++
[pid 3889] +++ exited (status 0) +++
[pid 3885] +++ exited (status 0) +++
[pid 3878] +++ exited (status 0) +++
[pid 3892] +++ exited (status 0) +++
root@kali:/media/sf_CTFs/pico/Forky# ltrace -f ./vuln 2>&1 | grep "+++ exited" | wc -l
16
```

So, if we understand correctly what the question is asking from us, we need to calculate `1000000000 + (16 * 0x499602d2)`, taking into consideration integer overflow and sign:

```python
>>> import numpy
>>> base = numpy.int32(1000000000)
>>> step = numpy.int32(0x499602d2)
>>> base + 16*step
__main__:1: RuntimeWarning: overflow encountered in long_scalars
-721750240
```

Indeed, `picoCTF{-721750240}` was accepted as an answer.

