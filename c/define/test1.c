#include<stdio.h>
#include<string.h>

#define cqldbj_add_filter_long(base, key, value)   \
	do {    \
		char acValue[34] = {0};\
		sprintf(acValue,"%ld",value);\
	    if (base[0] == 0)    \
	        strcat(base, "?");  \
	    else    \
	        strcat(base, "&");  \
	    strcat(base, key);  \
			strcat(base,"="); \
	    strcat(base, acValue);    \
	}while(0)

int main()
{
	char acFilter[1024] = {0};
	char *key = "kk";
	cqldbj_add_filter_long(acFilter,"kk",(long)88);
	cqldbj_add_filter_long(acFilter,"kk",(long)20);
	printf("%s\n",acFilter);
	return -1;
}
