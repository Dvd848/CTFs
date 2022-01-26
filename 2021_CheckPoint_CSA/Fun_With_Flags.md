# Fun With Flags
Category: Reversing, 200 Points

## Description

> Welcome to Dr. Sheldon Cooper and Dr. Amy Farrah Fowler's flags shop!
> 
> Where you can buy and sell some of our favorite flags.
> 
> Colors? Stripes? Stars? We've got them all!
> 
> We've given you some coins, don't spend them all at once :)
> 
> Hope you'll have Fun With Flags!
> 
> Disclaimer:
> 
> Dr. Sheldon Cooper, who created the shop, allowed only himself to buy certain flags, so don't be disappointed if you can't have all of them.
> 
> He also decided to log every transaction, in case anyone tries to buy the special flags that are not allowed, so don't try to trick him!


```python
#!/usr/bin/env python3


class Flag:
    def __init__(self, name, price, description, colors, stripes, stars):
        self.name = name
        self.price = price
        self.description = description
        self.colors = colors
        self.stripes = stripes
        self.stars = stars

    def __str__(self):
        return f'{self.name} - {self.description} (price: {self.price})'


class User:
    def __init__(self, name):
        self.name = name
        self.owned_flags = []
        if name == "Dr. Sheldon Lee Cooper":
            self.is_VIP = True
            self.coins = INITIAL_COINS * 2
        else:
            self.is_VIP = False
            self.coins = INITIAL_COINS

    def __str__(self):
        user_value = self.coins + sum(flag.price for flag in self.owned_flags)
        result = f"User {self.name} "
        result += "(VIP)." if self.is_VIP else "(NON-VIP)"
        result += f" has {str(self.coins)} coins."
        result += f" Account value: {user_value}."

        if self.owned_flags:
            total_colors = sum(flag.colors for flag in self.owned_flags)
            total_stripes = sum(flag.stripes for flag in self.owned_flags)
            total_stars = sum(flag.stars for flag in self.owned_flags)

            result += "\nAverage value per: "
            result += "color (total {}) - {:0.2f}, ".format(total_colors, user_value / total_colors)
            result += "stripe (total {}) - {:0.2f}, ".format(total_stripes, user_value / total_stripes)
            result += "star (total {}) - {:0.2f}.\n".format(total_stars, user_value / total_stars)

            result += "Owned flags:\n"
            result += "\n".join(str(flag) for flag in self.owned_flags)

        result += "\n"
        return result

    def buy_flag(self, flag):
        self.coins -= flag.price
        self.owned_flags.append(flag)
        self.update_VIP()

    def sell_flag(self, flag):
        self.coins += flag.price
        self.owned_flags.remove(flag)
        self.update_VIP()

    def update_VIP(self):
        if self.is_VIP:
            return  # once VIP, always VIP
        self.is_VIP = sum(flag.stars for flag in self.owned_flags) >= 120  # Real stars collectors can be VIP too :)

    def can_afford_flag(self, flag):
        return self.coins >= flag.price

    def allowed_to_buy_CSA_flag(self):
        return self.is_VIP


available_flags = [
    Flag("Suriname",    100, "The smallest South American country", 4, 5, 1),
    Flag("Togo",        100, "Has French as the official language", 4, 5, 1),
    Flag("Azerbaijan",  100, "The Land of Fire", 4, 3, 1),
    Flag("Liberia",     200, "Has Africa's cleanest cities", 3, 11, 1),
    Flag("Myanmar",     200, "Formerly known as Burma", 4, 3, 1),
    Flag("Philippines", 300, "Named after King Philip II of Spain", 4, 2, 1),
    Flag("Uzbekistan",  400, "Has the world's largest open-pit gold mine", 4, 5, 12),
    Flag("Tajikistan",  500, "Has the world's second highest dam", 4, 3, 7),
    Flag("Slovenia",    600, "Has the world's longest stone arch railroad bridge", 3, 3, 3),
    Flag("Syria",       700, "Has the world's oldest operational dam", 4, 3, 2),
    Flag("Honduras",    800, "Has a dual capital", 2, 2, 5),
    Flag("Cape Verde",  900, "Named after the Cap-Vert peninsula", 4, 5, 10),
    Flag("Israel",     1000, "The country that brought you CSA", 2, 2, 1),
    Flag("Russia",     1200, "Home to the Hermitage Museum", 3, 3, 0),
    Flag("USA",        1200, "United States of America", 3, 13, 50),
    Flag("Cuba",       1300, "Famous for it's cigars", 3, 5, 1),
    Flag("CSA",        1337, open("flag.txt", "rt").read(), 1, 33, 7),
    Flag("Jordan",     1400, "Home to Petra", 4, 3, 1),
    Flag("Singapore",  1400, "The world's second most densely populated country", 2, 2, 5),
    Flag("Venezuela",  1500, "Home to the world's highest waterfall", 4, 3, 8),
]


def send(message):
    print(message)


def recv():
    return input()


def get_int_from_user():
    try:
        user_selection = int(recv())
        return user_selection
    except:
        return 0


def list_flags(flags):
    res = "=================================\n"
    for i, flag in enumerate(flags):
        res += f'{i} {flag.name} - {flag.price}\n'
    res += "=================================\n"
    return res


def modify_flag_menu():
    raise NotImplementedError  # Sheldon todo - allow users to add and remove stars from their owned flags


def buy_flag_menu():
    send(f"There are {len(available_flags)} flags available to buy.")
    send("How many flags would you like to buy?")
    amount = get_int_from_user()
    if amount > len(available_flags):
        send("There aren't that many available flags!")
        return

    for _ in range(amount):
        send(f"You have {user.coins} coins.")
        if user.coins == 0:
            return
        send("Available flags to buy:")
        send(list_flags(available_flags))
        send("Which flag would you like to buy?")
        flag_index = get_int_from_user()
        flag_to_buy = None
        allowed_to_buy_flag = False
        try:
            if 0 <= flag_index < len(available_flags):
                flag_to_buy = available_flags[flag_index]
                allowed_to_buy_flag = user.can_afford_flag(flag_to_buy)
                log_message = str(user) + f" is trying to buy flag {flag_to_buy.name}"
                if flag_to_buy.name == "CSA":
                    log_message += "\n***ATTENTION - CSA FLAG PURCHASE. RUNNING ADDITIONAL CHECK***"
                    allowed_to_buy_flag = allowed_to_buy_flag and user.allowed_to_buy_CSA_flag()
                    log_message += " Additional checks result - user is " + ("ALLOWED" if allowed_to_buy_flag else "NOT ALLOWED") + " to purchase it"

                send("You tried to buy flag " + flag_to_buy.name + " (allowed to purchase? " + str(allowed_to_buy_flag) + "). This transaction was logged successfully.")
            else:
                log_message = str(user) + " is trying to buy an non-existing flag. This looks suspicious!"
                send("Invalid flag index. This attempt is logged!")

            open("log.txt", "a+").write(log_message + "\n")
        except:
            send(f'Failed to log transaction of flag purchase. Do you have write permission?')

        if flag_to_buy is not None:
            if allowed_to_buy_flag:
                user.buy_flag(flag_to_buy)
                available_flags.remove(flag_to_buy)
                send(f"{flag_to_buy.name} flag bought!")
            else:
                send(f"You can't buy flag {flag_to_buy.name}!")


def sell_flag_menu():
    send(f"You have {len(user.owned_flags)} flags.")
    if not user.owned_flags:
        return
    send("How many flags would you like to sell?")
    amount = get_int_from_user()
    if amount > len(user.owned_flags):
        send("You don't have that many flags!")
        return

    flag_to_sell = None
    for _ in range(amount):
        send("Available flags to sell:")
        send(list_flags(user.owned_flags))
        send("Which flag would you like to sell?")
        flag_index = get_int_from_user()

        try:
            if 0 <= flag_index < len(user.owned_flags):
                flag_to_sell = user.owned_flags[flag_index]
                log_message = str(user) + " is selling flag " + flag_to_sell.name
            else:
                log_message = str(user) + " is trying to sell a flag they don't have. This looks suspicious!"
                send("Invalid flag index. This attempt is logged!")

            if flag_to_sell is not None:
                user.sell_flag(flag_to_sell)
                available_flags.append(flag_to_sell)
                send(f"{flag_to_sell.name} flag sold!")
                log_message += f". {flag_to_sell.name} Sold successfully!"

            open("log.txt", "a+").write(log_message + "\n")
        except:
            send(f'Failed to log transaction of {flag_to_sell.name} flag. Do you have write permission?')


def main_menu():
    while True:
        send("What would you like to do?\n")
        send("\n".join([
#           "0 - MODIFY FLAGS",
            "1 - LIST AVAILABLE FLAGS",
            "2 - BUY FLAG",
            "3 - SELL FLAG",
            "4 - LIST MY FLAGS",
            "5 - LIST MY STATS",
            "6 - EXIT",
        ]))

        try:
            selection = get_int_from_user()

            if selection == 0:
                modify_flag_menu()
            elif selection == 1:
                send("Available flags to buy:")
                send(list_flags(available_flags))
            elif selection == 2:
                buy_flag_menu()
            elif selection == 3:
                sell_flag_menu()
            elif selection == 4:
                flags_amount = len(user.owned_flags)
                if flags_amount > 0:
                    send(f"You have {flags_amount} flags:")
                    send(list_flags(user.owned_flags))
                else:
                    send("You have no flags :(\n")
            elif selection == 5:
                send(str(user))
            elif selection == 6:  # exit
                break
            else:
                send(f"Invalid menu selection {selection}")
        except:
            send("Apologies, not all functions are implemented yet!")

    send("Hope you had Fun With Flags! Goodbye.")


def logo():
    send("""Welcome to
    
    █▀▀ █ █ █▄ █   █ █ █ █ ▀█▀ █ █   █▀▀ █   ▄▀█ █▀▀ █▀
    █▀  █▄█ █ ▀█   ▀▄▀▄▀ █  █  █▀█   █▀  █▄▄ █▀█ █▄█ ▄█
    """)


INITIAL_COINS = 1000
user = User("Dr. Amy Farrah Fowler")

if __name__ == '__main__':
    logo()
    main_menu()


```

