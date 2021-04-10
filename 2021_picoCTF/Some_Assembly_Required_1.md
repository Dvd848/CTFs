# Some Assembly Required 1
Category: Web, 70 points

## Solution

We visit the attached website and see the following:

```html
<html>
<head>
	<meta charset="UTF-8">
	<script src="G82XCw5CX3.js"></script>
</head>
<body>
	<h4>Enter flag:</h4>
	<input type="text" id="input"/>
	<button onclick="onButtonPress()">Submit</button>
	<p id="result"></p>
</body>
</html>
```

The Javascript file contains:
```javascript
const _0x402c=['value','2wfTpTR','instantiate','275341bEPcme','innerHTML','1195047NznhZg','1qfevql','input','1699808QuoWhA','Correct!','check_flag','Incorrect!','./JIFxzHyW8W','23SMpAuA','802698XOMSrr','charCodeAt','474547vVoGDO','getElementById','instance','copy_char','43591XxcWUl','504454llVtzW','arrayBuffer','2NIQmVj','result'];const _0x4e0e=function(_0x553839,_0x53c021){_0x553839=_0x553839-0x1d6;let _0x402c6f=_0x402c[_0x553839];return _0x402c6f;};(function(_0x76dd13,_0x3dfcae){const _0x371ac6=_0x4e0e;while(!![]){try{const _0x478583=-parseInt(_0x371ac6(0x1eb))+parseInt(_0x371ac6(0x1ed))+-parseInt(_0x371ac6(0x1db))*-parseInt(_0x371ac6(0x1d9))+-parseInt(_0x371ac6(0x1e2))*-parseInt(_0x371ac6(0x1e3))+-parseInt(_0x371ac6(0x1de))*parseInt(_0x371ac6(0x1e0))+parseInt(_0x371ac6(0x1d8))*parseInt(_0x371ac6(0x1ea))+-parseInt(_0x371ac6(0x1e5));if(_0x478583===_0x3dfcae)break;else _0x76dd13['push'](_0x76dd13['shift']());}catch(_0x41d31a){_0x76dd13['push'](_0x76dd13['shift']());}}}(_0x402c,0x994c3));let exports;(async()=>{const _0x48c3be=_0x4e0e;let _0x5f0229=await fetch(_0x48c3be(0x1e9)),_0x1d99e9=await WebAssembly[_0x48c3be(0x1df)](await _0x5f0229[_0x48c3be(0x1da)]()),_0x1f8628=_0x1d99e9[_0x48c3be(0x1d6)];exports=_0x1f8628['exports'];})();function onButtonPress(){const _0xa80748=_0x4e0e;let _0x3761f8=document['getElementById'](_0xa80748(0x1e4))[_0xa80748(0x1dd)];for(let _0x16c626=0x0;_0x16c626<_0x3761f8['length'];_0x16c626++){exports[_0xa80748(0x1d7)](_0x3761f8[_0xa80748(0x1ec)](_0x16c626),_0x16c626);}exports['copy_char'](0x0,_0x3761f8['length']),exports[_0xa80748(0x1e7)]()==0x1?document[_0xa80748(0x1ee)](_0xa80748(0x1dc))[_0xa80748(0x1e1)]=_0xa80748(0x1e6):document[_0xa80748(0x1ee)](_0xa80748(0x1dc))[_0xa80748(0x1e1)]=_0xa80748(0x1e8);}
```

