#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "process_table.h"

typedef struct last{
    int lastfit;
}last;

typedef struct vector{
    int endressAdress[300];
}alocationVector;

int *first_fit(Memory *memory, int sizeneeded);
int *next_fit(Memory *memory, int sizeneeded, last *lastfit);
int *best_fit(Memory *memory, int sizeneeded);
int *worst_fit(Memory *memory, int sizeneeded);
int alocation_manager(Memory *mem, ItemProcess process, alocationVector *alocvect);