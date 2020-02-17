#include <bits/stdc++.h>
#include "apue.h"
#include <fcntl.h>
 
#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
#define __RWRW (S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)


int main(int argc,char *argv[]) {
	// 首先设置	文件模式创建掩码
	// 111 111 111 所有权限为打开
	umask(0);
	// 111 111 111
	// 110 110 110
	// -----------
	// 110 110 110
	if (creat("foo",RWRWRW) < 0)
		err_sys("creat error for foo");
	//　相当于关闭了　g rw o rw
	//  111 001 001
	//  110 110 110
	//  -----------
	//  110 000 000
	umask(__RWRW);	
	if (creat("bar",RWRWRW) < 0)
		err_sys("creat error for bar");
    return 0;
}
