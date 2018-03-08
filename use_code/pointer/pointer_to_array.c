/*======================================================================
* Copyright (C) 2018 Sangfor Ltd. All rights reserved.
* Programmer :Linyuanpeng
* Date       :2018/03/04
*========================================================================*/

#include<stdio.h>
#include<string.h>
#include<malloc.h>

int main(void)
{
	char (*pointer_to_arr)[64] = NULL;

	pointer_to_arr = (char(*)[]) malloc(sizeof(char[64]) * 10);
	
	for(int i = 0;i < 10;i++)
	{
		sprintf(*(pointer_to_arr+i),"%s%d","array_to_pointer",i);
		printf("%s\n",*(pointer_to_arr+i));
	}

	free(pointer_to_arr);

	return 0;
}
