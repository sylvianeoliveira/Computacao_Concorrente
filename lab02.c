/* Multiplicacao de matriz-vetor (considerando matrizes quadradas) */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

int *mat1; //matriz 1 de entrada
int *mat2; //matriz 2 de entrada
int *saida_conc; //matriz de saida
int *saida_seq; //matriz de saida calculada de forma sequencial
int nthreads; //numero de threads
int dim; //dimensao das estruturas de entrada


//funcao que as threads executarao

void * tarefa(void *arg) {
    int argx = * (int*) arg;

    for(int i = argx ; i < dim; i+=nthreads){
        for(int j=0; j < dim; j++){
            for(int k=0; k < dim; k++){
                saida_conc[i*dim+j] += mat1[i*dim+k] * mat2[k*dim+j];
            }
        }
    }
    pthread_exit(NULL);
}

void mult_matriz_seq(void){
    int i, j, k;
    for(i=0; i < dim; i++){
        for(j=0; j < dim; j++){
            for(k=0; k < dim; k++){
                saida_seq[i*dim+j] += mat1[i*dim+k] * mat2[k*dim+j];
            }
        }
    }
}

//fluxo principal
int main(int argc, char* argv[]) {
    pthread_t *tid; //identificadores das threads no sistema
    int *id; //identificadores locais das threads e dimensao

    double inicio, fim, delta;
   
    if(argc < 3) {
        printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > dim) nthreads=dim;

    //alocacao de memoria para as estruturas de dados
    mat1 = (int *) malloc(sizeof(int) * dim * dim);
    if (mat1 == NULL) {printf("ERRO--malloc\n"); return 2;}
    mat2 = (int *) malloc(sizeof(int) * dim * dim);
    if (mat2 == NULL) {printf("ERRO--malloc\n"); return 2;}
    saida_conc = (int *) malloc(sizeof(int) * dim * dim);
    if (saida_conc == NULL) {printf("ERRO--malloc\n"); return 2;}
    saida_seq = (int *) malloc(sizeof(int) * dim * dim);
    if (saida_seq == NULL) {printf("ERRO--malloc\n"); return 2;}

    //inicializacao das estruturas de dados de entrada e saida

    for(int i=0; i < dim; i++) {
        for(int j=0; j < dim; j++){
            mat1[i*dim+j] = rand() % 100;
            mat2[i*dim+j] = rand() % 100;
            saida_conc[i*dim+j] = 0;
            saida_seq[i*dim+j] = 0;
        }
    }

    //multiplicacao da matriz pelo vetor de forma sequencial
    GET_TIME(inicio);

    mult_matriz_seq();
   
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo Sequencial: %lf\n", delta);

    //multiplicacao da matriz pelo vetor de forma concorrente

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
            puts("ERRO--pthread_create"); return 3;
        }
    }
   
    //espera pelo termino da threads
    
    for(int i=0; i<nthreads; i++) {
        pthread_join(*(tid+i), NULL);
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo Concorrente: %lf\n", delta);

    //corretude
    for(int i=0; i < dim; i++){
        for (int j=0; j < dim; j++){
            if(saida_seq[i*dim+j] != saida_conc[i*dim+j]) printf("Erro no elemento [%d][%d] da matriz de saída.\n", i, j);
        }  
    }
  
    //liberacao da memoria
    free(mat1);
    free(mat2);
    free(saida_conc);
    free(saida_seq);
    free(id);
    free(tid);
    GET_TIME(fim);

    return 0;
}
