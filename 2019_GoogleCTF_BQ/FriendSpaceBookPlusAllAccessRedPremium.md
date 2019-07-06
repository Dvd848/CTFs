# FriendSpaceBookPlusAllAccessRedPremium.com
Category: Reversing

## Description
> Having snooped around like the expert spy you were never trained to be, you found something that takes your interest: "Cookie/www.FriendSpaceBookPlusAllAccessRedPremium.com" But unbeknownst to you, it was only the 700nm Wavelength herring rather than a delicious cookie that you could have found. It looks exactly like a credential for another system. You find yourself in search of a friendly book to read. Having already spent some time trying to find a way to gain more intelligence... and learn about those fluffy creatures, you (several)-momentarily divert your attention here. It's a place of all the individuals in the world sharing large amounts of data with one another. Strangely enough, all of the inhabitants seem to speak using this weird pictorial language. And there is hot disagreement over what the meaning of an eggplant is. But not much Cauliflower here. They must be very private creatures. SarahH has left open some proprietary tools, surely running this will take you to them. Decipher this language and move forth!

A `vm.py` script was attached:

```python
import sys

# Implements a simple stack-based VM
class VM:

  def __init__(self, rom):
    self.rom = rom
    self.accumulator1 = 0
    self.accumulator2 = 0
    self.instruction_pointer = 1
    self.stack = []

  def step(self):
    cur_ins = self.rom[self.instruction_pointer]
    self.instruction_pointer += 1

    fn = VM.OPERATIONS.get(cur_ins, None)

    if cur_ins[0] == '🖋':
      return
    if fn is None:
      raise RuntimeError("Unknown instruction '{}' at {}".format(
          repr(cur_ins), self.instruction_pointer - 1))
    else:
      fn(self)

  def add(self):
    self.stack.append(self.stack.pop() + self.stack.pop())

  def sub(self):
    a = self.stack.pop()
    b = self.stack.pop()
    self.stack.append(b - a)

  def if_zero(self):
    if self.stack[-1] == 0:
      while self.rom[self.instruction_pointer] != '😐':
        if self.rom[self.instruction_pointer] in ['🏀', '⛰']:
          break
        self.step()
    else:
      self.find_first_endif()
      self.instruction_pointer += 1

  def if_not_zero(self):
    if self.stack[-1] != 0:
      while self.rom[self.instruction_pointer] != '😐':
        if self.rom[self.instruction_pointer] in ['🏀', '⛰']:
          break
        self.step()
    else:
      self.find_first_endif()
      self.instruction_pointer += 1

  def find_first_endif(self):
    while self.rom[self.instruction_pointer] != '😐':
      self.instruction_pointer += 1

  def jump_to(self):
    marker = self.rom[self.instruction_pointer]
    if marker[0] != '💰':
      print('Incorrect symbol : ' + marker[0])
      raise SystemExit()
    marker = '🖋' + marker[1:]
    self.instruction_pointer = self.rom.index(marker) + 1

  def jump_top(self):
    self.instruction_pointer = self.stack.pop()

  def exit(self):
    print('\nDone.')
    raise SystemExit()

  def print_top(self):
    sys.stdout.write(chr(self.stack.pop()))
    sys.stdout.flush()

  def push(self):
    if self.rom[self.instruction_pointer] == '🥇':
      self.stack.append(self.accumulator1)
    elif self.rom[self.instruction_pointer] == '🥈':
      self.stack.append(self.accumulator2)
    else:
      raise RuntimeError('Unknown instruction {} at position {}'.format(
          self.rom[self.instruction_pointer], str(self.instruction_pointer)))
    self.instruction_pointer += 1

  def pop(self):
    if self.rom[self.instruction_pointer] == '🥇':
      self.accumulator1 = self.stack.pop()
    elif self.rom[self.instruction_pointer] == '🥈':
      self.accumulator2 = self.stack.pop()
    else:
      raise RuntimeError('Unknown instruction {} at position {}'.format(
          self.rom[self.instruction_pointer], str(self.instruction_pointer)))
    self.instruction_pointer += 1

  def pop_out(self):
    self.stack.pop()

  def load(self):
    num = 0

    if self.rom[self.instruction_pointer] == '🥇':
      acc = 1
    elif self.rom[self.instruction_pointer] == '🥈':
      acc = 2
    else:
      raise RuntimeError('Unknown instruction {} at position {}'.format(
          self.rom[self.instruction_pointer], str(self.instruction_pointer)))
    self.instruction_pointer += 1

    while self.rom[self.instruction_pointer] != '✋':
      num = num * 10 + (ord(self.rom[self.instruction_pointer][0]) - ord('0'))
      self.instruction_pointer += 1

    if acc == 1:
      self.accumulator1 = num
    else:
      self.accumulator2 = num

    self.instruction_pointer += 1

  def clone(self):
    self.stack.append(self.stack[-1])

  def multiply(self):
    a = self.stack.pop()
    b = self.stack.pop()
    self.stack.append(b * a)

  def divide(self):
    a = self.stack.pop()
    b = self.stack.pop()
    self.stack.append(b // a)

  def modulo(self):
    a = self.stack.pop()
    b = self.stack.pop()
    self.stack.append(b % a)

  def xor(self):
    a = self.stack.pop()
    b = self.stack.pop()
    self.stack.append(b ^ a)

  OPERATIONS = {
      '🍡': add,
      '🤡': clone,
      '📐': divide,
      '😲': if_zero,
      '😄': if_not_zero,
      '🏀': jump_to,
      '🚛': load,
      '📬': modulo,
      '⭐': multiply,
      '🍿': pop,
      '📤': pop_out,
      '🎤': print_top,
      '📥': push,
      '🔪': sub,
      '🌓': xor,
      '⛰': jump_top,
      '⌛': exit
  }


if __name__ == '__main__':
  if len(sys.argv) != 2:
    print('Missing program')
    raise SystemExit()

  with open(sys.argv[1], 'r') as f:
    print('Running ....')
    all_ins = ['']
    all_ins.extend(f.read().split())
    vm = VM(all_ins)

    while 1:
      vm.step()

```

