# Me Tarjan, You Jane
Category: Logic, 60 Points

## Description

> Tarjan got lost in the jungle!
> 
> Can you help him to find the way to his beloved Jane?
> 
> We put our faith in you.
> 
> Good Luck!

Two files were attached.

## Solution

Let's take a look at the files.

The first file:
```console
root@kali:/media/sf_CTFs/checkpoint/Me_Tarjan_You_Jane/Tarjan# cat pairs.txt
[[9459011, 9459014], [8834567, 8834570], [16484715, 16484718], [10536999, 10537002], [5360200, 5360202], [9219552, 9219554], [12180815, 12180822], [6840232, 6840234], [9045060, 9045061], [11177208, 11177209], [11759735, 11759737], [14370512, 14370514], [5040815, 10081638], [14104364, 14104365], [5298400, 5298401], [520991, 520993], [4762656, 4762657], [3424404, 3424406], [1955787, 15646330], [15455827, 15455830], [16176083, 16176085], [16746452, 16746453], [5806147, 1451538], [8531676, 8531678], [847235, 6777909], [13936711, 13936714], [4484468, 4484470], [1783775, 1783777], [1515704, 757854], [1362647, 1362650], [9927216, 9927217], [12178935, 12178938], [15606523, 15606526], [8906755, 8906758], [3688248, 3688249], [13860120, 13860121], [7669927, 7669929], [11053915, 11053917], [10777908, 2694478], [12786419, 3196605], [13042600, 13042602], [15378273, 7689137], [15287808, 15287810], [1187667, 1187670], [5977260, 5977261], [8324863, 8324866], [1255851, 10046834], [2811044, 2811046]]
```

Like the name indicates, it's a list of pairs.

