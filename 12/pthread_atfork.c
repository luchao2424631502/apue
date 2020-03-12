#include "init.h"
 
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void 
prepare(void) {
	int err;
	printf("preparing locks \n");
	if ((err = pthread_mutex_lock(&lock1)) != 0)
		err_cont(err,"can't lock lock1 in prepare handler");
	if ((err = pthread_mutex_lock(&lock2)) != 0)
		err_cont(err,"can't lock lock2 in prepare handler");
}

void 
parent(void) {
	int err;

	printf("parent unlocking locks\n");
	if ((err = pthread_mutex_unlock(&lock1)) != 0)
		err_cont(err,"can't unlock lock1 in parent handler");
	if ((err = pthread_mutex_unlock(&lock2)) != 0)
		err_cont(err,"can't unlock lock2 in parent handler");
}

void 
child(void) {
	int err;

	printf("child unlocking locks\n");
	if ((err = pthread_mutex_unlock(&lock1)) != 0)
		err_cont(err,"can't unlock lock1 in child handler");
	if ((err = pthread_mutex_unlock(&lock2)) != 0)
		err_cont(err,"can't unlock lock2 in child handler");
}

void*
thread(void *arg) {
	printf("thread started\n");
	// 强制暂停
	pause();
	printf("end thread\n");
	return 0;
}

int main(int argc,char *argv[]) {
	int		  err; 
	pid_t	  pid;
	pthread_t tid;


	if ((err = pthread_atfork(prepare,parent,child)) != 0)
		err_exit(err,"can't install fork handlers");
	// 创建线程
	if ((err = pthread_create(&tid,NULL,thread,0)) != 0)
		err_exit(err,"can't create thread");

	sleep(2);
	printf("parent about to fork\n");

	if ((pid = fork()) < 0)
		err_quit("fork failed");
	else if (pid == 0)
		printf("child returned from fork\n");
	else 
		printf("parent returned from fork\n");
    exit(0);
}
