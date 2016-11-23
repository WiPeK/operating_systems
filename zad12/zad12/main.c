#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	int potomek;
	int status;
	sigblock(SIGINT);

	potomek = fork();
	if (potomek == 0) {
		kill(getppid(), SIGINT);
	}
	else {
		wait(&status);
		printf("Koniec programu.\n");
	}
	return 0;
}
