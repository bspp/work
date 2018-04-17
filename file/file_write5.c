#include <sys/stat.h>  
#include<stdio.h>
#include<string.h>

#define STR_LEN 1048576

long get_file_size(char* filename)  
{  
    struct stat statbuf;  
    stat(filename,&statbuf);  
    long size=statbuf.st_size;  
  
    return size;  
}

int main(int argc,char **argv)
{
	char *filename = "/pool1/file/test5.txt";
	char data[STR_LEN] = {0};//1M
	FILE *fp = NULL;
	long file_size = 0;
	char c = 'a';
	data[STR_LEN]='\0';

	memset(data,'a',STR_LEN);

	fp = fopen(filename,"a+");
	if(fp == NULL)
	{
		printf("open file failed");
		return -1;
	}

	file_size = get_file_size(filename);
	printf("file szie is %ld\n",file_size);
	if(1)
	{
		for(int i = 0;i < 128;i++){
			fprintf(fp,"%s",data);
		}//1M
	}
	else if(file_size/1024/1024 == 1 )
	{
		for(int i = 0;i < 31;i++){
			fprintf(fp,"%s",data);
		}
	}else if(file_size/1024/1024 == 1024)
	{
		for(int i = 0;i < 1024;i++){
			fprintf(fp,"%s",data);
		}//2G
	}

	fclose(fp);
	return 0;
}


