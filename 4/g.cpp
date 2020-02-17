#include <bits/stdc++.h>
#include "apue.h"
#include <fcntl.h>
#include <unistd.h>
 
const int maxn = 1024;

int main(int argc,char *argv[]) {
	std::string path("/home/luchao/Documents/Apue/4/1.in");
	int fd = open(path.c_str(),O_RDONLY);
	char buf[maxn];
	int readsize = 0;
	while (readsize = read(fd,buf,maxn)) {
		for (auto i : buf)
			if(i == 0) std::cout << i << "\n";
		std::cout << "readsize " << readsize << "\n";
		std::cout << buf;
	}
    return 0;
}
