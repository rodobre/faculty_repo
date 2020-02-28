#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_RESOURCES 1
#define MAX_THREADS   10

int available_resources = MAX_RESOURCES;

// Thread definitions
pthread_t tid[MAX_THREADS];
pthread_mutex_t resource_lock;

int decrease_count(int count)
{
	pthread_mutex_lock(&resource_lock);
	printf("Trying to decrease count by %d\n", count);
	if(available_resources < count)
	{
		pthread_mutex_unlock(&resource_lock);
		printf("Tried to decrease more than available, %d vs %d\n", available_resources, count);
		return -1;
	}
	else
		available_resources -= count;
	pthread_mutex_unlock(&resource_lock);
	return 0;
}

int increase_count(int count)
{
	pthread_mutex_lock(&resource_lock);
	printf("Trying to increase count by %d\n", count);
	available_resources += count;
	pthread_mutex_unlock(&resource_lock);
}

void* randomFunc(void* arg)
{
	int coeff = rand() % 2 ? -1 : 1;
	int a = rand() % 23 + 1;
	if(coeff < 0)
	{
		decrease_count(a);
	}
	else
	{
		increase_count(a);
	}
}


int main()
{
	int i = 0;
	int err;

	printf("Available resources: %d\n", available_resources);

	if(pthread_mutex_init(&resource_lock, NULL) != 0)
	{
		printf("mutex init failed\n");
		return 1;
	}

	while(i < MAX_THREADS)
	{
		err = pthread_create(&(tid[i]), NULL, &randomFunc, NULL);
		if (err != 0)
			printf("Could not create thread with index %d\n", i);
		++ i;
	}

	for(i = 0; i < MAX_THREADS; ++i)
		pthread_join(tid[i], NULL);

	pthread_mutex_destroy(&resource_lock);

	printf("Available resources: %d\n", available_resources);
	return 0;
}
