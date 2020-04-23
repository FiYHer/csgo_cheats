#pragma once
#include "help_func.hpp"

class input_system_class
{
public:
	//������Ϸ����
	constexpr void enable_input(bool enable) noexcept
	{
		call_virtual_method<void, bool>(this, 11, enable);
	}

	//��������״̬
	constexpr void reset_input_state() noexcept
	{
		call_virtual_method<void>(this, 39);
	}
};

