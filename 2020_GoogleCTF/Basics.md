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

The C++ part reads a password and feeds it to the HW module. The HW module reads one character at time into the `data` input, then stores it in a `memory` register. This causes the `magic` and `kittens` wires to get a value based on the memory, and if the value is right - the `open_safe` wire is set, allowing us to read the flag.

We can use a [Verilog simulator](https://www.tutorialspoint.com/compile_verilog_online.php) to simulate the flow. We'll add a print for the expected value of `kittens` based on the constant in the code, and we'll use trial an error to populate the different memory registers until the end result matches the expected result.

Eventually, we arrive to the following:

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

This gives us the flag:
```console
root@kali:/media/sf_CTFs/google/basics# nc basics.2020.ctfcompetition.com 1337
Enter password:
7LoX%*_x
CTF{W4sTh4tASan1tyCh3ck?}
```