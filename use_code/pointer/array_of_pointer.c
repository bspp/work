/*======================================================================
* Copyright (C) 2018 Sangfor Ltd. All rights reserved.
* Programmer :Linyuanpeng
* Date       :2018/03/04
* 指针数组（array of pointers）,用于存储指针的数组，也就是数组元素都是指针，本质上其实数组。
* char* a[4]:一个存放着4个char*的数组
*========================================================================*/

#include<stdio.h>
#include<string.h>
#include<malloc.h>

int main(void)
{
	char *array[4];
	for(int i = 0;i < 4 ;i++)
	{
		//array[i]其指向的地址是的一个未知的地址，很多情况下为NULL地址，
		//所以要使用array[i]指针，那么就必须为其分配地址
		array[i] = (char *)malloc(sizeof(char) * 64);
		sprintf(array[i],"%s%d","array",i);
	}

	for(int i = 0;i < 4 ;i++)
	{
		printf("%s\n",array[i]);

		//释放内存
		free(array[i]);
	}

	char *roman_basic[13] = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
	for(int i = 0;i < 13;i++)
	{
		printf("%s--%d\n",roman_basic[i],strlen(roman_basic[i]));
	}

}
