#include"node.h"
#include<sys/time.h>

uint32_t 
node_age(node_t *node)
{
	struct timeval time_now = {0};

	gettimeofday(&time_now,NULL);

	return time.now.tv_sec * 1000 + time_now.tv_usec/1000-node->node_timestamp;
}
