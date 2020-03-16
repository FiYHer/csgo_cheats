#pragma once
#include "help_func.hpp"
#include "player_info.hpp"

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

	//获取指定索引得人物信息
	constexpr auto get_player_info(int entityIndex, const player_info_struct& playerInfo) noexcept
	{
		return call_virtual_method<bool, int, const player_info_struct&>(this, 8, entityIndex, playerInfo);
	}

	//根据用户ID获取玩家
	constexpr auto get_player_for_user_id(int userId) noexcept
	{
		return call_virtual_method<int, int>(this, 9, userId);
	}

};