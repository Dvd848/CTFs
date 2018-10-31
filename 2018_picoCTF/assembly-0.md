# assembly-0
Reversing, 150 points

## Description:
> What does asm0(0xaa,0xf2) return? Submit the flag as a hexadecimal value (starting with '0x'). 

```assembly
.intel_syntax noprefix
.bits 32
	
.global asm0

asm0:
	push	ebp
	mov	ebp,esp
	mov	eax,DWORD PTR [ebp+0x8]
	mov	ebx,DWORD PTR [ebp+0xc]
	mov	eax,ebx
	mov	esp,ebp
	pop	ebp	
	ret
```


## Solution:

This is how the stack looks after performing the `mov	ebp,esp` command:

```
+---------+
| old ebp | <-- ebp
+---------+
| ret     | <-- ebp + 0x4
+---------+
| 0xaa    | <-- ebp + 0x8
+---------+
| 0xf2    | <-- ebp + 0xc
+---------+
```

What happens later is:
```assembly
	mov	eax,DWORD PTR [ebp+0x8]   ; eax = 0xaa
	mov	ebx,DWORD PTR [ebp+0xc]   ; ebx = 0xf2
	mov	eax,ebx                   ; eax = ebx, i.e. eax = 0xf2
	mov	esp,ebp                   ; teardown frame...
	pop	ebp	
	ret
```

So the function's return value is 0xf2.