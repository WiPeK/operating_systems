#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>

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

    if(msgsnd(mqid,&buffer,sizeof(buffer.mtext),0)<0)
        perror("msgsnd");

}

void receive_message(int mqid)
{
    struct msgbuf buffer;

    if(msgrcv(mqid, &buffer, sizeof(buffer.mtext), 1, 0)<0)
        perror("msgrcv");
    else
        printf("Odebrany komunikat: %s \n", buffer.mtext);
    wait();
}

int main()
{
    int key = ftok("/tmp", 8);
    int id = msgget(key, 0600|IPC_CREAT|IPC_EXCL);
    if (id < 0)
        perror("msgget");
    int pid = fork();
    if(pid < 0)
        perror("fork");
    if(pid == 0)
        send_message(id, "Zadanie 2");
    else
        receive_message(id);
    if(msgctl(id, IPC_RMID, NULL) < 0)
        perror("msgctl");
    return 0;
}
