# Client-side-again
Web Exploitation, 200 points

## Description:
> Can you break into this super secure portal?


## Solution: 

We visit the website and inspect the source code:
```html
<html>
<head>
<title>Secure Login Portal V2.0</title>
</head>
<body background="barbed_wire.jpeg" >
<!-- standard MD5 implementation -->
<script type="text/javascript" src="md5.js"></script>

<script type="text/javascript">
  var _0x5a46=['55670}','_again_0','this','Password\x20Verified','Incorrect\x20password','getElementById','value','substring','picoCTF{','not_this'];(function(_0x4bd822,_0x2bd6f7){var _0xb4bdb3=function(_0x1d68f6){while(--_0x1d68f6){_0x4bd822['push'](_0x4bd822['shift']());}};_0xb4bdb3(++_0x2bd6f7);}(_0x5a46,0x1b3));var _0x4b5b=function(_0x2d8f05,_0x4b81bb){_0x2d8f05=_0x2d8f05-0x0;var _0x4d74cb=_0x5a46[_0x2d8f05];return _0x4d74cb;};function verify(){checkpass=document[_0x4b5b('0x0')]('pass')[_0x4b5b('0x1')];split=0x4;if(checkpass[_0x4b5b('0x2')](0x0,split*0x2)==_0x4b5b('0x3')){if(checkpass[_0x4b5b('0x2')](0x7,0x9)=='{n'){if(checkpass[_0x4b5b('0x2')](split*0x2,split*0x2*0x2)==_0x4b5b('0x4')){if(checkpass[_0x4b5b('0x2')](0x3,0x6)=='oCT'){if(checkpass[_0x4b5b('0x2')](split*0x3*0x2,split*0x4*0x2)==_0x4b5b('0x5')){if(checkpass['substring'](0x6,0xb)=='F{not'){if(checkpass[_0x4b5b('0x2')](split*0x2*0x2,split*0x3*0x2)==_0x4b5b('0x6')){if(checkpass[_0x4b5b('0x2')](0xc,0x10)==_0x4b5b('0x7')){alert(_0x4b5b('0x8'));}}}}}}}}else{alert(_0x4b5b('0x9'));}}
</script>
<div style="position:relative; padding:5px;top:50px; left:38%; width:350px; height:140px; background-color:gray">
<div style="text-align:center">
<p>New and Improved Login</p>

<p>Enter valid credentials to proceed</p>
<form action="index.html" method="post">
<input type="password" id="pass" size="8" />
<br/>
<input type="submit" value="verify" onclick="verify(); return false;" />
</form>
</div>
</div>
</body>
</html>
```

Let's call a [Javascript Beautifier](http://www.jsnice.org/) in order to make the Javascript code a bit more readable:

```javascript
'use strict';
/** @type {!Array} */
var _0x5a46 = ["c2047}", "_again_6", "this", "Password Verified", "Incorrect password", "getElementById", "value", "substring", "picoCTF{", "not_this"];
(function(data, i) {
  /**
   * @param {number} isLE
   * @return {undefined}
   */
  var write = function(isLE) {
    for (; --isLE;) {
      data["push"](data["shift"]());
    }
  };
  write(++i);
})(_0x5a46, 435);
/**
 * @param {string} level
 * @param {?} ai_test
 * @return {?}
 */
var _0x4b5b = function(level, ai_test) {
  /** @type {number} */
  level = level - 0;
  var rowsOfColumns = _0x5a46[level];
  return rowsOfColumns;
};
/**
 * @return {undefined}
 */
function verify() {
  checkpass = document[_0x4b5b("0x0")]("pass")[_0x4b5b("0x1")];
  /** @type {number} */
  split = 4;
  if (checkpass[_0x4b5b("0x2")](0, split * 2) == _0x4b5b("0x3")) {
    if (checkpass[_0x4b5b("0x2")](7, 9) == "{n") {
      if (checkpass[_0x4b5b("0x2")](split * 2, split * 2 * 2) == _0x4b5b("0x4")) {
        if (checkpass[_0x4b5b("0x2")](3, 6) == "oCT") {
          if (checkpass[_0x4b5b("0x2")](split * 3 * 2, split * 4 * 2) == _0x4b5b("0x5")) {
            if (checkpass["substring"](6, 11) == "F{not") {
                if (checkpass[_0x4b5b("0x2")](12, 16) == _0x4b5b("0x7")) {
              if (checkpass[_0x4b5b("0x2")](split * 2 * 2, split * 3 * 2) == _0x4b5b("0x6")) {
                  alert(_0x4b5b("0x8"));
                }
              }
            }
          }
        }
      }
    }
  } else {
    alert(_0x4b5b("0x9"));
  }
}
;
```

