#include<stdio.h>
#include<time.h>
typedef long TIME_T;
typedef char CHAR;
typedef void VOID;


void time_t_to_time_str(TIME_T t, CHAR *pcTime)
{
	struct tm *tm_t;
	tm_t = localtime(&t);
	strftime(pcTime,128,"%F %T",tm_t);



}
VOID metis_strptime(char *pcTime,TIME_T *ptTime)
{
	struct tm stm;
	strptime(pcTime,"%Y-%m-%d %H:%M:%S",&stm);
	*ptTime = mktime(&stm);

}

int main(){
	char *t = "2017-10-12 00:00:00";
	TIME_T t_1 = 0;
	metis_strptime(t,&t_1);
	printf("%ld\n",t_1);
	return -1; 

}

