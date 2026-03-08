# simulated-computer
******************************************************************************

CS 4348 - Project 1 : Simulated Computer
Author: krk

This project simulates the hardware of a simplified computer in C. It is made
up of 4 files that each represent a component of the computer:

 - memory.c : The RAM of the computer. It is a 2d int array with dimensions
   1024x2. Position 0 of the second dimension stores the OP Code, and
   position 1 stores the argument.

 - cpu.c : The CPU of the computer. It holds the registers (Base, PC, IR0,
   IR1, AC, MAR, MBR) and contains the logic for fetching and executing
   instructions.

 - disk.c : The disk loader of the computer. It reads a program file,
   translates each instruction into its integer OP Code, and writes it
   into memory.

 - main.c : The main driver. It initializes everything, loads the program,
   runs the clock cycle loop, and prints the first 20 memory locations
   when the program exits.

******************************************************************************

How to Compile:

 gcc main.c cpu.c memory.c disk.c -o computer

How to Run:

 ./computer                      (uses default "program.txt" in same directory)
 ./computer /path/to/program.txt (pass a custom program file path)

******************************************************************************

Program File Format:

 The program file is a plain text file. Each line is one instruction.
 Lines starting with // are comments and are ignored.
 Each instruction is a command, optionally followed by a space and an integer
 argument.

 Example:
   // Multiply 3 * 5
   load_const 3
   move_to_mbr
   load_const 5
   multiply
   exit

******************************************************************************

Supported Instructions:

 - exit           : Ends the program
 - load_const int : Loads int into the AC register
 - move_from_mbr  : Moves MBR into AC
 - move_from_mar  : Moves MAR into AC
 - move_to_mbr    : Moves AC into MBR
 - move_to_mar    : Moves AC into MAR
 - load_at_addr   : Loads memory at address MAR into MBR
 - write_at_addr  : Writes MBR into memory at address MAR
 - add            : AC = AC + MBR
 - multiply       : AC = AC * MBR
 - and            : AC = AC && MBR (logical)
 - or             : AC = AC || MBR (logical)
 - ifgo addr      : If AC != 0, jump to addr
 - sleep          : Do nothing
 - //             : Comment, line is ignored

******************************************************************************