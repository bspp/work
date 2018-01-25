/*======================================================================
*   Copyright (C) 2018 Sangfor Ltd. All rights reserved.
*   Programmer :Linyuanpeng
* 	Date       :2018/01/08
*
========================================================================*/

#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include <unistd.h>
#include<string.h>

struct test_struct
{
	int i;
	double j;
};
typedef struct test_struct test_struct_s;
int main()
{
	int fd = 0;
	int size = 0;
	int buffer[sizeof(test_struct_s)];

	test_struct_s write_s = {0};
	write_s.i = 10;
	write_s.j = 10.88;

	fd = open("test.txt",O_WRONLY|O_CREAT);
	printf("The fd is %d \n",fd);
	write(fd,&write_s,sizeof(write_s));
	close(fd);

	//test_struct_s read_s= {0};
	//fd = open("test.txt",O_RDONLY|O_CREAT);
	//printf("The fd is %d \n",fd);
	//read(fd,buffer,sizeof(read_s));
	//close(fd);
	//printf("The i value is %s",buffer);

	//char *s_write = "Linux Programmer!\n";
	//char buffer[80];
	//fd = open("test.txt",O_WRONLY|O_CREAT);
	//printf("The fd is %d \n",fd);
	//write(fd,s_write,strlen(s_write));
	//close(fd);

	fd = open("test.txt",O_RDONLY|O_CREAT);
	read(fd,buffer,sizeof(test_struct_s));
	close(fd);

	printf("%s",buffer);

}
