#pragma once
#include "help_func.hpp"

//ʵ���б���
class entity_list_class
{
public:
	//��ȡָ������ʵ��
	constexpr auto get_entity(int index) noexcept
	{
		return call_virtual_method<entity_class*, int>(this, 3, index);
	}

};