## Solution

This is a simple program to buy and sell flags. Our goal is to buy the CSA flag:

```python
    Flag("CSA",        1337, open("flag.txt", "rt").read(), 1, 33, 7),
```

However, two things are blocking us. First, we don't have enough money to buy the flag, since we receive 1000 coins and the flag costs 1337 coins. And second, even if we found a way to make more money, we must have a VIP status in order to buy this specific flag:

```python
    def allowed_to_buy_CSA_flag(self):
        return self.is_VIP
```

First problem first. We can buy and sell flags, but when selling a flag we earn the same amount that we paid for it in the first place. How do we make money here? The answer is hidden very nicely in the following code:

```python
def sell_flag_menu():
    send(f"You have {len(user.owned_flags)} flags.")
    if not user.owned_flags:
        return
    send("How many flags would you like to sell?")
    amount = get_int_from_user()
    if amount > len(user.owned_flags):
        send("You don't have that many flags!")
        return

    flag_to_sell = None
    for _ in range(amount):
        send("Available flags to sell:")
        send(list_flags(user.owned_flags))
        send("Which flag would you like to sell?")
        flag_index = get_int_from_user()

        try:
            if 0 <= flag_index < len(user.owned_flags):
                flag_to_sell = user.owned_flags[flag_index]
                log_message = str(user) + " is selling flag " + flag_to_sell.name
            else:
                log_message = str(user) + " is trying to sell a flag they don't have. This looks suspicious!"
                send("Invalid flag index. This attempt is logged!")

            if flag_to_sell is not None:
                user.sell_flag(flag_to_sell)
                available_flags.append(flag_to_sell)
                send(f"{flag_to_sell.name} flag sold!")
                log_message += f". {flag_to_sell.name} Sold successfully!"

            open("log.txt", "a+").write(log_message + "\n")
        except:
            send(f'Failed to log transaction of {flag_to_sell.name} flag. Do you have write permission?')
```

