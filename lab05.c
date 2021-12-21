/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */
/* Codigo: Imprimir as frases das threads na ordem correta. Primeiramente a thread 5 e por último a thread 1 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  5

/* Variaveis globais */
int x = 0;
int y = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond, x_cond2;

/* Threads que imprime Volte sempre! */
void *A (void *t) {
  
    pthread_mutex_lock(&x_mutex);
    y++;
    while (y != 5){ // // Fica em bloqueio enquanto todas as thread não forem executadas
        pthread_cond_wait(&x_cond2, &x_mutex);
    }
 
    printf("Volte sempre!\n");

    pthread_mutex_unlock(&x_mutex); 
    pthread_exit(NULL);
}

/* Thread que imprime Fique a vontade. */
void *B (void *t) {

    pthread_mutex_lock(&x_mutex);
    while (x < 1){ // Fica em bloqueio enquanto a thread 5 não for executada
        pthread_cond_wait(&x_cond, &x_mutex);
    }

    printf("Fique a vontade.\n");
    y++;
    if (y == 5){ // Se todas as threads já foram impressas, libera a thread 1
        pthread_cond_signal(&x_cond2);
    }

    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

/* Thread que imprime Sente-se por favor. */
void *C (void *t) {

    pthread_mutex_lock(&x_mutex);
    while (x < 1){ // Fica em bloqueio enquanto a thread 5 não for executada
        pthread_cond_wait(&x_cond, &x_mutex);
    }

    printf("Sente-se por favor.\n");
    y++;
    if(y == 5){ // Se todas as threads já foram impressas, libera a thread 1
        pthread_cond_signal(&x_cond2);
    }
    pthread_mutex_unlock(&x_mutex); 
    pthread_exit(NULL);
}

/* Thread que imprime Aceita um copo d’agua? */
void *D (void *t) {
    pthread_mutex_lock(&x_mutex);

    while (x < 1){ // Fica em bloqueio enquanto a thread 5 não for executada
        pthread_cond_wait(&x_cond, &x_mutex);
    }

    printf("Aceita um copo d’agua?\n");

    y++;
    if(y == 5){ // Se todas as threads já foram impressas, libera a thread 1
        pthread_cond_signal(&x_cond2);
    }

    pthread_mutex_unlock(&x_mutex); 
    pthread_exit(NULL);
}

/* Thread que imprime Seja bem-vindo!! */
void *E (void *t) {
    printf("Seja bem-vindo!\n");

    pthread_mutex_lock(&x_mutex);
    x++;
    y++;
    if(y == 5){ // Se todas as threads já foram impressas, libera a thread 1
        pthread_cond_signal(&x_cond2);
    }

    pthread_cond_broadcast(&x_cond);
    
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
    int i; 
    pthread_t threads[NTHREADS];


    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init (&x_cond, NULL);
    pthread_cond_init (&x_cond2, NULL);

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

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    pthread_cond_destroy(&x_cond2);
}
