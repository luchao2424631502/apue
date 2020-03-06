#include "apue.h" 
#include <pthread.h>

int finished = 0;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *
do_homework(void *arg) {
	// 学生睡眠5s就是先放弃竞争
	sleep(5);
	pthread_mutex_lock(&lock);
	finished = 1;
	// 在通知　睡眠的线程唤醒前,自己先要把锁释放掉
	pthread_mutex_unlock(&lock);
	
	//pthread_cond_signal(&cond);
	pthread_cond_broadcast(&cond);
	printf("发送条件信号\n");
}

void *check_homework(void *arg) {
	sleep(1);
	pthread_mutex_lock(&lock);
	// 轮询条件
	printf("老师: 作业写完了吗?\n");
	while (finished == 0) {
		printf("学生: 没有写完\n");
		printf("老师: 接着写\n");
		printf("------------\n");
		//第一次说接着写,然后就进入了　睡眠阶段
		pthread_cond_wait(&cond,&lock);
		printf("老师: 作业写完了吗?\n");
	} 
	// 检查完了,那么等下一次学生交作业
	finished = 0;
	printf("学生: 写完了\n");
	pthread_mutex_unlock(&lock);
	printf("老师开始检查\n");
}
 
int main(int argc,char *argv[]) {
	pthread_t tid1,tid2,tid3;
	pthread_create(&tid1,NULL,do_homework,NULL);
	pthread_create(&tid2,NULL,check_homework,NULL);
	pthread_create(&tid3,NULL,check_homework,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);
    exit(0);
}
