#include <stdio.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <netdb.h>  
#include <unistd.h>


#define MAXLINE 409

typedef struct sockaddr SA;

int main(int argc,char **argv)
{
	int sockfd,n;
	char recvline[MAXLINE+1];
	struct sockaddr_in servaddr;

	if(argc != 2)
	{
		printf("usage : a.out <IPaddress>\n");
		return -1;
	}
	
	if((sockfd == socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		printf("socket error");
		return -1;
	}

	memset(&servaddr,0,sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(22);

	if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr) <= 0)
	{
		printf("inet_pton error for %s \n",argv[1]);
		return -1;
	}

	if(connect(sockfd,(SA *)&servaddr,sizeof(servaddr)) < 0)
	{
		printf("connect error.\n");
		return -1;
	}

	while((n = read(sockfd,recvline,MAXLINE)) > 0)
	{
		recvline[n] = 0;
		if(fputs(recvline,stdout) == EOF)
		{
			printf("fputs error");
		}
	}

	if(n < 0)
	{
		printf("read error");
	}

	return 0;
}
