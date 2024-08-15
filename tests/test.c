#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/control_process.h"
#include "test.h"

// Função auxiliar para criar um ItemProcess de exemplo
ItemProcess create_sample_process(int id) {
    ItemProcess process;

    process.simulated_process = test_simulated_process();

    return process;
}

// Função de teste para a CPU
void test_cpu() {
    printf("Iniciando os testes da CPU...\n");

    // Criação da CPU e memória
    CPU cpu;
    Memory memory;  // Exemplo de memória de 100 inteiros
    int quantum_time = 10;  // Exemplo de tempo quântico

    // Inicializando a CPU
    initialize_cpu(&cpu, quantum_time, &memory, 0, 100);

    // Verificando se a CPU está vazia
    if (is_cpu_empty(&cpu)) {
        printf("A CPU está vazia inicialmente.\n");
    } else {
        printf("A CPU não está vazia.\n");
    }

    // Criando e adicionando processos à CPU
    ItemProcess process1 = create_sample_process(1);
    add_process_to_cpu(&cpu, &process1);

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
}

SimulatedProcess test_simulated_process() {
    printf("Iniciando os testes do processo simulado...\n");

    // Criação do processo simulado
    SimulatedProcess temp_simulated_process = initialize_simulated_process("../data/init.txt");
    if (temp_simulated_process.process_instructions == NULL) {
        printf("Erro ao obter processo simulado do arquivo...\n");
        return;
    }

    // Exibindo o processo simulado
    show_simulated_process(temp_simulated_process);

    printf("Testes do processo simulado concluídos.\n");

    return temp_simulated_process;
}

// Função de teste para a fila

// Função auxiliar para criar um TypeItem de exemplo
TypeItem create_sample_item(int index, int priority) {
    TypeItem item;
    item.index_process_table = index;
    item.priority = &priority;
    return item;
}
void test_fila() {
    printf("Iniciando os testes da fila...\n");

    // Criação da fila
    TypeFila fila;
    initialize_empty_fila(&fila, "Fila de Teste");

    // Verificando se a fila está vazia
    if (is_fila_empty(fila)) {
        printf("A fila está vazia inicialmente.\n");
    } else {
        printf("A fila não está vazia.\n");
    }

    // Adicionando itens à fila
    for (int i = 0; i < 5; i++) {
        TypeItem item = create_sample_item(i, rand() % 10);  // Prioridade aleatória de 0 a 9
        add_item_to_fila(item, &fila);
        printf("Item com índice %d e prioridade %d adicionado à fila.\n", item.index_process_table, *item.priority);
    }

    // Exibindo o estado da fila
    show_fila(fila);

    // Removendo itens da fila e exibindo o estado após cada remoção
    for (int i = 0; i < 5; i++) {
        int removed_index = remove_item_from_fila(&fila);
        if (removed_index != -1) {
            printf("Item com índice %d removido da fila.\n", removed_index);
        } else {
            printf("Falha ao remover item da fila.\n");
        }
        // Exibindo o estado da fila após a remoção
        show_fila(fila);
    }

    // Verificando se a fila está vazia após remoções
    if (is_fila_empty(fila)) {
        printf("A fila está vazia após todas as remoções.\n");
    } else {
        printf("A fila não está vazia após todas as remoções.\n");
    }

    printf("Testes da fila concluídos.\n");
}

// Função de teste para a tabela de processos
void test_process_table() {
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
    simulated_process.process_id = 1; // Atribui um ID ao processo simulado
    simulated_process.int_quantity = 0; // Define a quantidade de instruções
    // Adicione mais inicializações para simulated_process conforme necessário

    // Cria um novo item de processo
    ItemProcess new_process = create_new_item_process(getpid(), simulated_process, 5, &process_table);
    
    // Adiciona o novo processo à tabela
    if (add_process_to_table(new_process, &process_table) == 0) {
        printf("Processo adicionado com sucesso. ID: %d\n", new_process.id);
    }

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
}