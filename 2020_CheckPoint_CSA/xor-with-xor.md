# xor-with-xor
Category: Misc., 30 Points

## Description

> We found this file on an evil hacker's computer. It's probably encrypted, but we don't know how to decrypt it.
> 
> We need your help! 
> 
> Thanks in advance :)
> 
> Good Luck!

A binary file was attached.

## Solution

Let's take a look at the attached binary:

```console
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor# file xor-with-xor.bin
xor-with-xor.bin: data
```

It is not recognized, let's peek inside:
```console
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor# xxd -g 1 xor-with-xor.bin | head
00000000: 28 24 71 7c 7b 72 78 6f 7a 78 4b dc f6 3f 4d 1c  ($q|{rxozxK..?M.
00000010: c9 e5 24 6d 72 78 38 70 78 6f 75 78 6f 72 4e 5c  ..$mrx8pxouxorN\
00000020: 46 56 0b 13 0c 6e 25 7a c7 8f b7 c3 0d c5 97 88  FV...n%z........
00000030: 07 94 68 05 2a 01 87 db 8d ab 91 0f bb 00 b0 cd  ..h.*...........
00000040: 12 a5 8e c2 61 5e 99 3c 94 d2 58 14 70 94 72 d2  ....a^.<..X.p.r.
00000050: e1 02 48 0f 6a a9 f4 71 ee 1a 43 18 aa 5e b3 64  ..H.j..q..C..^.d
00000060: de 84 e9 d6 80 8a 48 4e 0a 5f 42 05 40 d6 03 0a  ......HN._B.@...
00000070: 8c b5 c8 1c 01 9f 89 b5 39 93 b1 af 0f 74 55 62  ........9....tUb
00000080: 21 65 28 8d f7 b7 22 f7 e5 4f 28 93 39 7f 19 6e  !e(..."..O(.9..n
00000090: a0 2e 65 dd c0 98 bd d7 c7 0a 04 97 ec 85 5b ad  ..e...........[.
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor# xxd -g 1 xor-with-xor.bin | tail
000a94d0: 78 6f 72 78 6f 72 78 6f 72 78 6f c6 f9 1e cc 71  xorxorxorxo....q
000a94e0: 6f 41 41 5e 5c 1c 0e 06 28 24 73 7a 7b 71 6c 6f  oAA^\...($sz{qlo
000a94f0: 72 78 67 72 5c c1 fc 28 4d 3a 94 4e 2e 7a 6f 72  rxgr\..(M:.N.zor
000a9500: 2f 6d 72 78 68 72 78 6f 72 78 6f 72 78 6f 72 78  /mrxhrxorxorxorx
000a9510: db f3 8a af 7b 78 5b 43 4c 41 16 19 1b 22 33 6e  ....{x[CLA..."3n
000a9520: 70 6c 6c 66 78 6f 72 70 6f 56 d6 e1 22 64 66 8c  pllfxorpoV.."df.
000a9530: 05 33 70 78 6f 25 7a 6f 72 7f 6f 72 78 6f 72 78  .3pxo%zor.orxorx
000a9540: 6f 72 78 6f 72 cc ee 01 bb 66 72 4e 59 43 56 0b  orxor....frNYCV.
000a9550: 13 0c 3f 39 7d 69 72 78 6f 72 90 6c 9a 7b 31 bd  ..?9}irxor.l.{1.
000a9560: 78 6f 86 bd 66 72 78 6f                          xo..frxo
```

One thing that stands out is how common the string `xor` is within the binary. A possible explanation is that the binary was XOR-ed with the key `xor`, causing sequences of NULL bytes to receive the value `xor`. This happens since XOR-ing any value with 0x0 will produce the original value. The challenge name is a hint in this direction.

Let's try to decrypt the binary by XOR-ing it with the key `xor` again.

```python
>>> from pwn import *
>>> with open("xor-with-xor.bin", "rb") as f, open("out.bin", "wb") as o:
...     o.write(xor(f.read(), "xor"))
...
693608
```

Now let's see what we got:
```console
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor# file out.bin
out.bin: Zip archive data, at least v2.0 to extract
```

It's a zip file!

```console
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor# mv out.bin out.zip
```

