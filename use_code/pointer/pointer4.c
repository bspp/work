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
	int array[10] = {0,1,2,3,4,5,6,7,8,9};
	int value = 0;
	
	value = array[0];//也可以写成：value = *array;
	if(value == *array)
		printf("(value = array[0]) == *array\n");
	
	value = array[3];//也可以写成：value = *(array+3);
	if(value == *(array+3))
		printf("(value = array[3]) == *array\n");

	return 0;
}


