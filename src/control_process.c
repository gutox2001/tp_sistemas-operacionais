#include "../libs/control_process.h"

void initialize_control_process(int type_input, int type_escalonamento, char *string_input) {
    clock_t start = clock();
    clock_t end;

    int fd[2];  // Pipe para comunicação entre pai e filho
    pid_t pid;

    CPU CPU_list[QUANT_CPU];
    ProcessManager process_manager;            // Gerenciador de processos do sistema
    SimulatedProcess first_simulated_process;  // Primeiro processo simulado que será executado
    ItemProcess new_item_process;              // Item processo da tabela de processos
    TypeItem new_item;                         // Item da fila

    // int removidoBloqueado, removidoPronto;

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

        /*No pai, vamos ESCREVER, então vamos fechar a LEITURA do Pipe neste lado*/
        close(fd[0]);

        write(fd[1], string_input, sizeof(string_input));
        // close(fd[1]); // Fechar o pipe de escrita após a conclusão
    }

    else {  // Processo filho
        printf("Esse é o processo filho. \n");
        char string_recebida[TAM_MAX_MNS] = "";

        close(fd[1]);
        read(fd[0], string_recebida, sizeof(string_recebida));
        printf("%s", string_recebida);

        // Inicializa o gerenciador de processos
        initialize_process_manager(&process_manager, CPU_list);
    }
}

void mostrar_menu_escalonamento() {
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
        scanf("%c", &input);
        if (input >= 97 && input <= 120) {
            input = input - 32;
        }
        return_char[i] = input;

        strcat(return_char, " ");
        i++;
    } while (input != 'M');

    printf("Entradas obtidas: %s", return_char);
    // remove_char(return_char, '\n');
}

void read_arq(char *retorno) {
    FILE *arquivo;
    char str[2];

    arquivo = fopen("./data/initial_input.txt", "r");

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

// void remove_char(char *str, char c) {
//     char *pr = str, *pw = str;

//     while (*pr) {
//         *pw = *pr++;
//         pw += (*pw != c);
//     }

//     *pw = '\0';
// }