A "program" was attached as well. 

```
🚛 🥇 0️⃣ ✋ 📥 🥇
🚛 🥇 1️⃣ 7️⃣ 4️⃣ 8️⃣ 8️⃣ ✋ 📥 🥇
🚛 🥇 1️⃣ 6️⃣ 7️⃣ 5️⃣ 8️⃣ ✋ 📥 🥇
🚛 🥇 1️⃣ 6️⃣ 5️⃣ 9️⃣ 9️⃣ ✋ 📥 🥇
🚛 🥇 1️⃣ 6️⃣ 2️⃣ 8️⃣ 5️⃣ ✋ 📥 🥇
🚛 🥇 1️⃣ 6️⃣ 0️⃣ 9️⃣ 4️⃣ ✋ 📥 🥇
🚛 🥇 1️⃣ 5️⃣ 5️⃣ 0️⃣ 5️⃣ ✋ 📥 🥇
// More ...
```

## Solution

This virtual machine is based on emoji 😲!

Let's start from running the program:

```console
root@kali:/media/sf_CTFs/google/FriendSpaceBookPlusAllAccessRedPremium.com# python3 vm.py program
Running ....
http://emoji-t0anaxn
```

It prints the beginning of a URI, character by character, and at some point stops. It seems that each character takes longer to print compared to the previous one.

The first thing we'll want to do is write a simple disassembler to translate the emoji into meaningful instructions. We do that using the VM itself as a base, and printing the different commands instead of executing them.

