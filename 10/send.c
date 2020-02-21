#include "apue.h" 
 
#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

int main(int argc,char *argv[]) {
	if (argc < 2) {
		fprintf(stderr,"Usage %s pid\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	pid_t pid = atoi(argv[1]);
	// int sigval_int void* sigval_ptr
	union sigval val;
	val.sival_int = 100;

	// kill(pid,SIGINT), val是多余信息
	sigqueue(pid,SIGINT,val);
    exit(0);
}
