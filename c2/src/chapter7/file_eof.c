#include <stdio.h>
#include <stdlib.h>//为了使用exit
int main(){
  int ch;
  FILE *fp;
  char fname[50];
  printf("Enter the name of the file:");
  scanf("%s",fname);
  fp=fopen(fname,"r");//打开带读取文件
  if(fp == NULL){
    printf("Failed to open the file.Bye\n");
    exit(1);
  }
  while((ch = getc(fp)) != EOF){
    putchar(ch);
  }
  fclose(fp);
  return 0;
}
