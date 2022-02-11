/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Implementação de produtor/consumidor utilizando semáforo - Laboratório 10*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define N 5

// Variaveis globais
sem_t slotCheio, slotVazio; //condicao
sem_t mutexProd, mutexCons; //exclusao mutua

int contador = N, qtd_cons, qtd_prod;
int Buffer[N];

void Insere (int item) {
    //aguarda um buffer vazio
    sem_wait(&slotVazio);
    //exclusao mutua entre produtores
    sem_wait(&mutexProd);
    for (int in = 0; in < N; in++){
        Buffer[in] = item;
    }

    contador = N;
    sem_post(&mutexProd);
    //sinaliza um buffer cheio
    for (int i=0; i < N; i++){
        sem_post(&slotCheio);
    }
}

int Retira (void) {
    int item;
    static int out=0;
    //aguarda slot cheio
    sem_wait(&slotCheio);
    //exclusao mutua entre consumidores
    sem_wait(&mutexCons);
    item = Buffer[out];
    out = (out + 1) % N;
    contador--;
    sem_post(&mutexCons);
    //sinaliza um slot vazio
        if(contador == 0){
            sem_post(&slotVazio);
        } 
    return item;
}

void *produtor(void * arg) {
    int elemento = 2;
    int id;
    id = *(int*) arg;
    while(1){
        //produz o buffer completo....
        printf("Entrei no produtor %d\n", id);
        Insere(elemento);
        printf("Sai do produtor %d\n", id);
    }
    pthread_exit(NULL);
}

void *consumidor(void * arg) {
    int elemento, id;
    id = *(int*) arg;
    while(1){
        printf("Entrei no consumidor %d\n", id);
        elemento = Retira();
        printf("Sai do consumidor %d\n", id);
        //consome o elemento....
    }
    pthread_exit(NULL);
}

/*void imprime(void ){
        printf("Buffer: ");
    for(int i=0; i < N; i++){
        printf("%d - ", Buffer[i]);
    }
    printf("\n");
}*/

//funcao principal
int main(int argc, char *argv[]) {
    
    pthread_t *tid_cons, *tid_prod;

    sem_init(&mutexCons, 0, 1);
    sem_init(&mutexProd, 0, 1);
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, 1);

    if(argc < 3) {
        printf("Digite: %s <numero de threads consumidoras> <numero de threads produtoras>\n", argv[0]);
        return 1;
    }

    qtd_cons = atoi(argv[1]);
    qtd_prod = atoi(argv[2]);
    

    tid_cons = (pthread_t*) malloc(sizeof(pthread_t)*qtd_cons);
    if(tid_cons==NULL) {puts("ERRO--malloc"); return 2;}
    tid_prod = (pthread_t*) malloc(sizeof(pthread_t)*qtd_prod);
    if(tid_prod==NULL) {puts("ERRO--malloc"); return 2;}


    /* Cria as threads*/
    for(int i=0;i<qtd_prod;i++){
        if(pthread_create(&tid_prod[i], NULL, produtor, (void *) &i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }

    for(int i=0;i<qtd_cons;i++){
        if(pthread_create(&tid_cons[i], NULL, consumidor, (void *) &i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }   
    }
    
    /* Desaloca variaveis*/
    sem_destroy(&mutexCons);
    sem_destroy(&mutexProd);
    sem_destroy(&slotCheio);
    sem_destroy(&slotVazio);

    free(tid_cons);
    free(tid_prod);

    pthread_exit(NULL);
}