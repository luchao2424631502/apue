#include "apue.h" 
#include <pthread.h>

pthread_mutex_t mutex;
 
// 同一mutex加锁２此死锁
void *
func(void *arg)
{
	// 尝试２次对mutex进行加锁
	pthread_mutex_lock(&mutex);
	
	// 尝试加锁,但是mutex已经被加锁,于是阻塞,但是又是自己加的锁,自己被阻塞不能解开造成死锁
	pthread_mutex_lock(&mutex);
}

int main(int argc,char *argv[]) {
	pthread_t tid;
	void *ret;

	pthread_create(&tid,NULL,func,NULL);
	
	// 主线程等待线程返回,不然可能主线程return了子线程还没开始执行
	pthread_join(tid,&ret);
    exit(0);
}
