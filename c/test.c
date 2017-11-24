#include<stdio.h>
#include<string.h>
char *sstrncpy(char *dest, const char *src, size_t n) {
	strncpy(dest, src, n); 
	dest[n - 1] = '\0';
	return dest;
}

int main()
{
	char *p[2];
	char *kk = "HEllo";
	sstrncpy(p[0],kk,strlen(kk)+1);
	printf("%s\n",p[0]);
	return -1;
}
