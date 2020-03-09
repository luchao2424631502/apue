#include "init.h"
 
#define PERR(msg) \
	do { perror(msg); exit(-1); } while(0)
#define PPERR(err,msg) \
	do { err=errno; perror(msg); exit(-1); } while(0)

struct ticket {
	int remain;
	pthread_mutex_t lock;
};

// 打印mutex的　进程共享属性
void 
printshared(pthread_mutexattr_t *attr) {
	int err,shared;
	err = pthread_mutexattr_getpshared(attr,&shared);
	if (err != 0) PPERR(err,"pthread_mutexattr_getshared");
	
	if (shared == PTHREAD_PROCESS_PRIVATE)
		puts("shared = PTHREAD_PROCESS_PRIVATE");
	else if (shared == PTHREAD_PROCESS_SHARED)
		puts("shared = PTHREAD_PROCESS_SHARED");
	else 
		puts("shared = ???");
}

// 打印mutex的　健壮属性
void 
printrobust(pthread_mutexattr_t *attr) {
	int err,robust;
	err = pthread_mutexattr_getrobust(attr,&robust);
	if (err != 0) PPERR(err,"pthread_mutexattr_getrobust");
	if (robust == PTHREAD_MUTEX_STALLED)
		puts("robust = PTHREAD_MUTEX_STALLED");
	else if (robust == PTHREAD_MUTEX_ROBUST)
		puts("robust = PTHREAD_MUTEX_ROBUST");
	else 
		puts("robust = ???");
}

int main(int argc,char *argv[]) {
	int err,shared,robust = 0,flag = 1;
	//if (argc >= 2) 
	robust = 1;
	key_t key = 0x8888;

	// 创建共享内存
	int id = shmget(key,getpagesize(),IPC_CREAT | IPC_EXCL | 0666);
	if (id < 0) PERR("shmget");

	// 挂载共享内存
	struct ticket *t = (struct ticket *)shmat(id,NULL,0);

	if (t == (void *)-1) PERR("shmat");

	t->remain = 10;

	pthread_mutexattr_t mutexattr;
	err = pthread_mutexattr_init(&mutexattr);
	if (err != 0) PPERR(err,"pthread_mutexattr_init");

	printshared(&mutexattr);
	printrobust(&mutexattr);

	shared = PTHREAD_PROCESS_SHARED;
	err = pthread_mutexattr_setpshared(&mutexattr,shared);
	if (err != 0) PPERR(err,"pthread_mutexattr_setpshared");
	
	if (robust) {
		err = pthread_mutexattr_setrobust(&mutexattr,PTHREAD_MUTEX_ROBUST);
		if (err != 0) PPERR(err,"pthread_mutexattr_setrobust");
	}

	puts("modify attribute ----------------------");
	printshared(&mutexattr);
	printrobust(&mutexattr);

	// 用attr初始化mutex
	pthread_mutex_init(&t->lock,&mutexattr);
	// 反初始化　attr
	err = pthread_mutexattr_destroy(&mutexattr);
	if (err != 0) PPERR(err,"pthread_mutexattr_destroy");
	
	err = shmdt((void *)t);
	if (err != 0) PERR("shmdt");
	return 0;
}

