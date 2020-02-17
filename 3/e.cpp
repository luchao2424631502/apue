/*************************************************************************
    > File Name: e.cpp
  > Author: luchao
  > Mail: 18271496687@163.com 
  > Created Time: 2020年01月22日 星期三 14时41分03秒
 ************************************************************************/

#include <bits/stdc++.h>
#include "apue.h"
#include <fcntl.h>
 
int main (int argc,char *argv[]) {
//	std::for_each(argv,argv + argc,[](const char *ch){
//			std::cout << ch << "\n";
//			});
	int val;
	if (argc != 2)
		err_quit("usage: a.out <descriptor#>");
	// 读取数字argv[1],表示文件描述符
	if ((val = fcntl(atoi(argv[1]),F_GETFL,1)) < 0)
		err_sys("fcntl error for fd %d",atoi(argv[1]));

	std::map<int,std::string> stat {
		{O_RDONLY,std::string("read only")},
		{O_WRONLY,std::string("write only")},
		{O_RDWR,std::string("read write")},
		{0x3f3f3f3f,std::string("unknown access mode")}
	};

	for(const auto& i : stat) {
		// 用O_ACCMODE 取得访问方式位,再来比较
		if (std::get<0>(i) == (val & O_ACCMODE)) {
			std::cout << std::get<1>(i); 
			break;
		}
		if (std::get<0>(i) == 0x3f3f3f3f) {
			err_dump(std::get<1>(i).c_str());
			break;
		}
	}
	//上面是互斥属性,下面的属性不互斥
	if (val & O_APPEND)
		std::cout << ", append";
	if (val & O_NONBLOCK)
		std::cout << ", nonblocking";
	if (val & O_SYNC)
		std::cout << ", synchronous writes";
#if !defined(__POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
	if (val & O_FSYNC)
		std::cout << ", synchronous writes";
#endif 
	std::cout << "\n";	
    return 0;
}
