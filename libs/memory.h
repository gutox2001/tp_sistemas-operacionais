#ifndef MEMORY
#define MEMORY

#define MAX_TAM 100

typedef struct Memory
{
    int data[MAX_TAM];
} Memory;

void initialize_memory(Memory *memory);

#endif //MEMORY