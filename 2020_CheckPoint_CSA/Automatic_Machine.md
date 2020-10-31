# Automatic Machine
Category: Reversing, 50 Points

## Description

> You stand before a custom Virtual Machine.
> 
> Once you understand the code, the flag will be just there.
> 
> The machine is waiting for you at auto-machine.csa-challenge.com
> 
> Good Luck!



## Solution

We enter the website and are greeted with a fake SSH shell, implemented in Javascript:
```
ssh 192.12.23.934
Connecting...
ACCESS DENIED! I AM CALLING THE INTERNET POLICE!
Password:

$ test
UNAUTHORIZED LOGIN!
$
```

It looks like we'll have to reverse engineer the shell implementation:
```javascript
a = [3, 3, 71, 17251, 1, 581, 3, 17254, 17276, 17330, 34502, 17252, 18012, 17263, 18015, 18028, 18035, 35263, 18012, 20231, 18017, 17435, 17450, 17433, 34682, 17431, 19842, 17446, 17431, 17453, 17499, 34679, 17428, 17917, 17453, 17431, 17434, 17529, 34679, 17428, 18543, 17433, 16850, 16880, 16956, 34098, 16848, 17906, 16865, 16850, 16884, 16849, 34098, 16849, 16945, 16898, 16853, 16857, 16847, 34096, 16847, 16896, 16891, 17905, 17906, 17981, 35089, 17903, 16881, 17905, 35094, 35091, 35102, 50358, 35092, 33262, 35205, 50359, 50357, 50417, 69591, 50360, 50504, 50366, 50320, 50360, 50320, 69680, 50319, 50385, 50363, 50385, 50439, 50343, 69617, 50388, 52917, 50373, 50254, 50193, 50406, 69614, 50192, 53220, 50245, 53077, 53077, 53247, 68341, 53078, 53354, 53015, 53260, 53241, 53259, 68140, 53259, 52192, 53014, 53016, 53246, 53080, 68152, 53013, 52001, 53058, 52909, 
/* <many more numbers...>, */
-8885, -31718, -98167, -57138, -55337, -55760, -1170, -57171, -50005, 20016, -88196, -88319, -88232, 62300, -85014, -83314, 16504, -89403, -90024, -87915, 59621, -89762, -84051, -16256, -22899, -26468, -24993, -35667, -23500, 13690, -10735, -12571, -12626, -12813, -28347, -12570, 22384, 29007, -6485, -6576, -6635, -24117, -6486, -5901, -29023, 17964, 17967, 18016, 1292, 18122, 22694, 22177, -20769, -20966, -21001, 27391, -20776, -20434, 16445, 19592, 19575, 19638, 3304, 19634, 22019, 18495, 18810, 19099, 18768, 2650, 18803, 18803, 18803, -3591, -3580, -3490, -3514, -3514, -3493, -3514, 88, 22, 23, 118, 116, 116, 114, 100, 100, 23, 112, 101, 118, 121, 99, 114, 115, 5, 19, 98, 121, 118, 98, 99, 127, 120, 101, 126, 109, 114, 115, 23, 123, 120, 112, 126, 121, 22]

_g0 = 7

function _f4(_p2, _p1, _p3, _p0) {
    for (_g1 = _p1; _g1 < _p3; _g1++) {
        _p2[_g1] += _p0;
    }
}

function _f3(_p2, _p1, _p3, _p0) {
    for (_g1 = _p1; _g1 < _p3; _g1++) {
        _p2[_g1] -= _p0;
    }
}

function _f2(_p2, _p1, _p3, _p0) {
    for (_g1 = _p1; _g1 < _p3; _g1++) {
        _p2[_g1] ^= _p0;
    }
}

function _f1(_p1, _p0) {
    _g2 = _p1[_p0+1]
    _g3 = ""
    for(_g4 = 0; _g4 < _g2; _g4++) {
        _g3+= String.fromCharCode(_p1[_p0+2+_g4] ^ 0x37)
    }
    return _g3
}

_g5 = [_f4, _f3, _f2];

function _f0(_p1, _p0) {
    _g6 = 0;
    while(_g6 < _p1.length) {
        _g7 = _p1[_g6];

        if (_g7 == 5) {
            return _f1(_p1, _g6);
            break;
        }

        _g8 = _p1[_g6+1];
        _g9 = _p1[_g6+2];
        if ((_p0[_g8] % _g9 == 0) == (4 - _g7)) {
            _g6 = _p1[_g6+3];
            continue;
        }
        _p0[_g8] = (_p0[_g8] % _g9) ? _p0[_g8] : Math.floor(_p0[_g8]/_g9);
        _ga = _p1[_g6+4];
        _gb = _p1[_g6+5];
        _gc = _p1[_g6+6] ^ _g7-3;
        _gd = _p1[_gc];
        _g5[_ga](_p1, _g6+_g0, (_gb+1)*_g0, _gd);
        _g6+= _g0;
        

    }
}

function sleep (time) {
  return new Promise((resolve) => setTimeout(resolve, time));
};


$(function () {
    var term = new window.Terminal.Terminal();

    term.open(document.getElementById('terminal'));
    term.setOption('theme', {
      background: '#222',
      foreground: '#22da26',
    });
    
    function runFakeTerminal() {
        if (term._initialized) {
            return;
        }

        term._initialized = true;
        term.input = "";
        term.prompt = () => {
            term.write('\r\n$ ');
        };

        term.write('\r\n\u001b[37m$\u001b[0m ');

        term.onKey(e => {
            const printable = !e.domEvent.altKey && !e.domEvent.altGraphKey && !e.domEvent.ctrlKey && !e.domEvent.metaKey;

            if (e.domEvent.keyCode === 13) {
                prompt(term);
            } else if (e.domEvent.keyCode === 8) {
                // Do not delete the prompt
                if (term._core.buffer.x > 2) {
                    prev_len = term.input.length
                    curr_x = term._core.buffer.x
                    term.input = term.input.substr(0, curr_x-3) + 
                    term.input.substr(curr_x-2);
                    for (forward=0;forward<prev_len - curr_x +2; forward++) {
                        term.write('\u001b[C');
                    }
                    for(back=0;back<prev_len;back++) {
                        term.write('\b \b');
                    }
                    term.write(term.input);
                    for(back=0;back<term.input.length - curr_x+3;back++) {
                        term.write('\u001b[D');
	                }
                }
            } else if (e.domEvent.keyCode == 37) {
                if (term._core.buffer.x > 2)
                    term.write(e.key);
            }
            else if (e.domEvent.keyCode == 39) {
                if (term._core.buffer.x < term.input.length + 2)
                    term.write(e.key);
            }
            else if (printable && e.domEvent.keyCode != 38 && e.domEvent.keyCode != 40) {
                term.input = term.input.substr(0, term.buffer.cursorX-2) + 
                e.key + 
                term.input.substr(term.buffer.cursorX-2);
                // term.write(e.key);
                term.write(term.input.substr(term.buffer.cursorX-2));
                for(back=0;back<term.input.length - term.buffer.cursorX+1;back++) {
                    term.write('\u001b[D');
                }
            }
        });
    }

    function prompt(term) {
      term.write('\r\n');
      term.write(_f0(JSON.parse(JSON.stringify(a)),Array.from(term.input).map(x=>x.charCodeAt(0))));
      term.input = "";
      term.write('\r\n\u001b[37m$\u001b[0m ');
    }

    sleep(3000).then(runFakeTerminal);
});
```

