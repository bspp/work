#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "plugin.h"
#include "common.h"
static int parse_ds(data_source_t *dsrc,char *buf,size_t buf_len){
	char *dummy;
	char *saveptr;
	char *fields[8];
	char fields_num;
	if(buf_len < 11){
		printf("parse_ds: (buf_len = %zu) < 11",buf_len);
		return -1;
	}

	if (buf[buf_len - 1] == ',') {
		buf_len--;
		buf[buf_len] = '\0';
	}
	dummy = buf;
	saveptr = NULL;
	fields_num = 0;
	while(fields_num < 8){
		if((fields[fields_num] = strtok_r(dummy,":",&saveptr)) == NULL)
			break;
		dummy = NULL;
		fields_num++;
	}


	if (fields_num != 4) {
		printf("parse_ds: (fields_num = %i) != 4", fields_num);
		return -1;

	}
	sstrncpy(dsrc->name, fields[0], sizeof(dsrc->name));

	if (strcasecmp(fields[1], "GAUGE") == 0)
		dsrc->type = DS_TYPE_GAUGE;
	else if (strcasecmp(fields[1], "COUNTER") == 0)
		dsrc->type = DS_TYPE_COUNTER;
	else if (strcasecmp(fields[1], "DERIVE") == 0)
		dsrc->type = DS_TYPE_DERIVE;
	else if (strcasecmp(fields[1], "ABSOLUTE") == 0)
		dsrc->type = DS_TYPE_ABSOLUTE;
	else {
		printf("(fields[1] = %s) != (GAUGE || COUNTER || DERIVE || ABSOLUTE)",
				fields[1]);
		return -1;
	}

	if (strcasecmp(fields[2], "U") == 0)
		dsrc->min = NAN;
	else
		dsrc->min = atof(fields[2]);
	if (strcasecmp(fields[3], "U") == 0)
		dsrc->max = NAN;
	else
		dsrc->max = atof(fields[3]);
	return 0;
}//parse_ds 分解每一个小项
static void parse_line(char *buf){
	char *fields[64];
	size_t fields_num;
	data_set_t *ds;

	fields_num = strsplit(buf,fields,64);
	if(fields_num < 2)
		return;
	if(fields[0][0] == '#')
		return;
	ds = calloc(1,sizeof(*ds));
	if(ds == NULL){
		return;
	}
	sstrncpy(ds->type, fields[0], sizeof(ds->type));

	ds->ds_num = fields_num - 1;
	printf("%d\n",fields_num);
	ds->ds = (data_source_t *)calloc(ds->ds_num,sizeof(data_source_t));
	if(ds->ds == NULL){
		sfree(ds);
		return;
	}
	size_t i = 0;
	for(;i<ds->ds_num;i++){
		if (parse_ds(ds->ds + i, fields[i + 1], strlen(fields[i + 1])) != 0) {
			printf("types_list: parse_line: Cannot parse data source #%zu "
					"of data set %s",i, ds->type);
			sfree(ds->ds);
			sfree(ds);
			return;
		}
	}
	plugin_register_data_set(ds);
	sfree(ds->ds);
	sfree(ds);
}
static void parse_file(FILE *fh){
	char buf[4096];
	size_t buf_len;
	while(fgets(buf,sizeof(buf),fh) != NULL){
		buf_len = strlen(buf);
		if (buf_len >= 4095) {
			printf("Skipping line with more than 4095 characters.");
			do {
				if (fgets(buf, sizeof(buf), fh) == NULL)
					break;
				buf_len = strlen(buf);

			} while (buf_len >= 4095);
			continue;
		} /* if (buf_len >= 4095) */
		if((buf_len == 0) || buf[0] == '#'){
			continue;
		}
		while ((buf_len > 0) &&
				((buf[buf_len - 1] == '\n') || (buf[buf_len - 1] == '\r')))
			buf[--buf_len] = '\0';

		if (buf_len == 0)
			continue;
		parse_line(buf);

	}
}
int read_types_list(const char *file){
	FILE *fh;
	if(file == NULL){
		return -1;
	}
	fh = fopen(file,"r");

	if(fh == NULL){
		char errbuf[1024];
		printf("failed to open types database %s",file);
		return -1;
	}
	printf("parse_file\n");
	parse_file(fh);
	if(fh != NULL)
	{ 
		fclose(fh);
	}
	return 0;
}
int main()
{
	read_types_list("types2.db");
	return -1;
}
