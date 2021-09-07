# Unknown - Master Office
Category: Reversing

## Description

> You press a button and enter through a tinted glass door. There is a vast oil painting on the wall that depicts a bold man with a scar under his left eye, under his arms rests a white chubby cat. Below the painting is the very same man, and he’s addressing you: "Well, well, well. Isn’t it the trouble maker? Huh, how did you get past the guards? Well, I have a final offer for you. I’ll let you live only on one condition: START WORKING FOR ME! BWAHAHAHAHA-" While he goes on with his monologue about conquering the world and some twisted philosophy about how he is actually the good guy and so on you start discretely fiddling with a control panel labeled "Self destruction". You need to quickly figure out the activation code while he’s distracted.
> 
> Challenge: Strange Virtual Machine (reversing)
> 
> Everyone is coming up with their own programming language these days, so I came up with my own architecture. You can use it to run the attached program that will print the flag for you

A ROM file and an archive were attached.

## Solution

Let's check the attached files:

```console
┌──(user@kali)-[/media/sf_CTFs/google/18_Unknown_-_Master_Office/files]
└─$ xxd -g 1 vm.rom
00000000: 01 69 00 00 00 00 02 01 69 0b bc 00 00 00 ff 08  .i......i.......
00000010: 04 47 02 47 01 01 69 00 00 00 00 09 47 01 69 0a  .G.G..i.....G.i.
00000020: 30 00 00 00 01 69 00 00 00 00 02 00 69 05 47 08  0....i......i.G.
00000030: 01 69 00 01 00 00 09 47 00 69 0a 45 00 00 00 02  .i.....G.i.E....
00000040: 00 47 05 47 08 01 69 00 01 00 00 03 69 47 04 69  .G.G..i.....iG.i
00000050: 02 01 69 0b 10 00 00 00 05 47 08 05 47 08 04 47  ..i......G..G..G
00000060: 02 47 01 01 69 02 00 00 00 09 47 01 69 0a 7e 00  .G..i.....G.i.~.
00000070: 00 00 01 69 01 00 00 00 02 00 69 05 47 08 01 69  ...i......i.G..i
00000080: 01 00 00 00 03 69 47 04 69 02 01 69 0b 5e 00 00  .....iG.i..i.^..
00000090: 00 02 78 00 04 78 01 69 02 00 00 00 03 69 47 04  ..x..x.i.....iG.
000000a0: 69 02 01 69 0b 5e 00 00 00 05 78 02 79 00 03 69  i..i.^....x.y..i
000000b0: 78 03 79 02 00 69 05 47 08 05 47 08 04 47 02 47  x.y..i.G..G..G.G
000000c0: 01 02 01 42 fc 09 47 00 00 0a 2f 01 00 00 02 01  ...B..G.../.....
000000d0: 42 02 02 47 fd 02 48 00 04 48 04 48 01 69 01 00  B..G..H..H.H.i..
000000e0: 00 00 03 69 47 03 69 02 01 69 0b 5e 00 00 00 05  ...iG.i..i.^....
000000f0: 48 03 69 47 03 00 03 69 48 03 69 02 01 69 0b 10  H.iG...iH.i..i..
00000100: 00 00 00 05 48 02 48 00 04 48 04 48 02 01 48 fe  ....H.H..H.H..H.
00000110: 05 48 05 48 04 48 04 48 01 69 01 00 00 00 03 69  .H.H.H.H.i.....i
00000120: 47 03 69 02 01 69 0b bc 00 00 00 05 48 05 48 05  G.i..i......H.H.
00000130: 47 08                                            G.

┌──(user@kali)-[/media/sf_CTFs/google/18_Unknown_-_Master_Office/files]
└─$ tar -xvf vm-impl.tgz
vm/
vm/Cargo.toml
vm/src/
`vm/src/lib.rs`
vm/.gitignore
vm-cli/
vm-cli/Cargo.toml
vm-cli/src/
vm-cli/src/main.rs
vm-cli/Cargo.lock
vm-cli/.gitignore
```

The interesting files are `vm/src/lib.rs`:

