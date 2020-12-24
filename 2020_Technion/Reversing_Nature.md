#  Reversing Nature 
Category: Reversing & Binary Exploitation

## Description
> Reversing binaries is lame, I want you to reverse nature

A binary file was attached.

## Solution

Let's run the file:

```console```
root@kali:/media/sf_CTFs/technion/Reversing_Nature# ./reversing_nature
Enter the key: test
You're not good enough at reversing nature, try again
```

So we need to enter some key. Let's open the binary in Ghirda:

```c
undefined8 main(void)

{
  ulong is_valid;
  long j;
  undefined8 *pUserInput;
  long in_FS_OFFSET;
  int key_len;
  int i;
  uint32_t user_input [100];
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  key_len = 0;
  j = 50;
  pUserInput = (undefined8 *)user_input;
  while (j != 0) {
    j = j + -1;
    *pUserInput = 0;
    pUserInput = pUserInput + 1;
  }
  printf("Enter the key: ");
  __isoc99_scanf(&DAT_00102030,&key_len);
  if (100 < key_len) {
    key_len = 100;
  }
  i = 0;
  while (i < key_len) {
    j = (long)i;
    i = i + 1;
    __isoc99_scanf(&DAT_00102030,user_input + j,user_input);
  }
  is_valid = verify_key(key_len + -1,(int *)user_input,0);
  if ((char)is_valid == '\0') {
    puts("You\'re not good enough at reversing nature, try again");
  }
  else {
    print_flag((long)user_input,key_len);
  }
  if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0;
}
```

We can see that the input the program is requesting is integers (read via `"%d"`). First, the program asks for an integer smaller than 100. Then, it reads that many integers from the user. Finally, it calls `verify_key` to check the input, and prints the key if the function returns successfully. So, we should understand what `verify_key` does:

```c
ulong verify_key(int param_1,int *p_user_input,int param_3)

{
  uint uVar1;
  ulong uVar2;
  
  if (1 < param_1) {
    if ((p_user_input[(long)param_1 + -2] + p_user_input[(long)param_1 + -1] == p_user_input[param_1]) &&
       (uVar2 = verify_key(param_1 + -1,p_user_input,p_user_input[param_1] + param_3), (char)uVar2 != '\0')) {
      uVar1 = 1;
    }
    else {
      uVar1 = 0;
    }
    return (ulong)uVar1;
  }
  if ((((param_1 == 1) && (p_user_input[1] == p_user_input[0])) && (p_user_input[1] == 1)) &&
     (param_3 == -0x53f35e20)) {
    return 1;
  }
  return 0;
}
```

We can see that `verify_key` is a recursive function. We want it to return `1`. There are two cases where this happens. In the first condition, for `uVar1` to be set to `1` we need `uVar2` to be different than `0`, and that happens if the recursive call to `verify_key` returns `1`. So, the condition that really interests us is the second one:

```c
if ((((param_1 == 1) && (p_user_input[1] == p_user_input[0])) && (p_user_input[1] == 1)) &&
     (param_3 == -0x53f35e20)) {
    return 1;
  }
```

For the condition to be true, we need the first two numbers we enter to be `1` and `1`. At the same time, we also need `param_1` to be `1`. We can see that `param_1` starts as the number of keys and is decremented for each recursive call. Therefore, if we reverse our way back up the call stack, we know that the calling function had `param_1` as `2`. If we made the recursive call, we need to assume that `p_user_input[(long)param_1 + -2] + p_user_input[(long)param_1 + -1] == p_user_input[param_1]` was true. So, we translate this to: `p_user_input[0] + p_user_input[1] == p_user_input[2]`, which means that the third number we should enter is `1 + 1`, i.e. `2`.

Let's repeat this one more time in case the pattern isn't clear yet. The calling function has `param_1` as `3`. Therefore the condition would be `p_user_input[1] + p_user_input[2] == p_user_input[3]`, i.e. the forth number is `1 + 2`, i.e. `3`. It should be pretty clear by now that we're talking about the Fibonacci series.

At this point, instead of trying to calculate the exact length of the Fibonacci series that we need to provide in order to have `param_3` equal `-0x53f35e20` when arriving to the base condition, we can just brute force all the lengths up to `100`:

```python
from pwn import *

def fib(n):
    a, b = 1, 1
    for _ in range(n):
        yield a
        a, b = b, a + b

for i in range(1, 100):
    with context.local(log_level='ERROR'): 
        p = process("./reversing_nature")
        p.sendline(str(i))
        for f in fib(i):
            p.sendline(str(f))
        out = p.recvall().decode("ascii")
        if "not good" not in out:
            print(f"i = {i}")
            print(out)
            break
```

Output:
```console
root@kali:/media/sf_CTFs/technion/Reversing_Nature# python3 solve.py
i = 50
Enter the key: Yeah maybe I exaggerated about reversing nature, the flag is cstechnion{th3_g0ld3n_r3v3r53}
```