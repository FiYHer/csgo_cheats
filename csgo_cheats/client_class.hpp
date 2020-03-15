#pragma once
#include <windows.h>
#include <type_traits>
#include "rect_table.hpp"
#include "class_id.hpp"

class entity_class;

//客户类结构
typedef struct client_class_struct
{
	std::add_pointer_t<entity_class* __cdecl(int, int)> createFunction;//手套更换需要
	void* create_event_function;//创建事件函数
	char* network_name;//网络名称
	recv_table_struct* recv_table;//
	client_class_struct* next;
	classId_enum classId;
}client_class_struct;
