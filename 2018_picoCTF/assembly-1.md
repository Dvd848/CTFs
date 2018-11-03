# assembly-1
Reversing, 200 points

## Description:
> What does asm1(0x255) return? Submit the flag as a hexadecimal value (starting with '0x'). 

```assembly
.intel_syntax noprefix
.bits 32
	
.global asm1

asm1:
	push	ebp
	mov	ebp,esp
	cmp	DWORD PTR [ebp+0x8],0xea
	jg 	part_a	
	cmp	DWORD PTR [ebp+0x8],0x8
	jne	part_b
	mov	eax,DWORD PTR [ebp+0x8]
	add	eax,0x3
	jmp	part_d
part_a:
	cmp	DWORD PTR [ebp+0x8],0x6
	jne	part_c
	mov	eax,DWORD PTR [ebp+0x8]
	sub	eax,0x3
	jmp	part_d
part_b:
	mov	eax,DWORD PTR [ebp+0x8]
	sub	eax,0x3
	jmp	part_d
	cmp	DWORD PTR [ebp+0x8],0x126
	jne	part_c
	mov	eax,DWORD PTR [ebp+0x8]
	sub	eax,0x3
	jmp	part_d
part_c:
	mov	eax,DWORD PTR [ebp+0x8]
	add	eax,0x3
part_d:
	pop	ebp
	ret

```


## Solution:

This is how the stack looks like after performing the `mov	ebp,esp` command:

```
+---------+
| old ebp | <-- ebp
+---------+
| ret     | <-- ebp + 0x4
+---------+
| 0x255   | <-- ebp + 0x8
+---------+
```

What happens later is:
```assembly
	cmp	DWORD PTR [ebp+0x8],0xea ; compare 0x255 and 0xea
	jg 	part_a	                 ; jump is taken since 0x255 > 0xea
	cmp	DWORD PTR [ebp+0x8],0x8
	jne	part_b
	mov	eax,DWORD PTR [ebp+0x8]
	add	eax,0x3
	jmp	part_d
```

We jump to part_a:
```assembly
part_a:
	cmp	DWORD PTR [ebp+0x8],0x6 ; compare 0x255 and 0x6
	jne	part_c                  ; jump is taken, since 0x200 != 0x6
	mov	eax,DWORD PTR [ebp+0x8]
	sub	eax,0x3
	jmp	part_d
```

Jumping to part_c:
```assembly
part_c:
	mov	eax,DWORD PTR [ebp+0x8] ; eax = 0x255
	add	eax,0x3					; eax = 0x255 + 0x3 = 0x258
```

We move on to part_d, which performs the epilog and returns.

So the value returned by the function is 0x258.