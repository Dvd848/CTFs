# Some Assembly Required 2
Category: Web, 110 points

## Solution

This is the follow-up for [Some Assembly Required 1](Some_Assembly_Required_1.md). We'll skip the stuff that we've covered there and jump right to the WebAssembly.

Translating the `wasm` file to `wat`:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Some_Assembly_Required_2]
└─$ ~/utils/web/wabt/build/wasm2wat --generate-names script.wasm > script.wat
```

Result:

```wat
(module
  (type $t0 (func))
  (type $t1 (func (param i32 i32) (result i32)))
  (type $t2 (func (result i32)))
  (type $t3 (func (param i32 i32)))
  (func $__wasm_call_ctors (type $t0))
  (func $strcmp (type $t1) (param $p0 i32) (param $p1 i32) (result i32)
    (local $l2 i32) (local $l3 i32) (local $l4 i32) (local $l5 i32) (local $l6 i32) (local $l7 i32) (local $l8 i32) (local $l9 i32) (local $l10 i32) (local $l11 i32) (local $l12 i32) (local $l13 i32) (local $l14 i32) (local $l15 i32) (local $l16 i32) (local $l17 i32) (local $l18 i32) (local $l19 i32) (local $l20 i32) (local $l21 i32) (local $l22 i32) (local $l23 i32) (local $l24 i32) (local $l25 i32) (local $l26 i32) (local $l27 i32) (local $l28 i32) (local $l29 i32) (local $l30 i32) (local $l31 i32) (local $l32 i32) (local $l33 i32) (local $l34 i32) (local $l35 i32) (local $l36 i32) (local $l37 i32) (local $l38 i32) (local $l39 i32) (local $l40 i32) (local $l41 i32) (local $l42 i32) (local $l43 i32)
    global.get $g0
    local.set $l2
    i32.const 32
    local.set $l3
    local.get $l2
    local.get $l3
    i32.sub
    local.set $l4
    local.get $l4
    local.get $p0
    i32.store offset=24
    local.get $l4
    local.get $p1
    i32.store offset=20
    local.get $l4
    i32.load offset=24
    local.set $l5
    local.get $l4
    local.get $l5
    i32.store offset=16
    local.get $l4
    i32.load offset=20
    local.set $l6
    local.get $l4
    local.get $l6
    i32.store offset=12
    block $B0
      loop $L1
        local.get $l4
        i32.load offset=16
        local.set $l7
        i32.const 1
        local.set $l8
        local.get $l7
        local.get $l8
        i32.add
        local.set $l9
        local.get $l4
        local.get $l9
        i32.store offset=16
        local.get $l7
        i32.load8_u
        local.set $l10
        local.get $l4
        local.get $l10
        i32.store8 offset=11
        local.get $l4
        i32.load offset=12
        local.set $l11
        i32.const 1
        local.set $l12
        local.get $l11
        local.get $l12
        i32.add
        local.set $l13
        local.get $l4
        local.get $l13
        i32.store offset=12
        local.get $l11
        i32.load8_u
        local.set $l14
        local.get $l4
        local.get $l14
        i32.store8 offset=10
        local.get $l4
        i32.load8_u offset=11
        local.set $l15
        i32.const 255
        local.set $l16
        local.get $l15
        local.get $l16
        i32.and
        local.set $l17
        block $B2
          local.get $l17
          br_if $B2
          local.get $l4
          i32.load8_u offset=11
          local.set $l18
          i32.const 255
          local.set $l19
          local.get $l18
          local.get $l19
          i32.and
          local.set $l20
          local.get $l4
          i32.load8_u offset=10
          local.set $l21
          i32.const 255
          local.set $l22
          local.get $l21
          local.get $l22
          i32.and
          local.set $l23
          local.get $l20
          local.get $l23
          i32.sub
          local.set $l24
          local.get $l4
          local.get $l24
          i32.store offset=28
          br $B0
        end
        local.get $l4
        i32.load8_u offset=11
        local.set $l25
        i32.const 255
        local.set $l26
        local.get $l25
        local.get $l26
        i32.and
        local.set $l27
        local.get $l4
        i32.load8_u offset=10
        local.set $l28
        i32.const 255
        local.set $l29
        local.get $l28
        local.get $l29
        i32.and
        local.set $l30
        local.get $l27
        local.set $l31
        local.get $l30
        local.set $l32
        local.get $l31
        local.get $l32
        i32.eq
        local.set $l33
        i32.const 1
        local.set $l34
        local.get $l33
        local.get $l34
        i32.and
        local.set $l35
        local.get $l35
        br_if $L1
      end
      local.get $l4
      i32.load8_u offset=11
      local.set $l36
      i32.const 255
      local.set $l37
      local.get $l36
      local.get $l37
      i32.and
      local.set $l38
      local.get $l4
      i32.load8_u offset=10
      local.set $l39
      i32.const 255
      local.set $l40
      local.get $l39
      local.get $l40
      i32.and
      local.set $l41
      local.get $l38
      local.get $l41
      i32.sub
      local.set $l42
      local.get $l4
      local.get $l42
      i32.store offset=28
    end
    local.get $l4
    i32.load offset=28
    local.set $l43
    local.get $l43
    return)
  (func $check_flag (type $t2) (result i32)
    (local $l0 i32) (local $l1 i32) (local $l2 i32) (local $l3 i32) (local $l4 i32) (local $l5 i32) (local $l6 i32) (local $l7 i32) (local $l8 i32) (local $l9 i32) (local $l10 i32)
    i32.const 0
    local.set $l0
    i32.const 1072
    local.set $l1
    i32.const 1024
    local.set $l2
    local.get $l2
    local.get $l1
    call $strcmp
    local.set $l3
    local.get $l3
    local.set $l4
    local.get $l0
    local.set $l5
    local.get $l4
    local.get $l5
    i32.ne
    local.set $l6
    i32.const -1
    local.set $l7
    local.get $l6
    local.get $l7
    i32.xor
    local.set $l8
    i32.const 1
    local.set $l9
    local.get $l8
    local.get $l9
    i32.and
    local.set $l10
    local.get $l10
    return)
  (func $copy_char (type $t3) (param $p0 i32) (param $p1 i32)
    (local $l2 i32) (local $l3 i32) (local $l4 i32) (local $l5 i32) (local $l6 i32) (local $l7 i32) (local $l8 i32) (local $l9 i32) (local $l10 i32)
    global.get $g0
    local.set $l2
    i32.const 16
    local.set $l3
    local.get $l2
    local.get $l3
    i32.sub
    local.set $l4
    local.get $l4
    local.get $p0
    i32.store offset=12
    local.get $l4
    local.get $p1
    i32.store offset=8
    local.get $l4
    i32.load offset=12
    local.set $l5
    block $B0
      local.get $l5
      i32.eqz
      br_if $B0
      local.get $l4
      i32.load offset=12
      local.set $l6
      i32.const 8
      local.set $l7
      local.get $l6
      local.get $l7
      i32.xor
      local.set $l8
      local.get $l4
      local.get $l8
      i32.store offset=12
    end
    local.get $l4
    i32.load offset=12
    local.set $l9
    local.get $l4
    i32.load offset=8
    local.set $l10
    local.get $l10
    local.get $l9
    i32.store8 offset=1072
    return)
  (table $T0 1 1 funcref)
  (memory $memory 2)
  (global $g0 (mut i32) (i32.const 66864))
  (global $input i32 (i32.const 1072))
  (global $__dso_handle i32 (i32.const 1024))
  (global $__data_end i32 (i32.const 1328))
  (global $__global_base i32 (i32.const 1024))
  (global $__heap_base i32 (i32.const 66864))
  (global $__memory_base i32 (i32.const 0))
  (global $__table_base i32 (i32.const 1))
  (export "memory" (memory 0))
  (export "__wasm_call_ctors" (func $__wasm_call_ctors))
  (export "strcmp" (func $strcmp))
  (export "check_flag" (func $check_flag))
  (export "input" (global 1))
  (export "copy_char" (func $copy_char))
  (export "__dso_handle" (global 2))
  (export "__data_end" (global 3))
  (export "__global_base" (global 4))
  (export "__heap_base" (global 5))
  (export "__memory_base" (global 6))
  (export "__table_base" (global 7))
  (data $d0 (i32.const 1024) "xakgK\5cNs>n;jl90;9:mjn9m<0n9::0::881<00?>u\00\00"))

