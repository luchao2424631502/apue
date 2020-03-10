#include "init.h"
#include <signal.h> 
#include <stdlib.h>

#define PPERR(err,msg) do { errno = err; perror(msg); exit(-1); } while(0)

struct ticket {
	int remain;
	pthread_mutex_t lock;
};

struct ticket t;

void printtype(pthread_mutexattr_t *attr) {
	int err,type;
	char *s = "???";
	err = pthread_mutexattr_gettype(attr,&type);
	if (err != 0) PPERR(err,"pthread_mutexattr_gettype");	

	if (type == PTHREAD_MUTEX_NORMAL) {
		s = "pthread_mutex_normal";
		printf("mutex type = %s\n",s);
	}
	else if (type == PTHREAD_MUTEX_ERRORCHECK) {
		s = "pthread_mutex_errorcheck";
		printf("mutex type = %s\n",s);
	}
	else if (type == PTHREAD_MUTEX_RECURSIVE) {
		s = "pthread_mutex_recursive";
		printf("mutex type = %s\n",s);
	} else {
		s = "PTHREAD_MUTEX_DEFAULT";
		printf("mutex type = %s\n",s);
	}
}

void setrecursive(pthread_mutexattr_t *attr) {
	int err = pthread_mutexattr_settype(attr,PTHREAD_MUTEX_RECURSIVE);
	if (err != 0) PPERR(err,"pthread_mutexattr_settype");
}

void handler(int sig) {
	if (sig == SIGUSR1)
		printf("receive SIGUSR1\n");
	else if (sig == SIGINT) 
		printf("receive sigint\n");
	pthread_mutex_lock(&t.lock);
	printf("%s enter handler \n",(sig == SIGUSR1) ? "sigusr1" : "sigint");
	t.remain++;
	sleep(3);
	printf("%s enter handler \n",(sig == SIGUSR1) ? "sigusr1" : "sigint");
	pthread_mutex_unlock(&t.lock);
}

int main(int argc,char *argv[]) {
	int recursive = 0;
	if (argc >= 2) recursive = 1;

	t.remain = 3;

	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	printtype(&attr);
	// 多参数
	if (recursive == 1) {
		printf("modify type --------------\n");
		setrecursive(&attr);
		printtype(&attr);
	}
	// 初始化互斥量
	pthread_mutex_init(&t.lock,&attr);
	pthread_mutexattr_destroy(&attr);

	signal(SIGUSR1,handler);
	signal(SIGINT ,handler);
	
	printf("send SIGUSR1\n");
	kill(getpid(),SIGUSR1);

	pthread_mutex_destroy(&t.lock);

    exit(0);
}
