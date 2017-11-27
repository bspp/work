#include <stdio.h>  
#include <stdlib.h>  

#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <netdb.h>  
#include <unistd.h>
 #define SERV_PORT   3000
int main(int argc, char** argv) { 
	int sock_fd;////套接子描述符号
	int recv_num;
	int send_num;
	int client_len;
	char send_buf[1452] = {"Hello tiger  linyuanpeng"};
	send_buf[3] = 0;

	printf("%s  %zd",send_buf,strlen(send_buf));
	printf("%c--------------\n",send_buf[15]);
	struct sockaddr_in addr_serv;
	struct sockaddr_in addr_client;
	sock_fd = socket(AF_INET,SOCK_DGRAM,0);
	if(sock_fd < 0){
		perror("socket");
		exit(1);
	}else{
		  printf("sock sucessful\n");
	}
	//初始化服务器端地址
	memset(&addr_serv,0,sizeof(struct sockaddr_in));
	memset(&addr_client,0,sizeof(struct sockaddr_in));

	addr_serv.sin_family = AF_INET;//协议族
	addr_serv.sin_port = htons(8000);
	addr_serv.sin_addr.s_addr = inet_addr("172.16.24.240");

	addr_client.sin_family = AF_INET;//协议族
	addr_client.sin_port = htons(9090);
	addr_client.sin_addr.s_addr = inet_addr("172.16.56.102");


	if(bind(sock_fd,(struct sockaddr *)&addr_serv,sizeof(struct sockaddr_in))<0 ){
		perror("bind");
		exit(1);
	}else{
		printf("bind sucess \n");
	}
	int i = 1;
	int dest_len = sizeof(struct sockaddr_in);
	while(i>0){
		printf("%d\n",i);
		int m = sendto(sock_fd,send_buf,sizeof(send_buf),0,(struct sockaddr*)&addr_client,dest_len);
		i++;
		usleep(1000000);

	}
	close(sock_fd);

	

	

}  
