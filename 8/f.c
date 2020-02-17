#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
 
int main(int argc,char *argv[]) {
	pid_t pid;
	if ((pid = fork()) < 0)
		exit(0);
	else if (pid == 0) {
		//setbuf(stdout,NULL);
		printf("child");
		exit(0);
	} 
	setbuf(stdout,NULL);
	printf("parent");
	if (waitpid(pid,NULL,0) != pid)
		exit(1);
    exit(0);
}
