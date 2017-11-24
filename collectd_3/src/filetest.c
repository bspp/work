#include <stdio.h>   
void writeLogLine(const char *fileName,const char *content)     //参数：文件全名，写入内容  
{  
	FILE *fp;                                   //定义文件指针  
	if ((fp=fopen(fileName,"a"))==NULL)         //打开指定文件，如果文件不存在则新建该文件  
	{  
		printf("Open Failed.\n");  
		return;  
	}   
	fprintf(fp,"%s\n",content);                 //格式化写入文件（追加至文件末尾）  
	fclose(fp);                                 //关闭文件  
}  

int main()   
{   
	writeLogLine("test.c","写一行内容");  
	return 0;   
}  
