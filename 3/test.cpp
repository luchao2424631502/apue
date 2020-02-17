/*************************************************************************
    > File Name: test.cpp
  > Author: luchao
  > Mail: 18271496687@163.com 
  > Created Time: 2020年01月23日 星期四 14时05分42秒
 ************************************************************************/

#include <bits/stdc++.h>
#include "apue.h"
#include <unistd.h>
 
struct Node {
	int fd;
	Node *next;
};

// 回收所有空间
void
free_fd(Node *head) 
{
	while(head != nullptr) {
		Node *temp = head;
		// 先关闭文件描述符,
		close(head->fd);
		std::cout << "close " << head->fd << "\n";
		//　关闭后回收内存
		free(head);
		head = temp->next;
	}
}

int
mydup2(int fd,int fd2) 
{
	if (fd == fd2)
		return fd;
	//如果fd2已经打开 则先关闭fd2
	close(fd2);
	
	int temp_fd;
	Node *head = (Node *)malloc(sizeof(Node));
	Node *cur = head;
	// fd2 这一个文件描述符没有存
	while((temp_fd = dup(fd)) != fd2) {
		std::cout << "dup " << temp_fd << "\n";
		// 首先判断是否返回出错
		if (temp_fd == -1) {
			// 清楚所有已经分配的fd
			free_fd(head);
			return -1;
		}
		// 分配空间保存多余的且打开的文件描述符
		Node *temp = (Node *)malloc(sizeof(Node));
		temp->next = nullptr;
		temp->fd = temp_fd;
		cur->next = temp;
		cur = temp;
	}
	
	//　回收fd2之前的所有内存
	free_fd(head);

	return fd2;
}

int 
main () 
{
	int fd = mydup2(STDOUT_FILENO,100);
	std::cout << fd;
    return 0;
}
