#include <stdio.h>
#include <sys/types.h>
//#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
#include <unistd.h>

int main(void){

int pid=fork();
 if (pid>0){
	int lacze;
	char tekst[10]="tekst";
	if ((mkfifo("fifo",0777))==-1)
			printf("Blad (fifo)\n");
		lacze = open("fifo",O_WRONLY);
		if((write(lacze,tekst,10))==-1)
			printf("Blad (write)\n");
		close(lacze);
		unlink("fifo");

 	}
return 0;
}

