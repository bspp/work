/*======================================================================
* Copyright (C) 2018 Sangfor Ltd. All rights reserved.
* Programmer :Linyuanpeng
* Date       :2018/03/07
*========================================================================*/

#include<stdio.h>
#include<string.h>

int main()
{
	char s[80] = {0};
	char *str[3] = {
		"Hello,this is a simple .",
		"Hi,good moring .",
		"Hello world ."
	};

	memset(s,'\0',80);

	strcpy(s,str[0]);//也可以写成strcpy(s,*str)
	printf("s is %s and *str is %s \n",s,*str);

	memset(s,'\0',80);
	strcpy(s,str[0]);//也可以写成strcpy(s,*str)
	printf("s is %s and *str is %s \n",s,*(str+1));

	return 0;
}

