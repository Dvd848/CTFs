# special-pw
Reversing, 600 points

## Description:
> Can you figure out the right argument to this program to login? We couldn't manage to get a copy of the binary but we did manage to dump some machine code and memory from the running process. 

Assembly:

```assembly
.intel_syntax noprefix
.bits 32
	
.global main	; int main(int argc, char **argv)

main:
	push   ebp
	mov    ebp,esp
	sub    esp,0x10
	mov    DWORD PTR [ebp-0xc],0x0
	mov    eax,DWORD PTR [ebp+0xc]
	mov    eax,DWORD PTR [eax+0x4]
	mov    DWORD PTR [ebp-0x4],eax
	jmp    part_b
part_a:
	add    DWORD PTR [ebp-0xc],0x1
	add    DWORD PTR [ebp-0x4],0x1
part_b:	
	mov    eax,DWORD PTR [ebp-0x4]
	movzx  eax,BYTE PTR [eax]
	test   al,al
	jne    part_a
	mov    DWORD PTR [ebp-0x8],0x0
	jmp    part_d
part_c:	
	mov    eax,DWORD PTR [ebp+0xc]
	add    eax,0x4
	mov    edx,DWORD PTR [eax]
	mov    eax,DWORD PTR [ebp-0x8]
	add    eax,edx
	mov    DWORD PTR [ebp-0x4],eax
	mov    eax,DWORD PTR [ebp-0x4]
	movzx  eax,BYTE PTR [eax]
	xor    eax,0x4c
	mov    edx,eax
	mov    eax,DWORD PTR [ebp-0x4]
	mov    BYTE PTR [eax],dl
	mov    eax,DWORD PTR [ebp-0x4]
	movzx  eax,WORD PTR [eax]
	ror    ax,0x9
	mov    edx,eax
	mov    eax,DWORD PTR [ebp-0x4]
	mov    WORD PTR [eax],dx
	mov    eax,DWORD PTR [ebp-0x4]
	mov    eax,DWORD PTR [eax]
	rol    eax,0x7
	mov    edx,eax
	mov    eax,DWORD PTR [ebp-0x4]
	mov    DWORD PTR [eax],edx
	add    DWORD PTR [ebp-0x8],0x1
part_d:	
	mov    eax,DWORD PTR [ebp-0xc]
	sub    eax,0x3
	cmp    eax,DWORD PTR [ebp-0x8]
	jg     part_c
	mov    eax,DWORD PTR [ebp+0xc]
	mov    eax,DWORD PTR [eax+0x4]
	mov    DWORD PTR [ebp-0x4],eax
	mov    DWORD PTR [ebp-0x10],0x3c7fc74
	jmp    part_f
part_e:	
	mov    eax,DWORD PTR [ebp-0x4]
	movzx  edx,BYTE PTR [eax]
	mov    eax,DWORD PTR [ebp-0x10]
	movzx  eax,BYTE PTR [eax]
	cmp    dl,al
	je     part_k
	mov    eax,0x0
	jmp    part_h
part_k:	
	add    DWORD PTR [ebp-0x4],0x1
	add    DWORD PTR [ebp-0x10],0x1
part_f:	
	mov    eax,DWORD PTR [ebp-0x10]
	movzx  eax,BYTE PTR [eax]
	test   al,al
	jne    part_e
	mov    eax,DWORD PTR [ebp+0xc]
	add    eax,0x4
	mov    eax,DWORD PTR [eax]
	mov    edx,DWORD PTR [ebp-0x10]
	mov    ecx,0x3c7fc74
	sub    edx,ecx
	add    eax,edx
	movzx  eax,BYTE PTR [eax]
	test   al,al
	je     part_g
	mov    eax,0x0			; LOGIN_FAILED
	jmp    part_h
part_g:	
	mov    eax,0x1			; LOGIN_SUCCESS
part_h:	
	leave
	ret
```

Memory dump:
```
03C7FC74:  37 a1 2a a3 bd 33 22 ba  2f 3c 98 aa b9 2f 39 a4   |7.*..3"./<.../9.|
03C7FC84:  18 33 aa 9a 2f 39 18 b3  24 3a af 18 99 1c 1c b1   |.3../9..$:......|
03C7FC94:  19 b1 98 1b 3e 59 ae a6  00                        |....>Y...|
```

## Solution:

We need to provide an input so that the program will return 1 (LOGIN_SUCCESS).

We can see that all the logic is in the main function, and that the password is `argv[1]` (since `argv` is accessed by `mov eax, DWORD PTR [ebp+0xc]` and then `argv[1]` is `mov eax, DWORD PTR [eax+0x4]`)

