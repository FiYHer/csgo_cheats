#pragma once
#include "help_func.hpp"

//游戏引擎类
class engine_class
{
public:
	//获取本地玩家信息
	constexpr auto get_local_player() noexcept
	{
		return call_virtual_method<int>(this, 12);
	}

	//判断是否在游戏中
	constexpr auto is_in_game() noexcept
	{
		return call_virtual_method<bool>(this, 26);
	}
};