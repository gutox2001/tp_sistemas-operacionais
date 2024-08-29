#include "../libs/fila.h"

void initialize_empty_fila(TypeFila *fila) {
    // fila->start = (Cell *)malloc(sizeof(Cell));
    // if (fila->start == NULL) {
    //     fprintf(stderr, "Erro ao alocar memória para a fila.\n");
    //     exit(EXIT_FAILURE);
    // }
    fila->start = NULL;
    fila->end = NULL;
}

int is_fila_empty(TypeFila fila) {
    return (fila.start == NULL && fila.end == NULL);
}

void add_item_to_fila(TypeItem new_item, TypeFila *fila) {
    Cell *new_cell = (Cell *)malloc(sizeof(Cell));

    if (new_cell == NULL) {
        fprintf(stderr, "Erro ao alocar memória para nova célula.\n");
        exit(EXIT_FAILURE);
    }

    new_cell->item = new_item;
    new_cell->next_cell = NULL;

    if (is_fila_empty(*fila)) {
        // Fila vazia, nova célula será o primeiro e o último elemento
        new_cell->index = 0;
        fila->start = fila->end = new_cell;
        printf("Fila está vazia, item será adicionado na primeira posição. Índice na fila: %d\n", new_cell->index);
    } else {
        // Inserção no final da fila
        new_cell->index = fila->end->index + 1;
        fila->end->next_cell = new_cell;
        fila->end = new_cell;
        printf("Item adicionado ao final da fila. Índice na fila: %d\n", new_cell->index);
    }
}

int remove_item_from_fila(TypeFila *fila) {
    if (is_fila_empty(*fila)) {
        printf("Erro: a fila está vazia.\n");
        return -1;
    }

    Cell *aux = fila->start;
    fila->start = fila->start->next_cell;

    int index = aux->item.process_table_index;
    free(aux);

    if (fila->start == NULL) {  // Se a fila ficou vazia após a remoção
        fila->end = NULL;
    }

    return index;
}


void show_fila(TypeFila *fila) {
    if (is_fila_empty(*fila)) {
        printf("Essa fila está vazia!\n");
        return;
    }

    Cell *aux = fila->start;
    printf("\nImprimindo fila...");

    while (aux != NULL) {
        printf("Dados do Item:\n");
        printf("Índice na fila: %d\n", aux->index);
        printf("Índice na tabela de processos: %d\n", aux->item.process_table_index);
        printf("Prioridade: %d\n", *aux->item.priority);
        aux = aux->next_cell;
    }
}

void sort_fila_by_priority(TypeFila *fila) {
    if (is_fila_empty(*fila)) {
        printf("A fila está vazia, não é necessário ordenar.\n");
        return;
    }

    Cell *current_cell, *next_cell;
    TypeItem temp_item;
    int swapped;

    // Loop externo que continua enquanto ocorrem trocas
    do {
        swapped = 0;
        current_cell = fila->start;

        // Loop interno para comparar elementos adjacentes
        while (current_cell->next_cell != NULL) {
            next_cell = current_cell->next_cell;

            // Comparação de prioridades
            if (*(current_cell->item.priority) > *(next_cell->item.priority)) {
                // Troca de itens
                temp_item = current_cell->item;
                current_cell->item = next_cell->item;
                next_cell->item = temp_item;
                swapped = 1;
            }
            current_cell = next_cell;
        }
    } while (swapped);
}
