#include "apue.h" 
#include <pthread.h>

void 
clean_func(void *arg)
{
	printf("clean func\n");
}

void *
thr_func(void *arg)
{
	printf("thread 1 \n");		
	
	pthread_cleanup_push(clean_func,NULL);

	pthread_cancel(pthread_self());
	pthread_exit((void *)15);
	// 都将删除上次建立的清理处理程序,由参数来决定删除前是否调用
	// 成对出现 {}作用域问题
	pthread_cleanup_pop(1);
}

 
int main(int argc,char *argv[]) {
	pthread_t tid;
	void *res;
	pthread_create(&tid,NULL,thr_func,NULL);
	
	
	pthread_join(tid,&res);
	printf("res %d\n",(int)res);
    exit(0);
}
