#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

static void charatatime(char *str);

void err_sys(char *str)
{
	printf("%s\n",str);
}
int main(void)
{
	pid_t pid;
	
	
	if((pid = fork()) < 0)
		err_sys("fork error");
	else if(pid == 0)
	{
		charatatime("Out put from child\n");
	}else
	{
		charatatime("Out put from parent\n");
	}

	exit(0);
		
}

static void charatatime(char *str)
{
	char *ptr;
	int c;

	setbuf(stdout,NULL);/* set unbuffered */
	for(ptr = str; c = *ptr++;)
		putc(c,stdout);
}
