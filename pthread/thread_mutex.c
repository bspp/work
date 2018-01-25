/*======================================================================
* Copyright (C) 2017 Sangfor Ltd. All rights reserved.
* Programmer :Linyuanpeng
* Date       :2017/12/27
========================================================================*/


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

#define THREADS 10
int test_number = 1;
pthread_mutex_t test_mutex;


void *theThread(void *parm)
{
	int rc = 0;

	printf("Entered thre thread %lx \n",pthread_self());

	rc = pthread_mutex_lock(&test_mutex);
	checkResults("pthread_mutex_lock()",rc);

	printf("test_number add 1 by %lx \n",pthread_self());
	test_number++;
	printf("Test_number is %d\n",test_number);
	sleep(1);

	rc = pthread_mutex_unlock(&test_mutex);
	checkResults("pthread_mutex_unlock()",rc);
}

int main(int argc,char **argv)
{
	int rc = 0;
	pthread_t threads[THREADS];

	rc = pthread_mutex_init(&test_mutex,NULL);
	checkResults("pthread_mutex_init()",rc);

	printf("Create some  new  thread \n");
	printf("Wait for worker threads to complete, release their resources\n");
	for(int i = 0; i < THREADS;i++)
	{
		rc = pthread_create(&threads[i],NULL,theThread,NULL);
		checkResults("pthread_create()",rc);
	}

	for(int i = 0;i < THREADS;i++)
	{
		rc = pthread_join(threads[i],NULL);
		checkResults("pthread_join()",rc);
	}

	printf("Destroy pthread mutex \n");
	rc = pthread_mutex_destroy(&test_mutex);
	checkResults("pthread_mutex_destroy()",rc);
	
	printf("Main completed\n");
}
