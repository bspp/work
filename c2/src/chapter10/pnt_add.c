#include <stdio.h>
#define SIZE 4
int main(void){
  short dates[SIZE];
  short *pti;
  double bills[SIZE];
  double *ptf;
  pti = dates;
  ptf = bills;
  for(int i= 0; i< SIZE;i++){
    printf("pointers + %d：+%10p--%10p\n",i,pti+i,ptf+i);
  }
  return 0;
}
