#include "apue.h" 
#include <pthread.h>

pthread_t ntid;

void 
printids(const char *s)
{
	pid_t pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();
	printf("%s pid %lu tid %lu (0x%lx)\n",s,(unsigned long)pid,
			(unsigned long)tid,(unsigned long)tid);
}

// 用来初始化线程的函数,void *
void * 
thr_fn(void *arg)
{
	printids("new thread: ");
	return ((void *)0);
}
 
int main(int argc,char *argv[]) {
	int err;
	// tid,attr,func *,参数struct 指针
	err = pthread_create(&ntid,NULL,thr_fn,NULL);
	// 线程可能在err返回之前就执行完毕,那么err是一块未初始化的内存区域
	if (err != 0)
		err_exit(err,"can't create thread");
	printids("main thread: ");
	// 等待一下线程
	sleep(1);
    exit(0);
}
