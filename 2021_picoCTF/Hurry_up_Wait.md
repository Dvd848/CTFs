# Hurry up! Wait!
Category: Reverse Engineering, 100 points

## Description

A binary file was attached.

## Solution

Let's open the file with Ghidra. We can see many function names prefixed with "ada". Most are very short, and one stands out:

```c
void FUN_0010298a(void)

{
  ada__calendar__delays__delay_for(1000000000000000);
  FUN_00102616();
  FUN_001024aa();
  FUN_00102372();
  FUN_001025e2();
  FUN_00102852();
  FUN_00102886();
  FUN_001028ba();
  FUN_00102922();
  FUN_001023a6();
  FUN_00102136();
  FUN_00102206();
  FUN_0010230a();
  FUN_00102206();
  FUN_0010257a();
  FUN_001028ee();
  FUN_0010240e();
  FUN_001026e6();
  FUN_00102782();
  FUN_001028ee();
  FUN_001023da();
  FUN_0010230a();
  FUN_0010233e();
  FUN_0010226e();
  FUN_001022a2();
  FUN_001023da();
  FUN_001021d2();
  FUN_00102956();
  return;
}
```

The first function seems to add a runtime delay for a very long time. Let's check what the next function does:

```c
void FUN_00102616(void)

{
  ada__text_io__put__4(&DAT_00102cd8,&DAT_00102cb8,&DAT_00102cb8,&DAT_00102cd8);
  return;
}
```

There are two different globals referenced here: `DAT_00102cd8` and `DAT_00102cb8`. Each one is referenced twice. What are they?

```assembly
                             DAT_00102cd8                                    XREF[3]:     FUN_00102616:0010261f(*), 
                                                                                          FUN_00102616:0010262d(*), 
                                                                                          FUN_00102616:00102636(*)  
        00102cd8 70              ??         70h    p

; ...

                             DAT_00102cb8                                    XREF[84]:    FUN_00102102:00102112(*), 
                                                                                          FUN_00102102:00102125(*), 
                                                                                          FUN_00102206:00102216(*), 
                                                                                          FUN_00102206:00102229(*), 
                                                                                          FUN_0010223a:0010224a(*), 
                                                                                          FUN_0010223a:0010225d(*), 
                                                                                          FUN_0010230a:0010231a(*), 
                                                                                          FUN_0010230a:0010232d(*), 
                                                                                          FUN_0010233e:0010234e(*), 
                                                                                          FUN_0010240e:0010241e(*), 
                                                                                          FUN_0010240e:00102431(*), 
                                                                                          FUN_00102512:00102522(*), 
                                                                                          FUN_00102512:00102535(*), 
                                                                                          FUN_00102616:00102626(*), 
                                                                                          FUN_00102616:00102639(*), 
                                                                                          FUN_0010271a:0010272a(*), 
                                                                                          FUN_0010271a:0010273d(*), 
                                                                                          FUN_0010281e:0010282e(*), 
                                                                                          FUN_0010281e:00102841(*), 
                                                                                          FUN_00102922:00102932(*), [more]
        00102cb8 01              ??         01h
        00102cb9 00              ??         00h
        00102cba 00              ??         00h
        00102cbb 00              ??         00h
        00102cbc 01              ??         01h
        00102cbd 00              ??         00h
        00102cbe 00              ??         00h
        00102cbf 00              ??         00h

```

So `DAT_00102cd8` is `p`. What about the next function, `FUN_001024aa`?

```c
void FUN_001024aa(void)

{
  ada__text_io__put__4(&DAT_00102cd1,&DAT_00102cb8,&DAT_00102cb8,&DAT_00102cd1);
  return;
}
```

Again, we see the same `DAT_00102cb8` global from the previous function, together with a new global: `DAT_00102cd1`:

```assembly
                             DAT_00102cd1                                    XREF[3]:     FUN_001024aa:001024b3(*), 
                                                                                          FUN_001024aa:001024c1(*), 
                                                                                          FUN_001024aa:001024ca(*)  
        00102cd1 69              ??         69h    i

```

So `DAT_00102cd1` is `i`. This should be enough for us to assume that the first (and last) parameter to `ada__text_io__put__4` is a letter from the flag.

We can continue following the functions and taking note of the letter they print, or use the following Ghidra script to do it programmatically:

```python
import sys
def getAddress(offset):
    return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(offset)

listing = currentProgram.getListing()
functionManager = currentProgram.getFunctionManager()

main_func = getGlobalFunctions("FUN_0010298a")[0]

# Iterate the instructions that FUN_0010298a() is composed of
for codeUnit in listing.getCodeUnits(main_func.getBody(), True):

	if not codeUnit.toString().startswith("CALL"):
        # Ignore anything that isn't a "call"
		continue

	callee = functionManager.getFunctionAt(getAddress(str(codeUnit.getAddress(0))))
	if not callee.getName().startswith("FUN_"):
        # In practice - skip ada__calendar__delays__delay_for()
		continue

	for cu in listing.getCodeUnits(callee.getBody(), True):
        # Iterate the instructions that the callee is composed of
		if (not cu.toString().startswith("LEA RAX")):
            # Ignore anything that isn't LEA RAX, [addr]
            #  since that's the instruction that loads the flag character to be printed
			continue
		
        # Check what's at "addr" and print it
		sys.stdout.write(chr(getByte(getAddress(str(cu.getScalar(1))))))

print("")
```

Output:
```
Hurry_up_wait.py> Running...
picoCTF{d15a5m_ftw_eab78e4}
Hurry_up_wait.py> Finished!
```

The flag: `picoCTF{d15a5m_ftw_eab78e4}`