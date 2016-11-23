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

union semun {
    int val;
    struct semid_ds *buff;
    unsigned short *array;
    struct seminfo *__buf;
};
int main()
{
    union semun arg;
    struct semid_ds semid_ds;
    char error_message[15];
    int status;
    int key = ftok("/tmp", 36);
    if(key < 0)
        perror("ftok");
    int semset_id = semget(key, 5, 0600|IPC_CREAT|IPC_EXCL);
    if(semset_id < 0)
        perror("semget");
    unsigned short sems[5];
    int i;
    for(i = 0; i < 5; i++)
        sems[i] = i;
    arg.array = sems;

    semctl(semset_id, 0, SETALL, arg);
    for(i = 0; i < 5; i++)
        printf("(SETALL) Wartosc semafora %d: %d\n", i, semctl(semset_id, i, GETVAL));

    puts("Ustawiam wartosci od 10 do 14");
    for(i = 10; i < 15; i++)
        sems[i-10] = i;
    arg.array = sems;
    semctl(semset_id, 0, SETALL, arg);

    puts("Flaga GETALL");

    semctl(semset_id, 0, GETALL, arg);

    for(i = 0; i < 5; i++)
        printf("(GETALL) Wartosc semafora %d: %d\n", i, semctl(semset_id, i, GETVAL));

    arg.buff = & semid_ds;
    semctl(semset_id, 0, IPC_STAT, arg);
    printf("Info z IPC_STAT ilosc semaforow w zestawie: %d\n", arg.buff->sem_nsems);

    int pid = fork();
    if(pid == 0)
    {
        struct sembuf op = {2,1,0};
        if(semop(semset_id,&op,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }
    }
    else
    {
        wait(&status);
        printf("PID procesu ktory ostatnio modyfikowal semafor %d i nadal mu wartosc %d\n", semctl(semset_id, 2, GETPID), semctl(semset_id, 2, GETVAL));

        printf("Liczba procesow oczekujacych na semafor 0: %d", semctl(semset_id, 0, GETZCNT, arg));

        if(semctl(semset_id,0,IPC_RMID)<0)
            perror("semctl");
    }

    return 0;
}
