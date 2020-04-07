/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年04月04日 星期六 18时01分45秒
 ************************************************************************/

#include "apue.h"
#include "apue_db.h"
#include <fcntl.h>
 
int main(int argc,char *argv[]) 
{ 
	DBHANDLE db;

	if ((db = db_open("db4", O_RDWR | O_CREAT | O_TRUNC,
					FILE_MODE)) == NULL)
		err_sys("db_open error");

	if (db_store(db,"Alpha","datal",DB_INSERT) != 0)
		err_quit("db_store error for alpha");
	if (db_store(db,"beta","Data for beta",DB_INSERT) != 0)
		err_quit("db_store error for beta");
	if (db_store(db,"gamma","record3",DB_INSERT) != 0)
		err_quit("db_store error for gamma");

	db_close(db);

    return 0;
}
