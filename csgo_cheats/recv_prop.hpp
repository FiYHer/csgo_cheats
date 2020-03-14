#pragma once
#include "recv_proxy_data.hpp"

typedef struct recv_prop_struct 
{
	char* name;
	int type;
	int flags;
	int stringBufferSize;
	int insideArray;
	const void* extraData;
	recv_prop_struct* arrayProp;
	void* arrayLengthProxy;
	recv_proxy proxy;
	void* dataTableProxy;
	struct RecvTable* dataTable;
	int offset;
	int elementStride;
	int elementCount;
	const char* parentArrayPropName;
}recv_prop_struct;