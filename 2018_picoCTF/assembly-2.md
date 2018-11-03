# assembly-2
Reversing, 250 points

## Description:
> What does asm2(0x7,0x28) return? Submit the flag as a hexadecimal value (starting with '0x').  

```assembly
.intel_syntax noprefix
.bits 32
	
.global asm2

asm2:
	push   	ebp
	mov    	ebp,esp
	sub    	esp,0x10
	mov    	eax,DWORD PTR [ebp+0xc]
	mov 	DWORD PTR [ebp-0x4],eax
	mov    	eax,DWORD PTR [ebp+0x8]
	mov	DWORD PTR [ebp-0x8],eax
	jmp    	part_b
part_a:	
	add    	DWORD PTR [ebp-0x4],0x1
	add	DWORD PTR [ebp+0x8],0x76
part_b:	
	cmp    	DWORD PTR [ebp+0x8],0xa1de
	jle    	part_a
	mov    	eax,DWORD PTR [ebp-0x4]
	mov	esp,ebp
	pop	ebp
	ret
```


## Solution:

This is how the stack looks like after performing the `sub esp,0x10` command:

```
+---------+
|         | <-- ebp - 0x10 (local4)
+---------+
|         | <-- ebp - 0xc (local3)
+---------+
|         | <-- ebp - 0x8 (local2)
+---------+
|         | <-- ebp - 0x4 (local1)
+---------+
| old ebp | <-- ebp
+---------+
| ret     | <-- ebp + 0x4
+---------+
| 0x7     | <-- ebp + 0x8 (arg1)
+---------+
| 0x28    | <-- ebp + 0xc (arg2)
+---------+
```

What happens later is:
```assembly
	mov    	eax,DWORD PTR [ebp+0xc]	; eax = 0x28
	mov 	DWORD PTR [ebp-0x4],eax ; local1 = 0x28
	mov    	eax,DWORD PTR [ebp+0x8] ; eax = 0x7
	mov	    DWORD PTR [ebp-0x8],eax	; local2 = 0x7
	jmp    	part_b
```

We jump to part_b:
```assembly
part_b:	
	cmp    	DWORD PTR [ebp+0x8],0xa1de  ; compare local2 and 0xa1de
	jle    	part_a						; Jump to part_a, since 0x7 <= 0xa1de
	mov    	eax,DWORD PTR [ebp-0x4]
```

Jumping to part_a:
```assembly
part_a:	
	add    	DWORD PTR [ebp-0x4],0x1	; local1 += 1
	add	DWORD PTR [ebp+0x8],0x76	; local2 += 0x76
```

We then execute part_b again, comparing local2 and 0xa1de.

We do this until local2 > 0xa1de, then return local1.

In python, this is equal to:
```python
>>> local1 = 0x28
>>> local2 = 0x7
>>> while local2 <= 0xa1de:
...     local1 +=1
...     local2 += 0x76
...
>>> print(local1)
392
```

So the flag is 392 = 0x188.