/******************************************************************************

This is the disk loader of my simulated computer. It is responsible for
reading a program file from disk, translating each instruction into its
integer OP Code (and optional argument), and writing the result into memory.

Functions:
 - int* translate(char *instruction) : Parses one line of source text and
       returns a 2-element array: [OP Code, argument].  Argument is 0 when
       the instruction has none.  Returns NULL for comment lines (//).

 - void load_prog(char *fname, int addr) : Opens the file fname, iterates
       over each line, calls translate(), and writes the result into memory
       starting at address addr using mem_write().

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disk.h"
#include "memory.h"

/* Maximum length we expect for a single source line */
#define MAX_LINE 256

// translate
/*  Parses a single instruction line and returns a pointer to a static        
    2-element array [opcode, argument]. Returns NULL if the line is a 
    comment or blank (should be skipped). */
int* translate(char *instruction) {
    static int result[2]; // static so the pointer is valid after return

    result[0] = 0;
    result[1] = 0;

    if (instruction == NULL) return NULL;

    // Strip leading whitespace 
    while (*instruction == ' ' || *instruction == '\t') instruction++;

    // Skip blank lines
    if (*instruction == '\0' || *instruction == '\n') return NULL;

    // Skip comment lines
    if (strncmp(instruction, "//", 2) == 0) return NULL;

    // Pull out the command token
    char command[MAX_LINE];
    int  arg = 0;
    int  has_arg = 0;

    // Try to parse "command arg" or just "command"
    int parsed = sscanf(instruction, "%s %d", command, &arg);
    if (parsed == 2) has_arg = 1;

    // Match command string to OP Code
    if (strcmp(command, "exit") == 0) {
        result[0] = 0;

    } else if (strcmp(command, "load_const") == 0) {
        result[0] = 1;
        result[1] = has_arg ? arg : 0;

    } else if (strcmp(command, "move_from_mbr") == 0) {
        result[0] = 2;

    } else if (strcmp(command, "move_from_mar") == 0) {
        result[0] = 3;

    } else if (strcmp(command, "move_to_mbr") == 0) {
        result[0] = 4;

    } else if (strcmp(command, "move_to_mar") == 0) {
        result[0] = 5;

    } else if (strcmp(command, "load_at_addr") == 0) {
        result[0] = 6;

    } else if (strcmp(command, "write_at_addr") == 0) {
        result[0] = 7;

    } else if (strcmp(command, "add") == 0) {
        result[0] = 8;

    } else if (strcmp(command, "multiply") == 0) {
        result[0] = 9;

    } else if (strcmp(command, "and") == 0) {
        result[0] = 10;

    } else if (strcmp(command, "or") == 0) {
        result[0] = 11;

    } else if (strcmp(command, "ifgo") == 0) {
        result[0] = 12;
        result[1] = has_arg ? arg : 0;

    } else if (strcmp(command, "sleep") == 0) {
        result[0] = 13;

    } else {
        fprintf(stderr, "Warning: Unknown instruction '%s', skipping.\n", command);
        return NULL;
    }

    return result;
}

// load_prog
/*  Opens the program file fname, translates each instruction line, and
    writes it into memory starting at address addr. */
void load_prog(char *fname, int addr) {
    FILE *fp = fopen(fname, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'\n", fname);
        return;
    }

    char  line[MAX_LINE];
    int   current_addr = addr;

    while (fgets(line, sizeof(line), fp) != NULL) {
        // Strip trailing newline so translate() sees a clean string
        line[strcspn(line, "\n")] = '\0';

        int *encoded = translate(line);

        // NULL means blank line or comment — skip it
        if (encoded == NULL) continue;

        mem_write(current_addr, encoded);
        current_addr++;
    }

    fclose(fp);
}