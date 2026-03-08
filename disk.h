#ifndef CPU_H
#define CPU_H

void cpu_set_base(int base_addr);
void fetch_instruction(int addr);
void execute_instruction();
int  mem_address(int l_addr);
int  clock_cycle();

#endif