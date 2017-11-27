#include <string.h>
#include <stdio.h>
int main(){
	char buffer[]="Hello Word";
	printf("Buffer before memset:%s\n",buffer);
	printf("the len of buffer: %zd\n",strlen(buffer));

	printf("the len of array is %zd\n",sizeof(buffer)/sizeof(buffer[0]));
	memset(buffer,'*',4);
	printf("Buffer after memset:%s \n",buffer);

	int a[5];
	int i;
	memset(a,'a',20);
	for (i=0;i<5;i++)  
	{  
		printf("%d\n",a[i]);  
	}  
	return 0;
}
