/*======================================================================
 * Copyright (C) 2018 Sangfor Ltd. All rights reserved.
 * Programmer :Linyuanpeng
 * Date       :2018/03/02
 * 分解字符串
 *========================================================================*/
#include<stdio.h>
#include<string.h>

static void sstrtok(char *str);
int main()
{
	char pcStr[] = "kk1,kk2,kk3";
	sstrtok(pcStr);

}

static void sstrtok(char *str)
{
	char *saveptr = NULL;
	char *result = NULL;
	char *dumy = str;

	while(1)
	{
		if((result = strtok_r(dumy,",",&saveptr))== NULL)
			break;
		dumy = NULL;
		printf("result is %s\n",result);
	}
}
