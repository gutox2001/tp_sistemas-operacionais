#include "../libs/process_manager.h"

#include <ctype.h>

void initialize_process_manager(ProcessManager *process_manager, CPU *cpu_list) {
    process_manager->cpu_list = cpu_list;
    initialize_empty_fila(&process_manager->ReadyState);
    initialize_empty_fila(&process_manager->BlockedState);
    initialize_empty_fila(&process_manager->ExecutionState);

    initialize_table(&process_manager->process_table);

    //printf("Gerenciador de processos inicializado com sucesso.\n");
}

void troca_de_contexto(ProcessManager *process_manager, CPU *cpu, State old_process_new_state, char *receive_string, int selected_escalonador, int *command_index) {
    TypeItem new_process_item;

    if (cpu->actual_process != NULL) {
        // Salva o estado do processo atual na tabela de processos
        process_manager->process_table.item_process[cpu->actual_process->id].cpu_time = cpu->quantum;
        process_manager->process_table.item_process[cpu->actual_process->id].program_counter = cpu->program_counter;
        process_manager->process_table.item_process[cpu->actual_process->id].simulated_process = cpu->actual_process->simulated_process;
        process_manager->process_table.item_process[cpu->actual_process->id].priority = cpu->actual_process->priority;

        // //printf("PRIORIDADE DA CPU: %d", cpu->actual_process->priority);

        // Diminui a prioridade se a fatia de tempo foi completamente usada
        int max_time_slice = get_time_slice_by_priority(process_manager->process_table.item_process[cpu->actual_process->id].priority);
        if (cpu->used_time >= max_time_slice && process_manager->process_table.item_process[cpu->actual_process->id].priority < 3) {
            // Diminui a prioridade do processo atual
            process_manager->process_table.item_process[cpu->actual_process->id].priority++;
        }

        // Atualiza o estado do processo na tabela de processos
        process_manager->process_table.item_process[cpu->actual_process->id].process_state = old_process_new_state;

        // Remove o processo do /Estado de Execução e o coloca na fila de processos prontos ou bloqueados
        if (old_process_new_state == Bloqueado) {
            int old_process_index = remove_item_from_fila(&process_manager->ExecutionState);
            TypeItem new_item = {old_process_index, &process_manager->process_table.item_process[old_process_index].priority};

            add_item_to_fila(new_item, &process_manager->BlockedState);
            process_manager->process_table.item_process[old_process_index].process_state = Bloqueado;
        } else if (old_process_new_state == Pronto) {
            int old_process_index = remove_item_from_fila(&process_manager->ExecutionState);
            TypeItem new_item = {old_process_index, &process_manager->process_table.item_process[old_process_index].priority};

            add_item_to_fila(new_item, &process_manager->ReadyState);
            process_manager->process_table.item_process[old_process_index].process_state = Pronto;
        }
    }

    clean_cpu(cpu);

    int new_process_index = remove_item_from_fila(&process_manager->ReadyState);
    new_process_item.priority = &process_manager->process_table.item_process[new_process_index].priority;
    new_process_item.process_table_index = new_process_index;

    //printf("Processo que será adicionado à CPU: %d\n", new_process_index);
    //printf("Seu program counter: %d\n", process_manager->process_table.item_process[new_process_index].program_counter);

    // Colocar o processo na CPU e o adiciona na fila de execução. Depois continua a execução do processo na respectiva função 'run_processes'
    add_process_to_cpu(cpu, &process_manager->process_table.item_process[new_process_index]);
    add_item_to_fila(new_process_item, &process_manager->ExecutionState);
    process_manager->process_table.item_process[new_process_index].process_state = Execucao;

    //printf("Troca de contexto realizada com sucesso. Novo processo em execução.\n");
    return;
}

void escalona_by_priority(ProcessManager *process_manager, CPU *cpu, char *receive_string, int *command_index) {
    // Ordena as filas de processos prontos e bloqueados por prioridade
    sort_fila_by_priority(&process_manager->ReadyState);
    sort_fila_by_priority(&process_manager->BlockedState);

    // Se houver um processo pronto para executar
    if (!is_fila_empty(process_manager->ReadyState)) {
        pid_t next_process_index = process_manager->ReadyState.start->item.process_table_index;
        // Realiza a troca de contexto, passando o novo processo para a CPU
        troca_de_contexto(process_manager, cpu, Pronto, receive_string, 1, command_index);

        return;
    } else {
        //printf("Nenhum processo no estado Pronto! Verificando o estado Bloqueado...\n");

        // Caso não haja processos prontos, verifica se há processos bloqueados
        if (is_fila_empty(process_manager->BlockedState)) {
            //printf("Nenhum processo pronto ou bloqueado para executar! Aguardando...\n");
        }

        return;
    }
}

