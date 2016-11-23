#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void obsluz(int sygnal)
{
	printf("------------------------\n");
	printf("Otrzymano sygnal nr: %d\n", sygnal);
	printf("------------------------\n");
}

int main(int argc, char **argv)
{
    int status;

    int pid = fork();

    if(pid == 0)
    {
    sleep(10);
    pause();
        signal(SIGINT, obsluz);
    }
    else
    {
kill(pid, SIGINT);
        wait(&status);
        printf("Koniec programu.\n");
    }



	return 0;
}
