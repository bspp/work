/*======================================================================
* Copyright (C) 2018 Sangfor Ltd. All rights reserved.
* Programmer :Linyuanpeng
* Date       :2018/01/28
*========================================================================*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

#define checkResults(string, val) { \
	if (val) { \
		printf("Failed with %d at %s \n", val, string); \
		exit(1); \
	} \
}


