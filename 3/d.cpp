/*************************************************************************
    > File Name: d.cpp
  > Author: luchao
  > Mail: 18271496687@163.com 
  > Created Time: 2020年01月22日 星期三 10时42分55秒
 ************************************************************************/

#include <bits/stdc++.h>
#include "apue.h"
#include <unistd.h>
#include <fcntl.h>

#define BUFFSIZE 4 

bool err(int flag) {
	if (flag == -1)
		err_sys("error: ");
}

int main () {
	std::string path("/home/luchao/Documents/Apue/3/1.in");	
	int fd = open(path.c_str(),O_RDWR|O_SYNC);
	err(fd);
	// 复制现有文件描述符,那么共用一个文件表项
	int fd_back = dup(fd);
	err(fd_back);
	std::cout << "fd= " << fd << " fd_back= " << fd_back << "\n";
	
	// 
	sync();

	std::string content("new my content");
	int sync_flag = fdatasync(fd_back);
	err(sync_flag);
	if (write(fd_back,content.c_str(),content.length()) == -1)
		err_sys("write error");
	return 0;
}