```rust
use std::io::{Read, Write};

pub type Register = char;
pub const REG_RV: Register = 0 as Register;
pub const REG_ARG0: Register = 1 as Register;
pub const REG_ARG1: Register = 2 as Register;
pub const REG_FLAG: Register = 0xFF as Register;

// Instructions
#[derive(Copy, Clone, Debug)]
pub enum Instruction {
    Nop,
    MovConst {
        reg: Register,
        v: u32,
    },
    MovReg {
        reg: Register,
        reg2: Register,
    },
    MathOp {
        reg_out: Register,
        reg: Register,
        op: u8,
        reg2: Register,
    },
    PushReg(Register),
    PopReg(Register),
    PushConst(u32),
    JmpConst(u32),
    PopPc,
    Test {
        reg: Register,
        op: u8,
        reg2: Register,
    },
    JmpCond(u32),
    Call(u32),

    Strlen,
    CharAt,
    Print,
    Exit,
}

impl Instruction {
    pub fn read(f: &mut std::fs::File) -> std::io::Result<Self> {
        let mut opcode = [0u8];
        f.read_exact(&mut opcode)?;

        let read_u32 = |f: &mut std::fs::File| -> std::io::Result<u32> {
            let mut buf = [0u8; 4];
            f.read_exact(&mut buf)?;
            let res: u32 = unsafe { std::mem::transmute(buf) };
            Ok(res)
        };

        let read_u8 = |f: &mut std::fs::File| -> std::io::Result<u8> {
            let mut buf = [0u8];
            f.read_exact(&mut buf)?;
            Ok(buf[0])
        };

        let read_reg = |f: &mut std::fs::File| read_u8(f).map(|x| x as char);

        Ok(match opcode[0] {
            0 => Instruction::Nop,
            1 => Instruction::MovConst {
                reg: read_reg(f)?,
                v: read_u32(f)?,
            },
            2 => Instruction::MovReg {
                reg: read_reg(f)?,
                reg2: read_reg(f)?,
            },
            3 => Instruction::MathOp {
                reg_out: read_reg(f)?,
                reg: read_reg(f)?,
                op: read_u8(f)?,
                reg2: read_reg(f)?,
            },
            4 => Instruction::PushReg(read_reg(f)?),
            5 => Instruction::PopReg(read_reg(f)?),
            6 => Instruction::PushConst(read_u32(f)?),
            7 => Instruction::JmpConst(read_u32(f)?),
            8 => Instruction::PopPc,
            9 => Instruction::Test {
                reg: read_reg(f)?,
                op: read_u8(f)?,
                reg2: read_reg(f)?,
            },
            10 => Instruction::JmpCond(read_u32(f)?),
            11 => Instruction::Call(read_u32(f)?),

            0xFC => Instruction::Strlen,
            0xFD => Instruction::CharAt,
            0xFE => Instruction::Print,
            0xFF => Instruction::Exit,
            _ => unreachable!(),
        })
    }

    pub fn write(&self, f: &mut std::fs::File) -> std::io::Result<()> {
        f.write_all(&[(*self).into()])?;

        use Instruction::*;
        match self {
            MovConst { reg, v } => {
                let v_raw: [u8; 4] = unsafe { std::mem::transmute(*v) };
                f.write_all(&[*reg as u8])?;
                f.write_all(&v_raw)?;
            }
            MovReg { reg, reg2 } => {
                f.write_all(&[*reg as u8, *reg2 as u8])?;
            }
            MathOp {
                reg_out,
                reg,
                op,
                reg2,
            } => {
                f.write_all(&[*reg_out as u8, *reg as u8, *op, *reg2 as u8])?;
            }
            PushReg(reg) => {
                f.write_all(&[*reg as u8])?;
            }
            PopReg(reg) => {
                f.write_all(&[*reg as u8])?;
            }
            PushConst(c) => {
                let c_raw: [u8; 4] = unsafe { std::mem::transmute(*c) };
                f.write_all(&c_raw)?;
            }
            JmpConst(c) => {
                let c_raw: [u8; 4] = unsafe { std::mem::transmute(*c) };
                f.write_all(&c_raw)?;
            }
            Test { reg, op, reg2 } => {
                f.write_all(&[*reg as u8, *op, *reg2 as u8])?;
            }
            JmpCond(c) => {
                let c_raw: [u8; 4] = unsafe { std::mem::transmute(*c) };
                f.write_all(&c_raw)?;
            }
            Call(c) => {
                let c_raw: [u8; 4] = unsafe { std::mem::transmute(*c) };
                f.write_all(&c_raw)?;
            }
            PopPc | Strlen | CharAt | Print | Exit | Nop => {}
        }

        Ok(())
    }
}

impl std::fmt::Display for Instruction {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        use Instruction::*;
        match self {
            MovConst { reg, v } => write!(f, "mov {:?}, 0x{:08X}", *reg, v),
            MovReg { reg, reg2 } => write!(f, "mov {:?}, {:?}", *reg, *reg2),
            MathOp {
                reg_out,
                reg,
                op,
                reg2,
            } => write!(f, "{:?} = alu({:?}, {}, {:?}", *reg_out, *reg, *op, *reg2),
            PushReg(reg) => write!(f, "push {:?}", *reg),
            PopReg(reg) => write!(f, "pop {:?}", *reg),
            PushConst(c) => write!(f, "push 0x{:X}", *c),
            JmpConst(c) => write!(f, "jmp 0x{:X}", *c),
            Test { reg, op, reg2 } => write!(f, "test {:?}, {}, {:?}", *reg, *op, *reg2),
            JmpCond(c) => write!(f, "jmpc 0x{:X}", *c),
            Call(c) => write!(f, "call 0x{:X}", *c),
            PopPc => write!(f, "ret"),
            Strlen => write!(f, "call Strlen"),
            CharAt => write!(f, "call CharAt"),
            Print => write!(f, "chall Print"),
            Exit => write!(f, "exit"),
            Nop => write!(f, "nop"),
        }
    }
}

impl Into<u8> for Instruction {
    fn into(self) -> u8 {
        use Instruction::*;
        match self {
            Nop => 0,
            MovConst { .. } => 1,
            MovReg { .. } => 2,
            MathOp { .. } => 3,
            PushReg(_) => 4,
            PopReg(_) => 5,
            PushConst(_) => 6,
            JmpConst(_) => 7,
            PopPc => 8,
            Test { .. } => 9,
            JmpCond(_) => 10,
            Call(_) => 11,

            Strlen => 0xFC,
            CharAt => 0xFD,
            Print => 0xFE,
            Exit => 0xFF,
        }
    }
}

```

