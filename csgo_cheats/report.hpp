#pragma once
#include "config.hpp"

//举报命名空间
namespace report_space
{
	//举报列表
	static std::vector<uint64_t> report_players_list;

	//清空举报玩家列表
	void clear_report_list() noexcept
	{
		report_players_list.clear();
	}

	//获取当前房间玩家信息
	void get_room_players_info() noexcept
	{
		//获取本地玩家
		const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player());
		if (!local_player) return;

		//清空原本信息
		g_config.control.report_players.clear();

		//遍历玩家
		for (int i = 1; i < g_config.engine->get_max_clients(); i++)
		{
			//获取玩家
			auto entity = g_config.entity_list->get_entity(i);
			if (!entity || entity == local_player) continue;

			//获取玩家信息
			player_info_struct player_info;
			if (!g_config.engine->get_player_info(i, player_info) || player_info.fakeplayer) continue;

			//加入列表
			g_config.control.report_players.push_back(std::move(player_info));
		}
	}

	//举报单个玩家
	void report_player(const std::string& report_text, uint64_t player_xuid) noexcept
	{
		//举报玩家
		g_config.memory.submit_report(std::to_string(player_xuid).c_str(), report_text.c_str());
	}

	//举报全部玩家
	void report_players(const std::string& report_text) noexcept
	{
		for (const auto& it : g_config.control.report_players)
		{
			//如果举报过了的
			if (std::find(report_players_list.cbegin(), report_players_list.cend(), it.xuid) != report_players_list.cend()) continue;

			//举报玩家
			g_config.memory.submit_report(std::to_string(it.xuid).c_str(), report_text.c_str());

			//加入列表
			report_players_list.push_back(it.xuid);

			//防止举报过多
			break;
		}
	}

	//开始举报
	void run() noexcept
	{
		//没开
		if (!g_config.control.report_enable) return;

		//获取所有玩家
		get_room_players_info();

		//上一次举报时间,防止举报太多而卡出房间
		static float last_report_time = 0.0f;
		if (last_report_time  + g_config.control.report_interval > g_config.global_vars->real_time) return;

		//举报信息
		std::string report_info;
		if (g_config.control.report_text_abuse) report_info += "textabuse,";
		if (g_config.control.report_grief) report_info += "grief,";
		if (g_config.control.report_wall_hack) report_info += "wallhack,";
		if (g_config.control.report_aim_bot) report_info += "aimbot,";
		if (g_config.control.report_speed_hack) report_info += "speedhack,";
		if (report_info.empty()) return;

		//开始举报
		if (g_config.control.report_mode == 1) report_players(report_info);
		else report_player(report_info, g_config.control.report_player_xuid);

		//更新时间
		last_report_time = g_config.global_vars->real_time;
	}


}