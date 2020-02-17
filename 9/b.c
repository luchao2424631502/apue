#include "apue.h"
 
static void pr_ids(char *name) {
	printf("%s: pid = %ld,ppid = %ld, pgrp = %ld, tpgrp = %ld\n",
			name,(long)getpid(),(long)getppid(),(long)getpgrp(),
			(long)tcgetpgrp(STDIN_FILENO));
	fflush(stdout);
}

int main(int argc,char *argv[]) {
	pid_t pid;
	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0) {
		pr_ids("child");	
		int sid = setsid();
		pr_ids("child");
		
	}
    exit(0);
}
