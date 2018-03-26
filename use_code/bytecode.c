/*======================================================================
* Copyright (C) 2018 Sangfor Ltd. All rights reserved.
* Programmer :Linyuanpeng
* Date       :2018/03/13
*========================================================================*/

#include<stdio.h>
#include<stdint.h>
#include<limits.h>
#include<arpa/inet.h>

int main()
{
	int value = 1234567;
	// 00000000 00010010 11010110 10000111 
  //  0       18         214        135
	uint8_t *ui_value = (uint8_t *)&value;
	
	for(int i = 0;i < 4;i++)
	{
		if(i == 0)
			printf("%p 是低地址 %d 是低位\n",ui_value+i,ui_value[i]);
		printf("%p %d \n",ui_value+i,ui_value[i]);
	}

	value = htons(value);
	printf("%d\n",value);

	
	return 0;
}

