/**
 *  Demonstrates interation with a char
 ****************************************************************************/

#include<stdio.h>

 int main()
{
	  for(char c = 'A'; c <= 'Z';c = (char)((int)c+1))
			    printf("%c: %d\n",c,(int)c);

}
~                                                            
