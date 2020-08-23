# Beginner
Category: Reversing

> Dust off the cobwebs, let's reverse!

An executable was attached.

## Solution

Let's run the file:

```console
root@kali:/media/sf_CTFs/google/beginner# ./a.out
Flag: test
FAILURE
```

Looks like a simple program where we need to enter the flag.

Since `grep`, `strace` and `ptrace` don't show the flag in plaintext, let's take a look at the disassembly using Ghidra:

```c
ulong main(void)

{
  int iVar1;
  uint uVar2;
  undefined auVar3 [16];
  undefined user_input [16];
  undefined4 local_28;
  undefined4 uStack36;
  undefined4 uStack32;
  undefined4 uStack28;
  
  printf("Flag: ");
  __isoc99_scanf(&DAT_0010200b,user_input);
  auVar3 = pshufb(user_input,SHUFFLE);
  auVar3 = CONCAT412(SUB164(auVar3 >> 0x60,0) + ADD32._12_4_,
                     CONCAT48(SUB164(auVar3 >> 0x40,0) + ADD32._8_4_,
                              CONCAT44(SUB164(auVar3 >> 0x20,0) + ADD32._4_4_,
                                       SUB164(auVar3,0) + ADD32._0_4_))) ^ XOR;
  local_28 = SUB164(auVar3,0);
  uStack36 = SUB164(auVar3 >> 0x20,0);
  uStack32 = SUB164(XOR >> 0x40,0);
  uStack28 = SUB164(XOR >> 0x60,0);
  iVar1 = strncmp(user_input,(char *)&local_28,0x10);
  if (iVar1 == 0) {
    uVar2 = strncmp((char *)&local_28,EXPECTED_PREFIX,4); // EXPECTED_PREFIX = "CTF{"
    if (uVar2 == 0) {
      puts("SUCCESS");
      goto LAB_00101112;
    }
  }
  uVar2 = 1;
  puts("FAILURE");
LAB_00101112:
  return (ulong)uVar2;
}
```

Looks simple enough, except for the part where the user input is scrambled with `pshufb`, concatenations, XORs, shifts and what not.

So, instead of trying to understand all that, let's solve this with `angr`: 

> angr is a python framework for analyzing binaries. It combines both static and dynamic symbolic ("concolic") analysis, making it applicable to a variety of tasks.

Basically, we create a Python program that uses `angr` to explore the different paths that the program can take, and ask it to find us a path that will take us to a *good* location: Printing "SUCCESS".

The address we want to arrive to is:
```assembly
                             LAB_0010111d                                    XREF[1]:     001010fe(j)  
        0010111d 48 8d 3d        LEA        RDI,[s_SUCCESS_00102010]                         = "SUCCESS"
                 ec 0e 00 00
        00101124 e8 17 ff        CALL       puts                                             int puts(char * __s)
                 ff ff
```

The address we want to avoid it:
```assembly
                             LAB_00101100                                    XREF[1]:     001010e3(j)  
        00101100 48 8d 3d        LEA        RDI,[s_FAILURE_00102018]                         = "FAILURE"
                 11 0f 00 00
        00101107 41 bc 01        MOV        R12D,0x1
                 00 00 00
        0010110d e8 2e ff        CALL       puts                                             int puts(char * __s)
                 ff ff

```

Apart from that, we just need to know the flag length. From the buffer sizes and the `strncmp`, it looks like 15 (plus one extra character for the NULL terminator).

Note that this is an ASLR-enabled binary, and Ghidra gives it a base address of 0x100000. So, we align our angr script to the same base address.

Here's the script:
```python
import angr
import claripy

FLAG_LEN = 15
STDIN_FD = 0

base_addr = 0x100000 # To match addresses to Ghidra

proj = angr.Project("./a.out", main_opts={'base_addr': base_addr}) 

flag_chars = [claripy.BVS('flag_%d' % i, 8) for i in range(FLAG_LEN)]
flag = claripy.Concat( *flag_chars + [claripy.BVV(b'\n')]) # Add \n for scanf() to accept the input

state = proj.factory.full_init_state(
        args=['./a.out'],
        add_options=angr.options.unicorn,
        stdin=flag,
)

# Add constraints that all characters are printable
for k in flag_chars:
    state.solver.add(k >= ord('!'))
    state.solver.add(k <= ord('~'))

simgr = proj.factory.simulation_manager(state)
find_addr  = 0x101124 # SUCCESS
avoid_addr = 0x10110d # FAILURE
simgr.explore(find=find_addr, avoid=avoid_addr)

if (len(simgr.found) > 0):
    for found in simgr.found:
        print(found.posix.dumps(STDIN_FD))
```

We run the script and get the flag within a few seconds:
```console
(angr) root@kali:/media/sf_CTFs/google/beginner# python3 solve.py
WARNING | 2020-08-23 15:13:00,365 | angr.state_plugins.symbolic_memory | The program is accessing memory or registers with an unspecified value. This could indicate unwanted behavior.
WARNING | 2020-08-23 15:13:00,366 | angr.state_plugins.symbolic_memory | angr will cope with this by generating an unconstrained symbolic variable and continuing. You can resolve this by:
WARNING | 2020-08-23 15:13:00,366 | angr.state_plugins.symbolic_memory | 1) setting a value to the initial state
WARNING | 2020-08-23 15:13:00,367 | angr.state_plugins.symbolic_memory | 2) adding the state option ZERO_FILL_UNCONSTRAINED_{MEMORY,REGISTERS}, to make unknown regions hold null
WARNING | 2020-08-23 15:13:00,367 | angr.state_plugins.symbolic_memory | 3) adding the state option SYMBOL_FILL_UNCONSTRAINED_{MEMORY_REGISTERS}, to suppress these messages.
WARNING | 2020-08-23 15:13:00,368 | angr.state_plugins.symbolic_memory | Filling memory at 0x7fffffffffefff8 with 1 unconstrained bytes referenced from 0x299d80 (explicit_bzero+0x8c40 in libc.so.6 (0x99d80))
WARNING | 2020-08-23 15:13:00,425 | angr.state_plugins.symbolic_memory | Filling memory at 0x7fffffffffefff9 with 7 unconstrained bytes referenced from 0x299dad (explicit_bzero+0x8c6d in libc.so.6 (0x99dad))
WARNING | 2020-08-23 15:13:05,064 | angr.state_plugins.symbolic_memory | Filling memory at 0x7ffffffffff0000 with 48 unconstrained bytes referenced from 0x28a7f0 (strncmp+0x0 in libc.so.6 (0x8a7f0))
WARNING | 2020-08-23 15:13:05,086 | angr.state_plugins.symbolic_memory | Filling memory at 0x7ffffffffff0030 with 16 unconstrained bytes referenced from 0x28a7f0 (strncmp+0x0 in libc.so.6 (0x8a7f0))
b'CTF{S1MDf0rM3!}\n'
```