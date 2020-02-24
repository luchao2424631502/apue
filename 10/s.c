#include "apue.h" 
 
int main(int argc,char *argv[]) {
//	psignal(SIGUSR1,"my definition ");

	for (int i=1; i<=31; i++)
		printf("%s\n",strsignal(i));
	
    exit(0);
}
