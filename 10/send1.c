#include "apue.h" 
 
int main(int argc,char *argv[]) {
	if (argc < 2) {
		fprintf(stderr,"Usage %s pid\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	pid_t pid = atoi(argv[1]);
	union sigval val;
	val.sival_int = 100;
	// 不可靠信号
	sigqueue(pid,SIGINT,val);
	sigqueue(pid,SIGINT,val);
	sigqueue(pid,SIGINT,val);
	// 实时信号
	//sigqueue(pid,SIGRTMIN,val);
	//sigqueue(pid,SIGRTMIN,val);
	//sigqueue(pid,SIGRTMIN,val);
	kill(pid,SIGRTMIN);
	kill(pid,SIGRTMIN);
	kill(pid,SIGRTMIN);
	printf("before sleep\n");

	sleep(3);
	
	printf("sleep 1\n");
	
	kill(pid,SIGUSR1);
	
//	printf("sleep 2\n");
	
//	sleep(3);
	
//	printf("sleep 3\n");
	
//	sigqueue(pid,SIGRTMIN,val);
//	sigqueue(pid,SIGINT,val);
	printf("end\n");
    exit(0);
}
