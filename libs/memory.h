#ifndef MEMORY
#define MEMORY

#include "memory_alocation.h"

#define MAX_TAM 100

typedef struct Memory
{
    int data[MAX_TAM];
} Memory;

void initialize_memory(Memory *memory, alocationVector *vect);

#endif //MEMORY