```python
import sys

class VM:

  def __init__(self, rom):
    self.rom = rom
    self.instruction_pointer = 1

  def step(self):
    cur_ins = self.rom[self.instruction_pointer]
    self.instruction_pointer += 1

    fn = VM.OPERATIONS.get(cur_ins, None)

    if cur_ins[0] == '🖋':
      print ("--- label ---")
      return
    elif cur_ins == '😐':
      print ("endif")
      return
    elif cur_ins == '✋':
      print ("stop_atoi?")
      return
    if fn is None:
      raise RuntimeError("Unknown instruction '{}' at {}".format(
          repr(cur_ins), self.instruction_pointer - 1))
    elif isinstance(fn, str):
      print (fn)
    else:
      fn(self)
   

  def jump_to(self):
    print ("jmp ", end='')
    marker = self.rom[self.instruction_pointer]
    if marker[0] != '💰':
      print('Incorrect symbol : ' + marker[0])
      raise SystemExit()
    marker = '🖋' + marker[1:]
    print(self.rom.index(marker) + 1)
    self.instruction_pointer += 1
 

  def push(self):
    print ("push ", end='')
    if self.rom[self.instruction_pointer] == '🥇':
      print("accumulator1")
    elif self.rom[self.instruction_pointer] == '🥈':
      print("accumulator2")
    else:
      raise RuntimeError('Unknown instruction {} at position {}'.format(
          self.rom[self.instruction_pointer], str(self.instruction_pointer)))
    self.instruction_pointer += 1

  def pop(self):
    print ("pop ", end='')
    if self.rom[self.instruction_pointer] == '🥇':
      print("accumulator1")
    elif self.rom[self.instruction_pointer] == '🥈':
      print("accumulator2")
    else:
      raise RuntimeError('Unknown instruction {} at position {}'.format(
          self.rom[self.instruction_pointer], str(self.instruction_pointer)))
    self.instruction_pointer += 1

  def load(self):
    print ("load ", end='')
    num = 0

    if self.rom[self.instruction_pointer] == '🥇':
      print("accumulator1 ", end='')
    elif self.rom[self.instruction_pointer] == '🥈':
      print("accumulator2 ", end='')
    else:
      raise RuntimeError('Unknown instruction {} at position {}'.format(
          self.rom[self.instruction_pointer], str(self.instruction_pointer)))
    self.instruction_pointer += 1

    while self.rom[self.instruction_pointer] != '✋':
      num = num * 10 + (ord(self.rom[self.instruction_pointer][0]) - ord('0'))
      self.instruction_pointer += 1

    print(num)

    self.instruction_pointer += 1

  OPERATIONS = {
      '🍡': "add",
      '🤡': "clone",
      '📐': "divide",
      '😲': "if_zero",
      '😄': "if_not_zero",
      '🏀': jump_to,
      '🚛': load,
      '📬': "modulo",
      '⭐': "multiply",
      '🍿': pop,
      '📤': "pop_out",
      '🎤': "print_top",
      '📥': push,
      '🔪': "sub",
      '🌓': "xor",
      '⛰': "jump_top",
      '⌛': "exit"
  }


if __name__ == '__main__':
  if len(sys.argv) != 2:
    print('Missing program')
    raise SystemExit()

  with open(sys.argv[1], 'r', encoding="utf8") as f:
    all_ins = ['']
    all_ins.extend(f.read().split())
    vm = VM(all_ins)

    while vm.instruction_pointer < len(all_ins):
      print("{}: ".format(vm.instruction_pointer), end='')
      vm.step()

```

