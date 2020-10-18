# Basics
Category: Hardware

> With all those CPU bugs I don't trust software anymore, so I came up with my custom TPM (trademark will be filed soon!). You can't break this, so don't even try.

The following files were attached:

`main.cpp`:
```cpp
#include "obj_dir/Vcheck.h"

#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
    Verilated::commandArgs(argc, argv);
    std::cout << "Enter password:" << std::endl;
    auto check = std::make_unique<Vcheck>();

    for (int i = 0; i < 100 && !check->open_safe; i++) {
        int c = fgetc(stdin);
        if (c == '\n' || c < 0) break;
        check->data = c & 0x7f;
        check->clk = false;
        check->eval();
        check->clk = true;
        check->eval();
    }
    if (check->open_safe) {
        std::cout << "CTF{real flag would be here}" << std::endl;
    } else {
        std::cout << "=(" << std::endl;
    }
    return 0;
}
```

`check.sv`:
```verilog
module check(
    input clk,

    input [6:0] data,
    output wire open_safe
);

reg [6:0] memory [7:0];
reg [2:0] idx = 0;

wire [55:0] magic = {
    {memory[0], memory[5]},
    {memory[6], memory[2]},
    {memory[4], memory[3]},
    {memory[7], memory[1]}
};

wire [55:0] kittens = { magic[9:0],  magic[41:22], magic[21:10], magic[55:42] };
assign open_safe = kittens == 56'd3008192072309708;

always_ff @(posedge clk) begin
    memory[idx] <= data;
    idx <= idx + 5;
end

endmodule
```

## Solution

So this is basically a HW module representation in Verilog.

The C++ part reads a password and feeds it to the HW module. The HW module reads one character at time into the `data` input, then stores it in a `memory` register, which is an array of 8 element, where each element is 7 bits wide (just enough for an ASCII character). This causes the `magic` and `kittens` wires to get a value based on the memory, and if the value is right - the `open_safe` wire is set, allowing us to read the flag.

