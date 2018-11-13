# keygen-me-2
Reversing, 750 points

## Description:
> The software has been updated. Can you find us a new product key for the program?

A binary file was attached.


## Solution:

Inspecting the executable with Radare2 reveals that the program accepts the key only if a set of 12 constraints is met:
```assembly
[0x08048500]> s sym.validate_key
[0x08048cb7]> pdf
/ (fcn) sym.validate_key 325
|   sym.validate_key (int arg_8h);
|           ; var int local_ch @ ebp-0xc
|           ; arg int arg_8h @ ebp+0x8
|           ; CALL XREF from main (0x8048e78)
|           0x08048cb7      55             push ebp
|           0x08048cb8      89e5           mov ebp, esp
|           0x08048cba      83ec18         sub esp, 0x18
|           0x08048cbd      83ec0c         sub esp, 0xc
|           0x08048cc0      ff7508         push dword [arg_8h]
|           0x08048cc3      e8e8f7ffff     call sym.imp.strlen         ; size_t strlen(const char *s)
|           0x08048cc8      83c410         add esp, 0x10
|           0x08048ccb      8945f4         mov dword [local_ch], eax
|           0x08048cce      8b45f4         mov eax, dword [local_ch]
|           0x08048cd1      83ec08         sub esp, 8
|           0x08048cd4      50             push eax
|           0x08048cd5      ff7508         push dword [arg_8h]
|           0x08048cd8      e8b9faffff     call sym.key_constraint_01
|           0x08048cdd      83c410         add esp, 0x10
|           0x08048ce0      84c0           test al, al
|       ,=< 0x08048ce2      0f840d010000   je 0x8048df5
|       |   0x08048ce8      8b45f4         mov eax, dword [local_ch]
|       |   0x08048ceb      83ec08         sub esp, 8
|       |   0x08048cee      50             push eax
|       |   0x08048cef      ff7508         push dword [arg_8h]
|       |   0x08048cf2      e8f4faffff     call sym.key_constraint_02
|       |   0x08048cf7      83c410         add esp, 0x10
|       |   0x08048cfa      84c0           test al, al
|      ,==< 0x08048cfc      0f84f3000000   je 0x8048df5
|      ||   0x08048d02      8b45f4         mov eax, dword [local_ch]
|      ||   0x08048d05      83ec08         sub esp, 8
|      ||   0x08048d08      50             push eax
|      ||   0x08048d09      ff7508         push dword [arg_8h]
|      ||   0x08048d0c      e832fbffff     call sym.key_constraint_03
|      ||   0x08048d11      83c410         add esp, 0x10
|      ||   0x08048d14      84c0           test al, al
|     ,===< 0x08048d16      0f84d9000000   je 0x8048df5
|     |||   0x08048d1c      8b45f4         mov eax, dword [local_ch]
|     |||   0x08048d1f      83ec08         sub esp, 8
|     |||   0x08048d22      50             push eax
|     |||   0x08048d23      ff7508         push dword [arg_8h]
|     |||   0x08048d26      e86ffbffff     call sym.key_constraint_04
|     |||   0x08048d2b      83c410         add esp, 0x10
|     |||   0x08048d2e      84c0           test al, al
|    ,====< 0x08048d30      0f84bf000000   je 0x8048df5
|    ||||   0x08048d36      8b45f4         mov eax, dword [local_ch]
|    ||||   0x08048d39      83ec08         sub esp, 8
|    ||||   0x08048d3c      50             push eax
|    ||||   0x08048d3d      ff7508         push dword [arg_8h]
|    ||||   0x08048d40      e8cafbffff     call sym.key_constraint_05
|    ||||   0x08048d45      83c410         add esp, 0x10
|    ||||   0x08048d48      84c0           test al, al
|   ,=====< 0x08048d4a      0f84a5000000   je 0x8048df5
|   |||||   0x08048d50      8b45f4         mov eax, dword [local_ch]
|   |||||   0x08048d53      83ec08         sub esp, 8
|   |||||   0x08048d56      50             push eax
|   |||||   0x08048d57      ff7508         push dword [arg_8h]
|   |||||   0x08048d5a      e825fcffff     call sym.key_constraint_06
|   |||||   0x08048d5f      83c410         add esp, 0x10
|   |||||   0x08048d62      84c0           test al, al
|  ,======< 0x08048d64      0f848b000000   je 0x8048df5
|  ||||||   0x08048d6a      8b45f4         mov eax, dword [local_ch]
|  ||||||   0x08048d6d      83ec08         sub esp, 8
|  ||||||   0x08048d70      50             push eax
|  ||||||   0x08048d71      ff7508         push dword [arg_8h]
|  ||||||   0x08048d74      e880fcffff     call sym.key_constraint_07
|  ||||||   0x08048d79      83c410         add esp, 0x10
|  ||||||   0x08048d7c      84c0           test al, al
| ,=======< 0x08048d7e      7475           je 0x8048df5
| |||||||   0x08048d80      8b45f4         mov eax, dword [local_ch]
| |||||||   0x08048d83      83ec08         sub esp, 8
| |||||||   0x08048d86      50             push eax
| |||||||   0x08048d87      ff7508         push dword [arg_8h]
| |||||||   0x08048d8a      e8dffcffff     call sym.key_constraint_08
| |||||||   0x08048d8f      83c410         add esp, 0x10
| |||||||   0x08048d92      84c0           test al, al
| ========< 0x08048d94      745f           je 0x8048df5
| |||||||   0x08048d96      8b45f4         mov eax, dword [local_ch]
| |||||||   0x08048d99      83ec08         sub esp, 8
| |||||||   0x08048d9c      50             push eax
| |||||||   0x08048d9d      ff7508         push dword [arg_8h]
| |||||||   0x08048da0      e83efdffff     call sym.key_constraint_09
| |||||||   0x08048da5      83c410         add esp, 0x10
| |||||||   0x08048da8      84c0           test al, al
| ========< 0x08048daa      7449           je 0x8048df5
| |||||||   0x08048dac      8b45f4         mov eax, dword [local_ch]
| |||||||   0x08048daf      83ec08         sub esp, 8
| |||||||   0x08048db2      50             push eax
| |||||||   0x08048db3      ff7508         push dword [arg_8h]
| |||||||   0x08048db6      e89dfdffff     call sym.key_constraint_10
| |||||||   0x08048dbb      83c410         add esp, 0x10
| |||||||   0x08048dbe      84c0           test al, al
| ========< 0x08048dc0      7433           je 0x8048df5
| |||||||   0x08048dc2      8b45f4         mov eax, dword [local_ch]
| |||||||   0x08048dc5      83ec08         sub esp, 8
| |||||||   0x08048dc8      50             push eax
| |||||||   0x08048dc9      ff7508         push dword [arg_8h]
| |||||||   0x08048dcc      e8fcfdffff     call sym.key_constraint_11
| |||||||   0x08048dd1      83c410         add esp, 0x10
| |||||||   0x08048dd4      84c0           test al, al
| ========< 0x08048dd6      741d           je 0x8048df5
| |||||||   0x08048dd8      8b45f4         mov eax, dword [local_ch]
| |||||||   0x08048ddb      83ec08         sub esp, 8
| |||||||   0x08048dde      50             push eax
| |||||||   0x08048ddf      ff7508         push dword [arg_8h]
| |||||||   0x08048de2      e85bfeffff     call sym.key_constraint_12
| |||||||   0x08048de7      83c410         add esp, 0x10
| |||||||   0x08048dea      84c0           test al, al
| ========< 0x08048dec      7407           je 0x8048df5
| |||||||   0x08048dee      b801000000     mov eax, 1
| ========< 0x08048df3      eb05           jmp 0x8048dfa
| ```````-> 0x08048df5      b800000000     mov eax, 0
|           ; CODE XREF from sym.validate_key (0x8048df3)
| --------> 0x08048dfa      c9             leave
\           0x08048dfb      c3             ret
[0x08048cb7]>
```

For example, here is the first constraint:
```assembly
[0x08048796]> pdf
/ (fcn) sym.key_constraint_01 85
|   sym.key_constraint_01 (int arg_8h);
|           ; var int local_4h @ ebp-0x4
|           ; arg int arg_8h @ ebp+0x8
|           ; CALL XREF from sym.validate_key (0x8048cd8)
|           0x08048796      55             push ebp
|           0x08048797      89e5           mov ebp, esp
|           0x08048799      53             push ebx
|           0x0804879a      83ec04         sub esp, 4
|           0x0804879d      8b4508         mov eax, dword [arg_8h]     ; [0x8:4]=-1 ; 8
|           0x080487a0      0fb600         movzx eax, byte [eax]
|           0x080487a3      0fbec0         movsx eax, al
|           0x080487a6      83ec0c         sub esp, 0xc
|           0x080487a9      50             push eax
|           0x080487aa      e809ffffff     call sym.ord
|           0x080487af      83c410         add esp, 0x10
|           0x080487b2      0fbed8         movsx ebx, al
|           0x080487b5      8b4508         mov eax, dword [arg_8h]     ; [0x8:4]=-1 ; 8
|           0x080487b8      83c001         add eax, 1
|           0x080487bb      0fb600         movzx eax, byte [eax]
|           0x080487be      0fbec0         movsx eax, al
|           0x080487c1      83ec0c         sub esp, 0xc
|           0x080487c4      50             push eax
|           0x080487c5      e8eefeffff     call sym.ord
|           0x080487ca      83c410         add esp, 0x10
|           0x080487cd      0fbec0         movsx eax, al
|           0x080487d0      01d8           add eax, ebx
|           0x080487d2      83ec08         sub esp, 8
|           0x080487d5      6a24           push 0x24                   ; '$' ; 36
|           0x080487d7      50             push eax
|           0x080487d8      e894ffffff     call sym.mod
|           0x080487dd      83c410         add esp, 0x10
|           0x080487e0      83f80e         cmp eax, 0xe                ; 14
|           0x080487e3      0f94c0         sete al
|           0x080487e6      8b5dfc         mov ebx, dword [local_4h]
|           0x080487e9      c9             leave
\           0x080487ea      c3             ret
```

In words, this means that `(ord(key[0]) + ord(key[1])) % 36` need to be equal 14. 

Note that `ord` is implemented in the program in a non-standard way.

Translating the whole set of constraints to Python provides the following result:
```python
import string

