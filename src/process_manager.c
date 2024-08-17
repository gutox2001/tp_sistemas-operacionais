#include "../libs/process_manager.h"

void initialize_process_manager(ProcessManager *process_manager, CPU *cpu_list) {
    process_manager->cpu_list = cpu_list;
    initialize_empty_fila(&process_manager->ReadyState);
    initialize_empty_fila(&process_manager->BlockedState);
    for (int i = 0; i < QUANT_CPU; i++) {
        process_manager->ExecutionState[i] = -1;
    }

    initialize_table(&process_manager->process_table);
}

void troca_de_contexto(CPU *cpu, ProcessTable *process_table, int new_process_index, State new_state) {
    if (cpu->actual_process != NULL) {
        // Salva o estado do processo atual na tabela de processos
        ItemProcess *current_process = cpu->actual_process;
        current_process->program_counter = cpu->program_counter;
        current_process->cpu_time += cpu->used_time;
        current_process->process_state = new_state;

        // Diminui a prioridade se a fatia de tempo foi completamente usada
        int max_time_slice = get_time_slice_by_priority(current_process->priority);
        if (cpu->used_time >= max_time_slice && current_process->priority < 3) {
            current_process->priority++;  // Diminui a prioridade
        }
    }

    // Carrega o estado do novo processo da tabela de processos para a CPU
    ItemProcess *new_process = &process_table->item_process[new_process_index];
    cpu->actual_process = new_process;
    cpu->program_counter = new_process->program_counter;
    cpu->used_time = 0;  // Reinicia o tempo de uso da CPU
}

void escalona_by_priority(ProcessManager *process_manager, CPU *cpu) {
    // Ordena as filas de processos prontos e bloqueados por prioridade
    sort_fila_by_priority(&process_manager->ReadyState);
    sort_fila_by_priority(&process_manager->BlockedState);

    // Se houver um processo pronto para executar
    if (process_manager->ReadyState.start != NULL) {
        pid_t next_process_index = process_manager->ReadyState.start->item.process_table_index;

        // Realiza a troca de contexto, passando o novo processo para a CPU
        troca_de_contexto(cpu, &process_manager->process_table, next_process_index, Pronto);
    } else {
        printf("Nenhum processo pronto para executar.\n");
    }
}

void run_process(ProcessManager *process_manager, ItemProcess *process, const char *input_command_string) {
    // Inicialização do primeiro processo
    CPU *cpu = &process_manager->cpu_list[0];
    int i = 0;

    int index_cpu;

    // Colocar o processo na CPU
    add_process_to_cpu(cpu, process);

    // Loop para processar os comandos de entrada
    for (i = 0; input_command_string[i] != '\0'; i++) {
        switch (input_command_string[i]) {
            case 'U': {
                // Executar a próxima instrução do processo
                Instruction current_instruction = process->simulated_process.process_instructions[process->program_counter];
                switch (current_instruction.instruction_char) {
                    case 'N':
                        process->simulated_process.int_quantity = current_instruction.index;
                        process->simulated_process.program_counter++;
                        process->program_counter++;
                        cpu->program_counter++;
                        cpu->used_time++;
                        cpu->quantum++;
                        break;

                    case 'D':
                        run_instruction(cpu, current_instruction.index, current_instruction.instruction_char, 0);
                        break;

                    case 'A':
                    case 'S':
                    case 'V':
                        run_instruction(cpu, current_instruction.index, current_instruction.instruction_char, current_instruction.value);
                        break;

                    case 'T':
                        // Processo terminou
                        remove_process_from_table(process->id, &process_manager->process_table);
                        clean_cpu(cpu);
                        return;

                    case 'F':
                        // Criar um novo processo simulado idêntico ao pai
                        // Copia os dados do pai para o filho
                        create_new_item_process(process->id, process->program_counter, process->simulated_process, process->priority, &process_manager->process_table);

                        // Verifica se há CPU disponível
                        index_cpu = is_any_cpu_available(process_manager);
                        // Se houver CPU disponível, coloca o processo na CPU, se não passa a CPU do processo pai
                        if (index_cpu != -1) {
                            escalona_by_priority(process_manager, &process_manager->cpu_list[index_cpu]);
                        } else {
                            escalona_by_priority(process_manager, cpu);
                        }

                        // Retira prioridade do pai
                        process->priority = process->priority + 1;
                        break;

                    case 'R':
                        // Lê o arquivo de instruções do processo
                        SimulatedProcess temp_simulated_process = initialize_simulated_process(current_instruction.arq_name);
                        create_new_item_process(process->id, 0, temp_simulated_process, process->priority, &process_manager->process_table);

                        // Verifica se há CPU disponível
                        index_cpu = is_any_cpu_available(process_manager);
                        // Se houver CPU disponível, coloca o processo na CPU, se não passa a CPU do processo pai
                        if (index_cpu != -1) {
                            escalona_by_priority(process_manager, &process_manager->cpu_list[index_cpu]);
                        } else {
                            escalona_by_priority(process_manager, cpu);
                        }
                        break;

                    default:
                        printf("Instrução desconhecida: %c\n", current_instruction.instruction_char);
                        break;
                }
                process->program_counter++;
                break;
            }

            case 'I':
                // Mostrar o estado atual do sistema
                printf(BOLD "GERENCIADOR DE PROCESSOS: \n" RESET);
                show_process_table(process_manager->process_table);
                for (int j = 0; j < QUANT_CPU; j++) {
                    show_cpu(process_manager->cpu_list[j]);
                }
                break;

            case 'M':
                // Mostrar o estado do sistema e encerrar
                printf(BOLD "GERENCIADOR DE PROCESSOS: \n" RESET);
                show_process_table(process_manager->process_table);
                for (int j = 0; j < QUANT_CPU; j++) {
                    show_cpu(process_manager->cpu_list[j]);
                }
                printf("Encerrando o sistema.\n");
                return;

            default:
                printf("Comando desconhecido: %c\n", input_command_string[i]);
                break;
        }
    }
    return;
}

// Função para verificar se um processo está esperando por um recurso na fila de bloqueado
// void verify_if_process_is_waiting(ProcessManager *process_manager) {
//     // Se houver um processo esperando por um recurso
//     if (process_manager->BlockedState.start != NULL) {
//         // Verifica se o recurso está disponível
//         ItemProcess *process = process_manager->BlockedState.start->item.process_table_index;
//         if (process->simulated_process.int_quantity <= 0) {
//             // Recurso disponível
//             // Remove o processo da fila de bloqueados e coloca na fila de prontos
//             ItemProcess *process = remove_process_from_fila(&process_manager->BlockedState);
//             add_process_to_fila(&process_manager->ReadyState, process);
//         }
//     }
// }

int is_any_cpu_available(ProcessManager *process_manager) {
    for (int i = 0; i < QUANT_CPU; i++) {
        if (process_manager->cpu_list[i].actual_process == NULL) {
            printf("CPU %d está disponível.\n", i);
            return i;
        }
    }
    return -1;
}

int get_time_slice_by_priority(int priority) {
    switch(priority) {
        case 0:
            return TIME_SLICE_PRIORITY_0;
        case 1:
            return TIME_SLICE_PRIORITY_1;
        case 2:
            return TIME_SLICE_PRIORITY_2;
        case 3:
            return TIME_SLICE_PRIORITY_3;
        default:
            return TIME_SLICE_PRIORITY_3; // Maior tempo como padrão
    }
}