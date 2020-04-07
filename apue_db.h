#ifndef _APUE_DB_H
#define _APUE_DB_H

// 类型DBHANDLE是void * (空指针)
typedef	void *	DBHANDLE;

// 返回代表数据库的句柄
DBHANDLE  db_open(const char *, int, ...);

// 关闭数据库 关闭索引文件和数据文件
void      db_close(DBHANDLE);

// 查询数据,返回指向key存放数据的指针,
char     *db_fetch(DBHANDLE, const char *);

// 存储信息, key唯一,
int       db_store(DBHANDLE, const char *, const char *, int);

// 删除一条信息
int       db_delete(DBHANDLE, const char *);

// 回滚到数据库的第一条记录
void      db_rewind(DBHANDLE);

// 顺序读每一条记录
char     *db_nextrec(DBHANDLE, char *);

/*
 * Flags for db_store().
 */
#define DB_INSERT	   1	/* insert new record only */
#define DB_REPLACE	   2	/* replace existing record */
#define DB_STORE	   3	/* replace or insert */

/*
 * Implementation limits.
 */
#define IDXLEN_MIN	   6	/* key, sep, start, sep, length, \n */
#define IDXLEN_MAX	1024	/* arbitrary */
#define DATLEN_MIN	   2	/* data byte, newline */
#define DATLEN_MAX	1024	/* arbitrary */

#endif /* _APUE_DB_H */
