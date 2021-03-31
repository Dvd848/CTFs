# Hatter
Category: Reversing, 500 Points

## Description
> Hello agent!
> 
> According to our intelligence, the organization’s engineers are celebrating their evil inventions by having a party in which everyone is wearing the same crazy hat.
> 
> We must send agent to the party to find out what they’re up to. The only way for you to get in is by wearing the same hat as them. Our intelligence has located the store on which the organization bought the hats from, but the store was empty and contained a note:
> 
> "Hello fellas, this is the hatter! I’m attending a special event and will be back in a few days. If you wish to talk to me, you can find me at… well.. check out this file".
> 
> Note: The flag does not adhere to the usual format.
> 

A binary file was attached.

## Solution

Let's run the attached binary:

```console
root@kali:/media/sf_CTFs/matrix/Hatter# ./hatter
find_thE_hAttEr
```

Indeed, we will. Maybe `ltrace` might give us a hint as to what the program is doing under the hood:

```console
root@kali:/media/sf_CTFs/matrix/Hatter# ltrace ./hatter
getenv("DEBUG")                                                           = nil
getenv("WHERE_IS_THE_HATTER")                                             = nil
getenv("SHOW_PASSWORD")                                                   = nil
getenv("DUMP_DEBUG_DATA")                                                 = nil
putchar(102, 0x511373df0155593f, 7, 0)                                    = 102
putchar(105, 0x511373df0155593f, 6, 1)                                    = 105
putchar(110, 0x511373dc02010f01, 2, 2)                                    = 110
putchar(100, 0x511373de00012841, 0, 3)                                    = 100
putchar(95, 0x511373da046d0142, 1, 4)                                     = 95
putchar(116, 0x511373de00012841, 6, 5)                                    = 116
putchar(104, 0x511373dc02010f01, 0, 6)                                    = 104
putchar(69, 0x511373db050b7a72, 4, 7)                                     = 69
putchar(95, 0x511373da046d0142, 1, 8)                                     = 95
putchar(104, 0x511373dc02010f01, 0, 9)                                    = 104
putchar(65, 0x511373de00012841, 5, 10)                                    = 65
putchar(116, 0x511373de00012841, 6, 11)                                   = 116
putchar(116, 0x511373de00012841, 6, 12)                                   = 116
putchar(69, 0x511373db050b7a72, 4, 13)                                    = 69
putchar(114, 0x511373df0155593f, 5, 14)                                   = 114
putchar(10, 255, 255, 15find_thE_hAttEr
)                                                 = 10
+++ exited (status 0) +++
```

We see that the program queries some environment variables, then prints the message character by character. We can try setting these variables but let's take a look at the Ghidra output first to see if there's an expected value. This is the main function:

```c
undefined8 main(int argc,undefined8 param_2)
{
  undefined8 local_38;
  undefined8 local_30;
  undefined8 local_28;
  undefined8 local_20;
  undefined8 local_18;
  int num_args;
  undefined4 local_c;
  
  local_c = 0;
  local_38 = 0x20206010701;
  local_30 = 0x405000206000104;
  local_28 = 0x600050000020104;
  local_20 = 0xffff050104050600;
  local_18 = param_2;
  num_args = argc;
  read_env_vars();
  if (g_env_show_pass == '\x01') {
    handle_show_pass();
  }
  if (g_env_where_hatter == '\x01') {
    handle_where_hatter();
  }
  else {
    if (g_env_dump_debug == '\x01') {
      handle_dump_debug();
    }
    else {
      if (num_args == 1) {
        handle_no_args((char *)&local_38);
      }
      else {
        handle_some_args();
      }
    }
  }
  return 0;
}
```

We can see that it starts by reading the environment variables:

```c
void read_env_vars(void)
{
  char *pcVar1;
  
  pcVar1 = getenv("DEBUG");
  g_env_debug = pcVar1 != (char *)0x0;
  pcVar1 = getenv("WHERE_IS_THE_HATTER");
  g_env_where_hatter = pcVar1 != (char *)0x0;
  pcVar1 = getenv("SHOW_PASSWORD");
  g_env_show_pass = pcVar1 != (char *)0x0;
  pcVar1 = getenv("DUMP_DEBUG_DATA");
  g_env_dump_debug = pcVar1 != (char *)0x0;
  return;
}
```

If the variables are set, global flags get raised.

We can try to see what happens when each of the flags is set:

