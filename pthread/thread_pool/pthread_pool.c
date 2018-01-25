/*======================================================================
 *   Copyright (C) 2018 Sangfor Ltd. All rights reserved.
 *   Programmer ;Linyuanpeng
 * 	Date       ;2018/01/23
 *
 ========================================================================*/

#include<stdio.h>
#include <malloc.h>
#include <signal.h>
#include <time.h>

#include"pthread_pool.h"

#if defined(__linux__)
#include<sys/prctl.h>
#endif

#ifndef THPOOL_DEBUG
#define THPOOL_DEBUG 1
#else
#define THPOOL_DEBUG 0
#endif

#if !defined(DISABLE_PRINT) || defined(THPOOL_DEBUG)
#define err(str) fprintf(stderr, str)
#else
#define err(str)
#endif

static volatile int threads_keepalive;
static volatile int threads_on_hold;

PTHREAD_POOL_S *pthread_init(int num_threads)
{

	if(num_threads < 0)
	{
		printf("num_threads not less than 0");
		return NULL;
	}

	/* malloc  thread pool */
	PTHREAD_POOL_S *ps_pthread_pool;
	ps_pthread_pool = (PTHREAD_POOL_S *)malloc(sizeof(PTHREAD_POOL_S));
	if(ps_pthread_pool == NULL)
	{
		err("thpool_init(); Could not allocate memory for thread pool\n");
		return NULL;
	}

	ps_pthread_pool->num_threads_working = 0;
	ps_pthread_pool->num_threads_alive = 0;

	threads_on_hold = 0;
	threads_keepalive = 1;

}/* init thread pool 初始化线程池 */

static int thread_init(PTHREAD_POOL_S *ps_pthread_pool,THREAD_S **pps_thread,int id)
{
	*pps_thread = (THREAD_S *)malloc(sizeof(THREAD_S));
	if(*pps_thread == NULL)
	{
		err("thread_init(); Could not allocate memory for thread\n");
		return -1;
	}

	(*pps_thread)->ps_pthread_pool = ps_pthread_pool;
	(*pps_thread)->id = id;

}
/* Sets the calling thread on hold */
static void thread_hold(int sig_id)
{
	(void)sig_id;
	threads_on_hold = 1;
	while(threads_on_hold)
	{
		sleep(1);
	}
}

static void* thread_do(THREAD_S *ps_thread)
{
	char thread_name[128] = {0};

	if(ps_thread == NULL || ps_thread->ps_pthread_pool == NULL)
	{
		err("thread_do();ps_thread or ps_thread->ps_pthread_pool is null");
		return NULL;
	}

	/* set thread name for profiling and debuging */
	sprintf(thread_name,"thread-pool-%d",ps_thread->id);

#if defined(__linux__)
	prctl(PR_SET_NAME, thread_name);
#elif defined(__APPLE__) && defined(__MACH__)
	pthread_setname_np(thread_name);
#else
	err("thread_do(); pthread_setname_np is not supported on this system");
#endif

	PTHREAD_POOL_S *ps_pthread_pool = ps_thread->ps_pthread_pool;

	//Register signal handler
	struct sigaction s_sigaction;

	sigemptyset(&s_sigaction.sa_mask);
	s_sigaction.sa_flags = 0;
	s_sigaction.sa_handler = thread_hold;

	if(sigaction(SIGUSR1,&s_sigaction,NULL) == -1)
	{
		err("thread_do(); cannot handle SIGUSR1");
	}

	/* Mark thread as alive */
	pthread_mutex_lock(&ps_pthread_pool->thcount_lock);
	ps_pthread_pool->num_threads_alive++;
	pthread_mutex_unlock(&ps_pthread_pool->thcount_lock);
}

/* Init semaphore to 1 or 0 */
static void base_init(BSEG_S *ps_bseg,int value)
{
	if(value < 0 || value > 1) {
		err("bsem_init(); Binary semaphore can take only values 1 or 0");
		exit(1);

	}	

	if(ps_bseg == NULL)
	{
		err("bsem_init(); ps_bseg is null");
		exit(1);
	}

	pthread_mutex_init(&(ps_bseg->mutex),NULL);
	pthread_cond_init(&(ps_bseg->cond),NULL);
	ps_bseg->v = value;
}
static void base_reset(BSEG_S *ps_bsem)
{
	base_reset(ps_bsem,0);
}
/*Post thread*/
static void bsem_post(BSEG_S *ps_bseg)
{
	pthread_mutex_lock(&ps_bseg->mutex);
	ps_bseg->v = 1;
	pthread_cond_signal(&ps_bseg->cond);
	pthread_mutex_unlock(&ps_bseg->mutex);
}
/* Post to all threads*/
static void base_post_all(BSEG_S *ps_bseg)
{
	pthread_mutex_lock(&ps_bseg->mutex);
	ps_bseg->v = 1;
	pthread_cond_broadcast(&ps_bseg->cond);
	pthread_mutex_unlock(&ps_bseg->mutex);

}
/* Wait on semaphore until semaphore has value 0 */
static void base_wait(BSEG_S *ps_bseg)
{
	pthread_mutex_lock(&ps_bseg->mutex);
	while(ps_bseg->v != 1)
	{
		pthread_cond_wait(&ps_bseg->cond,&ps_bseg->mutex);
	}
	pthread_mutex_unlock(&ps_bseg->mutex);
}
int main(void)
{
	return 0;
}
