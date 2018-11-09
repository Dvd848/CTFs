# keygen-me-1
Reversing, 400 points

## Description:
> Can you generate a valid product key for the validation program?

A binary file was attached.

## Solution:

Let's run the executable:
```console
root@kali:/media/sf_CTFs/pico/keygen-me-1# ./activate
Usage: ./activate <PRODUCT_KEY>
```

As expected, we need to provide a product key. Time to disassemble. 

```
root@kali:/media/sf_CTFs/pico/keygen-me-1# r2 activate
 -- There's no way you could crash radare2. No. Way.
[0x08048500]> aa
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x08048500]> afl
0x08048410    3 35           sym._init
0x08048450    1 6            sym.imp.printf
0x08048460    1 6            sym.imp.fgets
0x08048470    1 6            sym.imp.fclose
0x08048480    1 6            sym.imp.__stack_chk_fail
0x08048490    1 6            sym.imp.puts
0x080484a0    1 6            sym.imp.exit
0x080484b0    1 6            sym.imp.strlen
0x080484c0    1 6            sym.imp.__libc_start_main
0x080484d0    1 6            sym.imp.setvbuf
0x080484e0    1 6            sym.imp.fopen
0x080484f0    1 6            fcn.080484f0
0x08048500    1 33           entry0
0x08048530    1 4            sym.__x86.get_pc_thunk.bx
0x08048540    4 43           sym.deregister_tm_clones
0x08048570    4 53           sym.register_tm_clones
0x080485b0    3 30           sym.__do_global_dtors_aux
0x080485d0    4 43   -> 40   entry1.init
0x080485fb    6 139          sym.print_flag
0x08048686    7 50           sym.check_valid_char
0x080486b8    8 82           sym.ord
0x0804870a   11 103          sym.check_valid_key
0x08048771    4 172          sym.validate_key
0x0804881d    8 195          main
0x080488e0    4 93           sym.__libc_csu_init
0x08048940    1 2            sym.__libc_csu_fini
0x08048944    1 20           sym._fini
```

The overview of `main` is as follows:
```
                                     .---------------------------------.
                                     | [0x804881d]                     |
                                     | 0x0804883a call sym.imp.setvbuf |
                                     `---------------------------------'
                                             f t
                                             | |
                                             | '----------------------.
    .----------------------------------------'                        |
    |                                                                 |
.-----------------------------------------------.                 .-------------------------------------.
|  0x8048847 [gf]                               |                 |  0x804885e [gb]                     |
| 0x0804884a str.Usage:_._activate__PRODUCT_KEY |                 | 0x0804886a call sym.check_valid_key |
| 0x0804884f call sym.imp.puts                  |                 `-------------------------------------'
`-----------------------------------------------'                         f t
    v                                                                     | |
    |                                                                     | |
    '-------------------.                                                 | |
                        |                                                 | '---------------------.
                        |       .-----------------------------------------'                       |
                        |       |                                                                 |
                        |   .------------------------------------------------------------.    .----------------------------------.
                        |   |  0x8048876 [gi]                                            |    |  0x804888d [gh]                  |
                        |   | 0x08048879 str.Please_Provide_a_VALID_16_byte_Product_Key. |    | 0x08048899 call sym.validate_key |
                        |   | 0x0804887e call sym.imp.puts                               |    `----------------------------------'
                        |   `------------------------------------------------------------'            f t
                        |       v                                                                     | |
                        |       |                                                                     | |
                        |   .---'                                                                     | |
                        |   |                                                                         | '----------.
                        |   |                                            .----------------------------'            |
                        |   |                                            |                                         |
                        |   |                                        .-------------------------------------.   .------------------------------------------------.
                        |   |                                        |  0x80488a5 [gl]                     |   |  0x80488bc [gk]                                |
                        |   |                                        | 0x080488a8 str.INVALID_Product_Key. |   | 0x080488bf str.Product_Activated_Successfully: |
                        |   |                                        | 0x080488ad call sym.imp.puts        |   | 0x080488c4 call sym.imp.printf                 |
                        |   |                                        `-------------------------------------'   | 0x080488cc call sym.print_flag                 |
                        |   |                                            v                                     `------------------------------------------------'
                        |   |                                            |                                         v
                        |   |                                            |                                         |
                        |   |                                   .--------'                                         |
                        |   |                                   | .------------------------------------------------'
                        '---|---------------------------------------.
                            '-----------------------------------------.
                                                                | | | |
                                                          .--------------------.
                                                          |  0x80488d6 [ge]    |
                                                          `--------------------'           
```

Let's start with `check_valid_key`:

```
                                          .--------------------------------------.
                                          | [0x804870a]                          |
                                          | (fcn) sym.check_valid_key 103        |
                                          |   sym.check_valid_key (int key);     |
                                          | ; var int c @ ebp-0x5                |
                                          | ; var int i @ ebp-0x4                |
                                          | ; arg int key @ ebp+0x8              |
                                          | ; CALL XREF from main (0x804886a)    |
                                          | push ebp                             |
                                          | mov ebp, esp                         |
                                          | sub esp, 0x10                        |
                                          | ; [0x8:4]=-1                         |
                                          | ; 0                                  |
                                          | cmp dword [key], 0                   |
                                          | jne 0x804871d;[ga]                   |
                                          `--------------------------------------'
                                                  f t
                                                  | |
                                                  | '---------------------------.
                        .-------------------------'                             |
                        |                                                       |
                    .-----------------------.                               .--------------------------------.
                    |  0x8048716 [gd]       |                               |  0x804871d [ga]                |
                    | mov eax, 0            |                               | ; [0x8:4]=-1                   |
                    | jmp 0x804876f;[gc]    |                               | ; 8                            |
                    `-----------------------'                               | mov eax, dword [key]           |
                        v                                                   | movzx eax, byte [eax]          |
                        |                                                   | mov byte [c], al               |
                        |                                                   | mov dword [i], 0               |
                        |                                                   | jmp 0x8048759;[ge]             |
                        |                                                   `--------------------------------'
                        |                                                       v
                        |                                                       |
                        '-------.                                               |
                                |                                        .------'
                               .-------------------------------------------.
                               ||                                        | |
                               ||                                  .--------------------------------------------------.
                               ||                                  |  0x8048759 [ge]                                  |
                               ||                                  | ; CODE XREF from sym.check_valid_key (0x804872d) |
                               ||                                  | cmp byte [c], 0                                  |
                               ||                                  | jne 0x804872f;[gh]                               |
                               ||                                  `--------------------------------------------------' 
                               ||                                        t f
                               ||                                        | |
                               ||       .--------------------------------' |
                               ||       |                                  '------------------------------------------.
                               ||       |                                                                             |
                               ||   .-----------------------------------.                                         .----------------------------------.
                               ||   |  0x804872f [gh]                   |                                         |  0x804875f [gk]                  |
                               ||   | movsx eax, byte [c]               |                                         | cmp dword [i], 0x10              |
                               ||   | push eax                          |                                         | je 0x804876c;[gj]                |
                               ||   | call sym.check_valid_char;[gf]    |                                         `----------------------------------'
                               ||   | add esp, 4                        |                                                 f t
                               ||   | test al, al                       |                                                 | |
                               ||   | jne 0x8048747;[gg]                |                                                 | |
                               ||   `-----------------------------------'                                                 | |
                               ||           f t                                                                           | |
                               ||           | |                                                                           | |
                               ||           | '---------------------.                                                     | |
                               ||       .---'                       |                                                     | '-----------.
                               ||       |                           |                                       .-------------'             |
                               ||       |                           |                                       |                           |
                               ||   .-----------------------.   .----------------------------------.    .-----------------------.   .---------------------------------.
                               ||   |  0x8048740 [gi]       |   |  0x8048747 [gg]                  |    |  0x8048765 [gl]       |   |  0x804876c [gj]                 |
                               ||   | mov eax, 0            |   | add dword [i], 1                 |    | mov eax, 0            |   | mov eax, dword [i]              |
                               ||   | jmp 0x804876f;[gc]    |   | ; [0x8:4]=-1                     |    | jmp 0x804876f;[gc]    |   `---------------------------------'
                               ||   `-----------------------'   | ; 8                              |    .-----------------------'       v
                               ||       v                       | mov edx, dword [key]             |        v                           |
                               ||       |                       | mov eax, dword [i]               |        |                           |
                               ||       |                       | add eax, edx                     |        |                           |
                               ||       |                       | movzx eax, byte [eax]            |        |                           |
                               ||       |                       | mov byte [c], al                 |        |                           |
                               ||       |                       `----------------------------------'        |                           |
                               ||       |                           v                                       |                           |
                               ||       |                           |                                       |                           |
                               ||       '--------------.            |                                       |                           |
                               `------------------------------------'                                       |                           |
                                |                      | .--------------------------------------------------'                           |
                                |                      | | .----------------------------------------------------------------------------'
                                '----------------------------.
                                                       | | | |
                                                 .-------------------------------------------------------------------------.
                                                 |  0x804876f [gc]                                                         |
                                                 | ; CODE XREFS from sym.check_valid_key (0x804871b, 0x8048745, 0x804876a) |
                                                 | leave                                                                   |
                                                 | ret                                                                     |
                                                 `-------------------------------------------------------------------------' 
