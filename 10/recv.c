#include "apue.h" 
 
#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

// 开启SA_SIGINFO标志并用替补handler函数,此函数还要多带其他的参数
void handler(int, siginfo_t *,void *);

int main(int argc,char *argv[]) {
	printf("pid = %d\n",getpid());
	struct sigaction act;
	act.sa_sigaction = handler;
	sigemptyset(&act.sa_mask);
	// sigqueue设置SA_SIGINFO标志位
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SIGINT,&act,NULL) < 0)
		ERR_EXIT("sigaction error");

	for (; ;)
		pause();

    exit(0);
}

void 
handler(int sig,siginfo_t *info,void *other)
{
	// siginfo_t 的成员 sigval_t si_value,
	printf("recv a sig = %d info->si_value.sival_int = %d info->si_int = %d\n",
			sig,info->si_value.sival_int,info->si_int);
}
