#include "../libs/memory_alocation.h"


int *first_fit(Memory *memory, int sizeneeded){
    int position_found = -1;
    int index = 0;
    int i = 0;
    int *beginend= (int *)calloc(2,sizeof(int));
    while(index<(sizeof(memory->data)/sizeof(int))){
        if(memory->data[index] == -1){
            position_found = index;
            for(i = index; i<(index + sizeneeded); i++){
                if(memory->data[index]!=-1){
                    position_found = -1;
                    break;
                }
            }
            if(i==sizeneeded){
                for(i = index; i<(index + sizeneeded); i++){
                memory->data[i]=1;
                }
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
                for (int j = i; j < i + sizeneeded && j < (sizeof(memory->data)/sizeof(int)); j++)
                {
                    memory->data[j] = 1;  // Aloca a memória (ex: 1 indica memória alocada)
                }
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
                for (int j = i; j < i + sizeneeded && j < (sizeof(memory->data)/sizeof(int)); j++)
                {
                    memory->data[j] = 1;  // Aloca a memória
                }
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
            memory->data[i] = 1;  // Aloca a memória
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
        for (int i = worst_index; i < worst_index + sizeneeded; i++)
        {
            memory->data[i] = 1;  // Aloca a memória
        }
        beginend[0]=worst_index;
        beginend[1]=worst_index+sizeneeded; //se você não intendeu não se preocupe
        return beginend;
    }
    return beginend; // Falha na alocação
}
int alocation_manager(Memory *mem, ItemProcess process, alocationVector *alocvect){
    int choice;
    last ult; // next fit
    ult.lastfit=10;
    int *result = NULL;
    process.simulated_process.int_quantity=3;
    puts("escolha uma das estrategias de alocação abaixo:");
    puts("1 - first fit");
    puts("2 - next fit");
    puts("3 - best fit");
    puts("4 - worst fit");
    scanf("%d",&choice);

    switch (choice)
    {
    case 1:
        result=first_fit(mem,process.simulated_process.int_quantity);
        break;
    case 2:
        result=next_fit(mem,process.simulated_process.int_quantity,&ult);
        break;
    case 3:
        result=best_fit(mem,process.simulated_process.int_quantity);
        break;
    case 4:
        result=worst_fit(mem,process.simulated_process.int_quantity);
        break;
    
    default:
        break;
    }

     if (result == NULL) {
        puts("Falha na alocação");
        return -1;
    }

    printf("Estado atual da memória:\n");
    for (int i = 0; i < MAX_TAM; i++) {
        printf("%d ", mem->data[i]);
        if(i%20==0 && i> 1){
            printf("\n");
        }
    }
  
    printf("Resultado da alocação: %d\n", *result);

    return 0;
}

int main(){
    Memory mem;
    ItemProcess item;
    

    initialize_memory(&mem);
    alocation_manager(&mem,item);
 

    return 0;
}

