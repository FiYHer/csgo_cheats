#pragma once
#include "config.hpp"
#include "user_cmd.hpp"

//���������ռ�
namespace aim_space
{
	//������ԽǶ�
	self_vector_struct calculate_relative_angle(const self_vector_struct& source, const self_vector_struct& destination, const self_vector_struct& viewAngles) noexcept
	{
		constexpr auto radiansToDegrees = [](float radians) constexpr noexcept { return radians * 180.0f / static_cast<float>(3.14159265358979323846); };
		self_vector_struct delta = destination - source;
		self_vector_struct angles{ radiansToDegrees(atan2f(-delta.z, std::hypotf(delta.x, delta.y))) - viewAngles.x, radiansToDegrees(atan2f(delta.y, delta.x)) - viewAngles.y };
		angles.normalize();
		return angles;
	}

	//��ʼ����
	void run(user_cmd_struct* cmd)
	{
		//��ȡ������Ϣ
		const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player());
		if (!local_player || local_player->get_next_attack() > g_config.global_vars->serverTime()) return;

		//��ȡ��ǰ������Ϣ
		const auto active_weapon = local_player->get_active_weapon();
		if (!active_weapon || !active_weapon->is_clip()) return;

		if (g_config.control.aim_enable //����������
			&& (cmd->buttons & user_cmd_struct::IN_ATTACK//���������
			|| g_config.control.aim_auto_shot //�Զ���ǹ
			|| g_config.control.aim_aimlock) //������һֱ��������
			&& active_weapon->get_inaccuracy() <= g_config.control.aim_max_aim_inaccuracy)//��ǰǹе�Ĳ���ȷ��������Χ��
		{
			//��ȡ�������鷶Χ
			auto best_fov = g_config.control.aim_fov;

			//�����׼�ĵ���λ��
			self_vector_struct best_target{};

			//��ȡ���ǵ����߷�Χ
			auto local_layer_eye_position = local_player->get_eye_position();

			//��ȡ����punch
			auto aim_punch = local_player->get_aim_punch();

			//����ÿһ�����
			for (int i = 1; i <= g_config.engine->get_max_clients(); i++)
			{
				//��ȡ�����Ϣ
				auto entity = g_config.entity_list->get_entity(i);
				if (!entity || entity == local_player || entity->is_dormant() || !entity->is_alive() || !entity->is_enemy() || entity->is_gun_game_immunity()) continue;

				//��ȡͷ��������λ��
				auto head_position = entity->get_bone_position(g_config.control.aim_bone);

				//������ɼ�
				if(!entity->is_visiable(head_position)) continue;

				//������ԽǶ�
				auto angle = calculate_relative_angle(local_layer_eye_position, head_position, cmd->view_angles);

				//�����ӽ�
				auto fov = std::hypotf(angle.x, angle.y);

				//����ڷ�Χ��
				if (fov < best_fov)
				{
					best_fov = fov;
					best_target = head_position;
					break;
				}
			}

			//��Ŀ����
			if (best_target)
			{
				//����Ƕ�
				auto angle = calculate_relative_angle(local_player->get_eye_position(), best_target, cmd->view_angles + aim_punch);

				//
				bool clamped = false;
				if (fabs(angle.x) > 255.0f || fabs(angle.y) > 255.0f)
				{
					angle.x = std::clamp(angle.x, -255.0f, 255.0f);
					angle.y = std::clamp(angle.y, -255.0f, 255.0f);
					clamped = true;
				}

				//���ýǶ�
				cmd->view_angles += angle;
				g_config.engine->set_view_angles(cmd->view_angles);

				//���鿪��
				if (g_config.control.aim_auto_scope && active_weapon->get_next_primary_attack() <= g_config.global_vars->serverTime() && active_weapon->is_sniper_rifle() && !local_player->get_is_scoped())
					cmd->buttons |= user_cmd_struct::IN_ATTACK2;

				//�Զ���ǹ
				if (g_config.control.aim_auto_shot && active_weapon->get_next_primary_attack() <= g_config.global_vars->serverTime() && !clamped && active_weapon->get_inaccuracy() <= g_config.control.aim_max_shot_inaccuracy)
					cmd->buttons |= user_cmd_struct::IN_ATTACK;

				//ȡ����ǹ
				if (clamped) cmd->buttons &= ~user_cmd_struct::IN_ATTACK;
			}
		}
	}
}