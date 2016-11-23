#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main ()
{
	int status;
	int potomek;

	potomek = fork ();
	if (potomek == 0) {
		execlp("./lab1zad5b", "lab1zad5b",NULL);
	}
	wait(&status);

	return 0;
}

