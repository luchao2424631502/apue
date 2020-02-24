#include "apue.h" 
#include <pthread.h>

#define NHASH 29
#define HASH(id) (((unsigned long)id)%NHASH)

struct foo *fh[NHASH];

// 只适合静态分配的mutex进行初始化or init (记住mutex使用之前必须初始化)
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo {
	int f_count;
	int f_id;
	pthread_mutex_t f_lock;
	struct foo *f_next;
};

//给struct foo分配内存空间,
struct foo *
foo_alloc(int id)
{
	struct foo *fp;
	int index;
	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->f_count = 1;
		fp->f_id = id;
		// 使用互斥量前一定要初始化
		if (pthread_mutex_init(&fp->f_lock,NULL) != 0) {
			free(fp);
			return NULL;	
		}
		index = HASH(id);
		pthread_mutex_lock(&hashlock);
		fp->f_next = fh[index];
		fh[index] = fp;
		pthread_mutex_lock(&fp->f_lock);
		pthread_mutex_unlock(&hashlock);

		/* 省略初始化,初始化完成之前,阻塞其他线程访问此结构*/

		pthread_mutex_unlock(&fp->f_lock);
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

struct foo *
foo_find(int id)
{
	struct foo *fp;
	
	pthread_mutex_lock(&hashlock);
	// 从hash链表头开始查,
	for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) {
		if (fp->f_id == id) {
			foo_hold(fp);
			break;
		}
	}
	pthread_mutex_unlock(&hashlock);
	// 增加引用计数,并返回指向该结构的指针
	return fp;
}

void 
foo_rele(struct foo *fp)
{
	struct foo *tfp;
	int index;

	// 首先别的线程在我访问时不能访问,则锁住该结构
	pthread_mutex_lock(&fp->f_lock);
	if (fp->f_count == 1) {
		// 解锁,因为想从散列表中删除
		pthread_mutex_unlock(&fp->f_lock);
		// 锁住hashlock,防止被find找到,但是之前已经找到的线程会从此时会增加引用计数,
		// 对散列表加锁,然后对结构加锁
		pthread_mutex_lock(&hashlock);
		pthread_mutex_lock(&fp->f_lock);

		// f_lock unlock后 其他线程找到了并对其进行hold,此时我们返回就行了
		if (fp->f_count != 1) {
			fp->f_count--;
			pthread_mutex_unlock(&fp->f_lock);
			pthread_mutex_unlock(&hashlock);
			return ;
		}

		index = HASH(fp->f_id);
		// tfp是链表头
		tfp = fh[index];
		if (tfp == fp) 
			fh[index] = fp->f_next;
		else {
			// 找到fp在链条中的上一个,然后移除fp
			while (tfp->f_next != fp)
				tfp = tfp->f_next;
			tfp->f_next = fp->f_next;
		}
		pthread_mutex_unlock(&hashlock);
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	} else {
		fp->f_count--;
		pthread_mutex_unlock(&fp->f_lock);
	}
}

int main(int argc,char *argv[]) {
 
    exit(0);
}
