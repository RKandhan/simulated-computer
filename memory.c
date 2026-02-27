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