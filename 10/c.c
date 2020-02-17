#include "apue.h"
 
int main(int argc,char *argv[]) {
	if (argc < 2) 
		exit(1);
	if (kill(atoi(argv[1]),SIGUSR1) == -1)
		exit(1);
	if (kill(atoi(argv[1]),SIGUSR2) == -1)
		exit(1);
    exit(0);
}
