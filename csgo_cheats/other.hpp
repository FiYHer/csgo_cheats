#pragma once
#include "config.hpp"

//����һЩ���õĹ���
namespace other_space
{
	//��������
	void again_jump(user_cmd_struct* cmd) noexcept
	{
		if (!g_config.control.other_again_jump) return;

		//��ȡ�Լ�ʵ��
		const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player());
		if (!local_player || !local_player->is_alive()) return;

		static int last_time_state = local_player->get_flags() & 1;

		if (!(local_player->get_flags() & 1)
			&& local_player->get_move_type() != move_type_enum::LADDER
			&& !last_time_state) cmd->buttons &= ~user_cmd_struct::IN_JUMP;

		last_time_state = local_player->get_flags() & 1;
	}



}