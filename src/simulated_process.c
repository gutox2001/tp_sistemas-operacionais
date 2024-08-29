#include "../libs/simulated_process.h"

SimulatedProcess initialize_simulated_process(char *arq_name, int id) {
    SimulatedProcess process;

    int quant_int = 0, quant_rows = 0;
    quant_rows = set_process_instructions(arq_name, &process.process_instructions, &quant_int);

    process.process_id = id;
    process.instruction_quantity = quant_rows;
    process.int_quantity = quant_int;
    process.program_counter = 0;
    process.priority = 0;

    process.time_blocked = 0;

    show_simulated_process(process);
    printf("Processo simulado inicializado com sucesso.\n");
    return process;
}

int set_process_instructions(char *arq_name, Instruction **instructions, int *quant_int) {
    int quant_rows, cont, value, index;
    FILE *arq;
    char new_arq_name[100];
    char temp_instruction_char;
    char numbers[100];

    quant_rows = count_arq_instructions(arq_name);
    *instructions = (Instruction *)malloc(quant_rows * sizeof(Instruction));

    arq = fopen(arq_name, "rt");

    for (cont = 0; cont < quant_rows; cont++) {
        fscanf(arq, "%c", &temp_instruction_char);

        (*instructions)[cont].instruction_char = temp_instruction_char;
        char instruction_char = (*instructions)[cont].instruction_char;
        // if (instruction_char != ' ') {
        //     printf("Instrução a ser executada no processo simulado: %c\n", instruction_char);
        // }

        switch (instruction_char) {
            case 'T':
                break;

            case 'N':
            case 'D':
            case 'F':
            case 'B':
                fscanf(arq, "%d", &index);
                (*instructions)[cont].index = index;

                if (cont == 0 && (*instructions)[cont].instruction_char == 'N') {
                    *quant_int = index;
                }
                break;

            case 'R':
                fscanf(arq, "%s", new_arq_name);
                strcpy((*instructions)[cont].arq_name, new_arq_name);
                break;

            case 'S':
            case 'A':
            case 'V':
                fscanf(arq,
                       "%d"
                       "%d",
                       &index, &value);
                (*instructions)[cont].index = index;
                (*instructions)[cont].value = value;
                break;

            default:
                cont--;
                break;
        }
    }

    fclose(arq);
    return quant_rows;
}

int count_arq_instructions(char *arq_name) {
    FILE *arq;
    int quant_rows;
    char instruction_char;
    quant_rows = 0;
    arq = fopen(arq_name, "rt");
    while ((instruction_char = fgetc(arq)) != EOF) {
        if (instruction_char == '\n')
            quant_rows++;
    }
    fclose(arq);
    return quant_rows;
}

// Função para exibir o processo simulado
void show_simulated_process(SimulatedProcess process) {
    printf(BOLD "Processo simulado:\n" RESET);
    printf(BOLD "ID do processo: " RESET YELLOW "%d\n" RESET, process.process_id);
    printf(BOLD "Quantidade de instruções: " RESET GREEN "%d\n" RESET, process.instruction_quantity);
    printf(BOLD "Quantidade de inteiros: " RESET GREEN "%d\n" RESET, process.int_quantity);
    printf(BOLD "Program Counter: " RESET YELLOW "%d\n" RESET, process.program_counter);
    printf(BOLD "Prioridade: " RESET RED "%d\n" RESET, process.priority);
    printf(BOLD "Tempo bloqueado (Instrução 'B'): " RESET CYAN "%d\n" RESET, process.time_blocked);

    // Exibe as instruções
    show_instructions(process.process_instructions, process.instruction_quantity);
}

// Função para exibir as instruções
void show_instructions(Instruction *instructions, int quant_rows) {
    printf(BOLD "Instruções:\n" RESET);
    for (int i = 0; i < quant_rows; i++) {
        printf(CYAN "Instrução %d: %c" RESET, i + 1, instructions[i].instruction_char);

        if (instructions[i].instruction_char == 'N' ||
            instructions[i].instruction_char == 'D' ||
            instructions[i].instruction_char == 'F' ||
            instructions[i].instruction_char == 'S' ||
            instructions[i].instruction_char == 'A' ||
            instructions[i].instruction_char == 'B' ||
            instructions[i].instruction_char == 'V') {
            printf(YELLOW ", Índice: %d" RESET, instructions[i].index);
        }

        if (instructions[i].instruction_char == 'R') {
            printf(MAGENTA ", Arquivo: %s" RESET, instructions[i].arq_name);
        }

        if (instructions[i].instruction_char == 'S' ||
            instructions[i].instruction_char == 'A' ||
            instructions[i].instruction_char == 'V') {
            printf(GREEN ", Valor: %d" RESET, instructions[i].value);
        }
        printf("\n");
    }
}
