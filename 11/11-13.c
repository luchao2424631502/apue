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
	strftime(buf,sizeof(buf),"%r",tmp);
	printf("current time is %s\n",buf);
	tout.tv_sec += 10;

	sleep(5);
	pthread_mutex_unlock(&lock);
	// 2次对自己加锁,就会造成死锁
	// 在绝对时间前,只要锁释放了就锁住,否则error返回
	err = pthread_mutex_timedlock(&lock,&tout);
	clock_gettime(CLOCK_REALTIME,&tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf,sizeof(buf),"%r",tmp);
	printf("the time is now %s\n",buf);

	if (err == 0)
		printf("mutex locked again!\n");
	else 
		printf("can't lock mvtex again:%s\n",strerror(err));
    exit(0);
}