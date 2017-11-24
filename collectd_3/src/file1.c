#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/file.h>
#include <regex.h>
#include <dirent.h>

#include "plugin.h"
#include "common.h"
#include "file.h"
#include "time.h"
#include "collectd.h"
static void metis_free_filenames(char *filenames[],const int num)
{
	int i = 0;
	for(;i++;i<num)
	{ 
		sfree(filenames[i]);
	}
}
static int metis_mall_filenames(char *filenames[],const int num,const int filename_len)
{
	int i = 0;
	for(;i<num;i++)
	{
		filenames[i] = (char *)malloc(sizeof(char)*filename_len);
		if(filenames[i] == NULL)
		{
			metis_free_filenames(filenames,i-1);
			return -1;
		}
	}
	return 0;
}
int write_value_to_file(value_list_t *vls[],char *path)
{
	char filename[DATA_MAX_NAME_LEN+sizeof(path)+11];
	char filename_new[DATA_MAX_NAME_LEN+sizeof(path)+11];
	char filename_pos[DATA_MAX_NAME_LEN];
	char *filenames[2];
	FILE *fp = NULL;
	FILE *fp_new = NULL;
	char pos_new[4]="new";
	int flag = 0;
	time_t value_time = CDTIME_T_TO_TIME_T(vls[0]->time);
	time_t value_interval = CDTIME_T_TO_TIME_T(vls[0]->interval);
	time_t file_interval = 0;
	time_t file_create_time = 0;

	metis_mall_filenames(filenames,2,DATA_MAX_NAME_LEN+11);
	
	assert(vls[0] != NULL);

	get_filename_pos(vls[0],filename_pos,'-');
	flag = filename_from_dir(path,filename_pos,filenames);
	printf("flag %d\n",flag);

	if(flag == -1)
		return -1;
	else if(flag == 0)
	{
		get_filename(vls[0],path,filename);
		if(create_file(filename,&value_time) != 0)
		{
			return -1;
		}
	}else if(flag == 1)
	{ 
		sprintf(filename,"%s/%s",path,filenames[0]);

		if(file_interval >= FILE_EXIT_TIME/2)
		{ 
			get_filename(vls[0],path,filename_new);
			if(create_file(filename_new,&value_time) != 0)
			{   
				return -1; 
			}   
		}
	}else if(flag == 2)
	{
		printf("filename0 %s\n",filenames[0]);
		printf("filename1 %s\n",filenames[1]);
		sprintf(filename,"%s/%s",path,filenames[0]);
		sprintf(filename_new,"%s/%s",path,filenames[1]);
		
		if(file_interval >= FILE_EXIT_TIME)
		{ 
			remove(filename);
			sstrncpy(filename,filename_new,strlen(filename)+1);
			get_filename(vls[0],path,filename_new);
			create_file(filename_new,&value_time);
		}
	}
	metis_free_filenames(filenames,2);
	return 0;
}
void get_filename_pos(value_list_t *vl,char *filename_pos,char del)
{
	  char *pos = strchr(vl->host,del);

		if(pos != NULL){
			    sstrncpy(filename_pos,vl->host,(pos-vl->host+1));
					  
		}else{
			    sstrncpy(filename_pos,vl->host,sizeof(filename_pos));	  
		}
}
void get_filename(value_list_t *vl,char *path,char *filename)
{
	char filename_pos[DATA_MAX_NAME_LEN];

	assert(vl->host != NULL);

	get_filename_pos(vl,filename_pos,'-');
	
	sprintf(filename,"%s/%ld_%s.csv",path,CDTIME_T_TO_TIME_T(vl->time),filename_pos);

}
time_t	get_file_create_time(const char *filename,time_t *file_create_time)
{
	char time_str[11];

	char *ptr = strchr(filename,'_');
	if(ptr != NULL)
	{
		sstrncpy(time_str,filename,(ptr-filename+1));
		*file_create_time = strtol(time_str,NULL,10);
	}
	printf("file_creat_time %ld\n",*file_create_time);
}
int filename_from_dir(const char *path,const char *filename_pos,char *filenames[])
{
	DIR *dfd;
	struct dirent *dp;
	int flag = 0;

	if ((dfd = opendir(path)) == NULL)  
	{  
		printf("dir_order: can't open %s\n %s", path,strerror(errno));  
		return -1; 
	}

	while((dp = readdir(dfd)) != NULL)
	{
		if(strcmp(dp->d_name,".") == 0)
		{
			continue;
		}else if(strstr(dp->d_name,filename_pos) != NULL)
		{ printf("dir====================================================\n");
			sstrncpy(filenames[flag],dp->d_name,strlen(dp->d_name)+1);
			flag++;
		}
	}

	if(flag == 2 )
	{
		time_t file1_time = 0;
		time_t file2_time = 0;
		get_file_create_time(filenames[0],&file1_time);
		get_file_create_time(filenames[1],&file2_time);
		if(file1_time >file2_time)
		{
		temp_filenames(filenames[0],filenames[1]);

		}
	}

	//printf("%s\n",filenames[0]);
	//printf("%s\n",filenames[1]);

	if(dfd != NULL)
	{
		closedir(dfd);
	}
	return flag;
}
void temp_filenames(char *filename1,char *filename2)
{
	char temp[DATA_MAX_NAME_LEN+11];
	memset(temp,'\0',sizeof(temp));

	sstrncpy(temp,filename1,strlen(filename1)+1);
	sstrncpy(filename1,filename2,strlen(filename2)+1);
	sstrncpy(filename2,temp,strlen(temp)+1);
}
int create_file(const char *filename,time_t *time){
	FILE *fp = NULL;
	if(access(filename,F_OK) != 0){
		if ((fp=fopen(filename,"a+"))==NULL)           
		{  
			printf("create file failed.\n");  
			return -1; 
		}
	}

	if(fp != NULL){
		fclose(fp);
	}
	return 0;
}

