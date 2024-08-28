#ifndef CPU_H
#define CPU_H

#define QUANT_CPU 1

#include "memory.h"
#include "process_table.h"
#include "colors.h"

typedef struct CPU {
    int program_counter;  // Contador de programa da CPU
    Memory *memory;
    int index_mem_init; // Índice de início da memória
    int index_mem_end;  // Índice de fim da memória
    int quantum;  // Quantidade de fatias de tempo usadas pelo processo
    int used_time;     // Quantidade de fatias de tempo da CPU
    int quant_int;     // Quantidade de inteiros usados pelo processo
    ItemProcess *actual_process;
} CPU;

void initialize_cpu(CPU *cpu, Memory *memory, int index_mem_init, int index_mem_end) ;
int is_cpu_empty(CPU *cpu);
void add_process_to_cpu(CPU *cpu, ItemProcess *process);
void clean_cpu(CPU *cpu);
void show_cpu(CPU cpu);

int run_instruction(CPU *cpu, int mem_index, char instruction, int value);

#endif //CPU_H
