# Hello World
Misc, 339 points

## Description:

> My first program!

The source code of a cpp file was attached.

## Solution:

Upon first inspection, the source code seems very innocent:

```c++
#include <iostream>
using namespace std;

int main()
{
	cout << "Hello, Worlds!\n";
	return 0;
}
```

However, the source code itself starts at line #148 of the file, and up to that line are empty lines. Or are they? A closer inspection shows that lines 1-145 contain spaces and tabs.

A visualization of this can be achieved using the following short script:

```python
with open ("hello_world.cpp") as f:
    contents = f.read()
    print contents.replace(" ", "\\s").replace("\t", "\\t")
```

Output:
```
\s\s\s\t\t\s\s\t\t\t
\s\s\s\t\t\s\t\s\s\t
\s\s\s\t\t\s\s\t\t\t
\s\s\s\t\t\s\s\t\s\t
\s\s\s\t\t\s\t\t\s\t
\s\s\s\t\t\t\t\s\t\t
\s\s\s\t\t\s\s\s\s
\s\s\s\t\t\s\t\s\s\s
\s\s\s\t\s\t\t\t\t\t
\s\s\s\t\t\s\t\t\s\t
\s\s\s\t\t\t\t\s\s\t
\s\s\s\t\s\t\t\t\t\t
\s\s\s\t\t\t\s\t\t\t
\s\s\s\t\t\s\t\s\s\s
\s\s\s\t\t\s\t\s\s
\s\s\s\t\t\t\s\t\s\s
\s\s\s\t\s\t\t\t\t\t
\s\s\s\t\t\t\s\s\t\t
\s\s\s\t\t\t\s\s\s\s
\s\s\s\t\t\s\t\s\s
\s\s\s\t\t\s\s\s\t\t
\s\s\s\t\t\s\s\s\t
\s\s\s\t\t\s\t\t\t\s
\s\s\s\t\t\s\s\t\t\t
\s\s\s\t\s\t\t\t\t\t
\s\s\s\t\t\t\t\s\s\t
\s\s\s\t\t\s\s\s\s
\s\s\s\t\t\t\s\t\s\t
\s\s\s\t\s\t\t\t\t\t
\s\s\s\t\t\s\t\s\s\s
\s\s\s\t\t\s\t\s\s
\s\s\s\t\t\t\s\t\t\s
\s\s\s\t\t\s\s\t\t
\s\s\s\t\t\t\t\t\s\t
\s\s\s\t\s\s\s\s\t
\s\s\s\t\t\s\s\t\s\t
\s\s\s\t\t\s\s\s\t\t
\s\s\s\t\t\s\s\s\s\t
\s\s\s\t\t\t\s\s\s\s
\s\s\s\t\t\t\s\s\t\t
\s\s\s\t\t\s\s\t\s\t
\s\s\s\t\t\t\s\t\s\s
\s\s\s\t\t\s\t\s\s\t
\s\s\s\t\t\s\t\s\s\s
\s\s\s\t\t\t\s\t\t\t
\s\s\s\t\s\s\s\s\s
\s\s\s\t\t\s\s\t\t\s
\s\s\s\t\t\s\t\t\t\t
\s\s\s\t\s\s\s\s\s
\s\s\s\t\t\t\s\t\s\s
\s\s\s\t\t\s\t\t\t\t
\s\s\s\t\t\s\t\t\s\s
\s\s\s\t\s\s\s\s\s
\s\s\s\t\t\s\s\s\s\t
\s\s\s\t\s\s\s\s\s
\s\s\s\t\t\t\s\s\t\t
\s\s\s\t\t\s\t\s\s\t
\s\s\s\t\s\s\s\s\s
\s\s\s\t\t\s\s\t\s\t
\s\s\s\t\t\t\s\s\t\s
\s\s\s\t\t\t\s\t\s\t
\s\s\s\t\t\t\s\s\t\t
\s\s\s\t\s\s\s\s\s
\s\s\s\t\t\t\s\t\s\s
\s\s\s\t\t\s\s\s\s\t
\s\s\s\t\t\s\t\s\s\s
\s\s\s\t\t\t\s\t\s\s
\s\s\s\t\s\s\s\s\s
\s\s\s\t\s\t\t\s\s
\s\s\s\t\t\s\s\t\s\t
\s\s\s\t\t\s\s\t\s\t
\s\s\s\t\t\s\s\t\t\t
\s\s\s\t\s\s\s\s\s
\s\s\s\t\t\t\t\s\s\t
\s\s\s\t\t\s\t\t\s\s
\s\s\s\t\t\s\t\t\s\s
\s\s\s\t\t\s\t\t\t\t
\s\s\s\t\t\s\s\t\t\t
\s\s\s\t\s\s\s\s\s
\s\s\s\t\t\t\s\t\s\s
\s\s\s\t\t\s\s\t\s\t
\s\s\s\t\t\s\s\t\s\t
\s\s\s\t\t\t\s\t\t\t
\s\s\s\t\t\t\s\s\t\t
\s\s\s\t\s\s\s\s\s
\s\s\s\t\t\s\t\t\s\s
\s\s\s\t\t\s\t\t\s\s
\s\s\s\t\t\s\s\t\s\t
\s\s\s\t\s\t\s\t\t\t
\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s\t
\s\s


#include\s<iostream>
using\snamespace\sstd;

int\smain()
{
\tcout\s<<\s"Hello,\sWorlds!\n";
\treturn\s0;
}

```

There's a programming language called [Whitespace](https://en.wikipedia.org/wiki/Whitespace_(programming_language)) that appears from time to time in CTFs.

A great online IDE for this language can be found [here](https://vii5ard.github.io/whitespace/).

Pasting the whitespaces into the IDE, we get:

```
Well sweet golly gee, that sure is a lot of whitespace! 
```

In addition, the memory tab shows:

```
Stack: [103, 105, 103, 101, 109, 123, 48, 104, 95, 109, 121, 95, 119, 104, 52, 116, 95, 115, 112, 52, 99, 49, 110, 103, 95, 121, 48, 117, 95, 104, 52, 118, 51, 125]
Heap: {	}
```

Read that as ASCII and we get:
```python
>>> "".join(chr(x) for x in [103, 105, 103, 101, 109, 123, 48, 104, 95, 109, 121, 95, 119, 104, 52, 116, 95, 115, 112, 52, 99, 49, 110, 103, 95, 121, 48, 117, 95, 104, 52, 118, 51, 125])
'gigem{0h_my_wh4t_sp4c1ng_y0u_h4v3}'
```