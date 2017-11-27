#include <stdio.h>
#define MSG "I am a symbolic string constant"
#define MAXLENGTH 90
int main(void){
  char words[MAXLENGTH]= "I am a string in an array";
  const char *pt1 = "Something is poingting at me.";
  puts("Her are some strings:");
  puts(MSG);
  puts(words);
  puts(pt1);
  words[8] = 'p';
  puts(words);
  return 0;
}
