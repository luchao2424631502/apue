#include <bits/stdc++.h>
#include "apue.h"
#include <fcntl.h>
 
int main(int argc,char *argv[]) {
	std::cout << "~-2 " << (~(-2)) << " ~2 " << (~2) << " ~0 " << (~0);
	return 0;
	if (argc != 2)
		err_quit("usage: a.out <pathname>");
	// 可以是相对路径,也可以是绝对路径
	if (access(argv[1],R_OK) < 0)
		err_ret("access error for %s",argv[1]);
	else
		std::cout << "read access OK\n";
	int fd;
	// 只读的形式打开
	if ((fd = open(argv[1],O_RDONLY)) < 0)
		err_ret("open error for %s",argv[1]);
	else
		std::cout << "open for reading OK\n";
	close(fd);

	if (access(argv[1],W_OK) < 0)
		err_ret("access error for %s",argv[1]);
	else
		std::cout << "write access Ok\n";
    return 0;
}
