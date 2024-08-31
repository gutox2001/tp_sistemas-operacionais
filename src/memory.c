#include "../libs/memory.h"
#include "../libs/memory_alocation.h"

void initialize_memory(Memory *memory, alocationVector *vect) {
    for (int i = 0; i < MAX_TAM; i++) {
        memory->data[i] = -1;
    }

    for (int i = 0; i < MAX_TAM * 3; i++){
        vect->endressAdress[i]=-1;
    }
}