And `vm-cli/src/main.rs`:

```rust
extern crate vm;

use std::io::{Seek, Write};
use vm::*;

const INPUT_DATA: [u32; 63] = [
    66, 82, 66, 117, 75, 91, 86, 87, 31, 51, 222, 187, 112, 236, 9, 98, 34, 69, 0, 198, 150, 29,
    96, 10, 69, 26, 253, 225, 164, 8, 110, 67, 102, 108, 103, 162, 209, 1, 173, 130, 186, 5, 123,
    109, 187, 215, 86, 232, 23, 215, 184, 79, 171, 232, 128, 67, 138, 153, 251, 92, 4, 94, 93,
];
struct Vm {
    f: std::fs::File,
    registers: [u32; 256],
    stack: [u32; 64 * 1024],
    flag: bool,
    sp: usize,
}

impl Vm {
    fn new<T: AsRef<std::path::Path>>(path: T) -> std::io::Result<Self> {
        let f = std::fs::File::open(path.as_ref())?;
        Ok(Self {
            f,
            registers: [0u32; 256],
            stack: [0u32; 64 * 1024],
            flag: false,
            sp: 0,
        })
    }

    fn step(&mut self) -> std::io::Result<()> {
        // let pos = self.f.stream_position()?;
        let instruction = Instruction::read(&mut self.f)?;
        /*
        if false {
            println!(
                "{:04X} | sp={:04} | flg={} | {:?}",
                pos, self.sp, self.flag, instruction,
            );
            for (idx, v) in self.registers.iter().enumerate() {
                if *v > 0 {
                    println!("{:?} -> {}", idx, v);
                }
            }
        }
        */
        use Instruction::*;
        match instruction {
            Nop => {}
            MovConst { reg, v } => self.registers[reg as usize] = v,
            MovReg { reg, reg2 } => self.registers[reg as usize] = self.registers[reg2 as usize],
            MathOp {
                reg_out,
                reg,
                op,
                reg2,
            } => {
                let r1 = self.registers[reg as usize];
                let r2 = self.registers[reg2 as usize];
                self.registers[reg_out as usize] = match op {
                    3 => r1 + r2,
                    4 => r1 - r2,
                    5 => r1 * r2,
                    6 => r1 / r2,
                    _ => unimplemented!(),
                };
            }
            PushReg(reg) => {
                self.stack[self.sp] = self.registers[reg as usize];
                self.sp += 1;
            }
            PopReg(reg) => {
                self.registers[reg as usize] = self.stack[self.sp - 1];
                self.sp -= 1;
            }
            PushConst(c) => {
                self.stack[self.sp] = c;
                self.sp += 1
            }
            JmpConst(c) => {
                self.f.seek(std::io::SeekFrom::Start(c as u64))?;
            }
            PopPc => {
                self.f
                    .seek(std::io::SeekFrom::Start(self.stack[self.sp - 1] as u64))?;
                self.sp -= 1;
            }
            Test { reg, op, reg2 } => {
                self.flag = match op {
                    0 => self.registers[reg as usize] < self.registers[reg2 as usize],
                    1 => self.registers[reg as usize] <= self.registers[reg2 as usize],
                    _ => unimplemented!(),
                }
            }
            JmpCond(c) => {
                if !self.flag {
                    self.f.seek(std::io::SeekFrom::Start(c as u64))?;
                }
            }
            Call(v) => {
                self.stack[self.sp] = self.f.stream_position().unwrap() as u32;
                self.sp += 1;
                self.f.seek(std::io::SeekFrom::Start(v as u64))?;
            }

            Strlen => {
                self.registers[REG_RV as usize] = INPUT_DATA.len() as u32;
            }
            CharAt => {
                let arg2 = self.registers[REG_ARG1 as usize];
                self.registers[REG_RV as usize] = INPUT_DATA
                    .iter()
                    .nth(arg2 as usize)
                    .map(|v| *v as u32)
                    .unwrap_or(0_u32);
            }
            Print => {
                let stdout = std::io::stdout();
                let mut lock = stdout.lock();
                lock.write_all(&[self.registers[REG_ARG0 as usize] as u8])?;
                lock.flush()?;
            }
            Exit => {
                std::process::exit(0);
            }
        };
        Ok(())
    }
}

fn main() -> std::io::Result<()> {
    if let Some(path) = std::env::args().nth(1) {
        let mut vm = Vm::new(path)?;
        loop {
            vm.step()?;
        }
    } else {
        println!("Missing ROM");
        Ok(())
    }
}
```

