#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct memstream {
	char *buf;		// 内存缓冲区
	size_t rsize;	// buffer的真正大小 
	size_t vsize;   // 虚拟大小
	size_t curpos;  // 当前在缓冲区的大小
	int flags;		
};

#define MS_READ 0x01
#define MS_WRITE 0x02
#define MS_APPEND 0x04
#define MS_TRUNCATE 0x08
#define MS_MYBUF 0x10 // 16
 
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b)) 
#endif

static int mstream_read(void *,char *,int);
static int mstream_write(void *,const char *,int);
static int mstream_seek(void *,int,int);
static int mstream_close(void *);
static int type_to_flags(const char *__restrict type);
static off_t find_end(char *buf,size_t len);

// 接受参数 buf,size,type
FILE * fmemopen(void *__restrict buf,size_t size,
		const char *__restrict type) {
	// 内存流　结构体
	struct memstream *ms;
	FILE *fp;

	// 不接受空的缓冲区
	if (size == 0) {
		errno = EINVAL;
		return NULL;
	}	
	if ((ms = malloc(sizeof(struct memstream))) == NULL) {
		errno = ENOMEM;
		return NULL;
	}
	// 将文件打开类型　转化成　flags
	if ((ms->flags = type_to_flags(type)) == 0) {
		errno = EINVAL;
		free(ms);
		return NULL;
	}
	// 手动分配缓冲区,
	if (buf == NULL) {
		// 由fmemopen分配的缓冲区,如果只读或者只写,则无意义,判断读写是否都打开,
		if ((ms->flags & (MS_READ | MS_WRITE)) !=
			(MS_READ | MS_WRITE)) {
			errno = EINVAL;
			free(ms);
			return NULL;
		}
		// 手动分配缓冲区大小
		if ((ms->buf = malloc(size)) == NULL) {
			errno = ENOMEM;
			free(ms);
			return NULL;
		}
		// 写入memstream　内存流的信息
		ms->rsize = size;
		// 打开　手动分配缓冲区标志
		ms->flags |= MS_MYBUF;
		// 缓冲区读写位置
		ms->curpos = 0;
	}
	// 用户提供缓冲区位置
	else {
		// 复制缓冲区位置
		ms->buf = buf;
		ms->rsize = size;
		if (ms->flags & MS_APPEND)
			// 找到第一个NULL字节地址
			ms->curpos = find_end(ms->buf,ms->rsize);
		else
			ms->curpos = 0;
		
	}
	// vsize的大小不受缓冲区又谁分配影响
	if (ms->flags & MS_APPEND) // a
		ms->vsize = ms->curpos;
	else if (ms->flags & MS_TRUNCATE) // w
		ms->vsize = 0;
	else // r
		ms->vsize = size;

	// freebsd系统系统此函数对一个流进行操作
	// fp = funopen(ms,mstream_read,mstream_write,mstream_seek,mstream_close);
	
	// 得到流信息错误
	if (fp == NULL) {
		if (ms->flags & MS_MYBUF)
			free(ms->buf);
		free(ms);
	}
	return fp;
}

// const char * 转换
static int type_to_flags(const char *__restrict type) {
	const char *cp;
	int flags = 0;
	for (cp = type; *cp!=0; cp++) {
		switch(*cp) {
			case 'r':
				if (flags != 0)
					return 0;
				flags |= MS_READ;
				break;
			case 'w':
				if (flags != 0)
					return 0;
				flags |= MS_WRITE|MS_TRUNCATE;
				break;
			case 'a':
				if (flags != 0)
					return 0;
				flags |= MS_APPEND;
				break;
			case '+':
				if (flags != 0)
					return 0;
				flags |= MS_READ|MS_WRITE;
				break;
			case 'b':
				if (flags != 0)
					return 0;
				// nothing
				break;
			default:
				return 0;
		}
	}
	return flags;
}

static off_t find_end(char *buf,size_t len) {
	off_t off = 0;
	while(off < len) {
		if (buf[off] == 0)
			break;
		off++;
	}
	return off;
}

// -1出错,返回读到的数量
static int mstream_read(void *cookie,char *buf,int len) {
	// 指向内存结构的指针
	struct memstream *ms = cookie;
	
	// read 位没有打开
	if (!(ms->flags & MS_READ)) {
		errno = EBADF;
		return -1;
	}

	if (ms->curpos >= ms->vsize)
		return 0;

	// 只读长度　和 len 2选一
	int nr = MIN(len,ms->vsize - ms->curpos);
	memcpy(buf,ms->buf + ms->curpos,nr);
	ms->curpos += nr;
	return nr;
}

//
static int mstream_write(void *cookie,const char *buf,int len) {
	// 复制内存流信息
	struct memstream *ms = cookie;
	int off;

	// 如果flags中MS_APPEND和MS_WRITE都不存在则退出
	if (!(ms->flags & (MS_APPEND | MS_WRITE))) {
		errno = EBADF;
		return -1;
	}
	// 如果是接着写
	if (ms->flags & MS_APPEND)
		off = ms->vsize;
	else
		// 其他的写模式 就从curpos开始
		off = ms->curpos;
	// 剩下能写的空间　和　len
	int nw = MIN(len,ms->rsize - off);
	// 将要写入的内容　复制到　缓冲区中
	memcpy(ms->buf + off,buf,nw);
	// 更新
	ms->curpos = off + nw;	
	if (ms->curpos > ms->vsize) {
		ms->vsize = ms->curpos;
		// 读写位同时打开是 `+`
		// ms的read位和write位是否同时打开,并且读写位置还在范围内　vsize < rsize
		if ((ms->flags & (MS_READ|MS_WRITE)) == 
			(MS_READ|MS_WRITE) && (ms->vsize < ms->rsize))
			//第三条规则,增加数据量是在当前位置写入NULL字节
			*(ms->buf + ms->vsize) = 0;
	}
	// 读写位置没有同时打开
	// write和append标志同时打开前提是read位没有打开
	if ((ms->flags & (MS_WRITE|MS_APPEND)) &&
		!(ms->flags & MS_READ)) {
		// 当前位置仍在缓冲区中
		if (ms->curpos < ms->rsize)
			*(ms->buf + ms->curpos) = 0;
		else
			*(ms->buf + ms->rsize - 1) = 0;
	}
	return nw;
}

static int mstream_seek(void *cookie,int pos,int whence) {
	struct memstream *ms = cookie;
	int off;
	switch(whence) {
		case SEEK_SET:
			off = pos;
			break;
		case SEEK_END:
			off = ms->vsize + pos;
			break;
		case SEEK_CUR:
			off = ms->curpos + pos;
			break;
	}
	// off最终不在范围内
	if (off < 0 || off > ms->vsize) {
		errno = EINVAL;
		return -1;
	}
	ms->curpos = off;
	return off;
}

static int mstream_close(void *cookie) {
	struct memstream *ms = cookie;
	// 根据宏来判断,缓冲区是否需要回收
	if (ms->flags & MS_MYBUF)
		free(ms->buf);
	free(ms);
	return 0;
}

int main(int argc,char *argv[]) {
    return 0;
}
