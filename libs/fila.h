#ifndef FILA
#define FILA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define MAX 1000

typedef struct TypeItem {
    int index_process_table;
    int *priority;
} TypeItem;

// typedef struct TipoCelula  *TipoApontadorCelulaFila;
typedef struct Cell {
    TypeItem item;
    struct Cell *next_cell; // Ponteiro para a próxima célula
} Cell;

typedef struct TypeFila {
    char name[100];
    Cell *start, *end;
} TypeFila;

void initialize_empty_fila(TypeFila *fila, char *name);
int is_fila_empty(TypeFila fila);
void add_item_to_fila(TypeItem x, TypeFila *fila);
int remove_item_from_fila(TypeFila *fila);
void show_fila(TypeFila fila);

#endif

// ok