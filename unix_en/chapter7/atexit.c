/**
 * 一个进程可以登记至多32个函数，这些函数将由exit自动调用。称这些函数
 * 为终止处理程序（exit handle）,并用atexit 函数登记
 * #include<stdlib.h>
 * int atexit(void (*func)(void));
 *
 *********************************************************************/
#include<stdio.h>
#include<stdlib.h>

static void my_exit1(void);
static void my_exit2(void);

int main()
{
	if(atexit(my_exit2) != 0)
	{
		printf("can't register my_exit2");
		return 1;
	}

	if(atexit(my_exit1) != 0)
	{
		printf("can't register my_exit1");
		return 1;
	}
	
	if(atexit(my_exit1) != 0)
	{
		printf("can't register my_exit1");
		return 1;
	}

	printf("main is done. \n");

	return 0;
}

static void my_exit1(void)
{
	printf("first exit handler.\n");
}

static void my_exit2(void)
{
	printf("second exit handler.\n");
}
