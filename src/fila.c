#include "../libs/fila.h"

void initialize_empty_fila(TypeFila *fila) {
    fila->start = (Cell *)malloc(sizeof(Cell));
    if (fila->start == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a fila.\n");
        exit(EXIT_FAILURE);
    }
    fila->end = fila->start;
    fila->start->next_cell = NULL;
}

int is_fila_empty(TypeFila fila) {
    return (fila.start == fila.end);
}

void add_item_to_fila(TypeItem new_item, TypeFila *fila) {
    Cell *new_cell, *temp_current_cell, *temp_next_cell;
    
    new_cell = (Cell *)malloc(sizeof(Cell));
    if (new_cell == NULL) {
        fprintf(stderr, "Erro ao alocar memória para nova célula.\n");
        exit(EXIT_FAILURE);
    }

    if (is_fila_empty(*fila)) {
        new_cell->index = 0;
    } else {
        new_cell->index = fila->end->index + 1;
    }
    
    new_cell->item = new_item;
    new_cell->next_cell = NULL;

    temp_current_cell = fila->start;
    temp_next_cell = fila->start->next_cell; // Corrigido para iniciar do primeiro item válido

    while (temp_next_cell != NULL && temp_next_cell->item.priority < new_item.priority) {
        temp_current_cell = temp_next_cell;
        temp_next_cell = temp_next_cell->next_cell;
    }

    new_cell->next_cell = temp_next_cell;
    temp_current_cell->next_cell = new_cell;

    if(temp_next_cell == NULL) {
        fila->end = new_cell; // Corrigido para apontar para a nova célula adicionada
    }

    printf("Item adicionado na fila com sucesso! Índice na fila: %d\n", fila->end->index);
}

int remove_item_from_fila(TypeFila *fila) {
    if (is_fila_empty(*fila)) {
        printf("Erro: a fila está vazia.\n");
        return -1;
    }

    Cell *aux = fila->start->next_cell;
    if (aux == NULL) {
        return -1;
    }

    fila->start->next_cell = aux->next_cell;
    int index = aux->item.process_table_index;
    free(aux);

    if (fila->start->next_cell == NULL) {
        fila->end = fila->start;
    }

    return index;
}

void show_fila(TypeFila *fila) {
    printf("Imprimindo fila...\n");
    Cell *aux = fila->start->next_cell; // Corrigido para começar a partir do primeiro item válido

    if (is_fila_empty(*fila)) {
        printf("Essa fila está vazia!\n");
        return;
    }

    while (aux != NULL) {
        printf("Dados do Item:\n");
        printf("Índice na fila: %d\n", aux->index);
        printf("Índice na tabela de processos: %d\n", aux->item.process_table_index);
        printf("Prioridade: %d\n", *aux->item.priority);
        aux = aux->next_cell;
    }
}

void sort_fila_by_priority(TypeFila *fila) {
    Cell *current_cell, *next_cell;
    TypeItem temp_item;
    int temp_index;

    for (current_cell = fila->start->next_cell; current_cell != NULL; current_cell = current_cell->next_cell) {
        for (next_cell = current_cell->next_cell; next_cell != NULL; next_cell = next_cell->next_cell) {
            if (*current_cell->item.priority > *next_cell->item.priority) {
                temp_item = current_cell->item;
                current_cell->item = next_cell->item;
                next_cell->item = temp_item;

                temp_index = current_cell->index;
                current_cell->index = next_cell->index;
                next_cell->index = temp_index;
            }
        }
    }
}