#ifndef FILA
#define FILA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define MAX 1000

typedef struct TypeItem {
    int process_table_index;
    int *priority;
} TypeItem;

// typedef struct TipoCelula  *TipoApontadorCelulaFila;
typedef struct Cell {
    TypeItem item;
    int index;
    struct Cell *next_cell; // Ponteiro para a próxima célula
} Cell;

typedef struct TypeFila {
    Cell *start, *end;
} TypeFila;

void initialize_empty_fila(TypeFila *fila);
int is_fila_empty(TypeFila fila);
void add_item_to_fila(TypeItem x, TypeFila *fila);
int remove_item_from_fila(TypeFila *fila);
void show_fila(TypeFila *fila);
void sort_fila_by_priority(TypeFila *fila);

#endif

// ok