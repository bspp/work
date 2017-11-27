#include<stdio.h>
#include<unistd.h>
int main()
{
	for(int i = 0; i <= 100; i++)
	{
		printf("FOR NUMBER %3d%",i);
		sleep(10);
	}

	return -1;
}