We get the following program:
```
root@kali:/media/sf_CTFs/google/FriendSpaceBookPlusAllAccessRedPremium.com# python3 disassembler.py program
1: load accumulator1 0
5: push accumulator1
7: load accumulator1 17488
15: push accumulator1
17: load accumulator1 16758
25: push accumulator1
27: load accumulator1 16599
35: push accumulator1
37: load accumulator1 16285
45: push accumulator1
47: load accumulator1 16094
55: push accumulator1
57: load accumulator1 15505
65: push accumulator1
67: load accumulator1 15417
75: push accumulator1
77: load accumulator1 14832
85: push accumulator1
87: load accumulator1 14450
95: push accumulator1
97: load accumulator1 13893
105: push accumulator1
107: load accumulator1 13926
115: push accumulator1
117: load accumulator1 13437
125: push accumulator1
127: load accumulator1 12833
135: push accumulator1
137: load accumulator1 12741
145: push accumulator1
147: load accumulator1 12533
155: push accumulator1
157: load accumulator1 11504
165: push accumulator1
167: load accumulator1 11342
175: push accumulator1
177: load accumulator1 10503
185: push accumulator1
187: load accumulator1 10550
195: push accumulator1
197: load accumulator1 10319
205: push accumulator1
207: load accumulator1 975
213: push accumulator1
215: load accumulator1 1007
222: push accumulator1
224: load accumulator1 892
230: push accumulator1
232: load accumulator1 893
238: push accumulator1
240: load accumulator1 660
246: push accumulator1
248: load accumulator1 743
254: push accumulator1
256: load accumulator1 267
262: push accumulator1
264: load accumulator1 344
270: push accumulator1
272: load accumulator1 264
278: push accumulator1
280: load accumulator1 339
286: push accumulator1
288: load accumulator1 208
294: push accumulator1
296: load accumulator1 216
302: push accumulator1
304: load accumulator1 242
310: push accumulator1
312: load accumulator1 172
318: push accumulator1
320: load accumulator1 74
325: push accumulator1
327: load accumulator1 49
332: push accumulator1
334: load accumulator1 119
340: push accumulator1
342: load accumulator1 113
348: push accumulator1
350: load accumulator1 119
356: push accumulator1
358: load accumulator1 106
364: push accumulator1
366: load accumulator2 1
370: --- label ---
371: pop accumulator1
373: push accumulator2
375: push accumulator1
377: load accumulator1 389
383: push accumulator1
385: push accumulator2
387: jmp 1040
389: xor
390: print_top
391: load accumulator1 1
395: push accumulator1
397: add
398: pop accumulator2
400: if_not_zero
401: jmp 371
403: endif
404: load accumulator1 98426
412: push accumulator1
414: load accumulator1 97850
422: push accumulator1
424: load accumulator1 97604
432: push accumulator1
434: load accumulator1 97280
442: push accumulator1
444: load accumulator1 96815
452: push accumulator1
454: load accumulator1 96443
462: push accumulator1
464: load accumulator1 96354
472: push accumulator1
474: load accumulator1 95934
482: push accumulator1
484: load accumulator1 94865
492: push accumulator1
494: load accumulator1 94952
502: push accumulator1
504: load accumulator1 94669
512: push accumulator1
514: load accumulator1 94440
522: push accumulator1
524: load accumulator1 93969
532: push accumulator1
534: load accumulator1 93766
542: push accumulator1
544: load accumulator2 99
549: --- label ---
550: pop accumulator1
552: push accumulator2
554: push accumulator1
556: load accumulator1 568
562: push accumulator1
564: push accumulator2
566: jmp 1040
568: xor
569: print_top
570: load accumulator1 1
574: push accumulator1
576: add
577: pop accumulator2
579: if_not_zero
580: jmp 550
582: endif
583: load accumulator1 101141058
595: push accumulator1
597: load accumulator1 101060206
609: push accumulator1
611: load accumulator1 101030055
623: push accumulator1
625: load accumulator1 100998966
637: push accumulator1
639: load accumulator1 100887990
651: push accumulator1
653: load accumulator1 100767085
665: push accumulator1
667: load accumulator1 100707036
679: push accumulator1
681: load accumulator1 100656111
693: push accumulator1
695: load accumulator1 100404094
707: push accumulator1
709: load accumulator1 100160922
721: push accumulator1
723: load accumulator1 100131019
735: push accumulator1
737: load accumulator1 100111100
749: push accumulator1
751: load accumulator1 100059926
763: push accumulator1
765: load accumulator1 100049982
777: push accumulator1
779: load accumulator1 100030045
791: push accumulator1
793: load accumulator1 9989997
803: push accumulator1
805: load accumulator1 9981858
815: push accumulator1
817: load accumulator1 9980815
827: push accumulator1
829: load accumulator1 9978842
839: push accumulator1
841: load accumulator1 9965794
851: push accumulator1
853: load accumulator1 9957564
863: push accumulator1
865: load accumulator1 9938304
875: push accumulator1
877: load accumulator1 9935427
887: push accumulator1
889: load accumulator1 9932289
899: push accumulator1
901: load accumulator1 9931494
911: push accumulator1
913: load accumulator1 9927388
923: push accumulator1
925: load accumulator1 9926376
935: push accumulator1
937: load accumulator1 9923213
947: push accumulator1
949: load accumulator1 9921394
959: push accumulator1
961: load accumulator1 9919154
971: push accumulator1
973: load accumulator1 9918082
983: push accumulator1
985: load accumulator1 9916239
995: push accumulator1
997: load accumulator2 765
1003: --- label ---
1004: pop accumulator1
1006: push accumulator2
1008: push accumulator1
1010: load accumulator1 1023
1017: push accumulator1
1019: push accumulator2
1021: jmp 1040
1023: xor
1024: print_top
1025: load accumulator1 1
1029: push accumulator1
1031: add
1032: pop accumulator2
1034: if_not_zero
1035: jmp 1004
1037: endif
1038: exit
1039: --- label ---
1040: load accumulator1 2
1044: push accumulator1
1046: --- label ---
1047: jmp 1098
1049: --- label ---
1050: if_zero
1051: pop_out
1052: jmp 1088
1054: stop_atoi?
1055: endif
1056: pop_out
1057: jmp 1144
1059: --- label ---
1060: if_zero
1061: pop_out
1062: jmp 1088
1064: endif
1065: pop_out
1066: pop accumulator1
1068: load accumulator2 1
1072: push accumulator2
1074: sub
1075: if_zero
1076: pop_out
1077: pop accumulator2
1079: push accumulator1
1081: push accumulator2
1083: jump_top
1084: endif
1085: push accumulator1
1087: --- label ---
1088: load accumulator2 1
1092: push accumulator2
1094: add
1095: jmp 1047
1097: --- label ---
1098: clone
1099: load accumulator1 2
1103: push accumulator1
1105: --- label ---
1106: sub
1107: if_zero
1108: pop_out
1109: load accumulator1 1
1113: push accumulator1
1115: jmp 1050
1117: endif
1118: pop_out
1119: clone
1120: push accumulator1
1122: modulo
1123: if_zero
1124: jmp 1050
1126: endif
1127: pop_out
1128: clone
1129: push accumulator1
1131: load accumulator1 1
1135: push accumulator1
1137: add
1138: clone
1139: pop accumulator1
1141: jmp 1106
1143: --- label ---
1144: clone
1145: clone
1146: load accumulator2 0
1150: push accumulator2
1152: --- label ---
1153: load accumulator1 10
1158: push accumulator1
1160: multiply
1161: pop accumulator2
1163: push accumulator1
1165: modulo
1166: push accumulator2
1168: add
1169: pop accumulator2
1171: pop accumulator1
1173: clone
1174: push accumulator2
1176: sub
1177: if_zero
1178: pop_out
1179: load accumulator2 1
1183: push accumulator2
1185: jmp 1060
1187: endif
1188: pop_out
1189: push accumulator1
1191: load accumulator1 10
1196: push accumulator1
1198: divide
1199: if_zero
1200: jmp 1060
1202: endif
1203: clone
1204: push accumulator2
1206: jmp 1153
```

