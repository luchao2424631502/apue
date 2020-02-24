#include "apue.h" 
#include <pthread.h>

pthread_mutex_t mutex1,mutex2;

void *
func1(void *arg)
{
	pthread_mutex_lock(&mutex1);

	sleep(2);

	pthread_mutex_lock(&mutex2);
}

// sleep的目的是想让另一个线程开始执行

void *
func2(void *arg)
{
	pthread_mutex_lock(&mutex2);
	
	sleep(1.5);
	
	pthread_mutex_lock(&mutex1);
}
 
int main(int argc,char *argv[]) {
	pthread_t tid1,tid2;
	
	//创建线程
	pthread_create(&tid1,NULL,func1,NULL);
	pthread_create(&tid2,NULL,func2,NULL);



	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
    exit(0);
}
