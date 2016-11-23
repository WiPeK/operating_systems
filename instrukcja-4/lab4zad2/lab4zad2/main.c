#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/sem.h>


int main()
{
    int key = ftok("/tmp", 14);
    if(key < 0)
        perror("ftok");
    int semset_id = semget(key, 10, 0600|IPC_CREAT|IPC_EXCL);
    if(semset_id < 0)
        perror("semget");

    char error_message[15];
    int i;

    for(i = 0; i < 10; i++){
        if(semctl(semset_id, i, SETVAL, 0)<0) {
            perror("semctl");
        }
        struct sembuf up_operation = {i,1,0};
        if(semop(semset_id,&up_operation,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }
    }

    i = 0;
    for(i = 0; i < 10; i++)
    {
        int pid = fork();
        if(pid == 0)
        {
            sleep(1);
            struct sembuf zero_operation = {i,-1,0};
            if(semop(semset_id,&zero_operation,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }
            exit(0);
        }
    }
    int status;
    wait(&status);

    printf("%d\n", semctl(semset_id, 9, GETVAL));

    if(semctl(semset_id,0,IPC_RMID)<0)
        perror("semctl");

    return 0;
}
