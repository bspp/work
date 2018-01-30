/*======================================================================
 * Copyright (C) 2018 Sangfor Ltd. All rights reserved.
 * Programmer :Linyuanpeng
 * Date       :2018/01/26
 *
 * pid_t wait(int *statloc);
 * pid)t watipid(pid_t pid,int *statloc,int options)
 *    两个函数返回：若成功则为进程 I D，若出错则为-1
 * wait()函数用于使父进程（也就是调用wait()的进程）阻塞，
 * 直到一个子进程结束或者该进程接收到了一个指定的信号为止。
 * 如果该父进程没有子进程或者它的子进程已经结束，则wait()函数就会立即返回。
 *========================================================================*/

#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#include <stdio.h>
#include <stdlib.h>

#define checkResults(string, val) { \
	if (val) { \
		printf("Failed with %d at %s \n", val, string); \
		exit(1); \
	} \
}
void pr_exit(int status)
{
	if(WIFEXITED(status))
		printf("normal termination,exit status = %d \n",WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("abnormal termination,signal number = %d%s\n",WTERMSIG(status),
#ifdef WCOREDUMP
				WCOREDUMP(status) ? "core file generated":"");
#else
	"");
#endif
	else if(WIFSTOPPED(status))
		printf("child stopped,signal number = %d \n",WSTOPSIG(status));
}


int main()
{
	pid_t pid = -1;
	int status = 0;

	if((pid = fork()) == -1)
	{
		checkResults("fork error",pid);
	}else if(pid == 0) //子进程
	{
		exit(7);
	}

	if(wait(&status) != pid)
		checkResults("wait err",-1);
	pr_exit(status);

	
	if((pid = fork()) == -1)
	{
		checkResults("fork error",pid);
	}else if(pid == 0) //子进程
	{
		abort(); /* generates SIGABORT */
	}

	
	if(wait(&status) != pid)
		checkResults("wait err",-1);
	pr_exit(status);
}

