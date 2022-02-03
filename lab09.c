/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */
/* Codigo: Imprimir as frases das threads na ordem correta. Primeiramente a thread 5
 e por último a thread 1, sendo as threads 2, 3 e 4 executadas entre as duas.*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS  5

/* Variaveis globais */
int x=0;
sem_t em, condinter, condfinal;     //semaforos para sincronizar a ordem de execucao das threads

/* Threads que imprime Volte sempre! */
void *A (void *t) {
    sem_wait(&condfinal);

    sem_wait(&em); //entrada na secao critica
    printf("Volte sempre!\n");
    sem_post(&em); //saida da secao critica

    pthread_exit(NULL);
}

/* Thread que imprime Fique a vontade. */
void *B (void *t) {
    sem_wait(&condinter);

    sem_wait(&em); //entrada na secao critica
    printf("Fique a vontade.\n");
    x++;
    sem_post(&em); //saida da secao critica

    sem_post(&condinter);
    if(x == 3) sem_post(&condfinal);

    pthread_exit(NULL);
}

/* Thread que imprime Sente-se por favor. */
void *C (void *t) {
    sem_wait(&condinter);

    sem_wait(&em); //entrada na secao critica
    printf("Sente-se por favor.\n");
    x++;
    sem_post(&em); //saida da secao critica

    sem_post(&condinter);
    if(x == 3) sem_post(&condfinal);

    pthread_exit(NULL);
}

/* Thread que imprime Aceita um copo d’agua? */
void *D (void *t) {
    sem_wait(&condinter);

    sem_wait(&em); //entrada na secao critica
    printf("Aceita um copo d’agua?\n");
    x++;
    sem_post(&em); //saida da secao critica

    sem_post(&condinter);
    if(x == 3) sem_post(&condfinal);
 
    pthread_exit(NULL);
}

/* Thread que imprime Seja bem-vindo!! */
void *E (void *t) {
    printf("Seja bem-vindo!\n");
    sem_post(&condinter);

    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
    int i; 
    pthread_t threads[NTHREADS];


    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    sem_init(&em, 0, 1);
    sem_init(&condinter, 0, 0);
    sem_init(&condfinal, 0, 0);

    /* Cria as threads */
    pthread_create(&threads[0], NULL, A, NULL);
    pthread_create(&threads[1], NULL, B, NULL);
    pthread_create(&threads[2], NULL, C, NULL);
    pthread_create(&threads[3], NULL, D, NULL);
    pthread_create(&threads[4], NULL, E, NULL);
    /* Espera todas as threads completarem */
    for (i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Desaloca variaveis*/
    sem_destroy(&em);
    sem_destroy(&condinter);
    sem_destroy(&condfinal);
}
