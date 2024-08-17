#ifndef PROCESS_MANAGER
#define PROCESS_MANAGER

#include "cpu.h"
#include "fila.h"
#include "process_table.h"

typedef struct ProcessManager {
    TypeFila ReadyState;
    TypeFila BlockedState;
    pid_t ExecutionState[QUANT_CPU];

    ProcessTable process_table;

    CPU *cpu_list;

    /* data */
} ProcessManager;

#define TIME_SLICE_PRIORITY_0 1
#define TIME_SLICE_PRIORITY_1 2
#define TIME_SLICE_PRIORITY_2 4
#define TIME_SLICE_PRIORITY_3 8


void initialize_process_manager(ProcessManager *process_manager, CPU *cpu_list);

void troca_de_contexto(CPU *cpu, ProcessTable *process_table, int new_process_index, State new_state);

void escalona_by_priority(ProcessManager *process_manager, CPU *cpu);

// void escalona_fi

void run_process(ProcessManager *process_manager, ItemProcess *process, const char *input_command_string);

void sort_fila_by_priority(TypeFila *fila);

void verify_if_process_is_waiting(ProcessManager *process_manager);

int is_any_cpu_available(ProcessManager *process_manager);

// Função auxiliar para determinar a fatia de tempo com base na prioridade
int get_time_slice_by_priority(int priority);

#endif