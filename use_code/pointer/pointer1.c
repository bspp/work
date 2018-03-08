/*======================================================================
* Copyright (C) 2018 Sangfor Ltd. All rights reserved.
* Programmer :Linyuanpeng
* Date       :2018/03/05
*
*
*========================================================================*/

#include<stdio.h>

int main()
{
	char a[32] = {"Hello-word-this-is-a-array"};
	int *ptr = (int *)a;
	ptr++;

	printf("a[4] addr %p \n",&a[4]);
	printf("a+4  addr %p \n",a+4);
	printf("ptr  addr %p \n",ptr);
	return 0;
}