Using an [online deobfuscator](http://jsnice.org/), we get:

```javascript
'use strict';
const _0x402c = ["value", "2wfTpTR", "instantiate", "275341bEPcme", "innerHTML", "1195047NznhZg", "1qfevql", "input", "1699808QuoWhA", "Correct!", "check_flag", "Incorrect!", "./JIFxzHyW8W", "23SMpAuA", "802698XOMSrr", "charCodeAt", "474547vVoGDO", "getElementById", "instance", "copy_char", "43591XxcWUl", "504454llVtzW", "arrayBuffer", "2NIQmVj", "result"];
const _0x4e0e = function(url, whensCollection) {
  /** @type {number} */
  url = url - 470;
  let _0x402c6f = _0x402c[url];
  return _0x402c6f;
};
(function(data, oldPassword) {
  const toMonths = _0x4e0e;
  for (; !![];) {
    try {
      const userPsd = -parseInt(toMonths(491)) + parseInt(toMonths(493)) + -parseInt(toMonths(475)) * -parseInt(toMonths(473)) + -parseInt(toMonths(482)) * -parseInt(toMonths(483)) + -parseInt(toMonths(478)) * parseInt(toMonths(480)) + parseInt(toMonths(472)) * parseInt(toMonths(490)) + -parseInt(toMonths(485));
      if (userPsd === oldPassword) {
        break;
      } else {
        data["push"](data["shift"]());
      }
    } catch (_0x41d31a) {
      data["push"](data["shift"]());
    }
  }
})(_0x402c, 627907);
let exports;
(async() => {
  const findMiddlePosition = _0x4e0e;
  let leftBranch = await fetch(findMiddlePosition(489));
  let rightBranch = await WebAssembly[findMiddlePosition(479)](await leftBranch[findMiddlePosition(474)]());
  let module = rightBranch[findMiddlePosition(470)];
  exports = module["exports"];
})();
/**
 * @return {undefined}
 */
function onButtonPress() {
  const navigatePop = _0x4e0e;
  let params = document["getElementById"](navigatePop(484))[navigatePop(477)];
  for (let i = 0; i < params["length"]; i++) {
    exports[navigatePop(471)](params[navigatePop(492)](i), i);
  }
  exports["copy_char"](0, params["length"]);
  if (exports[navigatePop(487)]() == 1) {
    document[navigatePop(494)](navigatePop(476))[navigatePop(481)] = navigatePop(486);
  } else {
    document[navigatePop(494)](navigatePop(476))[navigatePop(481)] = navigatePop(488);
  }
}
;
```

We can de-obfuscate the script a bit more by using the browser developer console to evaluate the different `navigatePop` and `findMiddlePosition` function calls, e.g.:

```javascript
>>> const navigatePop = _0x4e0e;
undefined
>>> navigatePop(484)
"input"
```

We get:

```javascript
(async() => {
  const findMiddlePosition = _0x4e0e;
  let leftBranch = await fetch("./JIFxzHyW8W");
  let rightBranch = await WebAssembly["instantiate"](await leftBranch["arrayBuffer"]());
  let module = rightBranch["instance"];
  exports = module["exports"];
})();
/**
 * @return {undefined}
 */
function onButtonPress() {
  const navigatePop = _0x4e0e;
  let params = document["getElementById"]("input")["value"];
  for (let i = 0; i < params["length"]; i++) {
    exports["copy_char"](params["charCodeAt"](i), i);
  }
  exports["copy_char"](0, params["length"]);
  if (exports["check_flag"]() == 1) {
    document["getElementById"]("result")["innerHTML"] = "Correct!";
  } else {
    document["getElementById"]("result")["innerHTML"] = "Incorrect!";
  }
}
;
```

So basically, we have a Javascript script that take each character of the user's flag, submits it to a WebAssembly script via `copy_char` and then calls `check_flag` to see if the flag is correct.

Let's download the WebAssembly script:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Some_Assembly_Required_1]
└─$ wget http://mercury.picoctf.net:26318/JIFxzHyW8W -q -O script.wasm

┌──(user@kali)-[/media/sf_CTFs/pico/Some_Assembly_Required_1]
└─$ file script.wasm
script.wasm: WebAssembly (wasm) binary module version 0x1 (MVP)
```

We'll convert it to `wat` for readability:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Some_Assembly_Required_1]
└─$ ~/utils/web/wabt/build/wasm2wat --generate-names script.wasm > script.wat
```

The result:

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
    (local $l2 i32) (local $l3 i32) (local $l4 i32) (local $l5 i32) (local $l6 i32)
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
    local.get $l4
    i32.load offset=8
    local.set $l6
    local.get $l6
    local.get $l5
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
  (data $d0 (i32.const 1024) "picoCTF{8857462f9e30faae4d037e5e25fee1ce}\00\00"))
