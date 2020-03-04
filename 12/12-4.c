#include "apue.h" 
#include <pthread.h>

void *func(void *arg) {
	printf("func\n");
	return (void *)0;
}

int makethread(void *(*fn)(void *),void *arg) {
	int err;
	pthread_t tid;
	pthread_attr_t *attr = (pthread_attr_t *)malloc(sizeof(pthread_attr_t));

	if ((err = pthread_attr_init(attr)) != 0)
		return err;
	
	// 配置 pthread_attr_t, init后线程默认属性是pthread_create_joinable
	err = pthread_attr_setdetachstate(attr,PTHREAD_CREATE_DETACHED);
	if (err == 0)
		err = pthread_create(&tid,attr,fn,arg);
	
	// 销毁pthread_attr_t
	pthread_attr_destroy(attr);
	free(attr);

	return err;
}
 
int main(int argc,char *argv[]) {
	makethread(func,NULL); 
	
	return 0;
	pthread_exit(NULL);
}
