#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>

#define MSG_LENGTH 100
#define MIN_TYPE_COMUNICAT 5
#define MAX_TYPE_COMUNICAT 1
#define howManyComunicats 100

struct msqid_ds *buf;
struct msgbuf{
    long type;
    char mtext[MSG_LENGTH];
};

void receive_message(int mqid, int order)
{
    printf("Odebrane komunikaty w kolejnosci: ");
    if(order == 1)
    {
        printf("rosnacej.\n");
        int i;
        for(i = 0; i < howManyComunicats; i++)
        {
            struct msgbuf buffer;
            if(msgrcv(mqid, &buffer, sizeof(buffer.mtext), 1, 0)<0)
                perror("msgrcv");
            else
                printf("Odebrany komunikat: %s \n", buffer.mtext);
        }

    }
    else if(order == 2){
        printf("malejacej.\n");
        int i;
        for(i = howManyComunicats; i > 0; i--)
        {
            struct msgbuf buffer;
            if(msgrcv(mqid, &buffer, sizeof(buffer.mtext), 1, 0)<0)
                perror("msgrcv");
            else
                printf("Odebrany komunikat: %s \n", buffer.mtext);
        }
    }
    else
        exit(1);
}

int main()
{
    int key = ftok("/tmp", 123);
    int id = msgget(key, 0600|IPC_EXCL);
    int order;
    puts("W jakiej kolejnosci odebrac komunikaty?");
    puts("1. Rosnaco");
    puts("2. Malejaco");
    scanf("%d", &order);
    receive_message(id, order);
    if(msgctl(id, IPC_RMID, buf) < 0)
        perror("msgctl");
    return 0;
}
