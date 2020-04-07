/*************************************************************************
    > File Name: db.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年04月04日 星期六 23时16分02秒
 ************************************************************************/

#include "apue.h"
#include "apue_db.h"
#include <fcntl.h>
#include <stdarg.h> // 可变参数
#include <errno.h>
#include <sys/uio.h>

#define IDXLEN_SZ	4
#define SEP			':'
#define SPACE		' '
#define NEWLINE     '\n'

#define PTR_SZ		7 // 哈希链指针域的大小
#define PTR_MAX		999999
#define NHASH_DEF   137
#define FREE_OFF	0 // 索引文件中空闲列表便宜
#define HASH_OFF	PTR_SZ // 第一个hash表的指针的偏移量是空指针的大小

typedef unsigned long DBHASH; // hash 值
typedef unsigned long COUNT;  // unsigned counter

// db.c对数据库的私有表示 (DB 内部类型)
typedef struct {
	int idxfd; // index file fd
	int datfd; // data file fd
	
	char *idxbuf; // index 记录缓冲区
	char *datbuf; // 数据记录缓冲区
	
	char *name; // 打开db的名字
	off_t idxoff; //在索引文件中 索引记录的偏移量 (idxoff + ptr_sz + idxlen_sz)
	size_t idxlen; // 索引记录的长度

	off_t datoff; // 在数据文件中的数据记录偏移量
	size_t datlen; // 数据记录的长度

	off_t ptrval; // 当前记录的下一条记录的偏移地址
	off_t ptroff; //  当前记录的上一条记录的偏移地址
	off_t chainoff; // 当前记录chain头的地址
	off_t hashoff;
	DBHASH nhash; //

	COUNT cnt_delok;
	COUNT cnt_delerr;
	COUNT cnt_fetchok;
	COUNT cnt_fetcherr;
	COUNT cnt_nextrec;
	COUNT cnt_store1;
	COUNT cnt_store2;
	COUNT cnt_store3;
	COUNT cnt_store4;
	COUNT cnt_storerr;
}DB;

// 内部函数
static DB * _db_alloc(int);
static void _db_dodelete(DB *);

static int _db_find_and_lock(DB *,const char *,int);
static int _db_findfree(DB *,int,int);
static void _db_free(DB *);

static DBHASH _db_hash(DB *,const char *);

static char *_db_readdat(DB *);
static off_t _db_readidx(DB *,off_t );
static off_t _db_readptr(DB *,off_t );

static void _db_writedat(DB *,const char *,off_t,int);
static void _db_writeidx(DB *,const char *,off_t,int,off_t);
static void _db_writeptr(DB *,off_t,off_t);

DBHANDLE db_open(const char *pathname,int oflag,...)
{
	DB		*db;
	int		len,mode;
	size_t	i;
	char	asciiptr[PTR_SZ + 1],
			hash[(NHASH_DEF + 1) * PTR_SZ + 2]; // + 2 newline + \n
	struct stat statbuff;  //文件信息

	// 分配空间给DB和他需要的缓冲区
	len = strlen(pathname);
	if ((db = _db_alloc(len)) == NULL)
		err_dump("db_open: _db_alloc error for DB");
	db->nhash	= NHASH_DEF; // 哈希值为默认值
	db->hashoff = HASH_OFF;  // 索引文件中hash table的偏移量
	
	strcpy(db->name,pathname); // 数据库name = pathname,创建2个文件 idx和dat
	strcat(db->name,".idx");
	
	// 创建数据库文件idx dat还是打开
	if (oflag & O_CREAT)
	{
		va_list ap;
		// 得到可变参数的值
		va_start(ap,oflag);
		mode = va_arg(ap,int);
		va_end(ap);

		//底层open创建idx文件
		db->idxfd = open(db->name,oflag,mode);
		// 打开了index文件后改名pathname.dat然后通过open创建
		strcpy(db->name+len,".dat");
		db->datfd = open(db->name,oflag,mode);
	}
	else 
	{
		db->idxfd = open(db->name,oflag);
		strcpy(db->name+len,".dat");
		db->datfd = open(db->name,oflag);
	}

	// 文件描述符出错
	if (db->idxfd < 0 || db->datfd < 0)
	{
		_db_free(db);
		return NULL;
	}

	// 仅打开了O_CREAT 位 和trunc位
	if ((oflag & (O_CREAT | O_TRUNC)) == (O_CREAT | O_TRUNC))
	{
		// 宏函数,->lock_reg()->记录锁,FD,off_t,seek_,len
		// 给整个索引文件加上写锁
		if (writew_lock(db->idxfd,0,SEEK_SET,0) < 0)
			err_dump("db_open: writew_lock error");

		if (fstat(db->idxfd,&statbuff) < 0)
			err_sys("db_open: fstat error");

		// 文件大小 字节数
		if (statbuff.st_size == 0)
		{
			// 构造NHASH_DEF+1(137 + 1)个指针(初始值是0),
			// +1的这个指针是hash table之前的空闲列表指针
			// ptr_sz == 7 ? 暂时还不知道为什么指针的大小是这样
			// 打印长度是ptr_sz的0
			sprintf(asciiptr,"%*d",PTR_SZ,0);
			hash[0] = 0;
			// 将字符版指针复制到 hash指针数组中
			for (i=0; i<NHASH_DEF + 1; i++)
				strcat(hash,asciiptr);
			strcat(hash,"\n");
			i = strlen(hash);
			// idx指针部分初始化写入文件,
			if (write(db->idxfd,hash,i) != i)
				err_dump("db_open: index file init write error");
		}
		// 将idx文件解锁
		if (un_lock(db->idxfd,0,SEEK_SET,0) < 0)
			err_dump("db_open: un_lock error");
	}
	//回滚,因为初始化 
	db_rewind(db);
	return db;
}

