/*======================================================================
 * Copyright (C) 2018 Sangfor Ltd. All rights reserved.
 * Programmer :Linyuanpeng
 * Date       :2018/01/31
 *========================================================================*/

#include<signal.h>
#include<time.h>

#include "process.h"

void handler(int sig);

int main()
{
	struct sigaction act = {0};

	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;

	if(sigaction(SIGINT,&act,NULL) < 0)
		checkResults("sigaction err",-1);

	sleep(9);

	return 0;

}
void handler(int sig)
{
	printf("rev sig=%d\n", sig);
}


