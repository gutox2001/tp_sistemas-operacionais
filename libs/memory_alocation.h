#ifndef MEMORY_ALOCATION_H
#define MEMORY_ALOCATION_H

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "process_table.h"

typedef struct last {
    int lastfit;
} last;

int *first_fit(Memory *memory, int sizeneeded);
int *next_fit(Memory *memory, int sizeneeded, last *lastfit);
int *best_fit(Memory *memory, int sizeneeded);
int *worst_fit(Memory *memory, int sizeneeded);
int alocation_manager(Memory *mem, ItemProcess process, alocationVector *alocvect, last *ult);
int deallocation_manager(Memory *mem, int process_id, alocationVector *alocvect);
double calculate_average_allocation_time(int num_allocations);
#endif // MEMORY_ALOCATION_H