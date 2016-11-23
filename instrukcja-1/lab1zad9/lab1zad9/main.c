#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main()
{
    int a = 10;
    int status;
    if(fork() == 0)
    {
        printf("Zmienna w potomku:: %d\n", a);
        a++;
        printf("Zmienna w potomku po modyfikacji:: %d\n", a);
    }
    else{
        wait(&status);
    printf("Zmienna w rodzicu:: %d\n", a);
}
    return 0;
}
