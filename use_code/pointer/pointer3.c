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
	char *p = (char *)a;
	char **ptr = &p;

	printf("**ptr=%c\n",**ptr);//输出为H
	
	ptr++;//&p+4,非法地址访问
	printf("**ptr=%c\n",**ptr);

	return 0;
}


