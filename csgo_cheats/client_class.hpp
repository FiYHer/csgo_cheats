#pragma once
#include <windows.h>
#include <type_traits>
#include "rect_table.hpp"
#include "class_id.hpp"

class entity_class;

//�ͻ���ṹ
typedef struct client_class_struct
{
	std::add_pointer_t<entity_class* __cdecl(int, int)> createFunction;//���׸�����Ҫ
	void* create_event_function;//�����¼�����
	char* network_name;//��������
	recv_table_struct* recv_table;//
	client_class_struct* next;
	classId_enum classId;
}client_class_struct;
