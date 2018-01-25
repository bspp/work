/**********************************************************************************************************
 *(1)waitpid等待一个特定的进程 (而w a i t则返回任一终止子进程的状态 )。在讨论p o p e n函数
 *时会再说明这一功能。
 *(2)waitpid支持作业控制
 *(3)waitpid提供了一个w a i t的非阻塞版本。有时希望取得一个子进程的状态，但不想阻塞。
 **********************************************************************************************************/

#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

void err_sys(char *str)
{
	printf("%s\n",str);
}
int main(void )
{
	pid_t pid = 0;


	printf("main pid is %d , parent pid is %d\n",getpid(),getppid());
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0){ /* first child */
		printf("firset pid is %d , parent pid is %d\n",getpid(),getppid());
		if((pid = fork()) < 0)
			err_sys("fork error");
		else if(pid > 0)
			exit(0);/* first child */

		sleep(2);
		printf("second pid is %d , parent pid is %d\n",getpid(),getppid());
		exit(0);
	}

	if(waitpid(pid,NULL,0) != pid)/* wait for first child */
		err_sys("waitpid error");
	else
	{
		printf("firsrt child process exit.\n");
	}

	return 0;
}