```

It iterates through all the characters in the key, check that they comply with `check_valid_char`, and makes sure that the key length is 0x10.

`check_valid_char` simply checks that the character is a digit or an uppercase letter.

The real logic is in `validate_key`:

```
              .------------------------------------.
              | [0x8048771]                        |
              | (fcn) sym.validate_key 172         |
              |   sym.validate_key (int key);      |
              | ; var int sum @ ebp-0x14           |
              | ; var int i @ ebp-0x10             |
              | ; var int key_len @ ebp-0xc        |
              | ; var int local_4h @ ebp-0x4       |
              | ; arg int key @ ebp+0x8            |
              | ; CALL XREF from main (0x8048899)  |
              | push ebp                           |
              | mov ebp, esp                       |
              | push ebx                           |
              | sub esp, 0x14                      |
              | sub esp, 0xc                       |
              | push dword [key]                   |
              | ; size_t strlen(const char *s)     |
              | call sym.imp.strlen;[ga]           |
              | add esp, 0x10                      |
              | mov dword [key_len], eax           |
              | mov dword [sum], 0                 |
              | mov dword [i], 0                   |
              | jmp 0x80487c9;[gb]                 |
              `------------------------------------'
                  v
                  |
               .--'
 .---------------.
 |             | |
 |       .-----------------------------------------------.
 |       |  0x80487c9 [gb]                               |
 |       | ; CODE XREF from sym.validate_key (0x8048797) |
 |       | mov eax, dword [key_len]                      |
 |       | sub eax, 1                                    |
 |       | cmp eax, dword [i]                            |
 |       | jg 0x8048799;[ge]                             |
 |       `-----------------------------------------------'
 |             t f
 |             | |
 |    .--------' |
 |    |          '----------------------------.
 |    |                                       |
 |.-----------------------------------.   .----------------------------------.
 ||  0x8048799 [ge]                   |   |  0x80487d4 [gf]                  |
 || mov edx, dword [i]                |   | mov ecx, dword [sum]             |
 || ; [0x8:4]=-1                      |   | mov edx, 0x38e38e39              |
 || ; 8                               |   | mov eax, ecx                     |
 || mov eax, dword [key]              |   | mul edx                          |
 || add eax, edx                      |   | mov ebx, edx                     |
 || movzx eax, byte [eax]             |   | shr ebx, 3                       |
 || movsx eax, al                     |   | mov eax, ebx                     |
 || sub esp, 0xc                      |   | shl eax, 3                       |
 || push eax                          |   | add eax, ebx                     |
 || call sym.ord;[gd]                 |   | shl eax, 2                       |
 || add esp, 0x10                     |   | sub ecx, eax                     |
 || movsx eax, al                     |   | mov ebx, ecx                     |
 || ; 1                               |   | mov eax, dword [key_len]         |
 || lea edx, [eax + 1]                |   | lea edx, [eax - 1]               |
 || mov eax, dword [i]                |   | ; [0x8:4]=-1                     |
 || add eax, 1                        |   | ; 8                              |
 || imul eax, edx                     |   | mov eax, dword [key]             |
 || add dword [sum], eax              |   | add eax, edx                     |
 || add dword [i], 1                  |   | movzx eax, byte [eax]            |
 |`-----------------------------------'   | movsx eax, al                    |
 |    v                                   | sub esp, 0xc                     |
 |    |                                   | push eax                         |
 |    |                                   | call sym.ord;[gd]                |
 |    |                                   | add esp, 0x10                    |
 |    |                                   | movsx eax, al                    |
 |    |                                   | cmp ebx, eax                     |
 |    |                                   | sete al                          |
 |    |                                   | mov ebx, dword [local_4h]        |
 |    |                                   | leave                            |
 |    |                                   | ret                              |
 |    |                                   `----------------------------------'
 |    |
 |    |
 `----' 
```

Translated to Python, this is equivalent to:
```python
def validate_key(key):
    log.info("Validating key: {}".format(key))
    if len(key) != 16:
        raise Exception("Bad length")
    if not re.match('^[A-Z0-9]+$', key):
        raise Exception("Bad characters")
    
    s = 0
    for i in range(len(key) - 1):
        edx = my_ord(key[i]) + 1
        eax = i + 1
        s += (edx * eax)

    ebx = 0x38E38E39 * s
    ebx = int(ebx >> 32) # needed only for python, mul puts lower bytes in eax, higher in edx
    ebx = ebx >> 3
    eax = ebx << 3
    eax += ebx
    eax = eax << 2
    ecx = s - eax

    log.info("ecx: {}".format(hex(ecx)))
    log.info("ord(key[-1]): {}".format(hex(my_ord(key[-1]))))

    return ecx == my_ord(key[-1])
```

