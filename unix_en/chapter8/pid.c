/**
 *进程标识符
 ****************/

#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>

int main()
{
	//pid_t getpid(void); 
	printf("pid : %d\n",getpid());

	//pid_t getppid(void) 父进程Id
	
	printf("ppid %d\n",getppid());

	//pid_t getgid(void) 调用进程的实际组ID
	
	printf("gid %d\n",getgid());

	//pid_t getegid(void) 调用进程的有效组ID
	
	printf("egid %d\n",getegid());



	return 0;
}

