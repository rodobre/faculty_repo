#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	char cmd[] = "/bin/ls";
	char* arg[] = { cmd, "-lah", NULL };

	if(fork() == 0)
	{
		printf("Parent: %d || Child: %d\n", getppid(), getpid());
		int err = execve(cmd, arg, NULL);
	}
	return 0;
}
