/*======================================================================
 * Copyright (C) 2018 Sangfor Ltd. All rights reserved.
 * Programmer :Linyuanpeng
 * Date       :2018/02/01
 *========================================================================*/

#include<stdio.h>
#include<stdlib.h>
#include <sys/socket.h>  
#include <arpa/inet.h> 
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_BUFFER_LEN 1024
#define MAX_TCP_CONN_NUM 20


#define checkResults(string) { \
	printf("Failed with %s \n",string); \
	exit(1); \
}


int main(int argc,char** argv)
{
	uint32_t i = 9;
	printf("%d\n",i);
	printf("%d\n",htonl(i));
	uint32_t m = htonl(i);
	printf("htonl(m) %d \n",htonl(m));

	uint32_t addr = inet_addr("172.16.56.59");
	uint32_t addr2 = htonl(addr);
	uint32_t addr3 = htonl(addr2);
	printf("addr   %ld\n",addr);
	printf("addr2  %ld\n",addr2);
	printf("addr3  %ld\n",addr3);

	return 0;
}

