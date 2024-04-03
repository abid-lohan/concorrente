#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// #define TEST

float* vector;

typedef struct{
	long int id;
	long int block_size;
} vector_info;

typedef struct {
	float sum;
} sum_info;

void* sum_elements(void* args){
	vector_info info = *(vector_info*) args;

	sum_info* local_sum = (sum_info*) malloc(sizeof(sum_info));
	if(local_sum == NULL) pthread_exit(NULL);

	local_sum->sum = 0;

	for(long int i = info.id*info.block_size; i < info.id*info.block_size+info.block_size; i++){
		local_sum->sum += vector[i];
#ifdef TEST
		printf("%.3f ", vector[i]);
#endif
	}

#ifdef TEST
	printf("\nID %ld\n", info.id);
	printf("Soma local: %f\n\n", local_sum->sum);
#endif

	pthread_exit((void*) local_sum);
}

int main(int argc, char** argv){
	long int length;
	int n_threads, block_size;
	pthread_t* tid;
	float expected_sum, sum = 0;
	sum_info* res;

	if(argc < 2){
		puts("Uso: ./sum_elements_vector <QNT_THREADS>");
		return 1;
	}

	n_threads = atoi(argv[1]);
	scanf("%ld", &length);
	block_size = length/n_threads;

	if(!block_size) puts("Execução sequencial: Thread > Tamanho do vetor");

#ifdef TEST
	printf("Tamanho: %ld\n", length);
	printf("Threads: %d\n", n_threads);
	printf("Tamanho do bloco: %d\n", block_size);
#endif

	tid = malloc(sizeof(pthread_t)*n_threads);
	if(tid == NULL) {
		puts("Erro: Alocação de memória falhou - tid"); 
		return 2;
	}

	vector = malloc(sizeof(float)*length);
	if(tid == NULL) {
		puts("Erro: Alocação de memória falhou - vector"); 
		return 2;
	}

	for(long int i = 0; i < length; i++) scanf("%f", &vector[i]);

	scanf("%f", &expected_sum);

#ifdef TEST
	printf("Vetor: ");
	for(int i = 0; i < length; i++) printf("%.3f ", vector[i]);

	printf("\n\nSoma esperada: %.3f\n\n", expected_sum);
#endif

	for(int i = 0; i < n_threads; i++){
		vector_info* info = (vector_info*) malloc(sizeof(vector_info));
		if(info == NULL){
			puts("Erro: Alocação de memória falhou - info"); 
			return 2;
		}

		info->id = i;
		info->block_size = block_size;

		if(pthread_create(tid+i, NULL, sum_elements, (void*) info)){
			puts("Erro: Thread falhou - pthread_create");
			return 3;
		}
	}

	if(length%n_threads) {
	    for(long int i=length-(length%n_threads); i<length; i++) {
	        sum += vector[i];
	    }
   	}

	for(int i = 0; i < n_threads; i++){
		if(pthread_join(tid[i], (void**) &res)){
			puts("Erro: Thread falhou - pthread_join");
			return 3;
		}
		sum += res->sum;
	}

	printf("Soma esperada: %.3f\n", expected_sum);
	printf("Soma final: %.3f\n", sum);

	free(tid);
	free(vector);
	pthread_exit(NULL);
	return 0;
}