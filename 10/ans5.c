//定时器链表
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

struct timer_t {
	unsigned int started_at;
	unsigned int ended_at;
	void (*proc)();
	struct timer_t *next;
};

struct timer_t *head = NULL;

// 信号捕捉程序,从链表头开始执行
void 
alarm_proc(int signo)
{
	// 链表是按照时间排序的
	struct timer_t *cur;
	cur = head;
	cur->proc();

	// 此node执行完毕后,删除此node
	if (cur->next) {
		alarm(cur->next->ended_at - cur->ended_at);
		free(cur);
		head = cur->next;
	}
	// 此node是最后一个node
	else {
		free(head);
		head = NULL;
	}
}

// 将node根据time加入到链表中
unsigned int
push_to_list(struct timer_t *node)
{
	unsigned int cur_time;
	time((time_t *)&cur_time);

	struct timer_t *cur = head;

	// 链表为空
	if (cur == NULL) {
		head = node;
		// 根据ended_at time设置alrm信号到来时间
		alarm(node->ended_at - cur_time);
		return (head->ended_at - cur_time);
	}
	// 链表不为空 普通情况
	else {
		//　根据结束时间插入链表
		if (head->ended_at > node->ended_at) {
			node->next = head;
			head = node;
			// 定时以当前时间为标准
			alarm(node->ended_at - cur_time);
			return (node->ended_at - cur_time);
		}
		else {
			// while循环找到合适位置
			while (cur->next && cur->next->ended_at < node->ended_at)
				cur = cur->next;

			node->next = cur->next;
			cur->next = node;
			return (head->ended_at - cur_time);
		}
	}
}

void 
settimeout(unsigned int seconds,void (*proc)())
{
	// 设置定时器时间为0
	if (seconds == 0) {
		proc();
		return ;
	}

	// 开始分配head
	struct timer_t *temp = (struct timer_t *)malloc(sizeof(struct timer_t));
	// 得到当前时间
	time((time_t *)&(temp->started_at));
	
	// 设置当前node
	temp->ended_at = temp->started_at + seconds;
	temp->next = NULL;
	temp->proc = proc;

	// 将该节点的信号处理函数注册
	if (signal(SIGALRM,alarm_proc) == SIG_ERR) {
		printf("signal (SIGALRM) error\n");
		exit(0);
	}
	
	// 将该节点加入到链表中
	push_to_list(temp);
}
 
void print1() { printf("test1\n"); }
void print2() { printf("test2\n"); } 

int main(int argc,char *argv[]) {
	settimeout(5,print2);
	settimeout(3,print1);
	settimeout(7,print1);
	for (;;)
		pause();
    exit(0);
}
