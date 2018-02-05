/*======================================================================
* Copyright (C) 2018 Sangfor Ltd. All rights reserved.
* Programmer :Linyuanpeng
* Date       :2018/02/01
*========================================================================*/

#include <sys/socket.h>  
#include <arpa/inet.h>  


#define MAX_BUFFER_LEN 1024

int main(int argc,char** argv)
{
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int listenfd = 0;
	int connfd = 0;

	listenfd = socket(AF_INET,SOCK_STREAM,0);
	printf("%d\n",listenfd);
	return 0;
	
}