```

For starters, we can see the flag at the bottom of the file. Before that, we have three function implementations: `strcmp` which isn't interesting to reverse, `check_flag` and `copy_char`. Since this is the simplest challenge of the `Some Assembly Required` series, let's use it to get acquainted with `wasm`, even though we already have the flag.

Web Assembly can be thought of as a *stack machine*. It's basically composed of instructions that push a value to the stack and/or pop it from the stack to a different location.

For example, a `local.get` instruction pushes a value from a local variable to the stack, a `local.set` instruction pops a value from the stack and pushes it to a local, and an instruction such as `i32.sub` performs an operation on values popped from the stack and pushes the result to the stack. However, following the execution of such a syntax can be somewhat exhausting. We can try to compile the code to C, but some might say the result in this case is even worse:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Some_Assembly_Required_1]
└─$ ~/utils/web/wabt/build/wasm2c  script.wasm -o script.c
```

An example function:

```c
static void w2c_copy_char(u32 w2c_p0, u32 w2c_p1) {
  u32 w2c_l2 = 0, w2c_l3 = 0, w2c_l4 = 0, w2c_l5 = 0, w2c_l6 = 0;
  FUNC_PROLOGUE;
  u32 w2c_i0, w2c_i1;
  w2c_i0 = w2c_g0;
  w2c_l2 = w2c_i0;
  w2c_i0 = 16u;
  w2c_l3 = w2c_i0;
  w2c_i0 = w2c_l2;
  w2c_i1 = w2c_l3;
  w2c_i0 -= w2c_i1;
  w2c_l4 = w2c_i0;
  w2c_i0 = w2c_l4;
  w2c_i1 = w2c_p0;
  i32_store((&w2c_memory), (u64)(w2c_i0) + 12, w2c_i1);
  w2c_i0 = w2c_l4;
  w2c_i1 = w2c_p1;
  i32_store((&w2c_memory), (u64)(w2c_i0) + 8, w2c_i1);
  w2c_i0 = w2c_l4;
  w2c_i0 = i32_load((&w2c_memory), (u64)(w2c_i0) + 12u);
  w2c_l5 = w2c_i0;
  w2c_i0 = w2c_l4;
  w2c_i0 = i32_load((&w2c_memory), (u64)(w2c_i0) + 8u);
  w2c_l6 = w2c_i0;
  w2c_i0 = w2c_l6;
  w2c_i1 = w2c_l5;
  i32_store8((&w2c_memory), (u64)(w2c_i0) + 1072, w2c_i1);
  goto w2c_Bfunc;
  w2c_Bfunc:;
  FUNC_EPILOGUE;
}
```

The best option seems to be de-compiling to pseudo-code:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Some_Assembly_Required_1]
└─$ ~/utils/web/wabt/build/wasm-decompile script.wasm -o script.dcmp
```

The result:

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

data d_picoCTF8857462f9e30faae4d037(offset: 1024) = 
"picoCTF{8857462f9e30faae4d037e5e25fee1ce}\00\00";

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
  var g:byte_ptr = e[2];
  g[1072] = f;
}
```

Now, if we try to get a high-level understanding of what's happening in `copy` and `check_flag`, we can see that `copy` takes the first parameter `a` (which according to the Javascript source is a character from the flag) and saves it at offset `1072+b` where `b` is the index of the character in the original user input. So basically it's just storing the flag, starting at offset `1072`, character after character.

`check_flag` calls `strcmp` between memory location `1072` (where we saved the user input) and memory location `1024` (where the flag is saved). It then performs some weird manipulations on the result (which can be compressed to `(((strcmp_res != 0) ^ (-1)) & 1)`). This seems to be a complicated way to say that if `strcmp` returned `0` (i.e. the strings were equal) then the return value will be `1`, and otherwise it will be `0`. So basically it just reflects if the user input is equal to the stored flag.

The flag: `picoCTF{8857462f9e30faae4d037e5e25fee1ce}`