#pragma once
#include "help_func.hpp"

class model_info_class
{
public:
	//��ȡģ����Ϣ
	constexpr auto get_model_index(const char* name) noexcept
	{
		return call_virtual_method<int, const char*>(this, 2, name);
	}
};