void escalona_fcfs(ProcessManager *process_manager, CPU *cpu, char *receive_string, int *command_index, Memory *memory, alocationVector *alocvect, last *ult, int type_alocacao) {
    // Não precisa ordenar a fila de processos prontos, pois o escalonamento é FCFS

    // Se houver um processo pronto para executar
    // Descobrir de o processo pronto está na memória
    if (!is_fila_empty(process_manager->ReadyState)) {
        pid_t next_process_index = process_manager->ReadyState.start->item.process_table_index;
        int is_in_memory = 0;
        for(int i = 0; i < 300; i+=3){
            if(alocvect->endressAdress[i] == next_process_index){
                is_in_memory = 1;
            }
        }
        if (is_in_memory == 0){
            // Alocar processo na memória
            
            alocation_manager(memory, process_manager->process_table.item_process[next_process_index], alocvect, ult, type_alocacao, &(process_manager->process_table), &(process_manager->ReadyState), &(process_manager->BlockedState), &(process_manager->ExecutionState));
        }
        
        // Realiza a troca de contexto, passando o novo processo para a CPU
        troca_de_contexto(process_manager, cpu, Pronto, receive_string, 1, command_index);
        return;
    } else {
        //printf("Nenhum processo pronto para executar.\n");
        // Caso não haja processos prontos, verifica se há processos bloqueados
        if (is_fila_empty(process_manager->BlockedState)) {
            //printf("Nenhum processo pronto ou bloqueado para executar.\n");
            return;
        }
        return;
    }
}

void run_selected_escalonador(ProcessManager *process_manager, CPU *cpu, char *receive_string, int selected_escalonador, int *command_index, Memory *memory, alocationVector *alocvect, last *ult, int type_alocacao) {
    switch (selected_escalonador) {
        case 1:
            escalona_by_priority(process_manager, cpu, receive_string, command_index);
            break;

        case 2:
            escalona_fcfs(process_manager, cpu, receive_string, command_index, memory, alocvect, ult, type_alocacao);
            break;

        default:
            //printf("Escalonador desconhecido: %d\n", selected_escalonador);
            break;
    }
}

