/*************************************************************************
    > File Name: g.cpp
  > Author: luchao
  > Mail: 18271496687@163.com 
  > Created Time: 2020年01月23日 星期四 10时26分20秒
 ************************************************************************/

#include <bits/stdc++.h>
#include "apue.h"
#include <fcntl.h>
#define BUFFSIZE 4096

void
set_fl(int fd,int flags)
{
	int val;
	
	if ((val = fcntl(fd,F_GETFL,0)) < 0)
		err_sys("fcntl F_GETFL error");
	
	// 无论开了没有都打开
	val |= flags;

	if (fcntl(fd,F_SETFL,val) < 0)
		err_sys("fcntl F_SETFL error");
}

int main () {
	//fd=标准输出文件描述符, 开启同步写的标志,
	set_fl(STDOUT_FILENO,O_SYNC);
	int n;
	char buf[BUFFSIZE];
	
	while((n = read(STDIN_FILENO,buf,BUFFSIZE)) > 0) {
		if (write(STDOUT_FILENO,buf,n) != n)
			err_sys("write wrror");
	}
	
	// 判断最后一次read多少
	if (n < 0)
		err_sys("read error");
    return 0;
}
