#pragma once
#include "help_func.hpp"
#include "player_info.hpp"
#include "self_vector.hpp"

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

	//获取全部游戏玩家数量
	constexpr auto get_max_clients() noexcept
	{
		return call_virtual_method<int>(this, 20);
	}

	//获取自身矩阵
	using self_matrix = float[4][4];
	constexpr auto world_to_screen_matrix() noexcept
	{
		return call_virtual_method<const self_matrix&>(this, 37);
	}

	//获取人物角度
	constexpr auto get_view_angles(self_vector_struct& angles) noexcept
	{
		call_virtual_method<void, self_vector_struct&>(this, 18, angles);
	}

	//设置人物角度
	constexpr auto set_view_angles(const self_vector_struct& angles) noexcept
	{
		call_virtual_method<void, const self_vector_struct&>(this, 19, angles);
	}


};