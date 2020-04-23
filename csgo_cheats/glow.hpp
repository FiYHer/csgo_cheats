#pragma once
#include "config.hpp"

#include <vector>

//辉光对象命名空间
namespace glow_space
{
	//自定义辉光信息
	std::vector<std::pair<int, int>> custom_glow_entities;

	//清空自定义辉光对象
	void clear_custom_objects() noexcept
	{
		for (const auto& [entity_index, glow_object_index] : custom_glow_entities)
			g_config.memory.glow_object_mamager->un_register_glow_object(glow_object_index);
		custom_glow_entities.clear();
	}

	//进行辉光操作
	void render() noexcept
	{
		if (!g_config.control.glow_enable) return;

		//获取自身信息
		const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player());
		if (!local_player) return;

		//清空所有自定义辉光对象
		clear_custom_objects();

		//对辉光对象进行操作
		for (int i = 0; i < g_config.memory.glow_object_mamager->glow_object_definitions.size; i++)
		{
			//获取辉光对象信息
			glow_object_definition_struct& glow_object = g_config.memory.glow_object_mamager->glow_object_definitions[i];

			//辉光对象
			auto set_glow_object = [&](self_vector_struct color, int is_fake)
			{
				glow_object.renderWhenOccluded = true;//设置为看不见玩家也辉光
				glow_object.alpha = is_fake ? 0.8f : 1.0f;//机器人和真人的透明度不同
				glow_object.glowStyle = 0;//默认类型
				glow_object.bloomAmount = 1.0f;//机器人和真人的厚度不同
				glow_object.glowColor = color;//颜色
			};

			//获取该辉光对象对应的实例
			auto entity = glow_object.entity;
			if(glow_object.is_unused() || !entity || entity->is_dormant()) continue;

			//如果不是玩家
			if (entity->get_client_class()->classId != classId_enum::CSPlayer) continue;

			//获取玩家信息
			player_info_struct player_info;
			if (!g_config.engine->get_player_info(entity->get_index(), player_info)) continue;

			//真实玩家和电脑玩家的辉光颜色有一点差别
			if (g_config.control.glow_friend && !entity->is_enemy()) set_glow_object({ 1.0f,0.0f,1.0f }, player_info.fakeplayer);
			else if (g_config.control.glow_enemy && entity->is_enemy())
			{
				int health = entity->get_health();//获取血量
				set_glow_object({ 1.0f - health / 100.0f,  health / 100.0f, 0.0f }, player_info.fakeplayer);
			}
		}
	}
}