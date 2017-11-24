#include<stdio.h>
static void calc(int array[],int len)
{   int i = 0;
		for(;i<len;i++)
		{
			array[i] += 10;
		}
}
int main()
{
	int array[10] = {1,2,3,4,5,6,7,8,9,10};
	calc(array,10);
	printf("%d\n",array[1]);
}
