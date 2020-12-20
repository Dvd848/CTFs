# pwning math
Category: Beginners

## Description
> Sometimes you would want to write a script which connects to a server and interacts with it instead of interacting by youself. There are some ways to do so, we recommend using pwntools. pwntools is a framework designed specificially for exploit development.
> 
> It contains two libraries, one called pwn designed for CTFs and as such contains tools optimized for that and pwnlib which is a cleaner version of pwn designed as a normal library.
> 
> pwntools is really great for developing payload and exploiting binaries but I personally use its pwnlib.tubes module more which allows for generic high-level interaction with a binary or a server in a simpler way then most libraries allow.
> 
> For the documentation on the module click here and for a quick tutorial click here.
> 
> Now, after you know a bit about pwntools, go ahead and 'pwn' this service:
> 
> nc ctf.cs.technion.ac.il 4011
> 


## Solution

Let's connect to the attached service:

```console
root@kali:/media/sf_CTFs/technion/pwning_math# nc ctf.cs.technion.ac.il 4011
What is 24 + 298?
>
Too Slow!
```

The service asks us to solve a simple math question and almost immediately closes. We must answer fast - using a script.

The following script uses `pwntools` to read the math equation, evaluate it and send the response:

```python
from pwn import *
import re

r = remote("ctf.cs.technion.ac.il", 4011)

math_exp_regex =  re.compile(r"^What is (\d+ [+\-*/] \d+)*\?$");

while(True):
    output = r.recvline().decode("ascii")
    if match := math_exp_regex.match(output):
        question = match.group(1)
        answer = str(int(eval(question)))
        log.info(f"Question: {question}, answer: {answer}")
        r.sendlineafter("> ", answer)
    elif "cstechnion" in output:
        print(output)
        break
    else:
        raise Exception(f"Unrecognized output: {output}")
```

