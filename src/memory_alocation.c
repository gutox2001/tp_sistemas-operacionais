#include "../libs/memory_alocation.h"
#include "../libs/memory.h"
int nos_atravessados=0;








int *first_fit(Memory *memory, int sizeneeded, alocationVector *alocvect){
    int position_found = -1;
    int index = 0;
    int i = 0;
    int flag = 0;
    int *beginend= (int *)calloc(2,sizeof(int));
    while(index<(sizeof(memory->data)/sizeof(int))){
        if(memory->data[index] == -1){
            for(int i = 1;i < 300;i+=3){ //e assim mesmo confia
                if(alocvect->endressAdress[i] < index && alocvect->endressAdress[i+1] >index){
                    flag=-1;
                    index=alocvect->endressAdress[i+1] +1;
                }
            }
            if(flag != -1){
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
            flag=0;
            
        }
    }
    return beginend;

}
int *next_fit(Memory *memory, int sizeneeded, last *lastfit, alocationVector *alocvect){
    int start_index = lastfit->lastfit;
    int *beginend= (int *)calloc(2,sizeof(int));
    int flag = 0;
    // Percorre a memória a partir do último índice usado
    for (int i = start_index; i < (sizeof(memory->data)/sizeof(int)); i++)
    {
        if (memory->data[i] == -1)
        {
            for(int j = 1;j < 300;j+=3){ //e assim mesmo confia
                if(alocvect->endressAdress[j] < i && alocvect->endressAdress[j+1] >i){
                    flag=-1;
                    i=alocvect->endressAdress[j+1] +1;
                }
            }
            if(flag != -1){
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
            flag=0;
        }
    }
    flag = 0;
    // Se não conseguiu alocar, começa do início da memória
    for (int i = 0; i < start_index; i++)
    {
        if (memory->data[i] == -1)
        {

            for(int x = 1;x < 300;x+=3){ //e assim mesmo confia
                if(alocvect->endressAdress[x] < i && alocvect->endressAdress[x+1] >i){
                    flag=-1;
                    i=alocvect->endressAdress[x+1] +1;
                }
            }
            if(flag != -1){

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

            }flag = 0;

            
        }
    }
    
    return beginend; // Falha na alocação
}

int *best_fit(Memory *memory, int sizeneeded, alocationVector *alocvect){
    int best_index = -1;
    int best_size = (sizeof(memory->data)/sizeof(int)) + 1;
    int *beginend= (int *)calloc(2,sizeof(int));
    int flag = 0;
    for (int i = 0; i < (sizeof(memory->data)/sizeof(int)); i++)
    {
        if (memory->data[i] == -1)
        {
            for(int j = 1;j < 300;j+=3){ //e assim mesmo confia
                if(alocvect->endressAdress[j] < i && alocvect->endressAdress[j+1] >i){
                    flag=-1;
                    i=alocvect->endressAdress[j+1] +1;
                }
            }
            if(flag!=-1){
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
            flag=0;

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
int *worst_fit(Memory *memory, int sizeneeded, alocationVector *alocvect){
    int worst_index = -1;
    int worst_size = -1;
    int *beginend= (int *)calloc(2,sizeof(int));
    int flag = 0;
    for (int i = 0; i < (sizeof(memory->data)/sizeof(int)); i++)
    {
        if (memory->data[i] == -1)
        {

            for(int j = 1;j < 300;j+=3){ //e assim mesmo confia
                if(alocvect->endressAdress[j] < i && alocvect->endressAdress[j+1] >i){
                    flag=-1;
                    i=alocvect->endressAdress[j+1] +1;
                }
            }
            if(flag!=-1){
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
            flag = 0;


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

int alocation_manager(Memory *mem, ItemProcess process, alocationVector *alocvect, last *ult, int type_alocacao, ProcessTable *process_table, TypeFila *fila_prontos, TypeFila *fila_bloqueados, TypeFila *fila_execucao) {
    //int choice;
    int *result = NULL;
    int temp_program_counter = 0;
    ItemProcess temp_item_process;
    temp_item_process.id = -1;
    

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
            result = first_fit(mem, process.simulated_process.int_quantity,alocvect);
            break;
        case 2:
            result = next_fit(mem, process.simulated_process.int_quantity, ult,alocvect);
            break;
        case 3:
            result = best_fit(mem, process.simulated_process.int_quantity,alocvect);
            break;
        case 4:
            result = worst_fit(mem, process.simulated_process.int_quantity,alocvect);
            break;
        default:
            puts("Erro de escolha de alocacao");
            return -1;
    }

    // Verifica se a alocação foi bem-sucedida
    if (result[0] == -1) {
        // Buscar processo mais antigo
        int index_process_at_process_table = remove_item_from_fila(fila_bloqueados);
        if(index_process_at_process_table == -1) {
            index_process_at_process_table = remove_item_from_fila(fila_prontos);
        }
        if(index_process_at_process_table == -1) {
            index_process_at_process_table = remove_item_from_fila(fila_execucao);
        }
        temp_item_process = process_table->item_process[index_process_at_process_table];
        printf("Processo mais antigo: %d\n", temp_item_process.id);
        // Retirar da memória
        deallocation_manager(mem, temp_item_process.id, alocvect);
        // Chamar alocationManager novamente
        alocation_manager(mem, process, alocvect, ult, type_alocacao, process_table, fila_prontos, fila_bloqueados, fila_execucao);
        // TODO Salvar processo na lista de prontos
        // TODO Descobrir de o processo pronto está na memória
        // TODO Alocar processo na memória
        puts("Falha na alocação");
        return -1;
    }

    // Exibe o estado atual da memória
    printf("Estado atual da memória:\n");
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


void escreverNoArquivo(const char *conteudo) {
    // Nome do arquivo
    const char *nomeDoArquivo = "D.txt";
    // Abre o arquivo no modo de escrita (write), cria o arquivo se não existir
    FILE *arquivo = fopen(nomeDoArquivo, "a");
    
    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita.\n", nomeDoArquivo);
        return;
    }

    // Escreve o conteúdo no arquivo
    fprintf(arquivo, "%s", conteudo);

    // Fecha o arquivo
    fclose(arquivo);
    printf("Conteúdo escrito no arquivo %s com sucesso.\n", nomeDoArquivo);
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

