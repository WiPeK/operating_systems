#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int desc[2];
    pipe(desc);
    char * buff = "test";
    write(desc[1], buff, 20);
    close(desc[1]);
    char *rbuf = malloc(sizeof(&buff));
    read(desc[0], rbuf, 20);
    close(desc[0]);
    printf("%s\n", rbuf);
    return 0;
}
