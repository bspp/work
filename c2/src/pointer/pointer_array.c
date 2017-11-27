#include<stdio.h>
#include<string.h>
char *sstrncpy(char *dest, const char *src, size_t n) { 
  strncpy(dest, src, n); 
  dest[n - 1] = '\0';

  return dest;
} 
int main(){
}
