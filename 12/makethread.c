#include "common.h"

int 
makethread(void *(*fn)(void *),void *arg) {
	int err;
	pthread_t tid;
	pthread_attr_t attr;

	err = pthread_attr_init(&attr);
	if (err != 0)
		err = pthread_create(&tid,&attr,fn,arg);
	pthread_attr_destroy(&attr);
	return err;
}
