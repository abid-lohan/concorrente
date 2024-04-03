#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv){
	long int n;
	float element, sum = 0;

	if(argc < 2){
		puts("Uso: ./generate_sample <QUANTIDADE>");
		return 1;
	}

	n = atol(argv[1]);
	printf("%ld\n", n);

	srand(time(NULL));

	for(int i=0; i < n; i++){
		element = (rand()%700)/7.;
		printf("%.3f ", element);
		sum += element;
	}

	printf("\n%f", sum);

	return 0;
}