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
    int con;
    if ((mkfifo("fifo",0600))==-1)
        printf("Blad (fifo)\n");
    int status;
    int key = ftok("/tmp", 28);
    if(key < 0)
        perror("ftok");
    int semset_id = semget(key, 2, 0600|IPC_CREAT|IPC_EXCL);
    if(semset_id < 0)
        perror("semget");
    if(semctl(semset_id, 0, SETVAL, 1)<0) {
        perror("semctl");
    }
    struct sembuf blockWrite = {0,-1,0};
    struct sembuf blockRead = {1,-1,0};
    struct sembuf freeWrite = {0,1,0};
    struct sembuf freeRead = {1,1,0};

    semctl(semset_id, 0, SETVAL, 1);
    semctl(semset_id, 1, SETVAL, 1);

    if(fork() == 0)
    {
        while(semctl(semset_id, 1, GETVAL) == 0)
            puts("Potomek: czekam na otwarcie czytania");

        if(semop(semset_id,&blockRead,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }

        con = open("fifo", O_RDONLY);

        char buf[20];
        if((read(con,buf,20))==-1)
            printf("Blad (read)\n");
        close(con);

        printf("Odebrana w procesie potomnym: %s\n", buf);
        if(semop(semset_id,&freeRead,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }

        while(semctl(semset_id, 0, GETVAL) == 0)
            puts("Potomek: czekam na otwarcie pisania");

        if(semop(semset_id,&blockWrite,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }

        char msg[20] = "proces potomny";

        con = open("fifo", O_WRONLY);

        if((write(con,msg,20))==-1)
            printf("Blad (write)\n");

        close(con);

        if(semop(semset_id,&freeWrite,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }

    }
    else
    {
        while(semctl(semset_id, 0, GETVAL) == 0)
            puts("Rodzic: czekam na otwarcie pisania");

        if(semop(semset_id,&blockWrite,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }

        char msg[20] = "proces rodzica";

        con = open("fifo", O_WRONLY);

        if((write(con,msg,20))==-1)
            printf("Blad (write)\n");

        close(con);

        if(semop(semset_id,&freeWrite,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }

        while(semctl(semset_id, 1, GETVAL) == 0)
            puts("Rodzic: czekam na otwarcie czytania");

        if(semop(semset_id,&blockRead,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }

        con = open("fifo", O_RDONLY);

        char buf[20];
        if((read(con,buf,20))==-1)
            printf("Blad (read)\n");
        close(con);

        printf("Odebrana w procesie rodzica: %s\n", buf);
        if(semop(semset_id,&freeRead,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }
        wait(&status);
    }


    if(semctl(semset_id,0,IPC_RMID)<0)
        perror("semctl");

    unlink("fifo");
    return 0;
}