We can use a [Verilog simulator](https://www.tutorialspoint.com/compile_verilog_online.php) to simulate the flow. We'll add a print for the expected value of `kittens` based on the constant in the code, and we'll use trial and error to populate the different memory registers until the end result matches the expected result.

We start with the following template, where all eight `memory` elements are zero:

```verilog
module main;
    integer i;
    reg [6:0] memory [7:0]; // 8-element memory 7 bits wide
    reg [2:0] idx = 0; // 3 bit register
    
    wire [55:0] magic = {
        {memory[0], memory[5]},
        {memory[6], memory[2]},
        {memory[4], memory[3]},
        {memory[7], memory[1]}
    };
    
    wire [55:0] kittens = { magic[9:0],  magic[41:22], magic[21:10], magic[55:42] };
    assign open_safe = kittens == 56'd3008192072309708;
    
    initial 
        begin
            // We change these values via trial and error until "current" matches "expected"
            memory[0] = 7'b0;
            memory[1] = 7'b0;
            memory[2] = 7'b0;
            memory[3] = 7'b0;
            memory[4] = 7'b0;
            memory[5] = 7'b0;
            memory[6] = 7'b0;
            memory[7] = 7'b0;
            
            #1 $display("current   = %056b", kittens);
            #1 $display("expected  = %056b", 3008192072309708);
            #1 $display("open_safe = %0b", open_safe);
                        
            $finish ;
        end
endmodule
```

Output: 
```
current   = 00000000000000000000000000000000000000000000000000000000
expected  = 00001010101011111110111101001011111000101101101111001100
open_safe = 0
```

So now, let's change the first memory element to `0b1111111` and see what happens:
```verilog
memory[0] = 7'b0;
```

Output:
```
current   = 00000000000000000000000000000000000000000011111110000000
expected  = 00001010101011111110111101001011111000101101101111001100
open_safe = 0
```

We can where the input ended up in `kittens`, compare it to the expected value of `kittens` and deduce that the value that the first element should contain is `0b0110111`.

Let's see what happens when we set the correct value:
```verilog
memory[0] = 7'b0110111;
```

Output:
```
current   = 00000000000000000000000000000000000000000001101110000000
expected  = 00001010101011111110111101001011111000101101101111001100
open_safe = 0
```

Looks good. We matched a segment. Let's continue to the next element:
```
memory[1] = 7'b1111111;
```

Output:
```
current   = 00011111110000000000000000000000000000000001101110000000
expected  = 00001010101011111110111101001011111000101101101111001100
open_safe = 0
```

So we know that the second element should have the value of `0b0101010`.

We continue until we get all the elements right:
```verilog
memory[0] = 7'b0110111;
memory[1] = 7'b0101010;
memory[2] = 7'b1101111;
memory[3] = 7'b1111000;
memory[4] = 7'b0100101;
memory[5] = 7'b1001100;
memory[6] = 7'b1011111;
memory[7] = 7'b1011000;
```

And the output:
```
current   = 00001010101011111110111101001011111000101101101111001100
expected  = 00001010101011111110111101001011111000101101101111001100
open_safe = 1
```

As we said earlier, each memory element is a character. We can print them as ASCII using the following code:
```verilog
for (i = 0; i < 8; i = i + 1) begin
    #1 $display("%d: %c", i, memory[i]);
end
```

Output:
```
0: 7
1: *
2: o
3: x
4: %
5: L
6: _
7: X
```

However, this isn't the password, these are just the characters *used in the* password. To get the correct password we must reorder them. To understand why, let's take a closer look at how the original Verilog program populates the `memory` array:
```verilog
reg [2:0] idx = 0;
//...
always_ff @(posedge clk) begin
    memory[idx] <= data;
    idx <= idx + 5;
end
```

We have a 3-bit register holding the destination index, and it is incremented by 5 after each iteration. This means that it will overflow a few times during the process of reading the password.
First, it will populate `memory[0]`, then `memory[5]`, then `memory[2]` (since `5 + 5 % 8 = 2`) and so on.

So, to get the right order, we have to follow the same logic:
```verilog
#1 $write("Password  = ");
for (i = 0; i < 8; i = i + 1) begin
    #1 $write("%c", memory[idx]);
    idx <= idx + 5;
end
```

This will print the password in the correct order according to what we saw earlier (0, 5, 2, ...), using the same overflow behavior of `idx`.

To sum things up, here's our final Verilog program:

```verilog
module main;
    integer i;
    reg [6:0] memory [7:0]; // 8-element memory 7 bits wide
    reg [2:0] idx = 0; // 3 bit register
    
    wire [55:0] magic = {
        {memory[0], memory[5]},
        {memory[6], memory[2]},
        {memory[4], memory[3]},
        {memory[7], memory[1]}
    };
    
    wire [55:0] kittens = { magic[9:0],  magic[41:22], magic[21:10], magic[55:42] };
    assign open_safe = kittens == 56'd3008192072309708;
    
    initial 
        begin
            // We change these values via trial and error until "current" matches "expected"
            memory[0] = 7'b0110111;
            memory[1] = 7'b0101010;
            memory[2] = 7'b1101111;
            memory[3] = 7'b1111000;
            memory[4] = 7'b0100101;
            memory[5] = 7'b1001100;
            memory[6] = 7'b1011111;
            memory[7] = 7'b1011000;
            
            #1 $display("current   = %056b", kittens);
            #1 $display("expected  = %056b", 3008192072309708);
            #1 $display("open_safe = %0b", open_safe);
            
            #1 $write("Password  = ");
            for (i = 0; i < 8; i = i + 1) begin
              #1 $write("%c", memory[idx]);
              idx <= idx + 5;
            end
            
            $finish ;
        end
endmodule
```

Output:
```
$iverilog -o main *.v
$vvp main
current   = 00001010101011111110111101001011111000101101101111001100
expected  = 00001010101011111110111101001011111000101101101111001100
open_safe = 1
Password  = 7LoX%*_x
```

We enter this to the attached server and get the flag:
```console
root@kali:/media/sf_CTFs/google/basics# nc basics.2020.ctfcompetition.com 1337
Enter password:
7LoX%*_x
CTF{W4sTh4tASan1tyCh3ck?}
```