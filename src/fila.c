#include "../libs/fila.h"

void initialize_empty_fila(TypeFila *fila, char *name) {
    // Inicializa a fila vazia
    fila->start = (Cell *)malloc(sizeof(Cell));
    if (fila->start == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a fila.\n");
        exit(EXIT_FAILURE);
    }
    fila->end = fila->start; // O início e o fim da fila apontam para a mesma célula inicialmente
    fila->start->next_cell = NULL; // A próxima célula é NULL, indicando que a fila está vazia
    strncpy(fila->name, name, sizeof(fila->name) - 1); // Copia o nome da fila
    fila->name[sizeof(fila->name) - 1] = '\0'; // Garante que a string esteja terminada
}

int is_fila_empty(TypeFila fila) {
    // Verifica se a fila está vazia
    return (fila.start == fila.end);
}

void add_item_to_fila(TypeItem new_item, TypeFila *fila) {
    // Adiciona um novo item à fila, mantendo a ordem por prioridade
    Cell *new_cell = (Cell *)malloc(sizeof(Cell));
    if (new_cell == NULL) {
        fprintf(stderr, "Erro ao alocar memória para nova célula.\n");
        exit(EXIT_FAILURE);
    }
    new_cell->item = new_item; // Armazena o novo item na nova célula
    new_cell->next_cell = NULL; // Inicializa a próxima célula como NULL

    // Busca a posição correta para inserir o novo item
    Cell *current = fila->start;
    Cell *previous = NULL;

    while (current != NULL && current->item.priority <= new_item.priority) {
        previous = current;
        current = current->next_cell;
    }

    // Insere o novo item na posição correta
    if (previous == NULL) {
        // Inserção no início da fila
        new_cell->next_cell = fila->start->next_cell;
        fila->start->next_cell = new_cell;
    } else {
        // Inserção após a célula anterior
        previous->next_cell = new_cell;
        new_cell->next_cell = current;
    }

    // Atualiza o final da fila se necessário
    if (current == NULL) {
        fila->end = new_cell; // Atualiza o fim da fila
    }
}

int remove_item_from_fila(TypeFila *fila) {
    // Remove e retorna o índice do item mais prioritário da fila
    if (is_fila_empty(*fila)) {
        printf("Erro: a fila está vazia.\n");
        return -1; // Retorna erro se a fila estiver vazia
    }

    Cell *aux = fila->start->next_cell; // O item a ser removido está após a célula de início
    if (aux == NULL) {
        return -1; // Caso não exista item para remover
    }

    fila->start->next_cell = aux->next_cell; // Avança o início da fila
    int index = aux->item.index_process_table; // Armazena o índice do item removido
    free(aux); // Libera a memória da célula removida

    // Se a fila estiver vazia após a remoção, atualiza o final
    if (fila->start->next_cell == NULL) {
        fila->end = fila->start; // Atualiza o fim da fila
    }

    return index; // Retorna o índice do item removido
}

void show_fila(TypeFila fila) {
    // Exibe os itens da fila
    printf("Imprimindo fila: %s\n", fila.name);
    Cell *aux = fila.start->next_cell; // Começa a partir da primeira célula

    if (is_fila_empty(fila))
    {
        printf("Essa fila está vazia!\n");
        return;
    }

    while (aux != NULL) {
        printf("Indice na tabela de processos: %d\n", aux->item.index_process_table);
        printf("Prioridade: %d\n", *aux->item.priority); // Acessa diretamente a prioridade
        aux = aux->next_cell; // Avança para a próxima célula
    }
}