This looks like the implementation of a VM. Let's run it:

```console
┌──(user@kali)-[/media/…/google/18_Unknown_-_Master_Office/files/vm-cli]
└─$ cargo build
   Compiling vm v0.1.0 (/media/sf_CTFs/google/18_Unknown_-_Master_Office/files/vm)
   Compiling vm-cli v0.1.0 (/media/sf_CTFs/google/18_Unknown_-_Master_Office/files/vm-cli)
    Finished dev [unoptimized + debuginfo] target(s) in 16.26s

┌──(user@kali)-[/media/…/google/18_Unknown_-_Master_Office/files/vm-cli]
└─$ ./target/debug/vm-cli ../vm.rom
CTF{ThisIsAVeryLongFlagA^C
```

The program starts printing the flag, but gets slower and slower with each character printed. We'll have to understand the algorithm and try to optimize it. We'll do that by utilizing the disassembler logic within the cli we've received. So, in `main.rs`, we modify `step` to the following:

```rust
fn step(&mut self) -> std::io::Result<()> {
  let pos = self.f.stream_position()?;
  let instruction = Instruction::read(&mut self.f)?;
  println!("0x{:03X}:\t{}", pos, instruction);
  Ok(())
}
```

We run the modified program and get the disassembly:

```console
┌──(user@kali)-[/media/…/google/18_Unknown_-_Master_Office/files/vm-cli]
└─$ ./target/debug/vm-cli ../vm.rom
0x000:  mov 'i', 0x00000000
0x006:  mov '\u{1}', 'i'
0x009:  call 0xBC
0x00E:  exit
0x00F:  ret
0x010:  push 'G'
0x012:  mov 'G', '\u{1}'
0x015:  mov 'i', 0x00000000
0x01B:  test 'G', 1, 'i'
0x01F:  jmpc 0x30
0x024:  mov 'i', 0x00000000
0x02A:  mov '\u{0}', 'i'
0x02D:  pop 'G'
0x02F:  ret
0x030:  mov 'i', 0x00000100
0x036:  test 'G', 0, 'i'
0x03A:  jmpc 0x45
0x03F:  mov '\u{0}', 'G'
0x042:  pop 'G'
0x044:  ret
0x045:  mov 'i', 0x00000100
0x04B:  'i' = alu('G', 4, 'i'
0x050:  mov '\u{1}', 'i'
0x053:  call 0x10
0x058:  pop 'G'
0x05A:  ret
0x05B:  pop 'G'
0x05D:  ret
0x05E:  push 'G'
0x060:  mov 'G', '\u{1}'
0x063:  mov 'i', 0x00000002
0x069:  test 'G', 1, 'i'
0x06D:  jmpc 0x7E
0x072:  mov 'i', 0x00000001
0x078:  mov '\u{0}', 'i'
0x07B:  pop 'G'
0x07D:  ret
0x07E:  mov 'i', 0x00000001
0x084:  'i' = alu('G', 4, 'i'
0x089:  mov '\u{1}', 'i'
0x08C:  call 0x5E
0x091:  mov 'x', '\u{0}'
0x094:  push 'x'
0x096:  mov 'i', 0x00000002
0x09C:  'i' = alu('G', 4, 'i'
0x0A1:  mov '\u{1}', 'i'
0x0A4:  call 0x5E
0x0A9:  pop 'x'
0x0AB:  mov 'y', '\u{0}'
0x0AE:  'i' = alu('x', 3, 'y'
0x0B3:  mov '\u{0}', 'i'
0x0B6:  pop 'G'
0x0B8:  ret
0x0B9:  pop 'G'
0x0BB:  ret
0x0BC:  push 'G'
0x0BE:  mov 'G', '\u{1}'
0x0C1:  mov '\u{1}', 'B'
0x0C4:  call Strlen
0x0C5:  test 'G', 0, '\u{0}'
0x0C9:  jmpc 0x12F
0x0CE:  mov '\u{1}', 'B'
0x0D1:  mov '\u{2}', 'G'
0x0D4:  call CharAt
0x0D5:  mov 'H', '\u{0}'
0x0D8:  push 'H'
0x0DA:  push 'H'
0x0DC:  mov 'i', 0x00000001
0x0E2:  'i' = alu('G', 3, 'i'
0x0E7:  mov '\u{1}', 'i'
0x0EA:  call 0x5E
0x0EF:  pop 'H'
0x0F1:  'i' = alu('G', 3, '\u{0}'
0x0F6:  'i' = alu('H', 3, 'i'
0x0FB:  mov '\u{1}', 'i'
0x0FE:  call 0x10
0x103:  pop 'H'
0x105:  mov 'H', '\u{0}'
0x108:  push 'H'
0x10A:  push 'H'
0x10C:  mov '\u{1}', 'H'
0x10F:  chall Print
0x110:  pop 'H'
0x112:  pop 'H'
0x114:  push 'H'
0x116:  push 'H'
0x118:  mov 'i', 0x00000001
0x11E:  'i' = alu('G', 3, 'i'
0x123:  mov '\u{1}', 'i'
0x126:  call 0xBC
0x12B:  pop 'H'
0x12D:  pop 'H'
0x12F:  pop 'G'
0x131:  ret
Error: Error { kind: UnexpectedEof, message: "failed to fill whole buffer" }
```

