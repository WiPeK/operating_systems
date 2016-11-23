#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/sem.h>


int main()
{
    int key = ftok("/tmp", 9);
    if(key < 0)
        perror("ftok");
    int semset_id = semget(key, 1, 0600|IPC_CREAT|IPC_EXCL);
    if(semset_id < 0)
        perror("semget");
    struct sembuf up_operation = {0,1,0};
    struct sembuf wait_operation = {0,0,0};
    char error_message[15];
    if(semop(semset_id,&up_operation,1)<0) {
        sprintf(error_message,"semop %u",__LINE__-1);
        perror(error_message);
    }
    if(semop(semset_id,&wait_operation,1)<0) {
        sprintf(error_message,"semop %u",__LINE__-1);
        perror(error_message);
    }
    if(semctl(semset_id,0,IPC_RMID)<0)
        perror("semctl");

    printf("Zakonczono program\n");

    return 0;
}
