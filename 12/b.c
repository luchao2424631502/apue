#include "apue.h" 
#include <stdlib.h>
#include <pthread.h>
 
pthread_mutex_t lock;

void 
printtype(pthread_mutexattr_t *attr) {
	int type;
	pthread_mutexattr_gettype(attr,&type);
	if (type == PTHREAD_MUTEX_NORMAL) 
		printf("pthread_mutex_normal\n");
	else if (type == PTHREAD_MUTEX_ERRORCHECK) 
		printf("pthread_mutex_errorcheck\n");
	else if (type == PTHREAD_MUTEX_RECURSIVE) 
		printf("pthread_mutex_recursive\n");
	else if (type == PTHREAD_MUTEX_DEFAULT)
		printf("pthread_mtuex_default\n");
}

void 
setrecursive(pthread_mutexattr_t *attr,int recursive) {
	int err = pthread_mutexattr_settype(attr,
			recursive?PTHREAD_MUTEX_NORMAL:PTHREAD_MUTEX_RECURSIVE);
	if (err != 0) {
		printf("set recursive type mutex error");
		exit(-1);
	}
}

void 
handler(int signo) {
	// 信号处理程序中 加锁
	pthread_mutex_lock(&lock);
	
	if (signo == SIGUSR1)
		printf("SIGUSR1 handle\n");
	if (signo == SIGINT)
		printf("SIGINT handle\n");

	sleep(5);

	pthread_mutex_unlock(&lock);
	printf("out handle\n");
}

int main(int argc,char *argv[]) {
	// 通过命令行参数来改变互斥量类型
	int recursive = 1;
	if (argc >= 2) 
		recursive = 0;
	
	// 定义mutex属性
	pthread_mutexattr_t attr;
	// 初始化属性
	pthread_mutexattr_init(&attr);
	// 打印属性
	printtype(&attr);
	
	// 设置属性
	setrecursive(&attr,recursive);
	printtype(&attr);

	// 初始化mutex
	pthread_mutex_init(&lock,&attr);
	// 回收attr
	pthread_mutexattr_destroy(&attr);

	// 注册信号处理函数
	signal(SIGUSR1,handler);
	signal(SIGINT ,handler);

	kill(getpid(),SIGUSR1);
	
	// 回收mutex
	pthread_mutex_destroy(&lock);
    exit(0);
}