We can see that the program pushes some values to the stack, then operates on them, then pushes another group of values and so on.

We can go two ways from here: Static Analysis and Dynamic Analysis. Dynamic is usually faster so let's try that.

We hack the VM to print the instruction, the accumulators and the (top of the) stack for each instruction it executes.

For example, the program starts with:
```
1: load accumulator1 0
a1: 0, a2: 0, stack: []
5: push accumulator1
a1: 0, a2: 0, stack: [0]
7: load accumulator1 17488
a1: 17488, a2: 0, stack: [0]
15: push accumulator1
a1: 17488, a2: 0, stack: [0, 17488]
17: load accumulator1 16758
a1: 16758, a2: 0, stack: [0, 17488]
25: push accumulator1
a1: 16758, a2: 0, stack: [0, 17488, 16758]
```

The first interesting part, after all the values are loaded to the stack, is:
```
366: load accumulator2 1
370: --- label ---
371: pop accumulator1
373: push accumulator2
375: push accumulator1
377: load accumulator1 389
383: push accumulator1
385: push accumulator2
387: jmp 1040
389: xor 
390: print_top 
```

Let's add comments to this section:
```
366: load accumulator2 1 
370: --- label ---
371: pop accumulator1           ; load next value from stack to acc1
373: push accumulator2          ; push local variable #1 (1 for the first iteration, based on line 366)
375: push accumulator1          ; push local variable #2
377: load accumulator1 389      ; load return address
383: push accumulator1          ; push return address to stack
385: push accumulator2          ; push parameter to upcoming function
387: jmp 1040                   ; call function at 1040
389: xor                        ; XOR value returned by function via stack with local variable #2
390: print_top                  ; print the result
```

