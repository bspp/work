/**
 *
 * 一个现存进程调用 f o r k函数是U N I X内核创建一个新进程的唯一方法
 * pid_t fork(void);
 *
 * 返回：子进程中为 0，父进程中为子进程 I D，出错为- 1
 * fork 创建的新进程被称为子进程（child process）,该函数别调用一次，但是返回两次。
 * 子进程的返回值是0，而父进程返回的是新子进程的进程ID
 * 将子进程ID返回给父进程的理由是：
 * 一个父进程的子进程可以多于一个，所有没有一个函数使一个进程可以获取其所有子进程的进程ID.
 * f o r k使子进程得到返回值 0的理由是：
 * 一个进程只会有一个父进
 * 程，所以子进程总是可以调用 g e t p p i d以获得其父进程的进程I D
 * 子进程和父进程继续执行 f o r k之后的指令。子进程是父进程的复制品。例如，子进程获得
 * 父进程数据空间、堆和栈的复制品。
 */
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<time.h>

int glob = 6;
char buf[] = "a write to stdout\n";

int main(void)
{
	int var = 88;
	pid_t pid = -1;

	if(write(STDOUT_FILENO,buf,sizeof(buf)-1) != sizeof(buf)-1)
	{
		printf("write error\n");
		return 1;
	}
	printf("Before fork\n");

	if((pid = fork()) < 0)
	{
		printf("fork error\n");
		return 1;
	}else if(pid == 0)
	{
		glob++;
		var++;
		printf("clild pid = %d,ppid = %d ,glob = %d var = %d\n",getpid(),getppid(),glob,var);
	}else
	{
		sleep(2);
		printf("main pid = %d,ppid = %d ,glob = %d var = %d\n",getpid(),getppid(),glob,var);
	}


	return 0;
}
