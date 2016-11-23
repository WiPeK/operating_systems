#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>

#define MSG_LENGTH 100
#define howManyComunicats 10

struct msgbuf{
    long type;
    int mnumber;
};

void send_message(int mqid)
{
    int i; int j;
    for(i = 1, j = 1; i < howManyComunicats+1; i++, j=j+2)
    {
        struct msgbuf buffer;

        buffer.type = 1;
        buffer.mnumber = j;

        if(msgsnd(mqid,&buffer,sizeof(buffer.mnumber),IPC_NOWAIT)<0)
            perror("msgsnd");
    }
}

int main()
{
    int key = ftok("/tmp", 911);
    int id = msgget(key, 0600|IPC_CREAT);
    if (id < 0)
        perror("msgget");
    send_message(id);
    return 0;
}
