#include "apue.h"
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include "path_alloc.h"

// 给函数声明类型
typedef int Myfunc(const char *,const struct stat *,int );

static Myfunc myfunc;
static int myftw(char *,Myfunc *);
static int dopath(Myfunc *);
static long nreg,ndir,nblk,nchr,nfifo,nslink,nsock,ntot;



int main(int argc,char *argv[]) {
	if (argc != 2)
		err_quit("usage: ftw <starting-pathname>");
	//　整个文件主要是,首先 myftw这个函数
	// 没错ret就是0,否则ret会返回具体的错误code
	int ret = myftw(argv[1],myfunc);
	ntot = (nreg + ndir + nblk + nchr + nfifo + nslink + nsock);
	if (ntot == 0)
		ntot = 1;

	// 打印文件信息
	printf("regular files= %10ld,%5.2f %%\n",nreg,nreg*100.0/ntot);
	printf("directories= %10ld,%5.2f %%\n",ndir,ndir*100.0/ntot);
	printf("block special= %10ld,%5.2f %%\n",nblk,nblk*100.0/ntot);
	printf("char special= %10ld,%5.2f %%\n",nchr,nchr*100.0/ntot);
	printf("FIFOS = %10ld,%5.2f %%\n",nfifo,nfifo*100.0/ntot);
	printf("symbolic links= %10ld,%5.2f %%\n",nslink,nslink*100.0/ntot);
	printf("sockets	  = %10ld,%5.2f %%\n",nsock,nsock*100.0/ntot);

	return (ret);
}

#define FTW_F 1
#define FTW_D 2
#define FTW_DNR 3
#define FTW_NS 4

static char *fullpath;
static size_t pathlen;

//　形参是函数指针,传递Myfunc函数
static int myftw(char *pathname,Myfunc *func) {
	// 分配PATH_MAX + 1 个字节的内存,然后保存在 pathlen中
	fullpath = (char *)path_alloc(&pathlen);
	
	// 默认分配长度小于 strlen pathname
	if (pathlen <= strlen(pathname)) {
		// 新长度
		pathlen = strlen(pathname) * 2;
		// 如果重新分配失败
		if ((fullpath = (char *)realloc(fullpath,pathlen)) == NULL)
			err_sys("realloc failed");
	}
	
	// 将地址pathname复制到fullpath
	strcpy(fullpath,pathname);
	return (dopath(func)); 
}
// 上面函数主要作用就是分配内存来存放fullpath
// 调用 dopath();函数

static int dopath(Myfunc *func) {
	// 文件信息
	struct stat statbuf;
	// 目录的具体信息
	struct dirent *dirp;
	// 打开目录后返回值
	DIR			*dp;
	int ret,n;
	// lstat 避免得到符号链接指向的文件信息
	if (lstat(fullpath,&statbuf) < 0)
		// type = FTW_NS 读取文件状态失败,得到错误返回值
		return (func(fullpath,&statbuf,FTW_NS));
	// fullpath 目录项不是目录
	if (S_ISDIR(statbuf.st_mode) == 0)
		// 统计此文件信息.因为不是目录
		return (func(fullpath,&statbuf,FTW_F));
	// 当做目录交给func检查,结果ret != 0 出错
	if ((ret = func(fullpath,&statbuf,FTW_D)) != 0)	
		// error code ret
		return (ret);

	n = strlen(fullpath);
	// 扩展路径缓冲
	if (n + NAME_MAX + 2 > pathlen) {
		pathlen *= 2;
		//　重新分配内存,判断是否成功,扩大内存,原区域内存内容不变
		if ((fullpath = (char *)realloc(fullpath,pathlen)) == NULL)
			err_sys("realloc failed");
	}
	// if /home n=5 /home/0 n=6
	fullpath[n++] = '/';
	fullpath[n]   = 0;
	// 打开目录,
	if ((dp = opendir(fullpath)) == NULL)
		//　func检测目录不能读的error code
		return (func(fullpath,&statbuf,FTW_DNR));
	// 读取DIR中的每个目录项
	while((dirp = readdir(dp)) != NULL) {
		// 当前目录和父目录不显示
		if (strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..") == 0)
			continue;
		//　复制到原来的path结尾处,/home/adcb n=6,子目录项也是目录
		strcpy(&fullpath[n],dirp->d_name);
		// 递归调用,
		if ((ret = dopath(func)) != 0)
			break;
	}
	// 消除 '/'
	fullpath[n-1] = 0;
	// DIR作为参数
	if (closedir(dp) < 0)	
		err_ret("can't close directory %s",fullpath);
	// 返回error code
	return (ret);
}

// dopath 函数　return 时调用 Myfunc函数

static int 
myfunc(const char *pathname,const struct stat *statptr,int type) {
	// 判断传入的type是什么文件类型
	switch(type) {
		// 普通文件
		case FTW_F:
			// 求出普通文件的类型
			switch(statptr->st_mode & S_IFMT) {
				case S_IFREG: nreg++; break;
				case S_IFBLK: nblk++; break;
				case S_IFCHR: nchr++; break;
				case S_IFIFO: nfifo++;break;
				case S_IFLNK: nslink++; break;
				case S_IFSOCK: nsock++; break;
				case S_IFDIR:
					err_dump("for S_IFDIR for %s",pathname);
			}
			break;
		// 目录
		case FTW_D:
			ndir++;
			break;
		// 无法读取的目录
		case FTW_DNR:
			err_ret("can't read directory %s",pathname);
			break;
		// 无法读取状态
		case FTW_NS:
			// 出错情况打印一下
			err_ret("stat error for %s",pathname);
			break;
		default:
			err_dump("unknown type %d for pathname %s",type,pathname);
	}
	return (0);
}

