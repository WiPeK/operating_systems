#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main()
{
    int status;
    int desc[2];

    char *rbuf = malloc(20);
    pipe(desc);
    int pid = fork();

    if(pid == 0)
    {
        char * buff = "testsdfsdfd";
        //fcntl(desc[1], O_NONBLOCK);
        write(desc[1], buff, 20);
        close(desc[1]);
    }
    else
    {
        wait(&status);
        read(desc[0], rbuf, 20);
        close(desc[0]);
        printf("%s\n", rbuf);
    }
    return 0;
}
