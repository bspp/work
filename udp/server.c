/*======================================================================
 * Copyright (C) 2018 Sangfor Ltd. All rights reserved.
 * Programmer :Linyuanpeng
 * Date       :2018/02/05
 *========================================================================*/

#include<stdio.h>
#include<stdlib.h>
#include <sys/socket.h>  
#include <arpa/inet.h> 
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>


#define checkResults(string) { \
	printf("Failed with %s \n",string); \
	perror(string);\
	exit(1); \
}

#define SERVER_PORT 7012
#define CLIENT_PORT 7013

#define MAX_MESSAGE_LEN 1024

int main()
{
	int sockfd = 0;
	int status = 0;
	struct sockaddr_in server_addr = {0};
	struct sockaddr_in client_addr = {0};
	int recv_message_len = 0;
	int send_message_len = 0;
	char message_from_client[MAX_MESSAGE_LEN];
	char message_to_client[MAX_MESSAGE_LEN] = "This is message from server.";
	socklen_t client_len = 0;


	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(socket < 0)
		checkResults("socket()");

	memset(&server_addr,'\0',sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr("172.16.56.42");

	memset(&client_addr,'\0',sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(CLIENT_PORT);
	client_addr.sin_addr.s_addr = inet_addr("172.16.56.42");

	status = bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(status < 0)
		checkResults("bind()");

	client_len = sizeof(client_addr);

	while(1)
	{
		send_message_len = sendto(sockfd,message_to_client,strlen(message_to_client),0,(struct sockaddr*)&client_addr,client_len);
		if(send_message_len > 0)
		{
			printf("send message client success \n");
		}

		memset(message_from_client,'\0',MAX_MESSAGE_LEN);
		recv_message_len = recvfrom(sockfd,message_from_client,MAX_MESSAGE_LEN,0,(struct sockaddr *)&client_addr,&client_len);
		if(recv_message_len > 0)
			printf("Message from client is %s ,and message len is %d\n",message_from_client,recv_message_len);
	}
}
