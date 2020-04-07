/*************************************************************************
    > File Name: apue_db.h
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年04月04日 星期六 23时04分56秒
 ************************************************************************/
#ifndef _APUE_DB_H
#define _APUE_DB_H

// void *类型封装
typedef void *DBHANDLE;

// 得到数据库句柄
DBHANDLE db_open(const char *,int,...);

void db_close(DBHANDLE);

// 根据key查询数据
char *db_fetch(DBHANDLE,const char *);

// db,key,data,flag
int db_store(DBHANDLE,const char *,const char *,int);

// 根据key来删除
int db_delete(DBHANDLE,const char *);

// 回滚到数据库的第一条记录
void db_rewind(DBHANDLE);

// 得到下一条数据data
char *db_nextrec(DBHANDLE,char *);

#define DB_INSERT  1 // 插入一条新数据
#define DB_REPLACE 2 // 取代存在的一条数据
#define DB_STORE   3 // 取代或者插入

#define IDXLEN_MIN  6  // 索引记录长度的最小值,
#define IDXLEN_MAX  1024

#define DATLEN_MIN 2 //数据字节
#define DATLEN_MAX 1024 

#endif
