#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
void str_cli(FILE *fp,int sockfd);
#define SERVER_IP "172.16.56.102"
typedef struct sockaddr SA;
int main()
{
	int sockfd;
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	memset(&servaddr,0,sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9877);

	inet_pton(AF_INET,SERVER_IP,&servaddr.sin_addr);
	connect(sockfd,(SA *)&servaddr,sizeof(servaddr));
	str_cli(stdin,sockfd);
}
void str_cli(FILE *fp,int sockfd)
{
	char sendline[4096];
	char recvline[4096];
	int n = -1;
	while(fgets(sendline,4096,fp) != NULL)
	{
		printf("sendlien to server:%s\n",sendline);
		n = write(sockfd,sendline,strlen(sendline));
		printf("%d\n",n);
	}
}
