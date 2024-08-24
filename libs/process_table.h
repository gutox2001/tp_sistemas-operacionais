#ifndef PROCESS_TABLE
#define PROCESS_TABLE

#include "simulated_process.h"

#include <sys/types.h> // Para pid_t
#include <unistd.h> // Para getpid, pipe, fork, close, write, read
#include <stdio.h>
#include <stdlib.h>

#define MAX_TAM 100

typedef enum State {
    Bloqueado = 0, 
    Pronto = 1, 
    Execucao = 2
} State;

typedef struct ItemProcess {
    pid_t id; // ID do processo
    pid_t parent_id;
    int program_counter; 
    SimulatedProcess simulated_process;
    int priority; // Valores vão de 0 a 3, onde 0 é a maior prioridade e 3 é a menor
    State process_state;
    int start_time;
    int cpu_time;
} ItemProcess;

typedef struct {
  ItemProcess item_process[MAX_TAM];
  int first_item, last_item;
} ProcessTable;

void initialize_table(ProcessTable *process_table);
int is_empty(ProcessTable *process_table);
int add_process_to_table(ItemProcess new_item_process, ProcessTable *process_table);
void remove_process_from_table(int indexToRemove, ProcessTable *process_table);
void show_process_table(ProcessTable process_table);
void show_item_process(ItemProcess process);
int get_first_empty_position(ProcessTable *process_table);

void create_new_item_process(pid_t parent_id, int program_counter, SimulatedProcess simulated_process, int priority, ProcessTable *process_table);
void clear_item_process(ItemProcess *item_process);

#endif //PROCESS_TABLE