The program asks us how many flags we'd like to sell, then loop and tries to sell that amount of flags. However, notice how `flag_to_sell` is scoped outside the loop and only initializes within it if `0 <= flag_index < len(user.owned_flags)`. This means that after entering a valid flag index for the first time, we can follow-up by entering invalid indices for the rest of the loop. Yes, the incident will be logged, but `user.sell_flag(flag_to_sell)` will still get executed and we'll make more money. Let's see that in action:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Fun_With_Flags]
└─$ nc fun-with-flags.csa-challenge.com 6666
Welcome to

    █▀▀ █ █ █▄ █   █ █ █ █ ▀█▀ █ █   █▀▀ █   ▄▀█ █▀▀ █▀
    █▀  █▄█ █ ▀█   ▀▄▀▄▀ █  █  █▀█   █▀  █▄▄ █▀█ █▄█ ▄█

What would you like to do?

1 - LIST AVAILABLE FLAGS
2 - BUY FLAG
3 - SELL FLAG
4 - LIST MY FLAGS
5 - LIST MY STATS
6 - EXIT
2
There are 20 flags available to buy.
How many flags would you like to buy?
2
You have 1000 coins.
Available flags to buy:
=================================
0 Suriname - 100
1 Togo - 100
2 Azerbaijan - 100
3 Liberia - 200
4 Myanmar - 200
5 Philippines - 300
6 Uzbekistan - 400
7 Tajikistan - 500
8 Slovenia - 600
9 Syria - 700
10 Honduras - 800
11 Cape Verde - 900
12 Israel - 1000
13 Russia - 1200
14 USA - 1200
15 Cuba - 1300
16 CSA - 1337
17 Jordan - 1400
18 Singapore - 1400
19 Venezuela - 1500
=================================

