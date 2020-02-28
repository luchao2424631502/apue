#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

void
maketimeout(struct timespec *tsp,long minutes)
{
	struct timeval now;
	gettimeofday(&now,NULL);
	tsp->tv_sec = now.tv_sec;
	tsp->tv_nsec = now.tv_usec * 1000;
	tsp->tv_sec += minutes * 60;
}

struct msg {
	struct msg *m_next;
};

struct msg *workq;

pthread_cond_t qready = PTHREAD_COND_INITIALIZER;

pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;


// 消费者(消费消息) 
void  
process_msg(void)
{
	struct msg *mp;
	for (;;) {
		pthread_mutex_lock(&qlock);
		// 如果工作队列为空,则线程进入等待(循环等待)
		while (workq == NULL) 
			// 线程放入等待条件的线程列表上
			pthread_cond_wait(&qready,&qlock);
		// 处于等待的线程结束等待后,立即进入临界区
		mp = workq;
		workq = mp->m_next;
		printf("proc msg \n");
		pthread_mutex_unlock(&qlock);
	}
}

// 生产者(生产消息)
void 
enqueue_msg(struct msg *mp)
{
	pthread_mutex_lock(&qlock);
	mp->m_next = workq;
	workq = mp;
	printf("create \n");
	pthread_mutex_unlock(&qlock);
	// 给处于条件变量等待队列中的线程发信号,
	pthread_cond_signal(&qready);

}

void *
func1(void *arg)
{
	process_msg();
	return (void *)0;
}

void *
func2(void *mp)
{
	enqueue_msg(mp);
	return (void *)0;
}

const int maxn = 100;

int main(int argc,char *argv[]) {
	pthread_t tid[maxn];
	struct msg* p[maxn];
	for (int i=0; i<maxn; i++)
		p[i] = (struct msg *)malloc(sizeof(struct msg));
		
	pthread_t tid1;
	pthread_create(&tid1,NULL,func1,NULL);
	//我先创造maxn个生产者
	for (int i=0; i<maxn; i++)
		pthread_create(&tid[i],NULL,func2,(void *)p[i]);
	
	for (int i=0; i<maxn; i++)
		pthread_join(tid[i],NULL);
	for (int i=0; i<maxn; i++)
		free(p[i]);
	pthread_join(tid1,NULL);
    exit(0);
}
