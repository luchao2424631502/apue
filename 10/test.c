#include "apue.h" 

static void 
sig_usr(int signo)
{
	sigset_t newmask,oldmask;
	sigemptyset(&newmask);
	sigprocmask(SIG_BLOCK,&newmask,&oldmask);
	if (sigismember(&oldmask,SIGUSR1))
		printf("阻塞\n");
	else 
		printf("no\n");
}
 
int main(int argc,char *argv[]) {
	//signal(SIGUSR1,sig_usr);
	struct sigaction act;
	act.sa_handler = sig_usr;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGUSR1,&act,NULL);
	kill(getpid(),SIGUSR1);
	kill(getpid(),SIGUSR1);
	kill(getpid(),SIGUSR1);
    exit(0);
}
