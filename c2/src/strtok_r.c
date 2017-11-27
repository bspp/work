#include<stdio.h>
#include<string.h>
int main()
{
	int in = 0;  
	char buffer[100] = ",John,Ann,kk,kk,ll,cc,dd";  
	char *p[3];  
	char *buf = buffer;
	char *save = NULL;
	printf("%s\n",strtok_r(buf,",",&save));
	printf("%s\n",buf);
	printf("%s\n",save);
}
