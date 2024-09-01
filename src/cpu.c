#include "../libs/cpu.h"

void initialize_cpu(CPU *cpu, Memory *memory, int index_mem_init, int index_mem_end) {
    cpu->actual_process = NULL;
    cpu->memory = memory;
    cpu->program_counter = 0;
    cpu->quant_int = 0;
    cpu->quantum = 0;
    cpu->used_time = 0;
    // cpu->index_mem_init = index_mem_init;
    // cpu->index_mem_end = index_mem_end;
}

void add_process_to_cpu(CPU *cpu, ItemProcess *process) {
    cpu->actual_process = process;
    cpu->quant_int = process->simulated_process.int_quantity;
    cpu->program_counter = process->simulated_process.program_counter;

    printf("Processo de ID %d adicionado da CPU.\n", cpu->actual_process->simulated_process.process_id);
    //show_cpu(*cpu);
    return;
}

int is_cpu_empty(CPU *cpu) {
    if (cpu->actual_process == NULL) {
        return 1;
    }

    return 0;
}

void clean_cpu(CPU *cpu) {
    if (cpu->actual_process != NULL) {
        int temp_id = cpu->actual_process->simulated_process.process_id;

        // Limpando a memória usada pelo processo atual
        // for (size_t i = 0; i < cpu->quant_int; i++) {
        //     int memory_index = cpu->index_mem_init + i;
        //     cpu->memory->data[memory_index] = -1;  // Marca o espaço como "limpo" ou "vazio"
        // }

        // Limpando os campos da CPU
        cpu->actual_process = NULL;
        cpu->program_counter = 0;
        cpu->quant_int = 0;
        cpu->quantum = 0;

        printf("\nProcesso de ID %d retirado da CPU.\nTempo de CPU usado até o momento: %d unidades de tempo.\n", temp_id, cpu->used_time);
        printf("\n***************************************\n");
    } else {
        printf("Nenhum processo para limpar na CPU.\n");
    }
}

// Função para exibir o estado da CPU
void show_cpu(CPU cpu) {
    printf(BOLD "\n\n====================================\n" RESET);
    printf(BOLD "|               CPU                |\n" RESET);
    printf(BOLD "====================================\n" RESET);
    printf(BOLD "| Program Counter       : " YELLOW "%-7d" RESET "|\n", cpu.program_counter);
    printf(BOLD "| Quantidade de inteiros: " GREEN "%-7d" RESET "|\n", cpu.quant_int);
    printf(BOLD "| Tempo de fatia de tempo: " CYAN "%-7d" RESET "|\n", cpu.quantum);
    printf(BOLD "| Tempo de uso          : " RED "%-7d" RESET "|\n", cpu.used_time);
    // printf(BOLD "| Índice de início da memória: " MAGENTA "%-4d" RESET "|\n", cpu.index_mem_init);
    // printf(BOLD "| Índice de fim da memória: " MAGENTA "%-6d" RESET "|\n", cpu.index_mem_end);
    printf(BOLD "====================================\n" RESET);

    if (cpu.actual_process != NULL) {
        printf(GREEN "| Processo atual        : " YELLOW "%-7d" RESET "|\n", cpu.actual_process->simulated_process.process_id);
    } else {
        printf(RED "| Processo atual        : " RED "Não há processo na CPU" RESET " |\n");
    }

    printf("\n" BOLD "====================================\n\n" RESET);
}

int run_instruction(CPU *cpu, int mem_index, char instruction, int value, alocationVector *vect) {
    for(int i = 0; i < 300; i += 3){
        if(vect->endressAdress[i] == cpu->actual_process->id){
            mem_index += vect->endressAdress[i+1];
            break;
        }
    }
    
    //mem_index += cpu->index_mem_init;
    
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