Now, the straightforward thing to do would be to reverse the disassembly and analyze the flow. We're going to take a shortcut and translate the disassembly to x64 syntax, then utilize Ghidra's decomplier to reconstruct the high-level functions.

Translating the disassembly is pretty straightforward, and without too much overhead we get the following code, saved to `vm.S`:

```assembly
#define REG_RV      rax // '\u{0}'
#define REG_ARG0    rdi // '\u{1}'
#define REG_ARG1    rsi // '\u{2}'
#define REG_G       rbx
#define REG_H       r10
#define REG_x       r11
#define REG_y       r12
#define REG_B       r13
#define REG_i       r14
#define REG_TMP     r15


.intel_syntax noprefix
.global run_vm

run_vm:
    mov REG_i, 0            # 0x000:  mov 'i', 0x00000000
    mov REG_ARG0, REG_i     # 0x006:  mov '\u{1}', 'i'
    call func_bc            # 0x009:  call 0xBC
                            # 0x00E:  exit
    ret                     # 0x00F:  ret
    
func_10:
    push  REG_G             # 0x010:  push 'G'
    mov REG_G, REG_ARG0     # 0x012:  mov 'G', '\u{1}'
    mov REG_i, 0            # 0x015:  mov 'i', 0x00000000
    cmp REG_G, REG_i        # 0x01B:  test 'G', 1, 'i'
    jg lab_30               # 0x01F:  jmpc 0x30
    mov REG_i, 0            # 0x024:  mov 'i', 0x00000000
    mov REG_RV, REG_i       # 0x02A:  mov '\u{0}', 'i'
    pop REG_G               # 0x02D:  pop 'G'
    ret                     # 0x02F:  ret
    lab_30:
        mov REG_i, 0x100    # 0x030:  mov 'i', 0x00000100
        cmp REG_G, REG_i    # 0x036:  test 'G', 0, 'i'
        jge lab_45          # 0x03A:  jmpc 0x45
        mov REG_RV, REG_G   # 0x03F:  mov '\u{0}', 'G'
        pop REG_G           # 0x042:  pop 'G'
        ret                 # 0x044:  ret
    lab_45:
        mov REG_i, 0x100    # 0x045:  mov 'i', 0x00000100
        mov REG_TMP, REG_G  # 0x04B:  'i' = alu('G', 4, 'i')
        sub REG_TMP, REG_i
        mov REG_i, REG_TMP
        mov REG_ARG0, REG_i # 0x050:  mov '\u{1}', 'i'
        call func_10        # 0x053:  call 0x10
        pop REG_G           # 0x058:  pop 'G'
        ret                 # 0x05A:  ret
        pop REG_G           # 0x05B:  pop 'G'
        ret                 # 0x05D:  ret
    
func_5e:
    push REG_G              # 0x05E:  push 'G'
    mov REG_G, REG_ARG0     # 0x060:  mov 'G', '\u{1}'
    mov REG_i, 0x2          # 0x063:  mov 'i', 0x00000002
    cmp REG_G, REG_i        # 0x069:  test 'G', 1, 'i'
    jg lab_7e               # 0x06D:  jmpc 0x7E
    mov REG_i, 1            # 0x072:  mov 'i', 0x00000001
    mov REG_RV, REG_i       # 0x078:  mov '\u{0}', 'i'
    pop REG_G               # 0x07B:  pop 'G'
    ret                     # 0x07D:  ret
    lab_7e:
        mov REG_i, 1        # 0x07E:  mov 'i', 0x00000001
        mov REG_TMP, REG_G  # 0x084:  'i' = alu('G', 4, 'i')
        sub REG_TMP, REG_i
        mov REG_i, REG_TMP
        mov REG_ARG0, REG_i # 0x089:  mov '\u{1}', 'i'
        call func_5e        # 0x08C:  call 0x5E
        mov REG_x, REG_RV   # 0x091:  mov 'x', '\u{0}'
        push REG_x          # 0x094:  push 'x'
        mov REG_i, 2        # 0x096:  mov 'i', 0x00000002
        mov REG_TMP, REG_G  # 0x09C:  'i' = alu('G', 4, 'i')
        sub REG_TMP, REG_i
        mov REG_i, REG_TMP
        mov REG_ARG0, REG_i # 0x0A1:  mov '\u{1}', 'i'
        call func_5e        # 0x0A4:  call 0x5E
        pop REG_x           # 0x0A9:  pop 'x'
        mov REG_y, REG_RV   # 0x0AB:  mov 'y', '\u{0}'
        mov REG_TMP, REG_x  # 0x0AE:  'i' = alu('x', 3, 'y')
        add REG_TMP, REG_y
        mov REG_i, REG_TMP
        mov REG_RV, REG_i   # 0x0B3:  mov '\u{0}', 'i'
        pop REG_G           # 0x0B6:  pop 'G'
        ret                 # 0x0B8:  ret
        pop REG_G           # 0x0B9:  pop 'G'
        ret                 # 0x0BB:  ret

func_bc:
    push REG_G              # 0x0BC:  push 'G'
    mov REG_G, REG_ARG0     # 0x0BE:  mov 'G', '\u{1}'
    mov REG_ARG0, REG_B     # 0x0C1:  mov '\u{1}', 'B'
    call Strlen             # 0x0C4:  call Strlen
    cmp REG_G, REG_RV       # 0x0C5:  test 'G', 0, '\u{0}'
    jge lab_12f             # 0x0C9:  jmpc 0x12F
    mov REG_ARG0, REG_B     # 0x0CE:  mov '\u{1}', 'B'
    mov REG_ARG1, REG_G     # 0x0D1:  mov '\u{2}', 'G'
    call charAt             # 0x0D4:  call CharAt
    mov REG_H, REG_RV       # 0x0D5:  mov 'H', '\u{0}'
    push REG_H              # 0x0D8:  push 'H'
    push REG_H              # 0x0DA:  push 'H'
    mov REG_i, 1            # 0x0DC:  mov 'i', 0x00000001
    mov REG_TMP, REG_G      # 0x0E2:  'i' = alu('G', 3, 'i')
    add REG_TMP, REG_i
    mov REG_i, REG_TMP
    mov REG_ARG0, REG_i     # 0x0E7:  mov '\u{1}', 'i'
    call func_5e            # 0x0EA:  call 0x5E
    pop REG_H               # 0x0EF:  pop 'H'
    mov REG_TMP, REG_G      # 0x0F1:  'i' = alu('G', 3, '\u{0}')
    add REG_TMP, REG_RV
    mov REG_i, REG_TMP
    mov REG_TMP, REG_H      # 0x0F6:  'i' = alu('H', 3, 'i')
    add REG_TMP, REG_i
    mov REG_i, REG_TMP
    mov REG_ARG0, REG_i     # 0x0FB:  mov '\u{1}', 'i'
    call func_10            # 0x0FE:  call 0x10
    pop REG_H               # 0x103:  pop 'H'
    mov REG_H, REG_RV       # 0x105:  mov 'H', '\u{0}'
    push REG_H              # 0x108:  push 'H'
    push REG_H              # 0x10A:  push 'H'
    mov REG_ARG0, REG_H     # 0x10C:  mov '\u{1}', 'H'
    call Print              # 0x10F:  call Print
    pop REG_H               # 0x110:  pop 'H'
    pop REG_H               # 0x112:  pop 'H'
    push REG_H              # 0x114:  push 'H'
    push REG_H              # 0x116:  push 'H'
    mov REG_i, 1            # 0x118:  mov 'i', 0x00000001
    mov REG_TMP, REG_G      # 0x11E:  'i' = alu('G', 3, 'i')
    add REG_TMP, REG_i
    mov REG_i, REG_TMP
    mov REG_ARG0, REG_i     # 0x123:  mov '\u{1}', 'i'
    call func_bc            # 0x126:  call 0xBC
    pop REG_H               # 0x12B:  pop 'H'
    pop REG_H               # 0x12D:  pop 'H'
    lab_12f:
        pop REG_G           # 0x12F:  pop 'G'
        ret                 # 0x131:  ret
```