The second file:
```console
root@kali:/media/sf_CTFs/checkpoint/Me_Tarjan_You_Jane/Tarjan# xxd -g 1 tree.txt | head
00000000: 21 34 4d 4b 7d 3b 6d 4e 46 34 7e 77 33 25 3e 76  !4MK};mNF4~w3%>v
00000010: 2a 3f 55 68 33 7d 6c 2a 5a 2b 57 4e 3c 71 5d 6b  *?Uh3}l*Z+WN<q]k
00000020: 61 65 5a 64 3d 27 52 4a 29 73 61 46 52 35 6f 67  aeZd='RJ)saFR5og
00000030: 3e 2d 75 3c 24 59 6e 78 3c 75 69 2e 40 24 72 44  >-u<$Ynx<ui.@$rD
00000040: 2a 5a 76 23 4d 71 3f 34 28 4e 6e 29 28 77 76 2b  *Zv#Mq?4(Nn)(wv+
00000050: 48 72 23 66 35 48 75 67 3e 5e 75 69 2b 7a 53 38  Hr#f5Hug>^ui+zS8
00000060: 48 67 2b 53 64 6b 7b 65 25 3c 75 29 54 4f 59 4c  Hg+Sdk{e%<u)TOYL
00000070: 2b 72 44 74 73 79 3e 63 6e 63 2e 21 64 3a 78 43  +rDtsy>cnc.!d:xC
00000080: 5d 27 72 34 6a 5a 6e 62 42 24 22 6c 5d 63 4d 3f  ]'r4jZnbB$"l]cM?
00000090: 46 52 5b 40 35 79 6e 7b 27 67 67 30 57 73 42 57  FR[@5yn{'gg0WsBW
root@kali:/media/sf_CTFs/checkpoint/Me_Tarjan_You_Jane/Tarjan# xxd -g 1 tree.txt | tail
00ffff60: 2c 4a 4d 45 29 3c 36 7a 46 78 63 43 48 44 63 2c  ,JME)<6zFxcCHDc,
00ffff70: 28 59 4a 7b 36 60 2b 6e 52 5c 44 2f 2d 5e 3a 66  (YJ{6`+nR\D/-^:f
00ffff80: 43 3f 5f 4a 63 44 23 38 6d 58 4c 53 48 53 6a 7a  C?_JcD#8mXLSHSjz
00ffff90: 55 3a 61 6c 42 6d 35 6b 40 49 45 67 46 7e 27 6d  U:alBm5k@IEgF~'m
00ffffa0: 27 26 5f 33 34 6a 3a 34 2e 3b 24 49 67 63 64 3c  '&_34j:4.;$Igcd<
00ffffb0: 7a 69 44 67 46 62 6f 6a 70 7e 40 6d 41 69 24 22  ziDgFbojp~@mAi$"
00ffffc0: 70 63 70 79 70 6c 49 21 43 2b 63 57 21 26 4e 3f  pcpyplI!C+cW!&N?
00ffffd0: 65 30 69 28 35 42 22 6a 52 42 59 6a 26 56 7b 39  e0i(5B"jRBYj&V{9
00ffffe0: 35 60 73 6b 5d 44 45 2a 6f 6d 46 69 73 2e 30 6f  5`sk]DE*omFis.0o
00fffff0: 2d 74 49 64 66 4c 4a 4f 35 76 57 22 50 6b 34     -tIdfLJO5vW"Pk4
root@kali:/media/sf_CTFs/checkpoint/Me_Tarjan_You_Jane/Tarjan# ls -lh tree.txt
-rwxrwx--- 1 root vboxsf 16M Apr 29 08:31 tree.txt
```

This file is 16MB of random (printable) data.

Now, we have a `pairs` file, a `tree` file and `Tarjen` in the challenge name, this all points to [Tarjan's off-line lowest common ancestors algorithm](https://en.wikipedia.org/wiki/Tarjan%27s_off-line_lowest_common_ancestors_algorithm). This algorithm, given a list of pairs and a tree, computes the [lowest common ancestors](https://en.wikipedia.org/wiki/Lowest_common_ancestor) of each pair. 

Assuming this is true, we first need to determine how the tree is represented in `tree.txt`. If we assume that this is a binary tree, there's a [classic representation using an array](https://en.wikipedia.org/wiki/Binary_tree#Arrays). 

For example, the following tree:
```
       A    
     /   \
    B     C
  /  \   /
 D    E F
```

Is represented as the following array:
```
+---+---+---+---+---+---+
| A | B | C | D | E | F |
+---+---+---+---+---+---+
```

Now, let's continue with our assumptions, and assume that the numbers in the pair-list are node numbers for a complete binary tree indexed as:
```
       0    
     /   \
    1     2
  /  \   /
 3    4 5
```

We assume that the expectation is to find the lowest common ancestor of each pair. For example, given 3 and 4, the answer would be 1. Given 3 and 2, the answer would be 0.

We can implement Tarjan's algorithm but it's a bit complicated and that would be overkill in our case. For a complete binary tree, there's a much more [elegant solution](https://stackoverflow.com/questions/60223983/finding-common-parent-in-perfect-binary-tree).

In short, we move the tree to a 1-based indexing and write each index as a binary number:
```
              1
            /   \
           /     \
          /       \
         /         \
        10          11
       /  \        /  \
      /    \      /    \
     /      \    /      \
    100    101 110      111
    / \
   /   \
1000   1001
```

In this case, we can see that the encoding of the lowest common ancestor for a pair of nodes is the greatest common prefix of both nodes. For example, For 1000 and 111 the answer is 1, while for 1000 and 101 the answer is 10.

Let's use this to solve the challenge. Once we've found the lowest common ancestor for each pair of nodes, we'll just display the character which is associated with the parent in `tree.txt`.

```python
import json, os, mmap

# https://stackoverflow.com/questions/60223983/

def memory_map(filename, access=mmap.ACCESS_READ):
    size = os.path.getsize(filename)
    fd = os.open(filename, os.O_RDONLY)
    return mmap.mmap(fd, size, access=access)


with open("pairs.txt") as f, memory_map("tree.txt") as tree:
    list = json.loads(f.read())
    for pair in list:
        n1, n2 = pair
        
        # Move from 0-based index to 1-based index:
        m1 = n1 + 1
        m2 = n2 + 1

        # Convert to binary string
        b1 = format(m1, 'b')
        b2 = format(m2, 'b')

        # Find longest common prefix
        longest_common_prefix = os.path.commonprefix([b1, b2])

        # Convert back to 0-based index
        target = int(longest_common_prefix, 2) - 1
        print(chr(tree[target]), end = '')
```

Output:
```console
root@kali:/media/sf_CTFs/checkpoint/Me_Tarjan_You_Jane/Tarjan# python3 solve.py
CSA{Put_your_fa1th_1n_what_you_m0st_bel1ev3_in!}
```