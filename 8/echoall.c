#include "apue.h"
 
extern char **environ;

int main(int argc,char *argv[]) {
	for (int i=0; i<argc; i++)
		printf("argv[%d]: %s\n",i,argv[i]);
	for (char **ptr = environ; *ptr != NULL; ptr++)
		printf("%s\n",*ptr);

	printf("\n\n\n\n");
    exit(0);
}
