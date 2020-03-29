/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月29日 星期日 09时17分47秒
 ************************************************************************/

#include "apue.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

// childpid[fd] = pid, 第几个描述符对应的进程
static pid_t *childpid = NULL;

static int maxfd;

// cmd , 读写模式
FILE *popen(const char *cmdstring,const char *type)
{
	int i;
	int pfd[2];
	pid_t pid;
	FILE *fp;

	// 检测type是否是规定值
	if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0)
	{
		errno = EINVAL;
		return NULL;
	}

	// 全局变量,多次调用popen时
	if (childpid == NULL)
	{
		// apue　函数,
		maxfd = open_max();
		if ((childpid = calloc(maxfd,sizeof(pid_t))) == NULL)
			return NULL;
	}

	// 创建管道
	if (pipe(pfd) < 0)
		return NULL;
	if (pfd[0] >= maxfd || pfd[1] >= maxfd)
	{
		close(pfd[0]);
		close(pfd[1]);
		errno =  EMFILE;
		return NULL;
	}

	if ((pid = fork()) < 0)
		return NULL;
	// 子进程
	else if (pid == 0)
	{
		if (*type == 'r')
		{
			// 父进程要读,子进程写,
			close(pfd[0]);
			if (pfd[1] != STDOUT_FILENO)
			{
				dup2(pfd[1],STDOUT_FILENO);
				// 把stdou_fileno替换成自己的描述符
				close(pfd[1]);
			}
		} 
		else
		{
			// 父进程写,子进程读
			close(pfd[1]);
			if (pfd[0] != STDIN_FILENO)
			{
				dup2(pfd[0],STDIN_FILENO);
				close(pfd[0]);
			}
		}

		// 关闭所有进程的描述符
		for (i=0; i<maxfd; i++)
			if (childpid[i] > 0)
				close(i);

		execl("/bin/sh","sh","-c",cmdstring,(char *)0);
		_exit(127);
	}

	if (*type == 'r')
	{
		close(pfd[1]);
		// 在描述符上使用FILE*
		if ((fp = fdopen(pfd[0],type)) == NULL)
			return NULL;
	}
	else
	{
		close(pfd[0]);
		if ((fp = fdopen(pfd[1],type)) == NULL)
			return NULL;
	}
	
	//记住这个fd代表的子进程 (fileno获得FILE*的文件描述符
	childpid[fileno(fp)] = pid;
	return fp;
}

int pclose(FILE *fp)
{
	int fd,stat;
	pid_t pid;

	// popen没有被调用
	if (childpid == NULL)
	{
		errno = EINVAL;
		return -1;
	}

	fd = fileno(fp);
	// 无效文件描述符
	if (fd >= maxfd)
	{	
		errno = EINVAL;
		return -1;
	}
	// fp不是被popen打开的
	if ((pid = childpid[fd]) == 0) 
	{
		errno = EINVAL;
		return -1;
	}

	// 清除该位
	childpid[fd] = 0;
	if (fclose(fp) == EOF)
		return -1;

	// 回收子进程
	while(waitpid(pid,&stat,0) < 0)
	{
		if (errno != EINTR)
			return -1;
	}

	return stat; // 返回子进程终止状态
}
 
int main(int argc,char *argv[]) 
{ 
    return 0;
}
