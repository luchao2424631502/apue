/*************************************************************************
    > File Name: b.cpp
  > Author: luchao
  > Mail: 18271496687@163.com 
  > Created Time: 2020年01月22日 星期三 09时33分51秒
 ************************************************************************/

#include <bits/stdc++.h>
#include "apue.h"
#include <unistd.h>
#include <fcntl.h>
const int SIZE = 1024;

int main () {
	const char *path = "/home/luchao/Documents/Apue/3/1.in";
	std::cout << STDIN_FILENO << " " << STDOUT_FILENO;
//	int fd = open(path,O_RDONLY);
//	// error
//	if (-1 == fd)
//		err_sys("打开文件错误");
//	char content[SIZE];
//	int read_size = read(fd,content,SIZE);
//	std::cout << "readsize: " << read_size << " \ncontent:\n" << content;
//	close(fd);
    return 0;
}
