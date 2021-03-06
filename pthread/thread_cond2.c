/*======================================================================
 *   Copyright (C) 2017 Sangfor Ltd. All rights reserved.
 *   Programmer :Linyuanpeng
 * 	Date       :2017/12/27
 * 	条件变量是线程同步的一种手段。条件变量用来自动阻塞一个线程，
 * 	直到条件(predicate)满足被触发为止。通常情况下条件变量和互斥锁同时使用。
 *
 ========================================================================*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<malloc.h>
#include<time.h>


#define checkResults(string, val) { \
	if (val) { \
		printf("Failed with %d at %s \n", val, string); \
		exit(1); \
	} \
}

int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *creator(void *arg)
{
	int i = 0;
	while(i < 10)
	{
		i++;
		sleep(1);


		pthread_mutex_lock(&mutex);

		count++;

		printf("In Creator count is : %d \n",count);

		if(count > 0)
		{
			printf("pthread_cond_signal\n");
			pthread_cond_signal(&cond);
		}

		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

void *consumer(void *arg)
{
	int i = 0;
	while(i < 10)
	{
		i++;

		pthread_mutex_lock(&mutex);

		if(count <= 0)
		{
			printf("Begin wait \n");
			pthread_cond_wait(&cond,&mutex);
			printf("End wait \n");
		}

		count--;
		printf("int consumer count is %d \n",count);
		pthread_mutex_unlock(&mutex);

	}
	return NULL;
}

int main(void)
{
	pthread_t create_thread;
	pthread_t consumer_thread;

	pthread_create(&consumer_thread,NULL,consumer,NULL);
	sleep(2);
	pthread_create(&create_thread,NULL,creator,NULL);

	pthread_join(create_thread,NULL);
	pthread_join(consumer_thread,NULL);

  return 0;
}




