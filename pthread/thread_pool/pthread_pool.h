/*======================================================================
*   Copyright (C) 2018 Sangfor Ltd. All rights reserved.
*   Programmer :Linyuanpeng
* 	Date       :2018/01/23
*
========================================================================*/

#ifndef PTHREAD_POOL_H
#define PTHREAD_POOL_H

#ifdef _cplusplus
extern "C"{
#endif

#include<pthread.h>
#include<unistd.h>

/*Binary swmaphore*/
struct bseg
{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int v;
};
typedef struct bseg BSEG_S;

/* Job */
struct job {
	struct job *prev; /*pointer to previour job*/
	void (*function)(void* arg);       /* 工作的线程函数 */
	void*  arg;                          /* 工作的线程参数*/
};
typedef struct job JOB_S;

/* Job queue */
struct job_queue
{
	pthread_mutex_t rwmutex; /* used for queue r/w access*/
	JOB_S *ps_front; /* pointer to front of queue */
	JOB_S *ps_rear; /* pointer to rear of queue */
	BSEG_S *ps_has_jobs; /* flag as binary semaphore */
	int len; /* number of jobs in queue */
};
typedef struct job_queue JOB_QUEUE_S;

struct pthread_pool;
typedef struct pthread_pool PTHREAD_POOL_S;

/* Thread */
struct thread{
	int id; /* friendly id */
	pthread_t pthread;
	PTHREAD_POOL_S *ps_pthread_pool;
};
typedef struct thread THREAD_S;

struct pthread_pool
{
	THREAD_S**   pps_threads;                  /* pointer to threads        */
	volatile int num_threads_alive;
	volatile int num_threads_working;
	pthread_mutex_t thcount_lock; /* used for the thread count etc */
	pthread_cond_t  threads_all_idle; /* signal to pthread_pool_wait*/
	JOB_QUEUE_S  s_job_queue;   /* job queue */
};


#ifdef __cplusplus
}
#endif

#endif


