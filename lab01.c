/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: Elevar ao quadrado cada elemento de um vetor com 10000 elementos usando 2 threads, além da main, em C. */

#include <stdio.h>
#include <pthread.h>

#define NELEMENTOS 10000 // Número de elementos do vetor
#define NTHREADS 2 // Número de threads além da main
#define NINICIALIZA 5 // Número que cada posição do vetor irá conter

int vetor[NELEMENTOS];

//Função que a thread irá executar
void * calc_quadrado (void * arg){
   int elemento = * (int *) arg;
   for(int i = elemento; i < NELEMENTOS; i+=2){
      vetor[i] = vetor[i]*vetor[i]; // Substitui o resultado da operação na posição correta
   }

   pthread_exit(NULL);
}

// Função principal
int main(void){
   pthread_t tid[NTHREADS]; // Identificador da thread no sistema
   int ident[NTHREADS]; // Identificador local da thread
   int i, resultado;

   for(i = 0; i < NELEMENTOS; i++){
      vetor[i] = NINICIALIZA; // Inicializa o vetor com elemento 5 em cada posição
   }

   // Cria as threads 
   for(i=0; i<NTHREADS; i++){
      ident[i] = i; // Passa o número 0 e 1 como parâmetro para percorrer este vetor em posições pares e ímpares
      if (pthread_create(&tid[i], NULL, calc_quadrado, (void *)&ident[i])) 
         printf("ERRO -- pthread_create\n");
   }

   // Espera as threads terminarem 
   for(i=0; i<NTHREADS; i++){
      if (pthread_join(tid[i], NULL)) 
         printf("ERRO -- pthread_join\n");
   }

   // Verifica se o vetor de elementos está correto após as operações
   resultado = NINICIALIZA * NINICIALIZA;

   for(i=0; i<NELEMENTOS; i++){
      if(vetor[i] != resultado){
         printf("Sua operação deu erro na posição %d.\n", i);
      }
   }
   return 0;
}