It looks like the entry point is `_f0(JSON.parse(JSON.stringify(a)),Array.from(term.input).map(x=>x.charCodeAt(0)))`, which takes our password and sends it to `_f0` together with a large number array. `_f0` implements a VM, taking numbers from the array and using them as instructions and data.

The first thing we should do is remove irrelevant code and rename some symbols for easier understanding:
```javascript
step_size = 7

function add_to_arr(vm_arr, from_index, to_index, value) {
    for (i = from_index; i < to_index; i++) {
        vm_arr[i] += value;
    }
}

function subtract_from_arr(vm_arr, from_index, to_index, value) {
    for (i = from_index; i < to_index; i++) {
        vm_arr[i] -= value;
    }
}

function xor_arr(vm_arr, from_index, to_index, value) {
    for (i = from_index; i < to_index; i++) {
        vm_arr[i] ^= value;
    }
}

function decode_message(vm_arr, from_index) {
    to_index = vm_arr[from_index+1]
    res = ""
    for(i = 0; i < to_index; i++) {
        res+= String.fromCharCode(vm_arr[from_index+2+i] ^ 0x37)
    }
    return res
}

action_arr = [add_to_arr, subtract_from_arr, xor_arr];


function test_password(vm_arr, user_input) {
    index = 0;
    while(index < vm_arr.length) {
        _g7 = vm_arr[index];

        if (_g7 == 5) {
            return decode_message(vm_arr, index);
            break;
        }

        user_input_index = vm_arr[index+1];
        _g9 = vm_arr[index+2];
        if ((user_input[user_input_index] % _g9 == 0) == (4 - _g7)) {
            index = vm_arr[index+3];
            continue;
        }
        user_input[user_input_index] = (user_input[user_input_index] % _g9) ? user_input[user_input_index] : Math.floor(user_input[user_input_index]/_g9);
        action_index = vm_arr[index+4];
        _gb = vm_arr[index+5];
        _gc = vm_arr[index+6] ^ _g7-3;
        _gd = vm_arr[_gc];
        action_arr[action_index](vm_arr, index+step_size, (_gb+1)*step_size, _gd);
        index+= step_size;
        

    }
}
```

