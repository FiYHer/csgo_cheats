#pragma once
#include "config.hpp"
#include "user_cmd.hpp"

//���������ռ�
namespace aim_space
{
	//��������
	void run(user_cmd_struct* cmd) noexcept
	{
		//û�п�������
		if (!g_config.control.aim) return;

		//��ȡ�Լ�
		const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player());
		if (!local_player || !local_player->is_alive()) return;

		//��ȡ���������Ϣ
		float self_matrix[4][4];
		g_config.engine->world_to_screen_matrix();

		//�������
		for (int i = 1; i < g_config.engine->get_max_clients(); i++)
		{
			//��ȡ��Ҷ���
			auto entity = g_config.entity_list->get_entity(i);
			if(!entity || entity == local_player) continue;

			//������������
			if (!entity->is_alive()) return;

			//�������Ƕ���
			if (!entity->is_enemy()) return;





		}
	}


}