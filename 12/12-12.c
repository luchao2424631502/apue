#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

extern char **environ;

pthread_mutex_t env_mutex;

static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void
thread_init(void) {
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&env_mutex,&attr);
	pthread_mutexattr_destroy(&attr);
}

int 
getenv_r(const char *name,char *buf,int buflen) {
	int i,len,olen;

	//多线程环境下,此函数只执行一次
	pthread_once(&init_done,thread_init);
	len = strlen(name);
	pthread_mutex_lock(&env_mutex);
	for (i=0; environ[i] != NULL; i++) {
		if ((strncmp(name,environ[i],len) == 0) &&
				(environ[i][len] == '=')) {
			olen = strlen(&environ[i][len+1]);
			// 配置长度大于缓冲区长度 返回错误
			if (olen >= buflen) {
				pthread_mutex_unlock(&env_mutex);
				return ENOSPC;
			}
			strcpy(buf,&environ[i][len+1]);
			pthread_mutex_unlock(&env_mutex);
			return 0;
		}
	}
	pthread_mutex_unlock(&env_mutex);
	return ENOENT;
}

//char *
//getenv(const char *name) {
//	int i,len;
//	len = strlen(name);
//	for (i = 0; environ[i] != NULL; i++) {
//		if ((strncmp(name,environ[i],len) == 0) &&
//				(environ[i][len] == '=')) {
//			strncpy(envbuf,&environ[i][len+1],MAXSTRINGSZ-1);
//			return envbuf;
//		}
//	}
//	return NULL;
//}

void func() {
	for (int i=0; environ[i] != NULL; i++) 
		printf("%s\n",environ[i]);
}

int main(int argc,char *argv[]) {
	func();
    exit(0);
}
