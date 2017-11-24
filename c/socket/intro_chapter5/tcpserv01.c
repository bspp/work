#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
typedef struct sockaddr SA;
#define SERVER_IP "172.16.56.102"
void str_echo(int sockfd);
int main()
{
	int sockfd = -1;
	int connfd = -1;
	pid_t childpid;//pid_t 进程号
	socklen_t chilen;
	struct sockaddr_in cliaddr,servaddr;

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	memset(&servaddr,0,sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9877);
	servaddr.sin_addr.s_addr =  inet_addr(SERVER_IP);

	if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1)
	{
		printf("BIND FAIL \n");
		return -1;
	}

	if(-1 == (listen(sockfd,5)))
	{
		printf("Listten error.\n");
		return -1;
	}

	while(1)
	{
		chilen = sizeof(cliaddr);
		connfd = accept(sockfd,(SA *)&cliaddr,&chilen);
		if((childpid == fork()) == 0)
		{
			close(sockfd);
			str_echo(connfd);
			exit(0);
		}
		close(connfd);
	}
}
void str_echo(int sockfd)
{
	ssize_t n = 0;
	char buf[4096];
again:
	while((n = read(sockfd,buf,4096)) > 0)
	{
		printf("From Clinet :%s",buf);
		write(sockfd,buf,n);
	}
	if(n < 0 && errno == EINTR)
		goto again;
	else if(n < 0){
		printf("STR ECHO ERROR\n");
	}
}

