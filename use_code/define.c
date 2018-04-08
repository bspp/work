#include<stdio.h>

#define P2PHASE(x, align)((x) & ((align) - 1))
int main()
{
  int m = P2PHASE(56,46);
	printf("%d\n",m);

	int k = 56 & 45;
	printf("k value is %d\n",k);
}
