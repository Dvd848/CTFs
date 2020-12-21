# babyrev
Category: Reversing & Binary Exploitation

## Description
> Another brick in the wall

A binary file was attached.

## Solution

Let's run the binary:

```console
root@kali:/media/sf_CTFs/technion/babyrev# ./babyrev
Enter Flag: test
This is not the flag...
```

The program is requesting the flag. Let's try to discover how it knows what the correct flag is.

We'll open the binary with Ghidra:

```c
undefined8 main(void)

{
  long lVar1;
  long in_FS_OFFSET;
  int i;
  char user_input [27];
  char expected_pass [27];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  printf("Enter Flag: ");
  user_input._0_8_ = 0;
  user_input._8_8_ = 0;
  user_input._16_8_ = 0;
  user_input._24_2_ = 0;
  user_input[26] = '\0';
  expected_pass._0_8_ = 0;
  expected_pass._8_8_ = 0;
  expected_pass._16_8_ = 0;
  expected_pass._24_2_ = 0;
  expected_pass[26] = '\0';
  __isoc99_scanf(&DAT_00102015,user_input);
  expected_pass._0_8_ = 0x696e686365747363;
  expected_pass._8_8_ = 0x745f57306e7b6e6f;
  expected_pass._16_8_ = 0x525f73695f733148;
  expected_pass._24_2_ = 0x7633;
  expected_pass[26] = '}';
  i = 0;
  do {
    if (26 < i) {
      puts("Good job! now go ahead and submit it");
LAB_001012ec:
      if (lVar1 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail();
      }
      return 0;
    }
    if (expected_pass[i] != user_input[i]) {
      puts("This is not the flag...");
      goto LAB_001012ec;
    }
    i = i + 1;
  } while( true );
}
```

This is a very simple program. The expected password is initialized and compared character by character to the user input. We can easily extract the password from the binary:

```python
>>> from pwn import *
>>> p64(0x696e686365747363) + p64(0x745f57306e7b6e6f) + p64(0x525f73695f733148) + p16(0x7633) + b'}'
b'cstechnion{n0W_tH1s_is_R3v}'
```