Which flag would you like to buy?
0
You tried to buy flag Suriname (allowed to purchase? True). This transaction was logged successfully.
Suriname flag bought!
You have 900 coins.
Available flags to buy:
=================================
0 Togo - 100
1 Azerbaijan - 100
2 Liberia - 200
3 Myanmar - 200
4 Philippines - 300
5 Uzbekistan - 400
6 Tajikistan - 500
7 Slovenia - 600
8 Syria - 700
9 Honduras - 800
10 Cape Verde - 900
11 Israel - 1000
12 Russia - 1200
13 USA - 1200
14 Cuba - 1300
15 CSA - 1337
16 Jordan - 1400
17 Singapore - 1400
18 Venezuela - 1500
=================================

Which flag would you like to buy?
10
You tried to buy flag Cape Verde (allowed to purchase? True). This transaction was logged successfully.
Cape Verde flag bought!
What would you like to do?

1 - LIST AVAILABLE FLAGS
2 - BUY FLAG
3 - SELL FLAG
4 - LIST MY FLAGS
5 - LIST MY STATS
6 - EXIT
3
You have 2 flags.
How many flags would you like to sell?
2
Available flags to sell:
=================================
0 Suriname - 100
1 Cape Verde - 900
=================================

Which flag would you like to sell?
1
Cape Verde flag sold!
Available flags to sell:
=================================
0 Suriname - 100
=================================

Which flag would you like to sell?
1
Invalid flag index. This attempt is logged!
Failed to log transaction of Cape Verde flag. Do you have write permission?
What would you like to do?

1 - LIST AVAILABLE FLAGS
2 - BUY FLAG
3 - SELL FLAG
4 - LIST MY FLAGS
5 - LIST MY STATS
6 - EXIT
5
User Dr. Amy Farrah Fowler (NON-VIP) has 1800 coins. Account value: 1900.
Average value per: color (total 4) - 475.00, stripe (total 5) - 380.00, star (total 1) - 1900.00.
Owned flags:
Suriname - The smallest South American country (price: 100)

What would you like to do?

