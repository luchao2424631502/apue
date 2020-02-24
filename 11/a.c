#include "apue.h" 
#include <pthread.h>

struct foo {
	int f_count;
	pthread_mutex_t f_lock;
	int f_id;
	int f_use;
};
 
struct foo *
foo_alloc(int id)
{

	struct foo *fp;

	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->f_count = 1;
		fp->f_id = id;
		if (pthread_mutex_init(&fp->f_lock,NULL) != 0) {
			free(fp);
			return NULL;
		}
	}
	return fp;
}

void 
foo_hold(struct foo *fp)
{
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;
	pthread_mutex_unlock(&fp->f_lock);
}

void 
foo_rele(struct foo *fp)
{
	pthread_mutex_lock(&fp->f_lock);
	if (--fp->f_count == 0) {
		pthread_mutex_unlock(&fp->f_lock);
		// 如果互斥量是动态分配的,在回收内存前要destroy(互斥量)
		pthread_mutex_destroy(&fp->f_lock);
		printf("end\n");
		free(fp);
	}

	pthread_mutex_unlock(&fp->f_lock);
}

void *
func(void *arg)
{
	// 使用对象前先原子操作(互斥)增加引用计数
	//foo_hold((struct foo *)arg);
	
	// 不仅使用动态对象需要管理他在多线程环境下的内存情况,使用他的成员还是避免互斥
	// 操作f_use,先上锁
	//pthread_mutex_lock(&((struct foo *)arg)->f_lock);
	((struct foo *)arg)->f_use += 3;
	int a = 1;
	int b = 2;
	for (int i=0; i<17;i++)
		((struct foo *)arg)->f_use += 2;
//	printf("f_use = %d\n",((struct foo *)arg)->f_use);	

	//pthread_mutex_unlock(&((struct foo *)arg)->f_lock);

	// 线程结束使用该　动态分配的内存对象
	//foo_rele((struct foo *)arg);
	return ((void *)0);
}

const int maxn = 2500;

int main(int argc,char *argv[]) {
	pthread_t tid[maxn]; 
	struct foo *p = foo_alloc(99);
	// 3个线程的线程池
	for (int i=0; i<maxn; i++) 
		pthread_create(&tid[i],NULL,func,(void *)p);

	// ３个元素的数组,元素是void *
	for (int i=0; i<maxn; i++) {
		pthread_join(tid[i],NULL);
	}
	printf("f_use = %d\n",p->f_use);
	free(p);
    exit(0);
}
