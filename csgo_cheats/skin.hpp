#pragma once
#include <algorithm>
#include <fstream>
#include <vector>
#include <limits>
#include <map>
#include "config.hpp"
#include "frame_state.hpp"
#include "game_event.hpp"
#include "player_info.hpp"
#include "weapon_item_definition_index.hpp"
#include "weapon_info.hpp"


//����Ƥ�������ռ�
namespace skin_space
{
	//�ж�ͼ���Ƿ���Ҫ�䶯
	static bool hud_update_required = false;

	//��ʼ��ǹе
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

	//��ʼ�����
	void initialize_skin() noexcept
	{
		//��ȡ��Ϸ����Ƥ���ļ�
		std::ifstream items{ "csgo/scripts/items/items_game_cdn.txt" };

		//���ļ����ݴ�ŵ�string
		const std::string gameItems{ std::istreambuf_iterator<char>{ items }, std::istreambuf_iterator<char>{ } };

		//�ر��ļ�
		items.close();

		//��ʼ����
		for (int i = 0; i < g_config.memory.weapen_item_schema()->paintKits.lastElement; i++)
		{
			//��ȡ������Ϣ
			const auto paint_kit = g_config.memory.weapen_item_schema()->paintKits.memory[i].value;
			if (paint_kit->id == 9001) continue;

			//��ȡ����
			const auto item_name{ g_config.localize->find(paint_kit->itemName.buffer + 1) };

			//��ȡ����
			const int item_name_length = WideCharToMultiByte(CP_UTF8, 0, item_name, -1, nullptr, 0, nullptr, nullptr);

			//ת��Ϊstring
			if (std::string name(item_name_length, 0);
				WideCharToMultiByte(CP_UTF8, 0, item_name, -1, &name[0], item_name_length, nullptr, nullptr))
			{
				if (paint_kit->id < 10000)//����Ƥ��
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
					if (weapen_name.size() && weapen_name[0] >= 'a' && weapen_name[0] <= 'z')
						g_config.control.skin_vector.emplace_back(paint_kit->id, std::move(weapen_name));
					else
						g_config.control.skin_vector.emplace_back(paint_kit->id, "null");
				}
			}
		}