First, let's compile the code so that we'll be able to use a debugger if we need it.

The program does not compile out of the box, and the following changes need to be done:
* `.bits 32` -> `.code32`
* Remove all comments
* Add `.text`
* Rename memory offset `0x3c7fc74` to known memory offset of data partition, and add the relevant data

This translates to the following result:
```assembly
.intel_syntax noprefix
.code32

.text
.global main

main:
	push   ebp
	mov    ebp,esp
	sub    esp,0x10
	mov    DWORD PTR [ebp-0xc],0x0
	mov    eax,DWORD PTR [ebp+0xc]
	mov    eax,DWORD PTR [eax+0x4]
	mov    DWORD PTR [ebp-0x4],eax
	jmp    part_b
part_a:
	add    DWORD PTR [ebp-0xc],0x1
	add    DWORD PTR [ebp-0x4],0x1
part_b:	
	mov    eax,DWORD PTR [ebp-0x4]
	movzx  eax,BYTE PTR [eax]
	test   al,al
	jne    part_a
	mov    DWORD PTR [ebp-0x8],0x0
	jmp    part_d
part_c:	
	mov    eax,DWORD PTR [ebp+0xc]
	add    eax,0x4
	mov    edx,DWORD PTR [eax]
	mov    eax,DWORD PTR [ebp-0x8]
	add    eax,edx
	mov    DWORD PTR [ebp-0x4],eax
    
	mov    eax,DWORD PTR [ebp-0x4]
	movzx  eax,BYTE PTR [eax]
	xor    eax,0x4c
	mov    edx,eax
	mov    eax,DWORD PTR [ebp-0x4]
	mov    BYTE PTR [eax],dl
    
	mov    eax,DWORD PTR [ebp-0x4]
	movzx  eax,WORD PTR [eax]
	ror    ax,0x9
	mov    edx,eax
	mov    eax,DWORD PTR [ebp-0x4]
	mov    WORD PTR [eax],dx
    
	mov    eax,DWORD PTR [ebp-0x4]
	mov    eax,DWORD PTR [eax]
	rol    eax,0x7
	mov    edx,eax
	mov    eax,DWORD PTR [ebp-0x4]
	mov    DWORD PTR [eax],edx
    
	add    DWORD PTR [ebp-0x8],0x1
part_d:	
	mov    eax,DWORD PTR [ebp-0xc]
	sub    eax,0x3
	cmp    eax,DWORD PTR [ebp-0x8]
	jg     part_c
	mov    eax,DWORD PTR [ebp+0xc]
	mov    eax,DWORD PTR [eax+0x4]
	mov    DWORD PTR [ebp-0x4],eax
	mov    DWORD PTR [ebp-0x10], OFFSET mydata
	jmp    part_f
part_e:	
	mov    eax,DWORD PTR [ebp-0x4]
	movzx  edx,BYTE PTR [eax]
	mov    eax,DWORD PTR [ebp-0x10]
	movzx  eax,BYTE PTR [eax]
	cmp    dl,al
	je     part_k
	mov    eax,0x0
	jmp    part_h
part_k:	
	add    DWORD PTR [ebp-0x4],0x1
	add    DWORD PTR [ebp-0x10],0x1
part_f:	
	mov    eax,DWORD PTR [ebp-0x10]
	movzx  eax,BYTE PTR [eax]
	test   al,al
	jne    part_e
	mov    eax,DWORD PTR [ebp+0xc]
	add    eax,0x4
	mov    eax,DWORD PTR [eax]
	mov    edx,DWORD PTR [ebp-0x10]
	mov    ecx,OFFSET mydata
	sub    edx,ecx
	add    eax,edx
	movzx  eax,BYTE PTR [eax]
	test   al,al
	je     part_g
	mov    eax,0x0
	jmp    part_h
part_g:	
	mov    eax,0x1
part_h:	
	leave
	ret

.data
mydata:
.byte  0x37
.byte  0xa1
.byte  0x2a
.byte  0xa3
.byte  0xbd
.byte  0x33
.byte  0x22
.byte  0xba
.byte  0x2f
.byte  0x3c
.byte  0x98
.byte  0xaa
.byte  0xb9
.byte  0x2f
.byte  0x39
.byte  0xa4
.byte  0x18
.byte  0x33
.byte  0xaa
.byte  0x9a
.byte  0x2f
.byte  0x39
.byte  0x18
.byte  0xb3
.byte  0x24
.byte  0x3a
.byte  0xaf
.byte  0x18
.byte  0x99
.byte  0x1c
.byte  0x1c
.byte  0xb1
.byte  0x19
.byte  0xb1
.byte  0x98
.byte  0x1b
.byte  0x3e
.byte  0x59
.byte  0xae
.byte  0xa6
.byte  0x00
```

