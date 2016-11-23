#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void obsluz(int sygnal)
{
	printf("------------------------\n");
	printf("Otrzymano sygnal nr: %d\n", sygnal);
	printf("------------------------\n");
}

int main(int argc, char **argv)
{
    int status;
	sigblock(SIGCHLD);
	int pid = fork();
	if(pid == 0)
	{
        kill(getppid(), SIGCHLD);
	}
	else
	{
        wait(&status);
        printf("%d\n", status);
	}

	return 0;
}
