#include <stdio.h>
#define SIZE 10
//数组名就是数组首元素的地址
//ar是指向数组首元素的一个指针，
int sum(int ar[],int n);
int main(void){
  int marbles[SIZE] = {
    10,20,30,40,50,60,70,80,90,100
  };
  long answer = sum(marbles,SIZE);
  printf("the *marbles is%d\n",*marbles);
  printf("The size of marbles is %zd bytes.\n",sizeof(marbles));
  printf("The address of marble is %10p bytes.\n",&marbles);
  printf("The value of marbles is %10p .\n",marbles);
}
int sum(int ar[],int n){
  int total = 0;
  for(int i = 0;i < n;i++){
    total += ar[i];
  }
  printf("the *ar is%d\n",*ar);
  printf("The size of ar is %zd bytes.\n",sizeof(ar));
  printf("The address of ar is %10p.\n",&ar);
  printf("The vlaue of ar is %10p.\n",ar);
  return total;
}
