#include "apue.h" 
#include <pthread.h>

struct foo { int a,b,c,d; };
 
void 
printfoo(const char *s,const struct foo *fp)
{
	printf("%s structure at 0x%lx\n",s,(unsigned long)fp);
	printf(" foo.a = %d\n",fp->a);
	printf(" foo.b = %d\n",fp->b);
	printf(" foo.c = %d\n",fp->c);
	printf(" foo.d = %d\n",fp->d);
}

void *
thr_fn1(void *arg)
{
	struct foo foo = {1,2,3,4};
	printfoo("thread 1:\n",&foo);
	pthread_exit((void *)&foo);
}

void *
thr_fn2(void *arg)
{
	printf("thread 2: ID is %lu\n",(unsigned long)pthread_self());
	pthread_exit((void *) 0);
}

int main(int argc,char *argv[]) {
	int err;
	pthread_t tid1,tid2;
	// 指向thread1返回的地址,但该地址是在线程的栈上
	struct foo *fp;

	//创建默认属性线程
	pthread_create(&tid1,NULL,thr_fn1,NULL);
	pthread_join(tid1,(void *)&fp);
	
	// 挂起调用者1s,等待线程执行
	sleep(1);

	// 调用一个线程,结束后看看栈有什么变化没
	pthread_create(&tid2,NULL,thr_fn2,NULL);
	sleep(1);
	printfoo("parent:\n",fp);
    exit(0);
}
