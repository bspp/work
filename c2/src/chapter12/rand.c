#include <stdio.h>
static unsigned long int next = 1;//种子
unsigned int rand0(void){
  next * 1103515245 + 12345;
  return (unsigned int)(next/65526)%332768;
}
