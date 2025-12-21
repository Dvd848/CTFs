# Optimize the code

Category: Reversing

## Description

> You found the key to your next step, the binary that will give you the flag. Unfortunately it's too slow. If you understand the code behind it, you can optimize it and pass to the next step. Will you be able to crack the code?

A binary was attached.

## Solution

Let's run the binary. It starts printing the flag, but slows down after each character, and at
some point takes way too long:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Optimize_the_code]
└─$ ./challange.out
CTF{IXPWVBGRHIZRABLWRXYFNCZLUPTSVXCJVOTRUVZEN^C
```

So, let's check the decompilation output from Ghidra to see what's the logic we need to optimize:

```c

long rem(long param_1)
{
  long local_10;
  
  for (local_10 = param_1; 0x19 < local_10; local_10 = local_10 + -0x1a) {
  }
  return local_10;
}

undefined8 main(void)
{
  long lVar1;
  long lVar2;
  int local_1c;
  
  for (local_1c = 0; local_1c < 0x3c; local_1c = local_1c + 1) {
    lVar1 = fib(local_1c);
    lVar2 = fib(local_1c + 1);
    lVar1 = rem(lVar2 * 0xc + lVar1 * 0x11 + 0x10);
    putchar((int)lVar1 + 0x41);
    if (local_1c == 2) {
      putchar(0x7b);
    }
    fflush(stdout);
  }
  puts("}");
  return 0;
}

long fib(int param_1)
{
  long lVar1;
  long lVar2;
  
  if (param_1 < 2) {
    lVar1 = (long)param_1;
  }
  else {
    lVar2 = fib(param_1 + -1);
    lVar1 = fib(param_1 + -2);
    lVar1 = lVar1 + lVar2;
  }
  return lVar1;
}
```

Looks like we have two functions to optimize here: `rem` and `fib`. The `fib` function
obviously calculates the Fibonacci series, and it's trivial to convert it from a recursive
function to a much faster while-loop-based function:

```python
def fibonacci(n):
    if n == 0:
        return 0
    elif n == 1:
        return 1

    a, b = 0, 1
    for _ in range(2, n + 1):
        a, b = b, a + b
    return b
```

If we think about what `rem` is trying to achieve, we'll come to the conclusion that it simply
calculates the remainder from `26`:

```python
def rem(param_1):
    return param_1 % 26
```

Putting it all together, we add the main function:

```python
def main():
    for local_1c in range(60):
        lVar1 = fibonacci(local_1c)
        lVar2 = fibonacci(local_1c + 1)
        lVar1 = rem(lVar2 * 12 + lVar1 * 17 + 16)
        print(chr(lVar1 + 65), end='')

        if local_1c == 2:
            print('{', end='')

    print('}')
```

Output:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Optimize_the_code]
└─$ python3 solve.py
CTF{IXPWVBGRHIZRABLWRXYFNCZLUPTSVXCJVOTRUVZENBYJRKLFAPZYHPGFV}
```