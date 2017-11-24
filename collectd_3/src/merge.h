#include<stdio.h>
#include"collectd.h"
#include "plugin.h"
#include "utils_avltree.h"

#ifndef MERAGE_H
#define MERAGE_H
#define CALC_TYPE_NUM 5

#define MAX_COUNT_FIVE_MIN 15
#define MAX_COUNT_THIRTY_MIN 6
#define MAX_COUNT_TWO_HOUR 4
#define MAX_COUNT_ONE_DEY 12

struct metis_merge_item{
	CHAR acName[DATA_MAX_NAME_LEN * 3];
	CHAR acTablePre[DATA_MAX_NAME_LEN];
	CHAR acTablePos[DATA_MAX_NAME_LEN];
	TIME_T tUpdateTime;
  
	// max min ave sum new
	DOUBLE daFiveMin[5];
	DOUBLE daThirtyMin[5];
	DOUBLE daTwoHour[5];
	DOUBLE daOneDay[5];


	INT32 iCountFiveMin;
	INT32 iCountThirtyMin;
	INT32 iCountTwoHour;
	INT32 iCountOneDay;
};
typedef struct metis_merge_item METIS_MERGE_ITEM_S;
INT32 metis_merge_data(VALUE_LIST_S *sValueList,c_avl_tree_t *psDataMap);
#endif


