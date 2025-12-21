# WASM

Category: Reversing

## Description

> Our keygen does not seem to work properly. Figure out what algorythm is performed by WASM app and fix it.

A website was attached.

## Solution

The website contained the following source code:

```html
<head>
<script type="text/javascript">
        async function go() {
            const src = 0;
            const dst = 0x100;

            const memory = new WebAssembly.Memory({initial: 1, maximum: 1});
            const wasm = await WebAssembly.instantiateStreaming(fetch("wasm.wasm"), {js: {mem: memory}});
            
            const input = new Uint8Array(await fetch("input.txt").then(e => e.arrayBuffer()));
            new Uint8Array(memory.buffer, src, input.byteLength).set(input);
            new Uint8Array(memory.buffer, src + input.byteLength, 1).set([0]);
            wasm.instance.exports.encode(src, dst);
            
            const view = new DataView(memory.buffer);
            var i;
            for (i = dst; view.getUint8(i) !== 0; i++) {}

            const flag = await fetch("/flag.txt", {method: "POST", headers: {"Content-Type": "application/octet-stream"}, body: memory.buffer.slice(dst, i)}).then(e => e.text());

            document.getElementById("output").value = flag;
        }
    </script>
</head>

<body>
    <div id="content">
        <div id="desc">Our keygen does not seem to work properly. Maybe you could fix it?</div>
        <button type="button" onclick="go()">Go!</button>
        <input type="text" id="output" placeholder="nil" readonly>
    </div>
</body>
```

Using the browser's developer tools, we can access the web assembly code as well:

```wasm
(module
  (memory $memory0 (import "js" "mem") 1)
  (export "encode" (func $func1))
  (func $func0 (param $var0 i32) (param $var1 i32) (result i32)
    (local $var2 i32) (local $var3 i32)
    local.get $var0
    i32.const 128
    i32.lt_u
    if
      local.get $var1
      local.get $var0
      i32.store8
      i32.const 1
      return
    end
    local.get $var0
    i32.const 63
    i32.and
    i32.const 128
    i32.add
    local.set $var2
    local.get $var0
    i32.const 2048
    i32.lt_u
    if
      local.get $var1
      i32.const 1
      i32.add
      local.get $var2
      i32.store8
      local.get $var1
      local.get $var0
      i32.const 6
      i32.shr_u
      i32.const 192
      i32.add
      i32.store8
      i32.const 2
      return
    end
    local.get $var0
    i32.const 6
    i32.shr_u
    i32.const 63
    i32.and
    i32.const 128
    i32.add
    local.set $var3
    local.get $var0
    i32.const 65536
    i32.lt_u
    if
      local.get $var1
      i32.const 2
      i32.add
      local.get $var2
      i32.store8
      local.get $var1
      i32.const 1
      i32.add
      local.get $var3
      i32.store8
      local.get $var1
      local.get $var0
      i32.const 12
      i32.shr_u
      i32.const 224
      i32.add
      i32.store8
      i32.const 3
      return
    end
    i32.const 65533
    local.get $var1
    call $func0
  )
  (func $func1 (param $var0 i32) (param $var1 i32)
    (local $var2 i32) (local $var3 i32) (local $var4 i32)
    i32.const 0
    local.set $var2
    i32.const 0
    local.set $var3
    loop $label1
      block $label0
        local.get $var0
        local.get $var2
        i32.add
        i32.load8_u
        local.tee $var4
        i32.eqz
        br_if $label0
        local.get $var4
        i32.const 128192
        i32.add
        local.get $var1
        local.get $var3
        i32.add
        call $func0
        local.get $var3
        i32.add
        local.set $var3
        local.get $var2
        i32.const 1
        i32.add
        local.set $var2
        br $label1
      end $label0
    end $label1
    local.get $var1
    local.get $var3
    i32.add
    i32.const 0
    i32.store8
  )
)
```

