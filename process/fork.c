/*======================================================================================
* Copyright (C) 2018 Sangfor Ltd. All rights reserved.
* Programmer :Linyuanpeng
* Date       :2018/01/26
*
* pid_t fork(void);
*  return：子进程中为 0，父进程中为子进程 I D，出错为- 1
*
* 由f o r k创建的新进程被称为子进程（child process）。该函数被调用一次，但返回两次。两次返
* 回的区别是子进程的返回值是 0，而父进程的返回值则是新子进程的进程 I D。
*========================================================================================*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define checkResults(string, val) { \
	if (val) { \
		printf("Failed with %d at %s \n", val, string); \
		exit(1); \
	} \
}

int glob = 6;

int main(void)
{
	int var = 0;
	pid_t pid = -1;

	if((pid = fork()) < 0)
	{
		checkResults("fork error",pid);
	}	else if (pid == 0)/* 子进程 */
	{
		glob++;
		var++;
		printf("This process pid is %d\n",getpid());
		printf("This process parent pid is  %d\n",getppid());
	}else
	{
		sleep(1);
		glob+=2;
		var+= 2;
		printf("This process pid is %d\n",getpid());
	}

	printf("glob value is %d and var value is %d\n",glob,var);
}


