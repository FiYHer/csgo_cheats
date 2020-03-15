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
		//没有启用人物辉光
		if (!g_config.control.glow) return;

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

			//获取该辉光对象对应的实例
			auto entity = glow_object.entity;
			if(glow_object.is_unused() || !entity || entity->is_dormant()) continue;

			//如果我玩家
			if (entity->get_client_class()->classId == classId_enum::CSPlayer)
			{
				glow_object.renderWhenOccluded = true;//设置为看不见玩家也辉光
				glow_object.alpha = 1.0f;//一点也不透明
				glow_object.glowStyle = 0;//默认类型
				glow_object.bloomAmount = 1.0f;//最厚
				int health = local_player->get_health();//获取自身血量
				glow_object.glowColor = { 1.0f - health / 100.0f,  health / 100.0f, 0.0f };//设置亮光
			}
		}
	}
}