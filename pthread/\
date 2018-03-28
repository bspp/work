#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define checkResults(string, val) { \
	if (val) { \
		printf("Failed with %d at %s \n", val, string); \
		exit(1); \
	} \
}

void *theThread(void *parm)
{
	printf("Entered the thread \n");
}

int main(int argc,char **argv)
{
	pthread_attr_t attr = {0};
	pthread_t thread = {0};
	int rc = 0;

	if(argc < 1)
	{
		printf("The args is erros \n");
		return -1;
	}

	printf("Enter Testcase - %s\n", argv[0]);

	printf("Create a default thread attributes object \n");
	rc = pthread_attr_init(&attr);
	checkResults("pthread_attr_init()\n",rc);

	printf("Set the detach state thread attribute\n");
	rc = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	checkResults("pthread_attr_setdetachstate()\n", rc);

	printf("Create a thread using the new attributes\n");
	rc = pthread_create(&thread, &attr,theThread, NULL);
	checkResults("pthread_create()\n", rc);

	printf("Destroy thread attributes object\n");
	rc = pthread_attr_destroy(&attr);
	checkResults("pthread_attr_destroy()\n", rc);

	printf("Join now fails because the detach state attribute was changed\n");
	rc = pthread_join(thread,NULL);
	if(rc == 0)
	{
		printf("Unexpected results from pthread_join()\n");
		exit(1);
	}

	sleep(2);
	printf("Main completed\n");
	return 0;

	return 0;
}
