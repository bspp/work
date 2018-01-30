/*======================================================================
 * Copyright (C) 2018 Sangfor Ltd. All rights reserved.
 * Programmer :Linyuanpeng
 * Date       :2018/01/26
 *
 * pid)t watipid(pid_t pid,int *statloc,int options)
 * wait函数的缺陷在于一旦有一个子进程结束,那么wait就会返回，而通常一个进程
 * 可以包含多个子进程，那么wait函数就会产生一个较大的缺陷：无法等待一个特定
 * 或指定的子进程。
 * POSIX.1标准提供了waitpid函数以满足wait无法满足的需求。
 * pid == -1 等待任一子进程
 * pid > 0 等待进程ID和pid相等的子进程。
 * pid == 0 等待其组ID和调用进程的组ID相同的任一子进程
 * pid <-1 等待其组ID等于pid的绝对值的任一子进程。
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
	pid_t pid = 0;

	if((pid = fork()) < 0)
	{
		checkResults("fork error",pid);
	}
	else if(pid == 0)
	{
		if((pid = fork()) < 0)
		{
			checkResults("fork error",pid);
		}
		else if (pid > 0)
		{
			printf("fist child,pid = %d \n",getpid);
			printf("fist child,parent pid = %d \n",getppid);
			exit(0);
		}

		sleep(2);
		printf("second child,parent pid = %d \n",getppid);
		exit(0);
	}

	if(waitpid(pid,NULL,0) != pid)
	{
		checkResults("wait error",-1);
	}/* wait for first child */

	exit(0);
}

