#include "apue.h" 
#include <fcntl.h>
 
int main(int argc,char *argv[]) {
	char *path = "/home/luchao/Documents/Apue/10/1.in";
	int fd;
	pid_t pid;
	if ((fd = open(path,O_CREAT|O_RDWR)) == -1)
		err_sys("open error");
	TELL_WAIT();
	int all = 0;

	// all各自有一份,缓冲区大小和for循环测试可以自己设置,主要是同步如何解决?
	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0) {
		all = 1;
		char buf[5];
		for (int i=0; i<10; i++) {
			// 等待父进程
			WAIT_PARENT();		

			sprintf(buf,"%d\n",all);
			printf("子进程写 %d\n",all);
			all += 2;			// 等待父进程
			write(fd,buf,5);
			
			// 子进程write执行完后,通知父进程
			TELL_PARENT(getppid());
		}
	} else {
		char buf[5];
		for (int i=0; i<10; i++) {
			sprintf(buf,"%d\n",all);
			printf("父进程写 %d\n",all);
			write(fd,buf,5);
			all += 2;
			
			// 父进程执行完后,通知子进程执行,但是自己不能继续执行
			// 所以还要等待子进程执行完毕后通知自己,
			TELL_CHILD(pid);
			WAIT_CHILD();	
		}
	}
    exit(0);
}
