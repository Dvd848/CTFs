# Endgame
Category: Reversing, 300 Points

## Description

> This guy Thanos snapped his fingers and now the flag is scattered across the universe.
> 
> Can you be the hero that saves the world?

## Solution

All we get is the following binary:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Endgame]
└─$ xxd -g 1 flag
00000000: 1a 05 1b 03 ab ab ef 08 ff 80 1c b3 1d 91 ba cd  ................
00000010: ef 22 ff 80 1e 05 1f 05 cd ef ef 19 ff 80 1b 30  .".............0
00000020: 1d 08 dc bd ef 06 ff 80 1a 01 1b 02 cd ab 1b 05  ................
00000030: cd ab 1c 1e cd ac 1e b2 1f 05 cd ef cd ae 1b e7  ................
00000040: 1c c6 ab bc 1d c1 1e 62 ab de 1f ad ab df ab bd  .......b........
00000050: ab ab 1b 04 cd ab 1b fa 1c fa ab bc cd ab 1b f1  ................
00000060: 1c c8 ab bc ab ab 1d 7d cd ad 1b fa 1c 04 cd bc  .......}........
00000070: 1f 68 ba bf cd ab 1c fd ab ac 1c 64 cd ac 1b 0a  .h.........d....
00000080: 1c 40 cd bc cd ab 1b f3 1c 3a 1d dc ab cd ab bc  .@.......:......
00000090: ab ab 1f 04 cd af 1b 05 1c 64 cd bc cd ab 1b 03  .........d......
000000a0: 1c 64 cd cb cd bc 1d 32 ba bd cd ab 1f 03 cd cf  .d.....2........
000000b0: 1b 44 ab cb cd ac 1b 06 1c 0a cd bc cd bc cd bc  .D..............
000000c0: cd bc 1d 7b ab bd ab ab 1b c8 cd ab cd bc 1f 02  ...{............
000000d0: dc bf 1c 73 ba bc cd ab 1c 3e ab bc ab ab cd ff  ...s.....>......
000000e0: cd af 1c 35 ab bc 1d 02 dc bd cd ab 1b f9 ab ab  ...5............
000000f0: 1c 0a cd ac dc cd ab ac cd af 1b 01 ab ab 00 00  ................
```

What are we supposed to do with that? Well, we tried a few things, such as looking for patterns or interpreting as assembly (this is a reversing challenge after all), but nothing seemed to work. Then we got a hint from *liadadash* (thanks!) that allowed us to proceed. Let's take a look at the binary when setting the line width to 10:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Endgame]
└─$ xxd -g 1 -c 10 flag
00000000: 1a 05 1b 03 ab ab ef 08 ff 80  ..........
0000000a: 1c b3 1d 91 ba cd ef 22 ff 80  ......."..
00000014: 1e 05 1f 05 cd ef ef 19 ff 80  ..........
0000001e: 1b 30 1d 08 dc bd ef 06 ff 80  .0........
00000028: 1a 01 1b 02 cd ab 1b 05 cd ab  ..........
00000032: 1c 1e cd ac 1e b2 1f 05 cd ef  ..........
0000003c: cd ae 1b e7 1c c6 ab bc 1d c1  ..........
00000046: 1e 62 ab de 1f ad ab df ab bd  .b........
00000050: ab ab 1b 04 cd ab 1b fa 1c fa  ..........
0000005a: ab bc cd ab 1b f1 1c c8 ab bc  ..........
00000064: ab ab 1d 7d cd ad 1b fa 1c 04  ...}......
0000006e: cd bc 1f 68 ba bf cd ab 1c fd  ...h......
00000078: ab ac 1c 64 cd ac 1b 0a 1c 40  ...d.....@
00000082: cd bc cd ab 1b f3 1c 3a 1d dc  .......:..
0000008c: ab cd ab bc ab ab 1f 04 cd af  ..........
00000096: 1b 05 1c 64 cd bc cd ab 1b 03  ...d......
000000a0: 1c 64 cd cb cd bc 1d 32 ba bd  .d.....2..
000000aa: cd ab 1f 03 cd cf 1b 44 ab cb  .......D..
000000b4: cd ac 1b 06 1c 0a cd bc cd bc  ..........
000000be: cd bc cd bc 1d 7b ab bd ab ab  .....{....
000000c8: 1b c8 cd ab cd bc 1f 02 dc bf  ..........
000000d2: 1c 73 ba bc cd ab 1c 3e ab bc  .s.....>..
000000dc: ab ab cd ff cd af 1c 35 ab bc  .......5..
000000e6: 1d 02 dc bd cd ab 1b f9 ab ab  ..........
000000f0: 1c 0a cd ac dc cd ab ac cd af  ..........
000000fa: 1b 01 ab ab 00 00              ......
```

Notice how the first four lines form a nice pattern (which later is lost, but never mind that right now). 

```
1a 05 1b 03 ab ab ef 08 ff 80
1c b3 1d 91 ba cd ef 22 ff 80
1e 05 1f 05 cd ef ef 19 ff 80
1b 30 1d 08 dc bd ef 06 ff 80
```

If we study the relationship between the second, forth and eighth bytes, we can see that they are related in the following way:

```
0x05 + 0x03 = 0x08
0xb3 - 0x91 = 0x22
0x05 * 0x05 = 0x19
0x30 / 0x08 = 0x06
```

What do the other bytes mean? It's hard to say for sure, but if we must guess then the seventh byte (which is always `0xef`) is probably translated to "equals". The last two bytes (`0xff 0x80`) might mean something like "end of expression" or "end of example" (we don't see them after the forth line). That leaves the first, third, fifth and sixth bytes. If you stare at it enough, the first part might look a bit like variable assignment: `A = 5, B = 3` etc. Now, we can assume that the mathematical operation needs to be encoded somewhere inside, and we have two candidates left: The fifth and the sixth byte. This also requires some leap of faith, but since the nibbles in the sixth byte always match the variable names, we'll assume that they are related to the order of operands and that the fifth byte encodes the operator.

