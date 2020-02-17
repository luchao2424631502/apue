#include <bits/stdc++.h>
#include "apue.h"
 
int main(int argc,char *argv[]) {
	struct stat buf;
	std::string ans;
	for(int i = 1; i<argc; i++) {
		std::cout << argv[i] << " ";
		// 关键函数,lstat(),类似stat,
		// 传入pathname,和指向文件信息的指针
		if (lstat(argv[1],&buf) < 0) {
			err_ret("lstat error");
			continue;
		}	
		std::cout << "gid " << buf.st_gid << " uid " << buf.st_uid << " ";
		// regular file,普通文件
		if (S_ISREG(buf.st_mode))
			ans = "regular";
		else if (S_ISDIR(buf.st_mode))
			ans = "directory";
		else if (S_ISCHR(buf.st_mode))
			ans = "character special";
		else if (S_ISBLK(buf.st_mode))
			ans = "block special";
		else if (S_ISFIFO(buf.st_mode))
			ans = "fifo";
		else if (S_ISLNK(buf.st_mode))
			ans = "symbolic link";
		else if (S_ISSOCK(buf.st_mode))
			ans = "socket";
		else
			ans = "unknown mode";
		std::cout << ans << "\n";
	}
    return 0;
}
