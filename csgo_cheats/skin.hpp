#pragma once
#include <algorithm>
#include <fstream>
#include <vector>
#include "config.hpp"
#include "frame_state.hpp"
#include "game_event.hpp"
#include "player_info.hpp"
#include "weapon_item_definition_index.hpp"
#include <limits>



//武器皮肤命名空间
namespace skin_space
{
	//判断图标是否需要变动
	static bool hud_update_required = false;

	//初始化枪械
	void initialize_weapon() noexcept
	{
		g_config.control.weapon_map.insert({ 42,"Knife" });
		g_config.control.weapon_map.insert({ 5028,"Glove" });
		g_config.control.weapon_map.insert({ 7, "AK-47" });
		g_config.control.weapon_map.insert({ 8, "AUG" });
		g_config.control.weapon_map.insert({ 9, "AWP" });
		g_config.control.weapon_map.insert({ 63, "CZ75 Auto" });
		g_config.control.weapon_map.insert({ 1, "Desert Eagle" });
		g_config.control.weapon_map.insert({ 2, "Dual Berettas" });
		g_config.control.weapon_map.insert({ 10, "FAMAS" });
		g_config.control.weapon_map.insert({ 3, "Five-SeveN" });
		g_config.control.weapon_map.insert({ 11, "G3SG1" });
		g_config.control.weapon_map.insert({ 13, "Galil AR" });
		g_config.control.weapon_map.insert({ 4, "Glock-18" });
		g_config.control.weapon_map.insert({ 14, "M249" });
		g_config.control.weapon_map.insert({ 60, "M4A1-S" });
		g_config.control.weapon_map.insert({ 16, "M4A4" });
		g_config.control.weapon_map.insert({ 17, "MAC-10" });
		g_config.control.weapon_map.insert({ 27, "MAG-7" });
		g_config.control.weapon_map.insert({ 23, "MP5-SD" });
		g_config.control.weapon_map.insert({ 33, "MP7" });
		g_config.control.weapon_map.insert({ 34, "MP9" });
		g_config.control.weapon_map.insert({ 28, "Negev" });
		g_config.control.weapon_map.insert({ 35, "Nova" });
		g_config.control.weapon_map.insert({ 32, "P2000" });
		g_config.control.weapon_map.insert({ 36, "P250" });
		g_config.control.weapon_map.insert({ 19, "P90" });
		g_config.control.weapon_map.insert({ 26, "PP-Bizon" });
		g_config.control.weapon_map.insert({ 64, "R8 Revolver" });
		g_config.control.weapon_map.insert({ 29, "Sawed-Off" });
		g_config.control.weapon_map.insert({ 38, "SCAR-20" });
		g_config.control.weapon_map.insert({ 40, "SSG 08" });
		g_config.control.weapon_map.insert({ 39, "SG 553" });
		g_config.control.weapon_map.insert({ 30, "Tec-9" });
		g_config.control.weapon_map.insert({ 24, "UMP-45" });
		g_config.control.weapon_map.insert({ 61, "USP-S" });
		g_config.control.weapon_map.insert({ 25, "XM1014" });
	}

