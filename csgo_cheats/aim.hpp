#pragma once
#include "config.hpp"
#include "user_cmd.hpp"

//自瞄命名空间
namespace aim_space
{
	//自瞄
	void aim_start(user_cmd_struct* cmd, self_vector_struct& local_pos, self_vector_struct& enemy_pos) noexcept
	{
		float x = local_pos.x - enemy_pos.x;
		float y = local_pos.y - enemy_pos.y;
		float z = local_pos.z - enemy_pos.z + 60.0f + g_config.control.aim_offset;

		static const float pi = 3.1415f;
		self_vector_struct angle{ atan(z / sqrt(x * x + y * y)) / pi * 180.f,atan(y / x) ,0 };

		if (x >= 0.0f && y >= 0.0f) angle.y = angle.y / pi * 180.0f - 180.0f;
		else if (x < 0.0f && y >= 0.0f) angle.y = angle.y / pi * 180.0f;
		else if (x < 0.0f && y < 0.0f) angle.y = angle.y / pi * 180.0f;
		else if (x >= 0.0f && y < 0.0f) angle.y = angle.y / pi * 180.f + 180.0f;

		//角度范围
		self_vector_struct old_angle;
		g_config.engine->get_view_angles(old_angle);
		if (abs(angle.x - old_angle.x) > g_config.control.aim_max_angle
			|| abs(angle.y - old_angle.y) > g_config.control.aim_max_angle) return;

		//设置角度
		cmd->view_angles = angle;
		g_config.engine->set_view_angles(cmd->view_angles);

		//开镜
		if (g_config.control.aim_auto_scoped 
			&& !g_config.entity_list->get_entity(g_config.engine->get_local_player())->get_is_scoped())
			cmd->buttons |= user_cmd_struct::IN_ATTACK2;

		//开枪
		if (g_config.control.aim_auto_fire) cmd->buttons |= user_cmd_struct::IN_ATTACK;

		//关闭开镜状态
		if (g_config.control.aim_close_scoped
			&& g_config.entity_list->get_entity(g_config.engine->get_local_player())->get_is_scoped())
			cmd->buttons |= user_cmd_struct::IN_ATTACK2;
	}

	//开启自瞄
	void run(user_cmd_struct* cmd) noexcept
	{
		//没有开启自瞄
		if (!g_config.control.aim) return;

		//获取游戏窗口句柄
		static auto hwnd = FindWindowW(L"Valve001", nullptr);
		int width, height;
		if (RECT rect; GetWindowRect(hwnd, &rect))
		{
			width = (rect.right - rect.left) / 2;
			height = (rect.bottom - rect.top) / 2;
		}

		//获取自己
		const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player());
		if (!local_player || !local_player->is_alive()) return;

		//获取自身矩阵信息
		auto self_matrix = g_config.engine->world_to_screen_matrix();

		//最近玩家位置
		int min_distance = INT_MAX;
		self_vector_struct min_pos;

		//遍历玩家
		for (int i = 1; i < g_config.engine->get_max_clients(); i++)
		{
			//获取玩家对象
			auto entity = g_config.entity_list->get_entity(i);
			if(!entity || entity == local_player) continue;

			//如果玩家死亡了或者是队友
			if (!entity->is_alive() || !entity->is_enemy()) continue;

			//获取敌人位置
			self_vector_struct enemy_pos = entity->get_bone_position();

			//如果玩家被墙体挡住了
			if (!entity->is_visiable(enemy_pos)) continue;

			//朝向
			float target = self_matrix[2][0] * enemy_pos.x
				+ self_matrix[2][1] * enemy_pos.y
				+ self_matrix[2][2] * enemy_pos.z
				+ self_matrix[2][3];
			if(target < 0.01f) continue;
			target = 1.0f / target;

			//将3D坐标转化为2D坐标进行距离判断
			float distence_width = width
				+ (self_matrix[0][0] * enemy_pos.x
					+ self_matrix[0][1] * enemy_pos.y
					+ self_matrix[0][2] * enemy_pos.z
					+ self_matrix[0][3]) * target *width;
			float distance_height = height
				- (self_matrix[1][0] * enemy_pos.x
					+ self_matrix[1][1] * enemy_pos.y
					+ self_matrix[1][2] * enemy_pos.z
					+ self_matrix[1][3]) * target * height;

			//到准星距离
			int distance = abs(width - (int)distence_width) + abs(height - (int)distance_height);
			if (distance < min_distance) 
			{
				min_distance = distance;
				min_pos = enemy_pos;
			}
		}

		//没有玩家
		if (min_distance == INT_MAX) return;

		//开镜自瞄
		if (g_config.control.aim_scoped && local_player->get_is_scoped())
			aim_start(cmd, local_player->get_origin(), min_pos);

		//开枪自瞄
		if (g_config.control.aim_fire && cmd->buttons & user_cmd_struct::IN_ATTACK)
			aim_start(cmd, local_player->get_origin(), min_pos);

		//静步自瞄
		if(g_config.control.aim_quiet_step && GetAsyncKeyState(VK_SHIFT) & 0x8000)
			aim_start(cmd, local_player->get_origin(), min_pos);
	}


}