#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
 
#define handle_error_en(en,msg)\
	do { errno = en; perror(msg); exit(EXIT_FAILURE); } while(0)

static void 
display_pthread_attr(pthread_attr_t *attr,char *prefix) 
{
	int res,i;
	size_t v;
	void *stkaddr;
	struct sched_param sp;

	res = pthread_attr_getdetachstate(attr,&i);
	if (res != 0)
		handle_error_en(res,"pthread_attr_getdetachstate");
	printf("%s Detach state = %s\n",prefix,
			(i == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
			(i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :
			"???");
	// 得到属性都传入指针,且属性一般实现为整数
	res = pthread_attr_getscope(attr,&i);
	if (res != 0)
		handle_error_en(res,"pthread_attr_getscope");
	printf("%s Scope        = %s\n",prefix,
			(i == PTHREAD_SCOPE_SYSTEM) ? "PTHREAD_SCOPE_SYSTEM" :
			(i == PTHREAD_SCOPE_PROCESS)? "PTHREAD_SCOPE_PROCESS":
			"???");
	
	res = pthread_attr_getinheritsched(attr,&i);
	if (res != 0)
		handle_error_en(res,"pthread_attr_getinheritsched");
	printf("%s Inherit scheduler = %s\n",prefix,
			(i == PTHREAD_INHERIT_SCHED) ? "PTHREAD_INHERIT_SCHED" :
			(i == PTHREAD_EXPLICIT_SCHED)? "PTHREAD_EXPLICIT_SCHED":
			"???");

	res = pthread_attr_getschedpolicy(attr,&i);
	if (res != 0)
		handle_error_en(res,"pthread_attr_getschedpolicy");
	printf("%s Scheduling policy = %s\n",prefix,
			(i == SCHED_OTHER)?"SCHED_OTHER":
			(i == SCHED_FIFO) ?"SCHED_FIFO" :
			(i == SCHED_RR)   ?"SCHED_RR":
			"???");

	res = pthread_attr_getschedparam(attr,&sp);
	if (res != 0)
		handle_error_en(res,"pthread_attr_getschedparam");
	printf("%s Scheduling priority = %d\n",prefix,
			sp.sched_priority);

	res = pthread_attr_getguardsize(attr,&v);
	if (res != 0)
		handle_error_en(res,"pthread_guard_size");
	printf("%s Guard size  = %d bytes\n",prefix,v);

	res = pthread_attr_getstack(attr,&stkaddr,&v);
	if (res != 0)
		handle_error_en(res,"pthread_attr_getstack");
	printf("%s Stack address = %p\n",prefix,stkaddr);
	printf("%s Stack size    = 0x%x bytes\n",prefix,v);
}

static void *
thread(void *arg) 
{
	int s;
	pthread_attr_t attr;
	s = pthread_getattr_np(pthread_self(),&attr);
	if (s != 0)
		handle_error_en(s,"pthread_getattr_np");
	printf("Thread attrbutes:\n");
	display_pthread_attr(&attr,"\t");

	exit(EXIT_SUCCESS);
}

int main(int argc,char *argv[]) {
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_t *attrp;
	
	int res;

	attrp = NULL;

	if (argc > 1) {
		int stack_size;
		void *sp;

		attrp = &attr;

		res = pthread_attr_init(&attr);
		if (res != 0)
			handle_error_en(res,"pthread_attr_init");

		res = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
		if (res != 0)
			handle_error_en(res,"pthread_attr_setdetachstate");

		res = pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
		if (res != 0)
			handle_error_en(res,"pthread_attr_setinheritsched");

		stack_size = strtoul(argv[1],NULL,0);

		res = posix_memalign(&sp,sysconf(_SC_PAGESIZE),stack_size);
		if (res != 0)
			handle_error_en(res,"posix_memalign");
		printf("posix_memalign() allocated at %p\n",sp);

		res = pthread_attr_setstack(&attr,sp,stack_size);
		if (res != 0)
			handle_error_en(res,"pthread_attr_setstack");
	}

	res = pthread_create(&tid,attrp,&thread,NULL);
	if (res != 0)
		handle_error_en(res,"pthread_create");
	if (attrp != NULL) {
		res = pthread_attr_destroy(attrp);
		if (res != 0)
			handle_error_en(res,"pthread_attr_destroy");
	}
	pause();
}
