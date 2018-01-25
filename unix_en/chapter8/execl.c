/*********************************************************************************
 * fork()函数创建子进程后，子进程往往要调用一种exec函数执行另外一个程序。
 * 但进程调用exec函数时，该进程完全由新程序代换，而新程序从其main函数开始执行。
 * 因为exec并不创建新进程，所以前后的进程ID并未改变。exec只是用一个程序替换了当前
 * 进程的正文，数据，堆，栈段。
 * 简单讲exec函数功能就是把当前进程替换为一个新进程，且新进程和原进程的pid相同
 * ecec关联函数组（execl,execlp,execle,execv,execvp）
 * int execl(const char *path,const char *arg,...);
 * ********************************************************************************/

#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>


void err_sys(char *str)
{
	printf("%s\n",str);
	exit(0);
}
int main(void)
{
	pid_t pid = 0;

	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
	{
		if(execl("/bin/ls","ls","-l",NULL) < 0)
			err_sys("exec error");

		printf("child process --------------------------------------------------------------------\n");
	}

	if(waitpid(pid,NULL,0) < 0)
		err_sys("wait error");

	printf("main process ------------------------------------------------------------------\n");

	return 0;
}
