#pragma once
#include "recv_prop.hpp"

//�������ݱ�ṹ
typedef struct recv_table_struct
{
	recv_prop_struct* props;
	int propCount;
	void* decoder;
	char* netTableName;
	bool isInitialized;
	bool isInMainList;
}recv_table_struct;