Basically the VM starts executing from the array. Each 7 elements of the array belong to a single operation, and the VM uses `index` to go execute these operations one by one. For example, the index into the user input which the VM will currently work on is retrieved from `vm_arr[index+2]`, and the selection of an action function is retrieved from `vm_arr[index+4]`. After the operation is completed, the VM performs `index += 7`, and moves on to the next operation. 

The actions that the VM can perform are adding a constant to a sequence of elements, subtracting from a sequence and XORing a sequence. 

The VM terminates when it reads a special instruction from the array or when it finishes executing all the operations.


Let's try to understand what the VM would do for the first operation:
```
3, 3, 71, 17251, 1, 581, 3
```

It sets `_g7` to `3`. Then it sets `user_input_index` to `3` as well, meaning that we're going to check `user_input[3]`. It continues by setting `_g9` to `71`.

Now, it checks if `user_input[3] % 71 == 0`. The (boolean) result is compared against a predefined result, which is expressed as `4 - _g7`. Since `_g7` is `3` in our case, the predefined result evaluates to `4 - 3 = 1` and therefore if `user_input[3] % 71 == 0` then the complete condition will be true.

Since we control the user input, we need to understand what happens if the condition evaluates to `true` and what happens if it evaluates to `false`.

If the condition is true, we jump to offset `17251` in the array and execute from there:
```
>>> a[17251]
5
```

This is the stop condition. We can emulate what would happen by calling:
```
_f1(a, 17251)
"UNAUTHORIZED LOGIN!"
```

So this is the failure branch, and we want `user_input[3] % 71 == 0` to be false in this case.

Let's continue analyzing what would happen in case the condition is false.

We then (might) reassign a new value to `user_input[3]`, and continue by reading an action from the current operation. In our example, the action index is `1`, so the action from the action array is `subtract_from_arr`. The rest of the logic is related to executing the action on a slice of the array (and moving to the next operation). This action actually decodes the next operation that will be executed.

Before decoding, the next action is:
```
17254, 17276, 17330, 34502, 17252, 18012, 17263
```

But after decoding (in our example, by subtracting `17251` from the array members between index `7` and `4074`), we get:
```
3, 25, 79, 17251, 1, 761, 12
```

To conclude:
 * Every iteration of the loop decodes part of the array
 * If the `(user_input[user_input_index] % _g9 == 0) == (4 - _g7)` condition evaluates to true with `vm_arr[index+3] = 17251` then the login is unauthorized.


