#include <stdio.h>
#include <string.h>
#define DENSITY 62.4
/**C语言没有专门用于储存字符串的变量类型，字符串都被储存在char类型的数组中。数
 * 组由连续的存储单元组成，字符串中的字符被储存在相邻的存储单元中，每个单元储存一个
 * 字符
 */
int main(){
  float weight,volume;
  int size,letters;
  char name[40];
  printf("Hi! What's your first name?\n");
  scanf("%s",name);
  printf("%s, what's your weight in pounds?\n", name);
  scanf("%f", &weight);
  size = sizeof name;
  letters = strlen(name);
  volume = weight / DENSITY;
  printf("Well, %s, your volume is %2.2f cubic feet.\n",
      name, volume);
  printf("Also, your first name has %d letters,\n",
      letters);
  printf("and we have %d bytes to store it.\n", size);
  return 0;
}