Here's the runtime data for this logic:
```
// Iteration #1
370: --- label ---
371: pop accumulator1
a1: 106, a2: 1, stack: [339, 208, 216, 242, 172, 74, 49, 119, 113, 119]
373: push accumulator2
a1: 106, a2: 1, stack: [208, 216, 242, 172, 74, 49, 119, 113, 119, 1]
375: push accumulator1
a1: 106, a2: 1, stack: [216, 242, 172, 74, 49, 119, 113, 119, 1, 106]
377: load accumulator1 389
a1: 389, a2: 1, stack: [216, 242, 172, 74, 49, 119, 113, 119, 1, 106]
383: push accumulator1
a1: 389, a2: 1, stack: [242, 172, 74, 49, 119, 113, 119, 1, 106, 389]
385: push accumulator2
a1: 389, a2: 1, stack: [172, 74, 49, 119, 113, 119, 1, 106, 389, 1]
387: jmp 1040
a1: 389, a2: 1, stack: [172, 74, 49, 119, 113, 119, 1, 106, 389, 1]
// Function 1040 runs
a1: 2, a2: 389, stack: [242, 172, 74, 49, 119, 113, 119, 1, 106, 2]
389: xor
a1: 2, a2: 389, stack: [216, 242, 172, 74, 49, 119, 113, 119, 1, 104]
390: print_top
h
a1: 2, a2: 389, stack: [208, 216, 242, 172, 74, 49, 119, 113, 119, 1]

// Iteration #2
371: pop accumulator1
a1: 119, a2: 2, stack: [264, 339, 208, 216, 242, 172, 74, 49, 119, 113]
373: push accumulator2
a1: 119, a2: 2, stack: [339, 208, 216, 242, 172, 74, 49, 119, 113, 2]
375: push accumulator1
a1: 119, a2: 2, stack: [208, 216, 242, 172, 74, 49, 119, 113, 2, 119]
377: load accumulator1 389
a1: 389, a2: 2, stack: [208, 216, 242, 172, 74, 49, 119, 113, 2, 119]
383: push accumulator1
a1: 389, a2: 2, stack: [216, 242, 172, 74, 49, 119, 113, 2, 119, 389]
385: push accumulator2
a1: 389, a2: 2, stack: [242, 172, 74, 49, 119, 113, 2, 119, 389, 2]
387: jmp 1040
a1: 389, a2: 2, stack: [242, 172, 74, 49, 119, 113, 2, 119, 389, 2]
// Function 1040 runs
a1: 3, a2: 389, stack: [216, 242, 172, 74, 49, 119, 113, 2, 119, 3]
389: xor
a1: 3, a2: 389, stack: [208, 216, 242, 172, 74, 49, 119, 113, 2, 116]
390: print_top
t
a1: 3, a2: 389, stack: [339, 208, 216, 242, 172, 74, 49, 119, 113, 2]

// Iteration #3
371: pop accumulator1
a1: 113, a2: 3, stack: [344, 264, 339, 208, 216, 242, 172, 74, 49, 119]
373: push accumulator2
a1: 113, a2: 3, stack: [264, 339, 208, 216, 242, 172, 74, 49, 119, 3]
375: push accumulator1
a1: 113, a2: 3, stack: [339, 208, 216, 242, 172, 74, 49, 119, 3, 113]
377: load accumulator1 389
a1: 389, a2: 3, stack: [339, 208, 216, 242, 172, 74, 49, 119, 3, 113]
383: push accumulator1
a1: 389, a2: 3, stack: [208, 216, 242, 172, 74, 49, 119, 3, 113, 389]
385: push accumulator2
a1: 389, a2: 3, stack: [216, 242, 172, 74, 49, 119, 3, 113, 389, 3]
387: jmp 1040
a1: 389, a2: 3, stack: [216, 242, 172, 74, 49, 119, 3, 113, 389, 3]
// Function 1040 runs
a1: 5, a2: 389, stack: [208, 216, 242, 172, 74, 49, 119, 3, 113, 5]
389: xor
a1: 5, a2: 389, stack: [339, 208, 216, 242, 172, 74, 49, 119, 3, 116]
390: print_top
t
a1: 5, a2: 389, stack: [264, 339, 208, 216, 242, 172, 74, 49, 119, 3]

// Iteration #4
371: pop accumulator1
a1: 119, a2: 4, stack: [267, 344, 264, 339, 208, 216, 242, 172, 74, 49]
373: push accumulator2
a1: 119, a2: 4, stack: [344, 264, 339, 208, 216, 242, 172, 74, 49, 4]
375: push accumulator1
a1: 119, a2: 4, stack: [264, 339, 208, 216, 242, 172, 74, 49, 4, 119]
377: load accumulator1 389
a1: 389, a2: 4, stack: [264, 339, 208, 216, 242, 172, 74, 49, 4, 119]
383: push accumulator1
a1: 389, a2: 4, stack: [339, 208, 216, 242, 172, 74, 49, 4, 119, 389]
385: push accumulator2
a1: 389, a2: 4, stack: [208, 216, 242, 172, 74, 49, 4, 119, 389, 4]
387: jmp 1040
a1: 389, a2: 4, stack: [208, 216, 242, 172, 74, 49, 4, 119, 389, 4]
// Function 1040 runs3
a1: 7, a2: 389, stack: [339, 208, 216, 242, 172, 74, 49, 4, 119, 7]
389: xor
a1: 7, a2: 389, stack: [264, 339, 208, 216, 242, 172, 74, 49, 4, 112]
390: print_top
p
a1: 7, a2: 389, stack: [344, 264, 339, 208, 216, 242, 172, 74, 49, 4]
```

