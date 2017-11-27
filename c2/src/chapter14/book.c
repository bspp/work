#include <stdio.h>
#include <stdint.h>
struct book_s {
  char title[10];
  char author[20];
  float value;
};
typedef unsigned long long counter_t;
typedef double gauge_t;
typedef int64_t derive_t;
typedef uint64_t absolute_t;

union value_u {
  counter_t counter;
  gauge_t gauge;
  derive_t derive;
  absolute_t absolute;

};
typedef union value_u value_t;

typedef struct book_s book_t;
int main(){
  printf("%zd\n",sizeof(counter_t));
  printf("%zd\n",sizeof(gauge_t));
  printf("%zd\n",sizeof(derive_t));
  printf("%zd\n",sizeof(absolute_t));
  printf("%zd\n",sizeof(value_t));

  printf("%zd\n",sizeof(book_t));
  printf("%zd",sizeof(float));
}
