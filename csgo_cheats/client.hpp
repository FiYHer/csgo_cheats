#pragma once
#include "help_func.hpp"
#include "client_class.hpp"

//客户类
class client_class
{
public:
	//获取所有的类
	constexpr auto get_all_classes() noexcept
	{
		return call_virtual_method<client_class_struct*>(this, 8);
	}
};