#pragma once
#include "help_func.hpp"
#include "client_class.hpp"

//�ͻ���
class client_class
{
public:
	//��ȡ���е���
	constexpr auto get_all_classes() noexcept
	{
		return call_virtual_method<client_class_struct*>(this, 8);
	}
};