We see that `_0x4b5b` is a function used to obfuscate different values. It is calculated in runtime. Luckily, we can use the browser's Javascript console ("Developer Tools") in order to evaluate `_0x4b5b` and read its values:

```
>>> _0x4b5b
function _0x4b5b()

>>> _0x4b5b("0x0")
"getElementById"
>>> _0x4b5b("0x1")
"value"
>>> _0x4b5b("0x2")
"substring"
>>> _0x4b5b("0x3")
"picoCTF{"
>>> _0x4b5b("0x4")
"not_this"
>>> _0x4b5b("0x5")
"55670}"
>>> _0x4b5b("0x6")
"_again_0"
>>> _0x4b5b("0x7")
"this"
>>> _0x4b5b("0x8")
"Password Verified"
>>> _0x4b5b("0x9")
"Incorrect password"
```

Let's replace the function calls with hardcoded values to improve readability:
```javascript
function verify() {
  checkpass = document["getElementById"]("pass")["value"];
  /** @type {number} */
  split = 4;
  if (checkpass["substring"](0, split * 2) == "picoCTF{") {
    if (checkpass["substring"](7, 9) == "{n") {
      if (checkpass["substring"](split * 2, split * 2 * 2) == "not_this") {
        if (checkpass["substring"](3, 6) == "oCT") {
          if (checkpass["substring"](split * 3 * 2, split * 4 * 2) == "55670}") {
            if (checkpass["substring"](6, 11) == "F{not") {
                if (checkpass["substring"](12, 16) == "this") {
              if (checkpass["substring"](split * 2 * 2, split * 3 * 2) == "_again_0") {
                  alert("Password Verified");
                }
              }
            }
          }
        }
      }
    }
  } else {
    alert("Incorrect password");
  }
}
```

So this is very similar to [dont-use-client-side](dont-use-client-side.md), using `substring` to authenticate the password.

We have:
```
(0-8)      == "picoCTF{"
(7-9)      == "{n"
(8-16)     == "not_this"
(3-6)      == "oCT"
(24-32)    == "c2047}"
(6-11)     == "F{not"
(16-24)    == "_again_6"
(12-16)    == "this"
```

Notice that there are some overlaps.

An evil way to turn this into a flag would be by transforming the javascript substrings into Python array-assignment code and executing it:

```python
text = """
  if (checkpass["substring"](0, split * 2) == "picoCTF{") {
    if (checkpass["substring"](7, 9) == "{n") {
      if (checkpass["substring"](split * 2, split * 2 * 2) == "not_this") {
        if (checkpass["substring"](3, 6) == "oCT") {
          if (checkpass["substring"](split * 3 * 2, split * 4 * 2) == "55670}") {
            if (checkpass["substring"](6, 11) == "F{not") {
                if (checkpass["substring"](12, 16) == "this") {
              if (checkpass["substring"](split * 2 * 2, split * 3 * 2) == "_again_0") {
"""

flag = [None] * 32
split = 4
for line in text.split("\n"):
    line = line.strip()
    if line == "": 
        continue
    line = line.replace('if (checkpass["substring"](', 'flag[').replace(', ', ":").replace(') == ', '] = ').replace(') {', '')
    exec(line)

print "".join(flag)
```

This gives us the flag at the price of allowing `exec` to slip into our code.

```console
root@kali:/media/sf_CTFs/pico/Client-side-again# python solve.py
flag[0:split * 2] = "picoCTF{"
flag[7:9] = "{n"
flag[split * 2:split * 2 * 2] = "not_this"
flag[3:6] = "oCT"
flag[split * 3 * 2:split * 4 * 2] = "55670}"
flag[6:11] = "F{not"
flag[12:16] = "this"
flag[split * 2 * 2:split * 3 * 2] = "_again_0"
picoCTF{not_this_again_055670}
```