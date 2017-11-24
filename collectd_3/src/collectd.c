#include"collectd.h"
struct part_header_s{
	uint16_t type;
	uint16_t length;
};
typedef struct part_header_s part_header_t;
typedef struct part_header_s PART_HEADER_S;

struct part_string_s{
	part_header_t *head;
	char *value;
};
typedef struct part_string_s part_string_t;
typedef struct part_string_s PART_STRING_S;

struct part_number_s {
	part_header_t *head;
	uint64_t *value;

};
typedef struct part_number_s part_number_t;
typedef struct part_number_s PART_NUMBER_T;

struct part_values_s {
	part_header_t *head;
	uint16_t *num_values;
	uint8_t *values_types;
	value_t *values;

};
typedef struct part_values_s part_values_t;

static CHAR send_buffer[MAX_PACKAGE_LENGTH];
static CHAR *send_buffer_ptr;
static INT32 send_buffer_fill;
static cdtime_t send_buffer_last_update;
static value_list_t send_buffer_vl;




static INT32 metis_write_part_number(CHAR **ret_buffer,INT32 *ret_buffer_len,
		INT32 type,UINT64 value)
{

	char *packet_ptr;
	size_t packet_len;

	PART_HEADER_S pkg_head;
	UINT64 pkg_value;

	size_t offset;

	packet_len = sizeof(pkg_head) + sizeof(pkg_value);

	if(*ret_buffer_len < packet_len)
		return -1;

	pkg_head.type = htons(type);
	pkg_head.length = htons(packet_len);
	pkg_value = htonll(value);

	packet_ptr = *ret_buffer;
	offset = 0;
	memcpy(packet_ptr+offset,&pkg_head,sizeof(pkg_head));
	offset += sizeof(pkg_head);
	memcpy(packet_ptr + offset, &pkg_value, sizeof(pkg_value));
	offset += sizeof(pkg_value);

	assert(offset == packet_len);

	*ret_buffer = packet_ptr + packet_len;
	*ret_buffer_len -= packet_len;

	return 0;	
}
static INT32 metis_write_values(CHAR **ret_buffer,INT32 *ret_buffer_len,const data_set_t *ds,const value_list_t  *vl)
{
	char *packet_ptr;
	INT32 packet_len;
	INT32 num_values;

	PART_HEADER_S pkg_head;
	uint16_t pkg_num_values;
	uint8_t *pkg_values_types;
	value_type_t *pkg_values;

	INT32 offset = 0;
	INT32 i = 0;

	num_values = vl->values_len;

	packet_len = sizeof(PART_HEADER_S) + sizeof(uint16_t)+
		+ num_values * sizeof(uint8_t) + num_values * sizeof(value_type_t);
	printf("%d\n",sizeof(uint16_t));
	if(*ret_buffer_len < packet_len)
		return;

	pkg_values_types = malloc(num_values * sizeof(pkg_values_types));
	if(pkg_values_types == NULL)
	{
		printf("out of space.\n");
		return -1;
	}

	pkg_values = malloc(num_values * sizeof(value_type_t));
	if(pkg_values == NULL)
	{
		sfree(pkg_values);
		printf("out of space.\n");
		return -1;
	}
	printf("vl->values[i].value%f\n",vl->values[i].value);

	pkg_head.type = htons(TYPE_VALUES);
	pkg_head.length = htons(packet_len);
	pkg_num_values = htons((uint16_t)vl->values_len);

	for(;i<num_values;i++)
	{
		pkg_values_types[i] = (uint8_t)ds->ds[i].type;
		switch(ds->ds[i].type)
		{
			case DS_TYPE_COUNTER:
				pkg_values[i].counter = htonll((counter_t)vl->values[i].value);
				break;

			case DS_TYPE_GAUGE:
				pkg_values[i].gauge = vl->values[i].value;
				break;

			case DS_TYPE_DERIVE:
				pkg_values[i].derive = htonll((derive_t)vl->values[i].value);
				break;

			case DS_TYPE_ABSOLUTE:
				pkg_values[i].absolute = htonll((absolute_t)vl->values[i].value);
				break;
			default:
				free(pkg_values);
				free(pkg_values_types);
				return -1;
		}
	}

	packet_ptr = *ret_buffer;
	offset = 0;
	memcpy(packet_ptr + offset, &pkg_head, sizeof(pkg_head));
	offset += sizeof(pkg_head);

	memcpy(packet_ptr + offset, &pkg_num_values, sizeof(pkg_num_values));
	offset += sizeof(pkg_num_values);

	memcpy(packet_ptr + offset, pkg_values_types, num_values * sizeof(uint8_t));
	offset += num_values * sizeof(uint8_t);

	memcpy(packet_ptr + offset, pkg_values, num_values * sizeof(value_type_t));
	offset += num_values * sizeof(value_type_t);

	assert(offset == packet_len);

	*ret_buffer = packet_ptr + packet_len;
	*ret_buffer_len -= packet_len;

	free(pkg_values_types);
	free(pkg_values);

	return 0;

}

