/******************************************************************************

This is the main driver of my simulated computer. It initializes the system,
loads a program from disk into memory at address 4, runs the CPU clock cycle
until the program exits, and then prints the first 20 memory locations.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "cpu.h"
#include "disk.h"

#define PROGRAM_ADDR 4           // Per spec: load program starting at address 4
#define PROGRAM_FILE "program.txt" // Hard-coded program file path

// print_memory
/*  Prints the first 20 memory locations in the format:
    [addr]: opcode  arg                                                        */
void print_memory(int count) {
    printf("\n=== Memory Dump (first %d locations) ===\n", count);
    printf("%-8s %-8s %-8s\n", "Addr", "OpCode", "Arg");
    printf("-----------------------------\n");
    for (int i = 0; i < count; i++) {
        int *data = mem_read(i);
        if (data != NULL) {
            printf("%-8d %-8d %-8d\n", i, data[0], data[1]);
        }
    }
    printf("=============================\n");
}

// main
int main(int argc, char *argv[]) {
    // Allow an optional command-line override for the program file
    char *fname = PROGRAM_FILE;
    if (argc == 2) {
        fname = argv[1];
    }

    printf("Starting simulated computer...\n");
    printf("Loading program '%s' into memory at address %d.\n\n", fname, PROGRAM_ADDR);

    // 1. Load the program from disk into memory
    load_prog(fname, PROGRAM_ADDR);

    // 2. Point the CPU's Base register at the program start
    cpu_set_base(PROGRAM_ADDR);

    // 3. Run clock cycles until the exit instruction is encountered
    int running = 1;
    while (running) {
        running = clock_cycle();
    }

    printf("\nProgram exited normally.\n");

    // 4. Print the first 20 memory locations
    print_memory(20);

    return 0;
}