#include<stdio.h>
#include<time.h>
#include<stdint.h>
#include<string.h>
#define CDTIME_T_TO_TIME_T(t)                                                  \
	(time_t) { (time_t)(((t) + (1 << 29)) >> 30)  }
char *sstrncpy(char *dest, const char *src, size_t n) { 
  strncpy(dest, src, n); 
  dest[n - 1] = '\0';

  return dest;
} 


int main(){
	uint64_t cd_time = 1616389058991226880;
	time_t now = CDTIME_T_TO_TIME_T(cd_time);
	time_t now2;
	time(&now2);
	struct tm *tm_now;
	tm_now = localtime(&now);
	char time_str[128];

	sprintf(time_str,"%d-%d-%d %d:%d:%d",tm_now->tm_year+1900, tm_now->tm_mon+1, tm_now->tm_mday, tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
	memset(time_str,'\0',128);
	strftime(time_str,128,"%F %T",tm_now);
	printf("time_str %s\n",time_str);

	
}
