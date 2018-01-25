/**
 * 当一个进程正常或异常终止时，内核就向其父进程发送SIGHCLD信号。
 * 因为子进程终止是一个异步事件（这可以在父进程运行的任何时候发生），所以这种信号也是
 * 内核向父进程发的异常通知。
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
	if(WIFEXITED(status))/* 若为正常终止子进程返回状态为真 */
		printf("normal termination,exit status = %d\n",WEXITSTATUS(status));
	else if(WIFSIGNALED(status))
		printf("abnormal termination,signal number = %d%s\n",WTERMSIG(status),
#ifdef WCOREDUMP
				WCOREDUMP(status) ? "(core file.generated)":"");
#else
	"");
#endif
	else if(WIFSTOPPED(status))
		printf("chilid stopped,signal number = %d \n",WSTOPSIG(status));
}

void err_sys(char *str)
{
	printf("%s\n",str);
	exit(1);
}

int main(void)
{
	pid_t pid = 0;
	pid_t wait_ret = 0;
	int status = 0;


	printf("main process ,pid is %d \n",getpid());
	if((pid = fork()) < 0)
	{
		printf("fork error\n");
		return -1;
	}else if(pid == 0)/* child */
	{
		printf("This is child process ,pid is %d \n",getpid());
		exit(7);
	}


	if((wait_ret = wait(&status)) != pid)/* wait for child */
		err_sys("wait error");
	else
	{
		printf("wait return values is %d \n",wait_ret);
		pre_exit(status);
	}

	if((pid = fork()) < 0)
		err_sys("fork error ");
	else if (pid == 0)
		abort();
	if((wait(&status)) != pid)/* wait for child */
		err_sys("wait error");
	else
		pre_exit(status);

	if((pid = fork()) < 0)
		err_sys("fork error ");
	else if (pid == 0)
		status /= 0;
	if((wait(&status)) != pid)/* wait for child */
		err_sys("wait error");
	else
		pre_exit(status);

	return 0;
}