Now we can compile with:
```console
root@kali:/media/sf_CTFs/pico/special-pw# gcc -m32 -o prog modified_assembly.s
```

We can see the graph view of the function with Radare2:
```
root@kali:/media/sf_CTFs/pico/special-pw# r2 prog
 -- This incident will be reported
[0x00001060]> aa
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x00001060]> s main
[0x00001060]> VV
```

The first part iterates the input, and calculates the length of the input string.
```
                                                     .-----------------------------------------------.
                                                     | [0x1199]                                      |
                                                     | (fcn) main 231                                |
                                                     |   main (int argc, char **argv, char **envp);  |
                                                     | ; var int local_10h @ ebp-0x10                |
                                                     | ; var int length @ ebp-0xc                    |
                                                     | ; var int i @ ebp-0x8                         |
                                                     | ; var int pointer @ ebp-0x4                   |
                                                     | ; arg int input @ ebp+0xc                     |
                                                     | push ebp                                      |
                                                     | mov ebp, esp                                  |
                                                     | sub esp, 0x10                                 |
                                                     | mov dword [length], 0                         |
                                                     | ; [0xc:4]=0                                   |
                                                     | mov eax, dword [input]                        |
                                                     | ; [0x4:4]=0x10101                             |
                                                     | mov eax, dword [eax + 4]                      |
                                                     | mov dword [pointer], eax                      |
                                                     | jmp loc.part_b;[ga]                           |
                                                     `-----------------------------------------------'
                                                         v
                                                         |
                                                         '-------.
                                              .--------------------.
                                              |                  | |
                                              |            .----------------------------------.
                                              |            |  0x11b9 [ga]                     |
                                              |            | ;-- part_b:                      |
                                              |            | ; CODE XREF from main (0x11af)   |
                                              |            | mov eax, dword [pointer]         |
                                              |            | movzx eax, byte [eax]            |
                                              |            | test al, al                      |
                                              |            | jne loc.part_a;[gc]              |
                                              |            `----------------------------------'
                                              |                  t f
                                              |                  | |
                                              |    .-------------' |
                                              |    |               '---------------------.
                                              |    |                                     |
                                              |.--------------------------------.    .-------------------------------.
                                              ||  0x11b1 [gc]                   |    |  0x11c3 [ge]                  |
                                              || ;-- part_a:                    |    | mov dword [i], 0              |
                                              || add dword [length], 1          |    | jmp loc.part_d;[gd]           |
                                              || add dword [pointer], 1         |    `-------------------------------'
                                              |`--------------------------------'        v
                                              |    v                                     |
                                              |    |                                     |
                                              `----'                                     |                     
```

The next part runs for i=0..(length-3):
```
                                  '---.
   .------------------------------------.
   |                                  | |
   |                            .----------------------------------.
   |                            |  0x1211 [gd]                     |
   |                            | ;-- part_d:                      |
   |                            | ; CODE XREF from main (0x11ca)   |
   |                            | mov eax, dword [length]          |
   |                            | sub eax, 3                       |
   |                            | cmp eax, dword [i]               |
   |                            | jg loc.part_c;[gf]               |
   |                            `----------------------------------'
   |                                  t f
   |                                  | |
   |    .-----------------------------' |
   |    |                               '-------.
   |    |                                       |
   |.----------------------------------.    .--------------------------------------
   ||  0x11cc [gf]                     |    |  0x121c [gh]                        |
   || ;-- part_c:                      |    | ; [0xc:4]=0                         |
   || ; [0xc:4]=0                      |    | mov eax, dword [input]              |
   || mov eax, dword [input]           |    | ; [0x4:4]=0x10101                   |
   || add eax, 4                       |    | mov eax, dword [eax + 4]            |
   || mov edx, dword [eax]             |    | mov dword [pointer], eax            |
   || mov eax, dword [i]               |    | ; 0x4018                            |
   || add eax, edx                     |    |                                     |
   || mov dword [pointer], eax         |    | mov dword [data_ptr], loc.mydata    |
   || mov eax, dword [pointer]         |    | jmp loc.part_f;[gg]                 |
   || movzx eax, byte [eax]            |    `-------------------------------------'
   || xor eax, 0x4c                    |        v
   || mov edx, eax                     |        |
   || mov eax, dword [pointer]         |        |
   || mov byte [eax], dl               |        |
   || mov eax, dword [pointer]         |        |
   || movzx eax, word [eax]            |        |
   || ror ax, 9                        |        |
   || mov edx, eax                     |        |
   || mov eax, dword [pointer]         |        |
   || mov word [eax], dx               |        |
   || mov eax, dword [pointer]         |        |
   || mov eax, dword [eax]             |        |
   || rol eax, 7                       |        |
   || mov edx, eax                     |        |
   || mov eax, dword [pointer]         |        |
   || mov dword [eax], edx             |        |
   || add dword [i], 1                 |        |
   |`----------------------------------'        |
   |    v                                       |
   |    |                                       |
   `----'                                       |
