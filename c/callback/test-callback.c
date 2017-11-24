#include<stdio.h>
typedef void (*FP)(char* s);
void f1(char* s)
{
	printf("%s\n",s);
}
void f2(char* s)
{
	printf("--------------------------------%d\n---------------------",s);
}
int main()
{
	FP t = f1;
	t("kkkkk");
	FP t1 = f2("kkkkkkkkkkkkkkkkkk");
	t1;
}