1 - LIST AVAILABLE FLAGS
2 - BUY FLAG
3 - SELL FLAG
4 - LIST MY FLAGS
5 - LIST MY STATS
6 - EXIT
6
Hope you had Fun With Flags! Goodbye.
```

We started with 1000 coins and ended up with 1800 coins, so we can afford the CSA flag. But how do we bypass the VIP protection now? This time, the bug is hidden in the "buy flag" functionality:

```python
def buy_flag_menu():
    send(f"There are {len(available_flags)} flags available to buy.")
    send("How many flags would you like to buy?")
    amount = get_int_from_user()
    if amount > len(available_flags):
        send("There aren't that many available flags!")
        return

    for _ in range(amount):
        send(f"You have {user.coins} coins.")
        if user.coins == 0:
            return
        send("Available flags to buy:")
        send(list_flags(available_flags))
        send("Which flag would you like to buy?")
        flag_index = get_int_from_user()
        flag_to_buy = None
        allowed_to_buy_flag = False
        try:
            if 0 <= flag_index < len(available_flags):
                flag_to_buy = available_flags[flag_index]
                allowed_to_buy_flag = user.can_afford_flag(flag_to_buy)
                log_message = str(user) + f" is trying to buy flag {flag_to_buy.name}"
                if flag_to_buy.name == "CSA":
                    log_message += "\n***ATTENTION - CSA FLAG PURCHASE. RUNNING ADDITIONAL CHECK***"
                    allowed_to_buy_flag = allowed_to_buy_flag and user.allowed_to_buy_CSA_flag()
                    log_message += " Additional checks result - user is " + ("ALLOWED" if allowed_to_buy_flag else "NOT ALLOWED") + " to purchase it"

                send("You tried to buy flag " + flag_to_buy.name + " (allowed to purchase? " + str(allowed_to_buy_flag) + "). This transaction was logged successfully.")
            else:
                log_message = str(user) + " is trying to buy an non-existing flag. This looks suspicious!"
                send("Invalid flag index. This attempt is logged!")

            open("log.txt", "a+").write(log_message + "\n")
        except:
            send(f'Failed to log transaction of flag purchase. Do you have write permission?')

        if flag_to_buy is not None:
            if allowed_to_buy_flag:
                user.buy_flag(flag_to_buy)
                available_flags.remove(flag_to_buy)
                send(f"{flag_to_buy.name} flag bought!")
            else:
                send(f"You can't buy flag {flag_to_buy.name}!")
```

Notice how the exception handling in this function is structured. If we are able to trigger an exception somewhere between `flag_to_buy = available_flags[flag_index]` and the additional checks performed if `flag_to_buy.name == "CSA"`, we'll jump directly to the `except` and then slide down to the logic that actually buys the flag. A closer examination of the flow shows that we'll also need to make sure that `allowed_to_buy_flag` is `True`, which is the case right after `allowed_to_buy_flag = user.can_afford_flag(flag_to_buy)` given that we have enough money. So, the best place to try and trigger an exception would be around `log_message = str(user) + f" is trying to buy flag {flag_to_buy.name}"`. This is promising, since `str(user)` contains lots of controllable logic:

```python
    def __str__(self):
        user_value = self.coins + sum(flag.price for flag in self.owned_flags)
        result = f"User {self.name} "
        result += "(VIP)." if self.is_VIP else "(NON-VIP)"
        result += f" has {str(self.coins)} coins."
        result += f" Account value: {user_value}."

        if self.owned_flags:
            total_colors = sum(flag.colors for flag in self.owned_flags)
            total_stripes = sum(flag.stripes for flag in self.owned_flags)
            total_stars = sum(flag.stars for flag in self.owned_flags)

            result += "\nAverage value per: "
            result += "color (total {}) - {:0.2f}, ".format(total_colors, user_value / total_colors)
            result += "stripe (total {}) - {:0.2f}, ".format(total_stripes, user_value / total_stripes)
            result += "star (total {}) - {:0.2f}.\n".format(total_stars, user_value / total_stars)

            result += "Owned flags:\n"
            result += "\n".join(str(flag) for flag in self.owned_flags)

        result += "\n"
        return result
