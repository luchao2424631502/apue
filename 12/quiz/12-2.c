#include "init.h"

extern char **environ;
pthread_mutex_t lock;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void thread_init(void) {
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	// 设置attr为递归锁
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);

	// 初始化mutex
	pthread_mutex_init(&lock,&attr);

	// 反初始化mutexattr
	pthread_mutexattr_destroy(&attr);
}
 
// 线程安全,异步信号安全
int putenv_r(char *string) {
	char *ans = strchr(string,'=');
	if (ans == NULL) 
		err_exit(-1,"strchr error");
	int len = ans - string;
	// 多线程环境下 只进行一次初始化
	pthread_once(&init_done,thread_init);
	// 使用recursive锁保证异步信号安全,得到锁,但不进入临界区
	pthread_mutex_lock(&lock);
	int i=0;
	for (i=0; environ[i] != NULL ; ++i) {
		if (strncmp(string,environ[i],len) == 0) {
			// 找到了,就覆盖
			environ[i] = string;
			pthread_mutex_unlock(&lock);
			return 0;
		}
	}
	// 最后面添加
	environ[i] = string;
	pthread_mutex_unlock(&lock);
	return 0;
}

int main(int argc,char *argv[]) {
	const char *string = "a=b";
	const char *ans = "a";
	char *ptr = malloc(sizeof(char) * strlen(string) + 1);
	if (ptr == NULL) 
		err_exit(-1,"main error");
	char *ptr;
	strcpy(ptr,string);
	putenv_r(ptr);
	char *value = getenv(ans);
	if (value == NULL) err_exit(-1,"getenv error");
	printf("%s\n",value);
	free(ptr);
    exit(0);
}
