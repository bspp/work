#include <stdio.h>  
#include <stdlib.h>  

#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <netdb.h>  
#include <unistd.h>
#include <malloc.h>
int main(int argc, char** argv) { 
	int sock_fd;////套接子描述符号
	int addr_len = sizeof(struct sockaddr_in);
	char *buf;
	buf = (char *)malloc(sizeof(char));
	struct sockaddr_in addr_client;
	struct sockaddr_in addr_serv;
	sock_fd = socket(AF_INET,SOCK_DGRAM,0);
	if(sock_fd < 0){
		perror("socket");
		exit(1);
	}else{
		printf("sock sucessful\n");
	}
	//初始化服务器端地址
	memset(&addr_client,0,sizeof(struct sockaddr_in));
	memset(&addr_serv,0,sizeof(struct sockaddr_in));
	memset(buf,-1,sizeof(char)*1452);


	addr_client.sin_family = AF_INET;//协议族
	addr_client.sin_port = htons(9000);
	addr_client.sin_addr.s_addr = htonl(INADDR_ANY);//任意本地址

	addr_serv.sin_family = AF_INET;//协议族
	addr_serv.sin_port = htons(8000);
	addr_serv.sin_addr.s_addr =inet_addr("172.16.24.240"); ;//任意本地址

	if(bind(sock_fd,(struct sockaddr *)&addr_client,sizeof(struct sockaddr_in))<0 ){
		perror("bind");
		exit(1);
	}else{
		printf("bind sucess \n");
	}
	while(1){
		int ret=recvfrom(sock_fd,buf,1452,0 ,NULL,NULL);
		int i = 0;
		for(;i<1452;i++){
			if(buf[i] == -1){
				printf("len:%zd",i);
				break;
			}
		}
		printf("%d----\n",buf[0]);
		printf("%d----\n",buf[1]);
		printf("%d----\n",buf[2]);
		printf("%d----\n",buf[3]);
		printf("%d----\n",buf[4]);
		printf("%d----\n",buf[250]);
		printf("%d----\n",buf[257]);

		memset(buf,-1,sizeof(char)*1452);
	}
	free(buf);
	close(sock_fd);

}  
