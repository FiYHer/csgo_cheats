#pragma once
#include "con_var.hpp"

class cvar_class 
{
public:
	//����ָ��ֵ
	constexpr auto findVar(const char* name) noexcept
	{
		return call_virtual_method<con_var_struct*, const char*>(this, 15, name);
	}
};
