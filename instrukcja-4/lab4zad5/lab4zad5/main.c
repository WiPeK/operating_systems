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
    char error_message[15];
    int status;
    int key = ftok("/tmp", 30);
    if(key < 0)
        perror("ftok");
    int semset_id = semget(key, 1, 0600|IPC_CREAT|IPC_EXCL);
    if(semset_id < 0)
        perror("semget");

    struct sembuf block = {0,-1,0};
    struct sembuf free = {0,1,0};

    if(semctl(semset_id, 0, SETVAL, 0)<0) {
        perror("semctl");
    }

    if(fork() == 0)
    {
        while(semctl(semset_id, 0, GETVAL) == 0)
            puts("Potomek: czekam na otwarcie semaforu");

        if(semop(semset_id,&block,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }

        puts("*** proces potomny ***");

        if(semop(semset_id,&free,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }

        exit(0);
    }
    else
    {
        while(semctl(semset_id, 0, GETVAL) == 0)
            puts("Rodzic: czekam na otwarcie semaforu");

        if(semop(semset_id,&block,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }

        puts("*** proces rodzica ***");

        if(semop(semset_id,&free,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }
        wait(&status);
    }


    if(semctl(semset_id,0,IPC_RMID)<0)
        perror("semctl");

    return 0;
}
