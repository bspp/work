/*======================================================================
* Copyright (C) 2018 Sangfor Ltd. All rights reserved.
* Programmer :Linyuanpeng
* Date       :2018/03/05
*========================================================================*/

#include<stdio.h>
#include<string.h>
#include<malloc.h>


int main()
{
	char **pointer_to_pointer = NULL;
	pointer_to_pointer = (char **)malloc(10 * sizeof(char *));
	printf("%p\n",pointer_to_pointer);

	for(int i = 0;i < 10;i++)
	{
		pointer_to_pointer[i] = (char *)malloc(64 * sizeof(char));
		sprintf(pointer_to_pointer[i],"%s%d\n","array",i);
		printf("%s\n",pointer_to_pointer[i]);
		printf("pointer_to_pointer[i] addr  %p\n",pointer_to_pointer[i]);
		printf("&pointer_to_pointer[i] addr  %p\n",&pointer_to_pointer[i]);

	  printf("pointer_to_pointer+i-->%p\n",pointer_to_pointer+i);
		printf("*(pointer_to_pointer+i)   %s\n",*(pointer_to_pointer+i));
		printf("\n");
	}

	for(int i = 0; i < 10 ;i++)
		free(pointer_to_pointer[i]);
	free(pointer_to_pointer);

	return 0;

}
