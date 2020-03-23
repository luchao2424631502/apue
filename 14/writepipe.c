/*************************************************************************
    > File Name: writepipe.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月23日 星期一 11时51分37秒
 ************************************************************************/

#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// 向pipe中写文件
int main(int argc,char *argv[]) 
{
	if (argc < 2) 
	{
		printf("Usage: %s <fifoname>\n",argv[0]);
		return 1;
	}
	char buf[64];
	int fd = open(argv[1],O_WRONLY);
	if (fd < 0)
	{
		perror("open pipe");
		return 1;
	}
	int n;
	while(1)
	{
		n = read(STDIN_FILENO,buf,64);	
		write(fd,buf,n);
	}
    return 0;
}
