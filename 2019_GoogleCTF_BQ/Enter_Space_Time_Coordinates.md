# Enter Space-Time Coordinates
Category: Misc.

## Description
> Ok well done. The console is on. It's asking for coordinates. Beating heavily on the console yields little results, but the only time anything changes on your display is when you put in numbers.. So what numbers are you going to go for? You see the starship's logs, but is there a manual? Or should you just keep beating the console?

A binary file was attached.

## Solution

Let's start by running the program:
```console
root@kali:/media/sf_CTFs/google/Enter_Space_Time_Coordinates# ./rand2
Travel coordinator
0: AC+79 3888 - 98071128902306, 227382149184267
1: Pliamas Sos - 272066008193594, 113707484169060
2: Ophiuchus - 128283301225840, 43562014214202
3: Pax Memor -ne4456 Hi Pro - 270921358930721, 242857169312391
4: Camion Gyrin - 172610997163993, 45283555200205
5: CTF - <REDACTED>

Enter your destination's x coordinate:
>>> 100
Enter your destination's y coordinate:
>>> 101
Arrived somewhere, but not where the flag is. Sorry, try again.
```

We get the chance to enter our destination coordinates, but randomly entering some coordinates brings out nowhere.

### Option 1 - The Easy Way:

Just run `strings` on the binary:
```console
root@kali:/media/sf_CTFs/google/Enter_Space_Time_Coordinates# strings rand2 | grep CTF
Arrived at the flag. Congrats, your flag is: CTF{welcome_to_googlectf}
```

### Option 2 - The Slightly Less Easy Way

We open the file in Ghidra.

The decompiled output of the main function is:
```c
undefined8 main(void)
{
  int r;
  long lVar1;
  long lVar2;
  long y_coord;
  long x_coord;
  ulong i;
  
  seed = time((time_t *)0x0);
  puts("Travel coordinator");
  i = 0;
  while (i < 6) {
    printf("%zu: %s - ",i,*(undefined8 *)(destinations + i * 8));
    r = strcmp(*(char **)(destinations + i * 8),"CTF");
    if (r == 0) {
      puts("<REDACTED>");
    }
    else {
      lVar2 = next_destination();
      lVar1 = next_destination();
      printf("%zu, %zu\n",lVar1,lVar2);
    }
    i = i + 1;
  }
  printf("\nEnter your destination\'s x coordinate:\n>>> ");
  __isoc99_scanf(&DAT_00100b25,&x_coord);
  printf("Enter your destination\'s y coordinate:\n>>> ");
  __isoc99_scanf(&DAT_00100b25,&y_coord);
  lVar2 = next_destination();
  if (lVar2 == x_coord) {
    lVar2 = next_destination();
    if (lVar2 == y_coord) {
      puts("Arrived at the flag. Congrats, your flag is: CTF{welcome_to_googlectf}");
    }
  }
  puts("Arrived somewhere, but not where the flag is. Sorry, try again.");
  return 0;
}
```

`next_destination` is defined as:
```c
long next_destination(void)
{
  ulong uVar1;
  ulong uVar2;
  long lVar3;
  
  uVar1 = seed * 0x5deece66d + 0xb;
  lVar3 = SUB168(ZEXT816(uVar1) * ZEXT816(0x10001) >> 0x40,0);
  uVar2 = (uVar1 - lVar3 >> 1) + lVar3 >> 0x2f;
  seed = uVar1 - ((uVar2 << 0x30) - uVar2);
  return seed;
}
```

So we have a random seed initialized with the current time, and then `next_destination` gets called, performing some manipulation on the seed, returning a value that will be considered the next coordinate and modifying the seed for the next iteration.

The program prints a few coordinates based on an initial seed of the current time, and then redacts the most interesting coordinates - the ones of the "CTF" location. However, since we know how `next_destination` works, we know that the seed used to calculate "CTF"'s coordinates was also outputted as Camion Gyrin's Y coordinate. Therefore, we can easily predict what are the correct values for "CTF".

It should be noted that for some reason, Ghidra messes up (or at least over complicates) the decompilation of `next_destination`. I asked a friend to check what IDA Pro outputs for the same function, and the result is much more elegant:

```c
__int64 next_destination()
{
  seed = (25214903917LL * seed + 11) % 0xFFFFFFFFFFFFuLL;
  return seed;
}
```

Moreover, taking Ghidra's output and using it as a C function doesn't produce the correct results. A hint that something isn't optimal can be seen from the fact that Ghidra is using "Internal Decompiler Functions" such as SUB168 and ZEXT816.

From the Ghidra documentation (F1 from within the Decompilation window):
> Occasionally, the decompiler may use one of several internal decompiler functions that don't get transformed into more 'C'-like expressions. Use of these can indicate that the pcode is incorrect or needs to be "Tuned" to make the decompiler output better. It can also mean that the decompiler needs an additional simplification rule to take care of that particular situation.

Instead of "cheating" and using the IDA Pro syntax, we'll just use the inline assembly:

```c
#include <stdio.h>
#include <stdlib.h>

long next_destination(long seed)
{
    long val;

    asm (
        "MOV RDX, %1;"
        "MOV RAX,0x5deece66d;"
        "IMUL RAX,RDX;"
        "LEA RCX,[RAX + 0xb];"
        "MOV EDX,0x10001;"
        "MOV RAX,RCX;"
        "MUL RDX;"
        "MOV RAX,RCX;"
        "SUB RAX,RDX;"
        "SHR RAX,1;"
        "ADD RAX,RDX;"
        "SHR RAX,0x2f;"
        "MOV RDX,RAX;"
        "SHL RDX,0x30;"
        "SUB RDX,RAX;"
        "MOV RAX,RCX;"
        "SUB RAX,RDX;"
        "MOV %0,RAX;"
            :"=r"(val)
            :"r"(seed)
    );
    
    return val;
}

int main(int argc, char** argv)
{
    long seed;

    if (argc != 2)
    {
        printf("Usage: %s seed\n", argv[0]);
        return 1;
    }

    seed = atol(argv[1]);

    do 
    {
        printf("%ld  ", seed);
        seed = next_destination(seed);
    } while (getchar() != 'q');
    
    return 0;
}
```

This program will give us an infinite list of coordinates based on the seed we provide it. We can compile it with `gcc -masm=intel nd.c -o nd`

Now, let's run the original program again:
```console
Travel coordinator
0: AC+79 3888 - 141960677260956, 165957638957271
1: Pliamas Sos - 7936473333170, 131765289057947
2: Ophiuchus - 122193665038813, 248203215098833
3: Pax Memor -ne4456 Hi Pro - 153040595218017, 88909980494411
4: Camion Gyrin - 163722192541106, 81028967743204
5: CTF - <REDACTED>

Enter your destination's x coordinate:
>>>
```

We enter AC+79's X coordiante as a base to our prediction program:
```console
root@kali:/media/sf_CTFs/google/Enter_Space_Time_Coordinates# ./test 165957638957271
165957638957271
141960677260956
131765289057947
7936473333170
248203215098833
122193665038813
88909980494411
153040595218017
81028967743204
163722192541106
94167578523291
252960223816806
```

We can see all the coordinates the original program has outputted, plus the X,Y values for CTF.

We enter them, and:

```console
Enter your destination's x coordinate:
>>> 94167578523291
Enter your destination's y coordinate:
>>> 252960223816806
Arrived at the flag. Congrats, your flag is: CTF{welcome_to_googlectf}
Arrived somewhere, but not where the flag is. Sorry, try again.
```