These are the first four iterations which print out "http". We can see that the variable passed to `func_1040` is a counter.
The return value from the function is `[2, 3, 5, 7]` and if we go on, `[2, 3, 5, 7, 11, 101, 131, 151, 181, 191]`.
This return value is XORed with the original value from the stack, producing the plaintext.

Let's perform a quick test for this theory:
```python
>>> a = [2, 3, 5, 7, 11, 101, 131, 151, 181, 191]

>>> b = [106, 119, 113, 119, 49, 74, 172, 242, 216, 208]

>>> for x, y in zip(a, b):
...     sys.stdout.write(chr(x^y))
http://emo
```

Looks good! So, if we could predict what the function will return, we can use this to XOR with the values pushed to the stack and get the message.

If it isn't obvious from just staring at it, we can always search for the sequence in Google and come up with the official name: [Palindromic Primes](https://oeis.org/A002385): Prime numbers whose decimal expansion is a palindrome. The only catch: The program uses 99 as the index into the sequence for the second chunk of values (see line 544) and 765 for the third chunk (see line 997).

The following code decodes all three chunks:
```python
from itertools import *
from itertools import count

#https://stackoverflow.com/questions/34853034/find-palindromic-primes-in-python
                                         # ideone.com/aVndFM
def postponed_sieve():                   # postponed sieve, by Will Ness      
    yield 2; yield 3; yield 5; yield 7;  # original code David Eppstein, 
    sieve = {}                           #   Alex Martelli, ActiveState Recipe 2002
    ps = postponed_sieve()               # a separate base Primes Supply:
    p = next(ps) and next(ps)            # (3) a Prime to add to dict
    q = p*p                              # (9) its sQuare 
    for c in count(9,2):                 # the Candidate
        if c in sieve:               # c's a multiple of some base prime
            s = sieve.pop(c)         #     i.e. a composite ; or
        elif c < q:  
             yield c                 # a prime
             continue              
        else:   # (c==q):            # or the next base prime's square:
            s=count(q+2*p,2*p)       #    (9+6, by 6 : 15,21,27,33,...)
            p=next(ps)               #    (5)
            q=p*p                    #    (25)
        for m in s:                  # the next multiple 
            if m not in sieve:       # no duplicates
                break
        sieve[m] = s                 # original test entry: ideone.com/WFv4f

def palindPrimes(n):
    ret = []
    for p in islice( filterfalse( lambda x: str(x) != str(x)[::-1], 
                                  postponed_sieve()), n):
      ret.append(p)
    return ret

numbers1 = """
17488
16758
16599
16285
16094
15505
15417
14832
14450
13893
13926
13437
12833
12741
12533
11504
11342
10503
10550
10319
975
1007
892
893
660
743
267
344
264
339
208
216
242
172
74
49
119
113
119
106
"""

numbers2 = """
98426
97850
97604
97280
96815
96443
96354
95934
94865
94952
94669
94440
93969
93766
"""

numbers3 = """
101141058
101060206
101030055
100998966
100887990
100767085
100707036
100656111
100404094
100160922
100131019
100111100
100059926
100049982
100030045
9989997
9981858
9980815
9978842
9965794
9957564
9938304
9935427
9932289
9931494
9927388
9926376
9923213
9921394
9919154
9918082
9916239
"""

def numbers_to_list(s):
    return list(map(lambda x: int(x), s.strip().split("\n")[::-1]))

xor = palindPrimes(800)

numbers1 = numbers_to_list(numbers1)
numbers2 = numbers_to_list(numbers2)
numbers3 = numbers_to_list(numbers3)

for i in range(len(numbers1)):
    print(chr(numbers1[i] ^ xor[i]), end='')

for i in range(len(numbers2)):
    print(chr(numbers2[i] ^ xor[99 - 1 + i]), end='')

for i in range(len(numbers3)):
    print(chr(numbers3[i] ^ xor[765 - 1 + i]), end='')

```

The result:
```
http://emoji-t0anaxnr3nacpt4na.web.ctfcompetition.com/humans_and_cauliflowers_network/
```

Visiting the website brings us to a basic social network:
```console
root@kali:/media/sf_CTFs/google/FriendSpaceBookPlusAllAccessRedPremium.com# curl http://emoji-t0anaxnr3nacpt4na.web.ctfcompetition.com/humans_and_cauliflowers_network/
<!DOCTYPE html>
<meta charset="utf-8">
<title>Humans + Cauliflower network</title>
<body>
  <h1>Browse through dozens! of profiles</h1>
  <p>Some of our most active users:</p>
  <ul>
    <li><a href="jade.html">Jade</a></li>
    <li><a href="amber.html">Amber</a></li>
    <li><a href="sarah.html">SarahH</a></li>
  </ul>
  <p>Register now!</p>
</body>
```

After visiting several pages, we finally find the flag in Amber's profile:
![](images/amber.png)

The flag: CTF{Peace_from_Cauli!} 😄