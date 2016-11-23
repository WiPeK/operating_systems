#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
#include <unistd.h>

int main()
{
    int desc[2];
    pipe(desc);
    int pid[4];
    int i;
    for(i=0;i<4;i++)
        pid[i] = fork();
    if(pid[0] == 0)
    {
        write(desc[1], "1234", 5);
        close(desc[1]);
    }
    if(pid[1] == 0)
    {
        char buf[5];
        read(desc[0], buf, 5);
        close(desc[0]);

        int a = atoi(buf[0]);
        a++;
        int b = atoi((const char)buf[1]);
        b++;
        int c = atoi((const char)buf[2]);
        c++;
        int d = atoi((const char)buf[3]);
        d++;


        sprintf(buf, "%d%d%d%d", a, b, c, d);

        write(desc[1], buf, 5);
        close(desc[1]);
    }
    if(pid[2] == 0)
    {
        char buf[5];
        read(desc[0], buf, 5);
        close(desc[0]);

        int a = atoi((const char)buf[0]);
        a++;
        int b = atoi((const char)buf[1]);
        b++;
        int c = atoi((const char)buf[2]);
        c++;
        int d = atoi((const char)buf[3]);
        d++;


        sprintf(buf, "%d%d%d%d", a, b, c, d);

        write(desc[1], buf, 5);
        close(desc[1]);
    }
    if(pid[3] == 0)
    {
        char *buf = malloc(sizeof(char) * 5);
        read(desc[0], buf, 5);
        close(desc[0]);

        printf("%d %d %d %d", buf[0], buf[1], buf[2], buf[3]);
    }
    int status;
    wait(&status);
    return 0;
}
