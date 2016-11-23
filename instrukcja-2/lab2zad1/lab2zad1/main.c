#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
int r,i;
FILE *f=popen("sort main.c","r");
char buff[22];
fread(buff, sizeof(char),22,f);
buff[21]='\0';
printf("%s\n",buff);
pclose(f);
//345678901234567890
return 0;
}
