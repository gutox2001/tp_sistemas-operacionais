#include "../libs/memory_alocation.h"
#include "../libs/memory.h"
int nos_atravessados=0;

int *first_fit(Memory *memory, int sizeneeded){
    int position_found = -1;
    int index = 0;
    int i = 0;
    int *beginend= (int *)calloc(2,sizeof(int));
    while(index<(sizeof(memory->data)/sizeof(int))){
        if(memory->data[index] == -1){
            nos_atravessados++;
            position_found = index;
            for(i = index; i<(index + sizeneeded); i++){
                if(memory->data[index]!=-1){
                    position_found = -1;
                    break;
                }
            }
            if(i==sizeneeded){
                /*for(i = index; i<(index + sizeneeded); i++){
                memory->data[i]=1;
                }*/
                beginend[0]=position_found;
                beginend[1]=position_found+sizeneeded;
                return beginend;


            }
            index=i+1;
            i=0;
            
        }
    }
    return beginend;

}
int *next_fit(Memory *memory, int sizeneeded, last *lastfit){
    int start_index = lastfit->lastfit;
    int *beginend= (int *)calloc(2,sizeof(int));
    
    // Percorre a memória a partir do último índice usado
    for (int i = start_index; i < (sizeof(memory->data)/sizeof(int)); i++)
    {
        if (memory->data[i] == -1)
        {
            nos_atravessados++;
            int can_allocate = 1;
            for (int j = i; j < i + sizeneeded && j < (sizeof(memory->data)/sizeof(int)); j++)
            {
                if (memory->data[j] != -1)
                {
                    can_allocate = -1;
                    break;
                }
            }
            if (can_allocate==1)
            {
                /*for (int j = i; j < i + sizeneeded && j < (sizeof(memory->data)/sizeof(int)); j++)
                {
                    memory->data[j] = 1;  // Aloca a memória (ex: 1 indica memória alocada)
                }*/
                lastfit->lastfit = i + sizeneeded;
                beginend[0]=i;
                beginend[1]=lastfit->lastfit; //se você não intendeu não se preocupe
                return beginend;
                
            }
        }
    }
    
    // Se não conseguiu alocar, começa do início da memória
    for (int i = 0; i < start_index; i++)
    {
        if (memory->data[i] == -1)
        {
            nos_atravessados++;
            int can_allocate = 1;
            for (int j = i; j < i + sizeneeded && j < (sizeof(memory->data)/sizeof(int)); j++)
            {
                if (memory->data[j] != -1)
                {
                    can_allocate = -1;
                    break;
                }
            }
            if (can_allocate==1)
            {
                /*for (int j = i; j < i + sizeneeded && j < (sizeof(memory->data)/sizeof(int)); j++)
                {
                    memory->data[j] = 1;  // Aloca a memória
                }*/
                beginend[0]=i;
                beginend[1]=lastfit->lastfit; //se você não intendeu não se preocupe
                return beginend;
            }
        }
    }
    
    return beginend; // Falha na alocação
}

int *best_fit(Memory *memory, int sizeneeded){
    int best_index = -1;
    int best_size = (sizeof(memory->data)/sizeof(int)) + 1;
    int *beginend= (int *)calloc(2,sizeof(int));

    for (int i = 0; i < (sizeof(memory->data)/sizeof(int)); i++)
    {
        if (memory->data[i] == -1)
        {
            nos_atravessados++;
            int free_block_size = 0;
            int j = i;
            while (j < (sizeof(memory->data)/sizeof(int)) && memory->data[j] == -1)
            {
                free_block_size++;
                j++;
            }
            if (free_block_size >= sizeneeded && free_block_size < best_size)
            {
                best_size = free_block_size;
                best_index = i;
            }
            i = j;
        }
    }
    if (best_index != -1)
    {
        for (int i = best_index; i < best_index + sizeneeded; i++)
        {
            //memory->data[i] = 1;  // Aloca a memória
                beginend[0]=i;
                beginend[1]=i+sizeneeded; //se você não intendeu não se preocupe
                return beginend;
        }
    }
    return beginend; // Falha na alocação
}
int *worst_fit(Memory *memory, int sizeneeded){
    int worst_index = -1;
    int worst_size = -1;
    int *beginend= (int *)calloc(2,sizeof(int));
    for (int i = 0; i < (sizeof(memory->data)/sizeof(int)); i++)
    {
        if (memory->data[i] == -1)
        {
            nos_atravessados++;
            int free_block_size = 0;
            int j = i;
            while (j < (sizeof(memory->data)/sizeof(int)) && memory->data[j] == -1)
            {
                free_block_size++;
                j++;
            }
            if (free_block_size >= sizeneeded && free_block_size > worst_size)
            {
                worst_size = free_block_size;
                worst_index = i;
            }
            i = j;
        }
    }
    if (worst_index != -1)
    {
        /*for (int i = worst_index; i < worst_index + sizeneeded; i++)
        {
            memory->data[i] = 1;  // Aloca a memória
        }*/
        beginend[0]=worst_index;
        beginend[1]=worst_index+sizeneeded; //se você não intendeu não se preocupe
        return beginend;
    }
    return beginend; // Falha na alocação
}

