#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>

#define MSG_LENGTH 100

struct msgbuf{
    long type;
    char mtext[MSG_LENGTH];
};

void send_message(int mqid)
{
        struct msgbuf buffer;
        buffer.type = 1;
        memset(buffer.mtext,0,sizeof(buffer.mtext));
        char message[] = "Komunikat zad 4";

        strncpy(buffer.mtext,message,MSG_LENGTH-1);

        if(msgsnd(mqid,&buffer,sizeof(buffer.mtext),0)<0)
            perror("msgsnd");

}

int main()
{
    int key = ftok("/tmp", 444);
    int id = msgget(key, 0600|IPC_CREAT);
    if (id < 0)
        perror("msgget");
    send_message(id);
    return 0;
}
