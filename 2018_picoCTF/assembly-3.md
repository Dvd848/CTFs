# assembly-3
Reversing, 400 points

## Description:
> What does asm3(0xb3fb1998,0xfe1a474d,0xd5373fd4) return? Submit the flag as a hexadecimal value (starting with '0x').

```assembly
.intel_syntax noprefix
.bits 32
	
.global asm3

asm3:
	push   	ebp
	mov    	ebp,esp
	mov	eax,0x62
	xor	al,al
	mov	ah,BYTE PTR [ebp+0xa]
	sal	ax,0x10
	sub	al,BYTE PTR [ebp+0xd]
	add	ah,BYTE PTR [ebp+0xe]
	xor	ax,WORD PTR [ebp+0x10]
	mov	esp, ebp
	pop	ebp
	ret
```


## Solution:

This is how the stack looks like after performing the `mov ebp,esp` command:

```
+---------------+
| old ebp       | <-- ebp
+---------------+
| ret           | <-- ebp + 0x4
+---------------+
| 0xb3fb1998    | <-- ebp + 0x8 (arg1)
+---------------+
| 0xfe1a474d    | <-- ebp + 0xc (arg2)
+---------------+
| 0xd5373fd4    | <-- ebp + 0x10 (arg3)
+---------------+
```

And due to endianness (little-endian), this is how the stack looks like, relative to ebp:
```
Byte grouping:

+0x8 +0x9 +0xA +0xB +0xC +0xD +0xE +0xF +0x10 0x11 0x12 0x13
+----+----+----+----+----+----+----+----+----+----+----+----+
| 98 | 19 | fb | b3 | 4d | 47 | 1a | fe | d4 | 3f | 37 | d5 |
+----+----+----+----+----+----+----+----+----+----+----+----+

Word grouping:
+0x8   +0xA   +0xC   +0xE   +0x10  +0x12
+------+------+------+------+------+------+
| 1998 | b3fb | 474d | fe1a | 3fd4 | d537 |
+------+------+------+------+------+------+
```

What happens later is:
```assembly
	push   	ebp
	mov    	ebp,esp
	mov	eax,0x62
	xor	al,al                      ; eax = 0
	mov	ah,BYTE PTR [ebp+0xa]      ; ah = 0xfb (ax = 0xfb00)
	sal	ax,0x10                    ; ax = 0
	sub	al,BYTE PTR [ebp+0xd]      ; al = 0 - 0x47 = 0xb9
	add	ah,BYTE PTR [ebp+0xe]      ; ah = 0 + 0x1a => ax = 0x1ab9
	xor	ax,WORD PTR [ebp+0x10]     ; ax ^ 0x3fd4
	mov	esp, ebp
	pop	ebp
	ret
```


So the flag is 0x1ab9 ^ 0x3fd4 = 0x256d.
