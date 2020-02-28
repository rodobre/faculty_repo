#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>

#define MAX_THREADS 50

pthread_mutex_t barrier_lock;
sem_t barrier_semaphore;
pthread_t tid[50];
int waiting_threads = 0;
unsigned char done_waiting = 0;

void init(int N)
{
	if(sem_init(&barrier_semaphore, 0, 0) != 0)
	{
		printf("Semaphore failed to be instantiated\n");
		return;
	}
}

void barrier_point()
{
	int i = 0;

	// Enter critical section
	pthread_mutex_lock(&barrier_lock);

	++ waiting_threads;
	if(waiting_threads == MAX_THREADS)
	{
		for(i = 0; i < MAX_THREADS; ++i)
			sem_post(&barrier_semaphore);
	}

	// Exit critical section
	pthread_mutex_unlock(&barrier_lock);
	sem_wait(&barrier_semaphore);
}

void* tfun(void* v)
{
	int* tid = (int* )v;

	printf("%d reached the barrier\n", *tid);
	barrier_point();
	printf("%d passed the barrier\n", *tid);

	return NULL;
}

int main()
{
	int i = 0, err = 0;

	int* tids = (int*)malloc(sizeof(int) * MAX_THREADS);

	if(pthread_mutex_init(&barrier_lock, NULL) != 0)
	{
		printf("Mutex failed to be instantiated\n");
		return -1;
	}

	init(MAX_THREADS);

	while(i < MAX_THREADS)
	{
		tids[i] = i;
		err = pthread_create(&(tid[i]), NULL, &tfun, &tids[i]);
		if (err != 0)
			printf("Could not create thread with index %d\n", i);
		++ i;
	}

	for(i = 0; i < MAX_THREADS; ++i)
		pthread_join(tid[i], NULL);

	sem_destroy(&barrier_semaphore);
	pthread_mutex_destroy(&barrier_lock);
	free(tids);
	return 0;
}
