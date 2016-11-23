#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/wait.h>


int main()
{
    int pid;
    int key = ftok("/tmp", 19);
    if(key < 0)
        perror("ftok");
    int semset_id = semget(key, 1, 0600|IPC_CREAT|IPC_EXCL);
    if(semset_id < 0)
        perror("semget");

    char error_message[15];
    int i;

    if(semctl(semset_id, 0, SETVAL, 0)<0) {
        perror("semctl");
    }
    struct sembuf z_operation = {0,0,0};
    if(semop(semset_id,&z_operation,1)<0) {
        sprintf(error_message,"semop %u",__LINE__-1);
        perror(error_message);
    }

    printf("%d\n", semctl(semset_id, 0, GETVAL));

    for(i = 0; i < 5; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            sleep(1);
            struct sembuf up_operation = {0,1,0};
            if(semop(semset_id,&up_operation,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }
            exit(0);
        }
        else
        {
            int status;
            wait(&status);
        }
    }

    printf("%d\n", semctl(semset_id, 0, GETVAL));

    if(semctl(semset_id,0,IPC_RMID)<0)
        perror("semctl");

    return 0;
}
