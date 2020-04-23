#pragma once
#include "config.hpp"
#include "user_cmd.hpp"

//自瞄命名空间
namespace aim_space
{
	/*
	////计算相对角度
	//self_vector_struct calculate_relative_angle(const self_vector_struct& source, const self_vector_struct& destination, const self_vector_struct& viewAngles) noexcept
	//{
	//	constexpr auto radiansToDegrees = [](float radians) constexpr noexcept { return radians * 180.0f / static_cast<float>(3.14159265358979323846); };
	//	self_vector_struct delta = destination - source;
	//	self_vector_struct angles{ radiansToDegrees(atan2f(-delta.z, std::hypotf(delta.x, delta.y))) - viewAngles.x, radiansToDegrees(atan2f(delta.y, delta.x)) - viewAngles.y };
	//	angles.normalize();
	//	return angles;
	//}
	////开始自瞄
	//void run(user_cmd_struct* cmd)
	//{
	//	//获取自身信息
	//	const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player());
	//	if (!local_player || local_player->get_next_attack() > g_config.global_vars->serverTime()) return;

	//	//获取当前武器信息
	//	const auto active_weapon = local_player->get_active_weapon();
	//	if (!active_weapon || !active_weapon->is_clip()) return;

	//	if (g_config.control.aim_enable //开启了自瞄
	//		&& (cmd->buttons & user_cmd_struct::IN_ATTACK//有射击命令
	//		|| g_config.control.aim_auto_shot //自动开枪
	//		|| g_config.control.aim_aimlock) //开启了一直锁定敌人
	//		&& active_weapon->get_inaccuracy() <= g_config.control.aim_max_aim_inaccuracy)//当前枪械的不正确率在允许范围内
	//	{
	//		//获取允许自瞄范围
	//		auto best_fov = g_config.control.aim_fov;

	//		//最好瞄准的敌人位置
	//		self_vector_struct best_target{};

	//		//获取我们的视线范围
	//		auto local_layer_eye_position = local_player->get_eye_position();

	//		//获取自瞄punch
	//		auto aim_punch = local_player->get_aim_punch();

	//		//遍历每一个玩家
	//		for (int i = 1; i <= g_config.engine->get_max_clients(); i++)
	//		{
	//			//获取玩家信息
	//			auto entity = g_config.entity_list->get_entity(i);
	//			if (!entity || entity == local_player || entity->is_dormant() || !entity->is_alive() || !entity->is_enemy() || entity->is_gun_game_immunity()) continue;

	//			//获取头部骨骼的位置
	//			auto head_position = entity->get_bone_position(g_config.control.aim_bone);

	//			//如果不可见
	//			if(!entity->is_visiable(head_position)) continue;

	//			//计算相对角度
	//			auto angle = calculate_relative_angle(local_layer_eye_position, head_position, cmd->view_angles);

	//			//计算视角
	//			auto fov = std::hypotf(angle.x, angle.y);

	//			//如果在范围内
	//			if (fov < best_fov)
	//			{
	//				best_fov = fov;
	//				best_target = head_position;
	//				break;
	//			}
	//		}

	//		//有目标了
	//		if (best_target)
	//		{
	//			//计算角度
	//			auto angle = calculate_relative_angle(local_player->get_eye_position(), best_target, cmd->view_angles + aim_punch);

	//			//
	//			bool clamped = false;
	//			if (fabs(angle.x) > 255.0f || fabs(angle.y) > 255.0f)
	//			{
	//				angle.x = std::clamp(angle.x, -255.0f, 255.0f);
	//				angle.y = std::clamp(angle.y, -255.0f, 255.0f);
	//				clamped = true;
	//			}

	//			//设置角度
	//			cmd->view_angles += angle;
	//			g_config.engine->set_view_angles(cmd->view_angles);

	//			//自瞄开镜
	//			if (g_config.control.aim_auto_scope && active_weapon->get_next_primary_attack() <= g_config.global_vars->serverTime() && active_weapon->is_sniper_rifle() && !local_player->get_is_scoped())
	//				cmd->buttons |= user_cmd_struct::IN_ATTACK2;

	//			//自动开枪
	//			if (g_config.control.aim_auto_shot && active_weapon->get_next_primary_attack() <= g_config.global_vars->serverTime() && !clamped && active_weapon->get_inaccuracy() <= g_config.control.aim_max_shot_inaccuracy)
	//				cmd->buttons |= user_cmd_struct::IN_ATTACK;

	//			//取消开枪
	//			if (clamped) cmd->buttons &= ~user_cmd_struct::IN_ATTACK;
	//		}
	//	}
	//}
	*/

