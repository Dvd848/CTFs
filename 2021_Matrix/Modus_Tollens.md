# Modus Tollens
Category: Reversing, 100 Points

## Description
> Remember your training,
> 
> keep calm and carry on.
> 
> 

A binary file was attached.

## Solution

Let's run the attached binary:

```console
root@kali:/media/sf_CTFs/matrix/Modus_Tollens# ./modus_tollens
Enter flag: test
Checking ...
Wrong! try again...
```

So we need to enter a flag. After checking if the flag is saved in plaintext as a string, and running `strace` in hope of some `memcmp`, we must open the program in a disassembler.

Our main function looks like this:
```c
undefined8 main(void)

{
  size_t sVar1;
  ulong uVar2;
  long in_FS_OFFSET;
  undefined8 uStack160;
  byte user_input [128];
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  uStack160 = 0x101309;
  printf("Enter flag: ");
  uStack160 = 0x101324;
  fgets((char *)user_input,128,stdin);
  uStack160 = 0x101333;
  sVar1 = strlen((char *)user_input);
  user_input[sVar1 - 1] = 0;
  uStack160 = 0x101350;
  printf("Checking ");
  uStack160 = 0x10135f;
  fflush(stdout);
  uStack160 = 0x101369;
  sleep(1);
  uStack160 = 0x101373;
  putchar(0x2e);
  uStack160 = 0x101382;
  fflush(stdout);
  uStack160 = 0x10138c;
  sleep(1);
  uStack160 = 0x101396;
  putchar(0x2e);
  uStack160 = 0x1013a5;
  fflush(stdout);
  uStack160 = 0x1013af;
  sleep(1);
  uStack160 = 0x1013bb;
  puts(".");
  uStack160 = 0x1013ca;
  fflush(stdout);
  uStack160 = 0x1013d4;
  sleep(1);
  uStack160 = 0x1013e7;
  uVar2 = f2(user_input[32],0xf);
  if ((int)uVar2 == 0) {
    uStack160 = 0x101402;
    uVar2 = f4(user_input[27],6);
    if ((int)uVar2 == 0) {
      uStack160 = 0x101420;
      uVar2 = f2(user_input[11],5);
      if ((int)uVar2 == 0) {
        uStack160 = 0x10143e;
        uVar2 = f2(user_input[6],0xd);
        if ((int)uVar2 == 0) {
          uStack160 = 0x101459;
          uVar2 = f1(user_input[31],2);
          if ((int)uVar2 == 0) {
            uStack160 = 0x101474;
            uVar2 = f4(user_input[19],0xc);
            if ((int)uVar2 == 0) {
              uStack160 = 0x10148f;
              uVar2 = f2(user_input[41],0);
              if ((int)uVar2 == 0) {
                uStack160 = 0x1014aa;
                uVar2 = f1(user_input[16],3);
                if ((int)uVar2 == 0) {
                  uStack160 = 0x1014c5;
                  uVar2 = f2(user_input[24],0xc);
                  if ((int)uVar2 == 0) {
                    uStack160 = 0x1014e0;
                    uVar2 = f2(user_input[42],0xf);
                    if ((int)uVar2 == 0) {
                      uStack160 = 0x1014fe;
                      uVar2 = f1(user_input[10],6);
                      if ((int)uVar2 == 0) {
                        uStack160 = 0x101519;
                        uVar2 = f3(user_input[33],9);
                        if ((int)uVar2 == 0) {
                          uStack160 = 0x101537;
                          uVar2 = f2(user_input[10],0xd);
                          if ((int)uVar2 == 0) {
                            uStack160 = 0x101552;
                            uVar2 = f3(user_input[21],0xb);
                            if ((int)uVar2 == 0) {
                              uStack160 = 0x10156d;
                              uVar2 = f1(user_input[28],7);
                              if ((int)uVar2 == 0) {
                                uStack160 = 0x10158b;
                                uVar2 = f3(user_input[11],9);
                                if ((int)uVar2 == 0) {
                                  uStack160 = 0x1015a6;
                                  uVar2 = f1(user_input[34],6);
                                  if ((int)uVar2 == 0) {
                                    uStack160 = 0x1015c1;
                                    uVar2 = f4(user_input[16],0xb);
                                    if ((int)uVar2 == 0) {
                                      uStack160 = 0x1015dc;
                                      uVar2 = f2(user_input[31],4);
                                      if ((int)uVar2 == 0) {
                                        uStack160 = 0x1015fa;
                                        uVar2 = f4(user_input[5],0xf);
                                        if ((int)uVar2 == 0) {
                                          uStack160 = 0x101615;
                                          uVar2 = f2(user_input[17],4);
                                          if ((int)uVar2 == 0) {
                                            uStack160 = 0x101630;
                                            uVar2 = f1(user_input[19],6);
                                            if ((int)uVar2 == 0) {
                                              uStack160 = 0x10164b;
                                              uVar2 = f1(user_input[35],3);
                                              if ((int)uVar2 == 0) {
                                                uStack160 = 0x101669;
                                                uVar2 = f3(user_input[5],0xc);
                                                if ((int)uVar2 == 0) {
                                                  uStack160 = 0x101684;
                                                  uVar2 = f2(user_input[43],0xd);
                                                  if ((int)uVar2 == 0) {
                                                    uStack160 = 0x10169f;
                                                    uVar2 = f3(user_input[25],8);
                                                    if ((int)uVar2 == 0) {
                                                      uStack160 = 0x1016ba;
                                                      uVar2 = f4(user_input[30],6);
                                                      if ((int)uVar2 == 0) {
                                                        uStack160 = 0x1016d5;
                                                        uVar2 = f3(user_input[27],9);
                                                        if ((int)uVar2 == 0) {
                                                          uStack160 = 0x1016f0;
                                                          uVar2 = f2(user_input[22],0xe);
                                                          if ((int)uVar2 == 0) {
                                                            uStack160 = 0x10170b;
                                                            uVar2 = f1(user_input[29],5);
                                                            if ((int)uVar2 == 0) {
                                                              uStack160 = 0x101726;
                                                              uVar2 = f2(user_input[29],0xf);
                                                              if ((int)uVar2 == 0) {
                                                                uStack160 = 0x101744;
                                                                uVar2 = f3(user_input[6],9);
                                                                if ((int)uVar2 == 0) {
                                                                  uStack160 = 0x101762;
                                                                  uVar2 = f2(user_input[15],4);
                                                                  if ((int)uVar2 == 0) {
                                                                    uStack160 = 0x101780;
                                                                    uVar2 = f2(user_input[3],0xb);
                                                                    if ((int)uVar2 == 0) {
                                                                      uStack160 = 0x10179b;
                                                                      uVar2 = f1(user_input[39],5);
                                                                      if ((int)uVar2 == 0) {
                                                                        uStack160 = 0x1017b9;
                                                                        uVar2 = f2(user_input[9],1);
                                                                        if ((int)uVar2 == 0) {
                                                                          uStack160 = 0x1017d7;
                                                                          uVar2 = f4(user_input[1],
                                                                                     0xc);
                                                                          if ((int)uVar2 == 0) {
                                                                            uStack160 = 0x1017f2;
                                                                            uVar2 = f3(user_input
                                                                                       [42],0xc);
                                                                            if ((int)uVar2 == 0) {
                                                                              uStack160 = 0x101810;
                                                                              uVar2 = f2(user_input
                                                                                         [4],3);
                                                                              if ((int)uVar2 == 0) {
                                                                                uStack160 = 0x10182e
                                                                                ;
                                                                                uVar2 = f3(
                                                  user_input[3],8);
                                                  if ((int)uVar2 == 0) {
                                                    uStack160 = 0x10184c;
                                                    uVar2 = f3(user_input[9],0xc);
                                                    if ((int)uVar2 == 0) {
                                                      uStack160 = 0x101867;
                                                      uVar2 = f4(user_input[38],7);
                                                      if ((int)uVar2 == 0) {
                                                        uStack160 = 0x101882;
                                                        uVar2 = f3(user_input[18],9);
                                                        if ((int)uVar2 == 0) {
                                                          uStack160 = 0x1018a0;
                                                          uVar2 = f3(user_input[12],10);
                                                          if ((int)uVar2 == 0) {
                                                            uStack160 = 0x1018bb;
                                                            uVar2 = f1(user_input[36],5);
                                                            if ((int)uVar2 == 0) {
                                                              uStack160 = 0x1018d9;
                                                              uVar2 = f1(user_input[13],5);
                                                              if ((int)uVar2 == 0) {
                                                                uStack160 = 0x1018f4;
                                                                uVar2 = f3(user_input[30],9);
                                                                if ((int)uVar2 == 0) {
                                                                  uStack160 = 0x101912;
                                                                  uVar2 = f4(user_input[13],0);
                                                                  if ((int)uVar2 == 0) {
                                                                    uStack160 = 0x10192d;
                                                                    uVar2 = f4(user_input[39],0);
                                                                    if ((int)uVar2 == 0) {
                                                                      uStack160 = 0x101948;
                                                                      uVar2 = f4(user_input[20],0);
                                                                      if ((int)uVar2 == 0) {
                                                                        uStack160 = 0x101966;
                                                                        uVar2 = f1(user_input[14],5)
                                                                        ;
                                                                        if ((int)uVar2 == 0) {
                                                                          uStack160 = 0x101984;
                                                                          uVar2 = f3(user_input[15],
                                                                                     8);
                                                                          if ((int)uVar2 == 0) {
                                                                            uStack160 = 0x10199f;
                                                                            uVar2 = f3(user_input
                                                                                       [32],10);
                                                                            if ((int)uVar2 == 0) {
                                                                              uStack160 = 0x1019ba;
                                                                              uVar2 = f1(user_input
                                                                                         [26],5);
                                                                              if ((int)uVar2 == 0) {
                                                                                uStack160 = 0x1019d5
                                                                                ;
                                                                                uVar2 = f1(
                                                  user_input[43],7);
                                                  if ((int)uVar2 == 0) {
                                                    uStack160 = 0x1019f0;
                                                    uVar2 = f4(user_input[33],10);
                                                    if ((int)uVar2 == 0) {
                                                      uStack160 = 0x101a0b;
                                                      uVar2 = f1(user_input[37],6);
                                                      if ((int)uVar2 == 0) {
                                                        uStack160 = 0x101a26;
                                                        uVar2 = f3(user_input[17],8);
                                                        if ((int)uVar2 == 0) {
                                                          uStack160 = 0x101a41;
                                                          uVar2 = f2(user_input[26],3);
                                                          if ((int)uVar2 == 0) {
                                                            uStack160 = 0x101a5f;
                                                            uVar2 = f1(user_input[7],3);
                                                            if ((int)uVar2 == 0) {
                                                              uStack160 = 0x101a7a;
                                                              uVar2 = f3(user_input[38],9);
                                                              if ((int)uVar2 == 0) {
                                                                uStack160 = 0x101a95;
                                                                uVar2 = f3(user_input[40],9);
                                                                if ((int)uVar2 == 0) {
                                                                  uStack160 = 0x101ab3;
                                                                  uVar2 = f3(user_input[2],0xb);
                                                                  if ((int)uVar2 == 0) {
                                                                    uStack160 = 0x101ace;
                                                                    uVar2 = f2(user_input[35],0);
                                                                    if ((int)uVar2 == 0) {
                                                                      uStack160 = 0x101ae9;
                                                                      uVar2 = f3(user_input[22],9);
                                                                      if ((int)uVar2 == 0) {
                                                                        uStack160 = 0x101b04;
                                                                        uVar2 = f4(user_input[23],
                                                                                   0xb);
                                                                        if ((int)uVar2 == 0) {
                                                                          uStack160 = 0x101b1f;
                                                                          uVar2 = f4(user_input[18],
                                                                                     6);
                                                                          if ((int)uVar2 == 0) {
                                                                            uStack160 = 0x101b3a;
                                                                            uVar2 = f4(user_input
                                                                                       [28],0xc);
                                                                            if ((int)uVar2 == 0) {
                                                                              uStack160 = 0x101b55;
                                                                              uVar2 = f4(user_input
                                                                                         [25],6);
                                                                              if ((int)uVar2 == 0) {
                                                                                uStack160 = 0x101b70
                                                                                ;
                                                                                uVar2 = f4(
                                                  user_input[37],8);
                                                  if ((int)uVar2 == 0) {
                                                    uStack160 = 0x101b8e;
                                                    uVar2 = f1(user_input[1],4);
                                                    if ((int)uVar2 == 0) {
                                                      uStack160 = 0x101bac;
                                                      uVar2 = f3(user_input[4],10);
                                                      if ((int)uVar2 == 0) {
                                                        uStack160 = 0x101bca;
                                                        uVar2 = f4(user_input[14],0xc);
                                                        if ((int)uVar2 == 0) {
                                                          uStack160 = 0x101be5;
                                                          uVar2 = f3(user_input[20],10);
                                                          if ((int)uVar2 == 0) {
                                                            uStack160 = 0x101c03;
                                                            uVar2 = f3(user_input[8],10);
                                                            if ((int)uVar2 == 0) {
                                                              uStack160 = 0x101c21;
                                                              uVar2 = f2(user_input[7],3);
                                                              if ((int)uVar2 == 0) {
                                                                uStack160 = 0x101c3c;
                                                                uVar2 = f4(user_input[40],1);
                                                                if ((int)uVar2 == 0) {
                                                                  uStack160 = 0x101c5a;
                                                                  uVar2 = f4(user_input[0],2);
                                                                  if ((int)uVar2 == 0) {
                                                                    uStack160 = 0x101c75;
                                                                    uVar2 = f3(user_input[24],9);
                                                                    if ((int)uVar2 == 0) {
                                                                      uStack160 = 0x101c90;
                                                                      uVar2 = f1(user_input[41],3);
                                                                      if ((int)uVar2 == 0) {
                                                                        uStack160 = 0x101cab;
                                                                        uVar2 = f2(user_input[36],5)
                                                                        ;
                                                                        if ((int)uVar2 == 0) {
                                                                          uStack160 = 0x101cc6;
                                                                          uVar2 = f2(user_input[21],
                                                                                     1);
                                                                          if ((int)uVar2 == 0) {
                                                                            uStack160 = 0x101ce4;
                                                                            uVar2 = f4(user_input[2]
                                                                                       ,3);
                                                                            if ((int)uVar2 == 0) {
                                                                              uStack160 = 0x101cff;
                                                                              uVar2 = f1(user_input
                                                                                         [23],3);
                                                                              if ((int)uVar2 == 0) {
                                                                                uStack160 = 0x101d1d
                                                                                ;
                                                                                uVar2 = f4(
                                                  user_input[8],0xb);
                                                  if ((int)uVar2 == 0) {
                                                    uStack160 = 0x101d3b;
                                                    uVar2 = f3(user_input[0],0xb);
                                                    if ((int)uVar2 == 0) {
                                                      uStack160 = 0x101d59;
                                                      uVar2 = f4(user_input[12],0xc);
                                                      if ((int)uVar2 == 0) {
                                                        uStack160 = 0x101d74;
                                                        uVar2 = f4(user_input[34],1);
                                                        if ((int)uVar2 == 0) {
                                                          uStack160 = 0x101d88;
                                                          puts("Sucess!");
                                                          goto LAB_00101f2d;
                                                        }
                                                      }
                                                    }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                  }
                                                }
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  uStack160 = 0x101f28;
  puts("Wrong! try again...");
LAB_00101f2d:
  if (local_10 == *(long *)(in_FS_OFFSET + 0x28)) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  uStack160 = 0x101f41;
  __stack_chk_fail();
}
```