static INT32 metis_write_part_string(CHAR **ret_buffer,INT32 *ret_buffer_len,
		INT32 type,const CHAR *str,SIZE_T str_len)
{
	char *buffer;
	size_t buffer_len;

	PART_HEADER_S pkg_head;

	INT32 offset;

	buffer_len = sizeof(pkg_head) + str_len + 1;
	if(*ret_buffer_len < buffer_len)
		return -1;

	pkg_head.type = htons(type);
	pkg_head.length = htons(buffer_len);

	buffer = *ret_buffer;
	offset = 0;
	memcpy(buffer+offset,&pkg_head,sizeof(pkg_head));
	offset += sizeof(pkg_head);
	memcpy(buffer+offset,str,str_len);
	offset += str_len;
	memset(buffer + offset, '\0', 1);
	offset += 1;

	assert(offset == buffer_len);
	*ret_buffer = buffer + buffer_len;
	*ret_buffer_len -= buffer_len;

	return 0;
}
static INT32 metis_add_to_buffer(CHAR *buffer,INT32 buffer_size,value_list_t *vl_def,const data_set_t *ds,const value_list_t *vl){
	char *buffer_orig = buffer;
	if(strcmp(vl_def->host,vl->host) != 0)
	{
		if(metis_write_part_string(&buffer,&buffer_size,TYPE_HOST,vl->host,strlen(vl->host)) != 0)
			return -1;
		sstrncpy(vl_def->host, vl->host, sizeof(vl_def->host));
	}

	if (vl_def->time != vl->time) {
		if (metis_write_part_number(&buffer, &buffer_size, TYPE_TIME_HR,
					(uint64_t)vl->time))
			return -1;
		vl_def->time = vl->time;
	}
	if (vl_def->interval != vl->interval) {
		if (metis_write_part_number(&buffer, &buffer_size, TYPE_INTERVAL_HR,
					(uint64_t)vl->interval))
			return -1;
		vl_def->interval = vl->interval;
	}

	if (strcmp(vl_def->plugin, vl->plugin) != 0) {
		if (metis_write_part_string(&buffer, &buffer_size, TYPE_PLUGIN, vl->plugin,
					strlen(vl->plugin)) != 0)
			return -1;
		sstrncpy(vl_def->plugin, vl->plugin, sizeof(vl_def->plugin));
	}

	if (strcmp(vl_def->plugin_instance, vl->plugin_instance) != 0) {
		if (metis_write_part_string(&buffer, &buffer_size, TYPE_PLUGIN_INSTANCE,
					vl->plugin_instance,
					strlen(vl->plugin_instance)) != 0)
			return -1;
		sstrncpy(vl_def->plugin_instance, vl->plugin_instance,sizeof(vl_def->type));
	}

	if (strcmp(vl_def->type, vl->type) != 0) {
		if (metis_write_part_string(&buffer, &buffer_size, TYPE_TYPE, vl->type,
					strlen(vl->type)) != 0)
			return -1;
		sstrncpy(vl_def->type, vl->type, sizeof(vl_def->type));
	}

	if (strcmp(vl_def->type_instance, vl->type_instance) != 0) {
		if (metis_write_part_string(&buffer, &buffer_size, TYPE_TYPE_INSTANCE,
					vl->type_instance, strlen(vl->type_instance)) != 0)
			return -1;
		sstrncpy(vl_def->type_instance, vl->type_instance,
				sizeof(vl_def->type_instance));
	}

	if (metis_write_values(&buffer, &buffer_size, ds, vl) != 0)
		return -1;
	return buffer - buffer_orig;
}
static void metis_init_buffer()
{
	memset(send_buffer,'\0',MAX_PACKAGE_LENGTH);
	send_buffer_ptr = send_buffer;
	send_buffer_fill = 0;
	send_buffer_last_update = 0;
}
static void metis_flush_buffer(INT32 iFd, ADDR_IN inAddr, USHORT usPort)
{
	udp_send(iFd,inAddr,usPort,send_buffer,(USHORT)send_buffer_fill);
	metis_init_buffer();
}
INT32 metis_write_udp(const data_set_t *ds,const value_list_t *vl,
		INT32 iFd, ADDR_IN inAddr, USHORT usPort)
{
	INT32 flag = -1;

	flag = metis_add_to_buffer(send_buffer_ptr,MAX_PACKAGE_LENGTH - send_buffer_fill,&send_buffer_vl,ds,vl);
	if(flag >= 0)
	{
		send_buffer_fill += flag;
		send_buffer_ptr += flag;
	}else
	{
		metis_flush_buffer(iFd,inAddr,usPort);
		flag = metis_add_to_buffer(send_buffer_ptr,MAX_PACKAGE_LENGTH - send_buffer_fill,&send_buffer_vl,ds,vl);
		if(flag >= 0)
		{
			send_buffer_fill += flag;
			send_buffer_ptr += flag;
		}
	}

	return flag; 
}
int parse_part_number(void **ret_buffer, size_t *ret_buffer_len,uint64_t *value) {
	char *buffer = *ret_buffer;
	size_t buffer_len = *ret_buffer_len;

	uint16_t tmp16;
	uint64_t tmp64;
	size_t exp_size = 2 * sizeof(uint16_t) + sizeof(uint64_t);

	uint16_t pkg_length;

	if (buffer_len < exp_size) {
		printf("network plugin: parse_part_number: "
				"packet too short: "
				"chunk of size %zu expected, "
				"but buffer has only %zu bytes left.",
				exp_size, buffer_len);
		return -1; 


	}

	memcpy((void *)&tmp16, buffer, sizeof(tmp16));
	buffer += sizeof(tmp16);
	/* pkg_type = ntohs (tmp16); */

	memcpy((void *)&tmp16, buffer, sizeof(tmp16));
	buffer += sizeof(tmp16);
	pkg_length = ntohs(tmp16);

	memcpy((void *)&tmp64, buffer, sizeof(tmp64));
	buffer += sizeof(tmp64);
	*value = ntohll(tmp64);

	*ret_buffer = buffer;
	*ret_buffer_len = buffer_len - pkg_length;

	return 0;

} /* int parse_part_number */