Output:
```console
root@kali:/media/sf_CTFs/technion/pwning_math# python3 solve.py
[+] Opening connection to ctf.cs.technion.ac.il on port 4011: Done
[*] Question: 243 * 450, answer: 109350
[*] Question: 438 + 363, answer: 801
[*] Question: 158 * 443, answer: 69994
[*] Question: 263 * 93, answer: 24459
[*] Question: 500 - 137, answer: 363
[*] Question: 672 - 277, answer: 395
[*] Question: 53946 / 111, answer: 486
[*] Question: 4 * 336, answer: 1344
[*] Question: 347 - 288, answer: 59
[*] Question: 160 * 218, answer: 34880
[*] Question: 17542 * 34055, answer: 597392810
[*] Question: 64498 - 35197, answer: 29301
[*] Question: 17288 - 6468, answer: 10820
[*] Question: 19044 + 28260, answer: 47304
[*] Question: 43033 + 18296, answer: 61329
[*] Question: 43582 * 33964, answer: 1480219048
[*] Question: 28572 - 4602, answer: 23970
[*] Question: 1936174002 / 39046, answer: 49587
[*] Question: 47068 + 18408, answer: 65476
[*] Question: 9709245 / 21339, answer: 455
[*] Question: 34097 + 39227, answer: 73324
[*] Question: 189712896 / 12777, answer: 14848
[*] Question: 81625 - 49019, answer: 32606
[*] Question: 13856 * 40616, answer: 562775296
[*] Question: 11412 + 45494, answer: 56906
[*] Question: 2580365 - 1261244, answer: 1319121
[*] Question: 170992 + 145124, answer: 316116
[*] Question: 1644495 * 4971760, answer: 8176034461200
[*] Question: 4722778 + 1019102, answer: 5741880
[*] Question: 5803933931328 / 2857296, answer: 2031268
[*] Question: 7006263 - 2472750, answer: 4533513
[*] Question: 4767288 + 3850962, answer: 8618250
[*] Question: 11808135515843 / 3006629, answer: 3927367
[*] Question: 3139125 * 2702480, answer: 8483422530000
[*] Question: 2946449 + 1789288, answer: 4735737
[*] Question: 1807968261960 / 1005060, answer: 1798866
[*] Question: 3372394 + 2809877, answer: 6182271
[*] Question: 7710484 - 4544102, answer: 3166382
[*] Question: 2029523 * 1362585, answer: 2765397596955
[*] Question: 2775925 * 629126, answer: 1746406591550
[*] Question: 5945778 - 1148704, answer: 4797074
[*] Question: 3374613 * 1643383, answer: 5545781635779
[*] Question: 3274444 - 191241, answer: 3083203
[*] Question: 715886923140 / 667620, answer: 1072297
[*] Question: 1874540763826 / 978454, answer: 1915819
[*] Question: 3549899 * 2378035, answer: 8441784068465
[*] Question: 4353738 * 2902369, answer: 12636154205322
[*] Question: 755545 + 4651114, answer: 5406659
[*] Question: 357061 * 2134873, answer: 762279888253
[*] Question: 4796466 - 1628000, answer: 3168466
[*] Question: 124372856269491762 / 322940759, answer: 385125918
[*] Question: 65719380399856602 / 366838734, answer: 179150603
[*] Question: 400511283 + 11148827, answer: 411660110
[*] Question: 399522283 + 278802527, answer: 678324810
[*] Question: 96467649996548328 / 212549368, answer: 453859971
[*] Question: 50124535 * 348943719, answer: 17490641656045665
[*] Question: 624348970 - 323604955, answer: 300744015
[*] Question: 385751938 + 111813584, answer: 497565522
[*] Question: 8517670208831196 / 261639402, answer: 32554998
[*] Question: 161748016 + 306277057, answer: 468025073
[*] Question: 678253810 - 464239808, answer: 214014002
[*] Question: 146416007 - 11670689, answer: 134745318
[*] Question: 471940922 - 210138799, answer: 261802123
[*] Question: 125928545 * 359749688, answer: 45302754774043960
[*] Question: 44300526889646430 / 495042210, answer: 89488383
[*] Question: 9256211 * 101765413, answer: 941962135230143
[*] Question: 507068906 - 191194080, answer: 315874826
[*] Question: 598100424 - 464292348, answer: 133808076
[*] Question: 364192967 + 311081241, answer: 675274208
[*] Question: 192267035 * 286487081, answer: 55082021629674835
[*] Question: 446549737 * 140465358, answer: 62724768672510846
[*] Question: 24997551558705740 / 205113580, answer: 121871753
[*] Question: 363372484 * 241721531, answer: 87834953155753004
[*] Question: 442966159 - 108097397, answer: 334868762
[*] Question: 48885038 + 184534309, answer: 233419347
[*] Question: 290070884 * 305768333, answer: 88694490652516372
[*] Question: 228827833 + 271995057, answer: 500822890
[*] Question: 46820236794134720 / 185381920, answer: 252560966
[*] Question: 36009586677218544 / 113361459, answer: 317652816
[*] Question: 647404722 - 429828252, answer: 217576470
[*] Question: 381579921 - 94107607, answer: 287472314
[*] Question: 168247692 + 422775389, answer: 591023081
[*] Question: 185171143 * 180069090, answer: 33343599214269870
[*] Question: 216103924531702530 / 484775259, answer: 445781670
[*] Question: 143857928284354882 / 435284978, answer: 330491369
[*] Question: 45732222771644435 / 402652315, answer: 113577449
[*] Question: 539448193 - 159572944, answer: 379875249
[*] Question: 261812895 - 127336718, answer: 134476177
[*] Question: 354471468 * 452070282, answer: 160246016499713976
[*] Question: 1895205181438920 / 182469390, answer: 10386428
[*] Question: 471023394 * 24075178, answer: 11339972052714132
[*] Question: 51199528 * 315265242, answer: 16141431585205776
[*] Question: 87953045536470645 / 497158989, answer: 176911305
[*] Question: 282049656 * 486089385, answer: 137101343824501560
[*] Question: 83908207 * 261893065, answer: 21974977509884455
[*] Question: 498827479 - 278710556, answer: 220116923
[*] Question: 82905177 * 383244380, answer: 31772943158155260
[*] Question: 907395970 - 432801555, answer: 474594415
[*] Question: 179437991 * 59509724, answer: 10678305319524484
[*] Question: 314744935 + 158660409, answer: 473405344
Nice! here's your flag: cstechnion{E4t_Sl33p_Pwn_m47H_R3pe47}
```

The flag: `cstechnion{E4t_Sl33p_Pwn_m47H_R3pe47}`