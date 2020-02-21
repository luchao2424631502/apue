#include "apue.h" 
 
int main(int argc,char *argv[]) {
	psignal(SIGUSR1,"my definition ");
	psignal(SIGUSR2,NULL);

	for (int i=0; i<70; i++)
		printf("%s\n",strsignal(i));
    exit(0);
}
