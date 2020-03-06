#include "apue.h" 
#include <pthread.h>

pthread_barrier_t barrier;
 
void *thread(void *arg) {
	pthread_t id = (pthread_t)arg;
	int res = 0;
	printf("thread %ld\n",id);
	res = pthread_barrier_wait(&barrier);
	printf("thread %d returning %d \n",id,res);
	return (void *)0;
}

int main(int argc,char *argv[]) {
	int res = 0;
	pthread_t tid[10];
	printf("pthread_barrier_serial_thread = %d\n",PTHREAD_BARRIER_SERIAL_THREAD);

	pthread_barrier_init(&barrier,NULL,11);
	
	int i;
	for (i=0; i<10; i++) 
		pthread_create(&tid[i],NULL,thread,(void*)i);

	res = pthread_barrier_wait(&barrier);
	printf("thread [main] return [%d]\n",res);

	for (i=0; i<10; i++)
		pthread_join(tid[i],NULL);
	
	pthread_barrier_destroy(&barrier);
	

    exit(0);
}