```

From a quick look at this code, the obvious candidate for an exception is the division taking place as part of the average calculation. If we can cause one of the *total*s to be zero, we'll get a "Divide by Zero Exception". Let's check the flag list and see if it's possible:

```python
available_flags = [
    Flag("Suriname",    100, "The smallest South American country", 4, 5, 1),
    Flag("Togo",        100, "Has French as the official language", 4, 5, 1),
    Flag("Azerbaijan",  100, "The Land of Fire", 4, 3, 1),
    Flag("Liberia",     200, "Has Africa's cleanest cities", 3, 11, 1),
    Flag("Myanmar",     200, "Formerly known as Burma", 4, 3, 1),
    Flag("Philippines", 300, "Named after King Philip II of Spain", 4, 2, 1),
    Flag("Uzbekistan",  400, "Has the world's largest open-pit gold mine", 4, 5, 12),
    Flag("Tajikistan",  500, "Has the world's second highest dam", 4, 3, 7),
    Flag("Slovenia",    600, "Has the world's longest stone arch railroad bridge", 3, 3, 3),
    Flag("Syria",       700, "Has the world's oldest operational dam", 4, 3, 2),
    Flag("Honduras",    800, "Has a dual capital", 2, 2, 5),
    Flag("Cape Verde",  900, "Named after the Cap-Vert peninsula", 4, 5, 10),
    Flag("Israel",     1000, "The country that brought you CSA", 2, 2, 1),
    Flag("Russia",     1200, "Home to the Hermitage Museum", 3, 3, 0),
    Flag("USA",        1200, "United States of America", 3, 13, 50),
    Flag("Cuba",       1300, "Famous for it's cigars", 3, 5, 1),
    Flag("CSA",        1337, open("flag.txt", "rt").read(), 1, 33, 7),
    Flag("Jordan",     1400, "Home to Petra", 4, 3, 1),
    Flag("Singapore",  1400, "The world's second most densely populated country", 2, 2, 5),
    Flag("Venezuela",  1500, "Home to the world's highest waterfall", 4, 3, 8),
]
```

Looks like the Russian flag has no stars, so if that's the only flag we own, we'll hit an exception at `result += "star (total {}) - {:0.2f}.\n".format(total_stars, user_value / total_stars)`.

So, we make enough money to buy the Russian flag and the CSA flag, sell all other flags and buy the Russian flag, request to buy the CSA flag, hit the exception which lets us skip the VIP check, and get the CSA flag.

```python
# First, generate a pwntools template using:
#  pwn template --host fun-with-flags.csa-challenge.com --port 6666
#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================

def read_menu(io):
    io.recvuntil(b'6 - EXIT\r\n')

def buy_flags(io, flags):
    read_menu(io)
    io.sendline(b'2')
    io.sendlineafter(b'How many flags would you like to buy?\r\n', str(len(flags)).encode())
    log.info(io.recvlineS())
    for flag in flags:
        log.info(f"Buying flag #{flag}")
        io.sendlineafter(b'Which flag would you like to buy?\r\n', str(flag).encode())
        io.recvline()
        log.info(io.recvlineS())


def sell_flags(io, flags):
    read_menu(io)
    io.sendline(b'3')
    io.sendlineafter(b'How many flags would you like to sell?\r\n', str(len(flags)).encode())
    for flag in flags:
        log.info(f"Trying to sell flag #{flag}")
        io.sendlineafter(b'Which flag would you like to sell?\r\n', str(flag).encode())
        log.info(io.recvlineS())

def list_stats(io):
    read_menu(io)
    io.sendline(b'5')
    log.info("Listing stats:\n" + io.recvuntilS(b"\r\n\r\n"))


io = start()

buy_flags(io, [0, 10])
list_stats(io)
sell_flags(io, [1, 1])
list_stats(io)
buy_flags(io, [0, 0, 0, 0, 0, 13])
list_stats(io)
sell_flags(io, [6]*7)
list_stats(io)
sell_flags(io, [0]*6)
list_stats(io)
buy_flags(io, [6])
list_stats(io)
buy_flags(io, [8])
list_stats(io)
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Fun_With_Flags]
└─$ python3 exploit.py
[+] Opening connection to fun-with-flags.csa-challenge.com on port 6666: Done
[*] You have 1000 coins.
[*] Buying flag #0
[*] Suriname flag bought!
[*] Buying flag #10
[*] Cape Verde flag bought!
[*] Listing stats:
    User Dr. Amy Farrah Fowler (NON-VIP) has 0 coins. Account value: 1000.
    Average value per: color (total 8) - 125.00, stripe (total 10) - 100.00, star (total 11) - 90.91.
    Owned flags:
    Suriname - The smallest South American country (price: 100)
    Cape Verde - Named after the Cap-Vert peninsula (price: 900)

