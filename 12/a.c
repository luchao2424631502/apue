#include "apue.h" 
#include <pthread.h>
 
int main(int argc,char *argv[]) {
	pthread_mutexattr_t *attr = (pthread_mutexattr_t *)malloc(sizeof(pthread_mutexattr_t));
	pthread_mutexattr_init(attr);
	
	int ans;
	pthread_mutexattr_getpshared(attr,&ans);
	printf("%s\n",(ans == PTHREAD_PROCESS_PRIVATE) ?
			"PTHREAD_PROCESS_PRIVATE":
			"NO");

	pthread_mutexattr_gettype(attr,&ans);
	printf("%s\n",(ans == PTHREAD_MUTEX_NORMAL)?"PTHREAD_MUTEX_NORMAL":"no");
	
	pthread_mutexattr_settype(attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutexattr_gettype(attr,&ans);
	printf("%s\n",(ans == PTHREAD_MUTEX_RECURSIVE)?"PTHREAD_MUTEX_RECURSIVE":"NO");	

	pthread_mutexattr_destroy(attr);
	free(attr);
    exit(0);
}
