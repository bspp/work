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

int main()
{
	pid_t pid = -1;
	
	if((pid = fork()) == -1)
	{
		checkResults("fork error",pid);
	}else if(pid == 0) //子进程
	{
		printf("this is child process\n");
		printf("sleep....\n");
		sleep(5);
	}else{
		wait(NULL);
		printf("This is parent process\n");
	}
}