int alocation_manager(Memory *mem, ItemProcess process, alocationVector *alocvect, last *ult, int type_alocacao) {
    //int choice;
    int *result = NULL;

    // Inicializa a estrutura last
    //ult->lastfit = 10;
    
    // Definição da quantidade de processos
    //process.simulated_process.int_quantity = 3;
    
    // Aloca memória para o resultado
    result = (int*)calloc(2, sizeof(int));
    result[0]=-1;


    // Seleciona a estratégia de alocação
    switch (type_alocacao) {
        case 1:
            result = first_fit(mem, process.simulated_process.int_quantity);
            break;
        case 2:
            result = next_fit(mem, process.simulated_process.int_quantity, ult);
            break;
        case 3:
            result = best_fit(mem, process.simulated_process.int_quantity);
            break;
        case 4:
            result = worst_fit(mem, process.simulated_process.int_quantity);
            break;
        default:
            puts("Erro de escolha de alocacao");
            return -1;
    }

    // Verifica se a alocação foi bem-sucedida
    if (result[0] == -1) {
        puts("Falha na alocação");
        return -1;
    }

    // Exibe o estado atual da memória
    printf("bolo %d Estado atual da memória:\n", nos_atravessados);
    for (int i = 0; i < MAX_TAM * 3; i++) {
        if (alocvect->endressAdress[i] == -1) {
            alocvect->endressAdress[i] = process.id;
            alocvect->endressAdress[i + 1] = result[0];
            alocvect->endressAdress[i + 2] = result[1];

            printf("Resultado da alocação id: %d \n", alocvect->endressAdress[i]);
            printf("Resultado da alocação índice: %d \n", alocvect->endressAdress[i + 1]);
            printf("Resultado da alocação fim: %d \n", alocvect->endressAdress[i + 2]);
            break;
        }
    }

    // Libera a memória alocada
    free(result);

    return 0;
}

int deallocation_manager(Memory *mem, int process_id, alocationVector *alocvect) {
    int start_index = -1;
    int end_index = -1;

    // Find the allocation entry for the given process_id
    for (int i = 0; i < MAX_TAM * 3; i += 3) {
        if (alocvect->endressAdress[i] == process_id) {
            start_index = alocvect->endressAdress[i + 1];
            end_index = alocvect->endressAdress[i + 2];
            
            // Clear the allocation entry
            alocvect->endressAdress[i] = -1;
            alocvect->endressAdress[i + 1] = -1;
            alocvect->endressAdress[i + 2] = -1;
            
            break;
        }
    }

    if (start_index == -1 || end_index == -1) {
        printf("ID do processo %d não encontrado no vetor de alocação.\n", process_id);

        return -1;
    }

    // Deallocate memory
    for (int i = start_index; i <= end_index; i++) {
        mem->data[i] = 0;  // Assuming 0 represents free memory
    }

    printf("Memória desalocada para o processo ID %d do índice %d ao %d\n", process_id, start_index, end_index);


    // Print current memory state
    printf("Estado atual da memória:\n");
    for (int i = 0; i < MAX_TAM; i++) {
        printf("%d ",mem->data[i]);
    }
    printf("\n");

    return 0;
}




// Função para calcular o tempo médio de alocação
double calculate_average_allocation_time(int num_allocations) {
    if (num_allocations == 0) {
        printf("Nenhuma alocação foi feita.\n");
        // Se não houver alocações, o tempo médio não pode ser calculado
        return 0.0;
    }

    // Calcula o tempo médio
    double average_nodes_traversed = (double)nos_atravessados / num_allocations;
    
    return average_nodes_traversed;  // Retorna o tempo médio de alocação
}



/*
int main(){
    Memory mem;
    ItemProcess item;

    alocationVector vect;
    last ult;
    for (int i = 0; i < MAX_TAM * 3; i++){
        vect.endressAdress[i]=-1;
    }
    initialize_memory(&mem);
    alocation_manager(&mem,item,&vect,&ult);
 

    return 0;
}*/

