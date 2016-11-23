#include <stdio.h>
#include <stdlib.h>

int main()
{
    int pid = fork();
    int pidd = fork();
    if(pid == 0)
        puts("******");
    if(pidd == 0)
        puts("@@@@@@@@");
    return 0;
}
