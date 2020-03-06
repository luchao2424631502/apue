#include "apue.h" 
#include <pthread.h>

pthread_cond_wait(mutex,cond) {
	// 保存条件变量的值
	value = cond->value;
	// 解锁传入的已经锁住的互斥量
	pthread_mutex_unlock(mutex);
	// 这里和pthread_cond_signal/boardcast()有竞争
	pthread_mutex_lock(cond->mutex);
	// 判断竞争谁赢了,我赢了,此线程休眠加入等待队列
	if (value == cond->value) {
		me->next_cond = cond->waiter;
		cond->waiter = me;
		pthread_mutex_unlock(cond->mutex);
		// 进行睡眠
		unable_to_run(me);
	} 
	// 竞争失败,那么就不加入等待队列了,相当于直接(唤醒)
	else 
		pthread_mutex_unlock(cond->mutex);
	// 重新锁住传入的互斥量
	pthread_mutex_lock(mutex);
}

pthread_cond_signal(cond) {
	// 竞争条件
	pthread_mutex_lock(cond->mutex);
	cond->value++;
	// 判断等待队列上是否有线程
	if (cond->waiter) {
		sleeper = cond->waiter;
		cond->waiter = sleeper->next_cond;
		//唤醒等待队列上的线程,且移出等待队列
		able_to_run(sleeper);
	}
	pthread_mutex_unlock(cond->mutex);
} 
 
int main(int argc,char *argv[]) {
 
    exit(0);
}
