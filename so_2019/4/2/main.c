#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

unsigned collatz(unsigned n)
{
	if(n % 2 == 1)
		return 3 * n + 1;
	else
		return n / 2;
}

int main(int argc, char** argv)
{
	int* childs = (int*)malloc(sizeof(int) * (argc - 1));
	int id = 0;
	int fork_result;

	for(int i = 1; i < argc; ++i)
	{
		fork_result = fork();

		if(fork_result == 0)
		{
			id = i - 1;
			printf("%d %d\n", getppid(), getpid());
			break;
		}
		else
			childs[i - 1] = fork_result;
	}


	if(fork_result != 0)
	{
		for(int i = 1; i < argc; ++i)
		{
			int status = 0;
			waitpid(childs[i - 1], &status, 0);
		}
	}
	else
	{
		printf("Parent: %d || Child: %d\n", getppid(), getpid());
		int num = atoi(argv[id + 1]);
		printf("PID[%d]: %d -> ", getpid(), num);
		while(num != 1)
		{
			num = collatz(num);
			printf("%d ", num);
		}
		printf("\n");
	}

	return 0;
}
