#include "../libs/memory.h"

void initialize_memory(Memory *memory) {
    for (int i = 0; i < MAX_TAM; i++) {
        memory->data[i] = -1;
    }
}