The `ord` function has a custom implementation, as follows:
```

          .------------------------------------------------------------.
          | [0x80486b8]                                                |
          | (fcn) sym.ord 82                                           |
          |   sym.ord (int arg_8h);                                    |
          | ; var int local_ch @ ebp-0xc                               |
          | ; arg int arg_8h @ ebp+0x8                                 |
          | ; CALL XREFS from sym.validate_key (0x80487ab, 0x8048808)  |
          | push ebp                                                   |
          | mov ebp, esp                                               |
          | sub esp, 0x18                                              |
          | ; [0x8:4]=-1                                               |
          | ; 8                                                        |
          | mov eax, dword [arg_8h]                                    |
          | mov byte [local_ch], al                                    |
          | ; '/'                                                      |
          | cmp byte [local_ch], 0x2f                                  |
          | jle 0x80486d9;[ga]                                         |
          `------------------------------------------------------------'
                  f t
                  | |
                  | '-------------------------------.
                .-'                                 |
                |                                   |
            .----------------------------------.    |
            |  0x80486ca [gc]                  |    |
            | ; '9'                            |    |
            | cmp byte [local_ch], 0x39        |    |
            | jg 0x80486d9;[ga]                |    |
            `----------------------------------'    |
                    f t                             |
                    | |                             |
                    | '------------------------.    |
     .--------------'                          |    |
     |                                         | .--'
     |                                         | |
 .----------------------------------.    .----------------------------------.
 |  0x80486d0 [ge]                  |    |  0x80486d9 [ga]                  |
 | movzx eax, byte [local_ch]       |    | ; '@'                            |
 | ; '0'                            |    | cmp byte [local_ch], 0x40        |
 | sub eax, 0x30                    |    | jle 0x80486ee;[gf]               |
 | jmp 0x8048708;[gd]               |    `----------------------------------'
 `----------------------------------'            f t
     v                                           | |
     |                                           | |
     '-------------.                             | |
                   |                             | '-----------.
                   |       .---------------------'             |
                   |       |                                   |
                   |   .----------------------------------.    |
                   |   |  0x80486df [gg]                  |    |
                   |   | ; 'Z'                            |    |
                   |   | cmp byte [local_ch], 0x5a        |    |
                   |   | jg 0x80486ee;[gf]                |    |
                   |   `----------------------------------'    |
                   |           f t                             |
                   |           | |                             |
                   |           | '-----------------------------|-----.
                   |       .---'                               '-------.
                   |       |                                         | |
                   |   .----------------------------------.    .-----------------------------------------.
                   |   |  0x80486e5 [gh]                  |    |  0x80486ee [gf]                         |
                   |   | movzx eax, byte [local_ch]       |    | sub esp, 0xc                            |
                   |   | ; '7'                            |    | ; 0x8048984                             |
                   |   | sub eax, 0x37                    |    | ; "Found Invalid Character!"            |
                   |   | jmp 0x8048708;[gd]               |    | push str.Found_Invalid_Character        |
                   |   `----------------------------------'    | ; int puts(const char *s)               |
                   |       v                                   | call sym.imp.puts;[gi]                  |
                   |       |                                   | add esp, 0x10                           |
                   |       |                                   | sub esp, 0xc                            |
                   |       |                                   | push 0                                  |
                   |       |                                   | ; void exit(int status)                 |
                   |       |                                   | call sym.imp.exit;[gj]                  |
                   |       |                                   `-----------------------------------------'
                   |       |
                   |       |
          .--------|-------'
          | .------'
          | |
    .--------------------------------------------------.
    |  0x8048708 [gd]                                  |
    | ; CODE XREFS from sym.ord (0x80486d7, 0x80486ec) |
    | leave                                            |
    | ret                                              |
    `--------------------------------------------------'  
```

Basically, this translates to:
```python
def my_ord(c):
    if c.isalpha():
        return ord(c) - ord('7')
    else:
        return ord(c) - ord('0')
```

So we have everything we need to understand how a valid key looks like.

Now, we can play with the key a bit:
```console
root@kali:/media/sf_CTFs/pico/keygen-me-1# python solve.py
[*] Validating key: ZZZZZZZZZZZZZZZZ
[*] ecx: 0x0
[*] ord(key[-1]): 0x23
False
```

We want the `ord` of the last character to match `ecx` - `0` will do the trick:
```console
root@kali:/media/sf_CTFs/pico/keygen-me-1# python solve.py
[*] Validating key: ZZZZZZZZZZZZZZZ0
[*] ecx: 0x0
[*] ord(key[-1]): 0x0
True
```

Let's try:
```console
$ ./activate $(python -c 'print ("Z"*15) + "0"')
Product Activated Successfully: picoCTF{k3yg3n5_4r3_s0_s1mp13_3718231394}
```

The flag: picoCTF{k3yg3n5_4r3_s0_s1mp13_3718231394}