// DB分配内存
static DB *_db_alloc(int namelen)
{
	DB *db;
	
	// 分配一个长度为sizeof DB 的连续内存
	if ((db = calloc(1,sizeof(DB))) == NULL)
		err_dump("_db_alloc: calloc error for DB");

	db->idxfd = db->datfd = -1;

	// +5 ".idx" or ".dat" + "\n" = 4 + 1 = 5
	if ((db->name = malloc(namelen + 5)) == NULL)
		err_dump("_db_alloc: malloc error for namelen");

	// idxlen:索引记录长度
	if ((db->idxbuf = malloc(IDXLEN_MAX + 2)) == NULL)
		err_dump("_db_alloc: malloc error for index buffer");
	// datlen:数据缓冲区大小
	if ((db->datbuf = malloc(DATLEN_MAX + 2)) == NULL)
		err_dump("_db_alloc: malloc error for data buffer");
	return db;
}

// DBHANDLE = void *
void db_close(DBHANDLE h)
{
	_db_free((DB *)h);
}

// 释放db结构,关闭DB中的point和fd
static void _db_free(DB *db)
{
	//先释放db中的指针分配的内存
	if (db->idxfd >= 0)
		close(db->idxfd);
	if (db->datfd >= 0)
		close(db->datfd);
	if (db->idxbuf != NULL)
		free(db->idxbuf);
	if (db->datbuf != NULL)
		free(db->datbuf);
	free(db);
}

// 给出key,在数据库中找到一条记录
char *db_fetch(DBHANDLE h,const char *key)	
{
	DB *db = h;
	char *ptr;

	// 数据库索引文件被加锁
	if (_db_find_and_lock(db,key,0) < 0)
	{
		ptr = NULL;
		db->cnt_fetcherr++;
	}
	else
	{
		//找到了,读取相应的记录
		ptr = _db_readdat(db);
		db->cnt_fetchok++;
	}

	//解锁索引文件中的hash链
	if (un_lock(db->idxfd,db->chainoff,SEEK_SET,1) < 0)
		err_dump("db_fetch: un_lock error");
	return ptr;
}

