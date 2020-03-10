#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXSTRINGSZ 4096

// 内部链接,静态存储时期,文件作用域
static pthread_key_t key;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;
pthread_mutex_t env_mutex = PTHREAD_MUTEX_INITIALIZER;

extern char **environ;

// 多线程环境下只执行一次的函数
static void
thread_init(void) { pthread_key_create(&key,free); }

// 线程安全,且数据放在,线程私有数据中
char *
getenv(const char *name) {
	printf("%s\n",name);
	int i,len;
	char *envbuf;

	// 执行一次 此函数
	pthread_once(&init_done,thread_init);
	pthread_mutex_lock(&env_mutex);
	
	// 传入 键 后, 返回线程私有数据地址
	envbuf = (char *)pthread_getspecific(key);
	// 为空,则没有设置,
	if (envbuf == NULL) {
		envbuf = malloc(MAXSTRINGSZ);
		// 申请内存失败
		if (envbuf == NULL) {
			pthread_mutex_unlock(&env_mutex);
			return "error";
		}
		// 申请 成功 的话
		pthread_setspecific(key,envbuf);
	}
	len = strlen(name);
	for (i=0; environ[i] != NULL; i++) {
		if ((strncmp(name,environ[i],len) == 0) && 
				(environ[i][len] == '=')) {
			strncpy(envbuf,&environ[i][len+1],MAXSTRINGSZ);
			// 忘记释放锁了
			pthread_mutex_unlock(&env_mutex);
			return envbuf;
		}
	}
	pthread_mutex_unlock(&env_mutex);
	return "error";
}
 
void *
thread_test(void *arg) {
	char * err;
	printf("main getenv _ %s\n",getenv("HOME"));
	return (void *)0;
}

// 当主线程调用getenv时,其他线程再次调用getenv,不会在执行Init函数了 
int main(int argc,char *argv[]) {
	pthread_t tid;
	pthread_create(&tid,NULL,thread_test,NULL);
	printf("main getenv _ %s\n",getenv("_"));
	pthread_join(tid,NULL);
    exit(0);
}
