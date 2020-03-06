#include "apue.h" 
#include <pthread.h>
 
int main(int argc,char *argv[]) {
	pthread_mutexattr_t *attr = (pthread_mutexattr_t *)malloc(sizeof(pthread_mutexattr_t));
	pthread_mutexattr_init(attr);
	
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex,attr);
	pthread_mutex_destroy(&mutex);
	
	int ans;
	pthread_mutexattr_getpshared(attr,&ans);
	printf("%d\n",(ans == PTHREAD_PROCESS_PRIVATE)?1:0);

	pthread_mutexattr_destroy(attr);
	free(attr);
    exit(0);
}
