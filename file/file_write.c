#include <sys/stat.h>  
#include<stdio.h>
#include<string.h>

long get_file_size(char* filename)  
{  
    struct stat statbuf;  
    stat(filename,&statbuf);  
    long size=statbuf.st_size;  
  
    return size;  
}

int main(int argc,char **argv)
{
	char *filename = "test.txt";
	char data[1025] = {0};
	FILE *fp = NULL;
	long file_size = 0;

	file_size = get_file_size(filename);
	memset(data,'d',1024);

	if(file_size/1024/1024/1024 == 2)
	{
		remove(filename);
	}

	fp = fopen(filename,"a+");
	if(fp == NULL)
	{
		printf("open file failed");
		return -1;
	}

	printf("filezie is %ld\n",file_size);
	if(file_size == 0)
	{
		for(int i = 0;i < 1024;i++)//1M
			fprintf(fp,"%s",data);
	}else if(file_size == 1048576)
	{
		for(int i = 0;i < 1047552;i++)//1G
			fprintf(fp,"%s",data);
	}else if(file_size/1024 == 1048576)
	{
		for(int i = 0;i < 1048576;i++)//1G
			fprintf(fp,"%s",data);
	}

	fclose(fp);
	return 0;
}


