#ifndef PROCESS_MANAGER
#define PROCESS_MANAGER

#include "cpu.h"
#include "fila.h"
#include "process_table.h"

typedef struct ProcessManager {
    TypeFila ReadyState;
    TypeFila BlockedState;
    TypeFila ExecutionState;

    ProcessTable process_table;

    CPU *cpu_list;

    /* data */
} ProcessManager;

void initialize_process_manager(ProcessManager *process_manager, CPU *cpu_list);

void run_process(ProcessManager *process_manager);

#endif