We also create `vm_main.c` with implementation for the named functions:

```c
#include <stdio.h>
#include <stdint.h>

uint8_t input_data[] = {
    66, 82, 66, 117, 75, 91, 86, 87, 31, 51, 222, 187, 112, 236, 9, 98, 34, 69, 0, 198, 150, 29,
    96, 10, 69, 26, 253, 225, 164, 8, 110, 67, 102, 108, 103, 162, 209, 1, 173, 130, 186, 5, 123,
    109, 187, 215, 86, 232, 23, 215, 184, 79, 171, 232, 128, 67, 138, 153, 251, 92, 4, 94, 93,
};

void run_vm();

uint64_t Strlen() {
    return sizeof(input_data);
}

uint64_t charAt(uint64_t dummy, uint64_t index) {
    return input_data[index];
}

void Print(uint64_t c) {
    printf("%c", c);
}

int main(int argc, char* argv[])
{
    setbuf(stdout, NULL);
    run_vm();
    return 0;
}
```

The main thing we have to keep in mind when we map x64 registers to the VM registers is to make sure that the registers used to send arguments to the named functions (e.g. `charAt`, `Print`) are indeed the registers that will be used by `gcc` when compiling the program. According to the convention, `rdi` and `rsi` are used as the first two arguments to functions, so they are mapped to `REG_ARG0` and `REG_ARG1`. Obviously the return value (`REG_RV`) is mapped to `rax`. The rest of the mapping doesn't really matter.


