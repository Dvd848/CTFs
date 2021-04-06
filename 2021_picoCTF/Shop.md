# Shop
Category: Reverse Engineering, 50 points

## Description

> Best Stuff - Cheap Stuff, Buy Buy Buy... 

A binary file was attached.

## Solution

Let's run the attached file:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Shop]
└─$ ./source
Welcome to the market!
=====================
You have 40 coins
        Item            Price   Count
(0) Quiet Quiches       10      12
(1) Average Apple       15      8
(2) Fruitful Flag       100     1
(3) Sell an Item
(4) Exit
Choose an option:

```

This is a market simulator where we can buy and sell things.

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Shop]
└─$ file ./source
./source: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), statically linked, Go BuildID=PjavkptB2tPNbBJewQBD/KlDP1g_fpBnKyhti11wQ/JIWBEgtPAt3YPE6g8qd7/pWlMkjZuAYGqbSv46xuR, with debug_info, not stripped
```

It's implemented in Go so we'll skip the analysis and just play around with it a bit.

We can safely assume that we want to buy a flag, but we don't have enough money:

```
Choose an option:
2
How many do you want to buy?
1
Not enough money.
You have 40 coins
```

We can buy other things though:

```
Choose an option:
0
How many do you want to buy?
1
You have 30 coins
```

Now we can sell it. 

```
Choose an option:
3
Your inventory
(0) Quiet Quiches       10      1
(1) Average Apple       15      0
(2) Fruitful Flag       100     0
What do you want to sell?
0
How many?
1
You have 40 coins
```

Let's buy something again, but try to enter a negative number when trying to sell it:

```
Choose an option:
0
How many do you want to buy?
1
You have 30 coins
        Item            Price   Count
(0) Quiet Quiches       10      10
(1) Average Apple       15      8
(2) Fruitful Flag       100     1
(3) Sell an Item
(4) Exit
Choose an option:
3
Your inventory
(0) Quiet Quiches       10      1
(1) Average Apple       15      0
(2) Fruitful Flag       100     0
What do you want to sell?
0
How many?
-10
You have -70 coins
```

Something weird happened and we got a negative amount of coins. The program probably isn't checking for negatives/wraparounds. Let's continue with this direction:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Shop]
└─$ nc mercury.picoctf.net 42159
Welcome to the market!
=====================
You have 40 coins
        Item            Price   Count
(0) Quiet Quiches       10      12
(1) Average Apple       15      8
(2) Fruitful Flag       100     1
(3) Sell an Item
(4) Exit
Choose an option:
3
Your inventory
(0) Quiet Quiches       10      0
(1) Average Apple       15      0
(2) Fruitful Flag       100     0
What do you want to sell?
2
How many?
-2000000000
You have 1863462952 coins
        Item            Price   Count
(0) Quiet Quiches       10      12
(1) Average Apple       15      8
(2) Fruitful Flag       100     1
(3) Sell an Item
(4) Exit
Choose an option:
2
How many do you want to buy?
1
Flag is:  [112 105 99 111 67 84 70 123 98 52 100 95 98 114 111 103 114 97 109 109 101 114 95 55 57 55 98 50 57 50 99 125]
```

Convert the decimal to ASCII:

```python
>>> flag = "112 105 99 111 67 84 70 123 98 52 100 95 98 114 111 103 114 97 109 109 101 114 95 55 57 55 98 50 57 50 99 125".split(" ")
>>> "".join(chr(int(c)) for c in flag)
'picoCTF{b4d_brogrammer_797b292c}'
```

The flag: `picoCTF{b4d_brogrammer_797b292c}`