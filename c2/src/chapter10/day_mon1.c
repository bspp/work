#include <stdio.h>
#define MONTHS 12
int main(void){
  int day[MONTHS] = {
    31,28,31,30,31,30,31,31,30,31,30,31
  };
  for(int i = 0; i < MONTHS; i++){
    printf("Month %3d has %3d day\n",i+1,day[i]);
  }
}
