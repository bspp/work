#include<stdio.h>
int main()
{
	float answer = 17 / (float)13;
	printf("%.2f \n",answer);
	printf("size of float %zu \n",sizeof(float));
	printf("size of int %zu \n",sizeof(int));
	printf("size of long %zu \n",sizeof(long));
	printf("size of double %zu \n",sizeof(double));
	printf("size of long lgon %zu \n",sizeof(long long));
	return -1;
}
