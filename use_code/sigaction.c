/*======================================================================
*   Copyright (C) 2018 Sangfor Ltd. All rights reserved.
*   Programmer :Linyuanpeng
* 	Date       :2018/01/25
*========================================================================*/


#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>

static void sig_user(int signum)
{
	switch(signum)
	{
		case SIGUSR1:
			printf("SIGUSER1 received \n");
			break;
		case SIGUSR2:
			printf("SIGUSER2 received \n");
			break;
		default:
			printf("signal %d received \n",signum);
	}
}
int main(void)
{
	char buf[512] = {0};
	int n = 0;
	struct sigaction s_sigaction = {0};

	s_sigaction.sa_flags = 0;
	s_sigaction.sa_handler = sig_user;

	sigaction(SIGUSR1, &s_sigaction, NULL);
	sigaction(SIGUSR2, &s_sigaction, NULL);

	printf("My PID is %d \n",getpid());

	while(1)
	{
		if((n = read(STDIN_FILENO,buf,511)) == -1)
		{
			if(errno == EINTR)
			{
				printf("read is interrupted by signal");
			}else
			{
				 buf[n] = '\0';
				 printf("%d bytes read: %s\n", n, buf);
			}
		}
	}

	return 0;
}


