/*********************************************************************************
 * int execlp(const char *file, const char *arg, ...);
 * 带p的exec函数：execlp,execvp 表示第一个参数path不同
 * 输入完整路径，只有给出命令名即可，她会在环境变量PATH当中查找。
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
		if(execl("ls","ls","-al","/etc/passwd",NULL) < 0)
			err_sys("exec error");

		printf("child process --------------------------------------------------------------------\n");
	}

	if(waitpid(pid,NULL,0) < 0)
		err_sys("wait error");

	printf("main process ------------------------------------------------------------------\n");

	return 0;
}
