# asm3
Reverse Engineering, 300 points

## Description:
> What does asm3(0xfe8cf7a4,0xf55018af,0xb8c70926) return?

```assembly
asm3:
	<+0>:	push   ebp
	<+1>:	mov    ebp,esp
	<+3>:	xor    eax,eax
	<+5>:	mov    ah,BYTE PTR [ebp+0x9]
	<+8>:	shl    ax,0x10
	<+12>:	sub    al,BYTE PTR [ebp+0xd]
	<+15>:	add    ah,BYTE PTR [ebp+0xe]
	<+18>:	xor    ax,WORD PTR [ebp+0x12]
	<+22>:	nop
	<+23>:	pop    ebp
	<+24>:	ret    
```

## Solution: 

Since this is a bit more complicated to analyze by hand, we can run it and inspect the result.

### Option #1: Using an Emulator

We can use [this](https://carlosrafaelgn.com.br/asm86/) online emulator to run the code, we just need to clean it up a bit and setup the stack:

```assembly
start:
    push 0xb8c70926
    push 0xf55018af
    push 0xfe8cf7a4
    call asm3

asm3:
	push   ebp
	mov    ebp,esp
	xor    eax,eax
	mov    ah,BYTE PTR [ebp+0x9]
	shl    ax,0x10
	sub    al,BYTE PTR [ebp+0xd]
	add    ah,BYTE PTR [ebp+0xe]
	xor    ax,WORD PTR [ebp+0x12]
	nop
	pop    ebp
	ret    
```

Using a breakpoint on the `ret` instruction, we see that the value is `0xe82f`.

### Option #2: Compiling the Code

Contents of `test.S`:
```assembly
.intel_syntax noprefix
.global asm3

asm3:
	push   ebp
	mov    ebp,esp
	xor    eax,eax
	mov    ah,BYTE PTR [ebp+0x9]
	shl    ax,0x10
	sub    al,BYTE PTR [ebp+0xd]
	add    ah,BYTE PTR [ebp+0xe]
	xor    ax,WORD PTR [ebp+0x12]
	nop
	pop    ebp
	ret    
```

Contents of `main.c`:
```assembly
#include <stdio.h>

int asm3(int, int, int);

int main(int argc, char* argv[])
{
    printf("0x%x\n", asm3(0xfe8cf7a4,0xf55018af,0xb8c70926));
    return 0;
}
```

Compiling and running:

```console
root@kali:/media/sf_CTFs/pico/asm3/out# gcc -masm=intel -m32 -c test.S -o test.o
root@kali:/media/sf_CTFs/pico/asm3/out# gcc -m32 -c main.c -o main.o
root@kali:/media/sf_CTFs/pico/asm3/out# gcc -m32 test.o main.o -o main
root@kali:/media/sf_CTFs/pico/asm3/out# ./main
0xe82f
```