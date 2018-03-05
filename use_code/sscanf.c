/*======================================================================
 * Copyright (C) 2018 Sangfor Ltd. All rights reserved.
 * Programmer :Linyuanpeng
 * Date       :2018/03/04
 *========================================================================*/

#include <stdio.h>
int main(void)
{
	char str[100] ="123568;qwerSDDAE";
	char lowercase[100] = {0};
	char lowercase2[100] = {0};

	sscanf(str,"%s;%s", lowercase2, lowercase);
	printf("The lowercase2 is: %s.\n", lowercase2);
	printf("The lowercase is: %s. \n", lowercase);
	return 0;

}
