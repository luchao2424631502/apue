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

int main() {
	int err;
	pid_t pid;
	pthread_t tid;

	pthread_create(&tid,NULL,func,NULL);
	printf("parent about to fork\n");

	// 因为 另一个线程占用锁,我们要等待它释放锁后,主线程然后再占用锁
	pthread_mutex_lock(&lock);
	pid = fork();
	pthread_mutex_unlock(&lock);
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