```console
root@kali:/media/sf_CTFs/matrix/Hatter# export DEBUG=1
root@kali:/media/sf_CTFs/matrix/Hatter# ./hatter
Enter "pRnTE"
find_thE_hAttEr
root@kali:/media/sf_CTFs/matrix/Hatter# unset DEBUG
root@kali:/media/sf_CTFs/matrix/Hatter# export WHERE_IS_THE_HATTER=1
root@kali:/media/sf_CTFs/matrix/Hatter# ./hatter
in ordEr to find thE hAttEr YoU hAvE to find ALL thE 6 LinE5
root@kali:/media/sf_CTFs/matrix/Hatter# unset WHERE_IS_THE_HATTER
root@kali:/media/sf_CTFs/matrix/Hatter# export SHOW_PASSWORD=1
root@kali:/media/sf_CTFs/matrix/Hatter# ./hatter
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMWWWWWNWWWWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMWd,.....       .............',;cloxO0XWMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMWd..o00KKXXNNWN0kO0OOOOOOOkkkkOkxdoc,,.  .';lxNMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMX' dWMMMMMMMXkxx0WMWXXKXXNMWNX000000000Ok0Oxo;. ;OMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMK..KMMMMMMWOkOW0xxxkkO0K0OkkO0KKKKK0OOkkkO0XN0dW0: .xWMMMMMMMMMMMMMM
MMMMMMMMMMMK..XMMMMMKOONOxxxkk0KK0OOO0NMMMMMWNNNNWWWWMMMMMNWMMXc .kMMMMMMMMMMMMM
MMMMMMMMMMN..KMMMMK0XWxd00dxKWMMMMMMNoNMMMMMMMM0dxxkkOKXOxXMMMMMK' dMMMMMMMMMMMM
MMMMMMMMMM; OMMMMXKMOoOlxWMMMMMMMMMMMWoMMMMMMMMM0KMMMMMMMNlkMMMMMM.;MMMMMMMMMMMM
MMMMMMMMMx oMMMMMMMK0XoWMMWKOkkk0XMMMMkMMMMMMMMMdMMMMMMMMMMOMMMMMM.'MMMMMMMMMMMM
MMMMMMMMx ,WMMMMMMMMMWMNo.         ,oXMMMMMMMMMMxMMMWNXKXNMMMMMMMMl xMMMMMMMMMMM
MMMMMMX, .KWWMMMMMMMWMO ..      NNKd. cWMMMMMMMMX0c.      lNMMMMMMW; :NMMMMMMMMM
MMMMMd  ..ldxOKMMX:;OX  ..      ';xWW' :MMMMWO00'     .'.  oXddk0KkXx..xMMMMMMMM
MMMN, ,;NMWOl:;:o0WNkWkdx0NW0lX0x:. '. oMMMMN'    ':ldxkOKWMMMMMMNXxxX: lMMMMMMM
MMN..xoWMx..:odo, .:kNMMMWO, ;MMMMMXolXMMMMMMMN'.WMMMMMMMMMMXkkkkOkxOkO,.MMMMMMM
MM;.XxWMl oWMMd:MXd;. .... :0MMMMMMMMMMMMMMMMMM;.MMMMMMdkWXl .,;. ;NoMd;.MMMMMMM
MX oMkM0 cMMMM. dXMMMWX000NMMMMMMMMMMMMMMMMMMMMc ;KMMMMl    ;WkWM0kMdMx:.MMMMMMM
M0 kMkMk OMXk: .. 'dXMMMMMMMMMMMMx0:..;WMMMMMMMWk' ,0MMMWXKKN0 OMMMWkMx;.MMMMMMM
MK lMxW0 od .. OMXo. 'o0WMMNk000O0'.xOKNNWMMMMMMMMc  :WMMMMMMx ;XMKoKW:.,MMMMMMM
MM. XOOW. NMMN..NMMMKl  .,oONMMMMM;.0k    .XMMMMM0...,kOkNMMW:  lMW0kd  OMMMMMMM
MM0 .NxKXcNMMM' .oXMMM.'kl,. .;oOKO';NxWWKKWMdkKl ,NMMMMN0Wk.   'WkKMl oMMMMMMMM
MMM0.'XOxOWMMMW:   .cO. KMMMNOo;.  'cx0NMMMMMl. .xMMMMMMXd. .'   WMMk cMMMMMMMMM
MMMMX, .xWMMMMMM: :o'    lKMMMMMx cl;'.  .,:lodxxkxxoc;. ,d'.N.  XMM..WMMMMMMMMM
MMMMMWl ,NMMMMMMW, OMW.    .;o0W;.WMMMMN0o :oc,,  ,;cdd.,MMo N:  KMX lMMMMMMMMMM
MMMMMMMO..XMMMMMMW; kMl xl'      .0NMMMMMK kMMMM;.MMMMMc ON, .   KM0 oMMMMMMMMMM
MMMMMMMMO :MMMMMMMMo ..'WMMNx:.     .':cl; ,cccc. cl:;'.         XMO dMMMMMMMMMM
MMMMMMMMM: kMMMMMMMMK. oWMMMMMM: o:.                            .MM0 dMMMMMMMMMM
MMMMMMMMMW, OMMMMMMMMWx..oWMMMN.'MMMNOo:.                     . ;MMK oMMMMMMMMMM
MMMMMMMMMMW: oWMMMMMMMMWd..oKW' KMMMMMMM,.WKkdd:  :c; .;l .d .: kMMN cMMMMMMMMMM
MMMMMMMMMMMMx 'KMNXMMNWMMWx.   lMMMMMMMM'.MMMMMl XMMk cMo xO . ,MMMM.;MMMMMMMMMM
MMMMMMMMMMMMMX, lKdoON0ddKMMKo' .;oOXWMW..MMMMM''MMW'.N0  l:  :NMMMM''MMMMMMMMMM
MMMMMMMMMMMMMMMd  c00xxONOk0NMMNOl;.  ..  ;cc:: .c:.  .  .;oOWMMMMMM:.MMMMMMMMMM
MMMMMMMMMMMMMMMMWk; .cKKxxOXKkxxXMMMWNK0Okxddoooodxxk0KNMMMMM0MMMWMMl WMMMMMMMMM
MMMMMMMMMMMMMMMMMMMWx' ;kXNOkO0KxxxONMMMMNKKK000OO00XWMMMMMNkOMMMxMMx XMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMO;. .:xNN0000xdOKXNNNXK0000000000000KXMMMNx0MMO 0MMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMNk:. ,xWMMWK00OOO0KNMMMMMWWNNWMMMMMMKxxWMMM0 OMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMWO:  :OWMMMMMMWX00000000OOOO000OxOMMMMMW: 0MMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWk; .l0XWMMMMMMMMMMMMMMMMMMMMMMMMMMMc cMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWk:.   .;lkXMMMMMMMMMMMMMMMMMMMO; dMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNKkl;. .,lx0NMMMMMMMMWKx:. cXMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNOd:'.  ...'... .'ckWMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWXK0OOO0XNMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWKKNMMMMMNKKWMMMMMMMMMMMMMMMMMMMMMMMN0OKWMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMK  oMMMMMc  KMMMMMMMMMMMMMMMMMMMMMMc    .0MMMMMMMM
MMMMMMMMxoOxcoXMKooOcxMKoclxNMK  :ocxWMc  KMNxlco0MMkokkll0KlckMK..oO  :MMMMMMMM
MMMMMMMM.  .. .Xx   .xo  ,. .KK   .  'Wc  KK. ,l. cM'  .   ..  dMMMk. .KMMMMMMMM
MMMMMMMM. 'M0  dx  dMW  ;MN  :K  lMo  Kc  Kc  ;:.  X' .Wo  OX  lMMK  cWMMMMMMMMM
MMMMMMMM. .0o  xx  OMM. 'NO  oK  ,K;  Nc  Ko  oklloW' .Md  0N  lMMK;;KMMMMMMMMMM
MMMMMMMM.     ;Wx  OMMK'    cWK  .   dMc  KW:  . .xM' .Md  0N  lMMk  kMMMMMMMMMM
MMMMMMMM. ,WKNMMWNNWMMMMNKKWMMMNNWNKWMMWNNMMMWKKXMMMNNNMWNNWMNNWMMWNNWMMMMMMMMMM
MMMMMMMM:'lMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
find_thE_hAttEr
root@kali:/media/sf_CTFs/matrix/Hatter# unset SHOW_PASSWORD
root@kali:/media/sf_CTFs/matrix/Hatter# export DUMP_DEBUG_DATA=1
root@kali:/media/sf_CTFs/matrix/Hatter# ./hatter
d)=%55A(sQeOVEAYUsQj:T^lsQj-pa'Gt
F/sQT+B]lBmsQu"<:Ex'rzsQ
root@kali:/media/sf_CTFs/matrix/Hatter# ./hatter | xxd -g 1
00000000: 64 29 3d 0c 18 25 35 35 41 28 01 00 de 73 13 51  d)=..%55A(...s.Q
00000010: 65 0b 4f 56 45 41 1b 0f 3f 59 55 01 df 73 13 51  e.OVEA..?YU..s.Q
00000020: 6a 01 07 3a 54 5e 6c 04 01 0f 01 02 dc 73 13 51  j..:T^l......s.Q
00000030: 6a 2d 70 61 27 47 74 0d 0a 46 2f 03 dd 73 13 51  j-pa'Gt..F/..s.Q
00000040: 54 0f 2b 06 42 01 5d 6c 42 01 6d 04 da 73 13 51  T.+.B.]lB.m..s.Q
00000050: 75 22 3c 3a 11 45 78 27 72 7a 0b 05 db 73 13 51  u"<:.Ex'rz...s.Q
```

