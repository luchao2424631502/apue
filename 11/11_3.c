#include "apue.h" 
#include <pthread.h>

void *
thr_fn1(void *arg)
{
	printf("thread 1\n");
	// 调用pthread_exit 退出线程
	pthread_exit((void *)8888888);
}

void *
thr_fn2(void *arg)
{
	printf("thread 2\n");
	// 线程简单的返回
	return ((void *)2222222222);
}
 
int main(int argc,char *argv[]) {
	pthread_t tid1,tid2;
	int err;
	void *ret;

	pthread_create(&tid1,NULL,thr_fn1,NULL);
	pthread_create(&tid2,NULL,thr_fn2,NULL);

	// void**
	pthread_join(tid1,&ret);
	printf("thread 1 return %ld\n",(long)ret);
	pthread_join(tid2,&ret);
	printf("thread 2 return %ld\n",(long)ret);

    exit(0);
}
