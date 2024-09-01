#ifndef PROCESS_MANAGER
#define PROCESS_MANAGER

#include "cpu.h"
#include "fila.h"
#include "process_table.h"
#include "memory_alocation.h"

typedef struct ProcessManager {
    TypeFila ReadyState;
    TypeFila BlockedState;
    TypeFila ExecutionState;

    ProcessTable process_table;

    CPU *cpu_list;

    /* data */
} ProcessManager;

#define TIME_SLICE_PRIORITY_0 1
#define TIME_SLICE_PRIORITY_1 2
#define TIME_SLICE_PRIORITY_2 4
#define TIME_SLICE_PRIORITY_3 8

void initialize_process_manager(ProcessManager *process_manager, CPU *cpu_list);

void troca_de_contexto(ProcessManager *process_manager, CPU *cpu, State old_process_new_state, char *receive_string, int selected_escalonador, int *command_index);

void escalona_by_priority(ProcessManager *process_manager, CPU *cpu, char *receive_string, int *command_index);

void escalona_fcfs(ProcessManager *process_manager, CPU *cpu, char *receive_string, int *command_index, Memory *memory, alocationVector *alocvect, last *ult, int type_alocacao);

void run_selected_escalonador(ProcessManager *process_manager, CPU *cpu, char *receive_string, int selected_escalonador, int *command_index, Memory *memory, alocationVector *alocvect, last *ult, int type_alocacao);

//Os processos são executados de acordo a lista de processos em execução.
int run_command_in_selected_process(ProcessManager *process_manager, CPU *cpu, ItemProcess *process, char current_command, int selected_escalonador, char *input_command_string, int *command_index, alocationVector *vect, last *ult, int type_alocacao, Memory *memory);

void run_commands(ProcessManager *process_manager, char *input_command_string, int selected_escalonador, int *command_index, alocationVector *vect, last *ult, int type_alocacao, Memory *memory);

void sort_fila_by_priority(TypeFila *fila);

void verify_if_process_is_waiting(ProcessManager *process_manager);

int is_any_cpu_available(ProcessManager *process_manager);

// Função auxiliar para determinar a fatia de tempo com base na prioridade
int get_time_slice_by_priority(int priority);

void verify_process_block_time(ProcessManager *process_manager);

void verify_all_filas(ProcessManager *process_manager);

CPU* get_cpu_by_process_id(ProcessManager *process_manager, int process_id);

void show_process_manager(ProcessManager *process_manager);

#endif