#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void obsluz(int sygnal)
{
	printf("Otrzymano sygnal nr: %d\n", sygnal);
}

int main()
{
    int pid[4];
    int i, status;
    for(i = 0; i<4; i++)
        pid[i] = fork();

    if(pid[0] == 0)
    {
    signal(SIGALRM, obsluz);
        while(1)
        {
            sleep(1);
            kill(pid[1], SIGALRM);
        }
    }
    if(pid[1] == 0)
    {
        signal(SIGALRM, obsluz);
        while(1)
        {
        pause();
            sleep(1);
            kill(pid[2], SIGALRM);
        }

    }
    if(pid[2] == 0)
    {
        signal(SIGALRM, obsluz);
        while(1)
        {
        pause();
            sleep(1);
            kill(pid[3], SIGALRM);
        }
    }
    if(pid[3] == 0)
    {
        signal(SIGALRM, obsluz);
    }
    wait(&status);

    return 0;
}