It's possible to decompile this into pseudo-code using [WABT](https://github.com/WebAssembly/wabt) to get a better idea of what it does:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/wasm]
└─$ ~/utils/web/wabt/build/wat2wasm wasm.wat -o wasm.wasm

┌──(user@kali3)-[/media/sf_CTFs/google/wasm]
└─$ ~/utils/web/wabt/build/wasm2c wasm.wasm -o wasm.c


┌──(user@kali3)-[/media/sf_CTFs/google/wasm]
└─$ ~/utils/web/wabt/build/wasm-decompile wasm.wasm -o wasm.dcmp
```

This gives us:

```
import memory js_mem;

function f_a(a:int, b:byte_ptr):int {
  if (a < 128) {
    b[0] = a;
    return 1;
  }
  var c:int = (a & 63) + 128;
  if (a < 2048) {
    (b + 1)[0]:byte = c;
    b[0] = (a >> 6) + 192;
    return 2;
  }
  var d:int = (a >> 6 & 63) + 128;
  if (a < 65536) {
    (b + 2)[0]:byte = c;
    (b + 1)[0]:byte = d;
    b[0] = (a >> 12) + 224;
    return 3;
  }
  return f_a(65533, b);
}

export function encode(a:int, b:int) {
  var c:int = 0;
  var d:int = 0;
  loop L_a {
    var e:int = (a + c)[0]:ubyte;
    if (eqz(e)) goto B_b;
    d = f_a(e + 128192, b + d) + d;
    c = c + 1;
    continue L_a;
    label B_b:
  }
  (b + d)[0]:byte = 0;
}
```

If you don't immediately recognize what the code does, feel free to throw it at ChatGPT to get
an idea:

> This code is a WebAssembly (Wasm) module that seems to implement a UTF-8 encoding function

While this is not entirely accurate, it gives us a very good direction. So it looks like 
the website code is supposed to read some input from `input.txt`, encode it in a similar manner
to UTF-8 encoding and send it to `/flag.txt` as the key to get the flag. What's the content of `input.txt`?

```console
┌──(user@kali3)-[/media/sf_CTFs/google/wasm]
└─$ curl https://wasm-reverse-web.2024-bq.ctfcompetition.com/input.txt -s | xxd -g 1
00000000: 52 b5 70 4e e5 b0 53 b6                          R.pN..S.
```

To see what it got translated to by our Web Assembly code, we can put a breakpoint right 
before the output is sent back to the server and dump its contents:

```javascript
>>> function buf2hex(buffer) {
  return [...new Uint8Array(buffer)]
      .map(x => x.toString(16).padStart(2, '0'))
      .join(':');
}

>>> buf2hex(memory.buffer.slice(dst, i))
"ef:bf:bd:ef:bf:bd:ef:bf:bd:ef:bf:bd:ef:bf:bd:ef:bf:bd:ef:bf:bd:ef:bf:bd" 
```

That doesn't look very good. Given that all the characters got translated to the same
value, we're probably hitting the `f_a(65533, b)` part of the encoding function for each
character. And, upon closer inspection, this makes sense given the fact that the caller
adds `128192` to the value of each byte. Looks like we'll need the logic for
values over `65533` to get a meaningful result. While we can try to extend the function ourselves,
an easier approach would be to utilize some existing infrastructure that knows to translate
UTF-8 code points to strings, and just feed it the input after adding `128192` to each value.

```python

res = ""
with open("input.txt", 'rb') as f:
    for b in f.read():
        res += chr(b + 128192)

print(res)
```

Output:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/wasm]
└─$ python3 solve.py
🔒🕵🔰🔎🖥🕰🔓🕶
```

Send it as the key to get the flag:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/wasm]
└─$ curl -X POST https://wasm-reverse-web.2024-bq.ctfcompetition.com/flag.txt -H "Content-Type: application/x-www-form-urlencoded" -d $(python3 solve.py)
FLAG{we_have_assembly_in_the_web}
```

