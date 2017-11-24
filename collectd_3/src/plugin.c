#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<assert.h>
#include "plugin.h"
#include "utils_avltree.h"
#include "common.h"
#include "types_list.h"
c_avl_tree_t *data_sets;
int set_value_name(value_list_t *vl_copy,value_list_t *vl){
	assert(vl != NULL);

	data_set_t *ds;

	if(c_avl_get(data_sets,vl->type,(void *)&ds) != 0){
		return -1;
	}

	if (ds->ds_num != vl->values_len) {
		printf("plugin_dispatch_values: ds->type = %s: "
				"(ds->ds_num = %zu) != "
				"(vl->values_len = %zu)",
				ds->type, ds->ds_num, vl->values_len);
		return -1;
	}
	int i = 0;
	for(;i<vl->values_len;i++){
		sstrncpy(vl_copy->values[i].name, ds->ds[i].name, sizeof(ds->ds[i].name));
	}   

}
void plugin_free_data_sets(void){
	void *key;
	void *value;
	if(data_sets == NULL){
		return;
	}
	while(c_avl_pick(data_sets,&key,&value) == 0){
		data_set_t *ds = value;
		sfree(ds->ds);
		sfree(ds);
	}
	c_avl_destroy(data_sets);
	data_sets = NULL;
}
int plugin_unregister_data_set(const char *name){
	data_set_t *ds;
	if(data_sets == NULL)
		return -1;
	if(c_avl_remove(data_sets,name,NULL,(void *)&ds) != 0){
		return -1;
	}
	sfree(ds->ds);
	sfree(ds);
}
int plugin_register_data_set(const data_set_t *ds){//data_sets from plugin.h
	data_set_t *ds_copy;	

	if((data_sets != NULL) && (c_avl_get(data_sets,ds->type,NULL) == 0)){
		printf("replaceing DS `%s` with another version.\n",ds->type);
		plugin_unregister_data_set(ds->type);
	}else if(data_sets == NULL){
		data_sets = c_avl_create((int(*)(const void *,const void *))strcmp);
		if(data_sets == NULL)
			return -1;
	}
	ds_copy = malloc(sizeof(*ds_copy -> ds)*ds->ds_num);
	if(ds_copy == NULL){
		return -1;
	}
	memcpy(ds_copy,ds,sizeof(data_set_t));
	ds_copy->ds = malloc(sizeof(*ds_copy->ds) * ds->ds_num);
	if(ds_copy->ds == NULL){
		sfree(ds_copy);
		return -1;
	}
	size_t i;
	for (i = 0; i < ds->ds_num; i++) 
		memcpy(ds_copy->ds + i, ds->ds + i, sizeof(data_source_t));

	return c_avl_insert(data_sets, (void *)ds_copy->type, (void *)ds_copy);

}
data_set_t *plugin_get_data_set(const char *type)
{
	data_set_t *ds;

	if((data_sets != NULL) && (c_avl_get(data_sets,type,(void *)&ds)) == 0)
	{
		return ds;
	}else{
		return NULL;
	}
}
int type_exit_data_set(const char *type){
	if((data_sets != NULL) && (c_avl_get(data_sets,type,NULL)) == 0){
		return 0;
	}else{
		return -1;
	}
}
