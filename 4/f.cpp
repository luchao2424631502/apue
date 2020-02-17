#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>
#include "apue.h"

int main(int argc,char *argv[]) {
	std::string path = "/home/luchao/Documents/Apue/4/1.in";
	umask(0);
	int fd = open(path.c_str(),O_CREAT|O_RDWR,0644);
	if (fd == -1)
		err_sys("open errro");	
	std::string content("this is write content");
	if (write(fd,content.c_str(),content.size()) != content.size()) 
		err_sys("write error");
	// 重新定位形成空洞
	int offset = lseek(fd,1024,SEEK_SET);
	if (offset != 1024)
		err_sys("offset error");
	content = "other content";
	if (write(fd,content.c_str(),content.size()) != content.size())
		err_sys("write second error");

	close(fd);
	return 0;
}
