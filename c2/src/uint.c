#include<stdio.h>
#include<stdint.h>
int main(){
	uint64_t a = 100;
	printf("%lu\n",a);
	printf("%zu\n",sizeof(uint64_t));
	printf("%zu\n",sizeof(long));
	printf("%zu\n",sizeof(long long));
	return 0;
}
