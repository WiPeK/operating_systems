#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>

#define MSG_LENGTH 100
#define howManyComunicats 10

struct msqid_ds *buf;
struct msgbuf{
    long type;
    char mtext[MSG_LENGTH];
};

void receive_message(int mqid)
{
    struct msgbuf buffer;

    if(msgrcv(mqid, &buffer, sizeof(buffer.mtext), 1, IPC_NOWAIT)<0)
        perror("msgrcv");
    else
        printf("Odebrany komunikat: %s \n", buffer.mtext);
}

int main()
{
    int key = ftok("/tmp", 444);
    int id = msgget(key, 0600|IPC_CREAT);
    receive_message(id);
    if(msgctl(id, IPC_RMID, buf) < 0)
        perror("msgctl");
    return 0;
}