int run_command_in_selected_process(ProcessManager *process_manager, CPU *cpu, ItemProcess *process, char current_command, int selected_escalonador, char *input_command_string, int *command_index, alocationVector *vect, last *ult, int type_alocacao, Memory *memory) {
    TypeItem new_item;
    SimulatedProcess new_simulated_process;
    ItemProcess process_table_item;
    int command_response = 1;
    Instruction current_instruction;

    switch (current_command) {
        case 'U': {
            // Executar a próxima instrução do processo
            //printf("\nProgram Counter do processo atual: %d\n", process->program_counter);

            current_instruction = process->simulated_process.process_instructions[process->program_counter];
            // show_simulated_process(process->simulated_process);

            //printf(BLUE "\nInstrução atual: %c; Indice: %d\n" RESET, current_instruction.instruction_char, current_instruction.index);
            fflush(stdout);
            switch (current_instruction.instruction_char) {
                case 'N':
                    //printf("Executando a instrução N: %d\n", current_instruction.index);
                    process->simulated_process.int_quantity = current_instruction.index;
                    process->simulated_process.program_counter++;
                    cpu->program_counter++;
                    cpu->used_time++;
                    cpu->quantum++;

                    //printf("Instrução %c executada com sucesso no processo %d\n", current_instruction.instruction_char, process->id);
                    alocation_manager(cpu->memory, *process, vect, ult, type_alocacao, &(process_manager->process_table), &(process_manager->ReadyState), &(process_manager->BlockedState), &(process_manager->ExecutionState));                   
                    fflush(stdout);
                    break;

                case 'D':
                    //printf("Executando a instrução D: %d\n", current_instruction.index);
                    run_instruction(cpu, current_instruction.index, current_instruction.instruction_char, 0, vect);
                    fflush(stdout);
                    //printf("Instrução %c executada com sucesso no processo %d\n", current_instruction.instruction_char, process->id);
                    break;

                case 'A':
                case 'S':
                case 'V':
                    //printf("Executando a instrução %c: %d\n", current_instruction.instruction_char, current_instruction.index);
                    run_instruction(cpu, current_instruction.index, current_instruction.instruction_char, current_instruction.value, vect);
                    fflush(stdout);
                    //printf("Instrução %c executada com sucesso no processo %d\n", current_instruction.instruction_char, process->id);
                    break;

                case 'B':
                    // Mover o processo atual para o estado bloqueado e trocar contexto
                    // Nessa instrução o escalonador tem que executar

                    process->process_state = Bloqueado;
                    // Retirar da fila de execução colocar na fila de bloqueados
                    // Chamar o escalonador para escolher o próximo processo

                    // troca_de_contexto(process_manager, cpu, Bloqueado, input_command_string, selected_escalonador, command_index);

                    //printf("Instrução %c executada com sucesso no processo %d\n", current_instruction.instruction_char, process->id);
                    break;

                case 'T':
                    // Finaliza o processo em execução e troca de contexto
                    // Nessa instrução o escalonador tem que executar

                    show_cpu(*cpu);
                    show_memory(*process_manager->cpu_list[0].memory);

                    remove_item_from_fila(&process_manager->ExecutionState);
                    remove_process_from_table(process->id, &process_manager->process_table);

                    show_fila(&process_manager->ExecutionState);

                    clean_cpu(cpu);

                    // Troca para o próximo processo pronto
                    run_selected_escalonador(process_manager, cpu, input_command_string, selected_escalonador, command_index, cpu->memory, vect, ult, type_alocacao);

                    //printf("Instrução %c executada com sucesso no processo %d\n", current_instruction.instruction_char, process->id);

                    break;

                case 'F':
                    // Criar um novo processo baseado no pai
                    int new_process_program_counter = process->program_counter + 1;
                    //printf("Program counter do novo processo %d\n", new_process_program_counter);
                    fflush(stdout);
                    new_simulated_process.instruction_quantity = process->simulated_process.instruction_quantity;
                    new_simulated_process.int_quantity = process->simulated_process.int_quantity;
                    new_simulated_process.time_blocked = 0;
                    new_simulated_process.process_instructions = process->simulated_process.process_instructions;
                    new_simulated_process.process_id = process_manager->process_table.last_item;
                    new_simulated_process.priority = process->priority;
                    new_simulated_process.program_counter = new_process_program_counter;

                    create_new_item_process(process->id, new_process_program_counter, new_simulated_process, new_simulated_process.priority, &process_manager->process_table);

                    process_table_item = process_manager->process_table.item_process[process_manager->process_table.last_item - 1];

                    if (new_simulated_process.program_counter > 0) {
                        alocation_manager(cpu->memory, process_table_item, vect, ult, type_alocacao, &(process_manager->process_table), &(process_manager->ReadyState), &(process_manager->BlockedState), &(process_manager->ExecutionState));
                    }

                    // Adicionar o novo processo na fila de prontos
                    new_item.priority = &process_manager->process_table.item_process[process_manager->process_table.last_item].priority;
                    new_item.process_table_index = process_manager->process_table.last_item - 1;

                    add_item_to_fila(new_item, &process_manager->ReadyState);
                    process_manager->process_table.item_process[new_item.process_table_index].process_state = Pronto;

                    // Ajustar prioridade do pai, reduzindo-a
                    process->priority = process->priority + 1;

                    // Verifica se há CPU disponível
                    int index_free_cpu = is_any_cpu_available(process_manager);

                    run_selected_escalonador(process_manager, &process_manager->cpu_list[index_free_cpu], input_command_string, selected_escalonador, command_index, cpu->memory, vect, ult, type_alocacao);

                    //printf("Instrução %c executada com sucesso no processo %d\n", current_instruction.instruction_char, process->id);

                    //printf("\nNovo processo criado! Tabela de processos:");
                    show_process_table(process_manager->process_table);

                    //printf("\nProcessos em execução no momento, após a criação do novo processo: \n");
                    show_fila(&process_manager->ExecutionState);

                    //printf("\nCPU atribuída ao novo processo: ");
                    show_cpu(process_manager->cpu_list[index_free_cpu]);
                    fflush(stdout);
                    show_memory(*process_manager->cpu_list[index_free_cpu].memory);
                    break;

                case 'R':
                    // Substitui a imagem do processo atual
                    SimulatedProcess temp_simulated_process = initialize_simulated_process(current_instruction.arq_name, process->id);
                    process->simulated_process = temp_simulated_process;
                    process->program_counter = 0;

                    // Verifica se há CPU disponível
                    index_free_cpu = is_any_cpu_available(process_manager);
                    if (index_free_cpu != -1) {
                        run_selected_escalonador(process_manager, &process_manager->cpu_list[index_free_cpu], input_command_string, selected_escalonador, command_index, cpu->memory, vect, ult, type_alocacao);
                    } else {
                        run_selected_escalonador(process_manager, cpu, input_command_string, selected_escalonador, command_index, cpu->memory, vect, ult, type_alocacao);
                    }
                    fflush(stdout);
                    //printf("Instrução %c executada com sucesso no processo %d\n", current_instruction.instruction_char, process->id);
                    break;

                default:
                    //printf("Instrução desconhecida: %c\n", current_instruction.instruction_char);
                    break;
            }
            process->program_counter++;
            command_response = 0;
            break;
        }

        case 'I':
            // Mostrar o estado atual do sistema

            show_process_manager(process_manager);
            command_response = 1;
            break;

        case 'M':
            // Mostrar o estado do sistema e encerra a execução do sistema
            show_process_manager(process_manager);

            //printf("\n#================================#");
            //printf("\nEncerrando o sistema.\n");
            //printf("#================================#\n");
            command_response = 1;
            break;

        default:
            //printf("\nComando desconhecido: %c\n", current_command);
            command_response = 0;
            break;
    }

    if (process->program_counter >= process->simulated_process.instruction_quantity) {
        command_response = 0;
        //printf(RESET BOLD GREEN "\nProcesso %d finalizado.\n" RESET, process->id);
        fflush(stdout);
        show_process_manager(process_manager);

        remove_item_from_fila(&process_manager->ExecutionState);
        remove_process_from_table(process->id, &process_manager->process_table);
        clean_cpu(cpu);

        escalona_by_priority(process_manager, cpu, input_command_string, command_index);
    }

    return command_response;
}

