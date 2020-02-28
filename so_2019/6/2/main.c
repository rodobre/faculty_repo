#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

#define MAX_THREADS 4
int matA[5][5], matB[5][5], matC[5][5];
int columnsA, linesA, columnsB, linesB;
int i, j, k, numberThreads;

void citire()
{
	printf("Columns matA: ");
	scanf("%d", &columnsA);
	printf("Rows matA: ");
	scanf("%d", &linesA);
	printf("Columns matB: ");
	scanf("%d", &columnsB);
	printf("Rows matB: ");
	scanf("%d", &linesB);
	printf("A:\n");

	for(i = 0; i < linesA; i++)
		for(j = 0; j < columnsA; j++)
			scanf("%d", &matA[i][j]);

	printf("B:\n");

	for(i = 0; i < linesB; i++)
		for(j = 0; j < columnsB; j++)
			scanf("%d", &matB[i][j]);
}

void afisareMatrici() {
	printf("A: \n");

	for(i = 0; i < linesA; i++)
	{

		for(j = 0; j < columnsA; j++)
			printf("%d ", matA[i][j]);
		printf("\n");
	}

	printf("B: \n");

	for(i = 0; i < linesB; i++)
	{
		for(j = 0; j < columnsB; j++)
			printf("%d ", matB[i][j]);
		printf("\n");
	}
}

void *multiply(void* args)
{
	int *arr = (int *)args;
	int i, multiplyResult = 0;
	int len = arr[0];

	for(i = 1; i <= len; i++) 
		multiplyResult += arr[i] * arr[len+i];

	int *p = (int*)malloc(sizeof(int));
	*p = multiplyResult;
	pthread_exit(p);
}

int main()
{
	citire();
	afisareMatrici();

	pthread_t *threads = (pthread_t*)malloc(linesA * columnsB * sizeof(pthread_t));

	for(i = 0; i < linesA; i++) {
		for(j = 0; j < columnsB; j++) {
			int *arr = (int*)malloc((linesA * columnsB + 1) * sizeof(int));
			arr[0] = linesA;
			for(k = 0; k < linesA; k++)
				arr[k+1] = matA[i][k];

			for(k = 0; k < columnsB; k++)
				arr[k+linesA+1] = matB[k][j];

			pthread_create(&threads[numberThreads++], NULL, multiply, (void*)arr);
		}
	}

	printf("\n \n");
	for(i = 0; i < linesA * columnsB; i++) {
		void *multiplyResult;
		if(pthread_join(threads[i], &multiplyResult)) {
			perror(NULL);
			return errno;
		}

		int *p = (int *)multiplyResult;
		printf("%d ", *p);

		if((i + 1) % linesB == 0)
			printf("\n");
	}
	return 0;
}
