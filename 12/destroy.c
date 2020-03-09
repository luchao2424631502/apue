#include "init.h"

#define PERR(msg) do { perror(msg); exit(-1); } while(0)
#define PPERR(err,msg) do { err=errno; perror(msg); exit(-1); } while(0)

struct ticket {
	int remain;
	pthread_mutex_t lock;
};
 
int main(int argc,char *argv[]) {
	int err;
	key_t key = 0x8888;
	int id = shmget(key,0,0);
	if (id < 0) PERR("shmget");

	struct ticket *t = (struct ticket*)shmat(id,NULL,0);

	if (t == (void *)-1) PERR("shmat");

	err = pthread_mutex_destroy(&t->lock);
//	if (err != 0) PPERR(err,"pthread_mutex_destroy");

	err = shmdt((void *)t);
	if (err != 0) PERR("shmdt");

	err = shmctl(id,IPC_RMID,NULL);
	if (err != 0) PERR("shmctl");
	return 0;
}
