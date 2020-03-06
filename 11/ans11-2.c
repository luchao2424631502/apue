#include "apue.h" 
#include <pthread.h>

struct job {
	struct job *j_next;
	struct job *j_prev;
	pthread_t j_id; // 线程id

	// 通过引用计数和互斥量进行修改job id
	pthread_mutex_t j_lock;
	int j_count;
};

struct queue {
	struct job *q_head;
	struct job *q_tail;
	pthread_rwlock_t q_lock;
};

int 
queue_init(struct queue *que)
{
	int err;
	que->q_head = NULL;
	que->q_tail = NULL;
	// 初始化任务队列的读写锁
	err = pthread_rwlock_init(&que->q_lock,NULL);
	if (err != 0)
		return err;
	return 0;
}

// job插入到队列头
void
job_insert(struct queue *que,struct job *jp)
{
	// 互斥锁住
	pthread_rwlock_wrlock(&que->q_lock);
	jp->j_next = que->q_head;
	jp->j_prev = NULL;
	if (que->q_head != NULL)
		que->q_head->j_prev = jp;
	else 
		// 队列中只有１个job了
		que->q_tail = jp;
	que->q_head = jp;

	pthread_rwlock_unlock(&que->q_lock);
}

// job插入到列队尾部
void 
job_append(struct queue *que,struct job *jp)
{
	pthread_rwlock_wrlock(&que->q_lock);
	jp->j_next = NULL;
	jp->j_prev = que->q_tail;
	if (que->q_tail != NULL)
		que->q_tail->j_next = jp;
	else
		//队列中只有１个job了
		que->q_head = jp;
	que->q_tail = jp;
	pthread_rwlock_unlock(&que->q_lock);
}

// 从job队列中　移出job
void
job_remove(struct queue *que,struct job *jp)
{
	pthread_rwlock_wrlock(&que->q_lock);

	if (jp == que->q_head) {	
		que->q_head = jp->j_next;
		// 队列只剩一个就是jp
		if (que->q_tail == jp)
			que->q_tail == NULL;
		else
			jp->j_next->j_prev = jp->j_prev;
		// jp是最后一个,且队列长度>1
	} else if (jp == que->q_tail) {
		que->q_tail = jp->j_prev;
		jp->j_prev->j_next = jp->j_next;
	} else {
		jp->j_prev->j_next = jp->j_next;
		jp->j_next->j_prev = jp->j_prev;
	}
	jp->j_count--;
	pthread_rwlock_unlock(&que->q_lock);
}

// 从队列中找到某个job
struct job *
job_find(struct queue *que,pthread_t id)
{
	struct job *jp;

	// 系统对读锁被占有的线程个数有限制,所以检查返回值
	if (pthread_rwlock_rdlock(&que->q_lock) != 0)
		return NULL;

	for (jp = que->q_head; jp != NULL; jp = jp->j_next)
		if (pthread_equal(id,jp->j_id)) {
			// 读模式的锁,此时增加引用计数需要使用互斥量
			pthread_mutex_lock(&jp->j_lock);
			jp->j_count++;
			pthread_mutex_unlock(&jp->j_lock);
			break;
		}
	pthread_rwlock_unlock(&que->q_lock);
	return jp;
}

// 修改队列中job的
void *
job_chid(struct job *jp,int newid)
{
	// 使用互斥量和引用计数就不使用写模式的锁了
	pthread_mutex_lock(&jp->j_lock);
	if (jp->j_count > 0) {
		pthread_mutex_unlock(&jp->j_lock);
		return (void *)-1;	
	} else 
		jp->j_id = newid;
	pthread_mutex_unlock(&jp->j_lock);
	return (void *)0;
}

int 
main(int argc,char *argv[]) {
	exit(0);
}
