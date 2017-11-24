#include <stdio.h>
#ifndef COMMON_H
#define COMMON_H
#define NAN (0.0 / 0.0)
#define sfree(ptr)                                                             \
	do {                                                                         \
		free(ptr);                                                                 \
		(ptr) = NULL;                                                              \
	} while (0)
int strsplit(char *string, char **fields,size_t  size);
char *sstrncpy(char *dest, const char *src, size_t n);
#ifndef HAVE_HTONLL
unsigned long long ntohll(unsigned long long n); 
unsigned long long htonll(unsigned long long n); 
#endif

double ntohd(double d); 
double htond(double d); 
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT32;
typedef unsigned long int UINT64, ULONG;

typedef char CHAR;
typedef short SHORT;
typedef int INT32;
typedef long int INT64, LONG,TIME_T;
typedef size_t SIZE_T;

typedef double DOUBLE;
typedef float FLOAT;

typedef void VOID;

#endif

