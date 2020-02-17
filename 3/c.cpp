/*************************************************************************
    > File Name: c.cpp
  > Author: luchao
  > Mail: 18271496687@163.com 
  > Created Time: 2020年01月22日 星期三 09时52分41秒
 ************************************************************************/

#include <bits/stdc++.h>
#include "apue.h"
#include <unistd.h>
#include <fcntl.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";


int main () {
	int fd;
	if ((fd = creat("file.hole",FILE_MODE)) < 0)
		err_sys("creat error");
	if (write(fd,buf1,10) != 10)
		err_sys("buf1 write error");

	if (lseek(fd,16384,SEEK_SET) == -1)
		err_sys("lseek error");

	if (write(fd,buf2,10) != 10)
		err_sys("buf2 write errro");
    return 0;
}

