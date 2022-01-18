/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 7 */
/* Codigo: Somando todos os elementos de um vetor em um programa multithreading Java */
/* -------------------------------------------------------------------*/

//classe da estrutura de dados (recurso) compartilhado entre as threads
class S {
    //recurso compartilhado
    private int somatorio;
    public final int N_THREADS;
    public final int[] vetor;

    //construtor
    public S(int[] vetor, int N_THREADS) { 
       this.somatorio = 0;
       this.vetor = vetor;
       this.N_THREADS = N_THREADS;
    }
  
    //operacao de leitura sobre o recurso compartilhado
    public synchronized int  get_somatorio() { 
       return this.somatorio; 
    }

    public synchronized void set_somatorio(int elemento){
        this.somatorio += elemento;
    }
  }
  
  //classe que estende Thread e implementa a tarefa de cada thread do programa 
  class T extends Thread {
     //identificador da thread
     private int id;
     //objeto compartilhado com outras threads
     S s;
    
     //construtor
     public T(int tid, S s) { 
        this.id = tid; 
        this.s = s;
     }
  
     //metodo main da thread
     public synchronized void run() {
        //System.out.println("Thread " + this.id + " iniciou!");
        for (int i= id; i < s.vetor.length; i+=s.N_THREADS) {
            s.set_somatorio(s.vetor[i]);
        }
        //System.out.println("Thread " + this.id + " terminou!"); 
     }
  }
  
  //classe da aplicacao
  class SomaElementoVetor {
     static int N_THREADS;
     static int QTD_ElEM;
     static final int VALOR_INICIAL = 5;
     static int somatorio;
  
    public static void main (String[] args) {
        if(args.length < 2) {
            System.out.println("Digite: java SomaElementoVetor <numero de threads> <numero de elementos do vetor>");
            return;
        }

        N_THREADS = Integer.parseInt(args[0]);
        QTD_ElEM = Integer.parseInt(args[1]);


        //reserva espaço para um vetor de threads
        Thread[] threads = new Thread[N_THREADS];
        int[] vetor = new int[QTD_ElEM];

        //inicializa o vetor
        for (int i=0; i<vetor.length; i++){
            vetor[i] = VALOR_INICIAL;
        }

        //cria uma instancia do recurso compartilhado entre as threads
        S s = new S(vetor, N_THREADS);
  
        //cria as threads da aplicacao
        for (int i=0; i<threads.length; i++) {
           threads[i] = new T(i, s);
        }
  
        //inicia as threads
        for (int i=0; i<threads.length; i++) {
           threads[i].start();
        }
  
        //espera pelo termino de todas as threads
        for (int i=0; i<threads.length; i++) {
           try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
  
        somatorio = s.get_somatorio();
        //verifica se o somatório está certo´
        if(somatorio == VALOR_INICIAL*QTD_ElEM){
            System.out.println("O valor do somatório está correto.");
            System.out.println("Valor do somatório = " + somatorio);
        }
        else{
            System.out.println("O valor do somatório está incorreto.");
            System.out.println("Valor do somatório esperado = " + VALOR_INICIAL*QTD_ElEM);
            System.out.println("Valor do somatório encontrado = " + somatorio);
        }
        
    }
  }