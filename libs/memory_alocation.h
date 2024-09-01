#ifndef MEMORY_ALOCATION_H
#define MEMORY_ALOCATION_H

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "process_table.h"
#include "fila.h"

typedef struct last {
    int lastfit;
} last;

int *first_fit(Memory *memory, int sizeneeded, alocationVector *alocvect);
int *next_fit(Memory *memory, int sizeneeded, last *lastfit, alocationVector *alocvect);
int *best_fit(Memory *memory, int sizeneeded, alocationVector *alocvect);
int *worst_fit(Memory *memory, int sizeneeded, alocationVector *alocvect);
int alocation_manager(Memory *mem, ItemProcess process, alocationVector *alocvect, last *ult,int type_alocacao, ProcessTable *process_table, TypeFila *fila_prontos, TypeFila *fila_bloqueados, TypeFila *fila_execucao);
int deallocation_manager(Memory *mem, int process_id, alocationVector *alocvect);
double calculate_average_allocation_time(int num_allocations);
void write_process_to_file(int process_id, int program_counter, int int_quantity, int *memory_vector);
void escreverNoArquivo(const char *conteudo);

#endif // MEMORY_ALOCATION_H