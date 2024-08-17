#ifndef SIMULATED_PROCESS
#define SIMULATED_PROCESS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TAM_ARQ_NAME 100

typedef struct Instruction {
    int index;
    int value;
    char arq_name[MAX_TAM_ARQ_NAME]; 
    char instruction_char;
} Instruction;

typedef struct SimulatedProcess {
    int int_quantity;
    pid_t process_id;
    int* memory;   // Ponteiro para vetor de inteiros que corresponde à memória
    int program_counter;  // contador de instrucao do processo
    int instruction_quantity;
    int priority;
    Instruction *process_instructions; // vetor dinâmico de instruções
} SimulatedProcess;

SimulatedProcess initialize_simulated_process(char* arq_name);
int count_arq_instructions(char* arq_name);
int set_process_instructions(char* arq_name, Instruction** process_instructions, int* int_quantity);
void show_simulated_process(SimulatedProcess processo);

#endif  // SIMULATED_PROCESS

// ok