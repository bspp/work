#include <stdio.h>
#include <stdlib.h>
int main(){
	char buffer[20]="1505723380";
	char *stop;
	printf("%ld\n",strtol(buffer, &stop, 10));
	printf("%s\n", stop);
}
