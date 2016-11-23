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
    int mnumber;
};

void receive_message(int mqid)
{
    int i;
    for(i = 1; i < howManyComunicats+1; i++)
    {
        struct msgbuf bufferp, buffern;
        if(msgrcv(mqid, &bufferp, sizeof(bufferp.mnumber), 1, IPC_NOWAIT)<0)
            perror("msgrcv");
        if(msgrcv(mqid, &buffern, sizeof(buffern.mnumber), 2, IPC_NOWAIT)<0)
            perror("msgrcv");

            printf("%d + %d = %d\n", bufferp.mnumber,buffern.mnumber, bufferp.mnumber + buffern.mnumber);
    }
}

int main()
{
    int key = ftok("/tmp", 911);
    int id = msgget(key, 0600|IPC_CREAT);
    receive_message(id);
    if(msgctl(id, IPC_RMID, buf) < 0)
        perror("msgctl");
    return 0;
}