void run_commands(ProcessManager *process_manager, char *input_command_string,
 int selected_escalonador, int *command_index, alocationVector *vect, last *ult, int type_alocacao, Memory *memory) {
    int index_free_cpu;
    int command_response = 0;

    //printf("Lista de comandos: %s\n", input_command_string);

    int cont = 0;
    char current_command = input_command_string[0];

    Cell *current_cell_execution_process;
    CPU *process_cpu;
    ItemProcess *current_process;
    CPU *cpu_first_process = &process_manager->cpu_list[0];

    current_cell_execution_process = process_manager->ExecutionState.start;

    if (current_cell_execution_process == NULL) {
        //printf("Nenhum processo inicial em execução.\n");
    } else {
        //printf("Processo inicial em execução: %d\n", current_cell_execution_process->item.process_table_index);
    }

    do {
        // show_fila(&process_manager->ExecutionState);

        current_command = input_command_string[*command_index];
        //printf("\nComando atual a ser executado: %c\nIndice do comando: %d\n", current_command, *command_index);
        fflush(stdout);
        // Verifica se há processos em alguma das filas
        verify_all_filas(process_manager);

        // Executa o comando para todos os processos em execução

        for (int i = 0; i < QUANT_CPU; i++) {
            process_cpu = &process_manager->cpu_list[i];
            if (process_cpu->actual_process == NULL) {
                //printf("\nCPU[%d] não possui processo em execução.\n", i);
                fflush(stdout);
                continue;
            } else {
                current_process = process_cpu->actual_process;

                //printf(BOLD BLUE "\nProcesso em execução no momento: %d" RESET, current_process->id);
                // //printf("\nChar comando passado para o run_command_: %c\n", current_command);

                command_response = run_command_in_selected_process(process_manager, process_cpu, current_process, current_command, selected_escalonador, input_command_string, command_index, vect, ult, type_alocacao, process_cpu->memory);
                fflush(stdout);
                if (command_response == 1) {
                    break;
                }
            }
        }

        *command_index = *command_index + 1;
    } while (current_command != 'M');
}

