#ifndef MEMORY_H
#define MEMORY_H

#define MAX_TAM 100

#include "colors.h"

typedef struct Memory
{
    int data[MAX_TAM];
} Memory;

typedef struct vector {
    int endressAdress[300];
} alocationVector;

void initialize_memory(Memory *memory, alocationVector *vect);

void show_memory(Memory memory);

#endif // MEMORY_H