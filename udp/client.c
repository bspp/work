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
#define SERVER_IP "172.16.56.42"
#define CLIENT_IP "172.16.56.42"

int main()
{
	int sockfd = 0;
	int status = 0;
	struct sockaddr_in server_addr = {0};
	struct sockaddr_in client_addr = {0};
	char message_to_server[MAX_MESSAGE_LEN] = "This is message from client .";
	char message_from_server[MAX_MESSAGE_LEN];
	int recv_message_len = 0;
	int send_message_len = 0;
	socklen_t server_len;

	memset(&server_addr,'\0',sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0)
		checkResults("socket()");

	memset(&client_addr,'\0',sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(CLIENT_PORT);
	client_addr.sin_addr.s_addr = inet_addr(CLIENT_IP);

	status = bind(sockfd,(struct sockaddr*)&client_addr,sizeof(client_addr));
	if(status < 0)
		checkResults("bind()");

	server_len = sizeof(server_addr);

	while(1)
	{
		memset(message_from_server,'\0',MAX_MESSAGE_LEN);
		recv_message_len = recvfrom(sockfd,message_from_server,MAX_MESSAGE_LEN,0,
				(struct sockaddr *)&server_addr,&server_len);

		if(recv_message_len > 0)
			printf("Messsage from server is %s and len is %d \n",message_from_server,recv_message_len);

		sleep(1);

		send_message_len = sendto(sockfd,message_to_server,strlen(message_to_server),0,
				(struct sockaddr *)&server_addr,server_len);
		if(send_message_len > 0)
			printf("send sucess \n");
	}
}
