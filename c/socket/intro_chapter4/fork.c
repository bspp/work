#include<stdio.h>
#include<unistd.h>
int main()
{
	pid_t fpid;
	int count = 0;
	fpid = fork();
	if(fpid < 0)
	{
		printf("error in fork");
	}else if(fpid == 0)
	{
		printf("this is child process, process id is %d\n",getpid());
		count ++;
	}else
	{
		printf("this is the parent process,process id  is %d\n",getpid());
		count++;
	}
	printf("Count %d\n",count);
	return 0;
}