int is_any_cpu_available(ProcessManager *process_manager) {
    int selected_cpu_index = -1;
    int biggest_cpu_time = 0;

    for (int i = 0; i < QUANT_CPU; i++) {
        if (process_manager->cpu_list[i].actual_process == NULL) {
            //printf("\nCPU %d está disponível.\n", i);
            selected_cpu_index = i;
            return selected_cpu_index;
        }
        fflush(stdout);
    }

    //printf("Todas as CPUs estão ocupadas.\n");
    fflush(stdout);
    for (int j = 0; j < QUANT_CPU; j++) {
        if (process_manager->cpu_list[j].actual_process->cpu_time > biggest_cpu_time) {
            selected_cpu_index = j;
        }
    }

    //printf("CPU %d será selecionada para troca de contexto.\n", selected_cpu_index);
    fflush(stdout);
    return selected_cpu_index;
}

int get_time_slice_by_priority(int priority) {
    switch (priority) {
        case 0:
            return 10;  // Maior prioridade
        case 1:
            return 20;
        case 2:
            return 30;
        case 3:
            return 40;  // Menor prioridade
        default:
            return 30;
    }
}

void verify_all_filas(ProcessManager *process_manager) {
    if (is_fila_empty(process_manager->ExecutionState) && is_fila_empty(process_manager->ReadyState) && is_fila_empty(process_manager->BlockedState)) {
        // Se não houver processos em execução, pronto ou bloqueado
        //printf("\nNão há processos em execução, prontos ou bloqueados.\n");
    } else {
        if (!is_fila_empty(process_manager->ExecutionState)) {
            //printf("Há processos em Execução.\n");
        }
        if (!is_fila_empty(process_manager->ReadyState)) {
            //printf("Há processos Prontos.\n");
        }
        if (!is_fila_empty(process_manager->BlockedState)) {
            //printf("Há processos Bloqueados.\n");
            // Verifica se há processos bloqueados e se o tempo de bloqueio acabou
            verify_process_block_time(process_manager);
        }
        fflush(stdout);
    }
    return;
}

void verify_process_block_time(ProcessManager *process_manager) {
    for (int k = 0; k < MAX_TAM; k++) {
        if (process_manager->process_table.item_process[k].process_state == Bloqueado) {
            if (process_manager->process_table.item_process[k].simulated_process.time_blocked > 0) {
                process_manager->process_table.item_process[k].simulated_process.time_blocked--;

                //printf("\nProcesso %d ainda bloqueado.\n", process_manager->process_table.item_process[k].simulated_process.time_blocked);
                //printf("Tempo de bloqueio restante: %d\n", process_manager->process_table.item_process[k].simulated_process.time_blocked);
                fflush(stdout);
            } else {
                // Se o tempo bloqueado acabou, o processo é movido para a fila de processos prontos
                TypeItem new_item = {k, &process_manager->process_table.item_process[k].priority};

                process_manager->process_table.item_process[k].process_state = Pronto;
                add_item_to_fila(new_item, &process_manager->ReadyState);
            }
        }
    }
}

CPU *get_cpu_by_process_id(ProcessManager *process_manager, int process_id) {
    for (int i = 0; i < QUANT_CPU; i++) {
        if (process_manager->cpu_list[i].actual_process != NULL &&
            process_manager->cpu_list[i].actual_process->id == process_id) {
            //printf("CPU encontrada para o processo: %d\n", process_id);
            return &process_manager->cpu_list[i];
        }
    }

    //printf("Nenhuma CPU encontrada para o processo: %d\n", process_id);
    return NULL;
}

void show_process_manager(ProcessManager *process_manager) {
    //printf(BOLD CYAN "\n\nGERENCIADOR DE PROCESSOS: \n" RESET);

    show_process_table(process_manager->process_table);

    for (int j = 0; j < QUANT_CPU; j++) {
        //printf("\n-> -> __ CPU %d: __ <- <-\n", j);
        show_cpu(process_manager->cpu_list[j]);
    }

    show_memory(*process_manager->cpu_list[0].memory);

    //printf("\n-> -> __ Processos bloqueados: __ <- <-");
    show_fila(&process_manager->BlockedState);

    //printf("\n-> -> __ Processos prontos: __ <- <-");
    show_fila(&process_manager->ReadyState);

    //printf("\n-> -> __ Processos em execução: __ <- <-");
    show_fila(&process_manager->ExecutionState);
    fflush(stdout);
}
