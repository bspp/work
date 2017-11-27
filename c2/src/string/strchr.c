#include<stdio.h>
#include<string.h>
int main()
{
	char *test=".lsjfsdjlfsdjflds";
	char *test1="lsjfsdjlfsdjflds.csv";
	printf("%d\n",strchr(test,'.')-test);
	printf("%d\n",strchr(test1,'.')-test1);
}
