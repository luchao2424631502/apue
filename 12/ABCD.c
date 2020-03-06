#include "apue.h" 
#include <pthread.h>

typedef void *(*Func)(void *);

int all = 0;
int c[5];
pthread_cond_t cond[5];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *
thread1(void *arg) {
	while (1) {
		pthread_mutex_lock(&lock);
		while (c[1] == 0) { 
			pthread_cond_wait(&cond[1],&lock);
			printf("A");
			c[1] = 1;
			c[2] = 0;
		}
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cond[2]);
	}
}

void *
thread2(void *arg) {
	while (1) {
		pthread_mutex_lock(&lock);
		while (c[2] == 0) { 
			pthread_cond_wait(&cond[2],&lock);
			printf("B");
			c[2] = 1;
			c[3] = 0;
		}
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cond[3]);
	}
}

void *
thread3(void *arg) {
	while (1) {
		pthread_mutex_lock(&lock);
		while (c[3] == 0) { 
			pthread_cond_wait(&cond[3],&lock);
			printf("C");
			c[3] = 1;
			c[4] = 0;
		}
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cond[4]);
	}
}

void *
thread4(void *arg) {
	while (1) {
		pthread_mutex_lock(&lock);
		while (c[4] == 0) { 
			pthread_cond_wait(&cond[4],&lock);
			printf("D");
			if (++all == 4)
				pthread_exit(NULL);
			c[4] = 1;
			c[1] = 0;
		}
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&cond[1]);
	}
}

int main(int argc,char *argv[]) {
	// 标准io无缓冲
	setbuf(stdout,NULL);
	pthread_t tid[5];
	Func func[5] = {NULL,thread1,thread2,thread3,thread4};
	for (int i=1; i<=4; i++)
		cond[i] = (pthread_cond_t)PTHREAD_COND_INITIALIZER;	
	for (int i=1; i<=4; i++)
		pthread_create(&tid[i],NULL,func[i],NULL);

	pthread_cond_signal(&cond[1]);

	for (int i=1; i<=4; i++)
		pthread_join(tid[i],NULL);
    exit(0);
}
