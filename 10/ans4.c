#include "apue.h" 
#include <errno.h>

void 
pr_mask(const char *str)
{
	sigset_t sigset;
	int errno_save;

	errno_save = errno;
	if (sigprocmask(0,NULL,&sigset) < 0)
		err_ret("sigprocmask error");
	else {
		printf("%s",str);
		for (int i=1; i<=32; i++)
			if (sigismember(&sigset,i))
				printf(" %s\n",strsignal(i));
	
	}
}
 
int main(int argc,char *argv[]) {
	sigset_t newmask;
	sigemptyset(&newmask);
	sigaddset(&newmask,1);
	sigprocmask(SIG_SETMASK,&newmask,NULL);
	pr_mask("test: ");
    exit(0);
}
