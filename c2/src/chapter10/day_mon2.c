#include <stdio.h>
#define MONTHS 12
int main(void){
  int days[] = {
    31,28,31,30,31,30,31,31,30,31,30,31
  };
  for(int i = 0; i < sizeof(days)/sizeof(days[0]); i++){
    printf("Month %3d has %3d day\n",i+1,days[i]);
  }
}
