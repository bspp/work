#include<stdio.h>
#include"cs50.h"
int main()
{
	printf("Give me on interger between 1 and 10 :");
	int n = GetInt();
  switch(n)
	{
		case 1:
		case 2:
		case 3:
			printf("You picked a small number\n");
			break;
		case 4:
		case 5:
		case 6:
			printf("YOu picked a meduium number.\n");
			break;
		case 7:
		case 8:
		case 9:
			printf("You picked a big number\n");
			break;
		default:
			printf("yout get a big big number\n");
			break;
	}
}
