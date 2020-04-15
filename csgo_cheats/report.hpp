#pragma once
#include "config.hpp"

//�ٱ������ռ�
namespace report_space
{
	//�ٱ��б�
	static std::vector<uint64_t> report_players_list;

	//��վٱ�����б�
	void clear_report_list() noexcept
	{
		report_players_list.clear();
	}

	//��ȡ��ǰ���������Ϣ
	void get_room_players_info() noexcept
	{
		//��ȡ�������
		const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player());
		if (!local_player) return;

		//���ԭ����Ϣ
		g_config.control.report_players.clear();

		//�������
		for (int i = 1; i < g_config.engine->get_max_clients(); i++)
		{
			//��ȡ���
			auto entity = g_config.entity_list->get_entity(i);
			if (!entity || entity == local_player) continue;

			//��ȡ�����Ϣ
			player_info_struct player_info;
			if (!g_config.engine->get_player_info(i, player_info) || player_info.fakeplayer) continue;

			//�����б�
			g_config.control.report_players.push_back(std::move(player_info));
		}
	}

	//�ٱ��������
	void report_player(const std::string& report_text, uint64_t player_xuid) noexcept
	{
		//�ٱ����
		g_config.memory.submit_report(std::to_string(player_xuid).c_str(), report_text.c_str());
	}

	//�ٱ�ȫ�����
	void report_players(const std::string& report_text) noexcept
	{
		for (const auto& it : g_config.control.report_players)
		{
			//����ٱ����˵�
			if (std::find(report_players_list.cbegin(), report_players_list.cend(), it.xuid) != report_players_list.cend()) continue;

			//�ٱ����
			g_config.memory.submit_report(std::to_string(it.xuid).c_str(), report_text.c_str());

			//�����б�
			report_players_list.push_back(it.xuid);

			//��ֹ�ٱ�����
			break;
		}
	}

	//��ʼ�ٱ�
	void run() noexcept
	{
		//û��
		if (!g_config.control.report_enable) return;

		//��ȡ�������
		get_room_players_info();

		//��һ�ξٱ�ʱ��,��ֹ�ٱ�̫�����������
		static float last_report_time = 0.0f;
		if (last_report_time  + g_config.control.report_interval > g_config.global_vars->real_time) return;

		//�ٱ���Ϣ
		std::string report_info;
		if (g_config.control.report_text_abuse) report_info += "textabuse,";
		if (g_config.control.report_grief) report_info += "grief,";
		if (g_config.control.report_wall_hack) report_info += "wallhack,";
		if (g_config.control.report_aim_bot) report_info += "aimbot,";
		if (g_config.control.report_speed_hack) report_info += "speedhack,";
		if (report_info.empty()) return;

		//��ʼ�ٱ�
		if (g_config.control.report_mode == 1) report_players(report_info);
		else report_player(report_info, g_config.control.report_player_xuid);

		//����ʱ��
		last_report_time = g_config.global_vars->real_time;
	}


}