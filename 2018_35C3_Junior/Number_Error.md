# Number Error
Misc.

## Description:
> The function `assert_number(num: number)` is merely a debug function for our Wee VM (WeeEm?). It proves additions always work. Just imagine the things that could go wrong if it wouldn't!  


## Solution:

This is a "Wee" challenge - see basic explanation [here](./Wee/).

The `assert_number` logic is as follows:
```typescript
externals.addFunction(
        "assert_number",
        [{name: "num", type: compiler.NumberType}], compiler.StringType,
        false,
        (num: number) => !isFinite(num) || isNaN(num) || num !== num + 1
            ? "NUMBERS WORK" : flags.NUMBER_ERROR
    )
```

I once had to work on a project which constantly required setting and clearing bits on a UINT32. I didn't like the calculator GUI so I wrote a [tiny Javascript utility](https://github.com/Dvd848/Online_Tools/blob/master/Bit_Parser.html) to help me. It worked well for 32 bits, but when trying the same logic on 64 bit integers, things started to break. That's where I learnt that Javascript has a [Number.MAX_SAFE_INTEGER](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Number/MAX_SAFE_INTEGER) definition:

> The MAX_SAFE_INTEGER constant has a value of 9007199254740991 (9,007,199,254,740,991 or ~9 quadrillion). The reasoning behind that number is that JavaScript uses double-precision floating-point format numbers as specified in IEEE 754 and can only safely represent numbers between -(253 - 1) and 253 - 1.
> 
> Safe in this context refers to the ability to represent integers exactly and to correctly compare them. For example, Number.MAX_SAFE_INTEGER + 1 === Number.MAX_SAFE_INTEGER + 2 will evaluate to true, which is mathematically incorrect. See Number.isSafeInteger() for more information.

So, to hit the assertion, we need to use:
```python
import requests

r = requests.post('http://35.207.189.79/wee/run', json={"code": "alert(assert_number(9007199254740991 + 1))"})
print(r.text)
```

The output:
```console
{"code":"alert(assert_number(9007199254740991 + 1))","result":"35C3_THE_AMOUNT_OF_INPRECISE_EXCEL_SH33TS\n"}
```

The flag: 35C3_THE_AMOUNT_OF_INPRECISE_EXCEL_SH33TS