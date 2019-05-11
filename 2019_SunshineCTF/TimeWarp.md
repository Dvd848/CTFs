# TimeWarp
Scripting, 50 points

## Description

> Oh no! A t3mp0ral anoma1y has di5rup7ed the timeline! Y0u'll have to 4nswer the qu3stion5 before we ask them!

## Solution

We connect to the service and get the following output:

```console
root@kali:/media/sf_CTFs/sunshine/TimeWarp# nc archive.sunshinectf.org 19004
I'm going to give you some numbers between 0 and 999.
Repeat them back to me in 30 seconds or less!
```

If we guess a number, we get the correct number as output:
```console
root@kali:/media/sf_CTFs/sunshine/TimeWarp# nc archive.sunshinectf.org 19004
I'm going to give you some numbers between 0 and 999.
Repeat them back to me in 30 seconds or less!
1
39
G3tting c0lder!
```

We can use that to proceed by opening a new session:
```console
root@kali:/media/sf_CTFs/sunshine/TimeWarp# nc archive.sunshinectf.org 19004
I'm going to give you some numbers between 0 and 999.
Repeat them back to me in 30 seconds or less!
39
39
Icr3dible!
1
61
00f, th4t can't b3 g00d!
```

At first I tried the following pwntools script, but it was too slow:
```python
from pwn import *
import time

def get_next_number(num_list):
    log.info("Starting to send new list of length {}".format(len(num_list)))
    with context.local(log_level='DEBUG'):
        r = remote("archive.sunshinectf.org", 19004)
        r.recvuntil("Repeat them back to me in 30 seconds or less!\n")
        for n in num_list:
            log.info("Sending {}".format(n))
            r.sendline(n)
            out = r.recvline()
            out += r.recvline()
            if "sun{" in out:
                print out
                return None
        r.sendline("1000")
        new_num = r.recvline()
        r.recvall()
        return new_num.rstrip()

num_list = []
while True:
    new_num = get_next_number(num_list)
    if new_num is None:
        break
    num_list.append(new_num)
    print num_list
    time.sleep(0.3)
```

The following bash script did the trick:

```bash
#!/bin/bash

output=""
numbers=()

while [[ ${output} != *"sun"* ]]; do
    output=$((printf '%s\n' "${numbers[@]}"; echo "9999") | nc archive.sunshinectf.org 19004)
    new_num=$(echo "$output" | tail -2 | egrep -x '[0-9]+')
    echo -n "$new_num "
    numbers+=("$new_num")
    #echo list: ${numbers[*]}
done

echo ""
echo "$output" | grep sun
```

Output:
```console
root@kali:/media/sf_CTFs/sunshine/TimeWarp# ./solve.sh
39 61 267 475 178 760 660 257 897 994 610 639 813 495 832 647 228 74 474 215 523 905 65 741 814 742 787 58 917 548 465 309 609 733 784 140 493 444 397 391 790 359 382 956 854 566 603 435 640 429 650 163 335 716 256 149 458 396 559 375 944 24 684 905 757 820 397 251 264 794 994 407 506 728 363 360 294 318 795 286 747 446 801 434 514 410 935 972 806 494 699 102 519 736 359 276 908 757 879 525 903 225 932 409 953 647 122 599 965 917 237 712 363 39 147 877 801 82 201 607 929 253 61 448 341 772 724 249 529 604 774 785 181 58 546 135 705 668 86 670 586 324 735 301 715 234 531 516 316 732 123 245 337 536 45 678 308 770 280 190 726 406 975 907 465 521 394 170 190 481 841 128 805 576 429 520 162 960 36 478 45 511 76 382 399 473 413 707 243 693 897 321 99 224 581 916 98 975 87 288 456 280 416 613 208 197 485 370 158 873 200 203 384 628 937 135 102 350 843 697 395 92 19 847 669 600 763 767 927 202 55 736 482 823 701 690 20 187 60 530 60 613 85 797 241 23 932 343 725 127 41 121 220 412 320 241 364 83 8 291 286 63 379 120 238 81 811 610 268 223 141 680 836 226 477 430 249 762 773 975 889 166 448 461 930 768 702 646 203 710 938 841 125 669 962 715 750 773 326 370  
sun{derotser_enilemit_1001131519}
```