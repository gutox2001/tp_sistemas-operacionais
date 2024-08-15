#include "../libs/simulated_process.h"

SimulatedProcess initialize_simulated_process(char *arq_name) {
    SimulatedProcess process;

    int quant_int = 0, quant_rows = 0;
    quant_rows = set_process_instructions(arq_name, &process.process_instructions, &quant_int);

    // Ao inicializar o processo simulado, o ponteiro para memória deve ser adicionado
    process.memory = NULL;
    process.instruction_quantity = quant_rows;
    process.int_quantity = quant_int;
    process.program_counter = 0;
    process.priority = 0;

    return process;
}

int set_process_instructions(char *arq_name, Instruction **instructions, int *quant_int) {
    int quant_rows, cont;
    FILE *arq;
    char new_arq_name[MAX_TAM_ARQ_NAME];  // Nome do novo arquivo
    char new_instruction_char;            // Caracter da nova instrução
    int value, index;                     // Variáveis para armazenar valor e índice

    // Conta o número de instruções no arquivo e aloca memória para as instruções
    quant_rows = count_arq_instructions(arq_name);
    *instructions = (Instruction *)malloc(quant_rows * sizeof(Instruction));
    if (*instructions == NULL) {  // Verifica se a alocação foi bem-sucedida
        fprintf(stderr, "Erro ao alocar memória para instruções.\n");
        return -1;
    }

    // Abre o arquivo para leitura
    arq = fopen(arq_name, "rt");
    if (arq == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", arq_name);
        free(*instructions);  // Libera a memória alocada em caso de erro
        return -1;            // Retorna erro se o arquivo não puder ser aberto
    }

    // Loop para ler cada instrução do arquivo
    for (cont = 0; cont < quant_rows; cont++) {
        fscanf(arq, " %c", &new_instruction_char);  // Lê o caracter da instrução
        (*instructions)[cont].instruction_char = new_instruction_char;

        // Processa a instrução com base no caracter lido
        switch ((*instructions)[cont].instruction_char) {
            case 'B':
            case 'T':
                // Não faz nada para 'B' e 'T'
                break;

            case 'N':                       // Se for 'N', lemos o índice
            case 'D':                       // Se for 'D', lemos o índice
            case 'F':                       // Se for 'F', lemos o índice
                fscanf(arq, "%d", &index);  // Lê o índice
                (*instructions)[cont].index = index;

                // Se for a primeira instrução e for 'N', atribui ao quant_int
                if (cont == 0 && (*instructions)[cont].instruction_char == 'N') {
                    *quant_int = index;
                }
                break;

            case 'R':  // Se for 'R', lemos o nome do arquivo
                fscanf(arq, "%s", new_arq_name);
                strcpy((*instructions)[cont].arq_name, new_arq_name);
                break;

            case 'S':                                  // Se for 'S', lemos índice e valor
            case 'A':                                  // Se for 'A', lemos índice e valor
            case 'V':                                  // Se for 'V', lemos índice e valor
                fscanf(arq, "%d %d", &index, &value);  // Lê índice e valor
                (*instructions)[cont].index = index;
                (*instructions)[cont].value = value;
                break;

            default:
                // Se a instrução não for reconhecida, decrementa o contador
                cont--;
                break;
        }
    }

    // Fecha o arquivo
    fclose(arq);
    return quant_rows;  // Retorna o número de instruções lidas
}

int count_arq_instructions(char *arq_name) {
    FILE *arq;
    int cont_rows = 0;
    char character;

    arq = fopen(arq_name, "rt");
    if (arq == NULL) {  // Verifica se o arquivo foi aberto corretamente
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", arq_name);
        return -1;  // Retorna erro se o arquivo não puder ser aberto
    }

    while ((character = fgetc(arq)) != EOF) {
        if (character == '\n') cont_rows++;  // Conta linhas
    }
    fclose(arq);
    return cont_rows;
}

void show_simulated_process(SimulatedProcess process) {
    // Exibe informações do processo simulado
    printf("Processo simulado:\n");
    printf("Quantidade de instruções: %d\n", process.instruction_quantity);
    printf("Quantidade de inteiros: %d\n", process.int_quantity);
    printf("Program Counter: %d\n", process.program_counter);
    printf("Prioridade: %d\n", process.priority);

    // Exibe cada instrução
    for (int i = 0; i < process.instruction_quantity; i++) {
        printf("Instrução %d: %c", i + 1, process.process_instructions[i].instruction_char);

        // Se a instrução possui um índice, exibe-o
        if (process.process_instructions[i].instruction_char == 'N' ||
            process.process_instructions[i].instruction_char == 'D' ||
            process.process_instructions[i].instruction_char == 'F' ||
            process.process_instructions[i].instruction_char == 'S' ||
            process.process_instructions[i].instruction_char == 'A' ||
            process.process_instructions[i].instruction_char == 'V') {
            printf(", Índice: %d", process.process_instructions[i].index);
        }

        // Se a instrução é 'R', exibe o nome do arquivo
        if (process.process_instructions[i].instruction_char == 'R') {
            printf(", Arquivo: %s", process.process_instructions[i].arq_name);
        }

        // Se a instrução possui um valor, exibe-o
        if (process.process_instructions[i].instruction_char == 'S' ||
            process.process_instructions[i].instruction_char == 'A' ||
            process.process_instructions[i].instruction_char == 'V') {
            printf(", Valor: %d", process.process_instructions[i].value);
        }
        printf("\n");
    }
}
