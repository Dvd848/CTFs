# asm2
Reverse Engineering, 250 points

## Description:
> What does asm2(0xc,0x15) return? Submit the flag as a hexadecimal value (starting with '0x').

```assembly
asm2:
	<+0>:	push   ebp
	<+1>:	mov    ebp,esp
	<+3>:	sub    esp,0x10
	<+6>:	mov    eax,DWORD PTR [ebp+0xc]
	<+9>:	mov    DWORD PTR [ebp-0x4],eax
	<+12>:	mov    eax,DWORD PTR [ebp+0x8]
	<+15>:	mov    DWORD PTR [ebp-0x8],eax
	<+18>:	jmp    0x50c <asm2+31>
	<+20>:	add    DWORD PTR [ebp-0x4],0x1
	<+24>:	add    DWORD PTR [ebp-0x8],0xaf
	<+31>:	cmp    DWORD PTR [ebp-0x8],0xa3d3
	<+38>:	jle    0x501 <asm2+20>
	<+40>:	mov    eax,DWORD PTR [ebp-0x4]
	<+43>:	leave  
	<+44>:	ret    
```

## Solution: 

This is how the stack looks after performing the `mov ebp,esp` command:

```
+---------+
| old ebp | <-- ebp
+---------+
| ret     | <-- ebp + 0x4
+---------+
| 0xc     | <-- ebp + 0x8
+---------+
| 0x15    | <-- ebp + 0xc
+---------+
```

What happens later is:

```assembly
	<+0>:	push   ebp
	<+1>:	mov    ebp,esp
	<+3>:	sub    esp,0x10
```

The stack becomes:
```
+---------+
|         | <-- ebp - 0x10 (local3)
+---------+
|         | <-- ebp - 0xc (local2)
+---------+
|         | <-- ebp - 0x8 (local1)
+---------+
|         | <-- ebp - 0x4 (local0)
+---------+
| old ebp | <-- ebp
+---------+
| ret     | <-- ebp + 0x4
+---------+
| 0xc     | <-- ebp + 0x8
+---------+
| 0x15    | <-- ebp + 0xc
+---------+

```

```assembly
	<+6>:	mov    eax,DWORD PTR [ebp+0xc]  ; eax = 0x15
	<+9>:	mov    DWORD PTR [ebp-0x4],eax  ; local0 = 0x15
	<+12>:	mov    eax,DWORD PTR [ebp+0x8]  ; eax = 0xc
	<+15>:	mov    DWORD PTR [ebp-0x8],eax  ; local1 = 0xc
	<+18>:	jmp    0x50c <asm2+31>
```

The stack:

```
+---------+
|         | <-- ebp - 0x10 (local3)
+---------+
|         | <-- ebp - 0xc (local2)
+---------+
| 0xc     | <-- ebp - 0x8 (local1)
+---------+
| 0x15    | <-- ebp - 0x4 (local0)
+---------+
| old ebp | <-- ebp
+---------+
| ret     | <-- ebp + 0x4
+---------+
| 0xc     | <-- ebp + 0x8
+---------+
| 0x15    | <-- ebp + 0xc
+---------+
```


```assembly
	<+20>:	add    DWORD PTR [ebp-0x4],0x1      ; local0 = 0x16
	<+24>:	add    DWORD PTR [ebp-0x8],0xaf     ; local1 = 0xbb
	<+31>:	cmp    DWORD PTR [ebp-0x8],0xa3d3   ; Compare 0xbb to 0xa3d3
	<+38>:	jle    0x501 <asm2+20>              ; Jump is taken (since 0xbb < 0xa3d3)
```

```
+---------+
|         | <-- ebp - 0x10
+---------+
|         | <-- ebp - 0xc
+---------+
| 0xbb    | <-- ebp - 0x8
+---------+
| 0x16    | <-- ebp - 0x4
+---------+
| old ebp | <-- ebp
+---------+
| ret     | <-- ebp + 0x4
+---------+
| 0xc     | <-- ebp + 0x8
+---------+
| 0x15    | <-- ebp + 0xc
+---------+
```

We run in a loop until local1 > 0xa3d3 (this takes 240 iterations). This means that we add `1` to `0x15` 240 times, bringing the value of `ebp - 0x4` to `0x105`.

Then:

```assembly
	<+40>:	mov    eax,DWORD PTR [ebp-0x4] ; eax = 0x105
	<+43>:	leave  
	<+44>:	ret    
```

The function returns with a return value of `0x105`.