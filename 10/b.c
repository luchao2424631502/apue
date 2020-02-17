#include <stdlib.h>
#include <signal.h>
#include <errno.h>
 
#define SIGBAD(signo) ((signo) <= 0 || (signo) >= NSIG)

int 
sigaddset(sigset_t *set,int signo)
{
	// 首先测试信号是否超出范围
	if (SIGBAD(signo)) {
		errno = EINVAL;
		return -1;
	}
	// signo - 1 是相应的位置
	*set |= 1 << (signo - 1);
	return 0;
}

int 
sigdelset(sigset_t *set,int signo)
{
	if (SIGBAD(signo)) {
		errno = EINVAL;
		return -1;
	}
	// 000001
	// 000100
	// 111011
	// 111111
	// 111011
	*set &= ~(1 << (signo - 1));
	return 0;
}
int 
setismember(const sigset_t *set,int signo)
{
	if (SIGBAD(signo)) {
		errno = EINVAL;
		return -1;
	}
	return ((*set & (1 << (signo - 1))) != 0);
}

int main(int argc,char *argv[]) {
 
    exit(0);
}
