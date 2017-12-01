/*
 *
 * main函数参数
 ***************************************************/

#include<stdio.h>

int main(int argv,char *argc[])
{
		printf("\n");
		for(int i = 0; i < argv ; i++)
		{
			printf("i = %d --->  %s\n",i,argc[i]);
		}
}
