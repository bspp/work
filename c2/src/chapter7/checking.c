#include <stdio.h>
#include <stdbool.h>
//验证输入是一个整数
long get_long(void);
//bool bad_limits(long begin,long end,long low,long high);
//double sum_squares(long a,long b);
long get_long(void){
  long input;
  char ch;
  while(scanf("%ld",&input) != 1){
    while((ch = getchar()) != '\n'){
      putchar(ch);
    }
    printf(" is not an integer.\nPlease enter an ");
    printf("integer value, such as 25, -178, or 3: ");
  }
  return input;
}
int main(void){
  get_long();
  printf("%ld",get_long());
  return 0;
}
