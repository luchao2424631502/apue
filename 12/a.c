#include "init.h"
 
extern int makethread(void *(*)(void *),void *);

struct to_info {
	void (*to_fn)(void *);
	void *to_arg;
	struct timespec to_wait;
}; 

#define SECTONSEC 1000000000

#if !defined(CLOCK_REALTIME) || defined(BSD)
#define clock_nanosleep(ID,FL,REQ,REM) nanosleep((REQ),(REM))
#endif

// if define clock_realtime
#ifndef CLOCK_REALTIME

#define CLOCK_REALTIME 0
#define USECTONSEC 1000

void 
clock_gettime(int id,struct timespec *tsp) {
	struct timeval tv;
	
	gettimeofday(&tv,NULL);
	tsp->tv_sec = tv.tv_sec;
	tsp->tv_nsec = tv.tv_usec * USECTONSEC;
}
// }
#endif

void *
timeout_helper(void *arg) {
	struct to_info *tip;

	tip = (struct to_info *)arg;
	// 通过nanosleep函数沉睡剩下的时间
	clock_nanosleep(CLOCK_REALTIME,0,&tip->to_wait,NULL);
	(*tip->to_fn)(tip->to_arg);
	free(arg);
	return 0;
}

// 超时调用函数
void
timeout(const struct timespec *when,void (*func)(void *),void *arg) {
	struct timespec now;
	struct to_info *tip;
	int err;

	clock_gettime(CLOCK_REALTIME,&now);
	// 时间还没有到达
	if ((when->tv_sec > now.tv_sec) ||
			(when->tv_sec == now.tv_sec && when->tv_nsec > now.tv_nsec)) {
		tip = malloc(sizeof(struct to_info));
		if (tip != NULL) {
			tip->to_fn = func;
			tip->to_arg = arg;
			tip->to_wait.tv_sec = when->tv_sec - now.tv_sec;
			// 整合纳秒
			if (when->tv_nsec >= now.tv_nsec) 
				tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
			else {
				tip->to_wait.tv_sec--;
				tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec + SECTONSEC;
			}
			err = makethread(timeout_helper,(void *)tip);
			if (err == 0)
				return ;
			else
				free(tip);
		}
	}
	// 时间已经过去
	(*func)(arg);
}

pthread_mutexattr_t attr;
pthread_mutex_t mutex;

void 
retry(void *arg) {
	pthread_mutex_lock(&mutex);

	pthread_mutex_unlock(&mutex);
}

int main(int argc,char *argv[]) {
	int err,condition,arg;
	struct timespec when;

	// 初始化mutex 的 attr
	if ((err = pthread_mutexattr_init(&attr)) != 0)
		err_exit(err,"pthread_mutexattr_init failed");
	// 设置attr
	if ((err = pthread_mutexattr_settype(&attr,
					PTHREAD_MUTEX_RECURSIVE)) != 0)
		err_exit(err,"can't set recursive type");
	// init mutex
	if ((err = pthread_mutex_init(&mutex,&attr)) != 0)
		err_exit(err,"can't create recursive mutex");

	pthread_mutex_lock(&mutex);
	if (condition) {
		clock_gettime(CLOCK_REALTIME,&when);
		when.tv_sec += 10;
		timeout(&when,retry,(void *)((unsigned long)arg));
	}
	pthread_mutex_unlock(&mutex);
    exit(0);
}
