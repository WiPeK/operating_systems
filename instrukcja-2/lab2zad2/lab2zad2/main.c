#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(){

	FILE *f=popen("wc","w");
	char *s="dowolny";

	fwrite(s, sizeof(char),512,f);

	pclose(f);
	return 0;
}
