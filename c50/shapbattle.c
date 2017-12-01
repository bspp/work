/*
 *双重循环
 * ********************************************************/

/**/
#include<stdio.h>

int main()
{
	printf("    ");

	for(int i = 0; i < 10;i++)
		printf("%2d  ",i+1);
	printf("\n");

	for(int i ='A'; i < 'A'+ 10;i++)
	{
		printf("%c   ",i);

		for(int j = 0;j < 10;j++)
			printf(" *  ");

		printf("\n");

	}

	return 0;


}

