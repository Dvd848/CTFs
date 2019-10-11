# seed-sPRiNG
Binary Exploitation, 350 points

## Description:
> The most revolutionary game is finally available: seed sPRiNG is open right now!


## Solution:

(Initial commit with just the script)

```c
#include <stdio.h> 
#include <time.h>
#include <stdlib.h> 
  
int main () 
{ 
    int i;
      
    srand(time(0)); 
    
    for (i = 0; i < 30; i++)
    {
        printf("%d\n", rand() & 0xf); 
    }
      
    return 0; 
} 
```

Output:
```console
dvdalt@pico-2019-shell1:~$ ./solve | nc 2019shell1.picoctf.com 12269



                          #                mmmmm  mmmmm    "    mm   m   mmm
  mmm    mmm    mmm    mmm#          mmm   #   "# #   "# mmm    #"m  # m"   "
 #   "  #"  #  #"  #  #" "#         #   "  #mmm#" #mmmm"   #    # #m # #   mm
  """m  #""""  #""""  #   #          """m  #      #   "m   #    #  # # #    #
 "mmm"  "#mm"  "#mm"  "#m##         "mmm"  #      #    " mm#mm  #   ##  "mmm"



Welcome! The game is easy: you jump on a sPRiNG.
How high will you fly?

LEVEL (1/30)

Guess the height: LEVEL (2/30)

Guess the height: LEVEL (3/30)

Guess the height: LEVEL (4/30)

Guess the height: LEVEL (5/30)

Guess the height: LEVEL (6/30)

Guess the height: LEVEL (7/30)

Guess the height: LEVEL (8/30)

Guess the height: LEVEL (9/30)

Guess the height: LEVEL (10/30)

Guess the height: LEVEL (11/30)

Guess the height: LEVEL (12/30)

Guess the height: LEVEL (13/30)

Guess the height: LEVEL (14/30)

Guess the height: LEVEL (15/30)

Guess the height: LEVEL (16/30)

Guess the height: LEVEL (17/30)

Guess the height: LEVEL (18/30)

Guess the height: LEVEL (19/30)

Guess the height: LEVEL (20/30)

Guess the height: LEVEL (21/30)

Guess the height: LEVEL (22/30)

Guess the height: LEVEL (23/30)

Guess the height: LEVEL (24/30)

Guess the height: LEVEL (25/30)

Guess the height: LEVEL (26/30)

Guess the height: LEVEL (27/30)

Guess the height: LEVEL (28/30)

Guess the height: LEVEL (29/30)

Guess the height: LEVEL (30/30)

Guess the height: picoCTF{pseudo_random_number_generator_not_so_random_66aacad47c332de30eb8d8170d96b772}Congratulation! You've won! Here is your flag:



dvdalt@pico-2019-shell1:~$
```