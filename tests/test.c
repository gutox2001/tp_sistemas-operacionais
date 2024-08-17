#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/control_process.h"

// Função auxiliar para criar um ItemProcess de exemplo
ItemProcess create_sample_process(int id) {
    ItemProcess process;

    process.simulated_process = test_simulated_process();

    return process;
}

// Função de teste para a CPU -- Passou no Teste !
void test_cpu() {
    printf("--------------------------------\n");
    printf("Iniciando os testes da CPU...\n");

    // Criação da CPU e memória
    CPU cpu;
    Memory memory;          // Exemplo de memória de 100 inteiros

    // Inicializando a CPU
    initialize_cpu(&cpu, &memory, 0, 100);

    // Verificando se a CPU está vazia
    if (is_cpu_empty(&cpu)) {
        printf("A CPU está vazia inicialmente.\n");
    } else {
        printf("A CPU não está vazia.\n");
    }

    // Criando e adicionando processos à CPU
    ItemProcess process1 = create_sample_process(1);
    add_process_to_cpu(&cpu, &process1);

    show_cpu(cpu);
    // Verificando se a CPU está vazia após adicionar um processo
    if (!is_cpu_empty(&cpu)) {
        printf("A CPU não está vazia após adicionar o processo %d.\n", cpu.actual_process->simulated_process.process_id);
    }

    // Limpando a CPU
    clean_cpu(&cpu);

    // Verificando se a CPU está vazia após limpar
    if (is_cpu_empty(&cpu)) {
        printf("A CPU está vazia após limpar.\n");
    } else {
        printf("A CPU não está vazia após limpar.\n");
    }

    // Liberando memória alocada para o processo
    free(process1.simulated_process.process_instructions);

    printf("Testes da CPU concluídos.\n");
    printf("--------------------------------\n\n");
}

// Função de teste para o processo simulado -- Passou no Teste !
SimulatedProcess test_simulated_process() {
    printf("||||||||||||||||||||||||||||||||||||||||||\n");

    printf("Iniciando os testes do processo simulado...\n");

    // Criação do processo simulado
    SimulatedProcess temp_simulated_process = initialize_simulated_process("data/init.txt");
    // if (temp_simulated_process.process_instructions == NULL) {
    //     printf("Erro ao obter processo simulado do arquivo...\n");
    //     return NULL;
    // }

    // Exibindo o processo simulado
    show_simulated_process(temp_simulated_process);

    printf("Testes do processo simulado concluídos.\n");

    printf("||||||||||||||||||||||||||||||||||||||||||\n");
    return temp_simulated_process;
}

// Função de teste para a fila -- Passou no Teste !
void test_fila() {
    printf("--------------------------------\n");
    printf("Iniciando os testes da fila...\n");

    // Criação da fila
    TypeFila fila;
    initialize_empty_fila(&fila);

    // Verificando se a fila está vazia
    if (is_fila_empty(fila)) {
        printf("A fila está vazia inicialmente.\n");
    } else {
        printf("A fila não está vazia.\n");
    }

    int priority_1 = 2;
    int priority_2 = 1;
    int priority_3 = 0;
    // Criando items de exemplo
    TypeItem item_1 = {1, &priority_1};
    TypeItem item_2 = {2, &priority_2};
    TypeItem item_3 = {3, &priority_3};
    // Adicionando itens à fila
    // for (int i = 0; i < 5; i++) {
    //     TypeItem item = create_sample_item(i, rand() % 10);  // Prioridade aleatória de 0 a 9
    //     add_item_to_fila(item, &fila);
    //     printf("Item com índice %d e prioridade %d adicionado à fila.\n", item.index_process_table, *item.priority);
    // }

    add_item_to_fila(item_1, &fila);
    add_item_to_fila(item_2, &fila);
    add_item_to_fila(item_3, &fila);

    sort_fila_by_priority(&fila);

    // Exibindo o estado da fila
    show_fila(&fila);

    // Removendo itens da fila
    int removed_index = remove_item_from_fila(&fila);
    printf("Item com índice %d removido da fila.\n", removed_index);
    int removed_index2 = remove_item_from_fila(&fila);
    printf("Item com índice %d removido da fila.\n", removed_index2);
    int removed_index3 = remove_item_from_fila(&fila);
    printf("Item com índice %d removido da fila.\n", removed_index3);
    show_fila(&fila);

    // Verificando se a fila está vazia após remoções
    if (is_fila_empty(fila)) {
        printf("A fila está vazia após todas as remoções.\n");
    } else {
        printf("A fila não está vazia após todas as remoções.\n");
    }

    printf("Testes da fila concluídos.\n");
    printf("--------------------------------\n\n");
}

// Função de teste para a tabela de processos
void test_process_table() {
    printf("_________________________________\n");
    ProcessTable process_table;
    initialize_table(&process_table);

    // Verifica se a tabela está vazia após a inicialização
    if (is_empty(&process_table)) {
        printf("A tabela de processos está vazia após a inicialização.\n");
    } else {
        printf("Erro: A tabela de processos não está vazia após a inicialização.\n");
    }

    // Cria um processo simulado para adicionar à tabela
    SimulatedProcess simulated_process;
    simulated_process.process_id = 1;    // Atribui um ID ao processo simulado
    simulated_process.int_quantity = 0;  // Define a quantidade de instruções
    // Adicione mais inicializações para simulated_process conforme necessário

    // Cria um novo item de processo e o adiciona à tabela
    create_new_item_process(1, 0, simulated_process, 5, &process_table);

    // Mostra a tabela de processos
    show_process_table(process_table);

    // Remove o processo da tabela
    remove_process_from_table(0, &process_table);
    printf("Processo no índice 0 removido da tabela.\n");

    // Mostra a tabela de processos após a remoção
    show_process_table(process_table);

    // Verifica se o índice 0 foi limpo
    if (process_table.item_process[0].id == -1) {
        printf("O índice 0 foi limpo corretamente.\n");
    } else {
        printf("Erro: O índice 0 não foi limpo corretamente.\n");
    }

    printf("_________________________________\n\n");
}