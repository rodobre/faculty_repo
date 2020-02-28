#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
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
	char shm_file[] = "/collatzConjecture";
	int shm = shm_open(shm_file, O_CREAT|O_RDWR, 0777);

	if(shm == -1)
	{
		printf("Cannot open shared memory file!: %s [%d]\n", strerror(errno), errno);
		return -1;
	}

	size_t num_count = argc - 1;
	size_t shm_size = getpagesize() * num_count;
	pid_t* childs = (pid_t*)malloc(sizeof(pid_t) * num_count);
	int fork_result = 0, id = 0;

	if(ftruncate(shm, shm_size) == -1)
	{
		printf("ftruncate call failed: %s [%d]\n", strerror(errno), errno);
		shm_unlink(shm_file);
		return errno;
	}

	char* shm_ptr = (char*)mmap(0, num_count * getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, shm, 0);

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
		int status = 0;
		for(int i = 1; i < argc; ++i)
		{
			printf("Child [%d]: %d -> ", childs[i - 1], atoi(argv[i]));
			int result = *(int*)(shm_ptr + (i - 1) * getpagesize()), ctr = 0;
			while(result != 1)
			{
				printf("%d ", result);
				++ ctr;
				result = *(int*)(shm_ptr + (i - 1) * getpagesize() + ctr * 4);
			}
			printf("\n");
			waitpid(childs[i - 1], &status, 0);
		}

		munmap(shm_ptr, num_count * getpagesize());
		shm_unlink(shm_file);
	}
	else if(fork_result == 0)
	{
		printf("Parent: %d || Child: %d || ID: %d\n", getppid(), getpid(), id);
		int num = atoi(argv[id + 1]), ctr = 0;
		while(num != 1)
		{
			num = collatz(num);
			memcpy(shm_ptr + id * getpagesize() + ctr * 4, (void*)&num,  sizeof(num));
			++ ctr;
		}
	}

	return 0;
}
