# Wee R Leet
Misc.

## Description:
> Somebody forgot a useless assert function in the interpreter somewhere. In our agile development lifecycle somebody added the function early on to prove it's possible. Wev've only heared stories but apparently you can trigger it from Wee and it behaves differently for some "leet" input(?) What a joker. We will address this issue over the next few sprints. Hopefully it doesn't do any harm in the meantime.  


## Solution:

This is a "Wee" challenge - see basic explanation [here](./Wee/).

The `assert_leet` logic is as follows:
```typescript
externals.addFunction(
        "assert_leet",
        [{name: "maybe_leet", type: compiler.NumberType}], compiler.StringType,
        false,
        (maybe_leet: number) => maybe_leet !== 0x1337 ? "WEE AIN'T LEET" : flags.WEE_R_LEET
    )
```

The solution is pretty simple, call `assert_leet` with 0x1337:
```python
import requests
r = requests.post('http://35.207.189.79/wee/run', json={"code": "alert(assert_leet(4919))"})
print(r.text)
```

The flag: 35C3_HELLO_WEE_LI77LE_WORLD