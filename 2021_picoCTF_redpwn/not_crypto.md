# not crypto
Reverse Engineering, 150 points

## Description

> there's crypto in here but the challenge is not crypto... 🤔

A binary file was attached.

## Solution

Let's run the attached file:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/not_crypto]
└─$ ./not-crypto
I heard you wanted to bargain for a flag... whatcha got?
test
test2
test3
aaaaaaaaaaaaaaaaaaaaaaaaaaaa
bbbbbbbbbbbbbbbbbbbbbbbbbbbbb
Nope, come back later
```

The program requests user input. After entering multiple inputs, we get an error message that our input is incorrect.

Let's check the decompiled output in Ghidra. The main function looks like a mess:

```c
int FUN_00101070(void)

{
  byte *pbVar1;
  undefined auVar2 [16];
  undefined auVar3 [16];
  undefined auVar4 [16];
  undefined auVar5 [16];
  undefined auVar6 [16];
  undefined auVar7 [16];
  undefined auVar8 [16];
  byte bVar9;
  byte bVar10;
  byte bVar11;
  byte bVar12;
  byte bVar13;
  byte bVar14;
  byte bVar15;
  byte bVar16;
  byte bVar17;
  byte bVar18;
  uint uVar19;
  uint6 uVar20;
  unkuint10 Var21;
  undefined auVar22 [12];
  undefined auVar23 [14];
  int iVar24;
  undefined4 uVar25;
  byte *pbVar26;
  byte bVar27;
  byte bVar28;
  byte bVar29;
  long lVar30;
  byte bVar31;
  byte bVar32;
  byte bVar33;
  ulong uVar34;
  byte bVar35;
  uint uVar36;
  ulong uVar37;
  byte bVar38;
  byte bVar39;
  byte bVar40;
  byte bVar41;
  byte bVar42;
  byte bVar43;
  byte bVar44;
  byte bVar45;
  byte *pbVar46;
  long in_FS_OFFSET;
  byte local_1fe;
  byte local_1fd;
  uint local_1fc;
  uint local_1f8;
  byte local_1f4;
  byte local_1f3;
  byte local_1f2;
  byte local_1f1;
  byte local_1f0;
  byte local_1ef;
  byte local_1ee;
  byte local_1ed;
  byte local_1ec;
  byte *local_1e8;
  undefined user_input [64];
  undefined local_158 [16];
  byte local_148 [144];
  byte local_b8;
  byte local_b7;
  byte local_b6;
  byte local_b5;
  byte local_b4;
  byte local_b3;
  byte local_b2;
  byte local_b1;
  byte local_b0;
  byte local_af;
  byte local_ae;
  byte local_ad;
  byte local_ac;
  byte local_ab;
  byte local_aa;
  byte local_a9;
  undefined local_a8 [3];
  undefined auStack165 [2];
  undefined auStack163 [2];
  undefined uStack161;
  undefined8 uStack160;
  undefined local_98 [16];
  undefined local_88 [16];
  undefined local_78 [16];
  undefined local_68 [16];
  undefined local_58 [16];
  byte local_48 [8];
  long local_40;
  
  local_40 = *(long *)(in_FS_OFFSET + 0x28);
  puts("I heard you wanted to bargain for a flag... whatcha got?");
  auVar7 = vmovdqa_avx(_DAT_001021a0);
  bVar40 = 0x98;
  bVar32 = 0x32;
  bVar27 = 0x6c;
  bVar28 = 0x1c;
  local_158 = vmovdqa_avx(auVar7);
  uVar37 = 4;
  pbVar26 = local_158;
  do {
    if ((uVar37 & 3) == 0) {
      uVar34 = (ulong)bVar32;
      bVar32 = (&DAT_001020a0)[bVar27];
      bVar27 = (&DAT_001020a0)[bVar28];
      bVar28 = (&DAT_001020a0)[bVar40];
      bVar40 = (&DAT_001020a0)[uVar34] ^ (&DAT_00102080)[uVar37 >> 2];
    }
    bVar40 = bVar40 ^ *pbVar26;
    uVar36 = (int)uVar37 + 1;
    uVar37 = (ulong)uVar36;
    bVar32 = bVar32 ^ pbVar26[1];
    bVar27 = bVar27 ^ pbVar26[2];
    bVar28 = bVar28 ^ pbVar26[3];
    pbVar26[0x10] = bVar40;
    pbVar26[0x11] = bVar32;
    pbVar26[0x12] = bVar27;
    pbVar26[0x13] = bVar28;
    pbVar26 = pbVar26 + 4;
  } while (uVar36 != 0x2c);
  auVar7 = vmovdqa_avx(_DAT_001021b0);
  _local_a8 = vmovdqa_avx(auVar7);
  fread(user_input,1,0x40,stdin);
  auVar7 = vmovdqa_avx(_DAT_001021c0);
  local_88 = vmovdqa_avx(auVar7);
  auVar7 = vmovdqa_avx(_DAT_001021d0);
  local_78 = vmovdqa_avx(auVar7);
  auVar7 = vmovdqa_avx(_DAT_001021e0);
  local_68 = vmovdqa_avx(auVar7);
  auVar7 = vmovdqa_avx(_DAT_001021f0);
  local_58 = vmovdqa_avx(auVar7);
  iVar24 = 0x10;
  local_1e8 = local_88;
  do {
    if (iVar24 == 0x10) {
      auVar7 = vmovdqa_avx(_local_a8);
      local_1f8 = local_1f8 & 0xffffff00 | (uint)(byte)(&DAT_001020a0)[local_158[0] ^ local_a8[0]];
      uVar25 = vpextrb_avx(auVar7,4);
      local_1fc = local_1fc & 0xffffff00 |
                  (uint)(byte)(&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[4])];
      local_1ee = (&DAT_001020a0)[local_158[8] ^ (byte)uStack160];
      uVar25 = vpextrb_avx(auVar7,0xc);
      local_1ef = (&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[12])];
      uVar25 = vpextrb_avx(auVar7,1);
      local_1f4 = (&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[1])];
      uVar25 = vpextrb_avx(auVar7,5);
      local_1fd = (&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[5])];
      uVar25 = vpextrb_avx(auVar7,9);
      local_1fe = (&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[9])];
      uVar25 = vpextrb_avx(auVar7,0xd);
      local_1f0 = (&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[13])];
      uVar25 = vpextrb_avx(auVar7,2);
      bVar32 = (&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[2])];
      uVar25 = vpextrb_avx(auVar7,6);
      local_1ec = (&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[6])];
      uVar25 = vpextrb_avx(auVar7,10);
      local_1f1 = (&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[10])];
      uVar25 = vpextrb_avx(auVar7,0xe);
      local_1f2 = (&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[14])];
      uVar25 = vpextrb_avx(auVar7,3);
      local_1ed = (&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[3])];
      uVar25 = vpextrb_avx(auVar7,7);
      bVar28 = (&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[7])];
      uVar25 = vpextrb_avx(auVar7,0xb);
      bVar27 = (&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[11])];
      uVar25 = vpextrb_avx(auVar7,0xf);
      local_1f3 = (&DAT_001020a0)[(byte)((byte)uVar25 ^ local_158[15])];
      pbVar26 = local_148;
      do {
        bVar41 = local_1fd ^ (byte)local_1f8;
        bVar31 = local_1f3 ^ local_1f1;
        bVar43 = bVar41 ^ bVar31;
        bVar35 = local_1fe ^ (byte)local_1fc;
        bVar38 = local_1ed ^ local_1f2;
        bVar39 = bVar35 ^ bVar38;
        bVar44 = local_1ed ^ (byte)local_1fc;
        bVar12 = bVar28 ^ bVar32;
        bVar10 = local_1f0 ^ local_1ee;
        bVar13 = local_1ee ^ bVar28;
        bVar42 = bVar10 ^ bVar12;
        bVar14 = local_1ec ^ bVar27;
        bVar9 = local_1ec ^ local_1f4;
        bVar11 = local_1f4 ^ local_1ef;
        bVar15 = local_1ef ^ bVar27;
        bVar16 = bVar11 ^ bVar14;
        bVar45 = pbVar26[7] ^ bVar39 ^ local_1ed;
        bVar17 = bVar32 ^ bVar42 ^ pbVar26[10];
        bVar33 = pbVar26[0xd] ^ bVar16 ^ local_1f4;
        bVar29 = pbVar26[0xe] ^ bVar16 ^ local_1ec;
        bVar18 = bVar27 ^ pbVar26[0xf] ^ bVar16;
        bVar40 = *pbVar26;
        uVar36 = local_1f8 & 0xffffff00;
        local_1f8 = uVar36 | (byte)(&DAT_001020a0)
                                   [(byte)((byte)local_1f8 ^ bVar40 ^ bVar43 ^
                                          ((char)bVar41 >> 7) * -0x1b ^ bVar41 * '\x02')];
        pbVar1 = pbVar26 + 4;
        uVar19 = local_1fc & 0xffffff00;
        local_1fc = uVar19 | (byte)(&DAT_001020a0)
                                   [(byte)(*pbVar1 ^ bVar39 ^ (byte)local_1fc ^
                                          ((char)bVar35 >> 7) * -0x1b ^ bVar35 * '\x02')];
        local_1ee = (&DAT_001020a0)
                    [(byte)(pbVar26[8] ^ bVar42 ^ local_1ee ^
                           ((char)bVar10 >> 7) * -0x1b ^ bVar10 * '\x02')];
        local_1ef = (&DAT_001020a0)
                    [(byte)(bVar16 ^ pbVar26[0xc] ^ local_1ef ^
                           bVar11 * '\x02' ^ ((char)bVar11 >> 7) * -0x1b)];
        local_1f4 = (&DAT_001020a0)
                    [(byte)(pbVar26[1] ^ bVar43 ^ local_1fd ^
                           ((char)(local_1f1 ^ local_1fd) >> 7) * -0x1b ^
                           (local_1f1 ^ local_1fd) * '\x02')];
        local_1fd = (&DAT_001020a0)
                    [(byte)(pbVar26[5] ^ bVar39 ^ local_1fe ^
                           (local_1f2 ^ local_1fe) * '\x02' ^
                           ((char)(local_1f2 ^ local_1fe) >> 7) * -0x1b)];
        local_1fe = (&DAT_001020a0)
                    [(byte)(local_1f0 ^ bVar42 ^ pbVar26[9] ^
                           (local_1f0 ^ bVar32) * '\x02' ^ ((char)(local_1f0 ^ bVar32) >> 7) * -0x1b
                           )];
        local_1f0 = (&DAT_001020a0)
                    [((uint)(bVar9 >> 7) * 0x1b ^ (uint)bVar9 + (uint)bVar9 ^ (uint)bVar33) & 0xff];
        pbVar46 = pbVar26 + 0x10;
        bVar32 = (&DAT_001020a0)
                 [(byte)(pbVar26[2] ^ bVar43 ^ local_1f1 ^
                        ((char)bVar31 >> 7) * -0x1b ^ bVar31 * '\x02')];
        local_1ec = (&DAT_001020a0)
                    [(byte)(local_1f2 ^ pbVar26[6] ^ bVar39 ^
                           bVar38 * '\x02' ^ ((char)bVar38 >> 7) * -0x1b)];
        local_1f1 = (&DAT_001020a0)
                    [((uint)bVar12 * 2 ^ (uint)(bVar12 >> 7) * 0x1b ^ (uint)bVar17) & 0xff];
        local_1f2 = (&DAT_001020a0)
                    [((uint)bVar14 * 2 ^ (uint)(bVar14 >> 7) * 0x1b ^ (uint)bVar29) & 0xff];
        bVar27 = (&DAT_001020a0)
                 [((uint)(bVar13 >> 7) * 0x1b ^ (uint)bVar13 * 2 ^
                  (uint)(byte)(bVar28 ^ bVar42 ^ pbVar26[0xb])) & 0xff];
        local_1ed = (&DAT_001020a0)
                    [(byte)(pbVar26[3] ^ bVar43 ^ local_1f3 ^
                           (local_1f3 ^ (byte)local_1f8) * '\x02' ^
                           ((char)(local_1f3 ^ (byte)local_1f8) >> 7) * -0x1b)];
        bVar28 = (&DAT_001020a0)[(byte)(bVar45 ^ ((char)bVar44 >> 7) * -0x1b ^ bVar44 * '\x02')];
        local_1f3 = (&DAT_001020a0)
                    [((uint)(bVar15 >> 7) * 0x1b ^ (uint)bVar15 * 2 ^ (uint)bVar18) & 0xff];
        pbVar26 = pbVar46;
      } while (&local_b8 != pbVar46);
      local_1f8 = uVar36 | (&DAT_001020a0)
                           [(byte)((byte)local_1f8 ^ bVar40 ^ bVar43 ^
                                  ((char)bVar41 >> 7) * -0x1b ^ bVar41 * '\x02')] ^ local_b8;
      auVar7 = vmovd_avx((uint)(bVar32 ^ local_ae));
      local_1fc = uVar19 | local_1f2 ^ local_b2;
      auVar2 = vmovd_avx((uint)(local_1ec ^ local_aa));
      auVar3 = vmovd_avx((uint)(local_1f1 ^ local_b6));
      auVar8 = vpinsrb_avx(auVar7,(uint)(local_ad ^ bVar28),1);
      auVar7 = vmovd_avx((uint)((&DAT_001020a0)
                                [(byte)(*pbVar1 ^ bVar39 ^ (byte)local_1fc ^
                                       ((char)bVar35 >> 7) * -0x1b ^ bVar35 * '\x02')] ^ local_b4));
      lVar30 = 0xf;
      auVar4 = vmovd_avx((uint)(local_1ee ^ local_b0));
      auVar5 = vmovd_avx(local_1f8);
      auVar6 = vmovd_avx(local_1fc);
      auVar3 = vpinsrb_avx(auVar3,(uint)(local_1f3 ^ local_b5),1);
      auVar5 = vpinsrb_avx(auVar5,(uint)(local_1fd ^ local_b7),1);
      auVar4 = vpinsrb_avx(auVar4,(uint)(local_1f0 ^ local_af),1);
      auVar5 = vpunpcklwd_avx(auVar5,auVar3);
      auVar7 = vpinsrb_avx(auVar7,(uint)(local_1fe ^ local_b3),1);
      auVar3 = vpinsrb_avx(auVar6,(uint)(local_1ed ^ local_b1),1);
      auVar4 = vpunpcklwd_avx(auVar4,auVar8);
      auVar3 = vpunpcklwd_avx(auVar7,auVar3);
      auVar7 = vmovd_avx((uint)(local_1ef ^ local_ac));
      auVar3 = vpunpckldq_avx(auVar5,auVar3);
      auVar7 = vpinsrb_avx(auVar7,(uint)(local_1f4 ^ local_ab),1);
      auVar2 = vpinsrb_avx(auVar2,(uint)(bVar27 ^ local_a9),1);
      auVar7 = vpunpcklwd_avx(auVar7,auVar2);
      auVar7 = vpunpckldq_avx(auVar4,auVar7);
      auVar7 = vpunpcklqdq_avx(auVar3,auVar7);
      local_98 = vmovdqa_avx(auVar7);
      if (uStack160._7_1_ == -1) {
        _local_a8 = _local_a8 & (undefined  [16])0xffffffffffffffff;
        lVar30 = 0xe;
        uStack160._7_1_ = uStack160._6_1_;
        if (uStack160._6_1_ == -1) {
          auVar23 = _local_a8;
          _local_a8 = ZEXT1415(_local_a8);
          lVar30 = 0xd;
          uStack160._7_1_ = uStack160._5_1_;
          if (uStack160._5_1_ == -1) {
            _local_a8 = _local_a8 & (undefined  [14])0xffffffffffffffff;
            _local_a8 = CONCAT214(uStack160._6_2_,auVar23) & (undefined  [16])0xffffffffffffffff;
            lVar30 = 0xc;
            uStack160._7_1_ = uStack160._4_1_;
            if (uStack160._4_1_ == -1) {
              auVar22 = _local_a8;
              _local_a8 = ZEXT1213(_local_a8);
              lVar30 = 0xb;
              uStack160._7_1_ = uStack160._3_1_;
              if (uStack160._3_1_ == -1) {
                _local_a8 = _local_a8 & (undefined  [12])0xffffffffffffffff;
                _local_a8 = CONCAT412(uStack160._4_4_,auVar22) & (undefined  [16])0xffffffffffffffff
                ;
                lVar30 = 10;
                uStack160._7_1_ = uStack160._2_1_;
                if (uStack160._2_1_ == -1) {
                  Var21 = _local_a8;
                  _local_a8 = ZEXT1011(_local_a8);
                  lVar30 = 9;
                  uStack160._7_1_ = uStack160._1_1_;
                  if (uStack160._1_1_ == -1) {
                    _local_a8 = _local_a8 & 0xffffffffffffffff;
                    _local_a8 = CONCAT610(uStack160._2_6_,Var21) &
                                (undefined  [16])0xffffffffffffffff;
                    lVar30 = 8;
                    uStack160._7_1_ = (byte)uStack160;
                    if ((byte)uStack160 == -1) {
                      uVar37 = _local_a8;
                      _local_a8 = (unkuint9)_local_a8;
                      lVar30 = 7;
                      uStack160._7_1_ = uStack161;
                      if (uStack161 == -1) {
                        _local_a8 = _local_a8 & 0xffffffffffffff;
                        _local_a8 = CONCAT88(uStack160,uVar37) & (undefined  [16])0xffffffffffffffff
                        ;
                        lVar30 = 6;
                        uStack160._7_1_ = auStack163[1];
                        if (auStack163[1] == -1) {
                          uVar20 = _local_a8;
                          _local_a8 = (uint7)_local_a8;
                          lVar30 = 5;
                          uStack160._7_1_ = auStack163[0];
                          if (auStack163[0] == -1) {
                            _local_a8 = _local_a8 & 0xffffffffff;
                            _local_a8 = CONCAT106(stack0xffffffffffffff5e,uVar20) &
                                        (undefined  [16])0xffff00ffffffffff;
                            lVar30 = 4;
                            uStack160._7_1_ = auStack165[1];
                            if (auStack165[1] == -1) {
                              uVar36 = _local_a8;
                              _local_a8 = (uint5)_local_a8;
                              lVar30 = 3;
                              uStack160._7_1_ = auStack165[0];
                              if (auStack165[0] == -1) {
                                _local_a8 = _local_a8 & 0xffffff;
                                _local_a8 = CONCAT124(stack0xffffffffffffff5c,uVar36) &
                                            (undefined  [16])0xffffffff00ffffff;
                                lVar30 = 2;
                                uStack160._7_1_ = local_a8[2];
                                if (local_a8[2] == -1) {
                                  lVar30 = 1;
                                  uStack160._7_1_ = local_a8[1];
                                  if (local_a8[1] == -1) {
                                    _local_a8 = CONCAT142(stack0xffffffffffffff5a,local_a8._0_2_) &
                                                (undefined  [16])0xffffffffffff00ff;
                                    lVar30 = 0;
                                    uStack160._7_1_ = local_a8[0];
                                    if (local_a8[0] == -1) {
                                      _local_a8 = ZEXT1416(stack0xffffffffffffff5a) << 0x10 &
                                                  (undefined  [16])0xffffffffffff0000;
                                      iVar24 = 0;
                                      goto LAB_00101385;
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      local_a8[lVar30] = uStack160._7_1_ + '\x01';
      iVar24 = 0;
    }
LAB_00101385:
    lVar30 = (long)iVar24;
    iVar24 = iVar24 + 1;
    *local_1e8 = *local_1e8 ^ local_98[lVar30];
    local_1e8 = local_1e8 + 1;
    if (local_48 == local_1e8) {
      iVar24 = memcmp(local_88,user_input,0x40);
      if (iVar24 == 0) {
        puts("Yep, that\'s it!");
      }
      else {
        iVar24 = 1;
        puts("Nope, come back later");
      }
      if (local_40 == *(long *)(in_FS_OFFSET + 0x28)) {
        return iVar24;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail();
    }
  } while( true );
}
```

However, we do see two interesting things in this function, without trying to reverse every line and line. The first thing is that it accepts `0x40` bytes from the user: `fread(user_input,1,0x40,stdin);`. And the second is how it knows if the input is correct:

```c
iVar24 = memcmp(local_88,user_input,0x40);
if (iVar24 == 0) {
    puts("Yep, that\'s it!");
}
```

So, the most trivial thing we can do is set a breakpoint at the `memcmp` and see what's being compared to what.

Trying to add a breakpoint at `memcmp` and run the program, we don't seem to hit it. So let's take it slower.

After opening the program with `gdb` again, we can execute `starti` to break at the first instruction of a program’s execution and then use `vmmap` to see the memory map:

```
gef>  vmmap
[ Legend:  Code | Heap | Stack ]
Start              End                Offset             Perm Path
0x0000555555554000 0x0000555555555000 0x0000000000000000 r-- /media/sf_CTFs/pico/not_crypto/not-crypto
0x0000555555555000 0x0000555555556000 0x0000000000001000 r-x /media/sf_CTFs/pico/not_crypto/not-crypto
0x0000555555556000 0x0000555555557000 0x0000000000002000 r-- /media/sf_CTFs/pico/not_crypto/not-crypto
0x0000555555557000 0x0000555555559000 0x0000000000002000 rw- /media/sf_CTFs/pico/not_crypto/not-crypto
0x00007ffff7fcc000 0x00007ffff7fd0000 0x0000000000000000 r-- [vvar]
0x00007ffff7fd0000 0x00007ffff7fd2000 0x0000000000000000 r-x [vdso]
0x00007ffff7fd2000 0x00007ffff7fd3000 0x0000000000000000 r-- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x00007ffff7fd3000 0x00007ffff7ff3000 0x0000000000001000 r-x /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x00007ffff7ff3000 0x00007ffff7ffb000 0x0000000000021000 r-- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x00007ffff7ffc000 0x00007ffff7ffe000 0x0000000000029000 rw- /usr/lib/x86_64-linux-gnu/ld-2.31.so
0x00007ffff7ffe000 0x00007ffff7fff000 0x0000000000000000 rw-
0x00007ffffffde000 0x00007ffffffff000 0x0000000000000000 rw- [stack]
```

We can see that our address space starts at `0x0000555555554000`. We can rebase Ghidra to use the same base address (`Window` -> `Memory Map` -> Home Icon) so that's it's easier to get oriented. 

Now, we see in Ghidra that `memcmp` is called here:

```assembly
    5555555553aa 48 8b 74        MOV        RSI,qword ptr [RSP + local_1c8]
                 24 40
    5555555553af ba 40 00        MOV        EDX,0x40
                 00 00
    5555555553b4 48 8b 7c        MOV        RDI,qword ptr [RSP + local_1c0]
                 24 48
    5555555553b9 e8 a2 fc        CALL       memcmp                                           int memcmp(void * __s1, void * _
                 ff ff
    5555555553be 41 89 c4        MOV        R12D,EAX
    5555555553c1 85 c0           TEST       EAX,EAX
```

So, let's set a breakpoint at `0x5555555553b9` by executing `b *0x5555555553b9`.

We run the program, enter some input and hit:

```
gef>  r
Starting program: /media/sf_CTFs/pico/not_crypto/not-crypto
I heard you wanted to bargain for a flag... whatcha got?
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

Breakpoint 1, 0x00005555555553b9 in ?? ()
[ Legend: Modified register | Code | Heap | Stack | String ]
───────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────$rax   : 0x10
$rbx   : 0x00007fffffffe840  →  0x00007fffffffe867  →  0x00555555555c7000
$rcx   : 0xa4
$rdx   : 0x40
$rsp   : 0x00007fffffffe680  →  0x0000000000000000
$rbp   : 0xa1
$rsi   : 0x00007fffffffe6f0  →  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa[...]"
$rdi   : 0x00007fffffffe800  →  "picoCTF{c0mp1l3r_0pt1m1z4t10n_15_pur3_w1z4rdry_but[...]"
$rip   : 0x00005555555553b9  →   call 0x555555555060 <memcmp@plt>
$r8    : 0xba
$r9    : 0x96
$r10   : 0xf0
$r11   : 0x6a
$r12   : 0x97
$r13   : 0x73
$r14   : 0xf9
$r15   : 0x3a
$eflags: [ZERO carry PARITY adjust sign trap INTERRUPT direction overflow resume virtualx86 identification]
$cs: 0x0033 $ss: 0x002b $ds: 0x0000 $es: 0x0000 $fs: 0x0000 $gs: 0x0000
───────────────────────────────────────────────────────────────────────────────────────────────────────────── stack ────0x00007fffffffe680│+0x0000: 0x0000000000000000   ← $rsp
0x00007fffffffe688│+0x0008: 0x000000c7e1530000
0x00007fffffffe690│+0x0010: 0x0c0c21f9000000cd
0x00007fffffffe698│+0x0018: 0xea97bc0c97f759d4
0x00007fffffffe6a0│+0x0020: 0x00007fffffffe840  →  0x00007fffffffe867  →  0x00555555555c7000
0x00007fffffffe6a8│+0x0028: 0xb4a0926e3f652ca3
0x00007fffffffe6b0│+0x0030: 0x00007fffffffe840  →  0x00007fffffffe867  →  0x00555555555c7000
0x00007fffffffe6b8│+0x0038: 0x00007fffffffe7d0  →  0x6ecb6adbc09a121a
─────────────────────────────────────────────────────────────────────────────────────────────────────── code:x86:64 ────   0x5555555553aa                  mov    rsi, QWORD PTR [rsp+0x40]
   0x5555555553af                  mov    edx, 0x40
   0x5555555553b4                  mov    rdi, QWORD PTR [rsp+0x48]
●→ 0x5555555553b9                  call   0x555555555060 <memcmp@plt>
   ↳  0x555555555060 <memcmp@plt+0>   jmp    QWORD PTR [rip+0x2fca]        # 0x555555558030 <memcmp@got.plt>
      0x555555555066 <memcmp@plt+6>   push   0x3
      0x55555555506b <memcmp@plt+11>  jmp    0x555555555020
      0x555555555070                  push   r15
      0x555555555072                  lea    rdi, [rip+0xf8f]        # 0x555555556008
      0x555555555079                  push   r14
─────────────────────────────────────────────────────────────────────────────────────────────── arguments (guessed) ────memcmp@plt (
   $rdi = 0x00007fffffffe800 → "picoCTF{c0mp1l3r_0pt1m1z4t10n_15_pur3_w1z4rdry_but[...]",
   $rsi = 0x00007fffffffe6f0 → "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa[...]",
   $rdx = 0x0000000000000040
)
─────────────────────────────────────────────────────────────────────────────────────────────────────────── threads ────[#0] Id 1, Name: "not-crypto", stopped 0x5555555553b9 in ?? (), reason: BREAKPOINT
───────────────────────────────────────────────────────────────────────────────────────────────────────────── trace ────[#0] 0x5555555553b9 → call 0x555555555060 <memcmp@plt>
[#1] 0x7ffff7e15d0a → __libc_start_main(main=0x555555555070, argc=0x1, argv=0x7fffffffe978, init=<optimized out>, fini=<optimized out>, rtld_fini=<optimized out>, stack_end=0x7fffffffe968)
[#2] 0x555555555c9e → hlt
```

We can see part the flag in `$rdi`, but the value is cut. Let's fully print it:

```
gef>  printf "%s", $rdi
picoCTF{c0mp1l3r_0pt1m1z4t10n_15_pur3_w1z4rdry_but_n0_pr0bl3m?}
```