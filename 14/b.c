/*************************************************************************
    > File Name: b.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月24日 星期二 16时07分52秒
 ************************************************************************/

#include "apue.h"
#include <ctype.h>
#include <fcntl.h>
#include <aio.h>
#include <errno.h>

#define BSZ 4096
#define NBUF 8

enum rwop {
	UNUSED = 0,
	READ_PENDING = 1,
	WRITE_PENDING = 2
};
 
struct buf {
	enum rwop op;
	int last;
	struct aiocb aiocb;
	unsigned char data[BSZ];
};

struct buf bufs[NBUF];

unsigned char
translate(unsigned char c)
{
	return '1';
}

int main(int argc,char *argv[]) 
{
	int ifd,ofd,i,j,n,err,numop;
	// 读入文件的信息
	struct stat sbuf;
	// aio块　指针列表(表示当前任务)
	const struct aiocb *aiolist[NBUF];
	off_t off = 0;

	if (argc != 3)
		err_quit("usage: rotl3 infile outfile");
	if ((ifd = open(argv[1],O_RDONLY)) < 0)
		err_sys("can't open %s",argv[1]);
	if ((ofd = open(argv[2],O_RDWR|O_CREAT|O_TRUNC)) < 0)
		err_sys("can't create %s",argv[2]);
	// 获得读入文件信息
	if (fstat(ifd,&sbuf) < 0)
		err_sys("fstat failed");

	for (i=0; i<NBUF; i++)
	{
		// 当前任务的类型
		bufs[i].op = UNUSED;
		// aio异步任务的缓冲块
		bufs[i].aiocb.aio_buf = bufs[i].data;
		// aio异步任务　回调类型
		bufs[i].aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
		// 当前工作任务列表
		aiolist[i] = NULL;
	}

	numop = 0;
	for (;;)
	{
		// 扫描8个aio任务
		for (i=0; i<NBUF; ++i)
		{
			// aio接口实现异步IO　以及　异步IO状态转换
			switch(bufs[i].op)
			{
			// 当前任务块没被使用
			case UNUSED:
				// 总偏移量还是小于文件总长度
				if (off < sbuf.st_size) 
				{
					//状态转为read
					bufs[i].op = READ_PENDING;
					// aio块操作的文件描述符
					bufs[i].aiocb.aio_fildes = ifd;
					// aio块 io开始的偏移量 = 当前总偏移量
					bufs[i].aiocb.aio_offset = off;
					// 总偏移量 += 此次任务使用的偏移量
					off += BSZ;
					// 判断文件是否读完
					if (off >= sbuf.st_size)
						// 标记为最后一个读任务
						bufs[i].last = 1;
					// 读的字节数
					bufs[i].aiocb.aio_nbytes = BSZ;
					// 提交　异步 read
					if (aio_read(&bufs[i].aiocb) < 0)
						err_sys("aio_read failed");
					// 在异步任务列表中标记
					aiolist[i] = &bufs[i].aiocb;
					// 计数
					numop++;
				}
				break;
			// 当前任务块　处于read状态
			case READ_PENDING:
				// 当前　异步read状态　处于进行中
				if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS)
					continue;
				// 
				if (err != 0)
				{
					// aio_error调用错误
					if (err == -1)
						err_sys("aio_error failed");
					// 异步read失败,err是错误代码
					else
						err_exit(err,"read failed");
				}
				// 当前异步read完成,aio_return返回read返回值,得到读取字节数
				if ((n = aio_return(&bufs[i].aiocb)) < 0)
					err_sys("aio_return failed");
				// 读取字节数不是bsize 并且也不是最后一个,(说明只读取了部分)
				if (n != BSZ && !bufs[i].last) 
					err_quit("short read (%d/%d)",n,BSZ);
				// translate
				for (j=0; j<n; j++)
					bufs[i].data[j] = translate(bufs[i].data[j]);
				// 提交异步写请求
				bufs[i].op = WRITE_PENDING;
				bufs[i].aiocb.aio_fildes = ofd;
				// 写请求要求的字节数
				bufs[i].aiocb.aio_nbytes = n;
				// 提交异步写请求
				if (aio_write(&bufs[i].aiocb) < 0)
					err_sys("aio_write failed");
				break;
			//当前任务块　处于write队列
			case WRITE_PENDING:
				//写请求仍在处理
				if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS)
					continue;
				if (err != 0)
				{
					// aio_error调用失败
					if (err == -1)
						err_sys("aio_error failed");
					else
						err_exit(err,"write failed");
				}

				if ((n = aio_return(&bufs[i].aiocb)) < 0)
					err_sys("aio_return failed");
				if (n != bufs[i].aiocb.aio_nbytes)
					err_quit("short write (%d/%d)",n,BSZ);
				//　从任务列表中取消
				aiolist[i] = NULL;
				bufs[i].op = UNUSED;
				numop--;
				break;
			}
		}
		// numop=0代表异步读执行完毕且异步写执行完毕
		if (numop == 0)
		{
			// 读写完成
			if (off >= sbuf.st_size)
				//结束无限循环
				break;
		}
		// 读大于写,那么下一轮继续读,但是先要等待此轮的异步写回调,否则下一轮用不了
		else
		{
			// 等待所有异步IO回调
			if (aio_suspend(aiolist,NBUF,NULL) < 0)
				err_sys("aio_suspend failed");
		}
	}
	// 同步所有在ofd上的异步操作,数据写入磁盘(持久化)
	bufs[0].aiocb.aio_fildes = ofd;
	if (aio_fsync(O_SYNC,&bufs[0].aiocb) < 0)
		err_sys("aio_fsync failed");
    return 0;
}
