/*************************************************************************
    > File Name: h.cpp
  > Author: luchao
  > Mail: 18271496687@163.com 
  > Created Time: 2020年01月23日 星期四 16时56分55秒
 ************************************************************************/

#include <bits/stdc++.h>
#include "apue.h"
#include <unistd.h>
#include <fcntl.h>

const int maxn = 1024;
char ReadBuf[maxn]; 
 
int main () {
	std::string path("/home/luchao/Documents/Apue/3/1.out");
	int fd = open(path.c_str(),O_APPEND|O_RDWR);
	std::string content = "abc";
	if (fd == -1)	
		err_sys("open failed");
	int offset = lseek(fd,0,SEEK_END);
	if (offset == -1)
		err_sys("lseek error");

	int ans = read(fd,ReadBuf,maxn);
//	int ans = write(fd,content.c_str(),content.size());
	if (ans == -1)
		err_sys("read error");
	std::cout << "\nread content\n" << ReadBuf;
    return 0;
}
