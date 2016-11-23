#include <stdio.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
#include <unistd.h>

int main()
{
    int desc[2];
    pipe(desc);
    int pid = fork();
    if(pid == 0)
    {
        write(desc[1], "pipe z potomka do rodzica", 30);
        close(desc[1]);
        char *buf = malloc(sizeof(char)*30);
        int con = open("fifo", O_RDONLY);
        read(con, buf, 30);
        close(con);
        unlink("fifo");
        printf("fifo odebrane w potomku: %s\n", buf);
    }
    else
    {
        char *buf = malloc(sizeof(char)*30);
        read(desc[0], buf, 30);
        close(desc[0]);
        printf("pipe odebrane w rodzicu: %s\n", buf);
        mkfifo("fifo", 0600);
        int con = open("fifo", O_WRONLY);
        write(con, "fifo z rodzica do potomka", 30);
        close(con);
        wait();
    }
    return 0;
}
