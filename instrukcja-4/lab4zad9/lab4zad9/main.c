#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/sem.h>
#include<sys/wait.h>
#include <fcntl.h>

int main()
{
    int status;

    int key = ftok("/tmp", 49);
    if(key < 0)
        perror("ftok");

    int semset_id = semget(key, 1, 0600|IPC_CREAT|IPC_EXCL);
    if(semset_id < 0)
        perror("semget");

    int desc[2];
    pipe(desc);

    struct sembuf block = {0,-1,0};
    semctl(semset_id, 0, SETVAL, 1);

    int pid = fork();
    if(pid == 0)
{
        close(desc[1]);
        int i;
        for (i = 1; i < 11; i++)
        {
            //sleep(1);
            char bufor[100];
            read(desc[0],bufor,sizeof(bufor));
            printf("%d\n",bufor[0]);
        }
printf("val sem: %d\n",semctl(semset_id, 0, GETVAL));
        semop(semset_id, &block, 1);
        printf("val sem: %d\n",semctl(semset_id, 0, GETVAL));
        exit(1);
    }
    else
    {
        close(desc[0]);
        int i;
        char bufor[100];
        for (i = 1; i < 11; i++)
        {
            sprintf(bufor,"%c",i);
            write(desc[1],bufor,10);
            sleep(1);
        }

        while(semctl(semset_id, 0, GETVAL) != 0)
            ;
        printf("val sem: %d\n",semctl(semset_id, 0, GETVAL));
        wait(&status);

        if (semctl(semset_id,IPC_RMID,0) == -1)
        {
            perror("semgctl");
        }
    }

    return 0;
}
