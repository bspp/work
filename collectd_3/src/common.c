#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<errno.h>
#include<assert.h>
#include "common.h"
char *sstrncpy(char *dest, const char *src, size_t n) { 
	strncpy(dest, src, n);
	dest[n - 1] = '\0';

	return dest;
} 
int strsplit(char *string, char **fields, size_t size) {
	size_t i;
	char *ptr;
	char *saveptr;

	i = 0; 
	ptr = string;
	saveptr = NULL;
	while ((fields[i] = strtok_r(ptr, " \t\r\n", &saveptr)) != NULL) {
		ptr = NULL;
		i++; 

		if (i >= size)
			break;
	}

	return (int)i;

}
#ifndef HAVE_HTONLL
unsigned long long ntohll(unsigned long long n) { 
#if BYTE_ORDER != BIG_ENDIAN
	return n;
#else
	return (((unsigned long long)ntohl(n)) << 32) + ntohl(n >> 32); 
#endif
} /* unsigned long long ntohll */

unsigned long long htonll(unsigned long long n) { 
#if BYTE_ORDER == BIG_ENDIAN
	return n;
#else
	return (((unsigned long long)htonl(n)) << 32) + htonl(n >> 32); 
#endif
} /* unsigned long long htonll */
#endif /* HAVE_HTONLL */


#if 0
void sfree (void **ptr)
{
	if (ptr == NULL)
		return;

	if (*ptr != NULL)
		free (*ptr);

	*ptr = NULL;

}
#endif

#if FP_LAYOUT_NEED_ENDIANFLIP
#define FP_CONVERT(A)                                                          \
	((((uint64_t)(A)&0xff00000000000000LL) >> 56) |                              \
	 (((uint64_t)(A)&0x00ff000000000000LL) >> 40) |                              \
	 (((uint64_t)(A)&0x0000ff0000000000LL) >> 24) |                              \
	 (((uint64_t)(A)&0x000000ff00000000LL) >> 8) |                               \
	 (((uint64_t)(A)&0x00000000ff000000LL) << 8) |                               \
	 (((uint64_t)(A)&0x0000000000ff0000LL) << 24) |                              \
	 (((uint64_t)(A)&0x000000000000ff00LL) << 40) |                              \
	 (((uint64_t)(A)&0x00000000000000ffLL) << 56))
#else
#define FP_CONVERT(A)                                                          \
	((((uint64_t)(A)&0xffffffff00000000LL) >> 32) |                              \
	 (((uint64_t)(A)&0x00000000ffffffffLL) << 32))
#endif
double ntohd(double d) {
	union {
		uint8_t byte[8];
		uint64_t integer;
		double floating;

	} ret;

	ret.floating = d;

	/* NAN in x86 byte order */
	if ((ret.byte[0] == 0x00) && (ret.byte[1] == 0x00) && (ret.byte[2] == 0x00) &&
			(ret.byte[3] == 0x00) && (ret.byte[4] == 0x00) && (ret.byte[5] == 0x00) &&
			(ret.byte[6] == 0xf8) && (ret.byte[7] == 0x7f)) {
		return NAN;

	} else {
		uint64_t tmp;

		tmp = ret.integer;
		ret.integer = FP_CONVERT(tmp);
		return ret.floating;

	}

} /* double ntohd */
double htond(double d) {
	union {
		uint8_t byte[8];
		uint64_t integer;
		double floating;

	} ret;

	if (isnan(d)) {
		ret.byte[0] = ret.byte[1] = ret.byte[2] = ret.byte[3] = 0x00;
		ret.byte[4] = ret.byte[5] = 0x00;
		ret.byte[6] = 0xf8;
		ret.byte[7] = 0x7f;
		return ret.floating;

	} else {
		uint64_t tmp;

		ret.floating = d;
		tmp = FP_CONVERT(ret.integer);
		ret.integer = tmp;
		return ret.floating;

	}

} /* double htond */






