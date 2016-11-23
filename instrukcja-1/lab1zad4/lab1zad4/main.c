#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int pr[argc][2];
    int i,j, status, pid;
    for(i = 0; i < argc; i++){
        pr[i][0] = fork();
        pr[i][1] = i;
        pid = getpid();
        for(j =0; j < 4; j++)
            printf("%d %d %d",getpid(), getppid(), i);
        sleep(i);
        exit(0);
    }

    while((pid = wait(&status)) > 0)
    {
        printf("%d %d", pid, status);
    }

    return 0;
}
