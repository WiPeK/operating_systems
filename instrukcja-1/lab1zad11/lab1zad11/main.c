#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void obsluz(int sygnal)
{
	printf("------------------------\n");
	printf("Otrzymano sygnal nr: %d\n", sygnal);
	printf("------------------------\n");
}

int main(int argc, char **argv)
{
	sigignore(SIGALRM);
	int pid = getpid();
	kill(pid, SIGALRM);
	printf("Koniec programu.\n");
	return 0;
}
