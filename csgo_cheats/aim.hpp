#pragma once
#include "config.hpp"
#include "user_cmd.hpp"

//自瞄命名空间
namespace aim_space
{
	//开启自瞄
	void run(user_cmd_struct* cmd) noexcept
	{
		//没有开启自瞄
		if (!g_config.control.aim) return;

		//获取自己
		const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player());
		if (!local_player || !local_player->is_alive()) return;

		//获取自身矩阵信息
		float self_matrix[4][4];
		g_config.engine->world_to_screen_matrix();

		//遍历玩家
		for (int i = 1; i < g_config.engine->get_max_clients(); i++)
		{
			//获取玩家对象
			auto entity = g_config.entity_list->get_entity(i);
			if(!entity || entity == local_player) continue;

			//如果玩家死亡了
			if (!entity->is_alive()) return;

			//如果玩家是队友
			if (!entity->is_enemy()) return;





		}
	}


}