After extracting it, we get the following files:
```console
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor/out# ls
 0.dat       '2$7$7.dat'  '3$5$3.dat'  '4$9$7.dat'   502.dat      604.dat      704.dat      805.dat      901.dat
'1$1$7.dat'  '2$9$3.dat'  '3$7$4.dat'   400.dat      503.dat      605.dat      705.dat      806.dat      902.dat
'1$4$0.dat'   200.dat     '3$7$9.dat'   401.dat      504.dat      606.dat      706.dat      807.dat      903.dat
'1$8$3.dat'   201.dat      300.dat      402.dat      505.dat      607.dat      707.dat      808.dat      904.dat
 100.dat      202.dat      301.dat      403.dat      5_0_6.dat    608.dat      709.dat      80.dat       905.dat
 101.dat      203.dat      302.dat      404.dat      507.dat      609.dat      70.dat       810.dat      906.dat
 102.dat      204.dat      303.dat      405.dat      508.dat      60.dat      '7 1 0.dat'   811.dat      907.dat
 103.dat      205.dat      304.dat     '4[0[6.dat'   509.dat      610.dat      711.dat      812.dat      908.dat
 104.dat      206.dat      305.dat      407.dat      50.dat       611.dat      712.dat      813.dat      909.dat
 105.dat      207.dat      306.dat      408.dat      510.dat      612.dat      713.dat      814.dat      90.dat
 106.dat      208.dat      307.dat      409.dat      511.dat      613.dat      714.dat      815.dat      910.dat
 107.dat      209.dat      308.dat      40.dat       512.dat      614.dat      715.dat      816.dat      911.dat
 108.dat      20.dat       309.dat     '4[1[0.dat'   513.dat      615.dat      716.dat      817.dat      912.dat
 109.dat      210.dat      30.dat       411.dat     '5 1 4.dat'   616.dat      717.dat      818.dat      913.dat
 10.dat       211.dat      310.dat      412.dat      515.dat      617.dat      718.dat      819.dat      914.dat
 111.dat      212.dat      311.dat      413.dat      516.dat      618.dat      719.dat      81.dat       915.dat
 112.dat      213.dat      313.dat      414.dat      517.dat      619.dat      71.dat       820.dat      916.dat
 113.dat      214.dat      314.dat      415.dat      519.dat      61.dat       722.dat      822.dat      917.dat
 114.dat      215.dat      315.dat      416.dat      51.dat       620.dat      723.dat      823.dat      918.dat
 115.dat      216.dat      316.dat     '4[1[7.dat'   520.dat      621.dat      725.dat      824.dat      919.dat
 116.dat      2_1_7.dat    317.dat      418.dat      521.dat      623.dat      726.dat      826.dat      91.dat
 118.dat      218.dat      318.dat      419.dat      523.dat      624.dat      727.dat      827.dat     '9[2[0.dat'
 119.dat      219.dat      319.dat      41.dat       524.dat      625.dat      728.dat      828.dat      921.dat
 11.dat       21.dat       31.dat       420.dat      525.dat      627.dat      729.dat      829.dat      922.dat
 120.dat      220.dat      320.dat      421.dat      526.dat      628.dat      72.dat       82.dat       923.dat
 121.dat      221.dat      321.dat      422.dat      527.dat      629.dat      730.dat      830.dat      925.dat
 122.dat      222.dat      322.dat      423.dat      528.dat      62.dat       731.dat      831.dat      926.dat
 123.dat      223.dat      324.dat      425.dat      529.dat      630.dat      732.dat      832.dat      927.dat
 124.dat      224.dat      325.dat      426.dat      52.dat       631.dat      733.dat      833.dat      928.dat
 125.dat      225.dat      326.dat      427.dat      530.dat      632.dat      734.dat      834.dat      92.dat
 126.dat      226.dat      327.dat      429.dat      531.dat     '6[3[3.dat'   735.dat      835.dat      930.dat
 127.dat      227.dat      328.dat      42.dat       532.dat      634.dat      737.dat      836.dat      931.dat
 128.dat      228.dat      32.dat       430.dat      533.dat      635.dat      738.dat      837.dat      932.dat
 129.dat      229.dat      330.dat      431.dat      534.dat      636.dat      7_3_9.dat    838.dat      933.dat
 12.dat       22.dat      '3 3 1.dat'   432.dat      535.dat      637.dat      73.dat       83.dat       934.dat
 130.dat      230.dat      332.dat      433.dat      536.dat      638.dat      740.dat      840.dat      935.dat
 131.dat      231.dat      333.dat      434.dat      538.dat      639.dat      742.dat      841.dat      936.dat
 132.dat      232.dat      334.dat      435.dat      539.dat      63.dat       743.dat      842.dat      937.dat
 133.dat      233.dat      335.dat      436.dat      53.dat       640.dat      744.dat      843.dat      938.dat
 134.dat      234.dat      337.dat     '4[3[7.dat'  '5[4[1.dat'   641.dat      745.dat      844.dat      939.dat
 135.dat      235.dat      338.dat      438.dat      542.dat      642.dat      746.dat     '8[4[5.dat'   93.dat
 136.dat      236.dat      339.dat      439.dat      543.dat      643.dat      747.dat      846.dat     '9[4[0.dat'
 137.dat      237.dat      33.dat       43.dat       544.dat      644.dat      748.dat      847.dat      941.dat
 138.dat      238.dat      340.dat      440.dat      545.dat      645.dat      749.dat      848.dat      942.dat
'1[3[9.dat'   239.dat      341.dat      442.dat      546.dat      646.dat      74.dat       849.dat      943.dat
 13.dat       23.dat       342.dat      443.dat      547.dat      6_4_7.dat    750.dat      84.dat       944.dat
 141.dat      240.dat      343.dat      444.dat      548.dat      648.dat      751.dat      850.dat     '9 4 5.dat'
 142.dat      241.dat      344.dat      445.dat      549.dat      64.dat       752.dat      851.dat      947.dat
 143.dat      242.dat      345.dat      446.dat      54.dat       650.dat      753.dat      852.dat      948.dat
 144.dat      243.dat      346.dat      447.dat      550.dat     '6 5 1.dat'  '7[5[4.dat'   853.dat      949.dat
 145.dat      244.dat      347.dat      449.dat      551.dat      652.dat      755.dat      854.dat      94.dat
 146.dat      245.dat      349.dat      44.dat       552.dat      653.dat      756.dat      855.dat      950.dat
 147.dat      246.dat      34.dat       450.dat      5_5_3.dat    654.dat     '7 5 7.dat'   856.dat      951.dat
 148.dat      247.dat      350.dat      451.dat      554.dat      655.dat      758.dat      857.dat      952.dat
 149.dat      248.dat      351.dat      452.dat      555.dat     '6 5 6.dat'   759.dat      858.dat      953.dat
 14.dat       24.dat       352.dat      453.dat      556.dat      657.dat      75.dat       859.dat      954.dat
 150.dat      250.dat      354.dat      454.dat      557.dat      658.dat      760.dat      85.dat       955.dat
 151.dat      2_5_1.dat    355.dat      455.dat      558.dat      659.dat      761.dat      860.dat      956.dat
 152.dat      252.dat      356.dat      456.dat      559.dat      65.dat       762.dat      861.dat      957.dat
 153.dat      253.dat      357.dat      457.dat      55.dat       660.dat     '7[6[3.dat'   862.dat      958.dat
 154.dat      254.dat      358.dat      458.dat      560.dat      661.dat      764.dat      863.dat      959.dat
 155.dat      255.dat      359.dat      459.dat      561.dat      662.dat      765.dat      864.dat      95.dat
 156.dat      256.dat      35.dat       45.dat       562.dat      664.dat      766.dat      865.dat      960.dat
 157.dat      257.dat      360.dat      460.dat      563.dat      665.dat      767.dat      866.dat      961.dat
 158.dat      258.dat      362.dat      461.dat      564.dat      666.dat      768.dat      867.dat      962.dat
 159.dat      259.dat      363.dat      462.dat      565.dat      667.dat      769.dat      868.dat     '9 6 3.dat'
 15.dat       25.dat       364.dat      463.dat      566.dat      668.dat      76.dat       869.dat     '9[6[4.dat'
 160.dat      260.dat      365.dat      464.dat      567.dat      669.dat      770.dat      86.dat       965.dat
 161.dat      261.dat      366.dat      465.dat      568.dat      66.dat       771.dat      870.dat      966.dat
 162.dat      262.dat      367.dat      466.dat      569.dat      671.dat      772.dat      871.dat      967.dat
 163.dat      263.dat      368.dat      467.dat      56.dat       672.dat      774.dat      872.dat      968.dat
 164.dat      264.dat      369.dat      468.dat      570.dat      673.dat      775.dat      873.dat      969.dat
 165.dat      265.dat      36.dat       469.dat      571.dat      674.dat      776.dat      874.dat      96.dat
 166.dat      266.dat      370.dat      46.dat       572.dat      675.dat     '7 7 7.dat'   875.dat      970.dat
 167.dat      267.dat      371.dat      470.dat      573.dat      676.dat      778.dat      876.dat      971.dat
 168.dat      268.dat      372.dat      471.dat      574.dat      677.dat      779.dat      877.dat      972.dat
 169.dat      269.dat      373.dat      472.dat      575.dat      678.dat      77.dat       878.dat      974.dat
 16.dat       26.dat       375.dat      473.dat      577.dat      679.dat      780.dat      879.dat      975.dat
 170.dat      270.dat      376.dat      474.dat      5_7_8.dat    67.dat       781.dat      87.dat       976.dat
 171.dat      271.dat      377.dat      475.dat      579.dat      680.dat      782.dat      880.dat      977.dat
 172.dat      272.dat      378.dat      477.dat      57.dat       681.dat      783.dat     '8[8[1.dat'   978.dat
 174.dat      273.dat      37.dat       478.dat      580.dat      682.dat      784.dat      883.dat      979.dat
 175.dat      274.dat      380.dat      479.dat      581.dat      683.dat      785.dat      884.dat      97.dat
 176.dat      275.dat      381.dat      47.dat       582.dat      684.dat      786.dat      885.dat      981.dat
 177.dat      276.dat      382.dat      480.dat      583.dat      685.dat      787.dat      886.dat      982.dat
'1[7[8.dat'   278.dat      383.dat      481.dat      584.dat      686.dat      788.dat      887.dat      983.dat
 179.dat      279.dat      384.dat      482.dat     '5 8 5.dat'   687.dat      789.dat      888.dat      984.dat
 17.dat       27.dat       385.dat      483.dat      5_8_6.dat    688.dat      78.dat       889.dat      985.dat
 180.dat      280.dat      386.dat      484.dat      587.dat      689.dat      790.dat      88.dat       986.dat
 181.dat      281.dat      387.dat     '4[8[5.dat'   588.dat      68.dat       791.dat      890.dat      987.dat
 184.dat      282.dat      388.dat      486.dat      589.dat      690.dat     '7[9[2.dat'   891.dat      988.dat
 185.dat      283.dat      389.dat      487.dat      58.dat       691.dat      793.dat      892.dat      98.dat
 186.dat      284.dat      38.dat      '4[8[9.dat'   590.dat      692.dat      794.dat      893.dat      990.dat
 187.dat      285.dat      390.dat      48.dat      '5 9 1.dat'   693.dat      795.dat      894.dat      991.dat
 188.dat      286.dat      391.dat      490.dat      592.dat      694.dat      796.dat      895.dat      992.dat
 189.dat      287.dat      392.dat      491.dat      593.dat      695.dat      797.dat      897.dat      993.dat
 18.dat       288.dat      393.dat      492.dat      594.dat      696.dat     '7 9 8.dat'   898.dat      994.dat
 190.dat      289.dat      394.dat      493.dat      595.dat      697.dat      799.dat      899.dat      995.dat
 191.dat      28.dat       395.dat      494.dat      596.dat      698.dat      79.dat       89.dat       996.dat
 192.dat      290.dat      396.dat      495.dat      597.dat      699.dat      7.dat        8.dat        998.dat
 193.dat      291.dat      397.dat      496.dat      5_9_8.dat    69.dat       7T2T1.dat    8T3T9.dat    999.dat
 194.dat      292.dat      398.dat      498.dat      599.dat      6.dat        7x0x8.dat    8T8T2.dat    99.dat
 195.dat      294.dat      399.dat      499.dat      59.dat       6T2T2.dat    7x2x0.dat    8T9T6.dat    9.dat
 196.dat      295.dat      39.dat       49.dat       5.dat        6T2T6.dat    7x2x4.dat    8x0x9.dat    9T2T9.dat
 197.dat      296.dat      3.dat        4.dat        5T1T8.dat    6T4T9.dat    7x3x6.dat    8x2x1.dat
 198.dat      297.dat      3T2T3.dat    4T8T8.dat    5T2T2.dat    6x6x3.dat    7x7x3.dat   '9$2$4.dat'
 199.dat      298.dat      3T2T9.dat    4x4x1.dat    5T3T7.dat    6x7x0.dat   '8$2$5.dat'  '9$4$6.dat'
 19.dat      '2 9 9.dat'   3T4T8.dat    4x4x8.dat    5x4x0.dat   '7$4$1.dat'   800.dat     '9$7$3.dat'
 1.dat        29.dat       3x1x2.dat    4x7x6.dat    600.dat     '7[0[0.dat'   801.dat     '9$8$0.dat'
 1x1x0.dat    2.dat        3x6x1.dat   '5$7$6.dat'   601.dat      701.dat      802.dat     '9$8$9.dat'
 1x7x3.dat    2x4x9.dat   '4$2$4.dat'   500.dat      6_0_2.dat    702.dat      803.dat     '9$9$7.dat'
 1x8x2.dat   '3$3$6.dat'  '4$2$8.dat'   501.dat      6_0_3.dat    703.dat      804.dat      900.dat
 ```

 We seem to have 1000 files:
 ```console
 root@kali:/media/sf_CTFs/checkpoint/xor-with-xor/out# ls | wc -l
1000
```

