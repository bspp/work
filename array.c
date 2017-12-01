#include<stdio.h>
#include<malloc.h>
int main()
{
	int data[10] = {0};
	data = realloc(data,11*sizeof(int));
	data[10] = 20;
}
