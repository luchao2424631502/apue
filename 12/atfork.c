#include "init.h"
 
int total;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void*
func(void *arg) {
	while(1) {
		pthread_mutex_lock(&lock);
		total++;
		printf("func: total++\n");
		// fork子进程,再次获得锁,导致死锁
		sleep(5);
		pthread_mutex_unlock(&lock);
		sleep(1);	
	}
	return (void *)0;
}

void prepare() {
	int err;
	printf("preparing lock\n");
	// 获得锁,进入临界区,好fork()
	err = pthread_mutex_lock(&lock);
	if (err != 0) err_exit(err,"prepare lock failed");
}

void parent() {
	int err;
	printf("parent unlocking lock\n");
	err = pthread_mutex_unlock(&lock);
	if (err != 0) err_exit(err,"parent unlock failed");
}

void child() {
	int err;
	printf("child unlocking lock\n");
	err = pthread_mutex_unlock(&lock);
	if (err != 0) err_exit(err,"child unlock failed");
}

int main() {
	int err;
	pid_t pid;
	pthread_t tid;

	pthread_create(&tid,NULL,func,NULL);
	printf("parent about to fork\n");

	err = pthread_atfork(prepare,parent,child);
	if (err != 0) err_exit(err,"atfork error");
	// 因为 另一个线程占用锁,我们要等待它释放锁后,主线程然后再占用锁
	pid = fork();
	if (pid < 0) err_sys("fork error");
	// 子线程
	else if (pid == 0) {
		int status;
		while (1) {
			printf("child require lock\n");
			// 子进程 获得锁,
			pthread_mutex_lock(&lock);
			total++;
			printf("child: total++\n");
			sleep(2);
			pthread_mutex_unlock(&lock);
			sleep(1);
		}
		exit(0);
	}

	pthread_join(tid,NULL);
}
