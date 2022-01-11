/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Implementação e uso de sincronização por barreira - Laboratório 6*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Variaveis globais */
int dim, *vetor;
int bloqueadas = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

//funcao barreira
void barreira(int n_threads) {
    pthread_mutex_lock(&x_mutex); //inicio secao critica
    if (bloqueadas == (n_threads-1)) { 
        //ultima thread a chegar na barreira
        pthread_cond_broadcast(&x_cond);
        bloqueadas=0;
    }
    else {
        bloqueadas++;
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); //fim secao critica
}

//funcao das threads
void *tarefa (void *arg){
    int id = *(int*)arg;
    int *somatorio_local, i, j;
    somatorio_local = (int*) malloc(sizeof(int));
    if(somatorio_local==NULL) {exit(1);}

    for(j=0; j < dim; j++){
            printf("Thread %d: - Passo=%d\n", id, j);
        for (i=0; i < dim; i++) {
            *somatorio_local = (*somatorio_local) + vetor[i];
        }
        //sincronizacao condicional
        barreira(dim);
        
        srand(time(NULL));
        vetor[id] = (rand() % 10);
        barreira(dim);
    }
   //retorna o resultado da contagem local
   pthread_exit((void *) somatorio_local);
   }

/* Funcao principal */
int main(int argc, char *argv[]){
    pthread_t *threads;
    int *id;
    int *retorno, *resultado;

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init (&x_cond, NULL);

    if(argc < 2) {
        printf("Digite: %s <numero de elementos do vetor>\n", argv[0]);
        return 1;
    }

    dim = atoi(argv[1]);

    //alocacao de memoria para as estruturas de dados
    vetor = (int*) malloc(sizeof(int)*dim);
    if (vetor == NULL) {printf("ERRO--malloc\n"); return 2;}
    resultado = (int*) malloc(sizeof(int)*dim);
    if (vetor == NULL) {printf("ERRO--malloc\n"); return 2;}

    srand(time(NULL));
    //inicializacao das estruturas de dados de entrada e saida
    for(int i=0; i < dim; i++) {
        vetor[i] = (rand() % 10);
    }

    threads = (pthread_t*) malloc(sizeof(pthread_t)*dim);
    if(threads==NULL) {puts("ERRO--malloc"); return 2;}
    id = (int*) malloc(sizeof(int)*dim);
    if(id==NULL) {puts("ERRO--malloc"); return 2;}

    /* Cria as threads */
    for(int i=0;i<dim;i++){
        id[i]=i;
        pthread_create(&threads[i], NULL, tarefa, (void *) &id[i]);
    }

    /* Espera todas as threads completarem */
    for(int i = 0; i < dim; i++){
        pthread_join(threads[i], (void**) &retorno);
        resultado[i] = *retorno;
        printf("Somatório da thread %d: %d.\n", id[i], *retorno);
    }

    for(int i=0; i < dim; i++){
        if(resultado[0] != resultado[i]){
            printf("Os somatórios são diferentes.\n");
        }
    }

    printf ("FIM.\n");

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    return 0;
}