static int parse_part_string(void **ret_buffer,size_t *ret_buffer_len,char *output,size_t const output_len){
	char *buffer = *ret_buffer;
	size_t buffer_len = *ret_buffer_len;

	uint16_t tmp16;
	size_t header_size = 2*sizeof(uint16_t);

	uint16_t pkg_length;
	size_t payload_size;

	if(output_len == 0){
		return -2;
	}
	if(buffer_len <  header_size){
		printf("pase_part_string:packet too short :chunk of at least size %zu expected,but buffer has only %zu bytes left\n",
				header_size,buffer_len);
		return -1;
	}
	memcpy((void *)&tmp16,buffer,sizeof(tmp16));
	buffer += sizeof(tmp16);

	memcpy((void *)&tmp16,buffer,sizeof(tmp16));
	buffer += sizeof(tmp16);
	pkg_length = ntohs(tmp16);//记录该字节流的长度

	payload_size = ((size_t)pkg_length) - header_size;//实际字符串的长度

	if(pkg_length >buffer_len){
		printf("parse_part_string:packet too short: chunk of size %zu received, but buffer has only %zu bytes left.\n",pkg_length,buffer_len);
		return -1;
	}

	if(pkg_length <= header_size){
		printf("parse_part_string:packet too short:header claims this packet is only %hu bytes long\n",pkg_length);
		return -1;
	}

	if (output_len < payload_size) {
		printf("network plugin: parse_part_string: "
				"buffer too small: "
				"output buffer holds %zu bytes, "
				"which is too small to hold the received "
				"%zu byte string.\n",
				output_len, payload_size);
		return -1;
	}

	memcpy((void*)output,(void *)buffer,payload_size);
	buffer+=payload_size;

	if (output[payload_size - 1] != 0) { 
		printf("network plugin: parse_part_string: "
				"received string does not end "
				"with a NULL-byte.\n");
		return -1;
	}

	*ret_buffer = buffer;
	*ret_buffer_len = buffer_len - pkg_length;
	return 0;

}
static int parse_part_values(void **ret_buffer,size_t *ret_buffer_len,value_t **ret_values,size_t *ret_num_values){

	char *buffer = *ret_buffer;
	size_t buffer_len = *ret_buffer_len;

	uint16_t tmp16;
	size_t exp_size;

	uint16_t pkg_length;
	uint16_t pkg_type;
	size_t pkg_numval;

	uint8_t *pkg_types;
	value_t *pkg_values;
	value_type_t *pkg_values_type;
	//type 0x 0006
	//length 16bite 
	//number of values 16 bite
	//values for each one
	//data type code(8 bite field)
	//counter -> 0
	//gauge -> 1
	//derive -> 2
	//absolute ->3
	//value(64 bit field)
	//counter->network (big endin) unsigned integer
	//gauge ->x86(little endian) double
	//derive -> networkk(big endian) signed integer
	//absolute -> network9(big endian) unsigned integer
	//2+2+2+1+8=15
	if(buffer_len < 15){
		printf("packet is too short:buffer_len = %zu\n",buffer_len);
	}

	memcpy((void*)&tmp16,buffer,sizeof(tmp16));
	buffer += sizeof(tmp16);
	pkg_type = ntohs(tmp16);

	memcpy((void*)&tmp16,buffer,sizeof(tmp16));
	buffer += sizeof(tmp16);
	pkg_length = ntohs(tmp16);


	memcpy((void*)&tmp16,buffer,sizeof(tmp16));
	buffer += sizeof(tmp16);
	pkg_numval = ntohs(tmp16);

	assert(pkg_type == TYPE_VALUES);

	exp_size = 3 * sizeof(uint16_t) + pkg_numval*(sizeof(uint8_t)+sizeof(double));

	if (pkg_length != exp_size) {
		printf("parse_part_values: "
				"length and number of values "
				"in the packet don't match.\n");
		return -1;

	}

	assert(pkg_numval <= ((buffer_len - 6) / 9));
	if(pkg_length != exp_size){
		printf("parse_part_values: "
				"length and number of values "
				"in the packet don't match.\n");
		return -1;

	}

	pkg_types = calloc(pkg_numval, sizeof(*pkg_types));
	pkg_values = calloc(pkg_numval, sizeof(*pkg_values));
	pkg_values_type = calloc(pkg_numval,sizeof(*pkg_values_type));

	if ((pkg_types == NULL) || (pkg_values == NULL)) {
		sfree(pkg_types);
		sfree(pkg_values);
		sfree(pkg_values_type);
		printf(" parse_part_values: calloc failed.\n");
		return -1;
	}

	memcpy(pkg_types,buffer,pkg_numval*sizeof(*pkg_types));
	buffer += pkg_numval * sizeof(*pkg_types);
	memcpy(pkg_values_type, buffer, pkg_numval * sizeof(*pkg_values_type));
	size_t i = 0;
	for(;i<pkg_numval;i++){
		memcpy(&pkg_values[i].value, buffer, sizeof(pkg_values[i].value));
		buffer += sizeof(pkg_values[i].value);
	}
	for(i = 0;i < pkg_numval;i++){
		switch(pkg_types[i]){
			case DS_TYPE_COUNTER:
				pkg_values[i].value = (double)ntohll(pkg_values[i].value);
				pkg_values[i].type = DS_TYPE_COUNTER;
				break;
			case DS_TYPE_GAUGE:
				pkg_values[i].value = (double)htond(pkg_values[i].value);
				pkg_values[i].type = DS_TYPE_GAUGE;
				break;
			case DS_TYPE_DERIVE:
				pkg_values[i].type = DS_TYPE_DERIVE;
				pkg_values[i].value = (double)ntohll(pkg_values[i].value);
				break;
			case DS_TYPE_ABSOLUTE:
				pkg_values[i].type = DS_TYPE_ABSOLUTE;
				pkg_values[i].value = (double)ntohll(pkg_values[i].value);
				break;
			default:
				printf(" parse_part_values: "
						"don't know how to handle data source type %zd\n" ,
						pkg_types[i]);
				sfree(pkg_types);
				sfree(pkg_values);
				return -1;
		}
	}
	*ret_buffer = buffer;
	*ret_buffer_len = buffer_len - pkg_length;
	*ret_num_values = pkg_numval;
	*ret_values = pkg_values;

	sfree(pkg_types);

	return 0;
}
static int parse_packet(void *buffer,size_t buffer_size,value_list_t *vls[]){
	read_types_list("/opt/collectd_3/src/types.db");
	int status = 0;
	int index = 0;
	value_list_t vl = {NULL};
	vls[index] = calloc(1,sizeof(*vls[0]));
	if (vls[index] == NULL){
		free_values_lists(vls,index+1);
		return -1;
	}
	while((status == 0) &&(0<buffer_size) &&((unsigned int)buffer_size > sizeof(part_header_t))){
		uint16_t pkg_type;
		uint16_t pkg_length;

		memcpy((void *)&pkg_type, (void *)buffer, sizeof(pkg_type));
		memcpy((void *)&pkg_length, (void *)(((char *)buffer) + sizeof(pkg_type)),
				sizeof(pkg_length));

		pkg_length = ntohs(pkg_length);
		pkg_type = ntohs(pkg_type);

		if (pkg_length > buffer_size)
			break;
		///* ensure that this loop terminates eventually */
		if (pkg_length < (2 * sizeof(uint16_t)))
			break;
		if (pkg_type == TYPE_HOST) {
			status = parse_part_string(&buffer, &buffer_size, vl.host,
					sizeof(vl.host));
		}
		else if(pkg_type == TYPE_TIME){
			uint64_t tmp = 0;
			status = parse_part_number(&buffer,&buffer_size,&tmp);
			if(status == 0){
				vl.time = TIME_T_TO_CDTIME_T(tmp);
			}
		}else if(pkg_type == TYPE_TIME_HR){
			uint64_t tmp = 0;
			status = parse_part_number(&buffer,&buffer_size,&tmp);
			if(status == 0){
				vl.time = (cdtime_t)tmp;
			}
		}else if (pkg_type == TYPE_INTERVAL) {

			uint64_t tmp = 0; 
			status = parse_part_number(&buffer, &buffer_size, &tmp);
			if (status == 0){
				vl.interval = TIME_T_TO_CDTIME_T(tmp);
			}
		} else if (pkg_type == TYPE_INTERVAL_HR) {
			uint64_t tmp = 0; 
			status = parse_part_number(&buffer, &buffer_size, &tmp);
			if (status == 0){
				vl.interval = (cdtime_t)tmp;
			}
		} else if (pkg_type == TYPE_PLUGIN) {
			status = parse_part_string(&buffer, &buffer_size, vl.plugin,
					sizeof(vl.plugin));
		} else if (pkg_type == TYPE_PLUGIN_INSTANCE) {
			status = parse_part_string(&buffer, &buffer_size, vl.plugin_instance,
					sizeof(vl.plugin_instance));
		} else if (pkg_type == TYPE_TYPE) {
			status =
				parse_part_string(&buffer, &buffer_size, vl.type, sizeof(vl.type));
		}else if(pkg_type == TYPE_TYPE_INSTANCE){
			status =
				parse_part_string(&buffer, &buffer_size, vl.type_instance, sizeof(vl.type_instance));
		}else if(pkg_type == TYPE_VALUES){			
			sstrncpy(vls[index]->host, vl.host,sizeof(vl.host));//copy host
			vls[index]->time = TIME_T_TO_CDTIME_T(vl.time);
			vls[index]->interval = TIME_T_TO_CDTIME_T(vl.interval);

			sstrncpy(vls[index]->plugin, vl.plugin, sizeof(vl.plugin));
			sstrncpy(vls[index]->plugin_instance, vl.plugin_instance, sizeof(vl.plugin_instance));
			sstrncpy(vls[index]->type, vl.type, sizeof(vl.type));
			sstrncpy(vls[index]->type_instance, vl.type_instance, sizeof(vl.type_instance));



			status = parse_part_values(&buffer,&buffer_size,&vl.values,&vl.values_len);
			if(status != 0){
				break;
			}
			vls[index]->values_len = vl.values_len;
			vls[index]->values = (value_t *)calloc(vls[index]->values_len,sizeof(value_t));
			if(vls[index]->values == NULL){
				free_values_lists(vls,index+1);
				return -1;
			}
			int i = 0;
			for(;i<vl.values_len;i++){
				(vls[index]->values)[i].value = vl.values[i].value;
				(vls[index]->values)[i].type = vl.values[i].type;
				if(strcmp(vls[index]->type,"cpu") == 0){
				}
			}
			set_value_name(vls[index],&vl);

			sfree(vl.values);
			index++;

			vls[index] = calloc(1,sizeof(*vls[index]));
			if (vls[index] == NULL){
				free_values_lists(vls,index+1);
				return -1;
			}
		}else{
			buffer = ((char *)buffer) + pkg_length;
			buffer_size -= (size_t)pkg_length;
		}

	}
	plugin_free_data_sets();
	if(index == 0){
		return -1;
	}
	return 0;
}
int free_values_lists(value_list_t *vls[],int len){
	int i = 0;
	for(;i<len;i++){
		sfree(vls[i]->values);
		sfree(vls[i]);
	}
}
INT32 udp_send(INT32 iFd, ADDR_IN inAddr, USHORT usPort, CHAR *pcBuffer, USHORT usSize)
{
	SOCK_IN_S  stAddr;
	INT32 iRet = -1;

	stAddr.sin_family = AF_INET;
	stAddr.sin_port = htons(usPort);
	stAddr.sin_addr.s_addr = inet_addr("172.16.56.102");

	iRet = sendto(iFd, pcBuffer, usSize, 0, (SOCKADDR_S *)&stAddr, sizeof(SOCK_IN_S));
	while (0 > iRet && 11 == errno)
	{   
		iRet = sendto(iFd, pcBuffer, usSize, 0, (SOCKADDR_S *)&stAddr, sizeof(SOCK_IN_S));
	}   

	return iRet;

}