That looks horrible to reverse, we have no choice but to turn to black magic: [angr](https://angr.io/).

> angr is a python framework for analyzing binaries. It combines both static and dynamic symbolic ("concolic") analysis, making it applicable to a variety of tasks.

Using a simple python program such as we'll soon see, we can ask `angr` to perform a symbolic execution of the code and find the input for a code path that takes us to the desired output. We just need to figure out the length of the flag, and based on the fact that `user_input[43]` is the last array index used, we'll guess that it's 44 characters long.

Here's the `angr` script:

```python
import sys
import angr
import claripy

FLAG_LEN = 44

base_addr = 0x100000 # To match addresses to Ghidra

proj = angr.Project("./modus_tollens", main_opts={'base_addr': base_addr}) 

flag_chars = [claripy.BVS('flag_%d' % i, 8) for i in range(FLAG_LEN)]
flag = claripy.Concat( *flag_chars + [claripy.BVV(b'\n')]) # Add \n for fgets() to accept the input

state = proj.factory.full_init_state(
        args=['./modus_tollens'],
        add_options=angr.options.unicorn,
        stdin=flag,
)

# Add constraints that all characters are printable
for k in flag_chars:
    state.solver.add(k >= ord('!'))
    state.solver.add(k <= ord('~'))

simgr = proj.factory.simulation_manager(state)
simgr.explore(
    find  = lambda st: b'Sucess!' in st.posix.dumps(sys.stdout.fileno()),
    avoid = lambda st: b'Wrong!'  in st.posix.dumps(sys.stdout.fileno())
)

for found in simgr.found:
    print(found.posix.dumps(sys.stdin.fileno()).decode("ascii"))

```

In high-level, the script creates a string composed of 44 symbolic bit vectors (BVS) - one for each character of the flag. It connects the string to the program's `stdin`. It adds  constraints so that the string is composed of printable characters only. Finally, it performs a symbolic execution of the code by analyzing the binary (not running the code) while searching for a code path that will print `'Sucess!'` and trying to avoid a code path that will print `'Wrong!'`. 

Output:

```console
root@kali:/media/sf_CTFs/matrix/Modus_Tollens# source ~/.virtualenvs/angr/bin/activate
(angr) root@kali:/media/sf_CTFs/matrix/Modus_Tollens# python3 angr.py
WARNING | 2021-02-27 22:21:22,373 | angr.simos.simos | stdin is constrained to 45 bytes (has_end=True). If you are only providing the first 45 bytes instead of the entire stdin, please use stdin=SimFileStream(name='stdin', content=your_first_n_bytes, has_end=False).
WARNING | 2021-02-27 22:21:22,445 | angr.exploration_techniques.explorer | Using unicorn with find/avoid conditions that are a lambda (not a number, set, tuple or list)
WARNING | 2021-02-27 22:21:22,445 | angr.exploration_techniques.explorer | Unicorn may step over states that match the condition (find or avoid) without stopping.
MCL{S0m3T1meS_St4tic_An4lySis_i$_en0Ugh_n0?}

(angr) root@kali:/media/sf_CTFs/matrix/Modus_Tollens# deactivate
```

Black magic indeed:

```console
root@kali:/media/sf_CTFs/matrix/Modus_Tollens# ./modus_tollens
Enter flag: MCL{S0m3T1meS_St4tic_An4lySis_i$_en0Ugh_n0?}
Checking ...
Sucess!
```