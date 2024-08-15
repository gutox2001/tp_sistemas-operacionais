#include "../libs/process_manager.h"

void initialize_process_manager(ProcessManager *process_manager, CPU *cpu_list) {
    process_manager->cpu_list = cpu_list;
    initialize_fila(&process_manager->ReadyState);
    initialize_fila(&process_manager->BlockedState);
    initialize_fila(&process_manager->ExecutionState);
    initialize_process_table(&process_manager->process_table);
}

void run_process(ProcessManager *process_manager) {
    int i;
    int index_process_table;
    ItemProcess *process;
    CPU *cpu;
    TypeItem item;

    for (i = 0; i < QUANT_CPU; i++) {
        cpu = &process_manager->cpu_list[i];
        if (is_cpu_empty(cpu)) {
            if (!is_fila_empty(process_manager->ReadyState)) {
                index_process_table = remove_item_from_fila(&process_manager->ReadyState);
                process = get_process(&process_manager->process_table, index_process_table);
                add_process_to_cpu(cpu, process);
            }
        } else {
            if (cpu->used_time == cpu->quantum_time) {
                process = cpu->actual_process;
                item.index_process_table = process->simulated_process.process_id;
                add_item_to_fila(item, &process_manager->ReadyState);
                clean_cpu(cpu);
            } else {
                cpu->used_time++;
            }
        }
    }
}