	//初始化相关
	void initialize_skin() noexcept
	{
		//读取游戏武器皮肤文件
		std::ifstream items{ "csgo/scripts/items/items_game_cdn.txt" };

		//将文件数据存放到string
		const std::string gameItems{ std::istreambuf_iterator<char>{ items }, std::istreambuf_iterator<char>{ } };

		//关闭文件
		items.close();

		//开始遍历
		for (int i = 0; i < g_config.memory.weapen_item_schema()->paintKits.lastElement; i++)
		{
			//获取武器信息
			const auto paint_kit = g_config.memory.weapen_item_schema()->paintKits.memory[i].value;
			if (paint_kit->id == 9001) continue;

			//获取名称
			const auto item_name{ g_config.localize->find(paint_kit->itemName.buffer + 1) };

			//获取长度
			const int item_name_length = WideCharToMultiByte(CP_UTF8, 0, item_name, -1, nullptr, 0, nullptr, nullptr);

			//转化为string
			if (std::string name(item_name_length, 0);
				WideCharToMultiByte(CP_UTF8, 0, item_name, -1, &name[0], item_name_length, nullptr, nullptr))
			{
				if (paint_kit->id < 10000)//武器皮肤
				{
					std::string weapen_name;
					if (auto pos = gameItems.find('_' + std::string{ paint_kit->name.buffer } +'=');
						pos != std::string::npos
						&& gameItems.substr(pos + paint_kit->name.length).find('_' + std::string{ paint_kit->name.buffer } +'=') == std::string::npos)
					{
						if (auto weaponName = gameItems.rfind("weapon_", pos); weaponName != std::string::npos)
						{
							name.back() = ' ';
							weapen_name = gameItems.substr(weaponName + 7, pos - weaponName - 7);
							weapen_name += "-" + name;
						}
					}
					if(weapen_name.size() && weapen_name[0] >= 'a')
						g_config.control.skin_vector.emplace_back(paint_kit->id, std::move(weapen_name));
				}
			}
		}

		//排序分类
		std::sort(g_config.control.skin_vector.begin(), g_config.control.skin_vector.end());
	}

	//更新武器皮肤数据
	void update_weapen_data(entity_class* entity) noexcept
	{
		//获取索引
		const auto local_index = entity->get_index();

		//玩家死亡
		if (!entity->is_alive()) return;

		//获取玩家信息
		player_info_struct player_info;
		if (!g_config.engine->get_player_info(local_index, player_info)) return;

		//获取武器信息
		auto& weapons_info = entity->get_weapons();

		//遍历武器信息
		for (auto weapon_handle : weapons_info)
		{
			//没有武器
			if(weapon_handle == -1) break;

			//获取武器
			auto weapon = g_config.entity_list->get_entity_from_handle(weapon_handle);
			if (!weapon) continue;

			//获取武器定义索引，AK-47就是7
			auto& weapon_index = weapon->get_item_definition_index();

			//保存武器ID
			g_config.control.weapon_id = weapon_index;

			//强制使用
			weapon->get_item_id_high() = -1;

			//设置用户ID
			weapon->get_account_id() = player_info.xuidLow;

			//设置武器质量
			weapon->get_entity_quality() = 0;

			//设置武器皮肤ID
			weapon->get_fallback_paint_kit() = g_config.control.weapon_skin_id;

			//设置武器sedd
			weapon->get_fallback_seed() = 0;

			//设置武器的
			weapon->get_fallback_stat_trak() = 0;

			//设置武器的
			weapon->get_fallback_wear() = 0;
		}
	}

	//更新图标
	void update_hud() noexcept
	{
		if (auto hud_weapons = g_config.memory.find_hud_element(g_config.memory.hud, "CCSGO_HudWeaponSelection") - 0x28) 
		{
			for (int i = 0; i < *(hud_weapons + 0x20); i++)
				i = g_config.memory.clear_hud_weapon(hud_weapons, i);
		}

		//图标更新完成
		hud_update_required = false;
	}

	//切换武器皮肤
	void run(frame_stage_enum state) noexcept
	{
		//没开
		if (!g_config.control.skin) return;

		//更新武器皮肤帧
		if (state == frame_stage_enum::NET_UPDATE_POSTDATAUPDATE_START)
		{
			//获取自己信息
			if (const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player()))
			{
				//更新武器皮肤数据
				update_weapen_data(local_player);

				//更新图标
				if (hud_update_required && !local_player->is_dormant()) update_hud();
			}
		}
	}

	//安排hud更新
	void schedule_hud_update() noexcept
	{
		//调用骨骼皮肤回调函数
		g_config.cvar->findVar("cl_fullupdate")->changeCallback();

		//需要更新图标
		hud_update_required = true;
	}

	//覆盖hud图标
	void override_hud_icon(game_event_class& event) noexcept
	{	
		if (g_config.engine->get_player_for_user_id(event.get_int("attacker")) == g_config.engine->get_local_player())
		{
			/*if (const auto iconOverride = iconOverrides[event.get_string("weapon")])
				event.set_string("weapon", iconOverride);*/
		}
	}

}
