#include "init.h"

#define PERR(msg) do { perror(msg); exit(-1); } while(0) 
#define PPERR(err,msg) do { err=errno; perror(msg); exit(-1); } while(0)

struct ticket {
	int remain;
	pthread_mutex_t lock;
};

int main(int argc,char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s <name>\n",argv[0]);
		exit(-1);
	} 

	char *name = argv[1];
	int err,shared,flag = 1;
	key_t key = 0x8888;
	int id = shmget(key,0,0);
	if (id < 0) PERR("shmget");

	struct ticket *t = (struct ticket*)shmat(id,NULL,0);

	if (t == (void *)-1) PERR("shmat");

	while (flag) {
		err = pthread_mutex_lock(&t->lock);
		// 需要恢复的成功
		if (err == EOWNERDEAD) {
			puts("EOWNERDEAD");
			// 拥有互斥锁的进程终止后,互斥量就变成inconsistent(不一致性)
			// 恢复锁的一致性,此时进程还是获得锁的状态
			err = pthread_mutex_consistent(&t->lock);
			if (err != 0) {
				printf("consistent error\n");
				exit(-1);
			}
		}
		// 失败
		else if (err == ENOTRECOVERABLE) {
			puts("ENOTRECOVERABLE");
		} 
		// 不需要恢复的成功
		int remain = t->remain;
		if (remain > 0) {
			sleep(1);
			--remain;
			t->remain = remain;
			printf("%s buy a ticket\n",name);
			sleep(3);
		} else 
			flag = 0;
		pthread_mutex_unlock(&t->lock);
		sleep(2);
	}
	err = shmdt((void *)t);
	if (err != 0) PERR("shmdt");
	return 0;
}
