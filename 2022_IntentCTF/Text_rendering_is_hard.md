# Text rendering is hard
* Category: Misc.
* 200 Points
* Solved by the JCTF Team

## Description

> Looking for text in the PDF structure is much like looking for the Cheshire Cat. It's almost completely invisible, only leaving trace hints that the file even contains text to those unfamiliar with the structure.
> 
> Try to understand how text is stored in PDF files and what happens when you tweak different parts to find the cipher key.

A PDF file was attached.

## Solution

We get a PDF file with the following text:

```
It was all very well to say SUBSTITUTE me but the wise little Alice was not going to do that in a hurry
No Ill look first she said and see whether its marked CMAP or fURmURS!c^u ODcdg! DOBNx
```

Well, we have some garbled text at the end, and there's a very thick hint towards a substitution cipher. 
We can clearly test that using the flag format:

```
fURmUR
INTENT
```

So we just need to find the key. Using `peepdf` we can browse the PDF internal objects, and find the following entry:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Text_rendering_is_hard]
└─$ python2 ~/utils/forensics/peepdf/peepdf.py -C "object 8" Text_rendering_is_hard.pdf

<< /Length 564
/Filter /FlateDecode >>
stream
/CIDInit/ProcSet findresource begin
12 dict begin
begincmap
/CIDSystemInfo<<
/Registry (Adobe)
/Ordering (UCS)
/Supplement 0
>> def
/CMapName/Adobe-Identity-UCS def
/CMapType 2 def
1 begincodespacerange
<00> <FF>
endcodespacerange
81 beginbfchar
<01> <0061> <15>
<02> <0062> <14>
<03> <0063> <27>
<04> <0064> <28>
<05> <0065> <0E>
<06> <0066> <0A>
<07> <0067> <34>
<08> <0068> <3A>
<09> <0069> <3E>
<0A> <006A> <47>
<0B> <006B> <4A>
<0C> <006C> <49>
<0D> <006D> <2A>
<0E> <006E> <29>
<0F> <006F> <07>
<10> <0070> <39>
<11> <0071> <20>
<12> <0072> <43>
<13> <0073> <4C>
<14> <0074> <03>
<15> <0075> <12>
<16> <0076> <4F>
<17> <0077> <45>
<18> <0078> <01>
<19> <0079> <21>
<1A> <007A> <44>
<1B> <0041> <16>
<1C> <0042> <30>
<1D> <0043> <38>
<1E> <0044> <05>
<1F> <0045> <0D>
<20> <0046> <02>
<21> <0047> <1F>
<22> <0048> <11>
<23> <0049> <06>
<24> <004A> <0C>
<25> <004B> <10>
<26> <004C> <3D>
<27> <004D> <46>
<28> <004E> <2F>
<29> <004F> <31>
<2A> <0050> <04>
<2B> <0051> <1D>
<2C> <0052> <23>
<2D> <0053> <09>
<2E> <0054> <2C>
<2F> <0055> <2B>
<30> <0056> <37>
<31> <0057> <3C>
<32> <0058> <2E>
<33> <0059> <22>
<34> <005A> <0B>
<35> <0031> <40>
<36> <0032> <0F>
<37> <0033> <1C>
<38> <0034> <36>
<39> <0035> <3F>
<3A> <0036> <08>
<3B> <0037> <4E>
<3C> <0038> <48>
<3D> <0039> <13>
<3E> <0030> <1E>
<3F> <0020> <33>
<40> <0021> <50>
<41> <0040> <42>
<42> <0023> <24>
<43> <0024> <49>
<44> <0025> <1B>
<45> <005E> <17>
<46> <0026> <4D>
<47> <002A> <41>
<48> <0028> <1A>
<49> <0029> <51>
<4A> <005B> <4B>
<4B> <005D> <25>
<4C> <007B> <2D>
<4D> <007D> <18>
<4E> <003A> <35>
<4F> <003F> <3B>
<50> <005C> <26>
<51> <002F> <19>
endbfchar
endcmap
CMapName currentdict /CMap defineresource pop
end
end

endstream
```

The object contains key words such as `begincmap` and `CMapName`. If we search for CMAP in the context of PDFs, we discover that:

> A CMap specifies the mapping from character codes to character selectors and serves the role of a font encoding. 

The first and last fields in the table look like indices, while the middle one looks like ASCII. Let's translate it:

```python
import os
import re
import subprocess

CMAP_REGEX = re.compile(r'<([0-9A-Z]+)> <([0-9A-Z]+)> <([0-9A-Z]+)>')

cmap_output = subprocess.run(['python2', os.path.expanduser('~/utils/forensics/peepdf/peepdf.py'), '-C', 'object 8', 'Text_rendering_is_hard.pdf'], capture_output=True)

