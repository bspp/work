/**********************************************************************************************
 *
 * 结束取消：取消线程通常不是由该程序自身发起的，而是由其它的线程发起。
 * int pthread_cancel(pthread_t thread)
 * 如 果 子 线 程 被 成 功 取 消 ， 那 么 pthread_join 函 数 取 到 的 状 态 将 为
 * PTHREAD_CANCELED。
 * ********************************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>

#define checkResults(string, val) { \
	if (val) { \
		printf("Failed with %d at %s \n", val, string); \
		exit(1); \
	} \
}


void *theThread(void *parm)
{

	printf("Thread ； Entered \n");
	while(1)
	{
		printf("Thread:Looping or long running request \n");
		sleep(1);
	}
	return NULL;
}

int main(int argc,char **argv)
{
	pthread_t thread = {0};
	int rc = 0;
	void *status;

	printf("Create a new  thread \n");
	printf("Wait for the thread to complete,and release its resource \n");
	rc = pthread_create(&thread, NULL,theThread,NULL);
	checkResults("pthread_create()\n", rc);

  printf("Wait a bit until we 'realize' the thread needs to be canceled\n");
	sleep(3);
	rc = pthread_cancel(thread);
	checkResults("pthread_cancel()\n", rc);

	printf("Wait for the thread to complete, and release its resources\n");
	rc = pthread_join(thread, &status);
	checkResults("pthread_join()\n", rc);

	printf("Thread status indicates it was canceled\n");
	if (status != PTHREAD_CANCELED) {
		printf("Unexpected thread status\n");

	}
	printf("Main completed\n");

	return 0;
}
