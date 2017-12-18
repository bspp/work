/**
 * pid_t wait(int *statloc)
 * pid_t waitpid(pid_t pid,int *statloc,int options)
 * 两个函数返回：若成功那么为进程ID,若出错那么为-1
 *************************************************/

#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<sys/wait.h>
#include<stdlib.h>

void pre_exit(int status)
{
	if(WIFEXITED(status))
		printf("normal termination,exit status = %d\n",WEXITSTATUS(status));
	else if(WIFSIGNALED(status))
		printf("abnormal termination, signal number = %d%s\n",WTERMSIG(status),
#ifdef WCOREDUMP 
				WCOREDUMP(status) ? "(Core file.generated)":"");
#else 
	"");
#endif
	else if(WIFSTOPPED(status))
		printf("child stopped,signal number = %d \n",WSTOPSIG(status));


}
void err_sys(char *str)
{
	printf("%s\n",str);
	exit;
}
int main(void)
{
	pid_t pid = 0;
	int status = 0;


	if((pid = fork()) < 0)/* create a child process*/
	{
		printf("fork error\n");
		return -1;
	}else if(pid == 0)/* child */
	{
		exit(7);
	}

	if(wait(&status) != pid)/* wait for child*/
		err_sys("wait error");
	else if(pid == 0) /* generates SIGABRT*/


	return 0;
}

