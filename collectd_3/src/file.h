#ifndef FILE_H
#define FILE_H
#include "plugin.h"
#include "common.h"
#include <time.h>
#define FILE_EXIT_TIME 60
#define FILE_PART_NUM 5
#define FILE_PART_NULL "     "
#define CDTIME_T_TO_TIME_T(t)                                                  \
	(time_t) { (time_t)(((t) + (1 << 29)) >> 30)   }
typedef struct filenames_s filenames_t;
int write_value_to_file(value_list_t *vls[],char *path);
//void get_filename(value_list_t *vl,char *filename,char *filename_pos,char *path);
void get_filename(value_list_t *vl,char *path,char *filename);
void metis_filename_pos(value_list_t *vl,char *filename_pos,char del);
int read_file_create_time(const char *filename,time_t *time);
int create_file(const char *filename,time_t *time);
int values_to_lines(char *ret, int ret_len,const value_list_t *vl);
void time_t_to_time_str(time_t t,char *time_str);
void temp_filenames(char *filename1,char *filename2);
int metis_filenames_from_dir(const char *path,const char *filename_pos,char *filenames[]);
void metis_file_time(char *filename,time_t *file_time);

INT32 str_chr_last(char *s,char c);
VOID metis_parse_file(FILE *pfFile,value_list_t *psValue,value_list_t *pasValues[],INT32 *piValuesLen,TIME_T startTime,TIME_T endTime);
#endif


