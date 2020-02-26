#include <stdlib.h>
#include <pthread.h>

#define NHASH 29
#define HASH(id) (((unsigned long)id)%NHASH)
 
struct foo *fh[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;
// initializer

struct foo {
	int f_count;
	pthread_mutex_t f_lock;
	int f_id;
	struct foo *f_next;
};

struct foo *
foo_alloc(int id)
{
	struct foo *fp;
	int index;

	if ((fp = malloc(sizeof(struct foo))) != NULL) {
		fp->f_count = 1;
		fp->f_id = id;
		// 第二个参数mutex_t attr,返回互斥量的错误码
		if (pthread_mutex_init(&fp->f_lock,NULL) != 0) {
			free(fp);
			// 分配失败
			return NULL;
		}
		// 根据id得到链表索引
		index = HASH(id);
		pthread_mutex_lock(&hashlock);
		fp->f_next = fh[index];
		fh[index] = fp;
		pthread_mutex_lock(&fp->f_lock);
		// 和前面一个,在全局锁解锁前,先把自己加锁
		pthread_mutex_unlock(&hashlock);
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
	// 操作链表,多个线程可能同时找个一个然后同时操作,所以锁住hashlock
	pthread_mutex_lock(&hashlock);
	for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) {
		if (fp->f_id == id) {
			fp->f_count++;
			break;
		}
	}
	pthread_mutex_unlock(&hashlock);
	return fp;
}

void 
foo_rele(struct foo *fp)
{
	struct foo *tfp;
	int index;
	pthread_mutex_lock(&hashlock);
	//此时可能有hold线程存在,但是hold中的f_lock能锁住结构体
	if (--fp->f_count == 0) {
		index = HASH(fp->f_id);
		tfp = fh[index];
		if (tfp == fp)
			fh[index] = fp->f_next;
		else {
			while (tfp->f_next != fp)
				tfp = tfp->f_next;
			tfp->f_next = fp->f_next;
		}
		pthread_mutex_unlock(&hashlock);
		pthread_mutex_destroy(&fp->f_lock);
		// 多线程及时被抢占也无所谓,已经从列表中移除了
		free(fp);
	} else
		// 不符合资格就不释放
		pthread_mutex_unlock(&hashlock);
}

int main(int argc,char *argv[]) {
 
    exit(0);
}
