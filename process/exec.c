/*======================================================================
* Copyright (C) 2018 Sangfor Ltd. All rights reserved.
* Programmer :Linyuanpeng
* Date       :2018/01/28
*
* fork函数创建子进程后，子进程往往要调用exec函数族以执行另一个程序。
* 当进程调用一种exec函数时，该进程完全由新程序代换，而程序则从main函数
* 开始执行。exec函数族并不创建新进程，所以执行exec函数族前后的进程ID并
* 不发生改变。exec函数族只是用另外一个新程序代替了当前进程中的正文，
* 数据，堆和和栈段。
* int execl(const char *path, const char *arg, ...);
* int execv(const char *path, char *const argv[]); 
* int execlp(const char *file, const char *arg, ...);  
* int execvp(const char *file, char *const argv[]);  
* l后续,命令参数部分必须以"," 相隔, 最后1个命令参数必须是NULL
* v后续,命令参数部分必须是1个以NULL结尾的字符串指针数组的头部指针.
* p后续,执行文件部分可以不带路径, exec函数会在$PATH中找
*========================================================================*/

#include<unistd.h>

#include"process.h"

int main(void)
{
	pid_t pid = 0;

	/* execl 函数*/
	printf("execl:\n");

	if((pid = fork()) < -1)
	{
		checkResults("fork error",-1);
	}else if (pid == 0)
	{
		if (execl("/bin/ls","ls","-al" ,NULL) <0 )
		{
			checkResults("execl error",-1);
			exit(0);
		}
	}
	
	if(wait(NULL) < 0)
	{
		checkResults("waitpid error",-1);
	}

	/* execlp 函数*/
	printf("execlp:\n");

	if((pid = fork()) < -1)
	{
		checkResults("fork error",-1);
	}else if (pid == 0)
	{
		if (execlp("ls","-al" ,NULL) <0 )
		{
			checkResults("execl error",-1);
			exit(0);
		}
	}
	
	if(wait(NULL) < 0)
	{
		checkResults("waitpid error",-1);
	}
	
	/* execv 函数 */

	printf("\nexecv:\n");

	if((pid = fork()) < -1)
	{
		checkResults("fork error",-1);
	}else if (pid == 0)
	{
		char *execv_str[] = {"ls","-l",NULL};
		if (execv("/bin/ls",execv_str) <0 )
		{
			checkResults("execl error",-1);
			exit(0);
		}
	}
	
	if(wait(NULL) < 0)
	{
		checkResults("waitpid error",-1);
	}
	return 0;
}

