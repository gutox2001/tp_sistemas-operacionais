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

void show_memory(Memory memory) {
    printf(BOLD YELLOW "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n" RESET);
    printf(BOLD MAGENTA"\nEstado da memória:\n" RESET);
    for (int i = 0; i < MAX_TAM; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }

        printf(i < 10 ? "[0%d]: %-3d " : "[%d]: %-3d ", i, memory.data[i]);
    }
    printf(BOLD YELLOW "\n\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n" RESET);
    fflush(stdout);
}