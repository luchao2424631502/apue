#include "apue.h" 
#include <pthread.h>
 
pthread_spinlock_t lock;

void *
func1(void *arg)
{
	printf("func1 before\n");
	pthread_spin_lock(&lock);
	printf("func1 中间\n");
	sleep(1);

	printf("func1 end\n");
	pthread_spin_unlock(&lock);	
	return (void *)0;
}

void *
func2(void *arg)
{
	printf("func2 before\n");
	pthread_spin_lock(&lock);
	printf("func2 sleep\n");
	sleep(10);
	printf("func2 end\n");
	pthread_spin_unlock(&lock);
	return (void *)0;
}

int main(int argc,char *argv[]) {
	pthread_spin_init(&lock,PTHREAD_PROCESS_SHARED);
	pthread_t tid1,tid2;

	pthread_create(&tid1,NULL,func1,NULL);
	pthread_create(&tid2,NULL,func2,NULL);


	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_spin_destroy(&lock);
    exit(0);
}
