# Unknown - Outside Secret Base
Category: Reversing

## Description

> You’re still located behind the tree, but now you have a plan on how to get in. You have taken notice that the guards are changing every 45 minutes, and when they do that, you have a gap of about 30 seconds to sneak past and get into the main entrance. It’s risky of course, but you have to give it a try. You run towards the entrance. The new guards have not come yet, but suddenly you hear voices closing in. You trip and fall in front of the main entrance. The voices are very close now, and you glimpse a coat that belongs to a guard behind the corner, but at the last moment you get up and enter! ...only to face another door, this one closed, with an electronic lock - this one brand new. The good news is that it seems you won't be bothered for some time, and whoever is entering or leaving most likely won't notice you if you stand really still next to the wall. You pull out your handy screwdriver, unscrew the panel next to it, and stare at the circuit board for a few seconds. It doesn't look bad, you can do it! You pull out your laptop, attach some wires, and dump the firmware in seconds. It looks fairly simple actually...
> 
Challenge: Just another keypad (rev)
> 
> You start to analyze the firmware, and uff, that's just a standard Linux. This does makes things easier (imagine having to reverse a custom 8051 RTOS on the spot!). You browse through the directories to find the lock controls, and spot both the right executable and THE SOURCE CODE! But wait... it's in Free Pascal?! Note: To make the final flag append "CTF{" prefix and "}" suffix to the keypad code you've got. E.g. if the code is 1234, the flag would be CTF{1234}

## Solution

We receive the following Pascal program:

```pascal
type
  DigitType = array[1..16] of SmallInt;

function CheckKeyPadCode(code : DigitType) : Boolean;
var
  digit : SmallInt;
  i : SmallInt;
  x, a, b, c, d : QWord;

begin
  x := 0;
  i := 0;

  For digit In code Do
  Begin
    If (digit < 0) or (digit > 9) then
    begin
      CheckKeyPadCode := False;
      Exit;
    end;

    x := x or (QWord(digit) shl i);
    i += 4;
  End;

  x := x xor &1275437152437512431354;
  x := RolQWord(x, 10);

  a := x and 1229782938247303441;
  b := x and &0210421042104210421042;
  c := x and RolQWord(1229782938247303441, 2);
  d := x and RolQWord(&0210421042104210421042, 2);

  if ((a = $0100101000011110) and (b = $2002220020022220) and
      (c = $4444040044044400) and (d = QWord($8880008080000880))) then
    CheckKeyPadCode := True
  else
    CheckKeyPadCode := False;
end;
```

It looks like we need to provide an input of 16 digits (`DigitType`), which after some manipulations will provide the expected values for `a`, `b`, `c` and `d`.

We'll first port the function to Python, then reverse the procedure. Noting that in Pascal `&` means octal and `$` means hex, we get:

```python

def ROL(data, shift, size=64):
    shift %= size
    remains = data >> (size - shift)
    body = (data << shift) - (remains << size )
    return (body + remains)

def ROR(data, shift, size=64):
    shift %= size
    body = data >> shift
    remains = (data << (size - shift)) - (body << size)
    return (body + remains)


def CheckKeyPadCode(code):
    x = 0
    i = 0

    for digit in code:
        if (digit < 0) or (digit > 9):
            return False

        x = x | (digit << i)
        i += 4


    x = x ^ 0o1275437152437512431354
    x = ROL(x, 10)

    a = x & 1229782938247303441
    b = x & 0o0210421042104210421042
    c = x & ROL(1229782938247303441, 2)
    d = x & ROL(0o0210421042104210421042, 2)

    if ((a == 0x0100101000011110) and (b == 0x2002220020022220) and
            (c == 0x4444040044044400) and (d == 0x8880008080000880)):
        return True
    else:
        return False
```

Now we start from the end result and work our way back up. The accepted values for  `a`, `b`, `c` and `d` are:

```python
x = 0

a = 0b0000000100000000000100000001000000000000000000010001000100010000 # 0x0100101000011110
b = 0b0010000000000010001000100000000000100000000000100010001000100000 # 0x2002220020022220
c = 0b0100010001000100000001000000000001000100000001000100010000000000 # 0x4444040044044400
d = 0b1000100010000000000000001000000010000000000000000000100010000000 # 0x8880008080000880
```

To receive these values, they are being masked with `1229782938247303441`, `0o0210421042104210421042`, `ROL(1229782938247303441, 2)` and `ROL(0o0210421042104210421042, 2)`. In a hex representation, that means:

```python
>>> hex(1229782938247303441)
'0x1111111111111111'
>>> hex(0o0210421042104210421042)
'0x2222222222222222'
>>> hex(ROL(1229782938247303441, 2))
'0x4444444444444444'
>>> hex(ROL(0o0210421042104210421042, 2))
'0x8888888888888888'
```

So these are very convenient values to mask with - we either take the first, second, third or forth bit of each nibble:

```python
x |= a & 0b0001000100010001000100010001000100010001000100010001000100010001 # 0x1111111111111111
x |= b & 0b0010001000100010001000100010001000100010001000100010001000100010 # 0x2222222222222222
x |= c & 0b0100010001000100010001000100010001000100010001000100010001000100 # 0x4444444444444444
x |= d & 0b1000100010001000100010001000100010001000100010001000100010001000 # 0x8888888888888888
```

Now we perform the ROR and XOR:

```python
x = ROR(x, 10)

x = x ^ 0o1275437152437512431354
```

And finally, extract the nibbles to recreate `DigitType`:

```python
reversed = format(x, 'x')[::-1]
res = list(map(int,reversed))
print(res)
assert(CheckKeyPadCode(res) == True)
print (f'CTF{{{reversed}}}')
```

We get:

```console
┌──(user@kali)-[/media/sf_CTFs/google/15_Unknown_-_Outside_Secret_Base]
└─$ python3 solve.py
[3, 3, 3, 3, 3, 1, 9, 5, 5, 2, 7, 9, 8, 5, 3, 4]
CTF{3333319552798534}
```