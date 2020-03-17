#pragma once
#include "help_func.hpp"
#include "entity.h"

//ʵ���б���
class entity_list_class
{
public:
	//��ȡָ������ʵ��
	constexpr auto get_entity(int index) noexcept
	{
		return call_virtual_method<entity_class*, int>(this, 3, index);
	}

	//���ݾ����ȡʵ��
	constexpr auto get_entity_from_handle(int handle) noexcept
	{
		return call_virtual_method<entity_class*, int>(this, 4, handle);
	}
};