// 找到一条记录,被db_delete,db_fetch,db_store调用,返回时hash chain被锁住
static int _db_find_and_lock(DB *db,const char *key,int writelock)
{
	off_t offset,nextoffset;

	// chainoff是key对应的散列链的起始地址,但hashoff是什么???
	db->chainoff = (_db_hash(db,key) * PTR_SZ) + db->hashoff;
	// db->ptroff 记录链的起始地址
	db->ptroff = db->chainoff;

	if (writelock)
	{
		if (writew_lock(db->idxfd,db->chainoff,SEEK_SET,1) < 0)
			err_dump("_db_find_and_lock: writew_lock error");
	}
	else
	{
		if (readw_lock(db->idxfd,db->chainoff,SEEK_SET,1) < 0)
			err_dump("_db_find_and_lock: readw_lock_error");
	}

	//读取hash chain上指针的内容,   db,chain的起始地址
	offset = _db_readptr(db,db->ptroff);

	// offset是第一条记录的偏移量
	while(offset != 0)
	{
		// 读取索引记录中的下一条索引记录的偏移地址,并且将记录中的key填入db->idxbuf字段
		nextoffset = _db_readidx(db,offset);

		// 比较key
		if (strcmp(db->idxbuf,key) == 0)
			break;
		
		// db->ptroff由记录chain的开始地址转化为记录　索引记录读取到的偏移地址
		db->ptroff = offset;
		offset = nextoffset;
	}
	
	// offset == 0 表示没有找到key对应的记录
	return (offset == 0 ? -1 : 0);
}

// 计算得到key对应的hash值
static DBHASH _db_hash(DB *db,const char *key)
{
	DBHASH hval = 0;
	char c;
	int  i;
	
	// 类似双指针
	for (i=1; (c = *key++) != 0; i++)
		hval += c * i;
	return (hval % db->nhash);
}

// offset是idx文件偏移量,返回类型是off_t原因是指针内容就是偏移量
static off_t 
_db_readptr(DB *db,off_t offset)
{
	char asciiptr[PTR_SZ + 1];
	if (lseek(db->idxfd,offset,SEEK_SET) == -1)
		err_dump("_db_readptr: lseek error to ptr field");
	if (read(db->idxfd,asciiptr,PTR_SZ) != PTR_SZ)
		err_dump("_db_readptr: read error of ptr field");
	asciiptr[PTR_SZ] = 0;
	return (atoi(asciiptr));
}

static off_t _db_readidx(DB *db,off_t offset)
{
	ssize_t i;
	char *ptr1,*ptr2;
	char asciiptr[PTR_SZ + 1],asciilen[IDXLEN_SZ + 1];
	struct iovec iov[2];

	// offset == 0 从seek_cur开始,(lseek记忆)
	if ((db->idxoff = lseek(db->idxfd,offset,
					offset == 0 ? SEEK_CUR : SEEK_SET)) == -1)
		err_dump("_db_readidx: lseek error");

	// 索引记录的前２项:　下一条记录偏移量+此条记录的长度
	iov[0].iov_base = asciiptr;
	iov[0].iov_len  = PTR_SZ;
	iov[1].iov_base = asciilen;
	iov[1].iov_len  = IDXLEN_SZ;

	// readv 读取定长字段
	if ((i = readv(db->idxfd,&iov[0],2)) != (PTR_SZ + IDXLEN_SZ))
	{
		// 读取数据=0,并且从cur开始,到底了
		if (i == 0 && offset == 0)
			return -1;
		err_dump("_db_readidx: readv error of index record");
	}

	asciiptr[PTR_SZ] = 0;
	db->ptrval = atol(asciiptr); //字符转long (apue中文版这里印错了)

	asciilen[IDXLEN_SZ] = 0;
	if ((db->idxlen = atoi(asciilen)) < IDXLEN_MIN ||
			db->idxlen > IDXLEN_MAX)
		err_dump("_db_readidx: invalid length");

	// 读取变长部分,长度:db->idxlen,读取到:db->idxbuf
	if ((i = read(db->idxfd,db->idxbuf,db->idxlen)) != db->idxlen)
		err_dump("_db_readidx: read error of index record");

	// 检查实际索引内容的最后是否是\n
	if (db->idxbuf[db->idxlen-1] != NEWLINE)
		err_dump("_db_readidx: missing newline");
	// 0 replace \n
	db->idxbuf[db->idxlen - 1] = 0;

	// 找到首个出现sep的地址
	if ((ptr1 = strchr(db->idxbuf,SEP)) == NULL)
		err_dump("_db_readidx: missing second separator");
	*ptr1++ = 0;

	if ((ptr2 = strchr(ptr1,SEP)) == NULL)
		err_dump("_db_readidx: missing second separator");
	*ptr2++ = 0;

	// 出现第三个分隔符 = error
	if (strchr(ptr2,SEP) != NULL)
		err_dump("_db_readidx: too many separators");

	// data的地址:在.dat中的偏移量 并且将char * -> long long int
	if((db->datoff = atol(ptr1)) < 0)
		err_dump("_db_readidx: starting offset < 0");

	//　data的长度
	if ((db->datlen = atol(ptr2)) < 0 ||
			db->datlen > DATLEN_MAX)
		err_dump("_db_readidx: invalid length");

	// 读取成功,返回下一条记录的偏移地址
	return db->ptrval;
}