We also know from `main()` that the programs takes a different path in case command line arguments are provided to it, so let's try that:

```console
root@kali:/media/sf_CTFs/matrix/Hatter# unset DUMP_DEBUG_DATA
root@kali:/media/sf_CTFs/matrix/Hatter# ./hatter test
The hatter left no traces
```

It's not very clear what we have so let's dig deeper. The first thing that `main` checked is whether `SHOW_PASSWORD` was set. If so, it calls:

```c
void handle_show_pass(void)
{
  undefined local_1178 [4464];
  
  memcpy(local_1178,
                  
         "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nMMMMMMMMMMMMMMMMMMMMMMMMMMWWWWWNWWWWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nMMMMMMMMMMMMMMMMWd,.....     .............\',;cloxO0XWMMMMMMMMMMMMMMMMMMMMMMMMM\nMMMMMMMMMMMMMMWd..o00KKXXNNWN0kO0OOOOOOOkkkkOkxdoc,,.  .\';lxNMMMMMMMMMMMMMMMMMMM\nMMMMMMMMMMMMMX\'dWMMMMMMMXkxx0WMWXXKXXNMWNX000000000Ok0Oxo;.;OMMMMMMMMMMMMMMMMM\nMMMMMMMMMMMMK..KMMMMMMWOkOW0xxxkkO0K0OkkO0KKKKK0OOkkkO0XN0dW0:.xWMMMMMMMMMMMMMM\nMMMMMMMMMMMK..XMMMMMKOONOxxxkk0KK0OOO0NMMMMMWNNNNWWWWMMMMMNWMMXc.kMMMMMMMMMMMMM\nMMMMMMMMMMN..KMMMMK0XWxd00dxKWMMMMMMNoNMMMMMMMM0dxxkkOKXOxXMMMMMK\'dMMMMMMMMMMMM\nMMMMMMMMMM;OMMMMXKMOoOlxWMMMMMMMMMMMWoMMMMMMMMM0KMMMMMMMNlkMMMMMM.;MMMMMMMMMMMM\nMMMMMMMMMxoMMMMMMMK0XoWMMWKOkkk0XMMMMkMMMMMMMMMdMMMMMMMMMMOMMMMMM.\'MMMMMMMMMMMM\nMMMMMMMMx,WMMMMMMMMMWMNo.         ,oXMMMMMMMMMMxMMMWNXKXNMMMMMMMMl xMMMMMMMMMMM\nMMMMMMX,.KWWMMMMMMMWMO ..      NNKd. cWMMMMMMMMX0c.      lNMMMMMMW; :NMMMMMMMMM\nMMMMMd ..ldxOKMMX:;OX  ..      \';xWW\' :MMMMWO00\'     .\'.  oXddk0KkXx..xMMMMMMMM\nMMMN,,;NMWOl:;:o0WNkWkdx0NW0lX0x:. \'. oMMMMN\'    \':ldxkOKWMMMMMMNXxxX:lMMMMMMM\nMMN..xoWMx..:odo, .:kNMMMWO,;MMMMMXolXMMMMMMMN\'.WMMMMMMMMMMXkkkkOkxOkO,.MMMMMMM\nMM;.XxWMl oWMMd:MXd;. ....:0MMMMMMMMMMMMMMMMMM;.MMMMMMdkWXl .,;. ;NoMd;.MMMMMMM\nMX oMkM0 cMMMM.dXMMMWX000NMMMMMMMMMMMMMMMMMMMMc ;KMMMMl    ;WkWM0kMdMx:.MMMMMMM\nM0 kMkMk OMXk: ..\'dXMMMMMMMMMMMMx0:..;WMMMMMMMWk\' ,0MMMWXKKN0 OMMMWkMx;.MMMMMMM\nMK lMxW0 od .. OMXo.\'o0WMMNk000O0\'.xOKNNWMMMMMMMMc  :WMMMMMMx ;XMKoKW:.,MMMMMMM\nMM. XOOW. NMMN..NMMMKl .,oONMMMMM;.0k    .XMMMMM0...,kOkNMMW:  lMW0kd  OMMMMMMM\nMM0 .NxKXcNMMM\' .oXMMM.\'kl,..;oOKO\';NxWWKKWMdkKl ,NMMMMN0Wk.   \'WkKMl oMMMMMMMM\nMMM0.\'XOxOWMMMW:   .cO. KMMMNOo;. \'cx0NMMMMMl. .xMMMMMMXd. .\'   WMMk cMMMMMMMMM\nMMMMX, .xWMMMMMM: :o\'    lKMMMMMx cl;\'. .,:lodxxkxxoc;. ,d\'.N.  XMM..WMMMMMMMMM\nMMMMMWl ,NMMMMMMW, OMW.    .;o0W;.WMMMMN0o:oc,,  ,;cdd.,MMo N:  KMX lMMMMMMMMMM\nMMMMMMMO..XMMMMMMW; kMl..." /* TRUNCATED STRINGLITERAL */
         ,4456);
  print_hint_in_debug(4);
  printf("%s",local_1178);
  return;
}
```

