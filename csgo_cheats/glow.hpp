#pragma once
#include "config.hpp"

#include <vector>

//�Թ���������ռ�
namespace glow_space
{
	//�Զ���Թ���Ϣ
	std::vector<std::pair<int, int>> custom_glow_entities;

	//����Զ���Թ����
	void clear_custom_objects() noexcept
	{
		for (const auto& [entity_index, glow_object_index] : custom_glow_entities)
			g_config.memory.glow_object_mamager->un_register_glow_object(glow_object_index);
		custom_glow_entities.clear();
	}

	//���лԹ����
	void render() noexcept
	{
		if (!g_config.control.glow_enable) return;

		//��ȡ������Ϣ
		const auto local_player = g_config.entity_list->get_entity(g_config.engine->get_local_player());
		if (!local_player) return;

		//��������Զ���Թ����
		clear_custom_objects();

		//�ԻԹ������в���
		for (int i = 0; i < g_config.memory.glow_object_mamager->glow_object_definitions.size; i++)
		{
			//��ȡ�Թ������Ϣ
			glow_object_definition_struct& glow_object = g_config.memory.glow_object_mamager->glow_object_definitions[i];

			//�Թ����
			auto set_glow_object = [&](self_vector_struct color, int is_fake)
			{
				glow_object.renderWhenOccluded = true;//����Ϊ���������Ҳ�Թ�
				glow_object.alpha = is_fake ? 0.8f : 1.0f;//�����˺����˵�͸���Ȳ�ͬ
				glow_object.glowStyle = 0;//Ĭ������
				glow_object.bloomAmount = 1.0f;//�����˺����˵ĺ�Ȳ�ͬ
				glow_object.glowColor = color;//��ɫ
			};

			//��ȡ�ûԹ�����Ӧ��ʵ��
			auto entity = glow_object.entity;
			if(glow_object.is_unused() || !entity || entity->is_dormant()) continue;

			//����������
			if (entity->get_client_class()->classId != classId_enum::CSPlayer) continue;

			//��ȡ�����Ϣ
			player_info_struct player_info;
			if (!g_config.engine->get_player_info(entity->get_index(), player_info)) continue;

			//��ʵ��Һ͵�����ҵĻԹ���ɫ��һ����
			if (g_config.control.glow_friend && !entity->is_enemy()) set_glow_object({ 1.0f,0.0f,1.0f }, player_info.fakeplayer);
			else if (g_config.control.glow_enemy && entity->is_enemy())
			{
				int health = entity->get_health();//��ȡѪ��
				set_glow_object({ 1.0f - health / 100.0f,  health / 100.0f, 0.0f }, player_info.fakeplayer);
			}
		}
	}
}