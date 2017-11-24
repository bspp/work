#include<stdlib.h>
#include"utils_avltree.h"
#include<string.h>
#include<malloc.h>
#include<time.h>
int compare(int *a,int *b){
	if( *((int*)a) == *((int *)b) )
	{
		return 0;
	}else if(*((int*)a) < *((int *)b))
	{
		return -1;
	}else{
		return 1;
	}
	return 1;
}
int main(){
	c_avl_tree_t *data_sets;
	data_sets = c_avl_create((int(*)(const void *,const void *))compare);
	if(data_sets == NULL)
	{
		return -1;
	}
	int i = 0;
	struct timeval  start,end;
	long iTimeInterval;

	gettimeofday(&start,NULL);
	for(;i<10000;i++)
	{
		int *p = (int *)malloc(sizeof(int));
		*p = rand();
		c_avl_insert(data_sets,(void*)p,NULL);

	}
	gettimeofday(&end,NULL);
	iTimeInterval = (end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000;
	printf("-------%ld\n",iTimeInterval);

	gettimeofday(&start,NULL);
	for(i=10000;i>=0;i--)
	{ 
		int *p = &i;
		c_avl_get(data_sets,p,NULL);
	}
	gettimeofday(&end,NULL);
	iTimeInterval = (end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000;
	printf("-------%ld\n",iTimeInterval);

	test(data_sets);

	int *p;
	while(c_avl_pick(data_sets,&p,NULL) == 0)
	{
		free(p);
	}
	c_avl_destroy(data_sets);
	return 0;

}