This function contains a call to what we named `print_hint_in_debug`, since it only prints something when `DEBUG` is set:
```c
void print_hint_in_debug(uint hint_num)

{
  if (g_env_debug == '\x01') {
    print_hint(hint_num);
  }
  return;
}
```

We'll look at `print_hint` later, let's check a different path first. The next path is taken if `WHERE_IS_THE_HATTER` is set.

```c
void handle_where_hatter(void)

{
  char local_238 [128];
  char *local_1b8;
  char *local_1b0;
  char *local_1a8;
  char *local_1a0;
  char *local_198;
  char *local_190;
  char local_188 [64];
  char local_148 [64];
  char local_108 [64];
  char local_c8 [64];
  char local_88 [64];
  char local_48 [53];
  undefined local_13 [11];
  
  local_190 = local_188;
  local_198 = local_148;
  local_1a0 = local_108;
  local_1a8 = local_c8;
  local_1b0 = local_88;
  local_1b8 = local_48;
  memcpy(local_1b8,&DAT_00403210,40);
  memcpy(local_1b0,&DAT_00403240,56);
  memcpy(local_1a8,&DAT_00403280,64);
  memcpy(local_1a0,&DAT_004032c0,64);
  memcpy(local_198,&DAT_00403300,64);
  memcpy(local_190,&DAT_00403340,64);
  memcpy(local_238,&DAT_00403380,122);
  memset(local_13,0xb,0xb);
  print_hint_in_debug(2);
  handle_no_args(local_238);
  return;
}
```

Again, we see a call to `print_hint_in_debug`. What about the path where `DUMP_DEBUG_DATA` is set?

```c
void handle_dump_debug(void)
{
  print_hint_in_debug(0);
  write(1,&DAT_00405060,96);
  return;
}
```

And the two remaining paths:
```c
void handle_no_args(char *param_1)
{
  ulong uVar1;
  int i;
  undefined8 local_24;
  undefined8 *p_hint;
  char *local_10;
  
  p_hint = (undefined8 *)0x0;
  local_10 = param_1;
  print_hint_in_debug(5);
  i = 0;
  while( true ) {
    if (local_10[(long)i * 2] == -1) {
      putchar(10);
      return;
    }
    p_hint = (undefined8 *)get_ptr_for_hint_num((uint)(byte)local_10[(long)i * 2]);
    if (p_hint == (undefined8 *)0x0) break;
    uVar1 = decode_hint(p_hint,&local_24);
    if ((char)uVar1 != '\0') {
      putchar((int)*(char *)((long)&local_24 + (ulong)(byte)local_10[(long)i * 2 + 1]));
    }
    i = i + 1;
  }
  return;
}
```

And:

```c
void handle_some_args(void)
{
  print_hint_in_debug(1);
  printf("The hatter left no traces\n");
  return;
}
```

So if we collect all calls to `print_hint_in_debug`, we see that it's called with the parameters `{4, 2, 0, 5, 1}`. We should repeat our previous experiment with `DEBUG=1` always set:

