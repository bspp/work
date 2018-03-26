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
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int listenfd = 0;
	int connfd = 0;
	int addr_len = 0;
	int n = 0;
	char buf[100] = {0};
	char str[100] = {0};

	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd < 0)
		checkResults("socket failed")

	memset(&server_addr,'\0',sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);//Host to Network short
	server_addr.sin_addr.s_addr = htonl(inet_addr("172.16.56.42"));//Host to NetWork Long
	server_addr.sin_addr.s_addr = inet_addr("172.16.56.42");//Host to NetWork Long

	bind(listenfd,(struct sockaddr *)&server_addr,16);
	listen(listenfd,MAX_TCP_CONN_NUM);

	printf("Accepting connections ...\n");
	connfd = accept(listenfd,(struct sockaddr*)&client_addr,&addr_len);

	while(1)
	{

		//n = read(listenfd,buf,100)
		n = recv(connfd,buf,100,0);

		printf("Reveive from %s : %d \n",inet_ntop(AF_INET,&client_addr.sin_addr,str,sizeof(str)),ntohs(client_addr.sin_port));
		printf("buf %s\n",buf);
		memset(buf,'\0',strlen(buf));
		for(int i = 0; i < n;i++)
		{
			buf[i] = toupper(buf[i]);
		}

		//write( connfd, buf, n+1  );  
		//send( connfd,buf,n+1,0); 
		//printf("Send : %s \n",buf);
	}

	close(connfd);
	close(listenfd);

	return 0;

}

