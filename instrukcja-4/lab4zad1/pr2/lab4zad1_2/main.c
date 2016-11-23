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
    int semset_id = semget(key, 1, 0600);
    if(semset_id < 0)
        perror("semget");
    struct sembuf down_operation = {0,-1,0};

    if(semop(semset_id,&down_operation,1)<0) {
        perror("semop");
    }


    return 0;
}
