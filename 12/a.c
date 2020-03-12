#include "init.h"

extern char **environ;
pthread_mutex_t mutex;
static pthread_once_t init_node = PTHREAD_ONCE_INIT;

static void thread_init(void) {
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	// 设置attr为递归锁
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);

	// 初始化mutex
	pthread_mutex_init(&mutex,&attr);

	// 反初始化mutexattr
	pthread_mutexattr_destroy(&attr);
}
 
int putenv_r(char *string) {

}

int main(int argc,char *argv[]) {
 
    exit(0);
}