for left, middle, right in CMAP_REGEX.findall(cmap_output.stdout.decode()):
    print(f"<{left}> <{middle} ({chr(int(middle, 16))})> <{right}>")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Text_rendering_is_hard]
└─$ python3 solve.py
<01> <0061 (a)> <15>
<02> <0062 (b)> <14>
<03> <0063 (c)> <27>
<04> <0064 (d)> <28>
<05> <0065 (e)> <0E>
<06> <0066 (f)> <0A>
<07> <0067 (g)> <34>
<08> <0068 (h)> <3A>
<09> <0069 (i)> <3E>
<0A> <006A (j)> <47>
<0B> <006B (k)> <4A>
<0C> <006C (l)> <49>
<0D> <006D (m)> <2A>
<0E> <006E (n)> <29>
<0F> <006F (o)> <07>
<10> <0070 (p)> <39>
<11> <0071 (q)> <20>
<12> <0072 (r)> <43>
<13> <0073 (s)> <4C>
<14> <0074 (t)> <03>
<15> <0075 (u)> <12>
<16> <0076 (v)> <4F>
<17> <0077 (w)> <45>
<18> <0078 (x)> <01>
<19> <0079 (y)> <21>
<1A> <007A (z)> <44>
<1B> <0041 (A)> <16>
<1C> <0042 (B)> <30>
<1D> <0043 (C)> <38>
<1E> <0044 (D)> <05>
<1F> <0045 (E)> <0D>
<20> <0046 (F)> <02>
<21> <0047 (G)> <1F>
<22> <0048 (H)> <11>
<23> <0049 (I)> <06>
<24> <004A (J)> <0C>
<25> <004B (K)> <10>
<26> <004C (L)> <3D>
<27> <004D (M)> <46>
<28> <004E (N)> <2F>
<29> <004F (O)> <31>
<2A> <0050 (P)> <04>
<2B> <0051 (Q)> <1D>
<2C> <0052 (R)> <23>
<2D> <0053 (S)> <09>
<2E> <0054 (T)> <2C>
<2F> <0055 (U)> <2B>
<30> <0056 (V)> <37>
<31> <0057 (W)> <3C>
<32> <0058 (X)> <2E>
<33> <0059 (Y)> <22>
<34> <005A (Z)> <0B>
<35> <0031 (1)> <40>
<36> <0032 (2)> <0F>
<37> <0033 (3)> <1C>
<38> <0034 (4)> <36>
<39> <0035 (5)> <3F>
<3A> <0036 (6)> <08>
<3B> <0037 (7)> <4E>
<3C> <0038 (8)> <48>
<3D> <0039 (9)> <13>
<3E> <0030 (0)> <1E>
<3F> <0020 ( )> <33>
<40> <0021 (!)> <50>
<41> <0040 (@)> <42>
<42> <0023 (#)> <24>
<43> <0024 ($)> <49>
<44> <0025 (%)> <1B>
<45> <005E (^)> <17>
<46> <0026 (&)> <4D>
<47> <002A (*)> <41>
<48> <0028 (()> <1A>
<49> <0029 ())> <51>
<4A> <005B ([)> <4B>
<4B> <005D (])> <25>
<4C> <007B ({)> <2D>
<4D> <007D (})> <18>
<4E> <003A (:)> <35>
<4F> <003F (?)> <3B>
<50> <005C (\)> <26>
<51> <002F (/)> <19>
```

So the first character we want to decode is an `f`, and we want it to be an `I`:

```
<06> <0066 (f)> <0A> --> <23> <0049 (I)> <06>
```

The `06` is common for both entries. What about the next character?

```
<2F> <0055 (U)> <2B> --> <28> <004E (N)> <2F>
```

The pattern should be clear, let's go ahead and decode:

```python
import os
import re
import subprocess

ENCRYPTED = "fURmURS!c^u ODcdg! DOBNx"

CMAP_REGEX = re.compile(r'<([0-9A-Z]+)> <([0-9A-Z]+)> <([0-9A-Z]+)>')

cmap_output = subprocess.run(['python2', os.path.expanduser('~/utils/forensics/peepdf/peepdf.py'), '-C', 'object 8', 'Text_rendering_is_hard.pdf'], capture_output=True)

index_to_char = {}
char_to_index = {}

for left, middle, right in CMAP_REGEX.findall(cmap_output.stdout.decode()):
    char = chr(int(middle, 16))
    index_to_char[right] = char
    char_to_index[char] = left

flag = ""
for c in ENCRYPTED:
    flag += index_to_char[char_to_index[c]]

print(flag)
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Text_rendering_is_hard]
└─$ python3 solve.py
INTENT{1twa5n0tPo150n3d}
```