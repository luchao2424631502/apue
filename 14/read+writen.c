/*************************************************************************
  > File Name: a.c
  > Author: llc
  > Mail: 18271496687@163.com 
  > Created Time: 2020年03月25日 星期三 08时03分04秒
 ************************************************************************/

#include <stdio.h> 
#include <unistd.h>

ssize_t
readn(int fd,void *ptr,size_t n)
{
	size_t  nleft;
	ssize_t nread;
	
	nleft = n;
	while (nleft > 0)
	{
		if (nread = read(fd,ptr,nleft) < 0)
		{
			// 刚开始读就出错
			if (nleft == n)
				return -1;
			// 读了部分
			else
				break;
		}
		else if (nread == 0)
		{
			// EOF
			break;
		}
		nleft -= nread;
		ptr += nread;
	}
	return (n - nleft);
}

ssize_t 
writen(int fd,const void *ptr,size_t n)
{
	size_t nleft;
	ssize_t nwritten;

	nleft = n;
	while(nleft > 0)
	{
		if ((nwritten = write(fd,ptr,nleft)) < 0)
		{
			if (nleft == n)
				return -1;
			else
				break;
		}
		else if (nwritten == 0)
		{
			break;
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return (n - nleft);
}

int main(int argc,char *argv[]) 
{ 
	return 0;
}
