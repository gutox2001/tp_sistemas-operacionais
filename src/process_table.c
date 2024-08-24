#include "../libs/process_table.h"

// Função para inicializar a tabela de processos
void initialize_table(ProcessTable *process_table) {
    process_table->first_item = 0;
    process_table->last_item = 0;

    for (size_t i = 0; i < MAX_TAM; i++) {
        process_table->item_process[i].id = -1;                    // Valor inválido para ID
        process_table->item_process[i].parent_id = -1;             // Valor inválido para o ID do pai
        process_table->item_process[i].program_counter = -1;       // Valor inválido para o contador de programa
        process_table->item_process[i].priority = -1;              // Valor inválido para prioridade
        process_table->item_process[i].process_state = Bloqueado;  // Estado inválido
        process_table->item_process[i].start_time = -1;            // Valor inválido para o tempo de início
        process_table->item_process[i].cpu_time = -1;              // Valor inválido para o tempo de CPU
    }

    printf("Tabela de processos inicializada com sucesso.\n");
}

// Função para criar um novo item de processo
void create_new_item_process(pid_t parent_id, int program_counter, SimulatedProcess simulated_process, int priority, ProcessTable *process_table) {
    ItemProcess new_item;                                   // Cria um novo item de processo
    new_item.id = get_first_empty_position(process_table);  // Atribui o ID do processo atual
    new_item.parent_id = parent_id;                         // Define o ID do processo pai
    new_item.program_counter = program_counter;             // Inicializa o contador de programa
    new_item.simulated_process = simulated_process;         // Define o processo simulado
    new_item.priority = priority;                           // Define a prioridade
    new_item.process_state = Pronto;                        // Inicializa o estado do processo como "Pronto"
    new_item.start_time = 0;                                // Inicializa o tempo de início
    new_item.cpu_time = 0;                                  // Inicializa o tempo de CPU

    add_process_to_table(new_item, process_table);
    printf("Processo criado e adicionado com sucesso a tabela. ID: %d\n", new_item.id);
}

// Função para verificar se a tabela de processos está vazia
int is_empty(ProcessTable *process_table) {
    return (process_table->first_item == process_table->last_item);
}

// Função para adicionar um processo à tabela
int add_process_to_table(ItemProcess new_item_process, ProcessTable *process_table) {
    if (process_table->last_item < MAX_TAM) {
        process_table->item_process[process_table->last_item] = new_item_process;  // Adiciona o novo processo
        process_table->last_item++;                                                // Atualiza o índice do último item
        return 0;                                                                  // Sucesso
    } else {
        printf("Erro: Tabela de processos cheia.\n");
        return -1;  // Erro, tabela cheia
    }
}

// Função para remover um processo da tabela, limpando seus dados
void remove_process_from_table(int indexToRemove, ProcessTable *process_table) {
    if (indexToRemove >= process_table->first_item && indexToRemove < process_table->last_item) {
        // Limpa os dados do item no índice especificado
        process_table->item_process[indexToRemove].id = -1;                    // Valor inválido para ID
        process_table->item_process[indexToRemove].parent_id = -1;             // Valor inválido para o ID do pai
        process_table->item_process[indexToRemove].program_counter = -1;       // Valor inválido para o contador de programa
        process_table->item_process[indexToRemove].priority = -1;              // Valor inválido para prioridade
        process_table->item_process[indexToRemove].process_state = Bloqueado;  // Estado inválido
        process_table->item_process[indexToRemove].start_time = -1;            // Valor inválido para o tempo de início
        process_table->item_process[indexToRemove].cpu_time = -1;              // Valor inválido para o tempo de CPU
    } else {
        printf("Erro: Índice fora dos limites.\n");
    }
}

void show_item_process(ItemProcess process) { 
    printf(BOLD "\n+----+------+------------+--------+\n" RESET);
    printf(BLUE "| ID | Pai  | Prioridade | Estado |\n" RESET);
    printf(BOLD "+----+------+------------+--------+\n" RESET);
    printf("| %-2d | %-4d | " RED "%-10d" RESET " | %-6d |\n",
           process.id,
           process.parent_id,
           process.priority,
           process.process_state);
    printf(BOLD "+----+------+------------+--------+\n" RESET);
}

// Função para exibir a tabela de processos
void show_process_table(ProcessTable process_table) {
    printf(BOLD "\n+----+------+------------+--------+\n" RESET);
    printf(BLUE "| ID | Pai  | Prioridade | Estado |\n" RESET);
    printf(BOLD "+----+------+------------+--------+\n" RESET);

    for (int i = 0; i < process_table.last_item; i++) {
        printf("| %-2d | %-4d | " RED "%-10d" RESET " | %-6d |\n",
               process_table.item_process[i].id,
               process_table.item_process[i].parent_id,
               process_table.item_process[i].priority,
               process_table.item_process[i].process_state);
    }

    printf(BOLD "+----+------+------------+--------+\n" RESET);
}

int get_first_empty_position(ProcessTable *process_table) {
    for (int i = 0; i < MAX_TAM; i++) {
        if (process_table->item_process[i].id == -1) {
            return i;
        }
    }
    return -1;
}
