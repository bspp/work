#include <sys/stat.h>  
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <time.h>

#define STR_LEN 1048576//定义随机输出的字符串长度。
#define CHAR_MIN 'a'
#define CHAR_MAX 'z' //定义输出随机字符串每个字符的最大最小值。

long get_file_size(char* filename)  
{  
	struct stat statbuf;  
	stat(filename,&statbuf);  
	long size=statbuf.st_size;  

	return size;  
}



int main(int argc,char **argv)
{
	char *filename = "/pool1/file/test.txt";
	char data[STR_LEN+1] = {0};//1M
	FILE *fp = NULL;
	long file_size = 0;

	data[STR_LEN]='\0';

	srand(time(NULL));
	for(int i = 0; i < STR_LEN; i ++)
		data[i] = rand()%(CHAR_MAX-CHAR_MIN + 1) + CHAR_MIN; //生成要求范围内的随机数。

	fp = fopen(filename,"a+");
	if(fp == NULL)
	{
		printf("open file failed");
		return -1;
	}

	file_size = get_file_size(filename);
	printf("file szie is %ld\n",file_size);
	if(file_size == 0)
	{
		for(int i = 0;i < 1;i++){
			fprintf(fp,"%s",data);
		}//1M
	}
	else if(file_size/1024/1024 == 1 )
	{
		for(int i = 0;i < 1023*1024;i++){
			fprintf(fp,"%s",data);
		}
	}else if(file_size/1024/1024 == 1024)
	{
		for(int i = 0;i < 1024*1024;i++){
			fprintf(fp,"%s",data);
		}//2G
	}

	fclose(fp);
	return 0;
}


