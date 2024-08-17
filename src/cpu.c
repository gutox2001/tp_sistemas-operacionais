#include "../libs/cpu.h"

void initialize_cpu(CPU *cpu, Memory *memory, int index_mem_init, int index_mem_end) {
    cpu->actual_process = NULL;
    cpu->memory = memory;
    cpu->program_counter = 0;
    cpu->quant_int = 0;
    cpu->quantum = 0;
    cpu->used_time = 0;
    cpu->index_mem_init = index_mem_init;
    cpu->index_mem_end = index_mem_end;
}

void add_process_to_cpu(CPU *cpu, ItemProcess *process) {
    cpu->actual_process = process;
    cpu->quant_int = process->simulated_process.int_quantity;
    cpu->program_counter = process->simulated_process.program_counter;

    printf("Processo de ID %d adicionado da CPU.\n", cpu->actual_process->simulated_process.process_id);
}

int is_cpu_empty(CPU *cpu) {
    if (cpu->actual_process == NULL) {
        return 1;
    }

    return 0;
}

void clean_cpu(CPU *cpu) {
    int temp_id = 0;

    // Limpando a memória usada
    for (size_t i = 0; i < cpu->quant_int; i++) {
        int memory_index = (cpu->actual_process->simulated_process.process_instructions[i].index + cpu->index_mem_init);
        // É adicionado o valor 0 da memória da CPU para que os índices sejam corrigidos para limpar os dados

        cpu->memory->data[i] = -1;
    }

    temp_id = cpu->actual_process->simulated_process.process_id;

    // Limpando os campos da CPU
    cpu->actual_process = NULL;
    cpu->program_counter = 0;
    cpu->quant_int = 0;
    cpu->quantum = 0;

    printf("Processo de ID %d retirado da CPU.\nTempo de CPU usado até o momento: %d und de tempo.\n", temp_id, cpu->used_time);
    printf("***************************************\n");
}

void show_cpu(CPU cpu) {
    printf("====================================\n");
    printf("|               CPU                |\n");
    printf("====================================\n");
    printf("| Program Counter       : %d       |\n", cpu.program_counter);
    printf("| Quantidade de inteiros: %d       |\n", cpu.quant_int);
    printf("| Tempo de fatia de tempo: %d      |\n", cpu.quantum);
    printf("| Tempo de uso          : %d       |\n", cpu.used_time);
    printf("| Índice de início da memória: %d  |\n", cpu.index_mem_init);
    printf("| Índice de fim da memória: %d     |\n", cpu.index_mem_end);
    printf("====================================\n");

    if (cpu.actual_process != NULL) {
        printf("| Processo atual        : %d       |\n", cpu.actual_process->simulated_process.process_id);
    } else {
        printf("| Processo atual        : Não há processo na CPU |\n");
    }

    printf("Estado da memória:\n");
    for (int i = cpu.index_mem_init; i < cpu.index_mem_end; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }

        printf("[%d]: %d ", i, cpu.memory->data[i]);
    }

    printf("====================================\n");
}

int run_instruction(CPU *cpu, int mem_index, char instruction, int value) {
    switch (instruction) {
        case 'A':
            cpu->memory->data[mem_index] += value;
            cpu->program_counter++;
            cpu->used_time++;
            cpu->quantum++;
            break;

        case 'S':
            cpu->memory->data[mem_index] -= value;
            cpu->program_counter++;
            cpu->used_time++;
            cpu->quantum++;
            break;

        case 'V':
            cpu->memory->data[mem_index] = value;
            cpu->program_counter++;
            cpu->used_time++;
            cpu->quantum++;
            break;

        case 'D':
            cpu->memory->data[mem_index] = value;
            cpu->program_counter++;
            cpu->used_time++;
            cpu->quantum++;
            break;
        default:
            printf("Erro: Instrução inválida.\n");
            return 1;
            break;
    }
}