	//自瞄角度
	self_vector_struct aim_angle(user_cmd_struct* cmd, self_vector_struct& local_pos,self_vector_struct& enemy_pos)
	{
		float x = local_pos.x - enemy_pos.x;
		float y = local_pos.y - enemy_pos.y;
		float z = local_pos.z - enemy_pos.z + 60.0f + 5.0f + g_config.control.aim_offset;
		if (cmd->buttons & user_cmd_struct::IN_DUCK) z -= 15.0f;

		static const float pi = 3.1415f;
		self_vector_struct angle{ atan(z / sqrt(x * x + y * y)) / pi * 180.f,atan(y / x) ,0 };

		if (x >= 0.0f && y >= 0.0f) angle.y = angle.y / pi * 180.0f - 180.0f;
		else if (x < 0.0f && y >= 0.0f) angle.y = angle.y / pi * 180.0f;
		else if (x < 0.0f && y < 0.0f) angle.y = angle.y / pi * 180.0f;
		else if (x >= 0.0f && y < 0.0f) angle.y = angle.y / pi * 180.f + 180.0f;

		return angle;
	}

	//计算自瞄距离
	float aim_distance(self_vector_struct& enemy_pos)
	{
		self_vector_struct origin_pos;
		g_config.engine->get_view_angles(origin_pos);

		float width = abs(origin_pos.x - enemy_pos.x);
		float height = abs(origin_pos.y - enemy_pos.y);

		return std::hypot(width, height);
	}

	//开启自瞄
	void run(user_cmd_struct* cmd) noexcept
	{
		//没有开启自瞄
		if (!g_config.control.aim_enable) return;

		if (cmd->buttons & user_cmd_struct::IN_ATTACK//开枪状态
			|| g_config.control.aim_auto_shot//自动射击
			|| g_config.control.aim_auto_scope//自动开镜
			|| g_config.control.aim_aimlock)//敌人锁定
		{
			//获取自己
			const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player());
			if (!local_player || !local_player->is_alive() || local_player->get_next_attack() > g_config.global_vars->serverTime()) return;

			//获取武器
			const auto active_weapon = local_player->get_active_weapon();
			if (!active_weapon || !active_weapon->is_clip()) return;

			//武器的不准确率超过了我们允许的范围
			if (active_weapon->get_inaccuracy() > g_config.control.aim_max_aim_inaccuracy) return;

			//最近玩家位置
			float distance = g_config.control.aim_fov;
			self_vector_struct angle{};

			//遍历玩家
			for (int i = 1; i <= g_config.engine->get_max_clients(); i++)
			{
				//获取玩家对象
				auto entity = g_config.entity_list->get_entity(i);
				if (!entity || entity == local_player || !entity->is_alive() || !entity->is_enemy() || entity->is_dormant() || entity->is_gun_game_immunity()) continue;

				//获取敌人位置
				int bone = g_config.control.aim_bone;

				//敌人身体部位
				std::vector<int> bones = (bone == -1) ? std::vector<int>{ 8, 7, 6, 5, 4, 3 } : std::vector<int>{ bone };

				//判断哪一个部位最近
				for (const auto& it : bones)
				{
					//获取该部位
					self_vector_struct enemy_pos = entity->get_bone_position(it);

					//如果玩家被墙体挡住了
					if (!entity->is_visiable(enemy_pos)) continue;

					//获取自瞄角度
					self_vector_struct temp_angle = aim_angle(cmd, local_player->get_origin(), enemy_pos);
					float temp_distance = aim_distance(temp_angle);
					if (temp_distance < distance)
					{
						distance = temp_distance;
						angle = temp_angle;
					}
				}
			}

			//可以自瞄
			if (angle)
			{
				//设置自瞄角度
				cmd->view_angles = angle;
				g_config.engine->set_view_angles(cmd->view_angles);

				//自动开镜
				if (g_config.control.aim_auto_scope && active_weapon->get_next_primary_attack() <= g_config.global_vars->serverTime() && active_weapon->is_sniper_rifle() && !local_player->get_is_scoped())
					cmd->buttons |= user_cmd_struct::IN_ATTACK2;

				//自动开枪
				if (g_config.control.aim_auto_shot && active_weapon->get_next_primary_attack() <= g_config.global_vars->serverTime() && active_weapon->get_inaccuracy() <= g_config.control.aim_max_shot_inaccuracy)
					cmd->buttons |= user_cmd_struct::IN_ATTACK;

			}
		}
	}
}