```console
root@kali:/media/sf_CTFs/matrix/Hatter# export DEBUG=1
root@kali:/media/sf_CTFs/matrix/Hatter# ./hatter
Enter "pRnTE"
find_thE_hAttEr
root@kali:/media/sf_CTFs/matrix/Hatter# export WHERE_IS_THE_HATTER=1
root@kali:/media/sf_CTFs/matrix/Hatter# ./hatter
Enter "hinT"
Enter "pRnTE"
in ordEr to find thE hAttEr YoU hAvE to find ALL thE 6 LinE5
root@kali:/media/sf_CTFs/matrix/Hatter# unset WHERE_IS_THE_HATTER
root@kali:/media/sf_CTFs/matrix/Hatter# export SHOW_PASSWORD=1
root@kali:/media/sf_CTFs/matrix/Hatter# ./hatter
Enter "P_tr01l"
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMWWWWWNWWWWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMWd,.....       .............',;cloxO0XWMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMWd..o00KKXXNNWN0kO0OOOOOOOkkkkOkxdoc,,.  .';lxNMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMX' dWMMMMMMMXkxx0WMWXXKXXNMWNX000000000Ok0Oxo;. ;OMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMK..KMMMMMMWOkOW0xxxkkO0K0OkkO0KKKKK0OOkkkO0XN0dW0: .xWMMMMMMMMMMMMMM
MMMMMMMMMMMK..XMMMMMKOONOxxxkk0KK0OOO0NMMMMMWNNNNWWWWMMMMMNWMMXc .kMMMMMMMMMMMMM
MMMMMMMMMMN..KMMMMK0XWxd00dxKWMMMMMMNoNMMMMMMMM0dxxkkOKXOxXMMMMMK' dMMMMMMMMMMMM
MMMMMMMMMM; OMMMMXKMOoOlxWMMMMMMMMMMMWoMMMMMMMMM0KMMMMMMMNlkMMMMMM.;MMMMMMMMMMMM
MMMMMMMMMx oMMMMMMMK0XoWMMWKOkkk0XMMMMkMMMMMMMMMdMMMMMMMMMMOMMMMMM.'MMMMMMMMMMMM
MMMMMMMMx ,WMMMMMMMMMWMNo.         ,oXMMMMMMMMMMxMMMWNXKXNMMMMMMMMl xMMMMMMMMMMM
MMMMMMX, .KWWMMMMMMMWMO ..      NNKd. cWMMMMMMMMX0c.      lNMMMMMMW; :NMMMMMMMMM
MMMMMd  ..ldxOKMMX:;OX  ..      ';xWW' :MMMMWO00'     .'.  oXddk0KkXx..xMMMMMMMM
MMMN, ,;NMWOl:;:o0WNkWkdx0NW0lX0x:. '. oMMMMN'    ':ldxkOKWMMMMMMNXxxX: lMMMMMMM
MMN..xoWMx..:odo, .:kNMMMWO, ;MMMMMXolXMMMMMMMN'.WMMMMMMMMMMXkkkkOkxOkO,.MMMMMMM
MM;.XxWMl oWMMd:MXd;. .... :0MMMMMMMMMMMMMMMMMM;.MMMMMMdkWXl .,;. ;NoMd;.MMMMMMM
MX oMkM0 cMMMM. dXMMMWX000NMMMMMMMMMMMMMMMMMMMMc ;KMMMMl    ;WkWM0kMdMx:.MMMMMMM
M0 kMkMk OMXk: .. 'dXMMMMMMMMMMMMx0:..;WMMMMMMMWk' ,0MMMWXKKN0 OMMMWkMx;.MMMMMMM
MK lMxW0 od .. OMXo. 'o0WMMNk000O0'.xOKNNWMMMMMMMMc  :WMMMMMMx ;XMKoKW:.,MMMMMMM
MM. XOOW. NMMN..NMMMKl  .,oONMMMMM;.0k    .XMMMMM0...,kOkNMMW:  lMW0kd  OMMMMMMM
MM0 .NxKXcNMMM' .oXMMM.'kl,. .;oOKO';NxWWKKWMdkKl ,NMMMMN0Wk.   'WkKMl oMMMMMMMM
MMM0.'XOxOWMMMW:   .cO. KMMMNOo;.  'cx0NMMMMMl. .xMMMMMMXd. .'   WMMk cMMMMMMMMM
MMMMX, .xWMMMMMM: :o'    lKMMMMMx cl;'.  .,:lodxxkxxoc;. ,d'.N.  XMM..WMMMMMMMMM
MMMMMWl ,NMMMMMMW, OMW.    .;o0W;.WMMMMN0o :oc,,  ,;cdd.,MMo N:  KMX lMMMMMMMMMM
MMMMMMMO..XMMMMMMW; kMl xl'      .0NMMMMMK kMMMM;.MMMMMc ON, .   KM0 oMMMMMMMMMM
MMMMMMMMO :MMMMMMMMo ..'WMMNx:.     .':cl; ,cccc. cl:;'.         XMO dMMMMMMMMMM
MMMMMMMMM: kMMMMMMMMK. oWMMMMMM: o:.                            .MM0 dMMMMMMMMMM
MMMMMMMMMW, OMMMMMMMMWx..oWMMMN.'MMMNOo:.                     . ;MMK oMMMMMMMMMM
MMMMMMMMMMW: oWMMMMMMMMWd..oKW' KMMMMMMM,.WKkdd:  :c; .;l .d .: kMMN cMMMMMMMMMM
MMMMMMMMMMMMx 'KMNXMMNWMMWx.   lMMMMMMMM'.MMMMMl XMMk cMo xO . ,MMMM.;MMMMMMMMMM
MMMMMMMMMMMMMX, lKdoON0ddKMMKo' .;oOXWMW..MMMMM''MMW'.N0  l:  :NMMMM''MMMMMMMMMM
MMMMMMMMMMMMMMMd  c00xxONOk0NMMNOl;.  ..  ;cc:: .c:.  .  .;oOWMMMMMM:.MMMMMMMMMM
MMMMMMMMMMMMMMMMWk; .cKKxxOXKkxxXMMMWNK0Okxddoooodxxk0KNMMMMM0MMMWMMl WMMMMMMMMM
MMMMMMMMMMMMMMMMMMMWx' ;kXNOkO0KxxxONMMMMNKKK000OO00XWMMMMMNkOMMMxMMx XMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMO;. .:xNN0000xdOKXNNNXK0000000000000KXMMMNx0MMO 0MMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMNk:. ,xWMMWK00OOO0KNMMMMMWWNNWMMMMMMKxxWMMM0 OMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMWO:  :OWMMMMMMWX00000000OOOO000OxOMMMMMW: 0MMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWk; .l0XWMMMMMMMMMMMMMMMMMMMMMMMMMMMc cMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWk:.   .;lkXMMMMMMMMMMMMMMMMMMMO; dMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNKkl;. .,lx0NMMMMMMMMWKx:. cXMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNOd:'.  ...'... .'ckWMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWXK0OOO0XNMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMWKKNMMMMMNKKWMMMMMMMMMMMMMMMMMMMMMMMN0OKWMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMK  oMMMMMc  KMMMMMMMMMMMMMMMMMMMMMMc    .0MMMMMMMM
MMMMMMMMxoOxcoXMKooOcxMKoclxNMK  :ocxWMc  KMNxlco0MMkokkll0KlckMK..oO  :MMMMMMMM
MMMMMMMM.  .. .Xx   .xo  ,. .KK   .  'Wc  KK. ,l. cM'  .   ..  dMMMk. .KMMMMMMMM
MMMMMMMM. 'M0  dx  dMW  ;MN  :K  lMo  Kc  Kc  ;:.  X' .Wo  OX  lMMK  cWMMMMMMMMM
MMMMMMMM. .0o  xx  OMM. 'NO  oK  ,K;  Nc  Ko  oklloW' .Md  0N  lMMK;;KMMMMMMMMMM
MMMMMMMM.     ;Wx  OMMK'    cWK  .   dMc  KW:  . .xM' .Md  0N  lMMk  kMMMMMMMMMM
MMMMMMMM. ,WKNMMWNNWMMMMNKKWMMMNNWNKWMMWNNMMMWKKXMMMNNNMWNNWMNNWMMWNNWMMMMMMMMMM
MMMMMMMM:'lMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
Enter "pRnTE"
find_thE_hAttEr
root@kali:/media/sf_CTFs/matrix/Hatter# unset SHOW_PASSWORD
root@kali:/media/sf_CTFs/matrix/Hatter# export DUMP_DEBUG_DATA=1
root@kali:/media/sf_CTFs/matrix/Hatter# ./hatter | xxd -g 1
00000000: 64 29 3d 0c 18 25 35 35 41 28 01 00 de 73 13 51  d)=..%55A(...s.Q
00000010: 65 0b 4f 56 45 41 1b 0f 3f 59 55 01 df 73 13 51  e.OVEA..?YU..s.Q
00000020: 6a 01 07 3a 54 5e 6c 04 01 0f 01 02 dc 73 13 51  j..:T^l......s.Q
00000030: 6a 2d 70 61 27 47 74 0d 0a 46 2f 03 dd 73 13 51  j-pa'Gt..F/..s.Q
00000040: 54 0f 2b 06 42 01 5d 6c 42 01 6d 04 da 73 13 51  T.+.B.]lB.m..s.Q
00000050: 75 22 3c 3a 11 45 78 27 72 7a 0b 05 db 73 13 51  u"<:.Ex'rz...s.Q
00000060: 45 6e 74 65 72 20 22 64 4d 70 7c 64 41 74 41 22  Enter "dMp|dAtA"
00000070: 0a                                               .
root@kali:/media/sf_CTFs/matrix/Hatter# unset DUMP_DEBUG_DATA
root@kali:/media/sf_CTFs/matrix/Hatter# ./hatter test
Enter "do v3rifY"
The hatter left no traces
root@kali:/media/sf_CTFs/matrix/Hatter#
```

