#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main ()
{
	pid_t potomek;

	potomek = fork ();
	if (potomek != 0) {
		printf ("RODZIC: ID procesu biezacego: %d\n", getpid ());
		printf ("ID dziecka: %d\n", potomek);
		wait();
		puts("chuj");
	}
	else {
		printf ("DZIECKO: ID procesu biezacego: %d\n", getpid ());
		printf ("ID rodzica: %d\n", getppid ());
	}

	return 0;
}

