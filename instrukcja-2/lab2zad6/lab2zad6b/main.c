#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void){
	int pid = fork();
	if (pid > 0) {
		int lacze;
		char tekst[10];
		lacze = open("fifo",O_RDONLY);
		if((read(lacze,tekst,20))==-1)
			printf("Blad (read)\n");
		printf("%s\n",tekst);
		close(lacze);
 	}
return 0;
}
