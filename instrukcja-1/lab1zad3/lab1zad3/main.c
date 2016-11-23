#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main ()
{
	int status;
	int potomek;

	potomek = fork ();
	//proces macierzysty
	if (potomek != 0) {
		wait(&status);
		if (WIFEXITED(status) > 0)  {
			printf("status: %d\n", WEXITSTATUS(status));
		}
		else
			printf("Cos jest nie tak");
	}
	//proces potomny
	else {
		printf("proces potomny...\n");
		exit(0);
	}

	return 0;
}

