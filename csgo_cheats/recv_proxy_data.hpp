#pragma once
#include "self_vector.hpp"
 
//结构数据结构
typedef struct recv_proxy_data_struct
{
	int pad;
	union 
	{
		float _float;
		long _int;
		char* _string;
		void* data;
		self_vector_struct vector;
		int64_t int64;
	} value;
}recv_proxy_data_struct;

using recv_proxy = std::add_pointer_t<void(recv_proxy_data_struct&, void*, void*)>;