#include "apue.h" 
#include <pthread.h>
 
int 
main(int argc,char *argv[]) {
	int err;
	struct timespec tout;
	struct tm *tmp;
	char buf[64];
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&lock);
	printf("mutex is locked\n");
	clock_gettime(CLOCK_REALTIME,&tout);
	tmp = localtime(&tout.tv_sec);
    exit(0);
}
