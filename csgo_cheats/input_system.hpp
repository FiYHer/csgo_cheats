#pragma once
#include "help_func.hpp"

class input_system_class
{
public:
	//启用游戏输入
	constexpr void enable_input(bool enable) noexcept
	{
		call_virtual_method<void, bool>(this, 11, enable);
	}

	//重置输入状态
	constexpr void reset_input_state() noexcept
	{
		call_virtual_method<void>(this, 39);
	}
};