//int main(){
//	int sock_fd = 0;
//	struct sockaddr_in addr_serv;
//	struct sockaddr_in addr_client;
//	sock_fd = socket(AF_INET,SOCK_DGRAM,0);
//	if(sock_fd < 0){ 
//		perror("socket");
//		exit(1);
//	}else{
//		printf("sock sucessful\n");
//
//	}
//	//初始化服务器端地址
//	memset(&addr_serv,0,sizeof(struct sockaddr_in));
//	memset(&addr_client,0,sizeof(struct sockaddr_in));
//
//	addr_serv.sin_family = AF_INET;//协议族
//	addr_serv.sin_port = htons(8000);
//	addr_serv.sin_addr.s_addr = inet_addr("172.16.56.102");
//
//	addr_client.sin_family = AF_INET;//协议族
//	addr_client.sin_port = htons(9090);
//	addr_client.sin_addr.s_addr = inet_addr("172.16.56.102");
//
//
//	if(bind(sock_fd,(struct sockaddr *)&addr_serv,sizeof(struct sockaddr_in))<0 ){
//		perror("bind");
//		exit(1);
//	}else{
//		printf("bind sucess \n");
//	}
//
//	int dest_len = sizeof(struct sockaddr_in);
//	char *send_buf = "kkkkkkkkkkkkkkkkkkkk";
//	//sendto(sock_fd,send_buf,strlen(send_buf),0,(struct sockaddr*)&addr_client,dest_len);
//
//	read_types_list("/opt/collectd_3/src/types.db");
//	metis_init_buffer();
//	value_list_t vl = VALUE_LIST_INIT;
//	char* hostname = "clusterId_hostId_vmId";
//	char* type = "cpu";
//	char*  type_instance = "usage";
//
//	value_t *values = (value_t *)malloc(sizeof(value_t) * 1);
//	values[0].value = 55.00;
//	vl.values = values;
//	
//	sstrncpy(vl.host,hostname,strlen(hostname) + 1);
//	sstrncpy(vl.type,type,strlen(type) + 1);
//	sstrncpy(vl.type_instance,type_instance,strlen(type_instance) + 1);
//	vl.values_len = 1;
//
//	data_set_t *ds = plugin_get_data_set("cpu");
//	if(ds == NULL)
//	{
//		printf("ds is null\n");
//	}
//	ADDR_IN addr_in = inet_addr("172.16.56.102");
//	int m = 2000;
//		while(m > 0){
//			int status = metis_write_udp(ds,&vl,sock_fd,addr_in,9090);
//			m--;
//		}
//	//sendto(sock_fd,send_buffer,send_buffer_fill,0,(struct sockaddr*)&addr_client,dest_len);
//	metis_flush_buffer(sock_fd,0,9090);
//	
//	sfree(values);
//}
//int main(int argc, char** argv) { 
//	int sock_fd;////套接子描述符号
//	int addr_len = sizeof(struct sockaddr_in);
//	char *buf;
//	buf = (char *)malloc(sizeof(char)*1452);
//	struct sockaddr_in addr_client;
//	struct sockaddr_in addr_serv;
//	sock_fd = socket(AF_INET,SOCK_DGRAM,0);
//	if(sock_fd < 0){
//		perror("socket");
//		exit(1);
//	}else{
//		printf("sock sucessful\n");
//	}
//	//初始化服务器端地址
//	memset(&addr_client,0,sizeof(struct sockaddr_in));
//	memset(&addr_serv,0,sizeof(struct sockaddr_in));
//	memset(buf,-1,sizeof(char)*1452);
//
//
//	addr_client.sin_family = AF_INET;//协议族
//	addr_client.sin_port = htons(9000);
//	addr_client.sin_addr.s_addr = htonl(INADDR_ANY);//任意本地址
//
//	addr_serv.sin_family = AF_INET;//协议族
//	addr_serv.sin_port = htons(8000);
//	addr_serv.sin_addr.s_addr =inet_addr("172.16.24.240"); ;//任意本地址
//
//	if(bind(sock_fd,(struct sockaddr *)&addr_client,sizeof(struct sockaddr_in))<0 ){
//		perror("bind");
//		exit(1);
//	}else{
//		printf("bind sucess \n");
//	}
//	int t = 0;
//
//	int i  = 0;
//	while(1){
//		int ret=recvfrom(sock_fd,buf,1452,0 ,NULL,NULL);
//		size_t i = 0;
//		for(;i<1452;i++){
//			if(buf[i] == -1){
//				break;
//			}
//		}
//		value_list_t *vls[128];
//		int k = parse_packet((void *)buf,i,vls);
//		printf("%d\n=========================",k);
//		if (k == 0){
//			for(i = 0;i<3;i++){
//			vls[i]->time = 1616765706148249600+t*10737418240;
//			vls[i]->interval = 10737418240;
//
//			}
//			printf("write_value_to_file\n");
//			write_value_to_file(vls,"/opt/collectd_3/src");
//			free_values_lists(vls,3);
//
//		}
//		t++;
//		sleep(1);
//		printf("%d\n",t);
//
//		memset(buf,-1,sizeof(char)*1452);
//	}
//
//	free(buf);
//	close(sock_fd);
//}
