#include <stdio.h>
#include <ctype.h>//包含isalpha()的函数原型
int main(void){
  char ch;
  while((ch = getchar()) != '\n'){
    if(isalpha(ch)){
      putchar(ch+1);//是否是一个字符
    }else{
      putchar(ch);
    }
  }
  putchar(ch);
}
