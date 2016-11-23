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

#define MSG_LENGTH 100

struct msgbuf{
    long type;
    char mtext[MSG_LENGTH];
};

void send_message(int mqid, char message[])
{
    struct msgbuf buffer;

    buffer.type = 1;
    memset(buffer.mtext,0,sizeof(buffer.mtext));
    strncpy(buffer.mtext,message,MSG_LENGTH-1);
printf("Odebrany komunikat: %s \n", buffer.mtext);
    if(msgsnd(mqid,&buffer,sizeof(buffer.mtext),IPC_NOWAIT)<0)
        perror("msgsnd");

}

int receive_message(int mqid)
{
    struct msgbuf buffer;

    int res = msgrcv(mqid, &buffer, sizeof(buffer.mtext), 1, IPC_NOWAIT);

    printf("Odebrany komunikat: %s \n", buffer.mtext);
    if(res == -1)
        return -1;
    else
        return res;


}

int main()
{
    int status;
    int pidwr[2];
    int pidrd[3];
    char error_message[15];
    int keysem = ftok("/tmp", 50);
    if(keysem < 0)
        perror("ftok");
    int keyipc = ftok("/tmp", 51);
    if(keyipc < 0)
        perror("ftok");
    int id = msgget(keyipc, 0600|IPC_CREAT|IPC_EXCL);
    if (id < 0)
        perror("msgget");

    int semset_id = semget(keysem, 2, 0600|IPC_CREAT|IPC_EXCL);
    if(semset_id < 0)
        perror("semget");

    struct sembuf blockWrite = {1,-1,0};
    struct sembuf blockRead = {0,-1,0};
    struct sembuf freeWrite = {1,1,0};
    struct sembuf freeRead = {0,1,0};

    semctl(semset_id, 0, SETVAL, 1);
    semctl(semset_id, 1, SETVAL, 1);

    pidwr[0] = fork();
    pidwr[1] = fork();
    pidrd[0] = fork();
    pidrd[1] = fork();
    pidrd[2] = fork();

    if(pidwr[0] == 0)
    {
        while(semctl(semset_id, 1, GETVAL) == 0)
            ;
        if(semop(semset_id,&blockWrite,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }
        if(semop(semset_id,&blockRead,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }
        int i;
        for(i = 0; i <10; i++)
            send_message(id, "Pierwszy pisarz");

        if(semop(semset_id,&freeRead,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }

        if(semop(semset_id,&freeWrite,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }
    }
    if(pidwr[1] == 0)
    {
        sleep(10);
        while(semctl(semset_id, 1, GETVAL) == 0)
            ;
        if(semop(semset_id,&blockWrite,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }
        if(semop(semset_id,&blockRead,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }
        int i;
        for(i = 0; i <10; i++)
            send_message(id, "Drugi pisarz");

        if(semop(semset_id,&freeRead,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }

        if(semop(semset_id,&freeWrite,1)<0) {
            sprintf(error_message,"semop %u",__LINE__-1);
            perror(error_message);
        }
    }
    if(pidrd[0] == 0)
    {
        sleep(2);
        while(1){
            if(semop(semset_id,&blockRead,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }

            if(semop(semset_id,&blockWrite,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }

            int res = receive_message(id);

            if(semop(semset_id,&freeWrite,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }

            if(semop(semset_id,&freeRead,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }
            if(res == -1) break;
        }
        exit(0);
    }
    /*if(pidrd[1] == 0)
    {
        sleep(2);
        while(1){


            if(semop(semset_id,&blockRead,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }

            if(semop(semset_id,&blockWrite,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }

            int res = receive_message(id);

            if(semop(semset_id,&freeWrite,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }

            if(semop(semset_id,&freeRead,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }
            if(res == -1) break;
        }
        exit(0);
    }
    if(pidrd[2] == 0)
    {
        sleep(2);
        while(1){
            if(semop(semset_id,&blockRead,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }

            if(semop(semset_id,&blockWrite,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }

            int res = receive_message(id);

            if(semop(semset_id,&freeWrite,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }

            if(semop(semset_id,&freeRead,1)<0) {
                sprintf(error_message,"semop %u",__LINE__-1);
                perror(error_message);
            }
            if(res == -1) break;
        }
        exit(0);
    }*/
    if((pidwr[0] > 0) && (pidwr[1] > 0) && (pidrd[0] > 0) && (pidrd[1] > 0) && (pidrd[2] > 0))
    {
        wait(&status);
        if(msgctl(id, IPC_RMID, 0) < 0)
            perror("msgctl");
        if(semctl(semset_id,0,IPC_RMID)<0)
            perror("semctl");
    }

    return 0;
}
