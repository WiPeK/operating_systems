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
    char * buff = "testsdfsdfd";
    char *rbuf = malloc(sizeof(buff));
    pipe(desc);
    int pid = fork();

    if(pid == 0)
    {
        write(desc[1], buff, 20);
        close(desc[1]);
        char *bff = malloc(sizeof(buff));
        //sleep(10);
        read(desc[0], bff, 20);
        close(desc[0]);
        printf("%s\n", bff);
    }
    else
    {
        //sleep(5);
        read(desc[0], rbuf, 20);
        close(desc[0]);
        printf("%s\n", rbuf);
        char *bfff = "gsdergrewsger";
        write(desc[1], bfff, 20);
        close(desc[1]);
        //sleep(10);
        wait(&status);
    }
    return 0;
}
