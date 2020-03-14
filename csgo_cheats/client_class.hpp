#pragma once
#include <windows.h>
#include <type_traits>
#include "entity.hpp"
#include "rect_table.hpp"
#include "class_id.hpp"

//�ͻ���ṹ
typedef struct client_class_struct
{
	//std::add_pointer_t<entity_class* __cdecl(int, int)> createFunction;//���׸�����Ҫ
	std::add_pointer_t<int* __cdecl(int, int)> create_function;//���׸�����Ҫ
	void* create_event_function;//�����¼�����
	char* network_name;//��������
	recv_table_struct* recv_table;//
	client_class_struct* next;
	classId_enum classId;
}client_class_struct;
