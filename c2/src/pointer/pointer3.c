#include<stdio.h>
int main(){
	int a = 12;
	int b;
	int *p;
	int **ptr;
	p = &a;//&a 的结果是一个指针，类型是int*,指向的类型是int,指向的地址是a的地址
	printf("p value is %p,p pointer value is %d \n",p,*p);
	*p = 24;//*p的结果，在这里它的类型是int,它所占用的地址是p所指向的地址，显然，*p就是变量a.
	

	return 0;
}
