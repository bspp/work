#include<stdio.h>
#include<malloc.h>
#include<string.h>

#include"merge.h"
static void *metis_to_zero(DOUBLE daValues[],INT32 iCount)
{
	INT32 i = 0;

	for(;i < CALC_TYPE_NUM;i++)
		daValues[i] = 0;

	iCount = 0;
}

static void *metis_calc(DOUBLE daValues[],DOUBLE daNewValues[],INT32 iCount)
{
	if(iCount == 0)
	{
		daValues[0] = daNewValues[0];//max
		daValues[0] = daNewValues[0];//min;
	}else
	{
		daValues[0] =
			daValues[0] > daNewValues[0] ? daValues[0] : daNewValues[0];//max
		daValues[1] =
			daValues[1] < daNewValues[1] ? daValues[1]:daNewValues[1];//min
	}

	daValues[2] = (daValues[3] + daNewValues[3])/(iCount + 1);//ave
	daValues[3] = daValues[3]+daValues[3];//sum
	daValues[4] = daNewValues[4];//new
}

static VOID *metis_calc_one_day(METIS_MERGE_ITEM_S *psMerge,DOUBLE daNewValues[])
{
	metis_calc(psMerge->daOneDay,daNewValues,psMerge->iCountOneDay);//udpate one day level data

	psMerge->iCountOneDay++;

	if(psMerge->iCountOneDay == MAX_COUNT_ONE_DEY)
	{
		metis_to_zero(psMerge->daOneDay,psMerge->iCountOneDay);    
	}
}


static VOID *metis_calc_two_hour(METIS_MERGE_ITEM_S *psMerge,DOUBLE daNewValues[])
{
	metis_calc(psMerge->daTwoHour,daNewValues,psMerge->iCountTwoHour);//udpate two hour level data

	psMerge->iCountTwoHour++;

	if(psMerge->iCountTwoHour == MAX_COUNT_TWO_HOUR)
	{
		metis_to_zero(psMerge->daTwoHour,psMerge->iCountTwoHour);			  
	}
}

static VOID *metis_calc_thirty_min(METIS_MERGE_ITEM_S *psMerge,DOUBLE daNewValues[])
{
	metis_calc(psMerge->daThirtyMin,daNewValues,psMerge->iCountThirtyMin);//update thirty min level data

	psMerge->iCountThirtyMin++;

	if(psMerge->iCountThirtyMin == MAX_COUNT_THIRTY_MIN)
	{
		metis_to_zero(psMerge->daThirtyMin,psMerge->iCountThirtyMin);
	}
}
//
static VOID *metis_calc_five_min(METIS_MERGE_ITEM_S *psMerge,DOUBLE dValue)
{
	INT32 i = 0;
	DOUBLE daNewValues[CALC_TYPE_NUM];

	for(;i<CALC_TYPE_NUM;i++)
		daNewValues[i] = dValue;

	metis_calc(psMerge->daFiveMin,daNewValues,psMerge->iCountFiveMin);//update five min  level data

	psMerge->iCountFiveMin++;


	if(psMerge->iCountFiveMin == MAX_COUNT_FIVE_MIN)
	{
		metis_calc_thirty_min(psMerge,daNewValues);
		metis_to_zero(psMerge->daFiveMin,psMerge->iCountFiveMin);
	}

}

static void metis_join_char_array(CHAR *result,CHAR *from)
{
	if (strlen(from)!=0)
	{
		sprintf(result,"%s-%s",result,from);
	}

	return;
}

static METIS_MERGE_ITEM_S *metis_init_merge_item(const CHAR *pcTypeName,const CHAR *pcTablePre,const CHAR *pcTablePos)
{
	METIS_MERGE_ITEM_S *psMerge;

	if(pcTypeName == NULL)
		return NULL;

	psMerge = (METIS_MERGE_ITEM_S *)malloc(sizeof(METIS_MERGE_ITEM_S));
	if(psMerge == NULL)
	{
		printf("Out of space. \n");
		return NULL;
	}
	
	psMerge->iCountFiveMin = 0;
	psMerge->iCountThirtyMin = 0;
	psMerge->iCountTwoHour = 0;
	psMerge->iCountOneDay = 0;

	sstrncpy(psMerge->acName,pcTypeName,strlen(pcTypeName)+1);
	sstrncpy(psMerge->acTablePre,pcTablePre,strlen(pcTablePre)+1);
	sstrncpy(psMerge->acTablePos,pcTablePos,strlen(pcTablePos)+1);


	return psMerge;
}
INT32 metis_merge_data(VALUE_LIST_S *psValueList,c_avl_tree_t *psDataMap)
{
	METIS_MERGE_ITEM_S *psMerge;
	INT32 i = 0;
	if(psDataMap == NULL)
		psDataMap = c_avl_create((INT32(*)(const VOID *,const VOID *))strcmp);

	if(psDataMap == NULL)
	{
		printf("Out of space .\n");
		return -1;
	}

	if(psValueList == NULL)
		return -1;

	for(;i<psValueList->values_len;i++)
	{
		CHAR pcTypeName[DATA_MAX_NAME_LEN * 3];
		metis_join_char_array(pcTypeName,psValueList->host);
		metis_join_char_array(pcTypeName,psValueList->type);

		if(psValueList->values_len > 1)
			metis_join_char_array(pcTypeName,psValueList->values[i].name);
		
		if(c_avl_get(psDataMap,pcTypeName,(void *)&psMerge) != 0)
		{
			psMerge = metis_init_merge_item(pcTypeName,psValueList->plugin,psValueList->type_instance);
			if(psMerge == NULL)
				return;
			psMerge->tUpdateTime = CDTIME_T_TO_TIME_T(psValueList->TIME_T);
			c_avl_insert(psDataMap,pcTypeName,psMerge);
		}

		metis_calc_five_min(psMerge,psValueList->values[i].value);
	}
}