If we check the file types, we get a reasonable file type for `0.dat`:
```console
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor/out# file 0.dat
0.dat: gzip compressed data, was "zipped_audio.zip", last modified: Tue Apr 14 18:49:08 2020, max compression, original size -1791305681
```

Other files don't produce a reasonable type, and even `0.dat` has an unreasonable size: -1791305681.

Let's take a look at how GZIP files are built (source: [Kaitai](https://formats.kaitai.io/gzip/index.html)):
![](https://formats.kaitai.io/gzip/gzip.svg)

We can see that the uncompressed length of the file is located at the last DWORD of the compressed file.

Here's the value for `0.dat`:
```console
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor/out# od -j $(($(stat --printf="%s" 0.dat)-4)) -t d4 0.dat
0001123 -1791305681
0001127
```

We can see that it's `-1791305681`, like `file` showed us before.

On the other hand, here it is for `999.dat`, which is the last file:
```console
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor/out# od -j $(($(stat --printf="%s" 999.dat)-4)) -t d4 999.dat
0000043     1419260
0000047
```

This looks much more reasonable, given the fact that the sum of file sizes is about half of the number above:
```console
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor/out# du --apparent-size -b
762280  .
```

So, the plan is to iterate the files by order (while cleaning up the names, e.g. `7$4$1.dat` becomes `741.dat` etc.) and concatenate them to one big file which hopefully will be a legal GZIP file.

We can use the following script:
```python
import os, re

directory = "out"
file_list = os.listdir(directory)
file_contents = [None] * len(file_list)

for filename in file_list:
    with open (os.path.join(directory, filename), "rb") as f:
        clean_name = re.sub(r"\D", "", filename)
        file_contents[int(clean_name)] = f.read()

full_file = bytearray()
for contents in file_contents:
    full_file += bytearray(contents)

with open("full.gz", "wb") as o:
    o.write(full_file)
```

We run it and get:
```console
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor# python3 solve.py
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor# file full.gz
full.gz: gzip compressed data, was "zipped_audio.zip", last modified: Tue Apr 14 18:49:08 2020, max compression, original size 1419260
```

Let's try to extract it:
```console
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor# python3 solve.py
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor# gunzip -v full.gz
full.gz:         57.8% -- replaced with full
root@kali:/media/sf_CTFs/checkpoint/xor-with-xor# file full
full: RIFF (little-endian) data, WAVE audio, Microsoft PCM, 16 bit, stereo 48000 Hz
```

We got an audio file!

Listening to it reveals the flag: `csa{you are a very good listener}`