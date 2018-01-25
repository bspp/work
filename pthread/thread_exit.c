/**********************************************************************************************
 *
 * 结束线程：结束线程通常是由该线程自身发起的。
 * pthread_exit(void *retval);
 * 以阻塞的方式等待thread指定的线程结束。
 * int pthread_join(pthread_t thread, void **retval);
 * ********************************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<malloc.h>

#define checkResults(string, val) { \
	if (val) { \
		printf("Failed with %d at %s \n", val, string); \
		exit(1); \
	} \
}

const int THREADFAIL = 1;
const int THREADPASS = 0;

struct thead_parm{
	int threadParm1;
	char threadParm2[128];
};
typedef struct thead_parm THREAD_PARM_S;

void *theThread(void *parm)
{

	printf("Thread : End with sucess \n");
	pthread_exit((void *)&THREADPASS);
	printf("Thread: Did not expect to get here!\n");

	return (void *)&THREADFAIL;
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

	rc = pthread_join(thread,&status);
	checkResults("pthread_join()\n", rc);

	if(status != ((void *)&THREADPASS))
	{
		printf("The thread failed\n");
	}

	printf("Main completed\n");

	return 0;
}