Now, we proceed to the body, which when formatted according to the rules above, gives us the following pairs, where each pair of bytes encodes a command:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Endgame]
└─$ hexdump -e '2/1 "%02x " "\n"' -s $((10*4)) -v flag
1a 01
1b 02
cd ab
1b 05
cd ab
1c 1e
cd ac
1e b2
1f 05
cd ef
cd ae
1b e7
1c c6
ab bc
1d c1
1e 62
ab de
1f ad
ab df
ab bd
ab ab
1b 04
cd ab
1b fa
1c fa
ab bc
cd ab
1b f1
1c c8
ab bc
ab ab
1d 7d
cd ad
1b fa
1c 04
cd bc
1f 68
ba bf
cd ab
1c fd
ab ac
1c 64
cd ac
1b 0a
1c 40
cd bc
cd ab
1b f3
1c 3a
1d dc
ab cd
ab bc
ab ab
1f 04
cd af
1b 05
1c 64
cd bc
cd ab
1b 03
1c 64
cd cb
cd bc
1d 32
ba bd
cd ab
1f 03
cd cf
1b 44
ab cb
cd ac
1b 06
1c 0a
cd bc
cd bc
cd bc
cd bc
1d 7b
ab bd
ab ab
1b c8
cd ab
cd bc
1f 02
dc bf
1c 73
ba bc
cd ab
1c 3e
ab bc
ab ab
cd ff
cd af
1c 35
ab bc
1d 02
dc bd
cd ab
1b f9
ab ab
1c 0a
cd ac
dc cd
ab ac
cd af
1b 01
ab ab
00 00
```

As we can see, the first three commands are:

```
1a 01
1b 02
cd ab
```

According to our assumptions, it's clear that `1a 01` means `A = 1` and `1b 02` means `B = 2`, but we said that `cd ab` means "`A * B`". Where does the result go to? Stacking up another assumption, we'll hope that it means `A *= B`, so the result is saved in the first variable.

Now, we can go ahead and implement a simple interpreter:

```python
from collections import namedtuple
from pprint import pprint

Operation = namedtuple("Operation", "representation function")

variables = { }

OPS = {
    0xAB: Operation("+",  lambda x, y: x + y),
    0xBA: Operation("-",  lambda x, y: x - y),
    0xCD: Operation("*",  lambda x, y: x * y),
    0xDC: Operation("//", lambda x, y: x // y)
}

with open("flag", "rb") as f:
    f.seek(10 * 4)
    while True:
        b = f.read(1)[0]
        if b == 0x0:
            print("END")
            break
        elif b >> 4 == 1:
            var_name = b & 0xF
            var_value = f.read(1)[0]
            print(f"SET {var_name:X} = {var_value}")
            variables[var_name] = var_value
        elif b in OPS:
            op = OPS[b]
            vars = f.read(1)[0]
            first_var  = vars >> 4
            second_var = vars & 0xF
            print(f"{first_var:X} = {first_var:X} {op.representation} {second_var:X}")
            variables[first_var] = op.function(variables[first_var], variables[second_var])
        else:
            raise Exception(f"Unknown command '0x{b:02X}'")

print("\nVariables:")
pprint(variables)
```

Running it, we get:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Endgame]
└─$ python3 solve.py
SET A = 1
SET B = 2
A = A * B
SET B = 5
A = A * B
SET C = 30
A = A * C
SET E = 178
SET F = 5
E = E * F
A = A * E
SET B = 231
SET C = 198
B = B + C
SET D = 193
SET E = 98
D = D + E
SET F = 173
D = D + F
B = B + D
A = A + B
SET B = 4
A = A * B
SET B = 250
SET C = 250
B = B + C
A = A * B
SET B = 241
SET C = 200
B = B + C
A = A + B
SET D = 125
A = A * D
SET B = 250
SET C = 4
B = B * C
SET F = 104
B = B - F
A = A * B
SET C = 253
A = A + C
SET C = 100
A = A * C
SET B = 10
SET C = 64
B = B * C
A = A * B
SET B = 243
SET C = 58
SET D = 220
C = C + D
B = B + C
A = A + B
SET F = 4
A = A * F
SET B = 5
SET C = 100
B = B * C
A = A * B
SET B = 3
SET C = 100
C = C * B
B = B * C
SET D = 50
B = B - D
A = A * B
SET F = 3
C = C * F
SET B = 68
C = C + B
A = A * C
SET B = 6
SET C = 10
B = B * C
B = B * C
B = B * C
B = B * C
SET D = 123
B = B + D
A = A + B
SET B = 200
A = A * B
B = B * C
SET F = 2
B = B // F
SET C = 115
B = B - C
A = A * B
SET C = 62
B = B + C
A = A + B
F = F * F
A = A * F
SET C = 53
B = B + C
SET D = 2
B = B // D
A = A * B
SET B = 249
A = A + B
SET C = 10
A = A * C
C = C // D
A = A + C
A = A * F
SET B = 1
A = A + B
END

Variables:
{10: 89490564489314326449816467341755769981, 11: 1, 12: 5, 13: 2, 14: 98, 15: 4}
```

Variable `A` looks like something we should decode:

```python
print(bytearray.fromhex(f"{variables[0xA]:02X}").decode())
```

As output, we get the flag: `CSA{cs@5s3mbl3d}`

### Acknowledgments

Thanks to *liadadash* for his help with this challenge!