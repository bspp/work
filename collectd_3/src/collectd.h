#ifndef COLLECTD_H
#define COLLECTD_H


#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<malloc.h>
#include<assert.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<errno.h>

#include <arpa/inet.h>
#include"plugin.h"
#include"file.h"
#include"common.h"
#include "types_list.h"
#include <unistd.h>

#define TYPE_HOST 0x0000
#define TYPE_TIME 0x0001
#define TYPE_TIME_HR 0x0008
#define TYPE_PLUGIN 0x0002
#define TYPE_PLUGIN_INSTANCE 0x0003
#define TYPE_TYPE 0x0004
#define TYPE_TYPE_INSTANCE 0x0005
#define TYPE_VALUES 0x0006
#define TYPE_INTERVAL 0x0007
#define TYPE_INTERVAL_HR 0x0009
#define TYPE_MESSAGE 0x0100
#define TYPE_SEVERITY 0x0101
#define TYPE_SIGN_SHA256 0x0200
#define TYPE_SIGN_AES256 0x0210
#define MAX_PACKAGE_LENGTH 1452

typedef unsigned short USHORT;
typedef UINT32 ADDR_IN;
typedef struct sockaddr SOCKADDR_S;
typedef INT32 (*collectd_udp_send)(INT32 iFd, ADDR_IN inAddr, USHORT usPort, CHAR *pcBuffer, USHORT usSize);
typedef struct sockaddr_in SOCK_IN_S;
int write_part_values(char **ret_buffer,size_t *ret_buffer_len,const data_set_t *ds,const value_list_t *vl);
int write_part_number(char **ret_buffer, size_t *ret_buffer_len,int type, uint64_t value);
int write_part_string(char **ret_buffer, size_t *ret_buffer_len,int type, const char *str, size_t str_len);
int add_to_buffer(char *buffer,size_t buffer_size,value_list_t *vl_def,const data_set_t *ds,const value_list_t *vl);
int free_values_lists(value_list_t *vls[],int len);
#define TIME_T_TO_CDTIME_T_STATIC(t) (((cdtime_t)(t)) << 30)
#define TIME_T_TO_CDTIME_T(t)                                                  \
	(cdtime_t) { TIME_T_TO_CDTIME_T_STATIC(t)  }
#define VALUE_LIST_INIT                                                        \
	  { .values = NULL}
INT32 udp_send(INT32 iFd, ADDR_IN inAddr, USHORT usPort, CHAR *pcBuffer, USHORT usSize);



#endif