It looks like we've collected five "Enter" strings:

```
Enter "pRnTE"
Enter "hinT"
Enter "P_tr01l"
Enter "dMp|dAtA"
Enter "do v3rifY"
```

Furthermore, one of the output strings we saw before said that `"in ordEr to find thE hAttEr YoU hAvE to find ALL thE 6 LinE5"`, so we must be on the right track. In order to print the remaining line, we should try to call `print_hint_in_debug` with the argument `3` which was missing from the list of arguments we saw above. We'll just intercept a different call with the debugger and change the parameter:

```console
root@kali:/media/sf_CTFs/matrix/Hatter# gdb -n -q ./hatter
Reading symbols from ./hatter...
(No debugging symbols found in ./hatter)
(gdb) b *0x004014a0
Breakpoint 1 at 0x4014a0
(gdb) r
Starting program: /media/sf_CTFs/matrix/Hatter/hatter

Breakpoint 1, 0x00000000004014a0 in ?? ()
(gdb) set disassembly-flavor intel
(gdb) disas $rip, +20
Dump of assembler code from 0x4014a0 to 0x4014b4:
=> 0x00000000004014a0:  push   rbp
   0x00000000004014a1:  mov    rbp,rsp
   0x00000000004014a4:  sub    rsp,0x10
   0x00000000004014a8:  mov    DWORD PTR [rbp-0x4],edi
   0x00000000004014ab:  movzx  eax,BYTE PTR ds:0x4050c4
   0x00000000004014b3:  mov    ecx,0x1
End of assembler dump.
(gdb) p $edi
$1 = 5
(gdb) set $edi=3
(gdb) c
Continuing.
Enter "iD4Ur5ALF"
find_thE_hAttEr
[Inferior 1 (process 2969) exited normally]
```

We got the missing line: 

```
Enter "iD4Ur5ALF"
```

But now what?

Since we don't have any obvious move, we should continue to reverse. We can try to understand how the hints are printed by reversing `print_hint`, `get_ptr_for_hint_num` and `decode_hint` which we saw get called earlier. However, we skipped something pretty odd earlier and we should probably go back to it first. Did you catch it? Take a look at `handle_where_hatter` again:

```c
void handle_where_hatter(void)
{
  char local_238 [128];
  char *local_1b8;
  char *local_1b0;
  char *local_1a8;
  char *local_1a0;
  char *local_198;
  char *local_190;
  char local_188 [64];
  char local_148 [64];
  char local_108 [64];
  char local_c8 [64];
  char local_88 [64];
  char local_48 [53];
  undefined local_13 [11];
  
  local_190 = local_188;
  local_198 = local_148;
  local_1a0 = local_108;
  local_1a8 = local_c8;
  local_1b0 = local_88;
  local_1b8 = local_48;
  memcpy(local_1b8,&DAT_00403210,40);
  memcpy(local_1b0,&DAT_00403240,56);
  memcpy(local_1a8,&DAT_00403280,64);
  memcpy(local_1a0,&DAT_004032c0,64);
  memcpy(local_198,&DAT_00403300,64);
  memcpy(local_190,&DAT_00403340,64);
  memcpy(local_238,&DAT_00403380,122);
  memset(local_13,0xb,0xb);
  print_hint_in_debug(2);
  handle_no_args(local_238);
  return;
}
```

Look at the amount of code that gets executed. Most of it isn't needed at all! we could rewrite the function as follows without anyone noticing:

```c
void handle_where_hatter(void)
{
  char local_238 [128];
  memcpy(local_238,&DAT_00403380,122);
  print_hint_in_debug(2);
  handle_no_args(local_238);
  return;
}
```

So what are the extra buffers for? The look similar to `local_238` but only `local_238` gets used. What if we call `handle_no_args` with the other buffers?

Earlier, we executed `print_hint_in_debug(3)` by setting a breakpoint at `print_hint_in_debug` and changing the argument from `5` to `3`. This time, we'll do something a bit more elegant in order to call `handle_no_args` with the different buffers: We'll use a `gdb` script.

For reference, here's the disassembly for the function:

```assembly
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined __stdcall handle_where_hatter(void)
             undefined         AL:1           <RETURN>
             undefined1        Stack[-0x13]:1 local_13                                XREF[1]:     0040176b(*)  
             undefined1        Stack[-0x48]:1 local_48                                XREF[1]:     0040178f(*)  
             undefined1        Stack[-0x88]:1 local_88                                XREF[1]:     0040178b(*)  
             undefined1        Stack[-0xc8]:1 local_c8                                XREF[1]:     00401784(*)  
             undefined1        Stack[-0x108   local_108                               XREF[1]:     0040177d(*)  
             undefined1        Stack[-0x148   local_148                               XREF[1]:     00401776(*)  
             undefined1        Stack[-0x188   local_188                               XREF[1]:     0040176f(*)  
             char *            Stack[-0x190   local_190                               XREF[1]:     004018d8(W)  
             char *            Stack[-0x198   local_198                               XREF[1]:     004018ca(W)  
             char *            Stack[-0x1a0   local_1a0                               XREF[1]:     004018bc(W)  
             char *            Stack[-0x1a8   local_1a8                               XREF[1]:     004018ae(W)  
             char *            Stack[-0x1b0   local_1b0                               XREF[1]:     004018a0(W)  
             char *            Stack[-0x1b8   local_1b8                               XREF[1]:     00401892(W)  
             undefined1        Stack[-0x238   local_238                               XREF[2]:     004018df(*), 
                                                                                                   0040191d(*)  
             undefined8        Stack[-0x240   local_240                               XREF[2]:     00401796(W), 
                                                                                                   004018fd(R)  
             undefined8        Stack[-0x248   local_248                               XREF[3]:     004017aa(W), 
                                                                                                   00401803(R), 
                                                                                                   004018a7(R)  
             undefined8        Stack[-0x250   local_250                               XREF[3]:     004017ba(W), 
                                                                                                   0040182b(R), 
                                                                                                   004018b5(R)  
             undefined8        Stack[-0x258   local_258                               XREF[3]:     004017c4(W), 
                                                                                                   0040186b(R), 
                                                                                                   004018d1(R)  
             undefined8        Stack[-0x260   local_260                               XREF[3]:     004017cb(W), 
                                                                                                   0040184b(R), 
                                                                                                   004018c3(R)  
             undefined8        Stack[-0x268   local_268                               XREF[3]:     004017d2(W), 
                                                                                                   004017e5(R), 
                                                                                                   00401899(R)  
             undefined8        Stack[-0x270   local_270                               XREF[2]:     004017d9(W), 
                                                                                                   0040188b(R)  
             undefined8        Stack[-0x278   local_278                               XREF[4]:     0040181f(W), 
                                                                                                   0040183f(R), 
                                                                                                   0040185f(R), 
                                                                                                   0040187f(R)  
                             handle_where_hatter                             XREF[3]:     main:004011fa(c), 00403470, 
                                                                                          00403640(*)  
        00401760 55              PUSH       RBP
        00401761 48 89 e5        MOV        RBP,RSP
        00401764 48 81 ec        SUB        RSP,0x270
                 70 02 00 00
        0040176b 48 8d 7d f5     LEA        RDI=>local_13,[RBP + -0xb]
        0040176f 48 8d 85        LEA        RAX=>local_188,[RBP + -0x180]
                 80 fe ff ff
        00401776 48 8d 8d        LEA        RCX=>local_148,[RBP + -0x140]
                 c0 fe ff ff
        0040177d 48 8d 95        LEA        RDX=>local_108,[RBP + -0x100]
                 00 ff ff ff
        00401784 48 8d b5        LEA        RSI=>local_c8,[RBP + -0xc0]
                 40 ff ff ff
        0040178b 4c 8d 45 80     LEA        R8=>local_88,[RBP + -0x80]
        0040178f 4c 8d 4d c0     LEA        R9=>local_48,[RBP + -0x40]
        00401793 4d 89 ca        MOV        R10,R9
        00401796 48 89 bd        MOV        qword ptr [RBP + local_240],RDI
                 c8 fd ff ff
        0040179d 4c 89 d7        MOV        RDI,R10
        004017a0 49 ba 10        MOV        R10,DAT_00403210                                 = 04h
                 32 40 00 
                 00 00 00 00
        004017aa 48 89 b5        MOV        qword ptr [RBP + local_248],RSI
                 c0 fd ff ff
        004017b1 4c 89 d6        MOV        RSI=>DAT_00403210,R10                            = 04h
        004017b4 41 ba 28        MOV        R10D,40
                 00 00 00
        004017ba 48 89 95        MOV        qword ptr [RBP + local_250],RDX
                 b8 fd ff ff
        004017c1 4c 89 d2        MOV        RDX,R10
        004017c4 48 89 85        MOV        qword ptr [RBP + local_258],RAX
                 b0 fd ff ff
        004017cb 48 89 8d        MOV        qword ptr [RBP + local_260],RCX
                 a8 fd ff ff
        004017d2 4c 89 85        MOV        qword ptr [RBP + local_268],R8
                 a0 fd ff ff
        004017d9 4c 89 8d        MOV        qword ptr [RBP + local_270],R9
                 98 fd ff ff
        004017e0 e8 9b f8        CALL       memcpy                                           void * memcpy(void * __dest, voi
                 ff ff
        004017e5 48 8b 85        MOV        RAX,qword ptr [RBP + local_268]
                 a0 fd ff ff
        004017ec 48 89 c7        MOV        RDI,RAX
        004017ef 48 be 40        MOV        RSI=>DAT_00403240,DAT_00403240                   = 04h
                 32 40 00 
                 00 00 00 00
        004017f9 ba 38 00        MOV        EDX,56
                 00 00
        004017fe e8 7d f8        CALL       memcpy                                           void * memcpy(void * __dest, voi
                 ff ff
        00401803 48 8b 85        MOV        RAX,qword ptr [RBP + local_248]
                 c0 fd ff ff
        0040180a 48 89 c7        MOV        RDI,RAX
        0040180d 48 be 80        MOV        RSI=>DAT_00403280,DAT_00403280                   = 04h
                 32 40 00 
                 00 00 00 00
        00401817 b8 40 00        MOV        EAX,64
                 00 00
        0040181c 48 89 c2        MOV        RDX,RAX
        0040181f 48 89 85        MOV        qword ptr [RBP + local_278],RAX
                 90 fd ff ff
        00401826 e8 55 f8        CALL       memcpy                                           void * memcpy(void * __dest, voi
                 ff ff
        0040182b 48 8b 85        MOV        RAX,qword ptr [RBP + local_250]
                 b8 fd ff ff
        00401832 48 89 c7        MOV        RDI,RAX
        00401835 48 be c0        MOV        RSI=>DAT_004032c0,DAT_004032c0                   = 04h
                 32 40 00 
                 00 00 00 00
        0040183f 48 8b 95        MOV        RDX,qword ptr [RBP + local_278]
                 90 fd ff ff
        00401846 e8 35 f8        CALL       memcpy                                           void * memcpy(void * __dest, voi
                 ff ff
        0040184b 48 8b 85        MOV        RAX,qword ptr [RBP + local_260]
                 a8 fd ff ff
        00401852 48 89 c7        MOV        RDI,RAX
        00401855 48 be 00        MOV        RSI=>DAT_00403300,DAT_00403300                   = 04h
                 33 40 00 
                 00 00 00 00
        0040185f 48 8b 95        MOV        RDX,qword ptr [RBP + local_278]
                 90 fd ff ff
        00401866 e8 15 f8        CALL       memcpy                                           void * memcpy(void * __dest, voi
                 ff ff
        0040186b 48 8b 85        MOV        RAX,qword ptr [RBP + local_258]
                 b0 fd ff ff
        00401872 48 89 c7        MOV        RDI,RAX
        00401875 48 be 40        MOV        RSI=>DAT_00403340,DAT_00403340                   = 04h
                 33 40 00 
                 00 00 00 00
        0040187f 48 8b 95        MOV        RDX,qword ptr [RBP + local_278]
                 90 fd ff ff
        00401886 e8 f5 f7        CALL       memcpy                                           void * memcpy(void * __dest, voi
                 ff ff
        0040188b 48 8b 85        MOV        RAX,qword ptr [RBP + local_270]
                 98 fd ff ff
        00401892 48 89 85        MOV        qword ptr [RBP + local_1b8],RAX
                 50 fe ff ff
        00401899 48 8b 8d        MOV        RCX,qword ptr [RBP + local_268]
                 a0 fd ff ff
        004018a0 48 89 8d        MOV        qword ptr [RBP + local_1b0],RCX
                 58 fe ff ff
        004018a7 48 8b 95        MOV        RDX,qword ptr [RBP + local_248]
                 c0 fd ff ff
        004018ae 48 89 95        MOV        qword ptr [RBP + local_1a8],RDX
                 60 fe ff ff
        004018b5 48 8b b5        MOV        RSI,qword ptr [RBP + local_250]
                 b8 fd ff ff
        004018bc 48 89 b5        MOV        qword ptr [RBP + local_1a0],RSI
                 68 fe ff ff
        004018c3 48 8b bd        MOV        RDI,qword ptr [RBP + local_260]
                 a8 fd ff ff
        004018ca 48 89 bd        MOV        qword ptr [RBP + local_198],RDI
                 70 fe ff ff
        004018d1 4c 8b 85        MOV        R8,qword ptr [RBP + local_258]
                 b0 fd ff ff
        004018d8 4c 89 85        MOV        qword ptr [RBP + local_190],R8
                 78 fe ff ff
        004018df 4c 8d 8d        LEA        R9=>local_238,[RBP + -0x230]
                 d0 fd ff ff
        004018e6 4c 89 cf        MOV        RDI,R9
        004018e9 48 be 80        MOV        RSI=>DAT_00403380,DAT_00403380                   = 01h
                 33 40 00 
                 00 00 00 00
        004018f3 ba 7a 00        MOV        EDX,122
                 00 00
        004018f8 e8 83 f7        CALL       memcpy                                           void * memcpy(void * __dest, voi
                 ff ff
        004018fd 48 8b bd        MOV        RDI,qword ptr [RBP + local_240]
                 c8 fd ff ff
        00401904 be 0b 00        MOV        ESI,0xb
                 00 00
        00401909 ba 0b 00        MOV        EDX,0xb
                 00 00
        0040190e e8 5d f7        CALL       memset                                           void * memset(void * __s, int __
                 ff ff
        00401913 bf 02 00        MOV        EDI,0x2
                 00 00
        00401918 e8 83 fb        CALL       print_hint_in_debug                              undefined print_hint_in_debug(ui
                 ff ff
        0040191d 48 8d bd        LEA        RDI=>local_238,[RBP + -0x230]
                 d0 fd ff ff
        00401924 e8 37 f9        CALL       handle_no_args                                   void handle_no_args(char * param
                 ff ff
        00401929 48 81 c4        ADD        RSP,0x270
                 70 02 00 00
        00401930 5d              POP        RBP
        00401931 c3              RET

```