```

This part performs the encryption in `[gf]` by performing xor, ror and rol. Once done, `[gh]` and the sections after it compare the encrypted result to the expected result stored in `mydata`.

Translated to Python, the encryption algorithm is:
```python
def encrypt(b_arr):
    b = bytearray(b_arr)
    for i in range(len(b_arr) - 3):
        # XOR Byte
        t = b[i]
        t ^= 0x4c
        b[i] = t

        # ROR WORD
        t = struct.unpack('<H', b[i:i+2])[0]
        t = ror(t, 9, word_size = 16)
        b[i:i+2] = bytearray(struct.pack("<H", t))

        # ROL DWORD
        t = struct.unpack('<I', b[i:i+4])[0]
        t = rol(t, 7, word_size = 32)
        b[i:i+4] = bytearray(struct.pack("<I", t))
    return str(b)
```

Note that each of the operations is done on a differnt word size.

To decrypt, we will have to start from the end and move back, performing the opposite operation for each step:
```python
def decrypt(b_arr):
    b = bytearray(b_arr)
    for i in range(len(b_arr) - 5, -1, -1):
        # ROR DWORD
        t = struct.unpack('<I', b[i:i+4])[0]
        t = ror(t, 7, word_size = 32)
        b[i:i+4] = bytearray(struct.pack("<I", t))

        # ROL WORD
        t = struct.unpack('<H', b[i:i+2])[0]
        t = rol(t, 9, word_size = 16)
        b[i:i+2] = bytearray(struct.pack("<H", t))

        # XOR Byte
        t = b[i]
        t ^= 0x4c
        b[i] = t
```

Note that the opposite of rol(7) is ror(7), the opposite of ror(9) is rol(9), and the opposite of xor(0x4c) is xor(0x4c).

Now that we can decrypt, we run the decryption function on `mydata` and get the flag:
```python
from pwn import *
import struct

def encrypt(b_arr):
    b = bytearray(b_arr)
    for i in range(len(b_arr) - 3):
        # XOR Byte
        t = b[i]
        t ^= 0x4c
        b[i] = t

        # ROR WORD
        t = struct.unpack('<H', b[i:i+2])[0]
        t = ror(t, 9, word_size = 16)
        b[i:i+2] = bytearray(struct.pack("<H", t))

        # ROL DWORD
        t = struct.unpack('<I', b[i:i+4])[0]
        t = rol(t, 7, word_size = 32)
        b[i:i+4] = bytearray(struct.pack("<I", t))
    return str(b)

def decrypt(b_arr):
    b = bytearray(b_arr)
    for i in range(len(b_arr) - 5, -1, -1):
        # ROR DWORD
        t = struct.unpack('<I', b[i:i+4])[0]
        t = ror(t, 7, word_size = 32)
        b[i:i+4] = bytearray(struct.pack("<I", t))

        # ROL WORD
        t = struct.unpack('<H', b[i:i+2])[0]
        t = rol(t, 9, word_size = 16)
        b[i:i+2] = bytearray(struct.pack("<H", t))

        # XOR Byte
        t = b[i]
        t ^= 0x4c
        b[i] = t
             
    return str(b)



b = """37 a1 2a a3 bd 33 22 ba  2f 3c 98 aa b9 2f 39 a4
18 33 aa 9a 2f 39 18 b3 24 3a af 18 99 1c 1c b1
19 b1 98 1b 3e 59 ae a6 00""".replace(" ", "").replace("\n", "").decode("hex")

d = decrypt(b)
log.success("Flag: {}".format(d))
```

The output:
```console
root@kali:/media/sf_CTFs/pico/special-pw# python solve.py
[+] Flag: picoCTF{gEt_y0Ur_sH1fT5_r1gHt_0389c2b16}\x00
```

The flag: picoCTF{gEt_y0Ur_sH1fT5_r1gHt_0389c2b16}