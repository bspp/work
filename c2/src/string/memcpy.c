#include <stdio.h>
#include <string.h>
int main(void){
	//void *memcpy(void*dest, const void *src, size_t n);
	//由src指向地址为起始地址的连续n个字节的数据复制到以destin指向地址为起始地址的空间内。
	char *s="Golden Global View";
	char d[20];
	memcpy(d,s,3);
	printf("%s",d);
	int i = 0;
	for(;i<sizeof(d)/sizeof(d[0]);i++){
		d[i] = '*';
		printf("%c\n",d[i]);
	}
	return 0;
}
