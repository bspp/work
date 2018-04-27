
#ifndef _GOSSIP_NODE
#define _GOSSIP_NODE
#ifdef ___cplusplus
extern "C" {
#endif

#include<stdint.h>

#define NODE_IP_LEN 16

	typedef enum ping_error{
		ping_no_data = -1,
		ping_time_out = -2
	}ping_error_t;

	typedef enum node_status{
		//Unknown is the default node status of newly-created nodes.
		node_status_unknown = 0,
		//Alive indicates that a node is alive and healthy.
		node_status_alive,
		//suspected ndicatates that a node is suspected of being dead
		node_status_suspected,
		//Dead indicatates that a node is dead and no longer healthy.
		node_status_dead,
		//ForwardTo is a pseudo status used by message to indicate the target of a ping request
		node_status_forward_to
	}node_status_t;

	typedef struct node{
		char node_ip[16];
		uint16_t node_port;
		uint32_t node_timestamp;
		int32_t  node_pint_millis;
		node_status_t node_statu;
		int8_t node_emit_counter;
		uint32_t node_heartbeat;
		struct node *node_status_source;
	}node_t;

#ifdef ___cplusplus
}
#endif
#endif
