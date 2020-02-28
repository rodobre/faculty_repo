#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

char *strrev(char *str)
{
    if (!str || ! *str)
        return str;

    int i = strlen(str) - 1, j = 0;

    char ch;
    while (i > j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}

void* rev(void* arg)
{
	arg = strrev(arg);
	return arg;
}

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("Error! %s string\n", argv[0]);
		return -1;
	}

	char* str = argv[1];
	pthread_t thr;
	int err;

	err = pthread_create(&thr, NULL, &rev, str);

	if(err != 0)
		printf("AAAAAAAAAAAAA %d\n", err);

	pthread_join(thr, (void**)&str);
	printf("%s\n", str);
	return 0;
}