def ord2(c):
    o = ord(c)
    if o >= ord('0') and o <= ord('9'):
        return o - ord('0')
    elif o >= ord('A') and o <= ord('Z'):
        return o - ord('7')
    raise Exception("Invalid character: '{}'".format(c))

def uninit(k, arr):
    for i in arr:
        if k[i] == '?':
            return True
    return False
        
def c1(k):
    if uninit(k, [0, 1]):
        return True
    return (ord2(k[0]) + ord2(k[1])) % 36 == 14

def c2(k):
    if uninit(k, [2, 3]):
        return True
    return (ord2(k[2]) + ord2(k[3])) % 36 == 24

def c3(k):
    if uninit(k, [2, 0]):
        return True
    return (ord2(k[2]) - ord2(k[0])) % 36 == 6

def c4(k):
    if uninit(k, [1, 3, 5]):
        return True
    return (ord2(k[1]) + ord2(k[3]) + ord2(k[5])) % 36 == 4

def c5(k):
    if uninit(k, [2, 4, 6]):
        return True
    return (ord2(k[2]) + ord2(k[4]) + ord2(k[6])) % 36 == 13

def c6(k):
    if uninit(k, [3, 4, 5]):
        return True
    return (ord2(k[3]) + ord2(k[4]) + ord2(k[5])) % 36 == 22