int values_to_lines(char *ret,int ret_len,const value_list_t *vl) {
	char *buffer;
	size_t buffer_size;

	buffer = ret; 
	buffer_size = (size_t)ret_len;

#define APPEND(str)                                                            \
	do {                                                                         \
		size_t l = strlen(str);                                                    \
		if (l >= buffer_size)                                                      \
		return ENOBUFS;                                                          \
		memcpy(buffer, (str), l);                                                  \
		buffer += l;                                                               \
		buffer_size -= l;                                                          \
	} while (0)
	assert(vl->host != NULL);
	assert(vl->type != NULL);

	//metricNamePre
	APPEND(vl->host);
	APPEND("-");
	APPEND(vl->type);
	APPEND(",");

	//metricNamePos
	if ((vl->plugin != NULL) && (vl->plugin[0] != 0))
	{
		APPEND(vl->plugin);
	}
	if ((vl->plugin_instance != NULL) && (vl->plugin_instance[0] != 0)) {
		if(ret[strlen(ret)-1] != ',')
		{
			APPEND("-");
		}
		APPEND(vl->plugin_instance);
	}
	if ((vl->type_instance != NULL) && (vl->type_instance[0] != 0)) 
	{

		if(ret[strlen(ret)-1] != ',')

		{
			APPEND("-");
		}
		APPEND(vl->type_instance);
	}
	APPEND(",");

	//metric time
	char time_str[64];
	time_t_to_time_str(CDTIME_T_TO_TIME_T(vl->time),time_str);
	APPEND(time_str);
	APPEND(",");

	int i = 0;
	for(;i<vl->values_len;i++)
	{
		APPEND(vl->values[i].name);
		APPEND(":");

		char value_str[32];
		sprintf(value_str,"%f",vl->values[i].value);
		APPEND(value_str);
		APPEND(" ");
	}

	printf("%s\n",ret);
	assert(buffer_size > 0); 
	buffer[0] = 0;

#undef APPEND

	return 0;
}
void time_t_to_time_str(time_t t, char *time_str)
{
	struct tm *tm_t;
	tm_t = localtime(&t);
	sprintf(time_str,"%d-%d-%d %d:%d:%d",
			tm_t->tm_year+1900, 
			tm_t->tm_mon+1, 
			tm_t->tm_mday, 
			tm_t->tm_hour, 
			tm_t->tm_min,
			tm_t->tm_sec);
}
