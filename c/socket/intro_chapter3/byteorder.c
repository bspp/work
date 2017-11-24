#include<stdio.h>

typedef union un UN_S;
int main()
{
	union{
		short s;
		char c[sizeof(short)];
	}un;
	un.s = 0x0210;//十进制258 二进制 0000 0002 0000 0001
	printf("%d\n",un.s);
	printf("size of short:%zu\n",sizeof(short));
	printf("c[0]: %d\n",un.c[0]);
	printf("c[1]: %d\n",un.c[1]);

	if(sizeof(un.c) == 2)
	{
		if(un.c[0] == 2 && un.c[1] == 16)
			printf("big-endian\n");
		else if(un.c[0] == 16 && un.c[1] == 2)
			printf("little_endian\n");
		else
			printf("unknown.\n");
	}
}

//0x 0210 
//0000 0002 0000 0010 那么其位大端排序
//0000 0010 0000 0020 小端排序 
//include<netinet/in.h>
//uint16_t htons(uint16_t host16bitvalue) host to network short 返回网络字节序
//uint16_t ntohs(uint16_t net16bitvalue) network to host short 返回主机字节序
//uint32_t htonl(uint32_t host32bitvalue) host to network long 
//uint32_t ntohl(uint32_t net32bitvalue) network to hsot long
//注：把s看成一个16位的值
//注：把l看成一个32位的值
//网络协议建议使用大端字节序来传送这些（16位端口号和32位ipv4地址）多字节整数