		//�������
		std::sort(g_config.control.skin_vector.begin(), g_config.control.skin_vector.end());
	}

	//��ʼ��С��
	void initialize_knife() noexcept
	{
		g_config.control.knife_vector.push_back({ 0, "Default" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_BAYONET, "Bayonet" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_CSS, "Classic Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_SKELETON, "Skeleton Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_OUTDOOR, "Nomad Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_CORD, "Paracord Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_CANIS, "Survival Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_FLIP, "Flip Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_GUT, "Gut Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_KARAMBIT, "Karambit" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_M9_BAYONET, "M9 Bayonet" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_TACTICAL, "Huntsman Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_FALCHION, "Falchion Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_SURVIVAL_BOWIE, "Bowie Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_PUSH, "Shadow Daggers" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_URSUS, "Ursus Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_GYPSY_JACKKNIFE, "Navaja Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_STILETTO, "Stiletto Knife" });
		g_config.control.knife_vector.push_back({ WEAPON_KNIFE_WIDOWMAKER, "Talon Knife" });

	}

	//��ȡ���׺�С����Ϣ
	const weapon_info_struct* get_knife_glove_info(int index = -1) noexcept
	{
		//���׺�С������Ϣ
		const static std::map<int, weapon_info_struct> info =
		{
			{WEAPON_KNIFE,{"models/weapons/v_knife_default_ct.mdl", "knife"}},
			{WEAPON_KNIFE_T,{"models/weapons/v_knife_default_t.mdl", "knife_t"}},
			{WEAPON_KNIFE_BAYONET,{"models/weapons/v_knife_bayonet.mdl", "bayonet"}},
			{WEAPON_KNIFE_CSS,{"models/weapons/v_knife_css.mdl", "knife_css"}},
			{WEAPON_KNIFE_SKELETON,{"models/weapons/v_knife_skeleton.mdl", "knife_skeleton"}},
			{WEAPON_KNIFE_OUTDOOR,{"models/weapons/v_knife_outdoor.mdl", "knife_outdoor"}},
			{WEAPON_KNIFE_CORD,{"models/weapons/v_knife_cord.mdl", "knife_cord"}},
			{WEAPON_KNIFE_CANIS,{"models/weapons/v_knife_canis.mdl", "knife_canis"}},
			{WEAPON_KNIFE_FLIP,{"models/weapons/v_knife_flip.mdl", "knife_flip"}},
			{WEAPON_KNIFE_GUT,{"models/weapons/v_knife_gut.mdl", "knife_gut"}},
			{WEAPON_KNIFE_KARAMBIT, {"models/weapons/v_knife_karam.mdl", "knife_karambit"}},
			{WEAPON_KNIFE_M9_BAYONET, {"models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet"}},
			{WEAPON_KNIFE_TACTICAL, {"models/weapons/v_knife_tactical.mdl", "knife_tactical"}},
			{WEAPON_KNIFE_FALCHION, {"models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion"}},
			{WEAPON_KNIFE_SURVIVAL_BOWIE, {"models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie"}},
			{WEAPON_KNIFE_BUTTERFLY, {"models/weapons/v_knife_butterfly.mdl", "knife_butterfly"}},
			{WEAPON_KNIFE_PUSH, {"models/weapons/v_knife_push.mdl", "knife_push"}},
			{WEAPON_KNIFE_URSUS,{"models/weapons/v_knife_ursus.mdl", "knife_ursus"}},
			{WEAPON_KNIFE_GYPSY_JACKKNIFE,{"models/weapons/v_knife_gypsy_jackknife.mdl", "knife_gypsy_jackknife"}},
			{WEAPON_KNIFE_STILETTO,{"models/weapons/v_knife_stiletto.mdl", "knife_stiletto"}},
			{WEAPON_KNIFE_WIDOWMAKER,{"models/weapons/v_knife_widowmaker.mdl", "knife_widowmaker"}},

			{GLOVE_STUDDED_BLOODHOUND,{"models/weapons/w_models/arms/w_glove_bloodhound.mdl"}},
			{GLOVE_T_SIDE,{"models/weapons/w_models/arms/w_glove_fingerless.mdl"}},
			{GLOVE_CT_SIDE,{"models/weapons/w_models/arms/w_glove_hardknuckle.mdl"}},
			{GLOVE_SPORTY,{"models/weapons/w_models/arms/w_glove_sporty.mdl"}},
			{GLOVE_SLICK,{"models/weapons/w_models/arms/w_glove_slick.mdl"}},
			{GLOVE_LEATHER_WRAP,{"models/weapons/w_models/arms/w_glove_handwrap_leathery.mdl"}},
			{GLOVE_MOTORCYCLE,{"models/weapons/w_models/arms/w_glove_motorcycle.mdl"}},
			{GLOVE_SPECIALIST,{"models/weapons/w_models/arms/w_glove_specialist.mdl"}},
			{GLOVE_HYDRA,{"models/weapons/w_models/arms/w_glove_bloodhound_hydra.mdl"}}
		};

		const auto entry = info.find(index);
		return entry == end(info) ? nullptr : &entry->second;
	}

	//�ж��Ƿ���С��
	bool is_knife(const int i) noexcept
	{
		return (i >= WEAPON_KNIFE_BAYONET && i < GLOVE_STUDDED_BLOODHOUND) || i == WEAPON_KNIFE_T || i == WEAPON_KNIFE;
	}

	//��������Ƥ������
	void update_weapen_data(entity_class* entity) noexcept
	{
		//��ȡ����
		const auto local_index = entity->get_index();

		//�������
		if (!entity->is_alive()) return;

		//��ȡ�����Ϣ
		player_info_struct player_info;
		if (!g_config.engine->get_player_info(local_index, player_info)) return;

		//��ȡ������Ϣ
		auto& weapons_info = entity->get_weapons();

		//����������Ϣ
		for (auto weapon_handle : weapons_info)
		{
			//û������
			if(weapon_handle == -1) break;

			//��ȡ����
			auto weapon = g_config.entity_list->get_entity_from_handle(weapon_handle);
			if (!weapon) continue;

			//ǿ��ʹ��
			weapon->get_item_id_high() = -1;

			//�����û�ID
			weapon->get_account_id() = player_info.xuidLow;

			//��������Ƥ��ID
			weapon->get_fallback_paint_kit() = g_config.control.weapon_skin_id;

			//��ȡС����Ϣ
			if (is_knife(weapon->get_item_definition_index()))
			{
				if (const auto knife_glove_info = get_knife_glove_info(g_config.control.knife_index))
				{
					weapon->get_item_definition_index() = g_config.control.knife_index;
					weapon->set_model_index(g_config.model_info->get_model_index(knife_glove_info->model));
					weapon->pre_data_update(0);
				}
			}
		}

		//��ȡ��ͼģ��
		const auto view_model = g_config.entity_list->get_entity_from_handle(entity->get_view_model());
		if (!view_model) return;

		//��ȡ��ͼ�е�����
		const auto view_model_weapon = g_config.entity_list->get_entity_from_handle(view_model->get_weapon());
		if (!view_model_weapon) return;

		//��ȡС����Ϣ
		const auto override_info = get_knife_glove_info(view_model_weapon->get_item_definition_index());
		if (!override_info) return;

		//��ȡ������ͼ����
		const auto override_model_index = g_config.model_info->get_model_index(override_info->model);
		view_model->get_model_index() = override_model_index;

		//��ȡ����ģ��
		const auto world_model = g_config.entity_list->get_entity_from_handle(view_model_weapon->get_weapon_world_model());
		if (!world_model) return;

		//��������ģ��
		world_model->get_model_index() = override_model_index + 1;
	}

	//����ͼ��
	void update_hud() noexcept
	{
		if (auto hud_weapons = g_config.memory.find_hud_element(g_config.memory.hud, "CCSGO_HudWeaponSelection") - 0x28) 
		{
			for (int i = 0; i < *(hud_weapons + 0x20); i++)
				i = g_config.memory.clear_hud_weapon(hud_weapons, i);
		}

		//ͼ��������
		hud_update_required = false;
	}

	//�л�����Ƥ��
	void run(frame_stage_enum state) noexcept
	{
		//û��
		if (!g_config.control.skin_enable) return;

		//��������Ƥ��֡
		if (state == frame_stage_enum::NET_UPDATE_POSTDATAUPDATE_START)
		{
			//��ȡ�Լ���Ϣ
			if (const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player()))
			{
				//��������Ƥ������
				update_weapen_data(local_player);

				//����ͼ��
				if (hud_update_required && !local_player->is_dormant()) update_hud();
			}
		}
	}

	//����hud����
	void schedule_hud_update() noexcept
	{
		//���ù���Ƥ���ص�����
		g_config.cvar->findVar("cl_fullupdate")->changeCallback();

		//��Ҫ����ͼ��
		hud_update_required = true;
	}

}
