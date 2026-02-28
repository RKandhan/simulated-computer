/******************************************************************************

This is the memory(RAM) of my computer. It is a 2d int array with dimensions
1024x2.
It has the following functions:
 
 - int* mem_read(int addr) : It is a integer pointer used to read from memory 
 at location addr and return the data
 
  - void mem_write(int addr, int* data): Used to write the given data into 
 memory at the location given by addr.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

#define MEMORY_SIZE 1024
#define DATA_SIZE 2

static int memory[MEMORY_SIZE][DATA_SIZE]; // 2D array representing the memory

int* mem_read(int addr) {
    if (addr < 0 || addr >= MEMORY_SIZE) {
        fprintf(stderr, "Error: Address: %d out of bounds\n", addr);
        return NULL; // Return NULL for invalid address
    }
    return memory[addr]; // Return pointer to the data at the specified address
}

void mem_write(int addr, int* data) {
    if (addr < 0 || addr >= MEMORY_SIZE) {
        fprintf(stderr, "Error: Address: %d out of bounds\n", addr);
        return; // Do nothing for invalid address
    }
    if (data == NULL) {
        fprintf(stderr, "Error: Data pointer is NULL\n");
        return; // Do nothing for NULL data pointer
    }
    for (int i = 0; i < DATA_SIZE; i++) {
        memory[addr][i] = data[i]; // Write data to the specified address
    }
}