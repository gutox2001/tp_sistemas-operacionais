#include "../libs/control_process.h"

void initialize_control_process(int type_input, int type_escalonamento, char *send_string, int selected_escalonador) {
    clock_t start = clock();

    int fd[2];  // Pipe para comunicação entre pai e filho
    pid_t pid;

    Memory memory;  // Memória do sistema
    CPU CPU_list[QUANT_CPU];
    ProcessManager process_manager;  // Gerenciador de processos do sistema
    SimulatedProcess control_process;
    SimulatedProcess first_simulated_process;  // Primeiro processo simulado que será executado
    ItemProcess new_item_process;              // Item processo da tabela de processos

    // Inicializa a memória do sistema
    initialize_memory(&memory);

    for (int i = 0; i < QUANT_CPU; i++) {
        int index_mem_init = (50 * i);
        int index_mem_end = index_mem_init + 50;
        initialize_cpu(&CPU_list[i], &memory, index_mem_init, index_mem_end);
    }

    /* Criando nosso Pipe */
    if (pipe(fd) == -1) {
        perror("pipe error");
        exit(1);
    }

    /* Processo Pai*/
    pid = fork();
    if (pid < 0) {
        perror("fork error");
        exit(1);
    }

    if (pid > 0) {
        printf("Esse é o processo pai.");

        printf("Comandos enviados: %s\n", send_string);

        /*No pai, vamos ESCREVER, então vamos fechar a LEITURA do Pipe neste lado*/
        close(fd[0]);

        write(fd[1], send_string, strlen(send_string) + 1);
        close(fd[1]);  // Fechar o pipe de escrita após a conclusão
    }

    else {  // Processo filho
        printf("Esse é o processo filho. \n");

        char receive_string[TAM_MAX_MNS] = "";
        int is_system_running = 1;  // Flag para verificar se o sistema está em execução
        int command_index = 0;      // Índice do comando atual

        close(fd[1]);
        read(fd[0], receive_string, sizeof(receive_string));

        close(fd[0]);  // Fechar o pipe de leitura após a conclusão

        remove_spaces(receive_string);
        printf("Comandos recebidos: %s\nO sistema será inicializado.\n", receive_string);
        printf("*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_\n");

        // Inicializa o gerenciador de processos
        initialize_process_manager(&process_manager, CPU_list);
        create_new_item_process(pid, 0, control_process, 0, &process_manager.process_table);

        // Instancia o primeiro processe simulado
        first_simulated_process = initialize_simulated_process("data/first_process.txt", 1);
        // Percorre a string de entrada passando os comandos para o gerenciador de processos
        create_new_item_process(0, 0, first_simulated_process, 0, &process_manager.process_table);
        TypeItem new_item = {1, &process_manager.process_table.item_process[1].priority};

        // Colocar o processo na CPU e o adiciona na fila de execução. Depois o executa
        add_process_to_cpu(&CPU_list[0], &process_manager.process_table.item_process[1]);
        add_item_to_fila(new_item, &process_manager.ExecutionState);
        run_process(&process_manager, &CPU_list[0], &process_manager.process_table.item_process[1], receive_string, selected_escalonador, &is_system_running, &command_index);
        // return;
    }

    clock_t end = clock();

    // Calcula o tempo decorrido em segundos
    double tempo_de_execucao = (double)(end - start) / CLOCKS_PER_SEC;

    show_control_process(tempo_de_execucao, selected_escalonador);
}

void show_menu_escalonamento() {
    printf("\n");
    printf("╔═════════════════════════════════════╗\n");
    printf("║ Qual método de escalonamento usar?  ║\n");
    printf("╠═════════════════════════════════════╣\n");
    printf("║ 1. Escalonamento por Prioridades    ║\n");
    printf("║ 2. Escalonamento FCFS               ║\n");
    printf("╚═════════════════════════════════════╝\n");
    printf("Escolha uma opção (1 ou 2): ");
}

void show_main_menu() {
    printf("\n");
    printf("╔════════════════════════════════╗\n");
    printf("║     Escolha o Modo de Entrada  ║\n");
    printf("╠════════════════════════════════╣\n");
    printf("║ 1. Ler do Terminal             ║\n");
    printf("║ 2. Ler de um Arquivo           ║\n");
    printf("╚════════════════════════════════╝\n");
    printf("Escolha uma opção (1 ou 2): ");
}

void read_terminal_input(char *return_char) {
    char input;
    int i = 0;

    printf("Comandos disponíveis:\n");
    printf("  U: Fim de uma unidade de tempo.\n");
    printf("  I: Imprime o estado atual do sistema.\n");
    printf("  M: Imprime o tempo médio de resposta e finaliza o sistema.\n");
    printf("\nDigite os comandos seguidos por ENTER.\n");
    printf("O comando 'M' deve ser o último e aparece exatamente uma vez.\n");

    do {
        scanf(" %c", &input);
        if (input >= 97 && input <= 120) {
            input = input - 32;
        }
        return_char[i] = input;

        strcat(return_char, " ");
        i++;
    } while (input != 'M');

    printf("Entradas obtidas: %s", return_char);
}

void read_arq(char *retorno) {
    FILE *arquivo;
    char str[2];

    arquivo = fopen("data/commands.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    // Lê o conteúdo do arquivo e armazena na string de retorno
    while (fscanf(arquivo, "%s", str) != EOF) {
        strcat(retorno, str);
        strcat(retorno, " ");
    }

    fclose(arquivo);
}

void remove_spaces(char *str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (str[i] != ' ') {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';
}

void show_control_process(double execution_time, int selected_escalonador) {
    printf("====================================\n");
    printf("|          Processo de Controle    |\n");
    printf("====================================\n");
    printf("| Tempo de execução da aplicação: %.2f segundos |\n", execution_time);
    printf("====================================\n");
    printf("| Escalonador escolhido: %d\n", selected_escalonador);
    printf("====================================\n");
    printf("\n");
}