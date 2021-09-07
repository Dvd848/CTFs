# Secret Location - Base
Category: HW

## Description

> "Welcome back AGENT. It seems like you've got a marvelous lead that perhaps gives a clue about where you should head to next. Visit the lab, and talk to that Dr. Klostermann, or is it Cloysterman?, he will know how to decrypt the device.. you would think". ... Dr Klostermann: "Welcome to the technical department AGENT, I’m Dr. Klostermann, and this is my assistant, Konstantin. Let’s not waste any time, is that the device that you’re holding in your hand? Konstantin, start the basic procedure."
> 
> Challenge: Electronics Research Lab (hw)
> 
> Welcome back AGENT. It seems like you got a lead that perhaps gives a clue about where the next journey on your quest goes. Visit the lab, and talk to Dr. Klostermann, he will know how to decrypt the device Note: If you solved the challenge before but could not submit the flag, please try again, we had the wrong flag in our database.

A C file (and a `pico.uf2` file) were attached.

<details>
  <summary>Click to expand</summary>

```c
#include <stdbool.h>

#include "hardware/gpio.h"
#include "hardware/structs/sio.h"
#include "pico/stdlib.h"

int main(void)
{
	for (int i = 0; i < 8; i++) {
		gpio_init(i);
		gpio_set_dir(i, GPIO_OUT);
	}
	gpio_put_all(0);

	for (;;) {
		gpio_set_mask(67);
		gpio_clr_mask(0);
		sleep_us(100);
		gpio_set_mask(20);
		gpio_clr_mask(3);
		sleep_us(100);
		gpio_set_mask(2);
		gpio_clr_mask(16);
		sleep_us(100);
		gpio_set_mask(57);
		gpio_clr_mask(4);
		sleep_us(100);
		gpio_set_mask(0);
		gpio_clr_mask(25);
		sleep_us(100);
		gpio_set_mask(5);
		gpio_clr_mask(2);
		sleep_us(100);
		gpio_set_mask(18);
		gpio_clr_mask(65);
		sleep_us(100);
		gpio_set_mask(1);
		gpio_clr_mask(2);
		sleep_us(100);
		gpio_set_mask(64);
		gpio_clr_mask(17);
		sleep_us(100);
		gpio_set_mask(2);
		gpio_clr_mask(0);
		sleep_us(100);
		gpio_set_mask(1);
		gpio_clr_mask(6);
		sleep_us(100);
		gpio_set_mask(18);
		gpio_clr_mask(65);
		sleep_us(100);
		gpio_set_mask(1);
		gpio_clr_mask(0);
		sleep_us(100);
		gpio_set_mask(4);
		gpio_clr_mask(2);
		sleep_us(100);
		gpio_set_mask(0);
		gpio_clr_mask(0);
		sleep_us(100);
		gpio_set_mask(64);
		gpio_clr_mask(16);
		sleep_us(100);
		gpio_set_mask(16);
		gpio_clr_mask(64);
		sleep_us(100);
		gpio_set_mask(2);
		gpio_clr_mask(4);
		sleep_us(100);
		gpio_set_mask(0);
		gpio_clr_mask(3);
		sleep_us(100);
		gpio_set_mask(9);
		gpio_clr_mask(0);
		sleep_us(100);
		gpio_set_mask(0);
		gpio_clr_mask(1);
		sleep_us(100);
		gpio_set_mask(0);
		gpio_clr_mask(8);
		sleep_us(100);
		gpio_set_mask(8);
		gpio_clr_mask(0);
		sleep_us(100);
		gpio_set_mask(65);
		gpio_clr_mask(24);
		sleep_us(100);
		gpio_set_mask(22);
		gpio_clr_mask(64);
		sleep_us(100);
		gpio_set_mask(0);
		gpio_clr_mask(0);
		sleep_us(100);
		gpio_set_mask(0);
		gpio_clr_mask(5);
		sleep_us(100);
		gpio_set_mask(0);
		gpio_clr_mask(2);
		sleep_us(100);
		gpio_set_mask(65);
		gpio_clr_mask(16);
		sleep_us(100);
		gpio_set_mask(22);
		gpio_clr_mask(65);
		sleep_us(100);
		gpio_set_mask(1);
		gpio_clr_mask(6);
		sleep_us(100);
		gpio_set_mask(4);
		gpio_clr_mask(0);
		sleep_us(100);
		gpio_set_mask(66);
		gpio_clr_mask(21);
		sleep_us(100);
		gpio_set_mask(1);
		gpio_clr_mask(0);
		sleep_us(100);
		gpio_set_mask(0);
		gpio_clr_mask(2);
		sleep_us(100);
		gpio_set_mask(24);
		gpio_clr_mask(65);
		sleep_us(100);
		gpio_set_mask(67);
		gpio_clr_mask(24);
		sleep_us(100);
		gpio_set_mask(24);
		gpio_clr_mask(67);
		sleep_us(100);
		gpio_set_mask(2);
		gpio_clr_mask(8);
		sleep_us(100);
		gpio_set_mask(65);
		gpio_clr_mask(18);
		sleep_us(100);
		gpio_set_mask(16);
		gpio_clr_mask(64);
		sleep_us(100);
		gpio_set_mask(2);
		gpio_clr_mask(0);
		sleep_us(100);
		gpio_set_mask(68);
		gpio_clr_mask(19);
		sleep_us(100);
		gpio_set_mask(19);
		gpio_clr_mask(64);
		sleep_us(100);
		gpio_set_mask(72);
		gpio_clr_mask(2);
		sleep_us(100);
		gpio_set_mask(2);
		gpio_clr_mask(117);
		sleep_us(100);

		gpio_put_all(0);
		sleep_ms(500);
	}

	return 0;
}
```
</details>