// 读取datn,返回以空值结尾的数据缓冲区指针
static char *_db_readdat(DB *db)
{
	// 在.dat中定位到数据开始的地方
	if (lseek(db->datfd,db->datoff,SEEK_SET) == -1)
		err_dump("_db_readdat: lseek error");
	//读取到db->datbuf中
	if (read(db->datfd,db->datbuf,db->datlen) != db->datlen)
		err_dump("_db_readdat: read error");
	// \n完整性检查
	if (db->datbuf[db->datlen - 1] != NEWLINE)
		err_dump("_db_readdat: missing newline");

	db->datbuf[db->datlen - 1] = 0;
	return db->datbuf;
}

// 删除与给定键匹配的记录
int db_delete(DBHANDLE h,const char *key)
{
	DB *db = h;
	int rc = 0;

	// 加上写锁
	if (_db_find_and_lock(db,key,1) == 0)
	{
		_db_dodelete(db);
		// 性能分析计数器(不做性能分析时没屌用)
		db->cnt_delok++;
	}
	else
	{
		rc = -1;
		db->cnt_delerr++;
	}
	// 无论是否找到该锁,都解锁
	if (un_lock(db->idxfd,db->chainoff,SEEK_SET,1) < 0)
		err_dump("db_delete: un_lock error");
	return rc;
}

// _db_find_and_lock 将数据填入DB中,
static void _db_dodelete(DB *db)
{
	int i;
	char *ptr;
	off_t freeptr,saveptr;

	// 存储的数据中间可能有\n
	for (ptr = db->datbuf,i = 0; i<db->datlen - 1; i++)
		*ptr++ = SPACE;
	*ptr = 0;

	ptr = db->idxbuf;
	while(*ptr)
		*ptr++ = SPACE;

	//free_off = 0 锁住了free list
	if (writew_lock(db->idxfd,FREE_OFF,SEEK_SET,1) < 0)
		err_dump("_db_dodelete: writew_lock error");

	// 将空的内容写入到dat = 删除dat中的记录
	_db_writedat(db,db->datbuf,db->datoff,SEEK_SET);
	
	// 读取当前空闲列表表头指针,
	freeptr = _db_readptr(db,FREE_OFF);

	// saveptr 是　下一条记录的偏移量
	saveptr = db->ptrval;

	//下面操作就是向　空闲链表中添加当前记录这一节点
	// 此条记录指向了下一条改为freeptr(即当前空闲列表表头指针)
	_db_writeidx(db,db->idxbuf,db->idxoff,SEEK_SET,freeptr);

	// 空闲列表表头指针指向当前记录 = db->idxoff
	_db_writeptr(db,FREE_OFF,db->idxoff);

	// 修改此记录的上一条记录指向此记录的下一条记录
	_db_writeptr(db,db->ptroff,saveptr);

	if (un_lock(db->idxfd,FREE_OFF,SEEK_SET,1) < 0)
		err_dump("_db_dodelete: un_lock error");
}

// 仅仅被_db_dodelete 和 db_store调用,而_db_dodelete已经加锁了
static void _db_writedat(DB *db,const char *data,off_t offset,int whence)
{
	struct iovec iov[2];
	static char newline = NEWLINE;

	if (whence == SEEK_END)
		// 锁住整个文件
		if(writew_lock(db->datfd,0,SEEK_SET,0) < 0)
			err_dump("_db_writedat: writew_lock error");

	if ((db->datoff = lseek(db->datfd,offset,whence)) == -1)
		err_dump("_db_writedat: lseek error");
	db->datlen = strlen(data) + 1;

	iov[0].iov_base = (char *)data;
	iov[0].iov_len  = db->datlen - 1;
	iov[1].iov_base = &newline;
	iov[1].iov_len  = 1;

	// 写到db->datfd中
	if (writev(db->datfd,&iov[0],2) != db->datlen)
		err_dump("_db_writedat: write error of data record");

	if (whence == SEEK_END)
		if (un_lock(db->datfd,0,SEEK_SET,0) < 0)
			err_dump("_db_writedat: un_lock error");
}

