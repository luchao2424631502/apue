/*************************************************************************
    > File Name: a.cpp
  > Author: luchao
  > Mail: 18271496687@163.com 
  > Created Time: 2020年01月21日 星期二 20时44分40秒
 ************************************************************************/

#include <bits/stdc++.h>
#include "apue.h"
#include <fcntl.h>
 
int main () {
	const char *path = "/home/luchao/Documents/Apue/3/1.in";
	if (-1 == open(path,O_RDWR))
		err_sys("not exist");
	else
		std::cout << "file exist\n";
    return 0;
}