For convenience, we also create a `Makefile`:

```makefile
CC = gcc

all: solve

clean:
	rm -f *.o

vm.o: vm.S
	$(CC) -masm=intel -c vm.S -o vm.o

vm_main.o: vm_main.c
	$(CC) -c vm_main.c -o vm_main.o

solve: vm.o vm_main.o
	$(CC) vm.o vm_main.o -o solve
```

Let's run it:

```console
┌──(user@kali)-[/media/sf_CTFs/google/18_Unknown_-_Master_Office]
└─$ make
gcc -masm=intel -c vm.S -o vm.o
gcc -c vm_main.c -o vm_main.o
gcc vm.o vm_main.o -o solve

┌──(user@kali)-[/media/sf_CTFs/google/18_Unknown_-_Master_Office]
└─$ ./solve
CTF{ThisIsAVeryLongFlagAndYouMightRunOutzsh: segmentation fault  ./solve
```

So far, so good. Now we open it in Ghidra:

```c
void run_vm(void)
{
  func_bc(0);
  return;
}

void func_bc(long param_1)
{
  long lVar1;
  long lVar2;
  undefined8 unaff_R13;
  
  lVar1 = Strlen();
  if (param_1 < lVar1) {
    lVar1 = charAt(unaff_R13,param_1);
    lVar2 = func_5e(param_1 + 1);
    lVar1 = func_10(lVar1 + param_1 + lVar2);
    Print(lVar1);
    func_bc(param_1 + 1);
  }
  return;
}

long func_5e(long param_1)
{
  long lVar1;
  long lVar2;
  
  if (param_1 < 3) {
    return 1;
  }
  lVar1 = func_5e(param_1 + -1);
  lVar2 = func_5e(param_1 + -2);
  return lVar1 + lVar2;
}

long func_10(long param_1)
{
  long lVar1;
  
  if (param_1 < 1) {
    return 0;
  }
  if (param_1 < 0x100) {
    return param_1;
  }
  lVar1 = func_10(param_1 + -0x100);
  return lVar1;
}
```