[*] Trying to sell flag #1
[*] Cape Verde flag sold!
[*] Trying to sell flag #1
[*] Invalid flag index. This attempt is logged!
[*] Listing stats:
    User Dr. Amy Farrah Fowler (NON-VIP) has 1800 coins. Account value: 1900.
    Average value per: color (total 4) - 475.00, stripe (total 5) - 380.00, star (total 1) - 1900.00.
    Owned flags:
    Suriname - The smallest South American country (price: 100)

[*] You have 1800 coins.
[*] Buying flag #0
[*] Togo flag bought!
[*] Buying flag #0
[*] Azerbaijan flag bought!
[*] Buying flag #0
[*] Liberia flag bought!
[*] Buying flag #0
[*] Myanmar flag bought!
[*] Buying flag #0
[*] Philippines flag bought!
[*] Buying flag #13
[*] Cape Verde flag bought!
[*] Listing stats:
    User Dr. Amy Farrah Fowler (NON-VIP) has 0 coins. Account value: 1900.
    Average value per: color (total 27) - 70.37, stripe (total 34) - 55.88, star (total 16) - 118.75.
    Owned flags:
    Suriname - The smallest South American country (price: 100)
    Togo - Has French as the official language (price: 100)
    Azerbaijan - The Land of Fire (price: 100)
    Liberia - Has Africa's cleanest cities (price: 200)
    Myanmar - Formerly known as Burma (price: 200)
    Philippines - Named after King Philip II of Spain (price: 300)
    Cape Verde - Named after the Cap-Vert peninsula (price: 900)

[*] Trying to sell flag #6
[*] Cape Verde flag sold!
[*] Trying to sell flag #6
[*] Invalid flag index. This attempt is logged!
[*] Trying to sell flag #6
[*] Invalid flag index. This attempt is logged!
[*] Trying to sell flag #6
[*] Invalid flag index. This attempt is logged!
[*] Trying to sell flag #6
[*] Invalid flag index. This attempt is logged!
[*] Trying to sell flag #6
[*] Invalid flag index. This attempt is logged!
[*] Trying to sell flag #6
[*] Invalid flag index. This attempt is logged!
[*] Listing stats:
    User Dr. Amy Farrah Fowler (NON-VIP) has 6300 coins. Account value: 7300.
    Average value per: color (total 23) - 317.39, stripe (total 29) - 251.72, star (total 6) - 1216.67.
    Owned flags:
    Suriname - The smallest South American country (price: 100)
    Togo - Has French as the official language (price: 100)
    Azerbaijan - The Land of Fire (price: 100)
    Liberia - Has Africa's cleanest cities (price: 200)
    Myanmar - Formerly known as Burma (price: 200)
    Philippines - Named after King Philip II of Spain (price: 300)

[*] Trying to sell flag #0
[*] Suriname flag sold!
[*] Trying to sell flag #0
[*] Togo flag sold!
[*] Trying to sell flag #0
[*] Azerbaijan flag sold!
[*] Trying to sell flag #0
[*] Liberia flag sold!
[*] Trying to sell flag #0
[*] Myanmar flag sold!
[*] Trying to sell flag #0
[*] Philippines flag sold!
[*] Listing stats:
    User Dr. Amy Farrah Fowler (NON-VIP) has 7300 coins. Account value: 7300.

[*] You have 7300 coins.
[*] Buying flag #6
[*] Russia flag bought!
[*] Listing stats:
    Apologies, not all functions are implemented yet!
    What would you like to do?

[*] You have 6100 coins.
[*] Buying flag #8
[*] CSA flag bought!
[*] Listing stats:
    User Dr. Amy Farrah Fowler (NON-VIP) has 4763 coins. Account value: 7300.
    Average value per: color (total 4) - 1825.00, stripe (total 36) - 202.78, star (total 7) - 1042.86.
    Owned flags:
    Russia - Home to the Hermitage Museum (price: 1200)
    CSA - CSA{M4y_Th3Re_ALwaY5_8E_A_ST4R_0n_y0UR_fL4G} (price: 1337)

[*] Closed connection to fun-with-flags.csa-challenge.com port 6666
```