// db,key,索引记录的偏移量,whence,下一指针
static void _db_writeidx(DB *db,const char *key,
		off_t offset,int whence,off_t ptrval)
{
	struct iovec iov[2];
	char asciiptrlen[PTR_SZ + IDXLEN_SZ + 1];
	int len;

	// 修改当前记录指向的下一条记录偏移地址为 ptrval
	if ((db->ptrval = ptrval) < 0 || ptrval > PTR_MAX)
		err_quit("_db_writeidx: invalid ptr: %d",ptrval);

	// 重写索引记录变长部分
	sprintf(db->idxbuf,"%s%c%lld%c%ld\n",key,SEP,
			(long long)db->datoff,SEP,(long)db->datlen);
	
	len = strlen(db->idxbuf);
	if (len < IDXLEN_MIN || len > IDXLEN_MAX)
		err_dump("_db_writeidx: invalid length");

	// 重写索引记录的定长部分
	sprintf(asciiptrlen,"%*lld%*d",PTR_SZ,(long long)ptrval,
			IDXLEN_SZ,len);
	// 上面的准备工作完成
	// appending
	if (whence == SEEK_END)
		// +1 ->　空闲链表表头,len = 0说明锁的范围可以扩展到最大的可能偏移量
		if (writew_lock(db->idxfd,((db->nhash + 1) * PTR_SZ)+1,SEEK_SET,0) < 0)
			err_dump("_db_writeidx: writew_lock error");
	if ((db->idxoff = lseek(db->idxfd,offset,whence)) == -1)
		err_dump("_db_writeidx: lseek error");

	// 索引记录的定长+变长写入
	iov[0].iov_base = asciiptrlen;
	iov[0].iov_len  = PTR_SZ + IDXLEN_SZ;
	iov[1].iov_base = db->idxbuf;
	iov[1].iov_len  = len;
	if (writev(db->idxfd,&iov[0],2) != PTR_SZ +	IDXLEN_SZ + len)
		err_dump("_db_writeidx: writev error of index record");

	// 解锁
	if (whence == SEEK_END)
		if (un_lock(db->idxfd,((db->nhash+1)*PTR_SZ)+1,
					SEEK_SET,0) < 0)
			err_dump("_db_writeidx: un_lock error");
}

// offset处的指针内容改为ptrval
static void _db_writeptr(DB *db,off_t offset,off_t ptrval)
{
	char asciiptr[PTR_SZ + 1];	
	
	//指针内容检测
	if (ptrval < 0 || ptrval > PTR_MAX)
		err_quit("_db_writeptr: invalid ptr: %d",ptrval);
	sprintf(asciiptr,"%*lld",PTR_SZ,(long long)ptrval);

	// 定位
	if (lseek(db->idxfd,offset,SEEK_SET) == -1)
		err_dump("_db_writeptr: lseek error to ptr field");
	if (write(db->idxfd,asciiptr,PTR_SZ) != PTR_SZ)
		err_dump("_db_writeptr: write error of ptr field");
}

