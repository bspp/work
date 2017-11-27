#include<stdio.h>
#include<time.h>
int main()  
{  
  time_t time_utc = 0;
  struct tm *p_tm_time;
  int time_zone = 0;

  p_tm_time = localtime( &time_utc   );   //转成当地时间
  time_zone = ( p_tm_time->tm_hour > 12   ) ?   ( p_tm_time->tm_hour-=  23   )  :  p_tm_time->tm_hour;
  printf("%d\n",time_zone);
}  
                                         