Given these insights, we can implement a solver using `z3`:
```python
from pwn import *
from z3 import *
from arr import a
import math
import itertools

STEP_SIZE = 7
FAILURE_ADDRESS = 17251

def add_to_arr(vm_arr, from_index, to_index, value):
    for i in range(from_index, to_index):
        vm_arr[i] += value

def subtract_from_arr(vm_arr, from_index, to_index, value):
    for i in range(from_index, to_index):
        vm_arr[i] -= value

def xor_arr(vm_arr, from_index, to_index, value):
    for i in range(from_index, to_index):
        vm_arr[i] ^= value

def decode_message(vm_arr, _p0):
    to_index = vm_arr[_p0+1]
    res = ""
    for i in range(to_index):
        res += chr(vm_arr[_p0+2+i] ^ 0x37)
    return res

action_arr = [add_to_arr, subtract_from_arr, xor_arr]

def crack_password(vm_arr, solver, key, progress):
    index = 0

    while(index < len(vm_arr)):
        progress.status("Working on index #{}".format(index))
        _g7 = vm_arr[index]

        if (_g7 == 5):
            return decode_message(vm_arr, index)
            break

        user_input_index = vm_arr[index+1]
        _g9 = vm_arr[index+2]

        if (vm_arr[index+3] == FAILURE_ADDRESS):
            assert((4 - _g7) in [0, 1])
            if (4 - _g7):
                solver.add(key[user_input_index][-1] % _g9 != 0)
            else:
                solver.add(key[user_input_index][-1] % _g9 == 0)
                # The way to account for the following logic:
                # "user_input[user_input_index] = Math.floor(user_input[user_input_index]/_g9);"
                # is to create a new z3 variable and add this relationship as a constraint.
                # The new variable will be used from now on for future constraints.
                # This is the reason that "key" is a list of lists of variables and not a flat list of variables.
                k = key[user_input_index][-1]
                new_var = Int("{}-{}".format(user_input_index, len(key[user_input_index])+1))
                key[user_input_index].append(new_var)
                solver.add(new_var * _g9 == k) # Rephrased as multiplication since z3 has problems with division of integers
                
        else:
            print (vm_arr[index+3])

        action_index = vm_arr[index+4]
        _gb = vm_arr[index+5]
        _gc = vm_arr[index+6] ^ _g7-3
        _gd = vm_arr[_gc]
        action_arr[action_index](vm_arr, index+STEP_SIZE, (_gb+1)*STEP_SIZE, _gd)
        index+= STEP_SIZE


for key_len in itertools.count():
    try:
        with log.progress('Trying a key of length {}'.format(key_len)) as progress:
            
            key = [[Int("{}".format(i))] for i in range(key_len)]
            solver = Solver()

            # all values are printable characters excluding space (33 - 126)
            for i in range(key_len):
                solver.add(key[i][0] >= ord('!'))
                solver.add(key[i][0] <= ord('~'))

            crack_password(a.copy(), solver, key, progress)

            progress.status("Trying to solve the constraints...")
            if solver.check() == sat:
                model = solver.model()

                res = ""
                for i in range(key_len):
                    res += chr(model[key[i][0]].as_long())
                
                break

    except IndexError:
        pass

log.success("Found the following password: '{}'".format(res))

```

Running it, we get:
```console
[-] Trying a key of length 36: Failed
[-] Trying a key of length 37: Failed
[-] Trying a key of length 38: Failed
[-] Trying a key of length 39: Failed
[-] Trying a key of length 40: Failed
[+] Trying a key of length 41: Done
[+] Found the following password: 'CSA{w0w_th4t_wa$_re@lly_s1mpLe_wasn7_1t}q'
```

Of course, the accepted flag was `CSA{w0w_th4t_wa$_re@lly_s1mpLe_wasn7_1t}` (without the `q` at the end). For some reason, the VM instructions try to access index #41 of the user input, and that's why the solver had to add an extra character to the solution. In the Javascript implementation, accessing an extra character after the user input is not as fatal as in Python though...

