/* Encontrar a quantidade de valores de um vetor de numeros reais que estao dentro de uma faixa de valores dada. */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *vetor; //vetor
long long int dim; //numero de elementos do vetor
float min; //limiar inferior da faixa
float max;; //limiar superior da faixa
int contador_seq; //numero de valores x que estao no intervalo calculado de forma sequencial
int contador_conc; //numero de valores x que estao no intervalo calculado de forma concorrente
int nthreads; //numero de threads do programa

//funcao que as threads executarao

void * tarefa(void *arg) {
    int *contador_local; //variavel local da contagem de elementos no intevalo
    contador_local = (int*) malloc(sizeof(int));
    if(contador_local==NULL) {exit(1);}

    long int argx = * (int*) arg;
    float elemento;
    for(long int i = argx ; i < dim; i+=nthreads){
        elemento = vetor[i];
        if (elemento > min && elemento < max){
            *contador_local= (*contador_local) + 1;
        }
    }
   //retorna o resultado da contagem local
   pthread_exit((void *) contador_local); 
}

void verifica__elemento_seq(void){
    int i;
    float elemento;
    for(i=0; i < dim; i++){
        elemento = vetor[i];
        if (elemento > min && elemento < max){
            contador_seq++;
        }
    }
}

//fluxo principal
int main(int argc, char* argv[]) {
    pthread_t *tid; //identificadores das threads no sistema
    int *id; //identificadores locais das threads e dimensao
    int *retorno;

    double inicio, fim, delta;
   
    if(argc < 3) {
        printf("Digite: %s <numero de elementos do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > dim) nthreads = dim;

    //alocacao de memoria para as estruturas de dados
    vetor = (float *) malloc(sizeof(float)*dim);
    if (vetor == NULL) {printf("ERRO--malloc\n"); return 2;}
    

    //inicializacao das estruturas de dados de entrada e saida

    for(int i=0; i < dim; i++) {
        vetor[i] = (rand() % 200 ) / 2.3;
    }

    printf("Insira o limiar inferior da faixa: ");
    scanf("%f", &min);

    printf("Insira o limiar superior da faixa: ");
    scanf("%f", &max);

    //encontra a quantidade de valores de forma sequencial
    GET_TIME(inicio);

    verifica__elemento_seq();
   
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo Sequencial: %lf\n", delta);

    //encontra a quantidade de valores de forma concorrente

    //alocacao das estruturas
    GET_TIME(inicio);

    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}
    id = (int*) malloc(sizeof(int)*nthreads);
    if(id==NULL) {puts("ERRO--malloc"); return 2;}
   
    //criacao das threads
    for(int i=0; i < nthreads; i++) {
        id[i] = i;
        
        if(pthread_create(tid+i, NULL, tarefa, (void*) (id+i))){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
   
    //espera pelo termino da threads
    
    for(int i=0; i<nthreads; i++) {
        if(pthread_join(*(tid+i), (void**) &retorno)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    //contador global
    contador_conc += *retorno;
    }

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo Concorrente: %lf\n", delta);

    //corretude
    if(contador_conc != contador_seq) printf("Erro!\n");

    //liberacao da memoria
    free(vetor);
    free(id);
    free(tid);
    
    return 0;
}