def c7(k):
    if uninit(k, [6, 8, 10]):
        return True
    return (ord2(k[6]) + ord2(k[8]) + ord2(k[10])) % 36 == 31

def c8(k):
    if uninit(k, [1, 4, 7]):
        return True
    return (ord2(k[1]) + ord2(k[4]) + ord2(k[7])) % 36 == 7

def c9(k):
    if uninit(k, [9, 12, 15]):
        return True
    return (ord2(k[9]) + ord2(k[12]) + ord2(k[15])) % 36 == 20

def c10(k):
    if uninit(k, [13, 14, 15]):
        return True
    return (ord2(k[13]) + ord2(k[14]) + ord2(k[15])) % 36 == 12

def c11(k):
    if uninit(k, [8, 9, 10]):
        return True
    return (ord2(k[8]) + ord2(k[9]) + ord2(k[10])) % 36 == 27

def c12(k):
    if uninit(k, [7, 12, 13]):
        return True
    return (ord2(k[7]) + ord2(k[12]) + ord2(k[13])) % 36 == 23
        
ALPHA = string.ascii_uppercase + string.digits

def is_safe(k):
    for func in [c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12]:
        if not func(k):
            return False
    return True

def try_place(k, index):
    if index == 16:
        print ("".join(k))
        return True
    
    for i in range(len(ALPHA)):
        k[index] = ALPHA[i]
        if is_safe(k):
            if try_place(k, index+1):
                return True
    k[index] = '?'
    return False

k = ['?']*16
try_place(k, 0)
```

The script searches for a result which fulfills all the constrains, and prints it.

The output:
```console
root@kali:/media/sf_CTFs/pico/keygen-me-2# python solve.py
A4G8MSBHA7AAAWD3
```

Using this key:
```console
$ ./activate A4G8MSBHA7AAAWD3
Product Activated Successfully: picoCTF{c0n5tr41nt_50lv1nG_15_W4y_f45t3r_2923966318}
```

The flag: picoCTF{c0n5tr41nt_50lv1nG_15_W4y_f45t3r_2923966318}