This is much better. So `func_5e` is used to calculate the fibonacci series and `func_10` is an expensive way of performing modulus. Let's re-implement these functions in an efficient manner:

```c
#include <stdio.h>
#include <stdint.h>

uint8_t input_data[] = 
{
    66, 82, 66, 117, 75, 91, 86, 87, 31, 51, 222, 187, 112, 236, 9, 98, 34, 69, 0, 198, 150, 29,
    96, 10, 69, 26, 253, 225, 164, 8, 110, 67, 102, 108, 103, 162, 209, 1, 173, 130, 186, 5, 123,
    109, 187, 215, 86, 232, 23, 215, 184, 79, 171, 232, 128, 67, 138, 153, 251, 92, 4, 94, 93,
};


uint64_t Strlen() 
{
    return sizeof(input_data);
}

uint64_t charAt(uint64_t dummy, uint64_t index)
{
    return input_data[index];
}

void Print(uint64_t c)
{
    printf("%c", c);
}

long fibonacci(long n)
{
    int  i;
    long a;
    long b;
    long c;
    
    if (n < 2)
    {
        return 1;
    }
    
    a = 1;
    b = 1;
    for (i = 2; i < n; i++)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

long mod(long n)
{
    return n % 0x100;
}

void decode(long index)
{
    long lVar1;
    long lVar2;
    long length;
    
    length = Strlen();
    while (index < length) 
    {
        lVar1 = charAt(0, index);
        lVar2 = fibonacci(index + 1);
        lVar1 = mod(lVar1 + index + lVar2);
        Print(lVar1);
        index += 1;
    }
}

void run_vm(void)
{
    decode(0);
    return;
}

int main(int argc, char* argv[])
{
    setbuf(stdout, NULL);
    run_vm();
    return 0;
}
```

We run and get the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/google/18_Unknown_-_Master_Office]
└─$ gcc vm_efficient.c -o vm_efficient && ./vm_efficient
CTF{ThisIsAVeryLongFlagAndYouMightRunOutOfJuiceWhileDecodingIt}
```