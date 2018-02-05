/*======================================================================
* Copyright (C) 2018 Sangfor Ltd. All rights reserved.
* Programmer :Linyuanpeng
* Date       :2018/02/04
*========================================================================*/

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
#include <netinet/in.h> 

#define checkResults(string) { \
	printf("Failed with %s \n",string); \
	exit(1); \
}

int main()
{
	struct sockaddr_in server_addr_in;
	struct sockaddr_in client_addr_in;
	char buf[100] = {0};
	int port_server = 8000;
	int port_client = 8001;
	int sockfd = 0;
	int status = 0;
	char *str = "test string";

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
		checkResults("scok failed");

	//该段代码为客户端指定一个地址，如果无该段代码，则客户端为任意地址
	memset(&client_addr_in,'\0',sizeof(server_addr_in));
	client_addr_in.sin_family = AF_INET;
	client_addr_in.sin_port = htons(port_client);
	inet_pton(AF_INET,"172.16.56.42",&client_addr_in.sin_addr);

	status = bind(sockfd,(struct sockaddr*)&client_addr_in,sizeof(client_addr_in));
	if(status< 0)
		checkResults("bind()");
	

	memset(&server_addr_in,'\0',sizeof(server_addr_in));
	server_addr_in.sin_family = AF_INET;
	server_addr_in.sin_port = htons(port_server);
	inet_pton(AF_INET,"172.16.56.42",&server_addr_in.sin_addr);

	connect(sockfd,(struct sockaddr*)&server_addr_in,sizeof(server_addr_in));
	while(1){
		recv(sockfd,buf,100,0);
		printf("Recive from server %s \n",buf);
	}
	
	close(sockfd);


	return 0;
}
