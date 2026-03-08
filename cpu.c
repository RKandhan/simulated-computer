/******************************************************************************

This is the CPU of my simulated computer. It holds the registers and contains
the logic for fetching and executing instructions.

Registers:
 - Base : Base Register - Stores the memory address of the currently executing
          program.
 - PC   : Program Counter - Stores the memory address (relative to Base) of
          the currently executing instruction.
 - IR0  : Instruction Register 0 - Stores the OP Code of the current
          instruction.
 - IR1  : Instruction Register 1 - Stores the argument of the current
          instruction (0 if none).
 - AC   : Accumulator Register - Stores results of instructions.
 - MAR  : Memory Address Register - Stores the memory address for read/write.
 - MBR  : Memory Buffer Register - Stores data to be read/written.

Functions:
 - void fetch_instruction(int addr) : Reads the instruction at addr into IR0/IR1
 - void execute_instruction()       : Executes the instruction in IR0/IR1
 - int  mem_address(int l_addr)     : Returns Base + l_addr (true memory address)
 - int  clock_cycle()               : Fetch + execute one instruction.
                                      Returns 0 on exit command, 1 otherwise.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "cpu.h"

/* ── Registers ────────────────────────────────────────────────────────────── */
static int Base = 0;   // Base register
static int PC   = 0;   // Program Counter
static int IR0  = 0;   // Instruction Register 0 (OP Code)
static int IR1  = 0;   // Instruction Register 1 (Argument)
static int AC   = 0;   // Accumulator
static int MAR  = 0;   // Memory Address Register
static int MBR  = 0;   // Memory Buffer Register

/* ── OP Code constants ────────────────────────────────────────────────────── */
#define OP_EXIT          0
#define OP_LOAD_CONST    1
#define OP_MOVE_FROM_MBR 2
#define OP_MOVE_FROM_MAR 3
#define OP_MOVE_TO_MBR   4
#define OP_MOVE_TO_MAR   5
#define OP_LOAD_AT_ADDR  6
#define OP_WRITE_AT_ADDR 7
#define OP_ADD           8
#define OP_MULTIPLY      9
#define OP_AND           10
#define OP_OR            11
#define OP_IFGO          12
#define OP_SLEEP         13

/* ── Public setter used by main.c to set the Base register ────────────────── */
void cpu_set_base(int base_addr) {
    Base = base_addr;
    PC   = 0;
}

/* ── mem_address ──────────────────────────────────────────────────────────── */
/*  Returns the true (absolute) memory address by adding Base to the local    */
/*  address l_addr.                                                            */
int mem_address(int l_addr) {
    return Base + l_addr;
}

/* ── fetch_instruction ────────────────────────────────────────────────────── */
/*  Reads the instruction stored at absolute address addr into IR0 (OP Code)  */
/*  and IR1 (argument).                                                        */
void fetch_instruction(int addr) {
    int *data = mem_read(addr);
    if (data == NULL) {
        fprintf(stderr, "Error: fetch_instruction failed at address %d\n", addr);
        IR0 = OP_EXIT;
        IR1 = 0;
        return;
    }
    IR0 = data[0]; // OP Code
    IR1 = data[1]; // Argument (0 if none)
}

/* ── execute_instruction ─────────────────────────────────────────────────── */
/*  Executes the instruction currently stored in IR0/IR1.                     */
void execute_instruction() {
    int *data;
    int  tmp[2];

    switch (IR0) {

        case OP_EXIT:
            /* Handled by clock_cycle() return value; nothing to do here. */
            break;

        case OP_LOAD_CONST:
            /* Load the integer argument into the AC register. */
            AC = IR1;
            break;

        case OP_MOVE_FROM_MBR:
            /* Copy MBR → AC. */
            AC = MBR;
            break;

        case OP_MOVE_FROM_MAR:
            /* Copy MAR → AC. */
            AC = MAR;
            break;

        case OP_MOVE_TO_MBR:
            /* Copy AC → MBR. */
            MBR = AC;
            break;

        case OP_MOVE_TO_MAR:
            /* Copy AC → MAR. */
            MAR = AC;
            break;

        case OP_LOAD_AT_ADDR:
            /* Read memory at address MAR; store the OP Code word into MBR. */
            data = mem_read(MAR);
            if (data == NULL) {
                fprintf(stderr, "Error: load_at_addr failed (MAR=%d)\n", MAR);
                break;
            }
            MBR = data[0]; // load the value stored at that address
            break;

        case OP_WRITE_AT_ADDR:
            /* Write MBR into memory at address MAR. */
            tmp[0] = MBR;
            tmp[1] = 0;
            mem_write(MAR, tmp);
            break;

        case OP_ADD:
            /* AC = AC + MBR */
            AC = AC + MBR;
            break;

        case OP_MULTIPLY:
            /* AC = AC * MBR */
            AC = AC * MBR;
            break;

        case OP_AND:
            /* Logical AND: 0 is False, non-zero is True. Result in AC. */
            AC = (AC != 0 && MBR != 0) ? 1 : 0;
            break;

        case OP_OR:
            /* Logical OR: 0 is False, non-zero is True. Result in AC. */
            AC = (AC != 0 || MBR != 0) ? 1 : 0;
            break;

        case OP_IFGO:
            /* If AC != 0, set PC to (addr - 1). The +1 from clock_cycle()
               will bring it to addr on the next cycle. */
            if (AC != 0) {
                PC = IR1 - 1;
            }
            break;

        case OP_SLEEP:
            /* Do nothing. */
            break;

        default:
            fprintf(stderr, "Error: Unknown OP Code %d at PC=%d\n", IR0, PC);
            break;
    }
}

/* ── clock_cycle ─────────────────────────────────────────────────────────── */
/*  Performs one full fetch-execute cycle.                                    */
/*  Returns 0 when the exit instruction is encountered, 1 otherwise.         */
int clock_cycle() {
    int abs_addr = mem_address(PC);

    fetch_instruction(abs_addr);
    execute_instruction();

    if (IR0 == OP_EXIT) {
        return 0; // Signal program termination
    }

    PC++; // Advance the program counter
    return 1;
}