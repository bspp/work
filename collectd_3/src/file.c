#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/file.h>
#include <dirent.h>

#include "plugin.h"
#include "common.h"
#include "file.h"
#include "time.h"
#include "collectd.h"
static void metis_free_str_arr(char *array[],const int num)
{
	int i = 0;
	for(;i<num;i++)
	{   
		sfree(array[i]);
	}

}
static int metis_mall_str_arr(char *array[],const int num,const int array_len)
{
	int i = 0;
	for(;i<num;i++)
	{
		array[i] = (char *)malloc(sizeof(char)*array_len);
		memset(array[i],'\0',array_len);

		if(array[i] == NULL)
		{   
			metis_free_str_arr(array,i-1);
			return -1; 
		}   
	}
	return 0;

}
static void metis_temp_filenames(char *filename1,char *filename2)
{
	char temp[DATA_MAX_NAME_LEN+11];
	memset(temp,'\0',sizeof(temp));

	sstrncpy(temp,filename1,strlen(filename1)+1);
	sstrncpy(filename1,filename2,strlen(filename2)+1);
	sstrncpy(filename2,temp,strlen(temp)+1);

}
int write_value_to_file(value_list_t *vls[],char *path)
{
	char filename[DATA_MAX_NAME_LEN+strlen(path)+11];
	char filename_new[DATA_MAX_NAME_LEN+strlen(path)+11];
	char filename_pos[DATA_MAX_NAME_LEN];
	char *filenames[2];
	FILE *fp = NULL;
	FILE *fp_new = NULL;
	int flag = 0;
	time_t value_time = CDTIME_T_TO_TIME_T(vls[0]->time);
	time_t value_interval = CDTIME_T_TO_TIME_T(vls[0]->interval);
	time_t file_interval = 0;
	time_t file_time = 0;

	assert(vls[0] != NULL);

	metis_mall_str_arr(filenames,2,DATA_MAX_NAME_LEN+11);

	metis_filename_pos(vls[0],filename_pos,'-');
	flag = metis_filenames_from_dir(path,filename_pos,filenames);
	//printf("flag : %ld\n",flag)dd;
	//printf("flag : %s\n",filenames[0]);
	//printf("flag : %s\n",filenames[1]);
	switch(flag)
	{
		case 0:
			get_filename(vls[0],path,filename);
			if(metis_create_file_file(filename,&value_time) != 0)
			{
				metis_free_str_arr(filenames,2);
				return -1;
			}
			break;
		case 1:
			sprintf(filename,"%s/%s",path,filenames[0]);

			metis_file_time(filenames[0],&file_time);
			file_interval = value_time-file_time;

			if(file_interval >= FILE_EXIT_TIME/2)
			{
				get_filename(vls[0],path,filename_new);
				if(metis_create_file_file(filename_new,&value_time) != 0)
				{
					metis_free_str_arr(filenames,2);
					return -1;
				}
			}
			break;
		case 2:
			sprintf(filename,"%s/%s",path,filenames[0]);
			sprintf(filename_new,"%s/%s",path,filenames[1]);

			metis_file_time(filenames[0],&file_time);
			file_interval = value_time-file_time;
			if(file_interval >= FILE_EXIT_TIME)
			{
				remove(filename);
				sstrncpy(filename,filename_new,strlen(filename)+1);
				get_filename(vls[0],path,filename_new);
				if(metis_create_file_file(filename_new,&value_time) != 0)
				{
					metis_free_str_arr(filenames,2);
					return -1;
				}
			}
			break;
		default:
			metis_free_str_arr(filenames,2);
			return -1;
	}
	//open file
	if ((fp=fopen(filename,"a+"))==NULL)
	{
		printf("Open Failed.\n");
		metis_free_str_arr(filenames,2);
		return -1;
	}else
	{
		while(1)
		{
			if(flock(fp->_fileno,LOCK_EX) == 0)
				break;
		}
	}

	//opne file_new
	if(access(filename_new,F_OK) == 0){
		if ((fp_new=fopen(filename_new,"a+"))==NULL)
		{
			printf("Open Failed.\n");
			metis_free_str_arr(filenames,2);
			return -1;
		}else
		{
			while(1)
			{
				if(flock(fp_new->_fileno,LOCK_EX) == 0)
					break;
			}
		}
	}
	int i = 0;
	for(;i < 3;i++)
	{
		char ret[MAX_PACKAGE_LENGTH];
		memset(ret,'\0',MAX_PACKAGE_LENGTH);
		values_to_lines(ret,MAX_PACKAGE_LENGTH,vls[i]);

		fprintf(fp,"%s\n",ret);

		if(fp_new != NULL)
		{
			fprintf(fp_new,"%s\n",ret);
		}
	}

	if(fp != NULL)
	{
		flock(fp->_fileno, LOCK_UN);
		fclose(fp);
	}

	if(fp_new != NULL)
	{
		flock(fp_new->_fileno,LOCK_UN);
		fclose(fp_new);
	}
	metis_free_str_arr(filenames,2);
	return 0;

}
void metis_filename_pos(value_list_t *vl,char *filename_pos,char del)
{
	char *pos = strchr(vl->host,del);

	if(pos != NULL)
	{
		sstrncpy(filename_pos,vl->host,(pos-vl->host+1));
	}else{
		sstrncpy(filename_pos,vl->host,strlen(filename_pos)+1);
	}
}
void get_filename(value_list_t *vl,char *path,char *filename)
{
	char filename_pos[DATA_MAX_NAME_LEN];

	assert(vl->host != NULL);

	metis_filename_pos(vl,filename_pos,'-');

	sprintf(filename,"%s/%ld_%s.csv",path,CDTIME_T_TO_TIME_T(vl->time),filename_pos);


}
void metis_file_time(char *filename,time_t *file_time)
{ 
	char time_str[16];

	memset(time_str,'\0',16);
	char *ptr = strchr(filename,'_');
	if(ptr != NULL)
	{
		sstrncpy(time_str,filename,(ptr-filename+1));
		*file_time = strtol(time_str,NULL,10);
	}
}
int metis_filenames_from_dir(const char *path,const char *filename_pos,char *filenames[])
{
	DIR *dfd;
	struct dirent *dp;
	int flag = 0;
	char *strtemp;
	char *strtemp1;
	char file_actual_pos[128];


	if ((dfd = opendir(path)) == NULL)
	{
		printf("dir_order: can't open %s\n %s", path,strerror(errno));
		return -1;

	}

	while((dp = readdir(dfd)) != NULL)
	{
		strtemp=strchr(dp->d_name,'_');
		strtemp1=strchr(dp->d_name,'.');

		if(strtemp == NULL || strtemp1 == NULL)
			continue;

		sstrncpy(file_actual_pos,strtemp+1,strtemp1-strtemp);

		if(strcmp(file_actual_pos,filename_pos) == 0)
		{
			sstrncpy(filenames[flag],dp->d_name,strlen(dp->d_name)+1);
			flag++;
		}
	}
	printf("flag:%d\n",flag);
	if(flag == 2){
		time_t  file1_time = 0;
		time_t file2_time = 0;

		metis_file_time(filenames[0],&file1_time);
		metis_file_time(filenames[1],&file2_time);

		if(file1_time > file2_time)
		{
			metis_temp_filenames(filenames[0],filenames[1]);
		}
	}

	if(dfd != NULL)
	{
		closedir(dfd);
	}

	return flag;
}
int metis_create_file_file(const char *filename,time_t *time){
	FILE *fp = NULL;

	if(access(filename,F_OK) != 0){
		if ((fp=fopen(filename,"a+"))==NULL)
		{
			printf("metis_create_file file failed.\n");
			return -1;
		}
	}

	if(fp != NULL)
	{
		fclose(fp);
	}

	return 0;
}
int values_to_lines(char *ret,int ret_len,const value_list_t *vl)
{
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
	APPEND(",");
	APPEND(vl->type);
	APPEND(",");

	//metricNamePos
	if ((vl->plugin != NULL) && (vl->plugin[0] != 0)) 
	{
		APPEND(vl->plugin);
	}
	if ((vl->plugin_instance != NULL) && (vl->plugin_instance[0] != 0)) {
		if(ret[strlen(ret) -1] != ',')
		{
			APPEND("-");
		}
		APPEND(vl->plugin_instance);
	}
	if ((vl->type_instance != NULL) && (vl->type_instance[0] != 0))
	{
		if(ret[strlen(ret) -1] != ',')
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
void time_t_to_time_str(TIME_T t, CHAR *pcTime)
{
	struct tm *tm_t;
	tm_t = localtime(&t);
	strftime(pcTime,128,"%F %T",tm_t);

}
VOID metis_strptime(char *pcTime,TIME_T *ptTime)
{
	struct tm stm;
	strptime(pcTime,"%Y-%m-%d %H:%M:%S",&stm);
	*ptTime = mktime(&stm);
}
INT32 metis_parse_values(CHAR *pcValue,value_t **psValue,SIZE_T *pstNum_value)
{
	CHAR *fields[8];
	SIZE_T fields_num = 0;
	value_t *pkg_values;

	fields_num =  strsplit(pcValue,fields,8);
	pkg_values = calloc(fields_num,sizeof(*pkg_values));
	if(pkg_values == NULL)
	{
		printf("metis_parse_values :calloc failed\n");
		return -1;
	}
	INT32 i = 0;
	for(;i<fields_num;i++)
	{
		INT32 ptr_len = 0;
		CHAR *pos = strchr(fields[i],':');

		ptr_len = pos-fields[i];

		sstrncpy(pkg_values[i].name,fields[i],ptr_len+1);
		pkg_values[i].value = atof(pos+1);

	}

	*psValue = pkg_values;
	*pstNum_value = fields_num;

	return 0;
}
INT32  metis_parse_line(CHAR *pcBuf,value_list_t *psValue1,value_list_t *psValue2)
{
	CHAR *dummy;
	CHAR *saveptr;
	CHAR *parts[8];
	UINT32 parts_num;
	TIME_T value_time = 0;
	SIZE_T values_len = 0;

	dummy = pcBuf;
	saveptr = NULL;
	parts_num = 0;

	while(parts_num < 8)
	{
		if((parts[parts_num] = strtok_r(dummy,",",&saveptr)) == NULL)
		{
			break;
		}
		dummy = NULL;
		parts_num++;
	}
	if(parts_num != FILE_PART_NUM 
			|| strcmp(psValue1->host,parts[0]) !=  0 
			|| strcmp(psValue1->type,parts[1]) != 0)
	{ 
		return -1;
	}

	sstrncpy(psValue2->host,parts[0],strlen(parts[0])+1);
	sstrncpy(psValue2->type,parts[1],strlen(parts[1])+1);

	if((psValue1->type_instance == NULL) ||(psValue1->type_instance[0] == 0))
	{
		if(strcmp(FILE_PART_NULL,parts[2]) != 0)
		{
			return -1;
		}
	}else{
		if(strcmp(psValue1->type_instance,parts[2]) != 0)
		{
			return -1;
		}
		sstrncpy(psValue2->type_instance,parts[2],strlen(parts[2])+1);
	}

	metis_strptime(parts[3],&value_time);
	psValue2->time = TIME_T_TO_CDTIME_T(value_time);

	metis_parse_values(parts[4],&psValue2->values,&values_len);
	psValue2->values_len = values_len;

	return 0;
}
VOID metis_parse_file(FILE *pfFile,value_list_t *psValue,value_list_t *pasValues[],INT32 *piValuesLen,TIME_T startTime,TIME_T endTime)
{
	CHAR buf[1452];
	CHAR type[128];
	SIZE_T buf_len = 0;
	memset(buf,'\0',1452);
	memset(type,'\0',128);
	INT32 index = 0;

	while(fgets(buf,sizeof(buf),pfFile) != NULL)
	{ 
		buf_len = strlen(buf);
		if (buf_len > 1452) {
			printf("line with more than 1452 characters.");
			do {
				if (fgets(buf, sizeof(buf), pfFile) == NULL)
					break;

				buf_len = strlen(buf);
			} while (buf_len > 1452);
			continue;
		} 

		if((buf_len == 0) || buf[0] == '#'){
			continue;
		}

		while ((buf_len > 0) &&
				((buf[buf_len - 1] == '\n') || (buf[buf_len - 1] == '\r')))
			buf[--buf_len] = '\0';

		if (buf_len == 0)
			continue;
		//	printf("%s\n",buf);
		pasValues[index] = calloc(1,sizeof(*pasValues[index]));
		if(pasValues[index] == NULL)
			free_values_lists(pasValues,index);

		if	(metis_parse_line(buf,psValue,pasValues[index]) == 0){
			TIME_T t = CDTIME_T_TO_TIME_T(pasValues[index]->time);
			if( t >= startTime  && t <= endTime)
			{ 
				index++;
			}else{
				sfree(pasValues[index]->values);
				sfree(pasValues[index]);
			}
		}
	}

	*piValuesLen = index;
}
INT32 metis_read_file(const char *pcFilePath,value_list_t *psValue,value_list_t *pasValues[],INT32 *piValuesNum,TIME_T startTime,TIME_T endTime)
{
	CHAR *filenames[2];
	CHAR filename_pos[DATA_MAX_NAME_LEN];
	CHAR filename[DATA_MAX_NAME_LEN+strlen(pcFilePath)+11];
	memset(filename_pos,'\0',DATA_MAX_NAME_LEN);
	memset(filename,'\0',DATA_MAX_NAME_LEN+strlen(pcFilePath)+11);
	FILE *fp = NULL;

	if(pcFilePath == NULL)
		goto _failed;

	metis_filename_pos(psValue,filename_pos,'-');

	if(metis_mall_str_arr(filenames,2,DATA_MAX_NAME_LEN+11) != 0)
		goto _failed;

	if(metis_filenames_from_dir(pcFilePath,filename_pos,filenames) < 0)
		goto _failed_fd;
	sprintf(filename,"%s/%s",pcFilePath,filenames[0]);

	if((fp = fopen(filename,"a+")) == NULL)
	{
		goto _failed_fd;
	}else
	{
		while(1)
		{
			if(flock(fp->_fileno,LOCK_EX) == 0)
				break;
		}
	}

	metis_parse_file(fp,psValue,pasValues,piValuesNum,startTime,endTime);
	metis_free_str_arr(filenames,2);
	return 0;

_failed:
	return -1;

_failed_fd:
	metis_free_str_arr(filenames,2);
	return -1;
}
INT32 time_zone()
{
	TIME_T tUTC = 0;
	struct tm *psUTC;
	INT32 iZone = 0;

	psUTC = localtime(&tUTC);
	iZone = (psUTC->tm_hour > 12 ) ? (psUTC->tm_hour-=23) : psUTC->tm_hour; 
}
INT32 str_chr_last(char *s,char c)
{
	int i = strlen(s);
	for(;i>=0;i--)
	{
		if(s[i] == c)
			break;
	}
	return i;
}
INT32 str_chr_first(char *s,char c)
{
	int i = 0;
	int len = strlen(s);

	for(;i<len;i++)
	{
		if(s[i] == c)
			break;
	}
	return i;
}
//int main()
//{
//
//	//	char *str_time="2011-12-31 11:43:07";
//	//	struct tm stm;  
//	//	printf("======================================================\n");
//	//	strptime(str_time, "%Y-%m-%d %H:%M:%S",&stm);
//	//	printf("======================================================\n");
//	//	long t = mktime(&stm);
//	//	printf("%ld\n",t);
//
//	//	FILE *fh;
//	//	CHAR *file = "test.csv";
//	//	CHAR *type = "cpu";
//	//	if(file == NULL){
//	//		return -1; 
//	//
//	//	}
//	//	fh = fopen(file,"r");
//	//	if(fh == NULL){
//	//		printf("failed to open types database : %s",file);
//	//		return -1; 
//	//	}
//	//	metis_parse_file(fh,type);
//	//value_list_t vl = {.host="cluster1-datacenter1-hostname",.type="test0",.type_instance="usage"};
//	//value_list_t vl2 = {.host="cluster1-datacenter-hostername",.type="cpu",.type_instance = "usage"};
//	//value_list_t *vls[5];
//	//INT32 len = 0;
//	//char *path = "/opt";
//	//char *time_start_str = "2017-10-26 10:19:10";
//	//char *time_end_str = "2017-10-27 10:19:10";
//	//TIME_T starTime = 0;
//	//TIME_T endTime = 0;
//	//metis_strptime(time_start_str,&starTime);
//	//metis_strptime(time_end_str,&endTime);
//	//printf("%ld\n",starTime);
//
//	//metis_read_file(path,&vl2,vls,&len,starTime,endTime);
//	//printf("<------------------>%s\n",vls[0]->type);
//	//printf("<------------------>%ld\n",vls[0]->time);
//	//printf("%d\n",len);
//	//free_values_lists(vls,len);
//	char *path = "/opt/";
//	char *pos = "Sniffer";
//	char *filenames[2];
//	metis_mall_str_arr(filenames,2,DATA_MAX_NAME_LEN+11);
//	metis_filenames_from_dir(path,pos,filenames);
//	printf("%s\n",filenames[0]);
//	//char *str="123123123_Sniffer.csv";
//	//char character='_' ;  //指定一个字符
//	//char *strtemp;
//	//char *strtemp1;
//	//char file_pos[128];
//
//	//strtemp=strchr(str,'_');
//	//strtemp1=strchr(str,'.');
//	//if(strtemp!=NULL)
//	//{
//	//	printf("%s\n ",strtemp+1);
//	//	printf("%s\n",strtemp1);
//	//	sstrncpy(file_pos,strtemp+1,(strtemp1-strtemp));
//	//	printf("%s\n",file_pos);
//
//	//}
//	//else
//	//{
//	//	printf("can not find %c !",strtemp);
//
//	//}
//
//}
