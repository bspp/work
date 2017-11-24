#ifndef PLUGIN_H
#define PLUGIN_H\

#include <stdio.h>
#include <stdint.h>
#include "utils_avltree.h"

#ifndef DATA_MAX_NAME_LEN
#define DATA_MAX_NAME_LEN 128
#endif

#define DS_TYPE_COUNTER 0
#define DS_TYPE_GAUGE 1
#define DS_TYPE_DERIVE 2
#define DS_TYPE_ABSOLUTE 3
void plugin_free_data_sets(void);
struct indentifier_s{
	char *host;
	char *plugin;
	char *plugin_instance;
	char *type;
	char *type_instance;
};
typedef struct indentifier_s indentifier_t;
typedef unsigned long long counter_t;
typedef double gauge_t;
typedef int64_t derive_t;
typedef uint64_t absolute_t;
union value_type_s{
	counter_t counter;
	gauge_t gauge;
	derive_t derive;
	absolute_t absolute;
};
typedef union value_type_s value_type_t;
struct value_u{
	uint8_t type;
	double value;
	char name[DATA_MAX_NAME_LEN];
};
typedef struct value_u value_t;
typedef uint64_t cdtime_t;
struct value_list_s{
	value_t *values;
	size_t values_len;
	cdtime_t time;
	cdtime_t interval;
	char host[DATA_MAX_NAME_LEN];
	char plugin[DATA_MAX_NAME_LEN];
	char plugin_instance[DATA_MAX_NAME_LEN];
	char type[DATA_MAX_NAME_LEN];
	char type_instance[DATA_MAX_NAME_LEN];
};
typedef struct value_list_s value_list_t;
typedef struct value_list_s VALUE_LIST_S;



struct data_source_s {
	char name[DATA_MAX_NAME_LEN];
	int type;
	double min;
	double max;

};
typedef struct data_source_s data_source_t;
struct data_set_s {
	char type[DATA_MAX_NAME_LEN];
	size_t ds_num;
	data_source_t *ds;
};
typedef struct data_set_s data_set_t;
data_set_t * plugin_get_data_set(const char *type);
int plugin_register_data_set(const data_set_t *ds);
int plugin_unregister_shutdown(const char *name);
int type_exit_data_set(const char *type);
int set_value_name(value_list_t *vl_copy,value_list_t *vl);


#endif
