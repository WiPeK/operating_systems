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
    int key = ftok("/tmp", 32);
    if(key < 0)
        perror("ftok");
    int semset_id = semget(key, 1, 0600|IPC_CREAT|IPC_EXCL);
    if(semset_id < 0)
        perror("semget");

    struct sembuf op = {0,1,SEM_UNDO};
    semctl(semset_id, 0, SETVAL, 0);
    printf("Wartosc poczatkowa semafora: %d\n", semctl(semset_id, 0, GETVAL));
    int pid = fork();
    if(pid == 0)
    {
        if(semop(semset_id,&op,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }

        printf("Zmieniona wartosc semafora w procesie: %d\n", semctl(semset_id, 0, GETVAL));
    }
    else
    {
        wait(&status);
        printf("Wartosc po wykonaniu semafora z SEM_UNDO: %d\n", semctl(semset_id, 0, GETVAL));

        if(semctl(semset_id,0,IPC_RMID)<0)
            perror("semctl");
    }

    return 0;
}
