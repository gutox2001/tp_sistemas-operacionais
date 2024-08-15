#ifndef CPU_H
#define CPU_H

#define QUANT_CPU 2

#include "memory.h"
#include "process_table.h"

typedef struct CPU {
    int program_counter;  // Contador de programa da CPU
    Memory *memory;
    int index_mem_init; // Índice de início da memória
    int index_mem_end;  // Índice de fim da memória
    int quantum_time;  // Tempo de fatia de tempo
    int used_time;     // Tempo de unidades de tempo usadas
    int quant_int;     // Quantidade de inteiros usados pelo processo
    ItemProcess *actual_process;
} CPU;

void initialize_cpu(CPU *cpu, int quantum_time, Memory *memory, int index_mem_init, int index_mem_end) ;
int is_cpu_empty(CPU *cpu);
void add_process_to_cpu(CPU *cpu, ItemProcess *process);
void clean_cpu(CPU *cpu);
void show_cpu(CPU cpu);

#endif //CPU_H