int db_store(DBHANDLE h,const char *key,const char *data,int flag)
{
	DB *db = h;
	int rc,keylen,datlen;
	off_t ptrval;

	if (flag != DB_INSERT && flag != DB_REPLACE && flag != DB_STORE)
	{
		errno = EINVAL;
		return -1;
	}
	keylen = strlen(key);
	datlen = strlen(data) + 1; // +1 \n
	if (datlen < DATLEN_MIN || datlen > DATLEN_MAX)
		err_dump("db_store: invalid data length");
	// 找key记录
	if (_db_find_and_lock(db,key,1) < 0)
	{
		if (flag == DB_REPLACE)
		{
			rc = -1;
			db->cnt_storerr++;
			errno = ENOENT;
			goto doreturn;
		}
		// 不是取代那么下面就是插入一条数据
		
		// 读取此条记录的chain首的偏移量
		ptrval = _db_readptr(db,db->chainoff);

		// 没找到keylen和datlen足够的空记录
		if (_db_findfree(db,keylen,datlen) < 0)
		{
			// seek_end锁住整个文件
			_db_writedat(db,data,0,SEEK_END);
			// 此记录的下一条记录是当期chain首的地址
			_db_writeidx(db,key,0,SEEK_END,ptrval);
			
			// chain的首地址改成idxoff,
			_db_writeptr(db,db->chainoff,db->idxoff);
			db->cnt_store1++;
		}
		// 有足够的空间存储key和data
		else
		{
			_db_writedat(db,data,db->datoff,SEEK_SET);
			_db_writeidx(db,key,db->idxoff,SEEK_SET,ptrval);
			_db_writeptr(db,db->chainoff,db->idxoff);
			db->cnt_store2++;
		}
	}
	// 记录找到
	else
	{
		if (flag == DB_INSERT)
		{
			rc = 1; // 记录存在但是flag是插入
			db->cnt_storerr++;
			goto doreturn;
		}

		// 接下来就是replace情况
		// 长度不是恰好
		if (datlen != db->datlen)
		{
			// 删除原来已经存在的此条记录+数据
			_db_dodelete(db);
	
			ptrval = _db_readptr(db,db->chainoff);
			// 写入idx和dat
			_db_writedat(db,data,0,SEEK_END);
			_db_writeidx(db,key,0,SEEK_END,ptrval);
			//改变chain
			_db_writeptr(db,db->chainoff,db->idxoff);
			db->cnt_store3++;
		}
		//带replace的数据长度正好
		else
		{
			_db_writedat(db,data,db->datoff,SEEK_SET);
			db->cnt_store4++;
		}
	
	}
	rc = 0;
doreturn:
	if (un_lock(db->idxfd,db->chainoff,SEEK_SET,1) < 0)
		err_dump("db_store: unlock_error");
	return rc;
}

static int _db_findfree(DB *db,int keylen,int datlen)
{
	int rc;
	off_t offset,nextoffset,saveoffset;

	// 锁住空闲列表
	if (writew_lock(db->idxfd,FREE_OFF,SEEK_SET,1) < 0)
		err_dump("_db_findfree: writew_lock error");

	saveoffset = FREE_OFF;
	offset = _db_readptr(db,saveoffset);

	while (offset != 0)
	{
		//将offset的idx填入db中,然后返回下一条idx的地址
		nextoffset = _db_readidx(db,offset);
		if (strlen(db->idxbuf) == keylen && db->datlen == datlen)
			break;
		saveoffset = offset;
		offset = nextoffset;
	}

	// 没有找到
	if (offset == 0)
	{
		rc = -1;
	}
	else
	{
		// 修改上一条记录的ptrval改成当前记录的ptrval,即删除３个节点中的中间节点
		_db_writeptr(db,saveoffset,db->ptrval);
		rc = 0;
	}

	if (un_lock(db->idxfd,FREE_OFF,SEEK_SET,1) < 0)
		err_dump("_db_findfree: un_lock error");
	return rc;
}

void db_rewind(DBHANDLE h)
{
	DB *db = h;
	off_t offset;

	// +1 = 空闲列表指针
	offset = (db->nhash + 1) * PTR_SZ;
	// +1 hash table后面的\n
	if ((db->idxoff = lseek(db->idxfd,offset + 1,SEEK_SET)) == -1)
		err_dump("db_rewind: lseek error");
}

char *db_nextrec(DBHANDLE h,char *key)
{
	DB *db = h;
	char c;
	char *ptr;

	// 上读锁
	if (readw_lock(db->idxfd,FREE_OFF,SEEK_SET,1) < 0)
		err_dump("db_nextrec: readw_lock error");
	// 0 在readidx的时候会在buf最后添加0,然后dodelete时会写回,
	// 通过0和空格跳过被删除的索引记录
	do 
	{
		// 0表示从seek_cur开始读取
		if (_db_readidx(db,0) < 0)
		{
			ptr = NULL;
			goto doreturn;
		}
		ptr = db->idxbuf;
		// c = 0 就终止,否则遇到space跳过
		while ((c = *ptr++) != 0 && c == SPACE)
			;
	}
	while(c == 0);

	if (key != NULL)
		strcpy(key,db->idxbuf);

	//通过db->datoff
	ptr = _db_readdat(db);
	db->cnt_nextrec++;
doreturn:
	if (un_lock(db->idxfd,FREE_OFF,SEEK_SET,1) < 0)
		err_dump("db_nextrec: un_lock error");
	return ptr;
}