We'll set a breakpoint after the locals get initialized and before the internal functions get called, and use the debugger to call `handle_no_args` with each of the other buffers when the breakpoint is hit. Here's the script to do that:

```
set environment WHERE_IS_THE_HATTER=1
b *0x0040191d
commands
silent
call ((void (*) (char*)) 0x401260) ($rbp-0x40)
call ((void (*) (char*)) 0x401260) ($rbp-0x80)
call ((void (*) (char*)) 0x401260) ($rbp-0xc0)
call ((void (*) (char*)) 0x401260) ($rbp-0x100)
call ((void (*) (char*)) 0x401260) ($rbp-0x140)
call ((void (*) (char*)) 0x401260) ($rbp-0x180)
q
end
r
```

The output:

```console
root@kali:/media/sf_CTFs/matrix/Hatter# gdb -n -q -x ./commands.gdb ./Hatter
Reading symbols from ./Hatter...
(No debugging symbols found in ./Hatter)
Breakpoint 1 at 0x40191d
.08....9... 00 ^__^
....5...C.. 01 (xx)\_______
...4....... 02 (__)\       )\/\
..6....2... 03  U  ||--WWW |
.7...B...1. 04     ||     ||
.A.....3... 05
A debugging session is active.

        Inferior 1 [process 2601] will be killed.

Quit anyway? (y or n) [answered Y; input not from terminal]
```

We got six weird lines printed, one for each buffer we called. We also got the dead cow from [cowsay](https://en.wikipedia.org/wiki/Cowsay). So what now? It took us a while, but eventually we figured out how to read this, after noticing that the left diagram contains all the hex digits from `0x0` to `0xC`.

First, we align the different "Enter" lines that we saw earlier with the secret output that we've extracted:

```
dMp|dAtA
.08....9... 00 ^__^

do v3rifY
....5...C.. 01 (xx)\_______

hinT
...4....... 02 (__)\       )\/\

iD4Ur5ALF
..6....2... 03  U  ||--WWW |

P_tr01l
.7...B...1. 04     ||     ||

pRnTE
.A.....3... 05
```

Line `#n` from the original output (numbered based on `print_hint_in_debug(n)`) gets paired with the matching secret-output line based on its line index.

Then, we traverse the sequence from `0x0` to `0xC` while taking note of the matching letter. We get:

```
M?L?T34_pAR1Y
```

We will have to guess the two remaining characters. The first one must be `'C'` since that's usually the flag format. The second one is probably `'_'` to separate the flag format from the word "tea". Eventually we get `"MCL_T34_pAR1Y"` which was accepted as the correct flag.