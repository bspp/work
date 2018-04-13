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
	char *filename = "/pool1/file/test.txt";
	char data[1025] = {0};//1M
	FILE *fp = NULL;
	long file_size = 0;
	char c = 'a';
	data[1024]='\0';




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
		for(int i = 0;i < 1024;i++){
			fprintf(fp,"%s",data);
			memset(data,(c+i%26),1024);
		}//1M
	}
	else if(file_size/1024/1024 == 1 )
	{
		for(int i = 0;i < 1023*1024;i++){
			fprintf(fp,"%s",data);
			memset(data,(c+i%26),1024);
		}
	}else if(file_size/1024/1024 == 1024)
	{
		for(int i = 0;i < 1024*1024;i++){
			fprintf(fp,"%s",data);
			memset(data,(c+i%26),1024);
		}//2G
	}

	fclose(fp);
	return 0;
}


