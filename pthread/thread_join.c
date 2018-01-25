/**********************************************************************************************
*
* 结束线程：结束线程通常是由该线程自身发起的。
* pthread_exit(void *retval);
* 以阻塞的方式等待thread指定的线程结束。
* int pthread_join(pthread_t thread, void **retval);
*
*********************************************************************************************/

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

#define THREADGROUPSIZE 5

void *theThread(void *parm)
{

	printf("Entered : Thread is %ld\n",pthread_self());
	printf("Working : Thread is %ld\n",pthread_self());
	sleep(15);
	printf("Dnoe : Thread is %ld\n",pthread_self());
	return NULL;
}

int main(int argc,char **argv)
{
	pthread_t thread[THREADGROUPSIZE];
	void  *status[THREADGROUPSIZE];
	int rc = 0;


	printf("Create some  new  thread \n");
	printf("Wait for worker threads to complete, release their resources\n");
	for(int i = 0; i < THREADGROUPSIZE;i++)
	{
		rc = pthread_create(&thread[i], NULL, theThread, NULL);
		checkResults("pthread_create()\n", rc);
	}

	for (int i = 0; i < THREADGROUPSIZE; i++) {
		rc = pthread_join(thread[i], &status[i]);
		checkResults("pthread_join()\n", rc);

	}

	printf("Check all thread's results\n");
	for (int i = 0; i < THREADGROUPSIZE; i++) {
		if (status[i] != NULL) {
			printf("Unexpected thread status\n");

		}
	}

	printf("Main completed\n");
	return 0;
}
