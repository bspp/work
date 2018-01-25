/**********************************************************************************************
 *
 * 启动线程pthread_create(pthread_t  *thread,pthread_attr_t  *attr,
 *                       void  *（*start_routine）（void  *）,void  *arg);
 * 说明：thread：线程标识符；
 * attr：线程属性设置；
 * start_routine：线程函数的起始地址；
 * arg：传递给start_routine的参数；
 * 返回值：成功，返回0；出错，返回-1。
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
struct thead_parm{
	int threadParm1;
	char threadParm2[128];
};
typedef struct thead_parm THREAD_PARM_S;

void *theThread(void *parm)
{
	THREAD_PARM_S *pthread_parm = (THREAD_PARM_S *)parm;
	printf("Thread Parameters :%d is the answer to %s \n",
			pthread_parm->threadParm1,pthread_parm->threadParm2);
	sleep(2);
	return NULL;
}

int main(int argc,char **argv)
{
	pthread_t thread = {0};
	THREAD_PARM_S *threadParm = NULL;
	int rc = 0;

	threadParm = (THREAD_PARM_S *)malloc(sizeof(threadParm));
	if(threadParm == NULL)
	{
		printf("Malloc threadParm : Out of space.\n");
		return -1;
	}

	threadParm->threadParm1 = 42;
	strcpy(threadParm->threadParm2, "Life, the Universe and Everything");

	printf("Create a new  thread \n");
	rc = pthread_create(&thread, NULL,theThread,threadParm);
	checkResults("pthread_create()\n", rc);

	printf("Main completed\n");

	return 0;
}