## Solution

The attached code is for a [Raspberry Pi Pico](https://www.raspberrypi.org/products/raspberry-pi-pico/). It calls a series of `gpio_set_mask` and `gpio_clr_mask` functions with various values. 

Let's check the [documentation](https://raspberrypi.github.io/pico-sdk-doxygen/group__hardware__gpio.html) for these functions:

> `gpio_set_mask`: Drive high every GPIO appearing in mask. 
> 
> `gpio_clr_mask`: Drive low every GPIO appearing in mask. 

So, we can "hook" and simulate these functions using Python:

```python

val = 0

def gpio_set_mask(mask):
    global val
    val |= mask

def gpio_clr_mask(mask):
    global val
    val &= ~mask

def sleep_us(msec):
    print(chr(val), end='')

gpio_set_mask(67)
gpio_clr_mask(0)
sleep_us(100)
gpio_set_mask(20)
gpio_clr_mask(3)
sleep_us(100)
gpio_set_mask(2)
gpio_clr_mask(16)
sleep_us(100)
gpio_set_mask(57)
gpio_clr_mask(4)
sleep_us(100)
gpio_set_mask(0)
gpio_clr_mask(25)
sleep_us(100)
gpio_set_mask(5)
gpio_clr_mask(2)
sleep_us(100)
gpio_set_mask(18)
gpio_clr_mask(65)
sleep_us(100)
gpio_set_mask(1)
gpio_clr_mask(2)
sleep_us(100)
gpio_set_mask(64)
gpio_clr_mask(17)
sleep_us(100)
gpio_set_mask(2)
gpio_clr_mask(0)
sleep_us(100)
gpio_set_mask(1)
gpio_clr_mask(6)
sleep_us(100)
gpio_set_mask(18)
gpio_clr_mask(65)
sleep_us(100)
gpio_set_mask(1)
gpio_clr_mask(0)
sleep_us(100)
gpio_set_mask(4)
gpio_clr_mask(2)
sleep_us(100)
gpio_set_mask(0)
gpio_clr_mask(0)
sleep_us(100)
gpio_set_mask(64)
gpio_clr_mask(16)
sleep_us(100)
gpio_set_mask(16)
gpio_clr_mask(64)
sleep_us(100)
gpio_set_mask(2)
gpio_clr_mask(4)
sleep_us(100)
gpio_set_mask(0)
gpio_clr_mask(3)
sleep_us(100)
gpio_set_mask(9)
gpio_clr_mask(0)
sleep_us(100)
gpio_set_mask(0)
gpio_clr_mask(1)
sleep_us(100)
gpio_set_mask(0)
gpio_clr_mask(8)
sleep_us(100)
gpio_set_mask(8)
gpio_clr_mask(0)
sleep_us(100)
gpio_set_mask(65)
gpio_clr_mask(24)
sleep_us(100)
gpio_set_mask(22)
gpio_clr_mask(64)
sleep_us(100)
gpio_set_mask(0)
gpio_clr_mask(0)
sleep_us(100)
gpio_set_mask(0)
gpio_clr_mask(5)
sleep_us(100)
gpio_set_mask(0)
gpio_clr_mask(2)
sleep_us(100)
gpio_set_mask(65)
gpio_clr_mask(16)
sleep_us(100)
gpio_set_mask(22)
gpio_clr_mask(65)
sleep_us(100)
gpio_set_mask(1)
gpio_clr_mask(6)
sleep_us(100)
gpio_set_mask(4)
gpio_clr_mask(0)
sleep_us(100)
gpio_set_mask(66)
gpio_clr_mask(21)
sleep_us(100)
gpio_set_mask(1)
gpio_clr_mask(0)
sleep_us(100)
gpio_set_mask(0)
gpio_clr_mask(2)
sleep_us(100)
gpio_set_mask(24)
gpio_clr_mask(65)
sleep_us(100)
gpio_set_mask(67)
gpio_clr_mask(24)
sleep_us(100)
gpio_set_mask(24)
gpio_clr_mask(67)
sleep_us(100)
gpio_set_mask(2)
gpio_clr_mask(8)
sleep_us(100)
gpio_set_mask(65)
gpio_clr_mask(18)
sleep_us(100)
gpio_set_mask(16)
gpio_clr_mask(64)
sleep_us(100)
gpio_set_mask(2)
gpio_clr_mask(0)
sleep_us(100)
gpio_set_mask(68)
gpio_clr_mask(19)
sleep_us(100)
gpio_set_mask(19)
gpio_clr_mask(64)
sleep_us(100)
gpio_set_mask(72)
gpio_clr_mask(2)
sleep_us(100)
gpio_set_mask(2)
gpio_clr_mask(117)
sleep_us(100)
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/google/4_Secret_Location_-_Base]
└─$ python3 solve.py
CTF{be65dfa2355e5309808a7720a615bca8c82a13d7}
```

An alternative method to solve the challenge was to use the `*.uf2` file (either by burning it on a real Pico device or using a simulator).