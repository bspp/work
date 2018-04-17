#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<errno.h>  
#include<unistd.h>  
#include<sys/types.h>  
#include<sys/stat.h>  
#include<fcntl.h>  
#define STR_LEN 16384//定义随机输出的字符串长度。



int main(int argc,char **argv)
{
	char *filename = "/pool1/file/test5.txt";
	char data[STR_LEN+1] = {0};//1M

	int fd = open(filename,O_RDWR);
	char buf[STR_LEN+1] = {0};
	read(fd,buf,STR_LEN);
	close(fd);
	return 0;
}


