#include "apue.h" 

#define SIG2STR_MAX 128

int sig2str(int signo,char *str) {
	strcpy(str,strsignal(signo));
	if (str == NULL)
		return -1;
	return 0;
}

int main(int argc,char *argv[]) {
	// 手动分配足够的内存
	char *str = malloc(SIG2STR_MAX * sizeof(char));
	int status;
	if ((status = sig2str(SIGUSR1,str)) < 0)
		err_sys("sig2str error");
	printf("%s",str);
	free(str);
    exit(0);
}
