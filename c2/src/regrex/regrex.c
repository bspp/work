#include<stdio.h>                                                                                                        
#include<sys/types.h>
#include<regex.h>
#include<memory.h>
#include<stdlib.h>

int main(){

	char *bematch = "2345643454";
	char *pattern = "[0-9]{10}";
	char errbuf[1024];
	char match[100];
	regex_t reg;
	int err,nm = 10;
	regmatch_t pmatch[nm];

	if(regcomp(&reg,pattern,REG_EXTENDED) < 0){
		regerror(err,&reg,errbuf,sizeof(errbuf));
		printf("err:%s\n",errbuf);

	}

	err = regexec(&reg,bematch,nm,pmatch,0);

	if(err == REG_NOMATCH){
		printf("no match\n");
		exit(-1);


	}else if(err){
		regerror(err,&reg,errbuf,sizeof(errbuf));
		printf("err:%s\n",errbuf);
		exit(-1);

	}

	for(int i=0;i<10 && pmatch[i].rm_so!=-1;i++){
		int len = pmatch[i].rm_eo-pmatch[i].rm_so;
		if(len){
			memset(match,'\0',sizeof(match));
			memcpy(match,bematch+pmatch[i].rm_so,len);
			printf("%s\n",match);

		}

	}
	return 0;

}
char *sstrncpy(char *dest, const char *src, size_t n) { 
  strncpy(dest, src, n); 
  dest[n - 1] = '\0';

  return dest;
} 
time_t get_file_create_time(const char *filename)
{
	char *bematch;
	char *pattern = "0-9{10}";
	char errbuf[1024];
	int status = 0;
	int len = 0;
	char time_str[11];
	time_t time;
	regmatch_t pmatch[1];
	regex_t reg;

	sstrncpy(bematch,filename,strlen(filename));

	if(regcomp(&reg,pattern,REG_EXTENDED) < 0)
	{   
		regerror(0,&reg,errbuf,sizeof(errbuf));
		printf("err:%s\n",errbuf);
		return -1; 
	}

	status = regexec(&reg,bematch,1,pmatch,0);
	if(status == REG_NOMATCH)
	{
		printf("get file create time err");
		return -1; 

	}

	len = pmatch[0].rm_eo-pmatch[0].rm_so;
	if(len == 10) 
	{
		memset(time_str,'\0',sizeof(time_str));
		memcpy(time_str,bematch+pmatch[0].rm_so,len);
		time = strtol(time_str,NULL,10);

	}
	return time;

}