```

Now to pseudo-code:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Some_Assembly_Required_2]
└─$ ~/utils/web/wabt/build/wasm-decompile script.wasm -o script.dcmp
```

Result:

```
export memory memory(initial: 2, max: 0);

global g_a:int = 66864;
export global input:int = 1072;
export global dso_handle:int = 1024;
export global data_end:int = 1328;
export global global_base:int = 1024;
export global heap_base:int = 66864;
export global memory_base:int = 0;
export global table_base:int = 1;

table T_a:funcref(min: 1, max: 1);

data d_xakgKNsnjl909mjn9m0n9088100u(offset: 1024) = 
"xakgK\Ns>n;jl90;9:mjn9m<0n9::0::881<00?>u\00\00";

export function wasm_call_ctors() {
}

export function strcmp(a:int, b:int):int {
  var c:int = g_a;
  var d:int = 32;
  var e:int = c - d;
  e[6]:int = a;
  e[5]:int = b;
  var f:int = e[6]:int;
  e[4]:int = f;
  var g:int = e[5]:int;
  e[3]:int = g;
  loop L_b {
    var h:ubyte_ptr = e[4]:int;
    var i:int = 1;
    var j:int = h + i;
    e[4]:int = j;
    var k:int = h[0];
    e[11]:byte = k;
    var l:ubyte_ptr = e[3]:int;
    var m:int = 1;
    var n:int = l + m;
    e[3]:int = n;
    var o:int = l[0];
    e[10]:byte = o;
    var p:int = e[11]:ubyte;
    var q:int = 255;
    var r:int = p & q;
    if (r) goto B_c;
    var s:int = e[11]:ubyte;
    var t:int = 255;
    var u:int = s & t;
    var v:int = e[10]:ubyte;
    var w:int = 255;
    var x:int = v & w;
    var y:int = u - x;
    e[7]:int = y;
    goto B_a;
    label B_c:
    var z:int = e[11]:ubyte;
    var aa:int = 255;
    var ba:int = z & aa;
    var ca:int = e[10]:ubyte;
    var da:int = 255;
    var ea:int = ca & da;
    var fa:int = ba;
    var ga:int = ea;
    var ha:int = fa == ga;
    var ia:int = 1;
    var ja:int = ha & ia;
    if (ja) continue L_b;
  }
  var ka:int = e[11]:ubyte;
  var la:int = 255;
  var ma:int = ka & la;
  var na:int = e[10]:ubyte;
  var oa:int = 255;
  var pa:int = na & oa;
  var qa:int = ma - pa;
  e[7]:int = qa;
  label B_a:
  var ra:int = e[7]:int;
  return ra;
}

export function check_flag():int {
  var a:int = 0;
  var b:int = 1072;
  var c:int = 1024;
  var d:int = strcmp(c, b);
  var e:int = d;
  var f:int = a;
  var g:int = e != f;
  var h:int = -1;
  var i:int = g ^ h;
  var j:int = 1;
  var k:int = i & j;
  return k;
}

function copy(a:int, b:int) {
  var c:int = g_a;
  var d:int = 16;
  var e:int_ptr = c - d;
  e[3] = a;
  e[2] = b;
  var f:int = e[3];
  if (eqz(f)) goto B_a;
  var g:int = e[3];
  var h:int = 8;
  var i:int = g ^ h;
  e[3] = i;
  label B_a:
  var j:int = e[3];
  var k:byte_ptr = e[2];
  k[1072] = j;
}
```

We can see that `check_flag` stayed pretty much the same as last time, so we can assume that the magic happens in `copy`. If we diff it against the previous implementation (which simply stored the provided flag characters in memory without any extra manipulation) we can see that the new version includes some extra logic:

```
  if (eqz(f)) goto B_a;
  var g:int = e[3];
  var h:int = 8;
  var i:int = g ^ h;
  e[3] = i;
  label B_a:
  var j:int = e[3];
```

What seems to be happening here is that characters from the flag are getting XORed with 8 before being saved at offset `1072+`. We can also see that the string that `check_flag` uses to compare the user input to the expected flag (at offset `1024+`) doesn't contain the flag in the clear anymore:

```
data d_xakgKNsnjl909mjn9m0n9088100u(offset: 1024) = 
"xakgK\Ns>n;jl90;9:mjn9m<0n9::0::881<00?>u\00\00";
```

We'll take this string and XOR it with `8` to get the flag:

```python
>>> from pwn import *
>>> xor("xakgK\\Ns>n;jl90;9:mjn9m<0n9::0::881<00?>u\00\00", 8)
b'picoCTF{6f3bd18312ebf1e48f12282200948876}\x08\x08'
```

The flag: `picoCTF{6f3bd18312ebf1e48f12282200948876}`
