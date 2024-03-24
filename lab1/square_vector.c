#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int id;
    int thread;
    int *vector;
} vector_info;

int inputThreads();
int inputLength();
int *inputVector(int N);
void printVector(int *vector, int N);
void *squareVector(void *arg);

int main(){
    vector_info *vi;
    
    int M = inputThreads();
    int N = inputLength();

    if (M > N) M = N;
    
    int *temp_vector = inputVector(N);
    pthread_t tid[N];
    int current_thread = 0;

    for(int i = 0; i < N; i++){
        if(current_thread >= M) current_thread = 0;

        vi = malloc(sizeof(vector_info));
        vi->vector = temp_vector;
        vi->id = i;
        vi->thread = current_thread;

        if(pthread_create(&tid[current_thread], NULL, squareVector, (void *)vi)){
            puts("Erro: pthread_create");
            exit(1);
        }
        
        current_thread++;
    }

    for(int i = 0; i < M; i++){
        if(pthread_join(tid[i], NULL)){
            puts("Erro: pthread_join");
            exit(2);
        }
    }

    puts("\nResultado:");
    printVector(vi->vector, N);

    free(vi);
    pthread_exit(NULL);
    return 0;
}

int inputThreads(){
    int M;

    puts("Quantidade de threads:");
    scanf("%d", &M);

    return M;
}

int inputLength(){
    int N;

    puts("Tamanho do vetor:");
    scanf("%d", &N);

    return N;
}

int *inputVector(int N){
    int *vector = calloc(N, sizeof(int));

    puts("Escreva o seu vetor de inteiros, cada elemento é separado com [ESPAÇO]:");

    for(int i = 0; i < N; i++){
        scanf(" %d", &vector[i]);
    }

    return vector;
}

void printVector(int *vector, int N){
    for(int i = 0; i < N; i++){
        printf("%d ", vector[i]);
    }
    printf("\n");
}

void *squareVector(void *arg){
    vector_info *vi = (vector_info *) arg;

    printf("Operando na posição %d, elemento %d, usando a thread %d\n", vi->id, vi->vector[vi->id], vi->thread);
    vi->vector[vi->id] *= vi->vector[vi->id];

    pthread_exit(NULL);
}