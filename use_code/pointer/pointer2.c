/*======================================================================
* Copyright (C) 2018 Sangfor Ltd. All rights reserved.
* Programmer :Linyuanpeng
* Date       :2018/03/05
* 用一个指针和一个循环来遍历一个数组
*========================================================================*/

#include<stdio.h>
int main()
{
	int array[10] = {0};
	int *ptr = (int *)array;
	
	//将数组里面的每个值都加1
	for(int i = 0;i < 10;i++)
	{
		(*ptr)++;
		*ptr++;
	}

	for(int i = 0;i < 10;i++)
		printf("array[%d] %d \n",i,array[i]);

	return 0;
}


