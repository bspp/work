/*********************************************************************************
 * int execv(const char *path,char *const argv[]);
 * 不带 l 的exec函数：execv,execvp表示命令所需的参数以char *arg[]形式给出且arg最后一个元素必须
 * 是NULL
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

	
	char *argv[] = {"ls","-l","/root",NULL};
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
	{
		printf("child process start --------------------------------------------------------------------\n");

		if(execv("/bin/ls",argv) < 0)
			err_sys("exec error");

		printf("child process end --------------------------------------------------------------------\n");
		exit(7);
	}

	if(waitpid(pid,NULL,0) < 0)
		err_sys("wait error");


	return 0;
}
